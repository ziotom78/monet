CXX := c++
CPPFLAGS = -I../include
CXXFLAGS = -g -Wall

EXAMPLE_FILES = \
	tut01.svg \
	tut02.svg \
	tut02-bad.svg \
	tut03.svg \
	tut03-no-saturation.svg \
	tut04.svg \
	tut04-fill.svg \
	tut04-fillandstroke.svg \
	tut04-closepath.svg \
	tut05.svg \
	tut06.svg \
	tut07.svg \
	tut08.svg \
	tut09.svg \
	ref-color.txt \
	ref-color-table.svg \
	ref-point.txt \
	ref-fonts.svg

.phony: all clean

all: $(EXAMPLE_FILES)

clean:
	rm -f $(EXAMPLE_FILES)

%.svg: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< && ./a.out && rm -f ./a.out

%.txt: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< && ./a.out > $@ && rm -f ./a.out
