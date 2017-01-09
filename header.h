#ifndef HEADER_H
#define HEADER_H

#include "elfStruct.h"

/*
Vérifie la présence des nombres magiques 0x7F, E, L, F
*/
int verifELF(FILE *f);

/*
Initialise le header de la structure pour un fichier elf.
*/
void initHeader(ElfFileStruct* elf,FILE* f);

#endif
