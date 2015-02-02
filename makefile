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

archive: misspell.tar.bz2
solution_files := bigrams.txt language_model.cpp language_model.h makefile misspell.cpp string_iterator.h unigrams.txt words_splitter.h
checker_files := checker.cpp makefile testlib.h
test_files := $(shell ls tests) 
archive_files := $(patsubst %, solution/%, $(solution_files)) $(patsubst %, checker/%, $(checker_files)) $(patsubst %, tests/%, $(test_files))
misspell.tar.bz2: $(archive_files)
	tar cjf $@ $(archive_files)
	
clean:
	make -C checker clean; \
    make -C solution clean; \
    rm misspell.tar.bz2
