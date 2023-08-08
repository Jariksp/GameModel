#ifndef CRX_TCONTAINERMANIPULATOR_H
#define CRX_TCONTAINERMANIPULATOR_H

#include <CRX/Reflection/TReflectionCommonTypeIdent.h>
#include <any>

namespace crx {

class TReflectionTypeFactory;

// управляет контейнерами. они бывают типа vector, в которых нельзя упралять индексом и типа map,
// в которых необходимо задавать и индекс и значениs

class TReflectionContainerManipulator : public TReflectionCommonTypeIdent {
	public:
	TReflectionContainerManipulator()=default;
	virtual ~TReflectionContainerManipulator()=default;

	public:
	static void Add(TReflectionContainerManipulator* man);
	static bool IsHave(const std::string& typeName);

	public:
	const std::string& IndexTypeName() const;
	const std::string& ValueTypeName() const;

	TReflectionTypeFactory* IndexTypeFactory();
	TReflectionTypeFactory* ValueTypeFactory();

	public:
	virtual void IndexAt(const std::any& container_ptr, unsigned pos, std::any& out)=0;
	virtual void ValueAt(const std::any& container_ptr, unsigned pos, std::any& out)=0;
	virtual unsigned Size(const std::any& container_ptr)=0;

	virtual void AddElement(std::any& container_ptr, const std::any& index_data, const std::any& value_data)=0;

	virtual void Clear(std::any& container_ptr)=0;

	protected:
	std::string m_sIndexTypeName;
	std::string m_sValueTypeName;
};



}//crx

/*
#include <string>


#include <TCommonTypeIdent.h>
#include <TDispatchData.h>

// TContainerManipulator is interface for mainpulating stl containers

class TContainerManipulator : public TCommonTypeIdent {
	public:

	public:
	static void AddContainerManipulator(TContainerManipulator*);




	public:
	void SetValueAt(TDispatchDataPtr& container_ptr, unsigned pos, const TDispatchDataPtr& value_copy_from) const;
	void RemoveElementAt(TDispatchDataPtr& container_ptr, unsigned pos) const;




	virtual void Clear(TDispatchDataPtr& container_ptr);

	public:
	virtual unsigned DoSize(const void* container_ptr) const=0;
	virtual bool IsMapLike() const=0;


	protected:
	virtual void* DoIndexAt(const void* container_ptr, unsigned pos) const=0;
	virtual void* DoValueAt(const void* container_ptr, unsigned pos) const=0;
	virtual void DoSetValueAt(void* container_ptr, unsigned pos, const void* value_copy_from) const=0;
	virtual void DoRemoveElementAt(void* container_ptr, unsigned pos) const=0;
	virtual void DoAddElement(void* container_ptr, const void* index_data, const void* value_data) const=0;
	virtual void DoClear(void* container_ptr)=0;


};
*/

#endif 
