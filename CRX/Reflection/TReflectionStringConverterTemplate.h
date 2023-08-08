#ifndef CRX_REFLECTION_TREFLECTIONSTRINGCONVERTERTEMPLATE_H_
#define CRX_REFLECTION_TREFLECTIONSTRINGCONVERTERTEMPLATE_H_

#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Base/TStringUtils.h>
#include <functional>

namespace crx {

namespace impl {

template<class CT>
class TReflectionStringConverterTemplate : public TReflectionStringConverter {
	public:
	typedef std::function<CT(const std::string&)> TFromString;
	typedef std::function<std::string(const CT&)> TToString;

	TReflectionStringConverterTemplate(TFromString from=nullptr, TToString to=nullptr) {
		SetNativeTypeInfo( &typeid(CT) );
		m_xFromString = from;
		m_xToString = to;
	}

	virtual ~TReflectionStringConverterTemplate()=default;

	public:
	virtual void Set(std::any& out, const std::string& value) override {
		if(m_xFromString) {
			out = m_xFromString(value);
		} else {
			out = from_string<CT>(value);
		}
	}

	virtual void Get(std::string& out, const std::any& value) override {
		if(m_xToString) {
			out = m_xToString( std::any_cast<const CT&>(value) );
		} else {
			out = std::to_string( std::any_cast<const CT&>(value) );
		}
	}

	TFromString m_xFromString = nullptr;
	TToString m_xToString = nullptr;
};

}//impl


}//crx






#endif /* REFLECTION_TREFLECTIONSTRINGCONVERTERTEMPLATE_H_ */
