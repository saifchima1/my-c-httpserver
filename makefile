flags = -Wall -Wextra -g

all: compile

main:
	@gcc $(flags) src/main.c -c
handler:
	@gcc $(flags) src/handler.c -c
utils:
	@gcc $(flags) src/utils.c -c 
compile:
	@gcc $(flags) ./*.o -o server
clear:
	@rm ./*.o
