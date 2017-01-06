#ifndef RELOCATION_H
#define RELOCATION_H

void readRel(Elf32_Shdr** sectionTable, Elf32_Ehdr* header, Elf32_Rel** relTable, FILE* f);

void afficheTableRel(Elf32_Rel** relTab, Elf32_Sym** symTab, char** symNames, char** sectionNames, int nbElem);

void afficheRel(Elf32_Rel* rel, Elf32_Sym** symTab, char** symNames, char** sectionNames);

int getNbRelEntries(Elf32_Shdr** sectionTable, Elf32_Ehdr* header);

#endif
