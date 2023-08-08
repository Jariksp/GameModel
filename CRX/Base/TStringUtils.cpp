#include <locale>
#include <codecvt>
#include <chrono>
#include <CRX/Base/TStringUtils.h>
#include <stdio.h>

namespace crx {

static unsigned s_uSplitSize = 2;

#ifdef _MSC_VER
//Warnings: C4244 | wstring_convert - deprecated
//Possibble solutions:
//Rewrite or use macro _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
//Now we use second option in Project -> Properties -> C/C++ -> Preprocessor
//For more read: https://stackoverflow.com/questions/42946335/deprecated-header-codecvt-replacement
//Better: rewrite

std::string to_utf8(const std::u32string &s) {
	static std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;
	auto p = reinterpret_cast<const int32_t *>(s.data());
	return convert.to_bytes(p, p + s.size());
}

std::string to_utf8(const std::u16string& utf16_string) {
	static std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	auto p = reinterpret_cast<const int16_t *>(utf16_string.data());
	return convert.to_bytes(p, p + utf16_string.size());
}


std::u32string to_utf32(const std::string &s) {
	static std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> convert;
	auto asInt = convert.from_bytes(s);
	return std::u32string(reinterpret_cast<char32_t const *>(asInt.data()), asInt.length());
}

std::u16string to_utf16(const std::string &s) {
	static std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	auto asInt = convert.from_bytes(s);
	return std::u16string(reinterpret_cast<char16_t const *>(asInt.data()), asInt.length());
}

std::wstring to_wstring(const std::string &s) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
	auto asInt = convert.from_bytes(s);
	return std::wstring(reinterpret_cast<wchar_t const *>(asInt.data()), asInt.length());
}

std::string to_utf8(const std::wstring &s) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
	auto p = reinterpret_cast<const wchar_t *>(s.data());
	return convert.to_bytes(p, p + s.size());
}


#else

std::string to_utf8(const std::u32string &s) {
	static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.to_bytes(s);
}

std::string to_utf8(const std::u16string &s) {
	static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
	return conv.to_bytes(s);
}


std::u32string to_utf32(const std::string &s) {
	static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
	return conv.from_bytes(s);
}

std::u16string to_utf16(const std::string &s) {
	static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
	return conv.from_bytes(s);
}

std::wstring to_wstring(const std::string &s) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
	return conv.from_bytes(s);

}

std::string to_utf8(const std::wstring &s) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
	return conv.to_bytes(s);
}


#endif


int to_int(const std::string &s) { return strtol(s.c_str(), 0, 0); }

unsigned to_uint(const std::string &s) { return strtoul(s.c_str(), 0, 0); }

double to_double(const std::string &s) { return strtod(s.c_str(), 0); }

float to_float(const std::string &s) { return strtof(s.c_str(), 0); }

bool to_bool(const std::string &s) { return s=="true" || s=="1"; }

uint64_t to_uint64(const std::string &s) { return strtoull(s.c_str(), 0, 0); }

uint64_t to_int64(const std::string& s) { return strtoull(s.c_str(), 0, 0); }

std::string to_hexstring(unsigned a) {
	char aa[32];
	snprintf(aa, 31, "%X", a);
	return std::string(aa);
}

static const std::string& s_sSpaceChars = "\t\n\v\f\r ";

void RecurciveTrim(std::string& s) {
	while(TrimRWithCheck(s));
	while(TrimLWithCheck(s));
}

bool TrimRWithCheck(std::string& s) {
	auto strSize = s.size();
	auto idx = s.find_last_not_of(s_sSpaceChars) + 1;
	if(idx == strSize) return false;
	s.erase();
	return strSize != s.size();
}

bool TrimLWithCheck(std::string& s) {
	auto strSize = s.size();
	auto idx = s.find_first_not_of(s_sSpaceChars);
	if(idx == 0) return false;
	s.erase(0, idx);
	return strSize != s.size();
}

std::string Trim(const std::string& s) { return TrimL(TrimR(s)); }

std::vector<std::string> Trim(const std::vector<std::string>& vv) {
	std::vector<std::string> out;
	for(const auto& a:vv) out.push_back( Trim(a) );
	return out;
}

std::string TrimR(const std::string& s) {
	auto str = s;
	str.erase( str.find_last_not_of(s_sSpaceChars)+1 );
	return str;
}

std::string TrimL(const std::string &s) {
	auto str = s;
	str.erase(0, str.find_first_not_of(s_sSpaceChars));
	return str;
}


std::string ToLowerCase(const std::string &s) {
	auto str = s;
	ToLowerCaseNoCopy(str);
	return str;
}

static bool s_bToLoweCaseTableInited = false;
static char s_vToLoweCaseTable[256];

static void InitToLowerCaseTable() {
	if( s_bToLoweCaseTableInited ) return;
	for(unsigned i=0;i<256;i++) s_vToLoweCaseTable[i] = tolower(i);
	s_bToLoweCaseTableInited = true;
}


void ToLowerCaseNoCopy(std::string &s) {
	InitToLowerCaseTable();
	for(unsigned i=0;i<s.size();i++) s[i] = s_vToLoweCaseTable[int(s[i])];
}

std::string GetFileExtension(const std::string &fn) {
	auto idx = fn.rfind('.');

	if(idx != std::string::npos) return fn.substr(idx+1);

	return "";
}

std::string GetDirectoryPath(const std::string& fn) {
	auto found = fn.find_last_of("/");
	if(found!=std::string::npos && found!=0) {
		return fn.substr(0,found+1);
	}
	return "";
}

std::string GetFileNameFromPath(const std::string& fileName, bool includeExtention) {
	auto idx = fileName.find_last_of('/');
	std::string fn;
	if(idx!=std::string::npos) {
		fn = fileName.substr(idx+1);
	} else {
		fn = fileName;
	}

	if(!includeExtention) {
		idx = fn.find_last_of(".");
		if(idx!=std::string::npos) fn = fn.erase(idx, fn.size());
	}

	return fn;
}


// разбивает строку на подстроки. может использоваться с разными тимами строк
template<class TT>
std::vector<TT> _StrTok(const TT& data, const TT& sep, bool includeEmpty=false, bool includeLastEmpty=false) {
	std::vector<TT> v;

	size_t pos = 0;
	while(true) {
		auto from = pos;
		pos = data.find(sep, pos);
		if( from<pos && from<data.size() ) v.push_back( data.substr(from, pos-from) );
		else if( includeEmpty && from==pos ) v.push_back( TT() );

		if(pos==std::string::npos) break;
		pos += sep.size();
	}
	if(includeLastEmpty && data.size()>=sep.size()) {
		auto size = sep.size();
		if( data.substr( data.size()-size, size )==sep ) v.push_back( TT() );
	}
	return v;
}

template<class TT>
TT _ReplaceAll(const TT& data, const TT& rf, const TT& rt) {
	TT s = data;
	size_t last_pos = 0;
	while(true) {
		auto a = s.find(rf, last_pos);
		if(a==std::string::npos) break;
		s.replace(a, rf.size(), rt);
		last_pos = a + rt.size();
		if( last_pos>=s.size() ) break;
	}
	return s;
}

std::vector<std::string> StrTok(const std::string &s, const std::string &sep, bool includeEmpty, bool includeLastEmpty) {
	return _StrTok(s, sep, includeEmpty, includeLastEmpty );
}

std::vector<std::u16string> StrTok(const std::u16string &s, const std::u16string &sep, bool includeEmpty, bool includeLastEmpty ) {
	return _StrTok(s, sep, includeEmpty, includeLastEmpty );
}

std::string ReplaceAll(const std::string &s, const std::string &replaceFrom, const std::string &replaceTo) {
	return _ReplaceAll(s, replaceFrom, replaceTo);
}

std::u16string ReplaceAll(const std::u16string &s, const std::u16string &replaceFrom, const std::u16string &replaceTo) {
	return _ReplaceAll(s, replaceFrom, replaceTo);
}

std::vector<std::string> SubStrsBetween(const std::string& text, const std::string& lhs, const std::string& rhs) {
	std::vector<std::string> result;

	auto externToks = crx::StrTok(text, lhs, true, true);
	if(externToks.size()>s_uSplitSize) externToks.erase( externToks.begin() );

	for(auto& externTok:externToks) {
		auto innerToks = crx::StrTok(externTok, rhs, true, true);
		if( innerToks.size()==s_uSplitSize ) result.push_back( innerToks.front() );
	}

	return result;
}

std::map<std::string, std::string> StringToMap(const std::string& data) {
	std::map<std::string, std::string> mm;

	auto vv = StrTok(data, "\n");
	for(auto& line:vv) {
		auto pp = StrTok(line, "=");
		if(pp.size()==1) {
			mm[Trim(pp[0])] = "";
		} else if(pp.size()==2) {
			mm[Trim(pp[0])] = Trim(pp[1]);
		}
	}

	return mm;

}

std::string FixPointFormat(double val, int digitsAfterDot) {
	char buf[1024];
	std::string format = std::string("%.")+std::to_string(digitsAfterDot)+std::string("lf");
	snprintf(buf, 1024, format.c_str(), val );
	return buf;
}

void _AssemblePath(std::string& out, const std::string& in) {
	if(out.empty()) {
		out = in;
		return;
	}
	if(in.empty()) return;

	if( (*out.rbegin())!='/' && (*in.begin())!='/' ) {
		out += '/';
	} else if( (*out.rbegin())=='/' && (*in.begin())=='/' ) {
		out.erase(out.end()-1);
	}

	out += in;
}

bool FindAnyWordInString(std::string str, std::string find) {
	find = ToLowerCase(find);
	auto vv = StrTok(find, " ");

	for(auto & v : vv) {
		if(str.find(v) != std::string::npos) return true;
	}
	return false;
}

std::string GreedFixPointFormat(const std::string& str) {
	auto result = str;
	if( result.find('+')==0 ) result.erase( result.begin() );

	if( result.find('.')==std::string::npos ) return result;
    
	// Remove trailing zeroes
    result = result.substr(0, result.find_last_not_of('0')+1);

    // If the decimal point is now the last character, remove that as well
    if( result.find('.')==result.size()-1) result.pop_back();

	return result;
}

}//crx

/*
std::ostream& operator<<(std::ostream& st, const std::u16string& utf16) {
	st<<crx::to_utf8(utf16);
	return st;
}
*/
