#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void readNames(char** table, Elf32_Ehdr* header, FILE* f, Elf32_Shdr** sectionTable) {
	fseek(f, header->e_shoff, SEEK_SET);
	int i;
		// Remplissage de sectionNames
		for(i=0; i<header->e_shnum;i++) {
			table[i]=malloc(sizeof(char)*20); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			fseek(f, sectionTable[header->e_shstrndx]->sh_offset + sectionTable[i]->sh_name, SEEK_SET);
			char c; int cmt = 0;
			fread(&c,1,1,f);
			while(c != 0x00) {
				table[i][cmt] = c;
				cmt++;
				fread(&c,1,1,f);
			}
		}

}
