CC=gcc
FLAGS=-Wall -Werror

edl_fusion: main.o afficheHeader.o header.o
	$(CC) -o $@ $^

main.o: main.c afficheHeader.h header.h
	$(CC) $(FLAGS) -c $<

afficheHeader.o: afficheHeader.c afficheHeader.h header.h
	$(CC) $(FLAGS) -c $<

header.o: header.c header.h
	$(CC) $(FLAGS) -c $<
