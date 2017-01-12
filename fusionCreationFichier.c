#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "elfStruct.h"
#include "header.h"
#include "util.h"
#include "sections.h"
#include "fusionHeader.h"


void creationFichierFusion(ElfFileStruct* elf) {
	FILE *f;
	int i;

	f = fopen("fusion.o","w");

	// Ecriture du elf header
	fseek(f,0,SEEK_SET);
	fwrite(elf->header,elf->header->e_ehsize,1, f);	
	
	// Ecriture sections header & contenu des sections dans l'ordre des sections header 
	i=0;
	while(i!=elf->header->e_shnum){
		// Ecriture du section Header de la section i 
		fseek(f,elf->header->e_shoff + i*elf->header->e_shentsize, SEEK_SET); // deb section header + place du header
		fwrite(elf->sections[i]->header,elf->header->e_shentsize,1, f);
		// Ecriture de la section i
		fseek(f,elf->sections[i]->header->sh_offset,SEEK_SET);	// debut de la section 
		fwrite(elf->sections[i]->content,elf->sections[i]->header->sh_size,1, f);
			
		i=i+1;

	}
	fclose(f);
}
