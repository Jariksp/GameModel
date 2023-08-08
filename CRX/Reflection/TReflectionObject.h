#ifndef CRX_TDISPATCH_H_H_
#define CRX_TDISPATCH_H_H_

#include <any>
#include <string>
#include <functional>
#include <memory>
#include <CRX/Reflection/NNSerializerFlags.h>

#ifdef _MSC_VER
#pragma warning(disable: 4250)
#endif

#define DECL_REFLECTION_TABLE() \
	static crx::TCallFunctionBeforMain CallInitReflectionTable;\
	static void InitReflectionTable();


namespace crx {

struct TCallFunctionBeforMain {
	TCallFunctionBeforMain(void(*fn)(void)) { fn(); }
};

class TReflectionClassPropertyData;
class TReflectionClassFunctionData;

// да-да, пришлось отнаследоваться от enable_shared_from_this иначе реализация работы решлекшена с shared_ptr
// превращалась в какую-то жесть.

class TReflectionObject : public virtual std::enable_shared_from_this<TReflectionObject> {
	public:
	TReflectionObject()=default;
	virtual ~TReflectionObject()=default;
	static void InitReflectionTable();


	public:
	virtual std::string TypeName() const;

	public:
	TReflectionClassPropertyData* FindProperty(const std::string& fieldName);
	TReflectionClassFunctionData* FindFunction(const std::string& fieldName);

	public:
	void SetPropertyData(const std::string& fieldName, const std::any& data);
	void GetPropertyData(const std::string& fieldName, std::any& saveTo);

	public:
	void SetPropertyFromString(const std::string& fieldName, const std::string& value);
	void SetPropertyFromString(TReflectionClassPropertyData* property, const std::string& value);
	std::string GetPropertyToString(const std::string& fieldName);
	std::string GetPropertyToString(TReflectionClassPropertyData* property);

	std::string GetPropertyToStringNoError(const std::string& fieldName);// если нет такого поля, то просто вернет пустую строку

	public:
	virtual std::string SerializeShared(NSerializerFlags flags=NNSerializerFlags::None, const std::string& rootNodeName="Object");//as shared_ptr
	virtual std::string SerializeShared(const std::string& rootNodeName);//bridge

	template<typename TT=TReflectionObject>
	static std::shared_ptr<TT> DeserializeShared(const std::string& data, NSerializerFlags flags=NNSerializerFlags::None) {
		return std::dynamic_pointer_cast<TT>( DeserializeSharedObject(data, flags) );
	}

	template<typename TT=TReflectionObject>
	static TT* DeserializePointered(const std::string& data, NSerializerFlags flags=NNSerializerFlags::None) {
		return dynamic_cast<TT*>( DeserializePointeredObject(data, flags) );
	}

	static std::shared_ptr<TReflectionObject> DeserializeSharedObject(const std::string& data, NSerializerFlags flags=NNSerializerFlags::None);
	static TReflectionObject* DeserializePointeredObject(const std::string& data, NSerializerFlags flags=NNSerializerFlags::None);

	public:
	virtual TReflectionObject* CloneThisReflection();

	template<class TT>
	TT* NewThis() {
		return dynamic_cast<TT*>( CloneThisReflection() );
	}



	public:
	// для простоты принимает this, чтобы не писатать decltype
	template<class T>
	std::shared_ptr<T> _get_this(T* sthis) { return std::dynamic_pointer_cast<T>( shared_from_this() ); }

	public:// чтобы упростить касты
	template<class TT>
	std::shared_ptr<TT> pcast() { return std::dynamic_pointer_cast<TT>(shared_from_this()); }

	template<class TT>
	TT* cast() { return dynamic_cast<TT*>(this); }

};


}//crx


/*

class TDispatch  {
	public:
	TDispatch* Clone();


	public:
	virtual void Serialize(IDispatchSerializer* data);
	virtual void DeSerialize(IDispatchDeserializer* data);



	public:
	bool IsPropertyEqual(const std::string& fieldName, const TDispatchDataPtr& data);

	public:
	bool IsDispatchEqual(TDispatch& rhs);
	bool operator==(const TDispatch& rhs) const;// bridge to IsEqual


};

*/





#endif
