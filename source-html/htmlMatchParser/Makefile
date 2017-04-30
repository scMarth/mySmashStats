exec = htmlMatchParser regex_example regex_match_example concat removecrlf

all:
	g++-4.9 -std=c++11 -o htmlMatchParser htmlMatchParser.cpp

secondary:
	g++-4.9 -std=c++11 -o regex_example regex_example.cpp
	g++-4.9 -std=c++11 -o regex_match_example regex_match_example.cpp
	g++-4.9 -std=c++11 -o concat concat.cpp
	gcc -o removecrlf removecrlf.c

clean:
	rm $(exec)

go:
	./htmlMatchParser > catchoutput
