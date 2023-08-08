#ifndef BASE_TPARSEFORMAT_H_
#define BASE_TPARSEFORMAT_H_

#include <map>
#include <string>
#include <functional>

namespace crx {

class TReflectionObject;
class TReflectionClassPropertyData;

class TParseFormat {
	public:
	// for owner and sequance "a.b.c" will write to cache innerest obj and property relation if possible
	std::map<std::string, std::pair<crx::TReflectionObject*, crx::TReflectionClassPropertyData*>> Parse(crx::TReflectionObject* owner,
																										const std::string& relationPropertyDataText,
																										const std::vector<std::string>& relationPropertyFoldData={});
	
	std::string Format(const std::map<std::string, std::pair<crx::TReflectionObject*, crx::TReflectionClassPropertyData*>>& data,
					   const std::string& relationPropertyDataText,					
					   std::function<bool(std::string&, TReflectionObject*)> converter=nullptr);

	public:
	// do parse and format in one call
	std::string ParseFormat(crx::TReflectionObject* owner,
							const std::string& relationPropertyDataText,
							std::function<bool(std::string&, TReflectionObject*)> converter=nullptr,
							const std::vector<std::string>& relationPropertyFoldData={});

	public:
	inline static std::string sOpenTokParse = "%";
	inline static std::string sCloseTokParse = ";";

	public:
	inline static std::string sDefaulFormat = "%s";
	inline static std::string sPercentFormat = "%s%%";
	
	public:
	std::string OpenTokParse = sOpenTokParse;
	std::string CloseTokParse = sCloseTokParse;
};

std::function<bool(std::string&, TReflectionObject*)> ArithmeticParseFormatConverter(bool forceFormat=true, const std::string& format=crx::TParseFormat::sDefaulFormat);

} /* namespace crx */

#endif // BASE_TPARSEFORMAT_H_
