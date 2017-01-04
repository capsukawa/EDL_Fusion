#ifndef HEADER_H
#define HEADER_H

void reverseBytes(Elf32_Ehdr* header);
Elf32_Ehdr* initHeader(char* fadr);

#endif
