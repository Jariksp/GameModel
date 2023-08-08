#ifndef CRX_REFLECTION_TVECTORMANIPULATOR_H
#define CRX_REFLECTION_TVECTORMANIPULATOR_H

#include <CRX/Reflection/TReflectionContainerManipulator.h>
#include <CRX/Base/TMiscUtils.h>
#include <vector>


namespace crx {

namespace impl {

template<class T>
class TReflectionVectorContainerManipulator : public TReflectionContainerManipulator {
	public:
	TReflectionVectorContainerManipulator() {
		SetNativeTypeInfo( &typeid(std::vector<T>) );
		m_sValueTypeName = GetTypeName<T>();
	}
	virtual ~TReflectionVectorContainerManipulator()=default;

	public:
	virtual unsigned Size(const std::any& container_ptr) override {
		return (unsigned)std::any_cast< const std::vector<T>& >(container_ptr).size();
	}

	virtual void IndexAt(const std::any& container_ptr, unsigned pos, std::any& out) override {
		out.reset();
	}

	virtual void ValueAt(const std::any& container_ptr, unsigned pos, std::any& out) override {
		out = std::any_cast< const std::vector<T>& >(container_ptr)[pos];
	}

	virtual void AddElement(std::any& container_ptr, const std::any& index_data, const std::any& value_data) override {
		std::any_cast< std::vector<T>& >(container_ptr).push_back( std::any_cast<const T&>(value_data) );
	}

	virtual void Clear(std::any& container_ptr) override {
		std::any_cast< std::vector<T>& >(container_ptr).clear();
	}

};

template<class CT>
void AddReflectionVectorContainerManipulatorIfNo() {
	if( TReflectionContainerManipulator::IsHave( GetTypeName<std::vector<CT>>() ) ) return;
	TReflectionContainerManipulator::Add( new TReflectionVectorContainerManipulator<CT>() );
}

}//impl

}//crx

#endif // TVECTORMANIPULATOR_H
