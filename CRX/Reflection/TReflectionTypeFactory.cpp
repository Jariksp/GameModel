#include <CRX/Reflection/TReflectionTypeFactory.h>
#include <CRX/Reflection/TReflectionGlobals.h>
#include <CRX/Base/TMiscUtils.h>

namespace crx {

void TReflectionTypeFactory::Add(TReflectionTypeFactory* fac, bool force) {
	TReflectionGlobals::GetInstance()->Add(fac,force);
}

const std::vector<std::string>& TReflectionTypeFactory::Parents() const { return m_vParents; }

void TReflectionTypeFactory::AddParent(const std::string& parent) { m_vParents.push_back(parent); }


bool TReflectionTypeFactory::CheckAndSetClassTypeAlreadyInited(const std::string &classType) {
	auto gi = TReflectionGlobals::GetInstance();
	if( gi->IsClassTypeInited(classType) ) return false;
	gi->SetClassTypeInited(classType);
	return true;
}

bool TReflectionTypeFactory::IsHave(const std::string &type) {
	return TReflectionGlobals::GetInstance()->FindTypeFactory(type)!=nullptr;
}

TReflectionStringConverter* TReflectionTypeFactory::StringConverter() {
	if(!m_bStringConverterInited) {
		m_pStringConverter = TReflectionGlobals::GetInstance()->FindStringConverter(m_sTypeName);
		m_bStringConverterInited = true;
	}
	return m_pStringConverter;
}

TReflectionObjectCasts* TReflectionTypeFactory::Cast() {
	if(!m_bCastInited) {
		m_pCast = TReflectionGlobals::GetInstance()->FindCast(m_sTypeName);
		m_bCastInited = true;
	}
	return m_pCast;
}

bool TReflectionTypeFactory::IsEqual(const std::any &a, const std::any &b) {
	if( a.type()!=b.type() ) _ERROR("Can't compare two differet types. a is "<<ConvertTypeName(a.type().name())<<" b is "<<ConvertTypeName(b.type().name()) );
	return DoIsEqual(a, b);
}

TReflectionContainerManipulator* TReflectionTypeFactory::ContainerManipulator() {
	if(!m_bContainerManipulatorInited) {
		m_pContainerManipulator = TReflectionGlobals::GetInstance()->FindContainerManipulator(m_sTypeName);
		m_bContainerManipulatorInited = true;
	}
	return m_pContainerManipulator;
}

}//crx


/*

#include <TTypeFactory.h>
#include <TGlobalDispatch.h>
#include <TAbiExt.h>
#include <TStringConverter.h>
#include <TBinaryConverter.h>
#include <debug.h>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

TTypeFactory::TTypeFactory() {
	m_xFactoryType = NNTypeFactoryType::None;
	m_pStringConverter = 0;
	m_pBinaryConverter = 0;
}





TBinaryConverter* TTypeFactory::BinaryConverter() const {
	if( !m_pBinaryConverter ) m_pBinaryConverter = TGlobalDispatch::GetInstance()->FindBinaryConverter( m_sTypeName );
	if( !m_pBinaryConverter ) _ERROR("BinaryConverter for "<<TypeName()<<" not found!");
	return m_pBinaryConverter;
}

TStringConverter* TTypeFactory::StringConverter() const {
	if( !m_pStringConverter ) m_pStringConverter = TGlobalDispatch::GetInstance()->FindStringConverter( m_sTypeName );
	if( !m_pStringConverter ) _ERROR("StringConverter for "<<TypeName()<<" not found!");
	return m_pStringConverter;
}

bool TTypeFactory::IsHaveStringConverter() const {
	return m_pStringConverter || TGlobalDispatch::GetInstance()->FindStringConverter( m_sTypeName );
}


void TTypeFactory::SetNativeTypeInfo(const std::type_info* ti) {
	_SetNativeTypeInfo(ti);
	
}

#define IMPL(xx) bool TTypeFactory::Is##xx() const { return FactoryType()==NNTypeFactoryType::xx; }
IMPL(Simple);
IMPL(Pointer);
IMPL(Complex);
IMPL(Container);
#undef IMPL

bool TTypeFactory::IsDispatch() const { return IsComplex(); }

NTypeFactoryType TTypeFactory::FactoryType() const {
	if( m_xFactoryType==NNTypeFactoryType::None )
		m_xFactoryType = TGlobalDispatch::GetInstance()->FactoryType( m_sTypeName );
	return m_xFactoryType;
}


TDispatchDataPtr TTypeFactory::Create() {
	auto a = new TDispatchData(New(), this);
	a->NeedDelete(true);
	return a;

}

*/

