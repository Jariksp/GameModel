#ifndef BASE_TLOWERCASEDSTRING_H_
#define BASE_TLOWERCASEDSTRING_H_

#include <string>

namespace crx {

class TLowerCasedString {
	public:
	TLowerCasedString()=default;
	TLowerCasedString(const std::string& str);
	~TLowerCasedString()=default;

	public:
	const std::string& Data() const;
	const std::string& LowerCaseData() const;;

	public:
	void Set(const std::string& d);
	void operator=(const std::string& d);


	protected:
	std::string m_sData;
	std::string m_sLowerCaseData;

};

} /* namespace crx */

#endif /* BASE_TLOWERCASEDSTRING_H_ */
