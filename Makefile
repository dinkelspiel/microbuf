run:
	gcc main.c microbuf/lexer.c microbuf/file.c microbuf/parser.c microbuf/utils.c -I microbuf -o main && ./main