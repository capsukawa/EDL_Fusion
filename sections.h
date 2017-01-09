#ifndef SECTIONS_H
#define SECTIONS_H

int findSectionHeader(char* name, ElfFileStruct* elf);
void initSections(ElfFileStruct* elf, FILE* f);

#endif
