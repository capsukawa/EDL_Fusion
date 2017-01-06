#ifndef SECTIONNAMES_H
#define SECTIONNAMES_H

void readNames(char** table, Elf32_Ehdr* header, FILE* f,Elf32_Shdr** sectionTable);

#endif
