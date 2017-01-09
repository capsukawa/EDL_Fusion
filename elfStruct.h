#ifndef ELFSTRUCT_H
#define ELFSTRUCT_H

#include <stdio.h>

/*
Structure pour les sections
*/
typedef struct {
    char* name;
    Elf32_Shdr* header;
    unsigned char* content;
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
    Elf32_Rel** relTab;
    int nbSym, nbRel;
} ElfFileStruct;

/*
Initialise la structure pour le fichier elf donné en paramètre
*/
void remplirStruct(FILE* f, ElfFileStruct* elf);

#endif
