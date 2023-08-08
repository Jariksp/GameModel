#include <CRX/XParse/TMapContainerWrapper.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Base/TDebug.h>

namespace crx {

TMapContainerWrapper::TMapContainerWrapper(const std::map<std::string, std::string>& data) : m_mData(data) {}

const std::string& TMapContainerWrapper::Value(const std::string& key) const {
	return m_mData.find(m_bUseLowerCase?ToLowerCase(key):key)->second;
}

bool TMapContainerWrapper::IsUseLowerCase() const { return m_bUseLowerCase; }

void TMapContainerWrapper::UseLowerCase(bool b) { m_bUseLowerCase = b; }

bool TMapContainerWrapper::IsHaveKey(const std::string& inkey) const {
	std::string key = inkey;
	if(m_bUseLowerCase) key = ToLowerCase(key);
	return m_mData.find(key)!=m_mData.end();
}

void TMapContainerWrapper::CheckForKey(std::string& name) const {
	if(m_bUseLowerCase) name = ToLowerCase(name);
	if( !IsHaveKey(name) ) _ERROR("key not found: "<<name);
}


bool TMapContainerWrapper::GetBool(std::string name, bool def) const {
	if( IsHaveKey(name) ) return GetBool(name);
	return def;
}

bool TMapContainerWrapper::GetBool(std::string name) const {
	CheckForKey(name);
	return crx::to_bool( Value(name) );
}

int TMapContainerWrapper::GetInt(std::string name, int def) const {
	if( IsHaveKey(name) ) return GetInt(name);
	return def;
}

int TMapContainerWrapper::GetInt(std::string name) const {
	CheckForKey(name);
	return crx::to_int( Value(name) );
}

const std::map<std::string, std::string>& TMapContainerWrapper::Data() const { return m_mData; }

std::string TMapContainerWrapper::GetString(std::string name, const std::string& def) const {
	if( IsHaveKey(name) ) return GetString(name);
	return def;
}

std::string TMapContainerWrapper::GetString(std::string name) const {
	CheckForKey(name);
	return Value(name);
}



} /* namespace crx */
