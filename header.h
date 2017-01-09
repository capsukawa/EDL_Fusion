#ifndef HEADER_H
#define HEADER_H

#include "elfStruct.h"

void reverseBytesHeader(Elf32_Ehdr* header);
int verifELF(FILE *f);
void initHeader(ElfFileStruct* elf,FILE* f);

#endif
