#COMPILE_OPTS := "-DDEBUG=1"

checker: full-checker.cpp
	g++ -g ${COMPILE_OPTS} full-checker.cpp -O3 -o $@

full-checker.cpp: checker.cpp testlib.h utf8.h utf8/core.h utf8/checked.h utf8/unchecked.h
	rm -f $@; (\
        find . -regex '.*\.\(cpp\|h\)' -exec sed -n '/#if !defined(HEADERS_GENERATION_GUARD)/,/#endif/p' {} \; ;\
        g++ -P -E -DHEADERS_GENERATION_GUARD=1 $<\
    ) > $@

clean:
	rm -f checker full-checker.cpp
