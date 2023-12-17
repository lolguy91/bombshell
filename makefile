build:
	gcc -g -O0 -Werror main.c -o bin/bombshell

run: build
	bin/bombshell