#ifndef FUSIONSECTIONHEADER_H
#define FUSIONSECTIONHEADER_H

void preTraitementF1(ElfFileStruct* elf1,ElfFileStruct* elf2, int *decalageSection);

void calculShOff(ElfFileStruct* elf1,ElfFileStruct* elf2 , int* addTailleSection, int* decalageTotal, int* sectionEnPlus);

void fusionSectionHeader(ElfFileStruct* elf1, ElfFileStruct* elf2, ElfFileStruct* elf);


#endif
