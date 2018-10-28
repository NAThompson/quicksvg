CXX:= clang++
CXXFLAGS := --std=c++17 -O3 -g -Wall -Wfatal-errors  -fsanitize=undefined -fsanitize=address
INCFLAGS := -I./include -I../boost/libs/math/include/ -I../boost/libs/multiprecision/include/


all: plot_fn.x

.PHONY: plot_fn.x
plot_fn.x: main.cpp
	rm -rf *.x *.svg *.x.dSYM examples/*.svg
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $? -o $@
	./plot_fn.x
	cat examples/*.svg

.PHONY: clean
clean:
	rm -rf *.x *.svg *.x.dSYM
