#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
// ./source
#include "errorStuff.h"
#include "regexStuff.h"
// rapidjson
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;


// Global Var
StringBuffer s;
Writer<StringBuffer> writer(s);

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
   string username_str = getSubstringThatMatches("smashladder(.)*>(.*)</a>", line);
   username_str = getSubstringThatMatches(">(.)*<", username_str);
   return getSubstringThatMatches("[^>](.*)[^<]", username_str);
}

string getOpponentMedal(ifstream &infile){
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*medal_summary(.)*tier_ladder_league_", infile);
   line = getSubstringThatMatches("tier_ladder_league_(.)*[^\']", line);
   line = getSubstringThatMatches("[^_]+\'>", line);
   return getSubstringThatMatches("[^(\'>)]+", line);
}


string getOpponentDivision(ifstream &infile){
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*division(.)*", infile);
   //cout << "getOpponentDivision: " << line << endl << endl;
   line = scrollAndAppendUntilFind("<div(.)*class(.)*=(.)*\'division(.)*\'(.)*>(.)*</div>", infile, line);
   //cout << "2 getOpponentDivision: " << line << endl << endl;
   line = getSubstringThatMatches(">(.)*</", line);
   //cout << "3 getOpponentDivision: " << line << endl << endl;
   //line = getSubstringThatMatches("[^> ]+[^ </]", line);
   line = getSubstringThatMatches("[^ <>/]+", line);
   //cout << "4 getOpponentDivision: " << line << endl << endl;
   return line;
}

string getOpponentPoints(ifstream &infile){
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\'points(.)*\'>(.)*</div>", infile);
   string s = getSubstringThatMatches(">(.)*</", line);
   return getSubstringThatMatches("[^> ]+[^ </]", s);
}

string getOpponentRawPoints(ifstream &infile){
   string line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*raw_points(.)*", infile);
   string s = getSubstringThatMatches("title(.)*=(.)*\'[0-9]+(.)*[0-9]*", line);
   return getSubstringThatMatches("[0-9][^\']*", s);
}

// get opponent information
int getOpponentInfo(ifstream &infile, string opponent_num){
   int this_person_won = 0;
   int not_friendlies = 0;

   string line = scrollUntilFind("<div(.)*class=\'opponent[^s](.)*", infile);
   line = scrollAndAppendUntilFind("<div(.)*class=\'opponent[^s](.)*><a(.)*class=(.)*</a>", infile, line);

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
      if (this_person_won == 1){
         cout << "WINNER:\n";
         writer.Key("winner");
      }else{
         cout << "LOSER:\n";
         writer.Key("loser");
      }
   }else{
      string keyString = "opponent" + opponent_num;
      writer.Key(keyString.c_str());
   }

   writer.StartObject();

   // get this opponent's user id
   line = scrollAndAppendUntilFind("<a(.)*class=(.)*\'(.)*username(.)*user_id_[0-9]+(.)*>(.)*</a>", infile, line);
   int opponent_user_id = getOpponentUserID(line);
   if (opponent_user_id == -1) die("getOpponentInfo: opponent_user_id = -1\n");
   cout << "User ID:" << opponent_user_id << "\n";

   writer.Key("user_id");
   writer.Int(opponent_user_id);

   // get this opponent's username
   string opponent_username = getOpponentUsername(line);
   if (opponent_username == "") die("getOpponentUsername failed\n");
   else cout << "Username: " << opponent_username << endl;

   writer.Key("username");
   writer.String(opponent_username.c_str());

   // get this opponent's medal
   string oppponent_medal = getOpponentMedal(infile);
   if (oppponent_medal == "") die("getOpponentMedal failed\n");
   else cout << "Medal: " << oppponent_medal << endl;

   writer.Key("medal");
   writer.String(oppponent_medal.c_str());

   // get this opponent's division
   string opponent_division = getOpponentDivision(infile);
   if (opponent_division == "") die("getOpponentDivision failed\n");
   else cout << "Division: " << opponent_division << endl;

   writer.Key("division");
   writer.String(opponent_division.c_str());

   // get this opponent's points
   int opponent_unranked = 0;
   int opp_points_num = 0;

   string opponent_points = getOpponentPoints(infile);
   if (opponent_points == "") opponent_unranked = 1;
   else{   
      stringstream convert(opponent_points);
      convert >> opp_points_num;
   }

   writer.Key("points");

   if (opponent_unranked){
      cout << "Points: (UNRANKED)\n";
      writer.String("(UNRANKED)");
   }
   else{
      cout << "Points: " << opp_points_num << endl;
      writer.String(to_string(opp_points_num).c_str());
   }

   // get this opponents raw points
   string opponent_raw_points = getOpponentRawPoints(infile);
   cout << "Raw Points: " << opponent_raw_points << endl;

   writer.Key("raw_points");
   writer.String(opponent_raw_points.c_str());

   writer.Key("player");
   writer.String(opponent_num.c_str());

   writer.EndObject();
}

string removeQuotes(string line){
   return getSubstringThatMatches("[^\']+", line);
}

/*
return 1 if <div class="feedback_section"> is found
return 0 if <div class="match_scores"> is found
*/
int getFeedbackSectionDiv(ifstream &infile){
   string line = "";
   string append;
   smatch m;
   regex expr("(.)*feedback_section(.)*");
   regex end_expr("<div(.)*class(.)*=(.)*\'(.)*match_scores");
   regex end_expr2("</div>(.)*</div>");

   do{
      getline(infile, append);
      line += append;

      if (regex_search(line, m, expr)){
         //cout << "case one in getFeedbackSectionDiv\n\n";
         return 1;
      }
      else if (regex_search(line, m, end_expr)){
         //cout << "case two in getFeedbackSectionDiv\n\n";
         return 0;
      }
      else if (regex_search(line, m, end_expr2)){
         //cout << "case three in getFeedbackSectionDiv\n\n";
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
string getFeedbackUsername(ifstream &infile, string initStr){
   string line = scrollAndAppendUntilFind("div(.)*<a(.)*>(.)*</a>(.)*<span(.)*votes_text", infile, initStr);
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

   regex p_expr("<i(.)*positive(.)*");
   regex n_expr("<i(.)*negative(.)*");

   do{
      getline(infile, line);
      if (regex_search(line, m, p_expr)) return 1;
      else if (regex_search(line, m, n_expr)) return 0;
   }while(1);
}

//template <template <class> class Writer, class StringBuffer>
void getPlayerFeedback(ifstream &infile){
   string line = scrollUntilFind("(.)*</div>(.)*", infile);
   line = scrollUntilFind("(.)*</div>(.)*", infile);
   while(1){
      int feedbackdiv = getFeedbackSectionDiv(infile);
      if (feedbackdiv){
         int personalityFeedback = getFeedbackType(infile);
         string username = getFeedbackUsername(infile, "");
         int feedbacktype = isGoodFeedback(infile);
         cout << endl << username << " submitted ";
         if (personalityFeedback == 1){ // personality feedback
            cout << "personality";
            writer.Key("personality_feedback");
         }else{ // connection feedback
            cout << "connection";
            writer.Key("connection_feedback");
         }
         writer.StartArray();
         writer.StartArray();

         writer.String(username.c_str());

         cout << ": thumbs ";
         if (feedbacktype == 1){
            writer.Int(1);
            cout << "up";
         }else{
            writer.Int(0);
            cout << "down";
         }
         cout << endl;
         
         writer.EndArray();


         getline(infile, line);
         if (stringMatchesExpr("</div>(.)*", line)){
            writer.EndArray();
            continue;
         }
         else {
            writer.StartArray();
            username = getFeedbackUsername(infile, line);
            int feedbacktype = isGoodFeedback(infile);
            cout << username << " submitted ";
            if (personalityFeedback == 1) // personality feedback
               cout << "personality";
            else // connection feedback
               cout << "connection";
            cout << ": thumbs ";

            writer.String(username.c_str());

            if (feedbacktype == 1){
               cout << "up";
               writer.Int(1);
            }else{
               cout << "down";
               writer.Int(0);
            }
            cout << endl;

            writer.EndArray();
         }

         writer.EndArray();
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
   string s = getSubstringThatMatches("Yosh(.)*Story", str);
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

   writer.Key("stages");
   writer.StartArray();

   while(1){
      divCount++;
      line = scrollUntilFind("(.)*data-name(.)*=(.)*\'[^\']+\'", infile);
      divCount++;
      string quoted_stage = getSubstringThatMatches("\'[^\']+\'", line);
      string stage = removeQuotes(quoted_stage);
      stage = fixYoshis(stage);

      cout << "Game " << game_count++ << ": " << stage << endl;
      writer.String(stage.c_str());

      line = scrollUntilFind("(.)*</div>(.)*", infile); //close data-name div
      divCount--;
      line = scrollUntilFind("(.)*</div>(.)*", infile); // close stage_pick div
      divCount--;
      getline (infile, line);

      if (stringMatchesExpr("(.)*</div>(.)*", line)){
         divCount--;
         if (divCount == 0){
            writer.EndArray();
            return; //closed off all opening div tags with closing div tags
         }else die("getStagesPlayed: number of opening div tags don't match with number of closing div tags");
      }if (stringMatchesExpr("<div(.)*class(.)*=(.)*\'(.)*stage_pick(.)*", line)) continue;
      else die("getStagesPlayed: unknown expression");
   }
}


void getPlayerResult(ifstream &infile, string player_num){
   string line;
   int divCount = 0;

   line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*team_result(.)*", infile);
   divCount++;
   line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*player_result(.)*", infile);
   divCount++;
   line = scrollUntilFind("<div(.)*class(.)*=(.)*\'(.)*character_pick(.)*", infile);

   string keyString = "p" + player_num + "_characters";
   writer.Key(keyString.c_str());
   writer.StartArray();

   while(1){
      divCount++;
      int wasVictorious = characterVictorious(line);
      line = scrollAndAppendUntilFind("<div(.)*class=(.)*character_for_game_melee(.)*</div>", infile, "");
      string character = getCharacterPlayed(line);
      scrollUntilFind("(.)*</div>(.)*", infile); // close character_pick
      divCount--;

      writer.StartArray();
      writer.String(character.c_str());
      cout << character << " (";
      if (wasVictorious){
         writer.Int(1);
         cout << "VICTORIOUS";  
      }else{
         writer.Int(0);
         cout << "DEFEATED";
      } 
      cout << ")\n";
      writer.EndArray();

      getline(infile, line); // get the next line

      if (stringMatchesExpr("(.)*</div>(.)*", line)){
         divCount--; // close player_result

         getline(infile, line); // get the next line
         if (stringMatchesExpr("(.)*</div>(.)*", line)){ // close team_result
            divCount--;
            if (divCount == 0){
               writer.EndArray();
               return; //closed off all opening div tags with closing div tags
            }else die("getPlayerResult: number of opening div tags don't match with number of closing div tags");
         }else die("getPlayerResult: number of opening div tags don't match with number of closing div tags");
      }else if (stringMatchesExpr("<div(.)*class(.)*=(.)*\'(.)*character_pick", line)) continue;
      else die("getPlayerResult: unknown expression");
   }
}

//template <template <class> class Writer, class StringBuffer>
void getGamesFromFile(string filename){
   ifstream infile(filename);

   for (string line; getline(infile, line); ){

      // find match div
      smatch sm;

      regex expr("<div(.)*data-match-id(.)*=");
      if (regex_search(line, sm, expr)){ // matchdiv found
         //cout << "matchdiv found!" << endl;

         string nextline;
         getline(infile, nextline);
         line += nextline;



         // get the match ID
         int match_id = findMatchID(sm);
         if (match_id == -1) die("Match ID wasn't found.\n");
         else cout << "The match id is: " << match_id << "\n";

         writer.StartObject();         
         writer.Key("match_id");
         writer.Int(match_id);

         // get the season ID
         int season_id = findSeasonID(line);
         if (season_id == -1) die("Season ID wasn't found.\n");
         else cout << "The season id is: " << season_id << endl;

         writer.Key("season_id");
         writer.Int(season_id);

         // get the ladder number
         int ladder_num = findLadderNum(line);
         if (ladder_num == -1) die("Ladder number wasn't found.\n");
         else cout << "The ladder number is: " << ladder_num << endl;

         writer.Key("ladder_num");
         writer.Int(ladder_num);

         // find out if the match is ranked or not
         int ranked = isRanked(line);
         if (ranked){

            writer.Key("match_type");
            writer.String("RANKED");

            // cout << "The match was ranked.\n";

            // find the result of the mass (win or loss)
            int match_result = getMatchResult(line);
            if (match_result == -1) die("Match result wasn't found.\n");
            else if (match_result == 0) cout << "Result: LOSS\n";
            else if (match_result == 1) cout << "Result: WIN\n";

            writer.Key("result");
            writer.String("WIN");

            // get the date of the match
            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id);

            line = scrollUntilFind(dateExpr, infile);

            string line2;
            getline(infile, line2);
            line += line2;

            // date_expr should've been found by now
            string date = getDate(line);
            cout << "Date: " << date << endl;

            writer.Key("date");
            writer.String(date.c_str());            

            cout << "\n[RANKED MATCH]\n";

            // get opponent information
            getOpponentInfo(infile, "1");

            // get opponent information
            getOpponentInfo(infile, "2");

            // get player feedback
            getPlayerFeedback(infile);

            cout << "\nStages:\n";
            getStagesPlayed(infile);

            cout << "\nP1 Characters:\n";
            getPlayerResult(infile, "1");

            cout << "\nP2 Characters:\n";
            getPlayerResult(infile, "2");


            cout << endl << 
            "========================================================="
            << endl << endl;

            //cout << "signal 1" << endl;
            //cout << s.GetString() << endl;
            writer.EndObject();
            //cout << "signal 2" << endl;
         }else{
            writer.Key("match_type");
            writer.String("UNRANKED");

            cout << "The match was NOT ranked.\n";

            string dateExpr = "https://www.smashladder.com/match/view/" + to_string(match_id);


            line = scrollUntilFind(dateExpr, infile);
            string line2;
            getline(infile, line2);
            line += line2;

            // date_expr should've been found by now
            string date = getDate(line);
            cout << "Date: " << date << endl;

            writer.Key("date");
            writer.String(date.c_str());

            cout << "\n[FRIENDLIES]\n";

            // get opponent information
            getOpponentInfo(infile, "1");

            // get opponent information
            getOpponentInfo(infile, "2");

            // get player feedback
            getPlayerFeedback(infile);

            cout << endl << 
            "========================================================="
            << endl << endl;

            writer.EndObject();
         }
      }
   }
}


int main(){

   //writer.StartObject();
   writer.StartArray();

   getGamesFromFile("../html/updatedHistories/updatedHistory.html");

   writer.EndArray();
   //writer.EndObject();

   //cout << "JSON DATA:" << endl;
   //cout << s.GetString() << endl;

   // Write JSON to file
   ofstream jsonDataFile;
   //jsonDataFile.open("../json/match-history/hist.json");
   jsonDataFile.open("../dist/js/history.js");
   jsonDataFile << "var HistoryJSON = " << s.GetString() << endl;
   jsonDataFile.close();

   return 0;
}