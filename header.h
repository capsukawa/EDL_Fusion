#ifndef HEADER_H
#define HEADER_H

void reverseBytesHeader(Elf32_Ehdr* header);
Elf32_Ehdr* initHeader(FILE* f);

#endif
