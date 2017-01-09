#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include "sections.h"
#include "symbols.h"
#include "relocation.h"

void remplirStruct(FILE* f, ElfFileStruct* elf) {
	initHeader(elf,f);
	initSections(elf,f);
	initSymbols(elf,f);
	initRel(elf,f);
}
