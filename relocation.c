#include <elf.h>
#include <stdlib.h>
#include "elfStruct.h"
#include "util.h"

void initNbRelEntries(ElfFileStruct* elf) {
	int i;
	for (i=0;i<elf->header->e_shnum;i++) { //pour chaque section
		//REL
		if(elf->sections[i]->header->sh_type == SHT_REL) {
			elf->sections[i]->nbRel = elf->sections[i]->header->sh_size/elf->sections[i]->header->sh_entsize;
		} else {
			elf->sections[i]->nbRel=0;
		}
	}
}

void initRel(ElfFileStruct* elf, FILE* f) {
	initNbRelEntries(elf);
	int i, j;
	unsigned char infoSymRel;
	unsigned char typeSymRel;
	
	for (i=0;i<elf->header->e_shnum;i++) { //pour chaque section
		//REL
		if(elf->sections[i]->header->sh_type == SHT_REL) {
			elf->sections[i]->relTab = malloc(sizeof(Elf_Rel)*elf->sections[i]->nbRel);
			if (elf->sections[i]->relTab==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			for (j=0;j<elf->sections[i]->nbRel;j++) { //pour chaque entrée dans la table de relocation
				fseek(f,elf->sections[i]->header->sh_offset+j*sizeof(Elf32_Rel),SEEK_SET); //seek au bon endroit
				elf->sections[i]->relTab[j]=malloc(sizeof(Elf_Rel)); //allocation memoire
				elf->sections[i]->relTab[j]->rel=malloc(sizeof(Elf32_Rel));
				if (elf->sections[i]->relTab[j]->rel==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
				fread(elf->sections[i]->relTab[j]->rel, 1, sizeof(Elf32_Rel), f); //lecture dans le fichier et affectation dans la structure

				//endianess
				if (elf->header->e_ident[5]==2) {
					elf->sections[i]->relTab[j]->rel->r_offset = reverse_4(elf->sections[i]->relTab[j]->rel->r_offset);
					elf->sections[i]->relTab[j]->rel->r_info = reverse_4(elf->sections[i]->relTab[j]->rel->r_info);
				}
				
				//name
				infoSymRel = ELF32_R_SYM(elf->sections[i]->relTab[j]->rel->r_info);
				typeSymRel = ELF32_ST_TYPE(elf->symbols[infoSymRel]->sym->st_info);
				if(typeSymRel == STT_SECTION) {
					Elf32_Half indexSectionSymRel = elf->symbols[infoSymRel]->sym->st_shndx;
					elf->sections[i]->relTab[j]->name = elf->sections[indexSectionSymRel]->name;
				} else {
					elf->sections[i]->relTab[j]->name = elf->symbols[infoSymRel]->name;
				}
			}
		}
	}
}
