#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>

using namespace std;

/*

Advance to the next line in 'infile' until the pattern provided in
'regex_expr' is found. For example, 'regex_expr' can be "[^\"](.)*[a-z]+"

Return the line found, if the pattern is matched.
Return "" otherwise.

*/

string scrollUntilFind(string regex_expr, ifstream &infile){
   string line;
   smatch m;
   regex expr(regex_expr);
   while (getline(infile, line)){
      if (regex_search(line, m, expr)) return line;
   }
   return "";
}

string scrollAndAppendUntilFind(string regex_expr, ifstream &infile, string initStr){
   string ret = initStr;
   string line;
   smatch m;
   regex expr(regex_expr);
   if (regex_search(ret, m, expr)) return ret; // does the initial String already match the expression?
   while (getline(infile, line)){
      ret += line;
      if (regex_search(ret, m, expr)) return ret;
   }
   return "";
}

/*

return the substring of 'line' that matches the expression 'regex_expr'

*/
string getSubstringThatMatches(string regex_expr, string str){
   smatch m;
   regex expr(regex_expr);
   if (regex_search(str, m, expr)){
      for (auto x : m){
         return (string)x;
      }
   }else{
      //cout << "getSubstringThatMatches: returning null\n";
      return "";
   }
}

/*

return 1 if string 'line' matches the expression 'regex_expr'
return 0 otherwise

*/
int stringMatchesExpr(string regex_expr, string line){
   smatch m;
   regex expr(regex_expr);
   if (regex_search(line, m, expr)) return 1;
   else return 0;
}
