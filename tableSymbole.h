#ifndef TABLESYMBOLE_H
#define TABLESYMBOLE_H

void afficherTableSymbole(Elf32_Ehdr* header, FILE* f,Elf32_Shdr* sectionTable[header->e_shnum], char* sectionNames[header->e_shnum]);

#endif
