#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include "sections.h"
#include "symbols.h"
#include "relocation.h"

void remplirStruct(FILE* f, ElfFileStruct* elf) {
	initHeader(elf,f);
	initSections(elf,f);
	initSymbols(elf,f);
	initRel(elf,f);
}

void freeELF(ElfFileStruct* elf) {
	int i,j;
	for (i=0;i<elf->header->e_shnum;i++) {
		free(elf->sections[i]->name);
		free(elf->sections[i]->content);
		free(elf->sections[i]->header);
		for(j=0;j<elf->sections[i]->nbRel;j++) {
			free(elf->sections[i]->relTab[j]->rel);
			free(elf->sections[i]->relTab[j]);
		}
		free(elf->sections[i]->relTab);
		free(elf->sections[i]);
	}
	free(elf->sections);
	for (i=0;i<elf->nbSym;i++) {
		free(elf->symbols[i]->name);
		free(elf->symbols[i]->sym);
		free(elf->symbols[i]);
	}
	free(elf->symbols);
	free(elf->header);
	free(elf);
}
