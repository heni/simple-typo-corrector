#COMPILE_OPTS := "-DDEBUG=1"

misspell: misspell.cpp language_model.h words_splitter.h string_iterator.h
	g++ -g -std=c++11 ${COMPILE_OPTS} -o $@ $<
