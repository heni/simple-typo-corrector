run-tests: checker/checker solution/misspell
	for test in tests/??; do \
        ln -sf $$(realpath $${test}) solution/input.txt; \
        make -C solution run; \
        checker/checker solution/input.txt solution/output.txt $${test}.a; \
        rm solution/input.txt solution/output.txt; \
    done

checker/checker:
	make -C checker

solution/misspell:
	make -C solution

solution_files := bigrams.txt language_model.cpp language_model.h makefile misspell.cpp string_iterator.h unigrams.txt words_splitter.h
checker_files := checker.cpp makefile
test_files := $(shell ls tests) 
archive_files := $(patsubst %, solution/%, $(solution_files)) $(patsubst %, checker/%, $(checker_files)) $(patsubst %, tests/%, $(test_files))

archives: full-archive solution-archive tests-archive checker-archive

full-archive: full-archive.tar.bz2
full-archive.tar.bz2: $(archive_files)
	tar cjf $@ $(archive_files)

solution-archive: solution.tar.bz2
solution.tar.bz2: $(patsubst %, solution/%, $(solution_files)) 
	tar cjf $@ -C solution $(solution_files)

tests-archive: tests.tar.bz2
tests.tar.bz2: $(patsubst %, tests/%, $(test_files))
	tar cjf $@ -C tests $(test_files)

checker-archive: checker.tar.bz2
checker.tar.bz2: $(patsubst %, checker/%, $(checker_files))
	tar cjf $@ -C checker $(checker_files)
	
clean:
	make -C checker clean; \
    make -C solution clean; \
    rm -f full-archive.tar.bz2 solution.tar.bz2 tests.tar.bz2 checker.tar.bz2 
