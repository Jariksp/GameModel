#ifndef CRX_REFLECTION_TCASTTOREFLECTIONOBJECT2_H_
#define CRX_REFLECTION_TCASTTOREFLECTIONOBJECT2_H_

#include <CRX/Reflection/TReflectionCommonTypeIdent.h>
#include <any>

namespace crx {

class TReflectionObject;

// единственная задача этого класса - "выковыривать" указанатель на TReflectionObject из
// любого места. из shared_ptr, поля класса или указателя на объект-потомок - он должен
// вернуть указатель на TReflectionObject. Это нужно для того, чтобы заполнить/считать его поля

class TReflectionObjectCasts : public TReflectionCommonTypeIdent {
	public:
	TReflectionObjectCasts()=default;
	virtual ~TReflectionObjectCasts()=default;

	static void Add(TReflectionObjectCasts* cast);

	public:
	virtual TReflectionObject* Cast(std::any& from)=0;//from me to TReflectionObject
	virtual void Cast(TReflectionObject* from, std::any& to)=0;// from TReflectionObject to me
	virtual void CastFromAnyToAny(std::any& from, std::any& to);

	// если завернут в "умный" указатель, то возвращает тип этого указателя.
	// просто в нем может находится потомок указанно в поле типа и нужно это как-то разгребать все
	virtual const std::string& Wrapper();

	virtual bool IsWeakPtr();

	public:// с чего кастить будем
	enum class NType {
		None=0,
		Object=1,
		Pointer=2,
		SmartPtr=3
	};

	NType Type();

	bool IsObject();
	bool IsPointer();
	bool IsSmartPtr();//shared_ptr OR weak_ptr. unique_ptr in future


	protected:
	std::string m_sWrapper;
	NType m_xType = NType::None;
};



} //crx


#endif /* REFLECTION_TCASTTOREFLECTIONOBJECT_H_ */
