#ifndef FUSIONHEADER_H
#define FUSIONHEADER_H

void preTraitementF1(ElfFileStruct* elf1,ElfFileStruct* elf2, int *decalageSection);

void calculShOff(ElfFileStruct* elf1,ElfFileStruct* elf2 , int* addTailleSection, int* decalageTotal, int* sectionEnPlus, int* indice);

void fusionHeader(ElfFileStruct* elf1, ElfFileStruct* elf2, ElfFileStruct* elf);


#endif
