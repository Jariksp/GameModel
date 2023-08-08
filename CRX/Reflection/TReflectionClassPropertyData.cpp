#include <CRX/Reflection/IReflectionProperyOperator.h>
#include <CRX/Reflection/TReflectionClassPropertyData.h>
#include <CRX/Reflection/TReflectionGlobals.h>
#include <CRX/Reflection/TReflectionTypeFactory.h>

namespace crx {

const std::string& TReflectionClassPropertyData::Name() { return m_sName; }

const std::string& TReflectionClassPropertyData::ClassType() { return m_sClassType; }

void TReflectionClassPropertyData::Add(TReflectionClassPropertyData *property) {
	TReflectionGlobals::GetInstance()->Add( property->m_sClassType, property );
}

TReflectionClassPropertyData* TReflectionClassPropertyData::Create(const std::string& classType, const std::string& name, IReflectionProperyOperatorGet* get, IReflectionProperyOperatorSet* set) {
	auto cl = new TReflectionClassPropertyData();
	cl->Init( classType, name, get, set );
	TReflectionClassPropertyData::Add( cl );
	return cl;
}

void TReflectionClassPropertyData::Init(const std::string &classType, const std::string &name, IReflectionProperyOperatorGet *get, IReflectionProperyOperatorSet *set) {
	m_sName = name;
	m_pGet = get;
	m_pSet = set;
	m_sClassType = classType;

	if(m_pGet) m_sTypeName = m_pGet->TypeName();
	else if(m_pSet) m_sTypeName = m_pSet->TypeName();
}

void TReflectionClassPropertyData::SetLazyInitializationChecker(IReflectionProperyLazyInitializationChecker* get) {
	m_pLazyInitializationChecker = get;
}

bool TReflectionClassPropertyData::IsLazyInitializationChecker() { return m_pLazyInitializationChecker; }

bool TReflectionClassPropertyData::IsLazyInitializationNullPtr(TReflectionObject* obj) {
	return m_pLazyInitializationChecker && m_pLazyInitializationChecker->IsNullptr(obj);
}


void TReflectionClassPropertyData::Set(TReflectionObject *obj, const std::any &copy_from) { m_pSet->Set(obj, copy_from); }

void TReflectionClassPropertyData::Get(TReflectionObject *obj, std::any &copy_to) { m_pGet->Get(obj, copy_to); }


TReflectionTypeFactory* TReflectionClassPropertyData::TypeFactory() {
	if(!m_pTypeFactory) m_pTypeFactory = TReflectionGlobals::GetInstance()->FindTypeFactory(m_sTypeName);
	return m_pTypeFactory;
}

TReflectionStringConverter* TReflectionClassPropertyData::StringConverter() {
	if(!m_pStringConverter) m_pStringConverter = TypeFactory()->StringConverter();
	return m_pStringConverter;
}

TReflectionObjectCasts* TReflectionClassPropertyData::Cast() { return TypeFactory()->Cast(); }

TReflectionContainerManipulator* TReflectionClassPropertyData::ContainerManipulator() { return TypeFactory()->ContainerManipulator(); }

bool TReflectionClassPropertyData::IsReadable() { return m_pGet; }
bool TReflectionClassPropertyData::IsWritable() { return m_pSet; }

void TReflectionClassPropertyData::SetCustomStringConverter(TReflectionStringConverter *conv) { m_pStringConverter = conv; }

}//crx



