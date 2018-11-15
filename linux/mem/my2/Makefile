files = dummy_main.cpp env.cpp fork.cpp globals.cpp hooks.cpp ptmalloc.cpp

malloc.a: $(files)
	g++ -o dummy_main -fmax-errors=1 -w -std=c++1y $(files)
