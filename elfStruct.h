#ifndef ELFSTRUCT_H
#define ELFSTRUCT_H

#include <stdio.h>

/*
Structure pour les rel
*/
typedef struct {
    char* name;
    Elf32_Rel* rel;
} Elf_Rel;

/*
Structure pour les sections
*/
typedef struct {
    char* name;
    Elf32_Shdr* header;
    unsigned char* content;
    Elf_Rel** relTab;
    int nbRel;
} Elf_Section;

/*
Structure pour les symboles
*/
typedef struct {
    char* name;
    Elf32_Sym* sym;
} Elf_Symbol;

/*
Structure pour le fichier elf
*/
typedef struct {
    Elf32_Ehdr* header;
    Elf_Section** sections;
    Elf_Symbol** symbols;
    int nbSym;
} ElfFileStruct;

/*
Initialise la structure pour le fichier elf donné en paramètre
*/
void remplirStruct(FILE* f, ElfFileStruct* elf);
void freeELF(ElfFileStruct* elf);

#endif
