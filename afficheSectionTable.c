#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "sectionTable.h"

void afficheType(unsigned int type)
{
	char* chaine = "";
	switch(type)
	{
		case 0x00:
			chaine = "SHT_NULL";
			break;
		case 0x01:
			chaine = "SHT_PROGBITS";
			break;
		case 0x02:
			chaine = "SHT_SYMTAB";
			break;
		case 0x03:
			chaine = "SHT_STRTAB";
			break;
		case 0x04:
			chaine = "SHT_RELA";
			break;
		case 0x05:
			chaine = "SHT_HASH";
			break;
		case 0x06:
			chaine = "SHT_DYNAMIC";
			break;
		case 0x07:
			chaine = "SHT_NOTE";
			break;
		case 0x08:
			chaine = "SHT_NOBITS";
			break;
		case 0x09:
			chaine = "SHT_REL";
			break;
		default:
			printf("%-20x", type);
	}
	if (strcmp(chaine,""))
	{
		printf("%-20s", chaine);
	}
}

void afficheFanions(unsigned int flag)
{
	char chaine[10]= "";
	int i= 0;
	
	if(flag & 0x1)
	{
		chaine[i] = 'W';
		i++;
	}
	if(flag & 0x2)
	{
		chaine[i] = 'A';
		i++;
	}
	if(flag & 0x4)
	{
		chaine[i] = 'X';
		i++;
	}
	if(flag & 0x10)
	{
		chaine[i] = 'M';
		i++;
	}
	if(flag & 0x20)
	{
		chaine[i] = 'S';
		i++;
	}
	if(flag & 0x40)
	{
		chaine[i] = 'I';
		i++;
	}
	if(flag & 0x80)
	{
		chaine[i] = 'L';
		i++;
	}
	if(flag & 0x100)
	{
		chaine[i] = 'O';
		i++;
	}
	if(flag & 0x200)
	{
		chaine[i] = 'G';
		i++;
	}
	if(flag & 0x400)
	{
		chaine[i] = 'T';
		i++;
	}
	if(flag & 0x0ff00000)
	{
		chaine[i] = 'o';
		i++;
	}
	if(flag & 0x80000000)
	{
		chaine[i] = 'E';
		i++;
	}
	if(flag & 0xf0000000)
	{
		chaine[i] = 'p';
		i++;
	}
	
	printf("%-10s", chaine);	
}

void afficheSectionTable(Elf32_Shdr** sectionTable, Elf32_Ehdr* header, char** tableName)
{

	printf("%-4s%-20s%-20s%-10s%-8s %-8s %-8s %-8s %-8s %-8s %-8s\n", 
		"Num", "Name", "Type", "Flags", "Adr", "Offset", "Size", "Link", "Info", "Adalign", "Entsize");
	
	int i;
	for(i=0; i<header->e_shnum; i++)
	{
		printf("%-4d",i);
		printf("%-20s", tableName[i]);
		afficheType(sectionTable[i]->sh_type);
		afficheFanions(sectionTable[i]->sh_flags);
		printf("%08x ", sectionTable[i]->sh_addr);
		printf("%08x ", sectionTable[i]->sh_offset);
		printf("%08x ", sectionTable[i]->sh_size);
		printf("%08x ", sectionTable[i]->sh_link);
		printf("%08x ", sectionTable[i]->sh_info);
		printf("%-8x ", sectionTable[i]->sh_addralign);
		printf("%-8x\n", sectionTable[i]->sh_entsize);
	}
	printf(" W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes)\n I (info), L (ordre des liens), G (groupe), T (TLS), E (exclu)\n O (traiterment additionnel requis pour l\'OS) o (spécifique à l\'OS), p (spécifique au processeur)\n");
}
