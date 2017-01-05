#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void reverseBytesShdr(Elf32_Shdr* section) {
	section->sh_name = reverse_4(section->sh_name);
	section->sh_type = reverse_4(section->sh_type);
	section->sh_flags = reverse_4(section->sh_flags);
	section->sh_addr = reverse_4(section->sh_addr);
	section->sh_offset = reverse_4(section->sh_offset);
	section->sh_size = reverse_4(section->sh_size);
	section->sh_link = reverse_4(section->sh_link);
	section->sh_info = reverse_4(section->sh_info);
	section->sh_addralign = reverse_4(section->sh_addralign);
	section->sh_entsize = reverse_4(section->sh_entsize);
}

void readSection(Elf32_Shdr** table, Elf32_Ehdr* header, FILE* f) {
	fseek(f, header->e_shoff, SEEK_SET);
	int i;
	for(i=0; i< header->e_shnum; i++) {
		table[i] = malloc(sizeof(Elf32_Shdr));
		fread(table[i],1,sizeof(Elf32_Shdr),f);
		if (header->e_ident[5]==2) reverseBytesShdr(table[i]);
	}
}
