#ifndef TABLESYMBOLE_H
#define TABLESYMBOLE_H

int rechercheSectionHeader(char nom[8],Elf32_Ehdr* header,char* sectionNames[header->e_shnum]);

void afficherTableSymbole(Elf32_Ehdr* header, FILE* f,Elf32_Shdr* sectionTable[header->e_shnum], char* sectionNames[header->e_shnum],char **tabNomSym);

#endif
