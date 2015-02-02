#COMPILE_OPTS := "-DDEBUG=1"

misspell: misspell.o language_model.o
	g++ -o $@ $^

misspell.o: misspell.cpp language_model.h words_splitter.h string_iterator.h
	g++ -c -std=c++0x ${COMPILE_OPTS} -O3 -o $@ $<

language_model.o: language_model.cpp language_model.h
	g++ -c -std=c++0x ${COMPILE_OPTS} -O3 -o $@ $<

run: misspell
	./misspell

clean:
	rm -f misspell misspell.o language_model.o input.txt output.txt
