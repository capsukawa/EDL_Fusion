#ifndef TABLESYMBOLE_H
#define TABLESYMBOLE_H

int rechercheSectionHeader(char* nom,Elf32_Ehdr* header,char** sectionNames);

void readSymTab(Elf32_Ehdr* header, FILE* f,Elf32_Shdr** sectionTable, char** sectionNames, Elf32_Sym** symTab, char **tabNomSym);

void afficherTableSymbole(Elf32_Sym** symTab, char **tabNomSym, int taille);

#endif
