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

			// Deuxième fichier pour la fusion
			ElfFileStruct* elf2;
			FILE *f2;

			// Fichier final fusionné
			ElfFileStruct* elff;
			FILE *ff;

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
						afficheSectionContent(elf1,argv[3]);
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
						f2 = fopen(argv[3],"r");
						elf2 = malloc(sizeof(ElfFileStruct));
						if (elf2==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
						remplirStruct(f2,elf2);

						//fusionSection(f,f2,elf1,elf2);

						fclose(f2);
						freeELF(elf2);
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
