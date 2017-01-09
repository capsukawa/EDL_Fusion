#ifndef SECTIONS_H
#define SECTIONS_H

/*
Renvoie l’index de l’entête de section du nom donné en paramètre.
*/
int findSectionHeader(char* name, ElfFileStruct* elf);

/*
Initialise la table des entête de section
*/
void initSections(ElfFileStruct* elf, FILE* f);


#endif
