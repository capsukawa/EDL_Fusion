#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "sectionTable.h"

void afficheSectionTable(Elf32_Shdr** sectionTable, Elf32_Ehdr* header, char** tableName) {
	printf("num\tName\tType\tFlags\tAdr\tOffset\tSize\tLink\tInfo\tAdalign\tEntsize\n");

	int i;
	for(i=0; i<header->e_shnum; i++) {
		printf("[ %d]\t",i);
		printf("%s\t", tableName[i]);
		printf("%x\t", sectionTable[i]->sh_type);
		printf("%x\t", sectionTable[i]->sh_flags);
		printf("%x\t", sectionTable[i]->sh_addr);
		printf("%x\t", sectionTable[i]->sh_offset);
		printf("%x\t", sectionTable[i]->sh_size);
		printf("%x\t", sectionTable[i]->sh_link);
		printf("%x\t", sectionTable[i]->sh_info);
		printf("%x\t", sectionTable[i]->sh_addralign);
		printf("%x\n", sectionTable[i]->sh_entsize);
	}
}
