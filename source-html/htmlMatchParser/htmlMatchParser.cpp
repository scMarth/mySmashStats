// regex_search example
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>

using namespace std;

void die(string s){
   cerr << s << endl;
   exit(1);
}

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

/*

return the substring of 'line' that matches the expression 'regex_expr'

*/
string getStringInLineThatMatches(string regex_expr, string line){
   smatch m;
   regex expr(regex_expr);
   if (regex_search(line, m, expr)){
      for (auto x : m){
         return (string)x;
      }
   }else{
      cout << "getStringInLineThatMatches: returning null\n";
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
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\"(.)*medal_summary(.)*tier_ladder_league_", infile);

   /*
   string line;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"(.)*medal_summary(.)*tier_ladder_league_");
   do {
      getline(infile, line);
   }while (!(regex_search(line, m, expr)));
   */

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
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\"division(.)*\"(.)*>", infile);
   string line2;

   /*
   string line;
   string line2;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"division(.)*\"(.)*>");
   do {
      getline(infile, line);
   }while (!(regex_search(line, m, expr)));
   */
   
   // find the closing div tag
   smatch divm;
   regex div_expr("</div>");

   //cout << "1 debugging: size=" << line.length() << " line:\n" << line << endl;

   do {
      getline(infile, line2);
      line += line2;   
   }while (!(regex_search(line2, divm, div_expr)));


   //getline(infile, line2);
   //line = line + line2;
   //cout << "after 1 debugging: size=" << line.length() << " line:\n" << line << endl;
   //cout << "after 2 debugging: size=" << line2.length() << " line:\n" << line2 << endl;
   
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
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\"points(.)*\"(.)*>", infile);
   string line2;

   /*
   string line;
   string line2;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"points(.)*\"(.)*>");
   do {
      getline(infile, line);
   }while (!(regex_search(line, m, expr)));
   */

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
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\"(.)*raw_points(.)*\"", infile);
   /*
   string line;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"(.)*raw_points(.)*\"");
   do {
      getline(infile, line);
   }while (!(regex_search(line, m, expr)));
   */

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

   string line = scrollUntilFind("<div(.)*class=(.*)\"(.)*opponent[^s](.*)\"(.*)>", infile);

   /*
   string line;
   smatch m;
   // match an opponent div
   //regex expr("<div(.)*class=(.*)\"(.)*opponent(.*)opponent_(.)*\"(.*)>");
   regex expr("<div(.)*class=(.*)\"(.)*opponent[^s](.*)\"(.*)>");

   do {
      getline(infile, line); // go to the next line until the regex is matched
   }while (!(regex_search(line, m, expr)));
   */

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
   
   // somehow this messes up the (stack?)
   //line = scrollUntilFind("<a(.)*class=(.)*\"(.)*username(.)*user_id_[0-9]+", infile);

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

string removeQuotes(string line){
   smatch m;
   regex expr("[^\"]+");
   if (regex_search(line, m, expr)){
      for (auto x : m){
         return (string)x;
      }
   }
}








/*

return 1 if <div class="feedback_section"> is found
return 0 if <div class="match_scores"> is found

*/
int getFeedbackSectionDiv(ifstream &infile){
   string line;
   smatch m;
   regex expr("<div(.)*class(.)*=(.)*\"(.)*feedback_section");
   regex end_expr("<div(.)*class(.)*=(.)*\"(.)*match_scores");

   do{
      getline(infile, line);
      if (regex_search(line, m, expr)) return 1;
      else if (regex_search(line, m, end_expr)) return 0;
   }while(1);
}

/*

return 1 if <h5 class="heading">Personality</h5> is found
return 0 if <h5 class="heading">Connection</h5> is found

*/
int getFeedbackType(ifstream &infile){
   string line;
   smatch m;
   regex expr("<h5(.)*class(.)*=(.)*\"(.)*heading(.)*Personality");
   regex end_expr("<h5(.)*class(.)*=(.)*\"(.)*heading(.)*Connection");

   do{
      getline(infile, line);
      if (regex_search(line, m, expr)) return 1;
      else if (regex_search(line, m, end_expr)) return 0;
   }while(1);
}

// return string containing the username who submitted the feedback
string getFeedbackUsername(ifstream &infile){
   string line = scrollUntilFind("div(.)*<a(.)*>(.)*</a>(.)*<span(.)*votes_text", infile);
   smatch m;
   regex expr(">[^>]+</a>");
   if (regex_search(line, m, expr)){
      for (auto x : m){
         regex expr2("[^><]+[^<]");
         if (regex_search((string)x, m, expr2)){
            for (auto y : m){
               return (string)y;
            }
         }
      }
   }
   return "";
}




/*

return 1 if feedback is positive
return 0 if feedback is negative

*/
int isGoodFeedback(ifstream &infile){
   string line;
   smatch m;
   regex p_expr("<i(.)*class(.)*=(.)*\"(.)*positive(.)*</i>");
   regex n_expr("<i(.)*class(.)*=(.)*\"(.)*negative(.)*</i>");

   do{
      getline(infile, line);
      if (regex_search(line, m, p_expr)) return 1;
      else if (regex_search(line, m, n_expr)) return 0;
   }while(1);
}

void getPlayerFeedback(ifstream &infile){
   while(1){
      int feedbackdiv = getFeedbackSectionDiv(infile);
      //cout << "getPlayerFeedback: got the int: " << feedbackdiv << endl;
      if (feedbackdiv){
         int personalityFeedback = getFeedbackType(infile);
         string username = getFeedbackUsername(infile);
         int feedbacktype = isGoodFeedback(infile);
         cout << username << " submitted ";
         if (personalityFeedback == 1) // personality feedback
            cout << "personality";
         else // connection feedback
            cout << "connection";
         cout << ": thumbs ";
         if (feedbacktype == 1) cout << "up";
         else cout << "down";
         cout << endl;
      }else return;
   }
}


/*
void getStagesPlayed(ifstream &infile){
   int stage_num = 1;
   while(1){

      string line;
      smatch m;
      smatch end_m;
      regex expr("<div(.)*class(.)*=(.)*\"(.)*stage_pick");
      regex end_expr("<div(.)*class(.)*=(.)*\"(.)*character_pick");

      getline(infile, line);
      if (regex_search(line, end_m, end_expr)){
         cout << "stages: BREAKING\n";
         break;
      }

      if (regex_search(line, m, expr)){
         smatch m2;
         regex expr2("<div(.)*data-name(.)*=(.)*\"[^\"]+\"");
         do {
            getline(infile, line);
         }while (!(regex_search(line, m2, expr2)));

         for (auto x : m2){
            cout << "Stages x: " << (string)x << endl;
            smatch m3;
            regex expr3("\"[^\"]+\"");
            if (regex_search((string)x, m3, expr3)){
               for (auto y : m3){
                  cout << "Stages y: " << (string)y << endl;
                  smatch m4;
                  //regex expr4("[^\"]+");
                  regex expr4("\"[^\"]+\"");
                  if (regex_search((string)y, m4, expr3)){
                     for (auto y2 : m4){
                        // this messes up... am I blowing up the call stack?
                        cout << "Stages y2: " << (string)y2 << endl;
                     }
                  }else{
                     cout << "not found!!\n";
                  }
               }
            }
            break;
         }
      }
   }
   return;
}
*/

/*

return 1 if character was victorious
return 0 if character was defeated

*/
int characterVictorious(string line){
   smatch m;
   regex d_expr("defeated");
   regex v_expr("victorious");

   if (regex_search(line, m, v_expr))
      return 1;
   else if (regex_search(line, m, d_expr))
      return 0;
   else die("characterVictorious: couldn't find 'victorious' or 'defeated'");
}

string getCharacterPlayed(string line){
   /*
   smatch m;
   regex expr("title=\"[^\"]+\"");


   if (regex_search(line, m, expr)){
      for (auto x : m){
         regex expr2("\"[^\"]+\"");
      }
   }
   */

   string character = getStringInLineThatMatches("title=\"[^\"]+\"", line);
   character = getStringInLineThatMatches("\"[^\"]+\"", character);
   character = getStringInLineThatMatches("[^\"]+", character);
   return character;
}

void getStagesPlayed(ifstream &infile){
   string line;
   int divCount = 0;
   int game_count = 1;

   line = scrollUntilFind("<div(.)*class(.)*=(.)*\"(.)*stage_result(.)*", infile);
   divCount++;
   line = scrollUntilFind("<div(.)*class(.)*=(.)*\"(.)*stage_pick(.)*", infile);


   while(1){
      divCount++;
      line = scrollUntilFind("<div(.)*data-name(.)*=(.)*\"[^\"]+\"", infile);
      divCount++;
      string quoted_stage = getStringInLineThatMatches("\"[^\"]+\"", line);
      string stage = removeQuotes(quoted_stage);
      cout << "Game " << game_count++ << ": " << stage << endl;

      line = scrollUntilFind("(.)*</div>(.)*", infile);
      if (stringMatchesExpr("(.)*</div>(.)*</div>(.)*", line))
         // two closing divs found
         divCount -= 2;
      else
         // one closing div
         divCount--;

      // get the next line
      getline(infile, line);

      if (stringMatchesExpr("(.)*</div>(.)*", line)){
         divCount--;
         //cout << "-->getStagesPlayed: returning from while loop\n";
         //cout << "divCount = " << divCount << endl;
         //cout << "the line:\n\n";
         //cout << line << endl << endl;
         if (divCount == 0) return; //closed off all opening div tags with closing div tags
         else die("getStagesPlayed: number of opening div tags don't match with number of closing div tags");
      }if (stringMatchesExpr("<div(.)*class(.)*=(.)*\"(.)*stage_pick(.)*", line)) continue;
      else die("getStagesPlayed: unknown expression");
   }
}


void getPlayerResult(ifstream &infile){
   string line;
   int divCount = 0;

   line = scrollUntilFind("<div(.)*class(.)*=(.)*\"(.)*team_result(.)*", infile);
   divCount++;
   line = scrollUntilFind("<div(.)*class(.)*=(.)*\"(.)*player_result(.)*", infile);
   divCount++;
   line = scrollUntilFind("<div(.)*class(.)*=(.)*\"(.)*character_pick(.)*", infile);


   while(1){
      divCount++;
      //cout << "-->getPlayerResult: while loop\n";
      string line2;
      getline(infile, line2);
      line += line2;
      int wasVictorious = characterVictorious(line);

      //line = scrollUntilFind("(.)*character(.)*character_for_game_melee", infile);
      line = scrollUntilFind("(.)*character(.)*character_for_game_melee(.)*", infile);
      //cout << "-->getPlayerResult: while loop line:\n" << line << endl << endl;
      divCount++;
      string character = getCharacterPlayed(line);
      cout << character << " (";
      if (wasVictorious) cout << "VICTORIOUS";
      else cout << "DEFEATED";
      cout << ")\n";

      line = scrollUntilFind("(.)*</div>(.)*", infile);
      if (stringMatchesExpr("(.)*</div>(.)*</div>(.)*", line))
         // two closing divs found
         divCount -= 2;
      else
         // one closing div
         divCount--;
      
      getline(infile, line); // get the next line

      if (stringMatchesExpr("(.)*</div>(.)*", line)){
         divCount--;

         getline(infile, line); // get the next line
         if (stringMatchesExpr("(.)*</div>(.)*", line)){
            divCount--;
            //cout << "-->getPlayerResult: returning from while loop\n";
            //cout << "divCount = " << divCount << endl;
            //cout << "the line:\n\n";
            //cout << line << endl << endl;
            //return;
            if (divCount == 0) return; //closed off all opening div tags with closing div tags
            else die("getPlayerResult: number of opening div tags don't match with number of closing div tags");
         }else die("getPlayerResult: number of opening div tags don't match with number of closing div tags");
      }if (stringMatchesExpr("<div(.)*class(.)*=(.)*\"(.)*character_pick", line)) continue;
      else die("getPlayerResult: unknown expression");
   }
}



/*
void getStagesPlayed(ifstream &infile){
   int game_num = 1;

   while(1){
      string stagedivstr = getStageDiv(infile);
      if (stagedivstr == "DONE") break;
      else{
         string quoted = getStageName(stagedivstr);
         cout << "Game " << game_num++ << ": " << removeQuotes(quoted) << endl;
      }
   }
}
*/

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

            
            // somehow this messes up the (stack?)
            // restarted comp, different day and all of a sudden this works..
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id) + "(.)*title=\"";
            line = scrollUntilFind(dateExpr, infile);
            

            /*            
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id) + "(.)*title=\"";
            regex date_expr(dateExpr);
            smatch sm2;
            
            do {
               getline(infile, line); // go to the next line
            }while (!(regex_search(line, sm2, date_expr)));
            */

            // date_expr should've been found by now
            string date = getDate(line);
            cout << "Date: " << date << endl;

            cout << "\n[RANKED MATCH]\n";

            // get opponent information
            getOpponentInfo(infile);

            // get opponent information
            getOpponentInfo(infile);

            // get player feedback
            getPlayerFeedback(infile);

            cout << "\nStages:\n";
            getStagesPlayed(infile);

            cout << "\nP1 Characters:\n";
            getPlayerResult(infile);

            cout << "\nP2 Characters:\n";
            getPlayerResult(infile);


            cout << endl << 
            "========================================================="
            << endl << endl;


         }else{
            cout << "The match was NOT ranked.\n";

            // for some reason, this works here but not in the above (ranked) case
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id) + "(.)*title=\"";
            line = scrollUntilFind(dateExpr, infile);

            /*
            // get the date of the match
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id) + "(.)*title=\"";
            regex date_expr(dateExpr);
            smatch sm2;
            
            do {
               getline(infile, line); // go to the next line
            }while (!(regex_search(line, sm2, date_expr)));
            */

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

   // works so far (without carriage returns)
   getGamesFromFile("../html-without-cr/loggedin_match-history-table.html");

   //getGamesFromFile("../html-without-cr/character_fail.html");

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