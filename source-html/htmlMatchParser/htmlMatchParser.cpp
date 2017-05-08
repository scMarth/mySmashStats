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


string scrollAndAppendUntilFind(string regex_expr, ifstream &infile){
   string ret = "";
   string line;
   smatch m;
   regex expr(regex_expr);
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

// get the match id
int findMatchID(smatch sm){
   smatch numm;

   for (auto x : sm){
      string match_id_str = getSubstringThatMatches("[0-9]+", (string)x);
      int match_id = 0;
      stringstream convert(match_id_str);
      convert >> match_id;
      
      return match_id;
   }
   return -1;
}

int findSeasonID(string line){
   regex expr("season_[0-9]+");
   smatch m;
   if (regex_search(line, m, expr)){

      for (auto x: m){
         string season_id_str = getSubstringThatMatches("[0-9]+", (string)x);
         int season_id = 0;
         stringstream convert(season_id_str);
         convert >> season_id;

         return season_id;
      }
   }
   return -1;
}

int findLadderNum(string line){
   regex expr("ladder_[0-9]+");
   smatch m;
   if (regex_search(line, m, expr)){

      for (auto x: m){
         string ladder_num_str = getSubstringThatMatches("[0-9]+", (string)x);
         int ladder_number = 0;
         stringstream convert(ladder_num_str);
         convert >> ladder_number;

         return ladder_number;
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
   string date = getSubstringThatMatches("title=\'(.*)\'", line);
   date = getSubstringThatMatches("\'(.*)\'", date);
   return getSubstringThatMatches("[^\']+", date);
}


int getOpponentUserID(string line){
   string userID_str = getSubstringThatMatches("user_id_[0-9]+", line);
   if (userID_str == "") return -1;
   userID_str = getSubstringThatMatches("[0-9]+", userID_str);
   if (userID_str == "") return -1;
   int user_id = 0;
   stringstream convert(userID_str);
   convert >> user_id;

   return user_id;
}


string getOpponentUsername(string line){
   string username_str = getSubstringThatMatches(">(.*)<", line);
   return getSubstringThatMatches("[^>](.*)[^<]", username_str);
}

string getOpponentMedal(ifstream &infile){
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*medal_summary(.)*tier_ladder_league_", infile);
   line = getSubstringThatMatches("tier_ladder_league_(.)*[^\']", line);
   line = getSubstringThatMatches("[^_]+\'>", line);
   return getSubstringThatMatches("[^(\'>)]+", line);
}


string getOpponentDivision(ifstream &infile){
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\'division(.)*\'(.)*>", infile);
   string line2;
   
   // find the closing div tag
   smatch divm;
   regex div_expr("</div>");

   do {
      getline(infile, line2);
      line += line2;   
   }while (!(regex_search(line2, divm, div_expr)));

   line = getSubstringThatMatches(">(.)*</", line);
   return getSubstringThatMatches("[^> ]+[^ </]", line);
}

string getOpponentPoints(ifstream &infile){
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\'points(.)*\'(.)*>", infile);
   string line2;

   // find the closing div tag
   smatch divm;
   regex div_expr("</div>");

   do {
      getline(infile, line2);
      line += line2;   
   }while (!(regex_search(line2, divm, div_expr)));

   string s = getSubstringThatMatches(">(.)*</", line);
   return getSubstringThatMatches("[^> ]+[^ </]", s);   
}

string getOpponentRawPoints(ifstream &infile){
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*raw_points(.)*\'", infile);
   string s = getSubstringThatMatches("title(.)*=(.)*\'[0-9]+(.)*[0-9]*", line);
   return getSubstringThatMatches("[0-9][^\']*", s);
}

// get opponent information
int getOpponentInfo(ifstream &infile){
   int this_person_won = 0;
   int not_friendlies = 0;

   string line = scrollUntilFind("<div(.)*class=(.*)\'(.)*opponent[^s](.*)\'(.*)>", infile);

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
   regex user_id_expr("<a(.)*class=(.)*\'(.)*username(.)*user_id_[0-9]+");

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
   return getSubstringThatMatches("[^\']+", line);
}

/*

return 1 if <div class="feedback_section"> is found
return 0 if <div class="match_scores"> is found

*/
int getFeedbackSectionDiv(ifstream &infile){
   //int count = 0;
   string line;
   smatch m;
   //regex expr("<div(.)*class(.)*=(.)*\'(.)*feedback_section");
   //regex expr("<div(.)*\'(.)*feedback_section");
   regex expr("(.)*feedback_section(.)*");
   regex end_expr("<div(.)*class(.)*=(.)*\'(.)*match_scores");

   do{
      getline(infile, line);
      //count++;
      if (regex_search(line, m, expr)) return 1;
      else if (regex_search(line, m, end_expr)){
         //cout << "count: " << count << endl;
         //cout << "found matchscores!\n";
         return 0;
      }
   }while(1);
}

/*

return 1 if <h5 class="heading">Personality</h5> is found
return 0 if <h5 class="heading">Connection</h5> is found

*/
int getFeedbackType(ifstream &infile){
   string line;
   smatch m;
   regex expr("<h5(.)*class(.)*=(.)*\'(.)*heading(.)*Personality");
   regex end_expr("<h5(.)*class(.)*=(.)*\'(.)*heading(.)*Connection");

   do{
      getline(infile, line);
      if (regex_search(line, m, expr)) return 1;
      else if (regex_search(line, m, end_expr)) return 0;
   }while(1);
}

// return string containing the username who submitted the feedback
string getFeedbackUsername(ifstream &infile){
   //string line = scrollUntilFind("div(.)*<a(.)*>(.)*</a>(.)*<span(.)*votes_text", infile);
   string line = scrollAndAppendUntilFind("div(.)*<a(.)*>(.)*</a>(.)*<span(.)*votes_text", infile);
   line = getSubstringThatMatches(">[^>]+</a>", line);
   return getSubstringThatMatches("[^><]+[^<]", line);
}


/*

return 1 if feedback is positive
return 0 if feedback is negative

*/
int isGoodFeedback(ifstream &infile){
   string line;
   smatch m;
   //regex p_expr("<i(.)*class(.)*=(.)*\'(.)*positive(.)*</i>");
   //regex n_expr("<i(.)*class(.)*=(.)*\'(.)*negative(.)*</i>");

   regex p_expr("<i(.)*positive(.)*");
   regex n_expr("<i(.)*negative(.)*");

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
         //cout << "feedbackdiv 1\n";
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
   string character = getSubstringThatMatches("title=\'[^\']+\'", line);
   character = getSubstringThatMatches("\'[^\']+\'", character);
   character = getSubstringThatMatches("[^\']+", character);
   return character;
}


string fixYoshis(string str){
   string s = getSubstringThatMatches("Yoshi&#039;s Story", str);
   if (s == "") return str;
   else return "Yoshi's Story";
}

void getStagesPlayed(ifstream &infile){
   string line;
   int divCount = 0;
   int game_count = 1;

   line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*stage_result(.)*", infile);
   divCount++;
   line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*stage_pick(.)*", infile);


   while(1){
      divCount++;
      //line = scrollUntilFind("<div(.)*data-name(.)*=(.)*\'[^\']+\'", infile);
      line = scrollUntilFind("(.)*data-name(.)*=(.)*\'[^\']+\'", infile);
      divCount++;
      string quoted_stage = getSubstringThatMatches("\'[^\']+\'", line);
      string stage = removeQuotes(quoted_stage);
      stage = fixYoshis(stage);

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
      }if (stringMatchesExpr("<div(.)*class(.)*=(.)*\'(.)*stage_pick(.)*", line)) continue;
      else die("getStagesPlayed: unknown expression");
   }
}


void getPlayerResult(ifstream &infile){
   string line;
   int divCount = 0;

   line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*team_result(.)*", infile);
   divCount++;
   line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*player_result(.)*", infile);
   divCount++;
   line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*character_pick(.)*", infile);


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
      
      getline(infile, line2);
      line += line2;

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
      }if (stringMatchesExpr("<div(.)*class(.)*=(.)*\'(.)*character_pick", line)) continue;
      else die("getPlayerResult: unknown expression");
   }
}


void getGamesFromFile(string filename){
   ifstream infile(filename);

   for (string line; getline(infile, line); ){

      // find match div
      smatch sm;
      //regex expr("<(.)*div(.)*data-match-id=\"[0-9]+\"(.)*match-information(.)*>");
      regex expr("data-match-id(.)*>");
      if (regex_search(line, sm, expr)){ // matchdiv found
         //cout << "matchdiv found!" << endl;
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
            //string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id) + "(.)*title=\"";
            //line = scrollUntilFind(dateExpr, infile);
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id);


            line = scrollUntilFind(dateExpr, infile);

            string line2;
            getline(infile, line2);
            line += line2;

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
            //string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id) + "(.)*title=\"";
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id);


            line = scrollUntilFind(dateExpr, infile);

            string line2;
            getline(infile, line2);
            line += line2;
            //cout << "line: " << line << endl;

            //line = scrollAndAppendUntilFind(dateExpr, infile);
            //cout << "line--> " << line << endl;

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
   //getGamesFromFile("../html-without-cr/loggedin_match-history-table.html");
   getGamesFromFile("../html-without-cr/head.html");
   //getGamesFromFile("../head.txt");

   //getGamesFromFile("../html-without-cr/character_fail.html");

   //getGamesFromFile("../html-without-cr/original-matchdiv.html");
   return 0;
}