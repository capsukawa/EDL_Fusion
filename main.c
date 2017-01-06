#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options.h"
#include "header.h"
#include "sectionTable.h"
#include "sectionNames.h"
#include "afficheHeader.h"
#include "contenuSection.h"
#include "afficheSectionTable.h"
#include "tableSymbole.h"
#include "fusionSection.h"

int main(int argc, char* argv[]) {
	int option = recupererOption(argv[1]);
	if (argc<3 || option==OPT_H)
		printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");

	else if (argc!=4 && option==OPT_S)
		printf("Usage : \"./edl_fusion -s [nom_fichier] [numero_section]\"\n");

	else if (option!=NO_OPT){
		Elf32_Ehdr* header;
		FILE *f;
		FILE *f2;
		f = fopen(argv[2],"r");
		if (f==NULL) printf("Echec d'ouverture du fichier\n");
		else {
			header = initHeader(f);
			Elf32_Shdr* sectionTable[header->e_shnum];
			char* sectionNames[header->e_shnum];
			readSection(sectionTable, header, f);
			readNames(sectionNames, header, f, sectionTable);
			int i;

	/*		// Creation de sectionName
			char* sectionNames[header->e_shnum];
			int i;
			// Remplissage de sectionNames
			for(i=0; i<header->e_shnum;i++) {
				sectionNames[i]=malloc(sizeof(char)*20);
				fseek(f, sectionTable[header->e_shstrndx]->sh_offset + sectionTable[i]->sh_name, SEEK_SET);
				char c; int cmt = 0;
				fread(&c,1,1,f);
				while(c != 0x00) {
					sectionNames[i][cmt] = c;
					cmt++;
					fread(&c,1,1,f);
				}
			} */

			// Creation de tabNomSym
			i = rechercheSectionHeader(".symtab",header,sectionNames);
			int nbNom = (sectionTable[i]->sh_size)/16;
			char *tabNomSym[nbNom];

			switch(option) {
				case OPT_E:
					afficheHeader(header);
					break;
				case OPT_S:
					affichageSection(header,f,argv[3],sectionNames);
					break;
				case OPT_SH:
					afficheSectionTable(sectionTable, header, sectionNames);
					break;
				case OPT_TS:
					afficherTableSymbole(header,f,sectionTable,sectionNames,tabNomSym);
					break;
				case OPT_F:
					f2 = fopen(argv[3],"r");
					Elf32_Ehdr* header2 = initHeader(f2);
					Elf32_Shdr* sectionTable2[header->e_shnum];
					char* sectionNames2[header->e_shnum];
					readSection(sectionTable2, header2, f2);
					readNames(sectionNames2, header2, f2, sectionTable2);
					fusionSection(header,header2, f, f2, sectionNames, sectionNames2, sectionTable, sectionTable2);
					break;
			}
			fclose(f);
		}
	}
	else printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
	return 0;
}
