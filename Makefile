TMCODE = code.tm

#exec file
compiler:main.o ParserGenerator.o Struct.o
	g++ -o compiler main.o ParserGenerator.o Struct.o

clean:
	rm -f  *.o
	rm -f  compiler
