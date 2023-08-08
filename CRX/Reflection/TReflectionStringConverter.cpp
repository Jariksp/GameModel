#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Reflection/TReflectionGlobals.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Base/TDebug.h>

namespace crx {

void TReflectionStringConverter::Add(TReflectionStringConverter *conv) {
	TReflectionGlobals::GetInstance()->Add(conv);
}

bool TReflectionStringConverter::IsHave(const std::string &typeName) {
	return TReflectionGlobals::GetInstance()->FindStringConverter(typeName)!=nullptr;
}

void TReflectionStringConverter::SetCustomFromString(const std::string &className, const std::string &publishAs, TReflectionStringConverter *conv) {
	TReflectionGlobals::GetInstance()->SetCustomStringConverter(className, publishAs, conv);
}


int TReflectionStringConverter::IStringSource::NameToInt(const std::string &name) {
	const auto& vv = StringData();
	auto ll = ToLowerCase(name);
	for(auto& a:vv) if(a.first==ll) return a.second;
	_ERROR("Name: "<<name<<" not found");
	return 0;
}


} //crx

