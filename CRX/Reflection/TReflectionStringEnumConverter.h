#ifndef CRX_REFLECTION_TREFLECTIONSTRINGENUMCONVERTER_H_
#define CRX_REFLECTION_TREFLECTIONSTRINGENUMCONVERTER_H_

#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Reflection/magic_enum.h>
#include <CRX/Base/TStringUtils.h>

namespace crx {

namespace impl {


template<class CT>
class TReflectionStringEnumConverter : public TReflectionStringConverter, public TReflectionStringConverter::IStringSource {
	public:
	TReflectionStringEnumConverter() {
		SetNativeTypeInfo( &typeid(CT) );
		const auto& vv = magic_enum::enum_names<CT>();
		for(const auto& n:vv) {
			auto val = magic_enum::enum_cast<CT>(n).value();
			std::string str(n);
			m_vStringSource.push_back(str);
			str = ToLowerCase(str);
			m_vValue.push_back( std::make_pair(str, val) );
			m_vIntValue.push_back( std::make_pair(str, int(val)) );
		}

	}
	virtual ~TReflectionStringEnumConverter()=default;


	public:
	virtual void Set(std::any& out, const std::string& inVal) override {
		m_sMemCache = ToLowerCase(inVal);
		for(const auto& n:m_vValue) if(n.first==m_sMemCache) {
			out = n.second;
			return;
		}
		_ERROR("Can't convert '"<<inVal<<"' to enum: "<<TypeName());

	}

	virtual void Get(std::string& out, const std::any& value) override {
		out = magic_enum::enum_name( std::any_cast<CT>(value) );
	}

	public:
	virtual const std::vector<std::string>& StringSource() override { return m_vStringSource; }
	virtual const std::vector<std::pair<std::string, int> >& StringData() override { return m_vIntValue; }

	protected:
	std::string m_sMemCache;
	std::vector<std::pair<std::string, CT> > m_vValue;
	std::vector<std::string> m_vStringSource;
	std::vector<std::pair<std::string, int> > m_vIntValue;
};

}//impl

}//crx





#endif /* REFLECTION_TREFLECTIONSTRINGENUMCONVERTER_H_ */
