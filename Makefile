CXX:= clang++
CXXFLAGS := -O3 --std=gnu++17 -g -Wall -Wfatal-errors  -fsanitize=undefined -fsanitize=address
INCFLAGS := -I./include -I../boost/libs/math/include/ -I../boost/libs/multiprecision/include/ -I../boost/libs/config/include -I../boost/libs/mpl/include -I../boost/libs/core/include -I/usr/local/include
PREFIX = /usr/local


all: plot_fn.x

.PHONY: plot_fn.x
plot_fn.x: lambertw_ulp.cpp
	rm -rf *.x *.svg *.x.dSYM examples/*.svg
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $? -o $@
	./plot_fn.x

.PHONY: test.x
test.x: test/test.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $? -o $@ -L/usr/local/lib -lgtest -pthread -lgtest_main
	./test.x


.PHONY: clean
clean:
	rm -rf *.x *.svg *.x.dSYM examples/*.svg



install:
	mkdir -p $(PREFIX)/include/quicksvg
	mkdir -p $(PREFIX)/include/quicksvg/detail
	install -m 0644 include/quicksvg/graph_fn.hpp include/quicksvg/ulp_plot.hpp include/quicksvg/plot_time_series.hpp $(PREFIX)/include/quicksvg
	install -m 0644 include/quicksvg/detail/generic_svg_functionality.hpp $(PREFIX)/include/quicksvg/detail/
