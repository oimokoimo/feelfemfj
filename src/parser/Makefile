CXX      = g++
CXXFLAGS = -std=c++20 -Wall -O2
FLEX     = flex
BISON    = bison

TARGET = feelfem

SRCS = feelfem.tab.c lex.yy.c

all: $(TARGET)

feelfem.tab.c feelfem.tab.h: feelfem.y
	$(BISON) -d -Wcounterexamples feelfem.y

lex.yy.c: feelfem.lex feelfem.tab.h
	$(FLEX) feelfem.lex

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) feelfem.tab.c feelfem.tab.h lex.yy.c
