// combineJSONMatches.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <fstream>
#include <sstream>
#include <map>
// ./source
#include "lookaheadparser.h"

using namespace rapidjson;
using namespace std;

// Global Var
StringBuffer s;
Writer<StringBuffer> writer(s);
map<int, int> matchesMap;

void addMatch(LookaheadParser &r, int match_id) {

	// add this match to the hashmap
	matchesMap[match_id] = 1;

	writer.StartObject();
	writer.Key("match_id");
	writer.Int(match_id);

	while (const char* key = r.NextObjectKey()) {

		if (0 == strcmp(key, "season_id")) {
			writer.Key(key);
			writer.Int(r.GetInt());
		}
		else if (0 == strcmp(key, "ladder_num")) {
			writer.Key(key);
			writer.Int(r.GetInt());
		}
		else if (0 == strcmp(key, "match_type")) {
			writer.Key(key);
			writer.String(r.GetString());
		}
		else if (0 == strcmp(key, "result")) {
			writer.Key(key);
			writer.String(r.GetString());
		}
		else if (0 == strcmp(key, "date")) {
			writer.Key(key);
			writer.String(r.GetString());
		}
		else if (0 == strcmp(key, "winner") || 0 == strcmp(key, "loser")
			|| 0 == strcmp(key, "opponent1") || 0 == strcmp(key, "opponent2"))
		{
			writer.Key(key);

			writer.StartObject();
			RAPIDJSON_ASSERT(r.PeekType() == kObjectType);
			r.EnterObject();
			while (const char* wkey = r.NextObjectKey()) {
				writer.Key(wkey);
				if (0 == strcmp(wkey, "user_id")) {
					writer.Int(r.GetInt());
				}
				else {
					writer.String(r.GetString());
				}
			}
			writer.EndObject();
		}
		else if (0 == strcmp(key, "stages")) {
			writer.Key(key);
			writer.StartArray();
			RAPIDJSON_ASSERT(r.PeekType() == kArrayType);
			r.EnterArray();
			while (r.NextArrayValue()) {
				if (r.PeekType() == kStringType) {
					writer.String(r.GetString());
				}
				else {
					r.SkipArray();
					break;
				}
			}
			writer.EndArray();
		}
		else if (0 == strcmp(key, "p1_characters") || 0 == strcmp(key, "p2_characters")
			|| 0 == strcmp(key, "connection_feedback") || 0 == strcmp(key, "personality_feedback"))
		{
			writer.Key(key);

			writer.StartArray();
			RAPIDJSON_ASSERT(r.PeekType() == kArrayType);
			r.EnterArray();
			while (r.NextArrayValue()) {
				writer.StartArray();
				RAPIDJSON_ASSERT(r.PeekType() == kArrayType);
				r.EnterArray();
				while (r.NextArrayValue()) {
					if (r.PeekType() == kStringType) {
						writer.String(r.GetString());
					}
					else {
						writer.Int(r.GetInt());
					}
				}
				writer.EndArray();
			}
			writer.EndArray();
		}
		else {
			r.SkipValue();
		}
	}
	writer.EndObject();
}

void getMatchesFromFile(string filename) {
	ifstream infile(filename);

	string JsonStr = "";
	string line = "";

	while (getline(infile, line))
		JsonStr += line;

	char *json = (char *)JsonStr.c_str();
	LookaheadParser r(json);

	RAPIDJSON_ASSERT(r.PeekType() == kArrayType);
	r.EnterArray();

	while (r.NextArrayValue()) {
		RAPIDJSON_ASSERT(r.PeekType() == kObjectType);
		r.EnterObject();
		const char* key = r.NextObjectKey();
		int match_id = r.GetInt();

		if (matchesMap.count(match_id)) { // match id is in hashmap
			while (r.NextObjectKey()) r.SkipValue();
		}
		else { // match id is not in hashmap
			addMatch(r, match_id);
		}
	}
}

int main() {
	writer.StartArray();

	getMatchesFromFile("../json/match-history/updated-hist.json");
	getMatchesFromFile("../json/match-history/all-hist.json");

	writer.EndArray();

	// Write to JSON Directory
	ofstream jsonDataFile;
	jsonDataFile.open("../json/match-history/all-hist.json");
	jsonDataFile << s.GetString() << endl;
	jsonDataFile.close();
}