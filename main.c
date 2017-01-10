#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "elfStruct.h"
#include "options.h"
#include "header.h"
#include "sections.h"
#include "symbols.h"
#include "relocation.h"
#include "affichage.h"
#include "fusionSection.h"
#include "fusionSectionHeader.h"
#include "fusionSymbole.h"

int main(int argc, char* argv[]) {
	if (argc<=1) afficheAide();
	else {
		int c;
		ElfFileStruct* elf1;
		elf1 = malloc(sizeof(ElfFileStruct));
		if (elf1==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
		FILE *f;

		// Utilisé uniquement pour la fusion
		ElfFileStruct* elf2;
		FILE *f2;

		// Fichier final fusionné
		ElfFileStruct* elff;
		FILE *ff;

		f = fopen(argv[argc-1],"r");
		if (!verifELF(f)) printf("Le fichier donné en paramètre n'est pas un fichier ELF.\n");
		else {
			remplirStruct(f, elf1);
			while ((c = getopt (argc, argv, "heStrf:s:")) != -1) {
				switch(c) {
					case 'e':
						afficheHeader(elf1);
						break;
					case 'S':
						afficheSectionHeaderTable(elf1);
						break;
					case 't':
						afficheSymbolTable(elf1);
						break;
					case 'r':
						afficheRelTable(elf1);
						break;
					case 's':
						afficheSectionContent(elf1,optarg);
						break;
					case 'f':
						f2 = fopen(optarg,"r");
						elf2 = malloc(sizeof(ElfFileStruct));
						if (elf2==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
						elff = malloc(sizeof(ElfFileStruct));
						if (elff==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
						remplirStruct(f2,elf2);

						fusionSectionHeader(elf1,elf2,elff);
						fusionSymbole(elf1,elf2,elff);

						fclose(f2);
						freeELF(elf2);
						break;
					default:
						break;
				}
			}
			fclose(f);
			freeELF(elf1);
		}
	}
	return 0;
}
