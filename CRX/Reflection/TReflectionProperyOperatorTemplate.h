#ifndef CRX_REFLECTION_TREFLECTIONPROPERYOPERATORTEMPLATE_H_
#define CRX_REFLECTION_TREFLECTIONPROPERYOPERATORTEMPLATE_H_

#include <CRX/Reflection/IReflectionProperyOperator.h>

namespace crx {

namespace impl {

// для поля класса операция чтения и записи всегда определена, тут нам нет смысла разделать.
template<class CL, class CV>
class TProperyOperatorMemberGetSet : public virtual IReflectionProperyOperatorGet, public virtual IReflectionProperyOperatorSet {
	public:
	TProperyOperatorMemberGetSet(CV& data) {
		SetNativeTypeInfo( &typeid(CV) );
		m_uOffset = uintptr_t(&data);
	}

	virtual ~TProperyOperatorMemberGetSet()=default;

	public:
	virtual void Get(TReflectionObject* obj, std::any& copy_to) override {
		auto ptr = GetDataPtr(obj);
		copy_to = std::make_any<CV>(*ptr);
	}

	virtual void Set(TReflectionObject* obj, const std::any& src) override {
		auto ptr = GetDataPtr(obj);
		*ptr = std::any_cast<const CV&>(src);
	}


	public:
	inline CV* GetDataPtr(TReflectionObject* obj) {
		auto ptra = dynamic_cast<CL*>(obj);
		return (CV*)(uintptr_t(ptra)+m_uOffset);
	}

	uintptr_t m_uOffset = 0;
};


// -------------------- FUNCTION ----------------------- //

template<class CL, class CV>
class TProperyOperatorFunctionGet : public IReflectionProperyOperatorGet {
	public:
	typedef CV(CL::*TGet)(void);

	public:
	TProperyOperatorFunctionGet(TGet get) {
		SetNativeTypeInfo( &typeid(CV) );
		m_pGet = get;
	}

	virtual ~TProperyOperatorFunctionGet()=default;

	public:
	virtual void Get(TReflectionObject* obj, std::any& copy_to) override {
		CL* cl = dynamic_cast<CL*>(obj);
		copy_to = std::make_any<CV>( (*cl.*m_pGet)() );
	}



	TGet m_pGet = nullptr;
};


template<class CL, class CV>
class TProperyOperatorFunctionSet : public IReflectionProperyOperatorSet {
	public:
	typedef void(CL::*TSet)(CV);

	public:
	TProperyOperatorFunctionSet(TSet set) {
		SetNativeTypeInfo( &typeid(CV) );
		m_pSet = set;
	}

	virtual ~TProperyOperatorFunctionSet()=default;

	public:
	virtual void Set(TReflectionObject* obj, const std::any& src) override {
		CL* cl = dynamic_cast<CL*>(obj);
		(*cl.*m_pSet)( std::any_cast<const CV&>(src) );
	}

	TSet m_pSet = nullptr;
};


// ------------------- отложеное создание, проверка ---------------------- //
template<class CL, class CV>
class TProperyLazyInitializationChecker : public virtual IReflectionProperyLazyInitializationChecker {
	public:
	TProperyLazyInitializationChecker(CV& data) {
		m_uOffset = uintptr_t(&data);
	}

	virtual ~TProperyLazyInitializationChecker()=default;

	public:
	virtual bool IsNullptr(TReflectionObject* obj) override {
		auto ptra = dynamic_cast<CL*>(obj);
		return *((CV*)(uintptr_t(ptra)+m_uOffset))==nullptr;
	}

	uintptr_t m_uOffset = 0;
};




}//impl


}//crx




#endif /* REFLECTION_TREFLECTIONPROPERYOPERATORTEMPLATE_H_ */
