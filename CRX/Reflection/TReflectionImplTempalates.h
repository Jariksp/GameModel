#ifndef CRX_REFLECTION_TREFLECTIONIMPLTEMPALATES_H_
#define CRX_REFLECTION_TREFLECTIONIMPLTEMPALATES_H_

#include <CRX/Reflection/TReflectionClassPropertyData.h>
#include <CRX/Reflection/TReflectionClassFunctionData.h>
#include <CRX/Reflection/TReflectionTypeFactoryTemplate.h>
#include <CRX/Reflection/TReflectionVectorContainerManipulator.h>
#include <CRX/Reflection/TReflectionMapContainerManipulator.h>
#include <CRX/Reflection/TReflectionObjectCastsTemplate.h>
#include <CRX/Reflection/TReflectionFunctionOperatorTemplate.h>
#include <CRX/Reflection/TReflectionProperyOperatorTemplate.h>
#include <CRX/Reflection/TReflectionStringEnumConverter.h>
#include <CRX/Base/TDebug.h>

namespace crx {

namespace impl {

template < typename T, typename std::enable_if< (!std::is_pointer<T>::value || !std::is_base_of<TReflectionObject, std::remove_pointer_t<T>>::value), void** >::type = nullptr >
void TryCallInitReflectionTable() {}

template < typename T, typename std::enable_if< (std::is_pointer<T>::value && std::is_base_of<TReflectionObject, std::remove_pointer_t<T>>::value), void** >::type = nullptr >
void TryCallInitReflectionTable() {
	std::remove_pointer_t<T>::InitReflectionTable();
}


template<typename TT>
void AddVectorManipulatorAndTypeFactoryIfNeed(TT& vv) {}


template<typename TT>
void AddVectorManipulatorAndTypeFactoryIfNeed(std::vector<TT>& vv) {
	AddReflectionTypeFactoryTemplateIfNo<TT>();
	AddReflectionVectorContainerManipulatorIfNo<TT>();
	AddReflectionTypeFactoryTemplateIfNo< std::vector<TT> >();
}


template<typename TT>
void AddMapManipulatorAndTypeFactoryIfNeed(TT& vv) {}

template<typename TKey, typename TValue>
void AddMapManipulatorAndTypeFactoryIfNeed(std::map<TKey, TValue>& vv) {
	AddReflectionTypeFactoryTemplateIfNo<TKey>();
	AddReflectionTypeFactoryTemplateIfNo<TValue>();
	AddReflectionMapContainerManipulatorIfNo<TKey,TValue >();
	AddReflectionTypeFactoryTemplateIfNo< std::map<TKey, TValue> >();
}

template<class TT>
void AddCastToReflectionObject() {
	TReflectionObjectCasts::Add( new TReflectionObjectCastsPointerTemplate<TT*>() );
	TReflectionObjectCasts::Add( new TReflectionObjectCastsTemplate<TT>() );
	TReflectionObjectCasts::Add( new TReflectionObjectSharedPtrCastsTemplate<std::shared_ptr<TT>>() );
	TReflectionObjectCasts::Add( new TReflectionObjectWeakPtrCastsTemplate<std::weak_ptr<TT>>() );
}


template < typename T, typename std::enable_if< (!std::is_enum<T>::value), void** >::type = nullptr >
void TryAddEnumReflectionStringConverver() {}

template < typename T, typename std::enable_if< (std::is_enum<T>::value), void** >::type = nullptr >
void TryAddEnumReflectionStringConverver() {
	if( TReflectionStringConverter::IsHave( GetTypeName<T>()) ) return;
	AddReflectionTypeFactoryTemplateIfNo<T>();
	TReflectionStringConverter::Add( new TReflectionStringEnumConverter<T>() );
}



// ------------------------------------------------- //


template<class CL, class CV >
TReflectionClassPropertyData* CreateClassPropertyData( const std::string& propertyName, CV& get, CV& set ) {
	AddReflectionTypeFactoryTemplateIfNo<CV>();
	TryAddEnumReflectionStringConverver<CV>();
	return TReflectionClassPropertyData::Create(
			GetTypeName<CL>(), propertyName, new TProperyOperatorMemberGetSet<CL, CV>(get), new TProperyOperatorMemberGetSet<CL, CV>(set)
	);
}

template<class CL, class CV >
TReflectionClassPropertyData* CreateClassPropertyData( const std::string& propertyName, CV(CL::*get)(void), void(CL::*set)(CV) ) {
	TryAddEnumReflectionStringConverver<CV>();
	return TReflectionClassPropertyData::Create(
			GetTypeName<CL>(), propertyName, new TProperyOperatorFunctionGet<CL, CV>(get), new TProperyOperatorFunctionSet<CL, CV>(set)
	);
}

template<class CL, class CV >
TReflectionClassPropertyData* CreateClassPropertyData( const std::string& propertyName, CV(CL::*get)(void) ) {
	TryAddEnumReflectionStringConverver<CV>();
	return TReflectionClassPropertyData::Create(
			GetTypeName<CL>(), propertyName, new TProperyOperatorFunctionGet<CL, CV>(get), nullptr
	);
}

template<class CL, class CV >
TReflectionClassPropertyData* CreateClassPropertyData( const std::string& propertyName, void(CL::*set)(CV) ) {
	TryAddEnumReflectionStringConverver<CV>();
	return TReflectionClassPropertyData::Create(
			GetTypeName<CL>(), propertyName, nullptr, new TProperyOperatorFunctionSet<CL, CV>(set)
	);
}


template<class CL, class CV, class CVV>
TReflectionClassPropertyData* CreateClassPropertyDataWithWeak( const std::string& propertyName, CV(CL::*get)(void), void(CL::*set)(CV), CVV& getWeak ) {
	auto cls = CreateClassPropertyData(propertyName, get, set);
	auto weak = new TProperyLazyInitializationChecker<CL, CVV>(getWeak);
	cls->SetLazyInitializationChecker(weak);
	return cls;
}


// ------------------------------------------------- //
template<class CL>
TReflectionClassFunctionData* CreateClassFunctionData(const std::string& publishAs, void(CL::*set)(void)) {
	auto op = new TFunctionOperatorVoid<CL>(set);
	return TReflectionClassFunctionData::Add(publishAs, op);
}

template<class CL, class VA>
TReflectionClassFunctionData* CreateClassFunctionData(const std::string& publishAs, void(CL::*set)(VA)) {
	TryAddEnumReflectionStringConverver<VA>();
	auto op = new TFunctionOperatorOne<CL, VA>(set);
	return TReflectionClassFunctionData::Add(publishAs, op);
}

template<class CL, class VA, class VB>
TReflectionClassFunctionData* CreateClassFunctionData(const std::string& publishAs, void(CL::*set)(VA, VB)) {
	TryAddEnumReflectionStringConverver<VA>();
	TryAddEnumReflectionStringConverver<VB>();
	auto op = new TFunctionOperatorTwo<CL, VA, VB>(set);
	return TReflectionClassFunctionData::Add(publishAs, op);
}

// ---------------------------------------- //

template<class CV>
class TCustomFromStringConverter : public TReflectionStringConverter {
	public:
	TCustomFromStringConverter(CV(*set)(const std::string&)) { m_pSet = set; };
	virtual ~TCustomFromStringConverter()=default;

	public:
	virtual void Set(std::any& out, const std::string& value) override {
		out = m_pSet(value);
	}

	virtual void Get(std::string& out, const std::any& value) override {
		_ERROR("ToString ont IMPL in TCustomFromStringConverter");
	}


	protected:
	CV(*m_pSet)(const std::string&) = nullptr;

};

template<class CL, class CV >
void SetCustomFromString(const std::string& publishAs, CV& get, CV(*set)(const std::string&) ) {
	auto conv = new TCustomFromStringConverter<CV>(set);
	TReflectionStringConverter::SetCustomFromString(GetTypeName<CL>(), publishAs, conv);
}



}//impl





}//crx




#endif /* REFLECTION_TREFLECTIONIMPLTEMPALATES_H_ */
