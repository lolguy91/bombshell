build:
	gcc -g -O0 -Werror main.c -o bin/bombshell

run: build
	PS1="%B\033[94m%n\033[93m@\033[34m%m\033[0m:\033[92m%B%p\033[0m" PS1_RIGHT="\033[95mDate\033[93m:\033[92m%d\033[0m - \033[95mTime\033[93m:\033[92m%T" PS1_LINE_2="\033[95m%B%#\033[0m" bin/bombshell