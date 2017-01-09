#ifndef CONTENUSECTION_H
#define CONTENUSECTION_H

unsigned char *recupererVarSection(Elf32_Word taille, FILE* f);

Elf32_Word recupererTailleSection(Elf32_Ehdr* header, FILE* f, char *nom, char** sectionNames);

void affichageSection(Elf32_Ehdr* header, FILE* f, char *nom, char** sectionNames);

#endif
