#include <CRX/Reflection/TReflectionContainerManipulator.h>
#include <CRX/Reflection/TReflectionGlobals.h>

namespace crx {

void TReflectionContainerManipulator::Add(TReflectionContainerManipulator *man) {
	TReflectionGlobals::GetInstance()->Add(man);
}

const std::string& TReflectionContainerManipulator::IndexTypeName() const { return m_sIndexTypeName; }

const std::string& TReflectionContainerManipulator::ValueTypeName() const { return m_sValueTypeName; }

bool TReflectionContainerManipulator::IsHave(const std::string &typeName) {
	return TReflectionGlobals::GetInstance()->FindContainerManipulator(typeName)!=nullptr;
}

TReflectionTypeFactory* TReflectionContainerManipulator::IndexTypeFactory() {
	if(m_sIndexTypeName.empty()) return nullptr;
	return TReflectionGlobals::GetInstance()->FindTypeFactory(m_sIndexTypeName);
}

TReflectionTypeFactory* TReflectionContainerManipulator::ValueTypeFactory() {
	if(m_sValueTypeName.empty()) return nullptr;
	return TReflectionGlobals::GetInstance()->FindTypeFactory(m_sValueTypeName);
}

}//crx



/*



TDispatchDataPtr TContainerManipulator::IndexAt(const TDispatchDataPtr& container_ptr, unsigned pos) const {
	void* pIndex = DoIndexAt(container_ptr->DataPtr(),pos);
	TTypeFactory* pFactory = TGlobalDispatch::GetInstance()->FindTypeFactory(IndexTypeName());
	return TDispatchDataPtr( new TDispatchData(pIndex,pFactory) );
}

TDispatchDataPtr TContainerManipulator::ValueAt(const TDispatchDataPtr& container_ptr, unsigned pos) const {
	void* pValue = DoValueAt(container_ptr->DataPtr(),pos);
	TTypeFactory* pFactory = TGlobalDispatch::GetInstance()->FindTypeFactory(ValueTypeName());
	return TDispatchDataPtr( new TDispatchData(pValue,pFactory) );
}

void TContainerManipulator::SetValueAt(TDispatchDataPtr& container_ptr, unsigned pos, const TDispatchDataPtr& value_copy_from) const {
	DoSetValueAt( container_ptr->DataPtr(), pos, value_copy_from->DataPtr() );
}

void TContainerManipulator::RemoveElementAt(TDispatchDataPtr& container_ptr, unsigned pos) const {
	DoRemoveElementAt(container_ptr->DataPtr(),pos);
}

void TContainerManipulator::AddElement(TDispatchDataPtr& container_ptr, const TDispatchDataPtr& index_data, const TDispatchDataPtr& value_data) const {
	DoAddElement( container_ptr->DataPtr(), index_data->DataPtr(), value_data->DataPtr() );
}

unsigned TContainerManipulator::Size(const TDispatchDataPtr& container_ptr) const {
	return DoSize(container_ptr->DataPtr());
}

void TContainerManipulator::Clear(TDispatchDataPtr& container_ptr) { DoClear(container_ptr->DataPtr()); }
*/
