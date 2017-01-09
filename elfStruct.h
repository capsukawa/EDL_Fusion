#ifndef ELFSTRUCT_H
#define ELFSTRUCT_H

#include <stdio.h>

typedef struct {
	char* name;
	Elf32_Shdr* header;
	unsigned char* content;
} Elf_Section;

typedef struct {
	char* name;
	Elf32_Sym* sym;
} Elf_Symbol;

typedef struct {
	Elf32_Ehdr* header;
	Elf_Section** sections;
	Elf_Symbol** symbols;
	Elf32_Rel** relTab;
	int nbSym, nbRel;
} ElfFileStruct;

void remplirStruct(FILE* f, ElfFileStruct* elf);

#endif
