CC = g++ -std=c++11
exec = htmlMatchParser regex_example regex_match_example concat removecrlf getUpdatedHistoryHTML testRegex
Topts = --drop-empty-elements no

all: errorStuff.h regexStuff.h
	$(CC) -o htmlMatchParser htmlMatchParser.cpp

gethtml: errorStuff.h regexStuff.h
	# Make the exec
	$(CC) -o getUpdatedHistoryHTML getUpdatedHistoryHTML.cpp -lcurl
	# retrieve the html
	./getUpdatedHistoryHTML
	# clean the retrieved html
	- cp ../html/updatedHistories/updatedHistory.html ../html/updatedHistories/RAW.html
	- ./tidy $(Topts)  ../html/updatedHistories/updatedHistory.html > ../html/updatedHistories/temp.html 2>/dev/null
	- rm ../html/updatedHistories/updatedHistory.html
	- rename 's/temp/updatedHistory/' ../html/updatedHistories/temp.html 

secondary:
	$(CC) -o regex_example regex_example.cpp
	$(CC) -o regex_match_example regex_match_example.cpp
	$(CC) -o concat concat.cpp
	gcc -o removecrlf removecrlf.c

testregex:
	$(CC) -o testRegex testRegexStuff.cpp

clean:
	rm $(exec)

go:
	./htmlMatchParser > catchoutput

diff:
	diff -y catchoutput catchoutput.old
