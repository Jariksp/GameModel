#ifndef CRX_TGLOBALDISPATCH_H_H_
#define CRX_TGLOBALDISPATCH_H_H_

#include <CRX/Base/TSingleton.h>
#include <map>
#include <any>
#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Base/TMiscUtils.h>


namespace crx {

class TReflectionTypeFactory;// фабрика, может создавать, копировать и сравнимать объекты
class TReflectionClassPropertyData;// данные о поле класса. позволяет получать к нему доступ, класс должен быть потомком от TReflectionObject
class TReflectionClassFunctionData;// данные о ф-ции класса. позволяет её вызывать и получать/передавать данные
class TReflectionStringConverter;// конертер в строку и отбратно TReflectionData
class TReflectionContainerManipulator;// манипулятор контейнерами, vector/map
class TReflectionObjectCasts;// класс для каста конечного объекта в указатель на TReflectionObject.
class TReflectionObject;

// этот класс является контейнером для всех данных рефлекшена.
// он собирает данные по типам, классам, конвертеры и манипуляторы
// сам TReflection обращается к нему
//
// ВАЖНО! Оно ловеркейсит названия всех типов и publish as, поэтому поиск string и String вернет одно и то же!

class TReflectionGlobals : public TCreativeSingleton<TReflectionGlobals> {
	public:
	void Add(TReflectionTypeFactory* factory, bool force=false);
	void Add(const std::string& classType, TReflectionClassPropertyData* cfd);
	void Add(const std::string& classType, TReflectionClassFunctionData* cfd);
	void Add(TReflectionStringConverter* conv);
	void Add(TReflectionContainerManipulator* manipulator);
	void Add(TReflectionObjectCasts* cast);



	public:
	TReflectionTypeFactory* FindTypeFactory(const std::string& typeName);
	TReflectionClassPropertyData* FindPropertyData(const std::string& classType, const std::string& publishedAs);
	TReflectionClassFunctionData* FindFunctionData(const std::string& classType, const std::string& publishedAs);
	TReflectionStringConverter* FindStringConverter(const std::string& typeName);// для полей класс использовать только TReflectionClassPropertyData::StringConverter !!
	TReflectionContainerManipulator* FindContainerManipulator(const std::string& typeName);
	TReflectionObjectCasts* FindCast(const std::string& typeName);


	public:
	template<class TT>
	TT* CreateReflection(const std::string& typeName) {
		return dynamic_cast<TT*>( CreateReflection(typeName) );
	}

	TReflectionObject* CreateReflection(const std::string& typeName);

	public:
	void SetCustomStringConverter(const std::string& classType, const std::string& publishedAs, TReflectionStringConverter* conv);

	public:
	std::string ToString(const std::any& data);
	template<class TT>
	TT FromString(const std::string& val) {
		auto conv = FindStringConverter( crx::GetTypeName<TT>() );
		std::any data;
		conv->Set(data, val);
		return std::any_cast<TT>(data);
	}

	public:
	bool IsChildOf(const std::string& typeName, const std::string& parentTypeName);

	public:
	// это супер-тупая хуйня. просто перебирает и засывыват
	void Fill(TReflectionObject* obj, const std::map<std::string, std::string>& data);

	public:
	bool IsClassTypeInited(const std::string& classType);
	void SetClassTypeInited(const std::string& classType);

	const std::map<std::string, TReflectionClassPropertyData*>* GetAllPropertys(const std::string& classType);
	const std::map<std::string, TReflectionClassFunctionData*>* GetAllFunctions(const std::string& classType);
	const std::map<std::string, TReflectionTypeFactory*>& GetAllTypeFactories();

	public:
	// не учитывает проперти предков
	const std::map<std::string, TReflectionClassPropertyData*>& GetAllPropertysInThisClassOnly(const std::string& classType);

	protected:
	//[classType]
	std::map< std::string, std::map<std::string, TReflectionClassPropertyData*> > m_mFieldData;
	std::map< std::string, std::map<std::string, TReflectionClassFunctionData*> > m_mFunctionData;

	std::map<std::string, TReflectionTypeFactory*> m_mTypeFactories;
	std::map<std::string, TReflectionStringConverter*> m_mStringConverters;
	std::map<std::string, TReflectionContainerManipulator*> m_mContainerManipulators;
	std::map<std::string, TReflectionObjectCasts*> m_mCasts;



	std::map< std::string, std::map<std::string, TReflectionClassPropertyData*>* > m_mGetAllFieldsCache;
	std::map< std::string, std::map<std::string, TReflectionClassFunctionData*>* > m_mGetAllFunctionsCache;

	std::map<std::string, std::string> m_mInitedClassType;


	private:
	friend class TCreativeSingleton<TReflectionGlobals>;
	TReflectionGlobals();
	virtual ~TReflectionGlobals()=default;

};

}//crx

/*


#include <BaseTypes.h>
#include <TSingletons.h>
#include <map>
#include <vector>
#include <string>
#include <NNTypeFactoryType.h>
#include <TAbiExt.h>


class TTypeFactory;
class TClassPropertyData;
class TStringConverter;
class TContainerManipulator;
class TBinaryConverter;
class TClassFunctionData;



class TGlobalDispatch : public TSingletons::TCreativeSingleton<TGlobalDispatch> {
	public:

	vector<string> GetAllTypeFactories(bool lowerCase=false);

	static void InitAllStatic();

	public:
	bool IsChildOf(const string& typeName, const string& parentTypeName);

	public:

	map<string, TClassPropertyData*> GetOnlyItPropertys(const string& classType);


	public:



	public:

	vector<string> GetAllStringConverters(bool lowerCase=false);



	public:

	vector<string> GetAllContainerManipulators(bool lowerCase=false);

	public:

	NTypeFactoryType FactoryType(const string& typeName);

	public:
	template<class TT>
	string ValueToString(const TT& val) {
		auto str_conv = FindStringConverter( TAbiExt::GetTypeName<TT>() );
		string data;
		str_conv->DoGet(data, &val);
		return data;
	}

	template<class TT>
	void StringToValue(TT& val, const string& data) {
		auto str_conv = FindStringConverter( TAbiExt::GetTypeName<TT>() );
		str_conv->DoSet(&val, data);
	}

	private:
	bool IsComplex(const string& tn);
	
	
	private:


};


	void AddBinaryConverter(TBinaryConverter*);
	TBinaryConverter* FindBinaryConverter(const string& typeName);
	vector<string> GetAllBinaryConverters(bool lowerCase=false);
	map<string, TBinaryConverter*> m_mBinaryConverters;

*/





#endif
