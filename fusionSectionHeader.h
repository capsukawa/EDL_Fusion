#ifndef FUSIONSECTIONHEADER_H
#define FUSIONSECTIONHEADER_H

void calculShOff(ElfFileStruct* elf1,ElfFileStruct* elf2 , int* addTailleSection, int* decalageTotal, int* sectionEnPlus, int **decalageSection);

void fusionSectionHeader(ElfFileStruct* elf1, ElfFileStruct* elf2, ElfFileStruct* elf);


#endif
