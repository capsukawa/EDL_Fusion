#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "elfStruct.h"
#include "sections.h"

void afficheHeader(ElfFileStruct* elf) {
	printf("\n-- En-tete du fichier --\n");
	
	//- Identification -----------------------------------------------------------
	printf("Identification : ");
	for (int i=0;i<EI_NIDENT;i++) printf("%x ",elf->header->e_ident[i]);

	printf("\n - Architecture -> ");
	if (elf->header->e_ident[4]==0) printf("None");
	else if (elf->header->e_ident[4]==1) printf("32bits");
	else if (elf->header->e_ident[4]==2) printf("64bits");

	printf("\n - Endianness -> ");
	if (elf->header->e_ident[5]==0) printf("None");
	else if (elf->header->e_ident[5]==1) printf("LSB");
	else if (elf->header->e_ident[5]==2) printf("MSB");

	printf("\n - Header Version -> ");
	if (elf->header->e_ident[6]==0) printf("0 (old)");
	else if (elf->header->e_ident[6]==1) printf("1 (current)");

	printf("\n - OS/ABI -> ");
	switch(elf->header->e_ident[7]) {
		case 0:
			printf("Unix System V"); break;
		case 3:
			printf("Linux"); break;
		case 64:
			printf("ARM EABI"); break;
		case 97:
			printf("ARM"); break;
		default:
			printf("Unknown"); break;
	}
	//- Type de fichier ----------------------------------------------------------
	printf("\nFile type : ");
	switch(elf->header->e_type) {
		case 0:
			printf("None"); break;
		case 1:
			printf("Relocatable"); break;
		case 2:
			printf("Executable"); break;
		case 3:
			printf("Shared"); break;
		case 4:
			printf("Core"); break;
	}
	//- Machine cible ------------------------------------------------------------
	printf("\nTarget architecture : ");
	switch(elf->header->e_machine) {
		case 40:
			printf("ARM"); break;
		case 62:
			printf("x64"); break;
		default:
			printf("Other"); break;
	}
	//- Version ------------------------------------------------------------------
	printf("\nVersion : %d\n",elf->header->e_version);
	//- Autres -------------------------------------------------------------------
	printf("Entry point : %d\n",elf->header->e_entry);
	printf("Program Header Offset : %d\n",elf->header->e_phoff);
	printf("Section Header Offset : %d\n",elf->header->e_shoff);
	printf("Flags : %d\n",elf->header->e_flags);
	printf("ELF Header Size : %d\n",elf->header->e_ehsize);
	printf("Program Header Entry Size : %d\n",elf->header->e_phentsize);
	printf("Program Header Entry Number : %d\n",elf->header->e_phnum);
	printf("Section Header Entry Size : %d\n",elf->header->e_shentsize);
	printf("Section Header Entry Number : %d\n",elf->header->e_shnum);
	printf("Index of .shstrtab Section Header : %d\n",elf->header->e_shstrndx);
}

void afficheType(unsigned int type) {
	char* chaine = "";
	switch(type) {
		case SHT_NULL:
			chaine = "SHT_NULL";
			break;
		case SHT_PROGBITS:
			chaine = "SHT_PROGBITS";
			break;
		case SHT_SYMTAB:
			chaine = "SHT_SYMTAB";
			break;
		case SHT_STRTAB:
			chaine = "SHT_STRTAB";
			break;
		case SHT_RELA:
			chaine = "SHT_RELA";
			break;
		case SHT_HASH:
			chaine = "SHT_HASH";
			break;
		case SHT_DYNAMIC:
			chaine = "SHT_DYNAMIC";
			break;
		case SHT_NOTE:
			chaine = "SHT_NOTE";
			break;
		case SHT_NOBITS:
			chaine = "SHT_NOBITS";
			break;
		case SHT_REL:
			chaine = "SHT_REL";
			break;
		default:
			printf("%-20x", type);
	}
	if (strcmp(chaine,"")) {
		printf("%-20s", chaine);
	}
}

void afficheFanions(unsigned int flag) {
	char chaine[10] = "";
	int i = 0;

	if(flag & 0x1) {
		chaine[i] = 'W';
		i++;
	}
	if(flag & 0x2) {
		chaine[i] = 'A';
		i++;
	}
	if(flag & 0x4) {
		chaine[i] = 'X';
		i++;
	}
	if(flag & 0x10) {
		chaine[i] = 'M';
		i++;
	}
	if(flag & 0x20) {
		chaine[i] = 'S';
		i++;
	}
	if(flag & 0x40) {
		chaine[i] = 'I';
		i++;
	}
	if(flag & 0x80) {
		chaine[i] = 'L';
		i++;
	}
	if(flag & 0x100) {
		chaine[i] = 'O';
		i++;
	}
	if(flag & 0x200) {
		chaine[i] = 'G';
		i++;
	}
	if(flag & 0x400) {
		chaine[i] = 'T';
		i++;
	}
	if(flag & 0x0ff00000) {
		chaine[i] = 'o';
		i++;
	}
	if(flag & 0x80000000) {
		chaine[i] = 'E';
		i++;
	}
	if(flag & 0xf0000000) {
		chaine[i] = 'p';
		i++;
	}
	printf("%-10s", chaine);
}

void afficheSectionHeaderTable(ElfFileStruct* elf) {
	printf("\n-- Table des en-tetes de section --\n");
	
	printf("%-4s %-20s%-20s%-10s%-8s %-8s %-8s %-8s %-8s %-8s %-8s\n",
			"[Nr]", "Name", "Type", "Flags", "Adr", "Offset", "Size", "Link", "Info", "Adalign", "Entsize");

	int i;
	for(i=0; i<elf->header->e_shnum; i++) {
		printf("[%2d] ",i);
		printf("%-20s", elf->sections[i]->name);
		afficheType(elf->sections[i]->header->sh_type);
		afficheFanions(elf->sections[i]->header->sh_flags);
		printf("%08x ", elf->sections[i]->header->sh_addr);
		printf("%08x ", elf->sections[i]->header->sh_offset);
		printf("%08x ", elf->sections[i]->header->sh_size);
		printf("%08x ", elf->sections[i]->header->sh_link);
		printf("%08x ", elf->sections[i]->header->sh_info);
		printf("%-8x ", elf->sections[i]->header->sh_addralign);
		printf("%-8x\n", elf->sections[i]->header->sh_entsize);
	}
	printf(" W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes)\n I (info), L (ordre des liens), G (groupe), T (TLS), E (exclu)\n O (traiterment additionnel requis pour l\'OS) o (spécifique à l\'OS), p (spécifique au processeur)\n");
}

//renvoie 1 si nb est un nombre, 0 sinon
int isNumber(char* input) {
	int i;
	int b=1;
	int length = strlen(input);
	for(i=0;i<length;i++) {
		if(!isdigit(input[i])) {
			b=0;
		}
	}
	return b;
}

void printSection(Elf_Section* section) {
	printf("\n-- Contenu de la section %s--\n",section->name);
	int taille = section->header->sh_size;
	
	int i=0;
	while(i<taille) {
		if(i%16==0) {
			printf("\n");
			printf("0x%08x ",i);
		}
		if(i%8==0) {
			printf(" ");
		}
		printf("%02x ",section->content[i]);
		i++;
	}
	printf("\n");
}

void afficheSectionContent(ElfFileStruct* elf,char* idSection) {
	int index;
	if(isNumber(idSection)) {
		index = atoi(idSection);
		if (index<elf->header->e_shnum) { // Pour si on demande une section qui n'existe pas..
			printSection(elf->sections[index]);
		}
		else { printf("La section %d n'existe pas; La dernière est la section %d\n",index,elf->header->e_shnum-1); }
	}
	else {
		index = findSectionHeader(idSection,elf);
		if(index>=0) {
			printSection(elf->sections[index]);
		}
		else { printf("La section de nom %s n'existe pas\n",idSection); }
	}
}

void afficheTypeSymbole(unsigned char type) {
	char* chaine = "";
	switch(type) {
		case STT_NOTYPE:
			chaine="NOTYPE";
			break;
		case STT_OBJECT:
			chaine = "OBJECT";
			break;
		case STT_FUNC:
			chaine = "FUNC";
			break;
		case STT_SECTION:
			chaine = "SECTION";
			break;
		case STT_FILE:
			chaine = "FILE";
			break;
		default:
			printf("%-12x", type);
	}
	if (strcmp(chaine,"")) printf("%-12s", chaine);
}

void afficheLienSymbole(unsigned char info) {
	char* chaine = "";
	switch(ELF32_ST_BIND(info)) {
		case STB_LOCAL:
			chaine="LOCAL";
			break;
		case STB_GLOBAL:
			chaine = "GLOBAL";
			break;
		case STB_WEAK:
			chaine = "WEAK";
			break;
		case STB_LOPROC:
			chaine = "LOPROC";
			break;
		case STB_HIPROC:
			chaine = "HIPROC";
			break;
		default:
			printf("%-6x ", ELF32_ST_BIND(info));
	}
	if (strcmp(chaine,"")) printf("%-6s ", chaine);
}

void afficheSymbolTable(ElfFileStruct* elf) {
	printf("\n-- Table des symboles --\n");
	
	printf("%4s%-8s %-5s %-12s %-6s %-7s %-8s %-20s","Num:"," Value"," Size"," Type","Lien","Vis","Ndx", "Name");
	printf("\n");

	int i;
	for(i=0; i<elf->nbSym; i++) {
		// --AFFICHAGE--

		printf("%3d: ", i); // index
		printf("%08x ", elf->symbols[i]->sym->st_value); // value
		printf("%5i ", elf->symbols[i]->sym->st_size); // size
		afficheTypeSymbole(elf->symbols[i]->sym->st_info); // type
		afficheLienSymbole(elf->symbols[i]->sym->st_info); // lien
		printf("%-7s ","DEFAULT");	// vis

		//index
		if (elf->symbols[i]->sym->st_shndx == 0) { printf("%-8s ","UND"); }
		else if (elf->symbols[i]->sym->st_shndx == 0xfff1) { printf("%-8s ","ABS"); }
		else { printf("%-8x ", elf->symbols[i]->sym->st_shndx); }
		
		printf("%-20s", elf->symbols[i]->name); // name
		printf("\n");
	}
}

void afficheTypeRel(Elf32_Rel* rel) {
	char* chaine = "";
	switch(ELF32_R_TYPE(rel->r_info)) {
		case R_ARM_ABS32:
			chaine = "R_ARM_ABS32";
			break;
		case R_ARM_CALL:
			chaine = "R_ARM_CALL";
			break;
		default:
			printf("%08x         ", ELF32_R_TYPE(rel->r_info));
	}
	if (strcmp(chaine,"")) printf("%-16s ", chaine);
}

void afficheRelTable(ElfFileStruct* elf) {
	int i,j;
	
	printf("\n-- Table de reloc --\n");
	for(i=0;i<elf->header->e_shnum;i++) {
		if(elf->sections[i]->header->sh_type == SHT_REL) {
			printf("\nSection de relocalisation \'%s\', offset 0x%04x avec %d entrées:\n", elf->sections[i]->name, elf->sections[i]->header->sh_offset, elf->sections[i]->nbRel);
			printf("%-8s %-8s %-16s %-20s\n", "Offset"," Info"," Type"," Sym");
			for(j=0;j<elf->sections[i]->nbRel;j++) {
				printf(" %08x %08x ", elf->sections[i]->relTab[j]->rel->r_offset, elf->sections[i]->relTab[j]->rel->r_info);
				afficheTypeRel(elf->sections[i]->relTab[j]->rel);
				printf("%-20s", elf->sections[i]->relTab[j]->name);
				printf("\n");
			}
		}
	}
}

void afficheAide() {
	printf("-e [nom_du_fichier] : affiche l'en-tete du fichier\n");
	printf("-s [id_de_section] [nom_du_fichier] : affiche le contenu de la section\n");
	printf("-S [nom_du_fichier] : affiche la table des sections\n");
	printf("-t [nom_du_fichier] : affiche la table des symboles\n");
	printf("-r [nom_du_fichier] : affiche la table de réimplantation\n");
	printf("-f [nom_du_fichier_2] [nom_du_fichier] : fusionne deux fichiers ELF\n");
}
