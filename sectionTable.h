#ifndef SECTIONTABLE_H
#define SECTIONTALBE_H

void readSection(Elf32_Shdr** table, Elf32_Ehdr* header, FILE* f);

#endif
