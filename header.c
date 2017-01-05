#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void reverseBytesHeader(Elf32_Ehdr* header) {
	header->e_type = reverse_2(header->e_type);
	header->e_machine = reverse_2(header->e_machine);
	header->e_version = reverse_4(header->e_version);
	header->e_entry = reverse_4(header->e_entry);
	header->e_phoff = reverse_4(header->e_phoff);
	header->e_shoff = reverse_4(header->e_shoff);
	header->e_flags = reverse_4(header->e_flags);
	header->e_ehsize = reverse_2(header->e_ehsize);
	header->e_phentsize = reverse_2(header->e_phentsize);
	header->e_phnum = reverse_2(header->e_phnum);
	header->e_shentsize = reverse_2(header->e_shentsize);
	header->e_shnum = reverse_2(header->e_shnum);
	header->e_shstrndx = reverse_2(header->e_shstrndx);
}

Elf32_Ehdr* initHeader(FILE* f) {
	Elf32_Ehdr* header;
	header = malloc(sizeof(Elf32_Ehdr));
	if (header==NULL) printf("Echec d'allocation mémoire\n");
	else {
		fread(header,1,sizeof(Elf32_Ehdr),f);
		if (header->e_ident[5]==2) reverseBytesHeader(header);
	}
	return header;
}
