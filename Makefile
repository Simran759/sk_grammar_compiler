CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = skc


LEX_GEN_SRC = lex.yy.cc
PARSER_GEN_SRC = parser.tab.cpp
PARSER_GEN_HDR = parser.tab.hpp


SRCS = main.cpp codegen.cpp $(PARSER_GEN_SRC) $(LEX_GEN_SRC)
OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(PARSER_GEN_SRC) $(PARSER_GEN_HDR): parser.ypp
	bison -d -o $(PARSER_GEN_SRC) parser.ypp

$(LEX_GEN_SRC): lexer.l $(PARSER_GEN_HDR) scanner.hpp
	flex -o $(LEX_GEN_SRC) lexer.l

main.o: main.cpp $(PARSER_GEN_HDR) scanner.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

codegen.o: codegen.cpp $(PARSER_GEN_HDR) ast.hpp
	$(CXX) $(CXXFLAGS) -c codegen.cpp -o codegen.o

parser.tab.o: $(PARSER_GEN_SRC) $(PARSER_GEN_HDR)
	$(CXX) $(CXXFLAGS) -c $(PARSER_GEN_SRC) -o parser.tab.o

lex.yy.o: $(LEX_GEN_SRC) $(PARSER_GEN_HDR)
	$(CXX) $(CXXFLAGS) -c $(LEX_GEN_SRC) -o lex.yy.o

clean:
	rm -f $(TARGET) $(OBJS) $(PARSER_GEN_SRC) $(PARSER_GEN_HDR) $(LEX_GEN_SRC)