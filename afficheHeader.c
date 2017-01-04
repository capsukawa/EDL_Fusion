#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

void afficheHeader(char* nf) {
	Elf32_Ehdr* header;
	header = initHeader(nf);

	printf("e_ident : ");
	for (int i=0;i<EI_NIDENT;i++) printf("%x ",header->e_ident[i]);
	printf("\ne_type : %d\n",header->e_type);
	printf("e_machine : %d\n",header->e_machine);
	printf("e_version : %d\n",header->e_version);
	printf("e_entry : %d\n",header->e_entry);
	printf("e_phoff : %d\n",header->e_phoff);
	printf("e_shoff : %d\n",header->e_shoff);
	printf("e_flags : %d\n",header->e_flags);
	printf("e_ehsize : %d\n",header->e_ehsize);
	printf("e_phentsize : %d\n",header->e_phentsize);
	printf("e_phnum : %d\n",header->e_phnum);
	printf("e_shentsize : %d\n",header->e_shentsize);
	printf("e_shnum : %d\n",header->e_shnum);
	printf("e_shstrndx : %d\n",header->e_shstrndx);
}
