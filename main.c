#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "elfStruct.h"
#include "options.h"
#include "header.h"
#include "sections.h"
#include "symbols.h"
#include "relocation.h"
#include "affichage.h"

int main(int argc, char* argv[]) {
	if (argc<=1) printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
	else {
		int option = recupererOption(argv[1]);
		if (argc<3 || option==OPT_H)
			printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");

		else if (argc!=4 && option==OPT_S)
			printf("Usage : \"./edl_fusion -s [nom_fichier] [numero_section]\"\n");

		else if (option!=NO_OPT){
			ElfFileStruct* elf1;
			elf1 = malloc(sizeof(ElfFileStruct));
			if (elf1==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			FILE *f;
			FILE *f2;
			f = fopen(argv[2],"r");
			if (f==NULL) printf("Echec d'ouverture du fichier\n");
			else if (!verifELF(f)) printf("Le fichier donné en paramètre n'est pas un fichier ELF.\n");
			else {
				remplirStruct(f, elf1);
				switch(option) {
					case OPT_E:
						afficheHeader(elf1);
						break;
					case OPT_S:
						afficheSectionContent(elf1,atoi(argv[3]));
						break;
					case OPT_SH:
						afficheSectionHeaderTable(elf1);
						break;
					case OPT_TS:
						afficheSymbolTable(elf1);
						break;
					case OPT_R:
						afficheRelTable(elf1);
						break;
					case OPT_F:
						/*f2 = fopen(argv[3],"r");
						Elf32_Ehdr* header2 = initHeader(f2);
						Elf32_Shdr* sectionTable2[elf1.header->e_shnum];
						char* sectionNames2[elf1.header->e_shnum];
						readSection(sectionTable2, header2, f2);
						readNames(sectionNames2, header2, f2, sectionTable2);
						fusionSection(elf1.header,header2, f, f2, elf1.sectionNames, sectionNames2, elf1.sectionTable, sectionTable2);
						fclose(f2);*/
						break;
				}
				fclose(f);
				freeELF(elf1);
			}
		}
		else printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
	}
	return 0;
}
