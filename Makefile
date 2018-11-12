CXX:= g++-8
CXXFLAGS := --std=gnu++17 -O3 -g -Wall -Wfatal-errors  -fsanitize=undefined -fsanitize=address
INCFLAGS := -I./include -I../boost/libs/math/include/ -I../boost/libs/multiprecision/include/ -I../boost/libs/config/include -I../boost/libs/mpl/include -I/usr/local/include
PREFIX = /usr/local


all: plot_fn.x

.PHONY: plot_fn.x
plot_fn.x: lambertw_ulp.cpp
	rm -rf *.x *.svg *.x.dSYM examples/*.svg
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $? -o $@ -lquadmath
	./plot_fn.x

.PHONY: clean
clean:
	rm -rf *.x *.svg *.x.dSYM examples/*.svg


install:
	install -m 0644 include/quicksvg/*.hpp $(PREFIX)/include/quicksvg
