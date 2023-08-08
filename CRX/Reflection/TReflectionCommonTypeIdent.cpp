#include <CRX/Base/TMiscUtils.h>
#include <CRX/Reflection/TReflectionCommonTypeIdent.h>


namespace crx {

const std::string& TReflectionCommonTypeIdent::TypeName() const { return m_sTypeName; }

void TReflectionCommonTypeIdent::SetNativeTypeInfo(const std::type_info* ti) {
	m_sTypeName = ConvertTypeName( ti->name() );
}

//void TCommonTypeIdent::TypeName(const string& tn) { m_sTypeName = tn; }

}
