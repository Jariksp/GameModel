#include <CRX/Base/TLowerCasedString.h>
#include <CRX/Base/TStringUtils.h>

namespace crx {

TLowerCasedString::TLowerCasedString(const std::string& str) { Set(str); }
void TLowerCasedString::operator =(const std::string &d) { Set(d); }

const std::string& TLowerCasedString::Data() const { return m_sData; }

const std::string& TLowerCasedString::LowerCaseData() const { return m_sLowerCaseData; }

void TLowerCasedString::Set(const std::string &d) {
	m_sData = d;
	m_sLowerCaseData = ToLowerCase(d);
}



} /* namespace crx */
