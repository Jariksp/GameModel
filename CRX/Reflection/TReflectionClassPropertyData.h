#ifndef CRX_TCLASSFIELDDATA__H_H_H
#define CRX_TCLASSFIELDDATA__H_H_H

#include <CRX/Reflection/TReflectionCommonTypeIdent.h>
#include <any>


namespace crx {

class TReflectionTypeFactory;
class TReflectionStringConverter;
class TReflectionObjectCasts;
class TReflectionContainerManipulator;
class IReflectionProperyOperatorGet;
class IReflectionProperyOperatorSet;
class TReflectionObject;
class IReflectionProperyLazyInitializationChecker;

// представляет собой поле класса, к которому можно обращаться
class TReflectionClassPropertyData : public TReflectionCommonTypeIdent {
	public:
	TReflectionClassPropertyData()=default;
	virtual ~TReflectionClassPropertyData()=default;

	public:
	void Init(const std::string& classType, const std::string& name, IReflectionProperyOperatorGet* get, IReflectionProperyOperatorSet* set);
	void SetLazyInitializationChecker(IReflectionProperyLazyInitializationChecker* get);

	static void Add(TReflectionClassPropertyData* property);
	static TReflectionClassPropertyData* Create(const std::string& classType, const std::string& name, IReflectionProperyOperatorGet* get, IReflectionProperyOperatorSet* set);

	public:
	const std::string& Name();
	const std::string& ClassType();

	public:
	bool IsReadable();
	bool IsWritable();


	public:
	TReflectionTypeFactory* TypeFactory();
	TReflectionStringConverter* StringConverter();
	TReflectionObjectCasts* Cast();
	TReflectionContainerManipulator* ContainerManipulator();

	void SetCustomStringConverter(TReflectionStringConverter* conv);

	public:
	void Set(TReflectionObject* obj, const std::any& copy_from);
	void Get(TReflectionObject* obj, std::any& copy_to);

	public:
	bool IsLazyInitializationChecker();
	bool IsLazyInitializationNullPtr(TReflectionObject* obj);// в поле - nullptr, серрелизации не подлежит


	protected:
	std::string m_sName;

	IReflectionProperyOperatorGet* m_pGet = nullptr;
	IReflectionProperyOperatorSet* m_pSet = nullptr;
	IReflectionProperyLazyInitializationChecker* m_pLazyInitializationChecker = nullptr;
	std::string m_sClassType;

	protected:
	TReflectionTypeFactory* m_pTypeFactory = nullptr;
	TReflectionStringConverter* m_pStringConverter = nullptr;
};



};



/*


class TClassPropertyData {
	public:
	bool IsEqual(TDispatch* obj, const TDispatchDataPtr& d);
	bool IsEqual(TDispatch* d1, TDispatch* d2);

	public:




	public:

	TDispatchDataPtr GetWeak(TDispatch* obj);
	TDispatch* GetDispatchPtr(TDispatch* obj);//if weak, return return if, overvise standart get, return nullptr, if not TDispatch
	bool IsDispatchPtr();


	protected:

};


template<class CL, class CV >
TClassPropertyData* CreateClassPropertyData( const string& propertyName, CV(CL::*get)(void) ) {
	auto cl = new TClassPropertyDataTempate<CV>();
	cl->Init(TAbiExt::GetTypeName<CL>(), propertyName, new TProperyOperatorFunctionGet<CL, CV>(get), nullptr );
	TClassPropertyData::AddProperty( cl );
	return cl;
}

template<class CL, class CV >
TClassPropertyData* CreateClassPropertyData( const string& propertyName, void(CL::*set)(CV) ) {
	auto cl = new TClassPropertyDataTempate<CV>();
	cl->Init(TAbiExt::GetTypeName<CL>(), propertyName, nullptr, new TProperyOperatorFunctionSet<CL, CV>(set) );
	TClassPropertyData::AddProperty( cl );
	return cl;
}

template<class CL, class CV >
IProperyOperatorGet* CreatePropertyWeakPtr(CV& get) {
	return new TProperyOperatorMemberGet<CL, CV>(get);
}

*/

#endif
