CC=gcc
FLAGS=-Wall -Werror

afficheHeader: afficheHeader.o header.o
	$(CC) -o $@ $^

afficheHeader.o: afficheHeader.c header.h
	$(CC) $(FLAGS) -c $<

header.o: header.c header.h
	$(CC) $(FLAGS) -c $<
