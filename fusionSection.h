#ifndef FUSIONSECTION_H
#define FUSIONSECTION_H


void fusionSection(Elf32_Ehdr* header1, Elf32_Ehdr* header2, FILE* f1, FILE* f2, char** sectionNamesf1,char** sectionNamesf2, Elf32_Shdr** sectionTable1, Elf32_Shdr** sectionTable2);


#endif
