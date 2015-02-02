run-tests: checkers/checker solutions/misspell
	for test in tests/??; do \
        ln -sf $$(realpath $${test}) solutions/input.txt; \
        make -C solutions run; \
        checkers/checker solutions/input.txt solutions/output.txt $${test}.a; \
        rm solutions/input.txt solutions/output.txt; \
    done

checkers/checker:
	make -C checkers

checkers/full-checker.cpp:
	make -C checkers full-checker.cpp

solutions/misspell:
	make -C solutions



solution_files := bigrams.txt language_model.cpp language_model.h makefile misspell.cpp string_iterator.h unigrams.txt words_splitter.h
checker_files := full-checker.cpp checker.cpp makefile utf8.h utf8/checked.h utf8/core.h utf8/unchecked.h
test_files := $(shell ls tests) 
archive_files := $(patsubst %, solutions/%, $(solution_files)) $(patsubst %, checkers/%, $(checker_files)) $(patsubst %, tests/%, $(test_files))

archives: full-archive solutions-archive tests-archive checkers-archive

full-archive: full-archive.tar.bz2
full-archive.tar.bz2: $(archive_files)
	tar cjf $@ $(archive_files)

solutions-archive: solutions.tar.bz2
solutions.tar.bz2: $(patsubst %, solutions/%, $(solution_files)) 
	tar cjf $@ -C solutions $(solution_files)

tests-archive: tests.tar.bz2
tests.tar.bz2: $(patsubst %, tests/%, $(test_files))
	tar cjf $@ -C tests $(test_files)

checkers-archive: checkers.tar.bz2
checkers.tar.bz2: $(patsubst %, checkers/%, $(checker_files))
	tar cjf $@ -C checkers $(checker_files)
	
clean:
	make -C checkers clean; \
    make -C solutions clean; \
    rm -f *.tar.bz2
