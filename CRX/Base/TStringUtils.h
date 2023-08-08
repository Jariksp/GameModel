#ifndef CRX_BASE_TSTRINGUTILS_H_
#define CRX_BASE_TSTRINGUTILS_H_

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <functional>
#include <type_traits>
#include <memory>


// нельзя внестси в неймспейс перегруженные операторы << иначе все это не будет работать.

namespace crx {
	// тут немного не по конвеншену называется, то это специально, чтобы на STL было похоже
	std::string to_utf8(const std::u16string &s);
	std::string to_utf8(const std::u32string &s);
	std::string to_utf8(const std::wstring &s);
	std::u32string to_utf32(const std::string &s);
	std::u16string to_utf16(const std::string &s);
	std::wstring to_wstring(const std::string &s);

	int to_int(const std::string& s);
	unsigned to_uint(const std::string& s);
	double to_double(const std::string& s);
	float to_float(const std::string& s);
	bool to_bool(const std::string& s);
	uint64_t to_uint64(const std::string& s);
	uint64_t to_int64(const std::string& s);

	std::string to_hexstring(unsigned v);

	template<class TT>
	typename std::remove_reference<TT>::type from_string(const std::string& data) {
		std::stringstream ss;
		ss.str(data);
		typename std::remove_reference<TT>::type ret;
		ss>>ret;
		return ret;
	}



	template<class T1, class T2>
	void Override( std::map<T1, T2>& dst, const std::map<T1, T2>& src ) {
		typename std::map<T1, T2>::const_iterator it = src.begin();
		for(;it!=src.end();it++) dst[ it->first ] = it->second;
	}

	// собственно сделал, чтобы удобнее было вызывать с всякими char*, а внутри оно темплейт использует
	std::vector<std::string> StrTok(const std::string& s, const std::string& sep, bool includeEmpty=false, bool includeLastEmpty=false);
	std::vector<std::u16string> StrTok(const std::u16string& s, const std::u16string& sep, bool includeEmpty=false, bool includeLastEmpty=false);

	std::string ReplaceAll(const std::string& s, const std::string& replaceFrom, const std::string& replaceTo);
	std::u16string ReplaceAll(const std::u16string& s, const std::u16string& replaceFrom, const std::u16string& replaceTo);

	std::vector<std::string> SubStrsBetween(const std::string& text, const std::string& lhs, const std::string& rhs);

	std::map<std::string, std::string> StringToMap(const std::string& data);

	void RecurciveTrim(std::string& s);
	bool TrimRWithCheck(std::string& s);
	bool TrimLWithCheck(std::string& s);

	std::string Trim(const std::string& s);
	std::vector<std::string> Trim(const std::vector<std::string>& vv);
	std::string TrimR(const std::string& s);
	std::string TrimL(const std::string &s);

	std::string ToLowerCase(const std::string& s);
	void ToLowerCaseNoCopy(std::string& s);

	std::string GetFileExtension(const std::string& fileName);
	std::string GetDirectoryPath(const std::string& fileName);// inclue last /
	std::string GetFileNameFromPath(const std::string& fileName, bool includeExtention=true);

	template<typename ... Args>
	std::string Format( const std::string& format, Args ... args ) {
	    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
	    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
	    std::unique_ptr<char[]> buf( new char[ size ] );
	    snprintf( buf.get(), size, format.c_str(), args ... );
	    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
	}

	std::string FixPointFormat(double val, int digitsAfterDot);

	void _AssemblePath(std::string& out, const std::string& in);

	template <class... Ts>
	std::string AssembleFilePath(Ts const&... args) {// собирает именно путь к файлу
		std::string out;
	    ((_AssemblePath(out, args)), ...);
	    return out;
	}


	template <class... Ts>
	std::string AssemblePath(Ts const&... args) {// путь к директрии. добаляет / в конце
		std::string out = AssembleFilePath(args...);
		if( out.size() && ( *out.rbegin() )!='/' ) out += '/';
	    return out;
	}

	template<class TB>
	std::ostream& OperatorToOS(std::ostream& st, const TB& rr) {//operator<<
		st<<rr.ToString();
		return st;
	}

	template<class TB>
	std::istream& OperatorFromIS(std::istream& is, TB& rr) {//operator>>
		std::string str;
		is>>str;
		rr.FromString(str);
		return is;
	}

	bool FindAnyWordInString(std::string str, std::string find);
	
	std::string GreedFixPointFormat(const std::string& str);

	// имплементация будет в другом .cpp
	std::string base64_decode(const std::string& s);
	std::string base64_encode(uint8_t const*, size_t len);
	std::string base64_encode(const std::string& s);
};

//ostream& operator<<(ostream& st, const u16string& ws);

// опять же, не по конвеншену, но зато стыкуется с std::to_string

namespace std {

template<class TT>
std::string to_string(const TT& val) {
	std::ostringstream ss;
	ss<<val;
	return ss.str();
}

template <class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
	os << "[";
	for(typename std::vector<T>::const_iterator ii = v.begin(); ii != v.end(); ++ii) {
    	os<<*ii;
    	if(ii!=(v.end()-1)) os<<", ";
	}
	os << "]";
	return os;
}

template <class TF, class TS>
std::ostream& operator << (std::ostream& os, const std::pair<TF, TS>& p) {
	os<<"("<< p.first<<" ; "<<p.second <<")";
	return os;
}


}


#endif /* BASE_TSTRINGUTILS_H_ */
