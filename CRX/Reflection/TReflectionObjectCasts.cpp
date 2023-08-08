#include <CRX/Reflection/TReflectionGlobals.h>
#include <CRX/Reflection/TReflectionObject.h>
#include <CRX/Reflection/TReflectionObjectCasts.h>
#include <CRX/Base/TMiscUtils.h>

namespace crx {

void TReflectionObjectCasts::Add(TReflectionObjectCasts *cast) {
	TReflectionGlobals::GetInstance()->Add(cast);
}

void TReflectionObjectCasts::CastFromAnyToAny(std::any &from, std::any &to) {
	auto fromTypeName = GetTypeNameFromAny(from);
	if(fromTypeName==TypeName()) {
		to = from;
	} else {
		auto cc = TReflectionGlobals::GetInstance()->FindCast(fromTypeName);
		auto refl = cc->Cast(from);
		Cast(refl, to);
	}
}

const std::string& TReflectionObjectCasts::Wrapper() { return m_sWrapper; }

TReflectionObjectCasts::NType TReflectionObjectCasts::Type() { return m_xType; }

bool TReflectionObjectCasts::IsObject() { return m_xType==NType::Object; }

bool TReflectionObjectCasts::IsPointer() { return m_xType==NType::Pointer; }

bool TReflectionObjectCasts::IsSmartPtr() { return m_xType==NType::SmartPtr; }

bool TReflectionObjectCasts::IsWeakPtr() { return false; }

}//crx
