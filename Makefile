CC=gcc
FLAGS=-Wall -Werror

edl_fusion: main.o affichage.o header.o sections.o symbols.o relocation.o elfStruct.o fusionHeader.o fusionSection.o fusionSymbole.o fusionCreationFichier.o
	$(CC) -o $@ $^

%.o: %.c %.h header.h
	$(CC) $(FLAGS) -c $<

main.o: affichage.o
