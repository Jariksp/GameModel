#ifndef CRX_REFLECTION_TMAPMANIPULATOR_H
#define CRX_REFLECTION_TMAPMANIPULATOR_H

#include <CRX/Reflection/TReflectionContainerManipulator.h>
#include <CRX/Base/TMiscUtils.h>
#include <map>


namespace crx {

namespace impl {

template<class TIndex,class TElement>
class TReflectionMapContainerManipulator : public TReflectionContainerManipulator {
	typedef std::map<TIndex,TElement> TMap;
	public:
	TReflectionMapContainerManipulator() {
		SetNativeTypeInfo( &typeid(TMap) );
		m_sValueTypeName = GetTypeName<TElement>();
		m_sIndexTypeName = GetTypeName<TIndex>();
	}
	virtual ~TReflectionMapContainerManipulator()=default;

	public:
	virtual unsigned Size(const std::any& container_ptr) override {
		return (unsigned)std::any_cast< const TMap& >(container_ptr).size();
	}

	virtual void IndexAt(const std::any& container_ptr, unsigned pos, std::any& out) override {
		const auto& mm = std::any_cast<const TMap&>(container_ptr);
		auto it = mm.begin();
		while(pos) { it++; pos--; }
		out = it->first;
	}

	virtual void ValueAt(const std::any& container_ptr, unsigned pos, std::any& out) override {
		const auto& mm = std::any_cast<const TMap&>(container_ptr);
		auto it = mm.begin();
		while(pos) { it++; pos--; }
		out = it->second;
	}

	virtual void AddElement(std::any& container_ptr, const std::any& index_data, const std::any& value_data) override {
		auto& mm = std::any_cast<TMap&>(container_ptr);
		mm[ std::any_cast<const TIndex&>(index_data) ] = std::any_cast<const TElement&>(value_data);
	}

	virtual void Clear(std::any& container_ptr) override {
		std::any_cast<TMap&>(container_ptr).clear();
	}
};

template<class CT, class VT>
void AddReflectionMapContainerManipulatorIfNo() {
	if( TReflectionContainerManipulator::IsHave( GetTypeName<std::map<CT, VT>>() ) ) return;
	TReflectionContainerManipulator::Add( new TReflectionMapContainerManipulator<CT, VT>() );
}

}//impl

}//crx


#endif // TVECTORMANIPULATOR_H
