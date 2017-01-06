#ifndef CONTENUSECTION_H
#define CONTENUSECTION_H

void affichageSection(Elf32_Ehdr* header, FILE* f, char *nom, char* sectionNames[header->e_shnum]);

#endif
