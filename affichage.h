#ifndef AFFICHAGE_H
#define AFFICHAGE_H

void afficheHeader(ElfFileStruct* elf);
void afficheSectionHeaderTable(ElfFileStruct* elf);
void afficheSectionContent(ElfFileStruct* elf,int numSection);
void afficheSymbolTable(ElfFileStruct* elf);
void afficheRelTable(ElfFileStruct* elf);

#endif
