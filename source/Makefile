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

secondary: lookaheadparser.h
	$(CC) -o regex_example regex_example.cpp
	$(CC) -o regex_match_example regex_match_example.cpp
	$(CC) -o concat concat.cpp
	$(CC) -o combineJSONMatches combineJSONMatches.cpp
	gcc -o removecrlf removecrlf.c

update: lookaheadparser.h
	[ ! -f ../json/match-history/all-hist.json ] && cp ../json/match-history/updated-hist.json ../json/match-history/all-hist.json
	make gethtml
	make go
	$(CC) -o combineJSONMatches combineJSONMatches.cpp
	./combineJSONMatches
	echo "var HistoryJSON = " > ../dist/js/history.js
	cat ../json/match-history/all-hist.json >> ../dist/js/history.js

testregex:
	$(CC) -o testRegex testRegexStuff.cpp

clean:
	- rm $(exec) *.o *.gch

go:
	./htmlMatchParser > catchoutput

diff:
	diff -y catchoutput catchoutput.old
