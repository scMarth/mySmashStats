#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
// rapidjson
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"


// This example demonstrates JSON token-by-token parsing with an API that is
// more direct; you don't need to design your logic around a handler object and
// callbacks. Instead, you retrieve values from the JSON stream by calling
// GetInt(), GetDouble(), GetString() and GetBool(), traverse into structures
// by calling EnterObject() and EnterArray(), and skip over unwanted data by
// calling SkipValue(). When you know your JSON's structure, this can be quite
// convenient.
//
// If you aren't sure of what's next in the JSON data, you can use PeekType() and
// PeekValue() to look ahead to the next object before reading it.
//
// If you call the wrong retrieval method--e.g. GetInt when the next JSON token is
// not an int, EnterObject or EnterArray when there isn't actually an object or array
// to read--the stream parsing will end immediately and no more data will be delivered.
//
// After calling EnterObject, you retrieve keys via NextObjectKey() and values via
// the normal getters. When NextObjectKey() returns null, you have exited the
// object, or you can call SkipObject() to skip to the end of the object
// immediately. If you fetch the entire object (i.e. NextObjectKey() returned  null),
// you should not call SkipObject().
//
// After calling EnterArray(), you must alternate between calling NextArrayValue()
// to see if the array has more data, and then retrieving values via the normal
// getters. You can call SkipArray() to skip to the end of the array immediately.
// If you fetch the entire array (i.e. NextArrayValue() returned null),
// you should not call SkipArray().
//
// This parser uses in-situ strings, so the JSON buffer will be altered during the
// parse.

using namespace rapidjson;
using namespace std;


class LookaheadParserHandler {
public:
    bool Null() { st_ = kHasNull; v_.SetNull(); return true; }
    bool Bool(bool b) { st_ = kHasBool; v_.SetBool(b); return true; }
    bool Int(int i) { st_ = kHasNumber; v_.SetInt(i); return true; }
    bool Uint(unsigned u) { st_ = kHasNumber; v_.SetUint(u); return true; }
    bool Int64(int64_t i) { st_ = kHasNumber; v_.SetInt64(i); return true; }
    bool Uint64(uint64_t u) { st_ = kHasNumber; v_.SetUint64(u); return true; }
    bool Double(double d) { st_ = kHasNumber; v_.SetDouble(d); return true; }
    bool RawNumber(const char*, SizeType, bool) { return false; }
    bool String(const char* str, SizeType length, bool) { st_ = kHasString; v_.SetString(str, length); return true; }
    bool StartObject() { st_ = kEnteringObject; return true; }
    bool Key(const char* str, SizeType length, bool) { st_ = kHasKey; v_.SetString(str, length); return true; }
    bool EndObject(SizeType) { st_ = kExitingObject; return true; }
    bool StartArray() { st_ = kEnteringArray; return true; }
    bool EndArray(SizeType) { st_ = kExitingArray; return true; }

protected:
    LookaheadParserHandler(char* str);
    void ParseNext();

protected:
    enum LookaheadParsingState {
        kInit,
        kError,
        kHasNull,
        kHasBool,
        kHasNumber,
        kHasString,
        kHasKey,
        kEnteringObject,
        kExitingObject,
        kEnteringArray,
        kExitingArray
    };
    
    Value v_;
    LookaheadParsingState st_;
    Reader r_;
    InsituStringStream ss_;
    
    static const int parseFlags = kParseDefaultFlags | kParseInsituFlag;
};

LookaheadParserHandler::LookaheadParserHandler(char* str) : v_(), st_(kInit), r_(), ss_(str) {
    r_.IterativeParseInit();
    ParseNext();
}

void LookaheadParserHandler::ParseNext() {
    if (r_.HasParseError()) {
        st_ = kError;
        return;
    }
    
    r_.IterativeParseNext<parseFlags>(ss_, *this);
}

class LookaheadParser : protected LookaheadParserHandler {
public:
    LookaheadParser(char* str) : LookaheadParserHandler(str) {} // constructor
    
    bool EnterObject();
    bool EnterArray();
    const char* NextObjectKey();
    bool NextArrayValue();
    int GetInt();
    double GetDouble();
    const char* GetString();
    bool GetBool();
    void GetNull();

    void SkipObject();
    void SkipArray();
    void SkipValue();
    Value* PeekValue();
    int PeekType(); // returns a rapidjson::Type, or -1 for no value (at end of object/array)
    
    bool IsValid() { return st_ != kError; }
    
protected:
    void SkipOut(int depth);
};

bool LookaheadParser::EnterObject() {
    if (st_ != kEnteringObject) {
        st_  = kError;
        return false;
    }
    
    ParseNext();
    return true;
}

bool LookaheadParser::EnterArray() {
    if (st_ != kEnteringArray) {
        st_  = kError;
        return false;
    }
    
    ParseNext();
    return true;
}

const char* LookaheadParser::NextObjectKey() {
    if (st_ == kHasKey) {
        const char* result = v_.GetString();
        ParseNext();
        return result;
    }
    
    if (st_ != kExitingObject) {
        st_ = kError;
        return 0;
    }
    
    ParseNext();
    return 0;
}

bool LookaheadParser::NextArrayValue() {
    if (st_ == kExitingArray) {
        ParseNext();
        return false;
    }
    
    if (st_ == kError || st_ == kExitingObject || st_ == kHasKey) {
        st_ = kError;
        return false;
    }

    return true;
}

int LookaheadParser::GetInt() {
    if (st_ != kHasNumber || !v_.IsInt()) {
        st_ = kError;
        return 0;
    }

    int result = v_.GetInt();
    ParseNext();
    return result;
}

double LookaheadParser::GetDouble() {
    if (st_ != kHasNumber) {
        st_  = kError;
        return 0.;
    }
    
    double result = v_.GetDouble();
    ParseNext();
    return result;
}

bool LookaheadParser::GetBool() {
    if (st_ != kHasBool) {
        st_  = kError;
        return false;
    }
    
    bool result = v_.GetBool();
    ParseNext();
    return result;
}

void LookaheadParser::GetNull() {
    if (st_ != kHasNull) {
        st_  = kError;
        return;
    }

    ParseNext();
}

const char* LookaheadParser::GetString() {
    if (st_ != kHasString) {
        st_  = kError;
        return 0;
    }
    
    const char* result = v_.GetString();
    ParseNext();
    return result;
}

void LookaheadParser::SkipOut(int depth) {
    do {
        if (st_ == kEnteringArray || st_ == kEnteringObject) {
            ++depth;
        }
        else if (st_ == kExitingArray || st_ == kExitingObject) {
            --depth;
        }
        else if (st_ == kError) {
            return;
        }

        ParseNext();
    }
    while (depth > 0);
}

void LookaheadParser::SkipValue() {
    SkipOut(0);
}

void LookaheadParser::SkipArray() {
    SkipOut(1);
}

void LookaheadParser::SkipObject() {
    SkipOut(1);
}

Value* LookaheadParser::PeekValue() {
    if (st_ >= kHasNull && st_ <= kHasKey) {
        return &v_;
    }
    
    return 0;
}

int LookaheadParser::PeekType() {
    if (st_ >= kHasNull && st_ <= kHasKey) {
        return v_.GetType();
    }
    
    if (st_ == kEnteringArray) {
        return kArrayType;
    }
    
    if (st_ == kEnteringObject) {
        return kObjectType;
    }

    return -1;
}

//-------------------------------------------------------------------------

// Global Var
StringBuffer s;
Writer<StringBuffer> writer(s);
map<int, int> matchesMap;

void addMatch(LookaheadParser &r, int match_id){
    //cout << "addMatch with id: " << match_id << endl;

    // add this match to the hashmap
    matchesMap[match_id] = 1;

    //cout << "-->it doesn't exist in the hashmap" << endl;
    //r.NextObjectKey();
    //cout << "ASDF: " << r.GetInt() << endl;

    writer.StartObject();
    writer.Key("match_id");
    writer.Int(match_id);

    while (const char* key = r.NextObjectKey()){
        /*
        cout << "key: ";
        cout << key << endl;
        r.SkipValue();
        */
        /*
        if (0 == strcmp(key, "match_type")) {
            cout << r.GetString() << endl;
        }else{
            r.SkipValue();
        }
        */
        if (0 == strcmp(key, "season_id")) {
            cout << "case 1\n";
            writer.Key(key);
            writer.Int(r.GetInt());
            cout << "end case 1\n";
        } else if (0 == strcmp(key, "ladder_num")) {
            cout << "case 2\n";
            writer.Key(key);
            writer.Int(r.GetInt());
            cout << "end case 2\n";
        } else if (0 == strcmp(key, "match_type")) {
            cout << "case 3\n";
            writer.Key(key);
            writer.String(r.GetString());
            cout << "end case 3\n";
        } else if (0 == strcmp(key, "result")) {
            cout << "case 4\n";
            writer.Key(key);
            writer.String(r.GetString());
            cout << "end case 4\n";
        } else if (0 == strcmp(key, "date")) {
            cout << "case 5\n";
            writer.Key(key);
            writer.String(r.GetString());
            cout << "end case 5\n";
        } else if (0 == strcmp(key, "winner") || 0 == strcmp(key, "loser")
            || 0 == strcmp(key, "opponent1") || 0 == strcmp(key, "opponent2"))
        {
            cout << "case 6\n";
            writer.Key(key);

            writer.StartObject();
            RAPIDJSON_ASSERT(r.PeekType() == kObjectType);
            r.EnterObject();
            while (const char* wkey = r.NextObjectKey()){
                writer.Key(wkey);
                if (0 == strcmp(wkey, "user_id")) {
                    writer.Int(r.GetInt());
                }else{
                    writer.String(r.GetString());
                }
            }
            writer.EndObject();
            cout << "end case 6\n";
        } else if (0 == strcmp(key, "stages")) {
            cout << "case 7\n";
            writer.Key(key);
            writer.StartArray();
            RAPIDJSON_ASSERT(r.PeekType() == kArrayType);
            r.EnterArray();
            while (r.NextArrayValue()){
                if (r.PeekType() == kStringType) {
                    writer.String(r.GetString());
                }else {
                    r.SkipArray();
                    break;
                }
            }
            writer.EndArray();
            cout << "end case 7\n";
        } else if (0 == strcmp(key, "p1_characters") || 0 == strcmp(key, "p2_characters")
            || 0 == strcmp(key, "connection_feedback") || 0 == strcmp(key, "personality_feedback"))
        {
            writer.Key(key);

            writer.StartArray();
            RAPIDJSON_ASSERT(r.PeekType() == kArrayType);
            r.EnterArray();
            while (r.NextArrayValue()){
                writer.StartArray();
                RAPIDJSON_ASSERT(r.PeekType() == kArrayType);
                r.EnterArray();
                while (r.NextArrayValue()){
                    if (r.PeekType() == kStringType) {
                        writer.String(r.GetString());
                    }else{
                        writer.Int(r.GetInt());
                    }
                }
                writer.EndArray();
            }
            writer.EndArray();
        }else{
            r.SkipValue();
        }
    }
    writer.EndObject();
}



int main() {
    //string filename = "test.json";
    string filename = "containsdoubles.json";
    ifstream infile(filename);

    string test = "";
    string line = "";

    while (getline(infile, line)){
        test += line;
    }

    cout << "test\n\n";
    cout << test;
    cout << endl;

    char *json = (char *)test.c_str();
    LookaheadParser r(json);
    
    //cout << "signal 1\n";
    //cout << r.PeekType() << endl;
    RAPIDJSON_ASSERT(r.PeekType() == kArrayType);
    //cout << "signal 2\n";
    r.EnterArray();
    //cout << "signal 3\n";

    writer.StartArray();

    while (r.NextArrayValue()){
        //cout << "signal 4\n";
        RAPIDJSON_ASSERT(r.PeekType() == kObjectType);
        r.EnterObject();
        const char* key = r.NextObjectKey();
        int match_id = r.GetInt();

        if (matchesMap.count(match_id)){ // match id is in hashmap
            cout << "Match ID: " << match_id;
            cout << " ; it exists in the hashmap!!!" << endl;
            while (r.NextObjectKey()) r.SkipValue();
        }else{ // match id is not in hashmap
            addMatch(r, match_id);
        }
    }
    cout << "endwhile\n";

    writer.EndArray();
    
   // Write JSON to file
   ofstream jsonDataFile;
   //jsonDataFile.open("../json/match-history/hist.json");
   jsonDataFile.open("history.js");
   jsonDataFile << s.GetString() << endl;
   jsonDataFile.close();

    return 0;
}
