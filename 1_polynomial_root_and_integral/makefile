target:	hw1

hw1:	hw1_main.c hw1_lib.h hw1_lib.c hw1_io.h hw1_io.c
		gcc -ansi -pedantic-errors hw1_main.c hw1_lib.c hw1_io.c -o hw1 -lm
		./hw1 < input.txt

clean:
		rm hw1