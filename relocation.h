#ifndef RELOCATION_H
#define RELOCATION_H

/*
Initialise la table de ré implémentation
*/
void initRel(ElfFileStruct* elf, FILE* f);

void initNbRelEntries(ElfFileStruct* elf);

#endif
