CXX:= clang++
CXXFLAGS := --std=c++17 -O3 -g -Wall -Wfatal-errors -I../boost/libs/math/include/ -I../boost/libs/multiprecision/include/

all: plot_fn.x

.PHONY: plot_fn.x
plot_fn.x: main.cpp
	rm -f *.x *.svg
	$(CXX) $(CXXFLAGS) $? -o $@
	./plot_fn.x
	cat *.svg
