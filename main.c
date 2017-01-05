#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options.h"
#include "header.h"
#include "sectionTable.h"
#include "afficheHeader.h"
#include "contenuSection.h"
#include "afficheSectionTable.h"
#include "tableSymbole.h"

int main(int argc, char* argv[]) {
	int option = recupererOption(argv[1]);
	if (argc<3 || option==OPT_H)
		printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");

	else if (argc!=4 && option==OPT_S)
		printf("Usage : \"./edl_fusion -s [nom_fichier] [numero_section]\"\n");

	else if (option!=NO_OPT){
		Elf32_Ehdr* header;
		FILE *f;
		f = fopen(argv[2],"r");
		if (f==NULL) printf("Echec d'ouverture du fichier\n");
		else {
			header = initHeader(f);
			Elf32_Shdr* sectionTable[header->e_shnum];
			readSection(sectionTable, header, f);

			// Creation de sectionName
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
			}

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
			}
			fclose(f);
		}
	}
	else printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
	return 0;
}
