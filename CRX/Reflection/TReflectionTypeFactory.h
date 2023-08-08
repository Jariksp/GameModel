#ifndef CRX_TBASETYPEFACTORY_H_H_
#define CRX_TBASETYPEFACTORY_H_H_

#include <CRX/Reflection/TReflectionCommonTypeIdent.h>
#include <any>
#include <vector>


namespace crx {

class TReflectionStringConverter;
class TReflectionObjectCasts;
class TReflectionContainerManipulator;

// создает экземпляр обеъекта в точности, как он задан в типе. Т.е. если это типа TReflectionObject, то
// в возвращаемом значении будет именно TReflectionObject, а не TReflectionObject*
// если нужен именно созданный через new объект и указатель на него, то нужно вызывать Create для
// factory типа TReflectionObject*. Она создаст через new объект, и поместит поинтер на него в возвращаемый any
// так же работает с shared_ptr, т.е. если фектори для shared_ptr<TReflectionObject>, то в Create будет
// выполнено make_any( MakeShared<TReflectionObject>() );
// для абстрактых объектов возращает ПУСТО

class TReflectionTypeFactory : public TReflectionCommonTypeIdent {
	public:
	TReflectionTypeFactory()=default;
	virtual ~TReflectionTypeFactory()=default;

	public:
	static void Add(TReflectionTypeFactory* fac, bool force=false);
	static bool IsHave(const std::string& type);
	static bool CheckAndSetClassTypeAlreadyInited(const std::string& classType);

	public:
	virtual std::any Create()=0;
	bool IsEqual(const std::any& a, const std::any& b);

	public:
	const std::vector<std::string>& Parents() const;
	void AddParent(const std::string& parent);

	public:
	TReflectionStringConverter* StringConverter();
	TReflectionObjectCasts* Cast();
	TReflectionContainerManipulator* ContainerManipulator();

	protected:
	virtual bool DoIsEqual(const std::any& a, const std::any& b)=0;

	protected:
	std::vector<std::string> m_vParents;

	protected:
	bool m_bStringConverterInited = false;
	TReflectionStringConverter* m_pStringConverter = nullptr;
	bool m_bCastInited = false;
	TReflectionObjectCasts* m_pCast = nullptr;
	bool m_bContainerManipulatorInited = false;
	TReflectionContainerManipulator* m_pContainerManipulator = nullptr;

};


};



/*

#include <TDispatchData.h>
#include <NNTypeFactoryType.h>


class TStringConverter;
class TBinaryConverter;
class TGlobalDispatch;
class TDispatch;

class TTypeFactory  {
	public:
	TTypeFactory();
	virtual ~TTypeFactory();


	TBinaryConverter* BinaryConverter() const;
	TStringConverter* StringConverter() const;
	bool IsHaveStringConverter() const;

	public:
	TDispatchDataPtr Create();


	public:
	bool IsSimple() const;
	bool IsComplex() const;
	bool IsContainer() const;
	bool IsPointer() const;
	bool IsDispatch() const;

	public:
	NTypeFactoryType FactoryType() const;


	friend class TDispatchData;
	public:
	virtual void* New() const = 0;
	virtual void Delete(void*) const = 0;
	virtual void Copy(void* to, const void* from) const = 0;
	virtual bool IsEqual(const void*, const void*) const = 0;

	public://some new interface
	virtual bool IsDispatchPtr() const = 0;
	virtual TDispatch* NewDispatch() const = 0;
	virtual void* FromDispatchPtr(TDispatch* d) const =0;
	virtual TDispatch* ToDispatchPtr(void* value ) const =0;

	protected:
	virtual void SetNativeTypeInfo(const std::type_info* ti);

	protected:

	mutable NTypeFactoryType m_xFactoryType;
	mutable TStringConverter* m_pStringConverter;
	mutable TBinaryConverter* m_pBinaryConverter;

};
*/

#endif
