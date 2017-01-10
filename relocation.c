#include <elf.h>
#include <stdlib.h>
#include "elfStruct.h"
#include "util.h"

void initNbRelEntries(ElfFileStruct* elf) {
	int i, nbEntries = 0;
	for (i=0;i<elf->header->e_shnum;i++) { //pour chaque section
		//REL
		if(elf->sections[i]->header->sh_type == SHT_REL) {
			nbEntries += elf->sections[i]->header->sh_size/elf->sections[i]->header->sh_entsize;
		}
	}
	elf->nbRel = nbEntries;
}

void initRel(ElfFileStruct* elf, FILE* f) {
	initNbRelEntries(elf);
	int i, j, k=0;
	int nbCurEntries; int tailleEntry;

	elf->relTab = malloc(sizeof(Elf32_Rel)*elf->nbRel);
	for (i=0;i<elf->header->e_shnum;i++) { //pour chaque section
		//REL
		if(elf->sections[i]->header->sh_type == SHT_REL) {
			nbCurEntries = elf->sections[i]->header->sh_size/elf->sections[i]->header->sh_entsize;
			tailleEntry = sizeof(Elf32_Rel);
			for (j=0;j<nbCurEntries;j++) { //pour chaque entrée dans la table de relocation
				fseek(f,elf->sections[i]->header->sh_offset+j*tailleEntry,SEEK_SET); //seek au bon endroit
				elf->relTab[k]=malloc(sizeof(Elf32_Rel)); //allocation memoire
				fread(elf->relTab[k], 1, tailleEntry, f); //lecture dans le fichier et affectation dans la structure

				//endianess
				if (elf->header->e_ident[5]==2) {
					elf->relTab[k]->r_offset = reverse_4(elf->relTab[k]->r_offset);
					elf->relTab[k]->r_info = reverse_4(elf->relTab[k]->r_info);
				}
				k++;
			}
		}
	}
}
