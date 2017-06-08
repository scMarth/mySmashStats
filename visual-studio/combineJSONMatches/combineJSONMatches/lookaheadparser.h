#pragma once

#include "../../../include/rapidjson/document.h"
#include "../../../include/rapidjson/writer.h"
#include "../../../include/rapidjson/stringbuffer.h"
#include <iostream>

// Lookaheadparser example from the rapidjson examples

using namespace rapidjson;

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
		st_ = kError;
		return false;
	}

	ParseNext();
	return true;
}

bool LookaheadParser::EnterArray() {
	if (st_ != kEnteringArray) {
		st_ = kError;
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
		st_ = kError;
		return 0.;
	}

	double result = v_.GetDouble();
	ParseNext();
	return result;
}

bool LookaheadParser::GetBool() {
	if (st_ != kHasBool) {
		st_ = kError;
		return false;
	}

	bool result = v_.GetBool();
	ParseNext();
	return result;
}

void LookaheadParser::GetNull() {
	if (st_ != kHasNull) {
		st_ = kError;
		return;
	}

	ParseNext();
}

const char* LookaheadParser::GetString() {
	if (st_ != kHasString) {
		st_ = kError;
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
	} while (depth > 0);
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