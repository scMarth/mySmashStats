call "source\curl" "https://www.smashladder.com/player/178230/illlilliillillli/match-history" > "html\updatedHistories\updatedHistory.html"
copy "html\updatedHistories\updatedHistory.html" "html\updatedHistories\RAW.html"
call "source\tidy" --drop-empty-elements no "html\updatedHistories\updatedHistory.html" > "html\updatedHistories\temp.html" 2>NUL
del "html\updatedHistories\updatedHistory.html"
rem PAUSE
rename "html\updatedHistories\temp.html" "updatedHistory.html"
rem PAUSE
call "source\removecr" "html\updatedHistories\updatedHistory.html" "html\updatedHistories\temp.html"
del "html\updatedHistories\updatedHistory.html"
rename "html\updatedHistories\temp.html" "updatedHistory.html"
rem PAUSE

rem call "source\htmlMatchParser" > "source\catchoutput"
cd source
call htmlMatchParser > catchoutput
cd ..

if exist "json\match-history\all-hist.json" (
	echo "all-hist.json detected"
) else (
	if exist "json\match-history\all-hist.json" del "json\match-history\all-hist.json"
	PAUSE
	copy "json\match-history\updated-hist.json" "json\match-history\all-hist.json"
)

cd source
call combineJSONMatches
cd ..

echo var HistoryJSON =  > "dist\js\history.js"
type "json\match-history\all-hist.json" >> "dist\js\history.js"

call "dist\mySmashStats.html"



PAUSE