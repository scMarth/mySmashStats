// regex_search example
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>

using namespace std;

void die(string s){
   cerr << s;
   exit(1);
}
// get the match id
int findMatchID(smatch sm){
   smatch numm;

   for (auto x : sm){
      string match_id_search = (string)x;
      regex num_expr("[0-9]+");
      if (regex_search(match_id_search, numm, num_expr)){
         for (auto y : numm){
            int match_id = 0;
            stringstream convert((string)y);
            convert >> match_id;

            return match_id;
         }
      }
   }
   return -1;
}

int findSeasonID(string line){
   regex expr("season_[0-9]+");
   smatch m;
   if (regex_search(line, m, expr)){

      for (auto x: m){
         string season_id_search = (string)x;
         regex sID("[0-9]+");
         smatch n;
         if (regex_search(season_id_search, n, sID)){
            for (auto y : n){
               int season_id = 0;
               stringstream convert((string)y);
               convert >> season_id;

               return season_id;
            }
         }
      }
   }
   return -1;
}

int findLadderNum(string line){
   regex expr("ladder_[0-9]+");
   smatch m;
   if (regex_search(line, m, expr)){

      for (auto x: m){
         string ladder_num_search = (string)x;
         regex num("[0-9]+");
         smatch n;
         if (regex_search(ladder_num_search, n, num)){
            for (auto y : n){
               int ladder_number = 0;
               stringstream convert((string)y);
               convert >> ladder_number;

               return ladder_number;
            }
         }
      }
   }
   return -1;
}

/*

return   -1     if no result was found
return   0      if match-loss is found
return   1      if match-win is found

*/
int getMatchResult(string line){
   //cout << "called getMatchResult on line: " << line << endl;

   // search for loss class
   smatch m1;
   regex expr1("match-loss");
   if (regex_search(line, m1, expr1)) return 0;

   // search for win class
   smatch m2;
   regex expr2("match-win");
   if (regex_search(line, m2, expr2)) return 1;

   return -1;
}

/*

return   1      if match_ranked is found
return   0      otherwise

*/
int isRanked(string line){
   smatch m;
   regex expr("match_ranked");
   if (regex_search(line, m, expr)) return 1;
   else return 0;
}

string getDate(string line){
   smatch m;
   regex expr("title=\"(.*)\"");
   if (regex_search(line, m, expr)){
      for (auto x: m){
         string date_search = (string)x;
         smatch n;
         regex end_expr("[^\"]+");
         if (regex_search(date_search, n, end_expr)){
            for (auto y : n){
               if (((string)y).find("title") != string::npos) continue;
               return (string)y;
            }
         }
      }
   }
}


int getOpponentUserID(string line){
   smatch m;
   regex expr("user_id_[0-9]+");
   if (regex_search(line, m, expr)){
      for (auto x : m){
         string user_id_search = (string)x;
         smatch n;
         regex id_expr("[0-9]+");
         if (regex_search(user_id_search, n, id_expr)){
            for (auto y : n){
               int user_id = 0;
               stringstream convert((string)y);
               convert >> user_id;

               return user_id;
            }
         }
      }
   }
   return -1;
}


string getOpponentUsername(string line){
   smatch m;
   regex expr(">(.*)<");
   if (regex_search(line, m, expr)){
      for (auto x : m){
         string username_search = (string)x;
         smatch n;
         regex un_expr("[^>](.*)[^<]");
         if (regex_search(username_search, n, un_expr)){
            for (auto y : n){
               return (string)y;
            }
         }
      }
   }
   return "";
}

string getOpponentMedal(ifstream &infile){
   string line;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"(.)*medal_summary(.)*tier_ladder_league_");
   do {
      getline(infile, line);
   }while (!(regex_search(line, m, expr)));

   smatch m2;
   regex expr2("tier_ladder_league_(.)*[^\"]");
   if (regex_search(line, m2, expr2)){
      for (auto x : m2){
         smatch m3;
         regex expr3("[^_]+\">");
         if (regex_search((string)x, m3, expr3)){
            for (auto y : m3){
               //cout << "debug: y = " << y << endl;               
               smatch m4;
               regex expr4("[^(\">)]+");
               if (regex_search((string)y, m4, expr4)){
                  for (auto z : m4){
                     return (string)z;
                  }
               }
            }
         }
      }
   }
   return "";
}


string getOpponentDivision(ifstream &infile){
   string line;
   string line2;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"division(.)*\"(.)*>");
   do {
      getline(infile, line);
   }while (!(regex_search(line, m, expr)));
   
   // find the closing div tag
   smatch divm;
   regex div_expr("</div>");

   do {
      getline(infile, line2);
      line += line2;   
   }while (!(regex_search(line2, divm, div_expr)));

   //getline(infile, line2);
   //line = line + line2;
   //cout << "1 debugging: size=" << line.length() << " line:\n" << line << endl;
   //cout << "2 debugging: size=" << line2.length() << " line:\n" << line2 << endl;
   
   //cout << "1 debugging: size=" << line.length() << " line:\n" << line << endl;

   smatch m2;
   regex expr2(">(.)*</");
   if (regex_search(line, m2, expr2)){
      for (auto x : m2){
         smatch m3;
         regex expr3("[^> ]+[^ </]");
         if (regex_search((string)x, m3, expr3)){
            for (auto y : m3){
               return (string)y;
            }
         }
      }
   }

   return "";
}

string getOpponentPoints(ifstream &infile){
   string line;
   string line2;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"points(.)*\"(.)*>");
   do {
      getline(infile, line);
   }while (!(regex_search(line, m, expr)));

   // find the closing div tag
   smatch divm;
   regex div_expr("</div>");

   do {
      getline(infile, line2);
      line += line2;   
   }while (!(regex_search(line2, divm, div_expr)));

   smatch m2;
   regex expr2(">(.)*</");
   if (regex_search(line, m2, expr2)){
      for (auto x : m2){
         smatch m3;
         regex expr3("[^> ]+[^ </]");
         if (regex_search((string)x, m3, expr3)){
            for (auto y : m3){
               return (string)y;
            }
         }
      }
   }

   return "";      
}

string getOpponentRawPoints(ifstream &infile){
   string line;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"(.)*raw_points(.)*\"");
   do {
      getline(infile, line);
   }while (!(regex_search(line, m, expr)));

   //cout << "getOpponentRawPoints line: " << line << endl;

   smatch m2;
   regex expr2("title(.)*=(.)*\"[0-9]+(.)*[0-9]*");
   if (regex_search(line, m2, expr2)){
      for (auto x : m2){

         //cout << "getOpponentRawPoints x: " << (string)x << endl;

         smatch m3;
         regex expr3("[0-9][^\"]*");
         if (regex_search((string)x, m3, expr3)){
            for (auto y : m3){
               return (string)y;
            }
         }
      }
   }
   return "";
}

// get opponent information
int getOpponentInfo(ifstream &infile){
   int this_person_won = 0;
   int not_friendlies = 0;

   string line;
   smatch m;
   // match an opponent div
   //regex expr("<div(.)*class=(.*)\"(.)*opponent(.*)opponent_(.)*\"(.*)>");
   regex expr("<div(.)*class=(.*)\"(.)*opponent[^s](.*)\"(.*)>");

   do {
      getline(infile, line); // go to the next line until the regex is matched
   }while (!(regex_search(line, m, expr)));

   // expr should've been found by now
   if (line.find("opponent_lost") != string::npos){
      //cout << "--> case one\n";
      this_person_won = 0;
      not_friendlies = 1;
   }else if (line.find("opponent_won") != string::npos){
      //cout << "--> case two\n";
      this_person_won = 1;
      not_friendlies = 1;
   }

   cout << endl;
   if (not_friendlies == 1){
      //cout << "[RANKED MATCH]\n";
      if (this_person_won == 1) cout << "WINNER:\n";
      else cout << "LOSER:\n";      
   }//else cout << "[FRIENDLIES]\n";

   // get this opponent's user id
   smatch m0;
   regex user_id_expr("<a(.)*class=(.)*\"(.)*username(.)*user_id_[0-9]+");

   do {
      getline(infile, line);
   }while (!(regex_search(line, m0, user_id_expr)));

   //cout << "Calling getOpponentUserID\n";
   int opponent_user_id = getOpponentUserID(line);
   if (opponent_user_id == -1) die("getOpponentInfo: opponent_user_id = -1\n");
   cout << "User ID:" << opponent_user_id << "\n";

   // get this opponent's username
   string opponent_username = getOpponentUsername(line);
   if (opponent_username == "") die("getOpponentUsername failed\n");
   else cout << "Username: " << opponent_username << endl;

   // get this opponent's medal
   string oppponent_medal = getOpponentMedal(infile);
   if (oppponent_medal == "") die("getOpponentMedal failed\n");
   else cout << "Medal: " << oppponent_medal << endl;

   // get this opponent's division
   string opponent_division = getOpponentDivision(infile);
   if (opponent_division == "") die("getOpponentDivision failed\n");
   else cout << "Division: " << opponent_division << endl;

   // get this opponent's points
   int opponent_unranked = 0;
   int opp_points_num = 0;

   string opponent_points = getOpponentPoints(infile);
   if (opponent_points == "") opponent_unranked = 1;
   else{   
      stringstream convert(opponent_points);
      convert >> opp_points_num;
   }
   if (opponent_unranked) cout << "Points: (UNRANKED)\n";
   else cout << "Points " << opp_points_num << endl;

   // get this opponents raw points
   string opponent_raw_points = getOpponentRawPoints(infile);
   cout << "Raw Points: " << opponent_raw_points << endl;
}


void getStagesPlayed(ifstream &infile){
   // while(1){
   //    string line;
   //    smatch m;
   //    regex expr("");


   // }
}


void getGamesFromFile(string filename){
   ifstream infile(filename);

   for (string line; getline(infile, line); ){

      // find match div
      smatch sm;
      regex expr("<(.)*div(.)*data-match-id=\"[0-9]+\"(.)*match-information(.)*>");
      if (regex_search(line, sm, expr)){ // matchdiv found

         // get the match ID
         int match_id = findMatchID(sm);
         if (match_id == -1) die("Match ID wasn't found.\n");
         else cout << "The match id is: " << match_id << "\n";

         // get the season ID
         int season_id = findSeasonID(line);
         if (season_id == -1) die("Season ID wasn't found.\n");
         else cout << "The season id is: " << season_id << endl;

         // get the ladder number
         int ladder_num = findLadderNum(line);
         if (ladder_num == -1) die("Ladder number wasn't found.\n");
         else cout << "The ladder number is: " << ladder_num << endl;

         // find out if the match is ranked or not
         int ranked = isRanked(line);
         if (ranked){
            // cout << "The match was ranked.\n";

            // find the result of the mass (win or loss)
            int match_result = getMatchResult(line);
            if (match_result == -1) die("Match result wasn't found.\n");
            else if (match_result == 0) cout << "Result: LOSS\n";
            else if (match_result == 1) cout << "Result: WIN\n";

            // get the date of the match
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id) + "(.)*title=\"";
            regex date_expr(dateExpr);
            smatch sm2;
            
            do {
               getline(infile, line); // go to the next line
            }while (!(regex_search(line, sm2, date_expr)));

            // date_expr should've been found by now
            string date = getDate(line);
            cout << "Date: " << date << endl;

            cout << "\n[RANKED MATCH]\n";

            // get opponent information
            getOpponentInfo(infile);

            // get opponent information
            getOpponentInfo(infile);
            cout << endl << 
            "========================================================="
            << endl << endl;

            getStagesPlayed(infile);
         }else{
            cout << "The match was NOT ranked.\n";  

            // get the date of the match
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id) + "(.)*title=\"";
            regex date_expr(dateExpr);
            smatch sm2;
            
            do {
               getline(infile, line); // go to the next line
            }while (!(regex_search(line, sm2, date_expr)));

            // date_expr should've been found by now
            string date = getDate(line);
            cout << "Date: " << date << endl;

            cout << "\n[FRIENDLIES]\n";

            // get opponent information
            getOpponentInfo(infile);

            // get opponent information
            getOpponentInfo(infile);

            cout << endl << 
            "========================================================="
            << endl << endl;
         }
      }
   }
}



int main(){

   // fails... is this due to windows crs? lol
   //getGamesFromFile("../old-html-with-cr/original-matchdiv.html");

   getGamesFromFile("../html-without-cr/loggedin_match-history-table.html");

   //getGamesFromFile("../html-without-cr/original-matchdiv.html");

   return 0;
}


/*

still need to find:

line 9:             <span class="result_type type">Singles</span>

line 29: opponent_lost
line 30: username
   division
   medal
   points
   raw points

line 42: (again)



line 61: stage result:
   stage 1
   stage 2
   stage 3


characters (top line)

characters (bottom line)

*/