#ifndef CRX_TCOMMONTYPEFACTORY_H_H_
#define CRX_TCOMMONTYPEFACTORY_H_H_

#include <typeinfo>
#include <string>


namespace crx {

class TReflectionCommonTypeIdent {
	public:
	TReflectionCommonTypeIdent()=default;
	virtual ~TReflectionCommonTypeIdent()=default;

	public:
	const std::string& TypeName() const;

	protected:
	virtual void SetNativeTypeInfo(const std::type_info* ti);
//	void TypeName(const string& tn);

	protected:
	std::string m_sTypeName;
};


}//crx






#endif
