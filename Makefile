CC=gcc
FLAGS=-Wall -Werror

edl_fusion: main.o affichage.o header.o sections.o symbols.o relocation.o elfStruct.o options.o fusionSectionHeader.o fusionSection.o fusionSymbole.o
	$(CC) -o $@ $^

%.o: %.c %.h header.h
	$(CC) $(FLAGS) -c $<

main.o: affichage.o
