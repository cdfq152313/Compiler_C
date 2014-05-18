TMCODE = code.tm
EDIR = ./exec/
ODIR = ./output/

#exec file
${EDIR}compiler:${EDIR}main.o ${EDIR}Scanner.o ${EDIR}Parser.o ${EDIR}ParserGenerator.o ${EDIR}CodeGenerator.o ${EDIR}MCodeGenerator.o ${EDIR}Struct.o
	g++ -o ${EDIR}compiler ${EDIR}main.o ${EDIR}Scanner.o ${EDIR}Parser.o ${EDIR}ParserGenerator.o ${EDIR}CodeGenerator.o ${EDIR}MCodeGenerator.o ${EDIR}Struct.o
${EDIR}main.o:main.cpp
	g++ -c main.cpp -o ${EDIR}main.o
${EDIR}Scanner.o:Scanner.h Scanner.cpp
	g++ -c Scanner.cpp -o ${EDIR}Scanner.o
${EDIR}Parser.o:Parser.h Parser.cpp 
	g++ -c Parser.cpp -o ${EDIR}Parser.o
${EDIR}ParserGenerator.o:ParserGenerator.h ParserGenerator.cpp 
	g++ -c ParserGenerator.cpp -o ${EDIR}ParserGenerator.o
${EDIR}CodeGenerator.o:CodeGenerator.h CodeGenerator.cpp
	g++ -c CodeGenerator.cpp -o ${EDIR}CodeGenerator.o
${EDIR}MCodeGenerator.o:MCodeGenerator.h MCodeGenerator.cpp
	g++ -c MCodeGenerator.cpp -o ${EDIR}MCodeGenerator.o
${EDIR}Struct.o:Struct.h Struct.cpp
	g++ -c Struct.cpp -o ${EDIR}Struct.o

#tiny machine
${ODIR}TM:Tiny_Machine.c
	gcc Tiny_Machine.c -o ${ODIR}TM

#exec
exec:${ODIR}TM ${ODIR}${TMCODE} ${EDIR}compiler
	cd ${ODIR}; ./TM ${TMCODE};
${ODIR}${TMCODE}: ${EDIR}compiler ./input/grammar.txt ./input/main.c
	${EDIR}compiler

clean:
	rm -f ${EDIR}*
	rm -f ${ODIR}*
