#ifndef CRX_REFLECTION_TREFLECTIONOBJECTCASTSTEMPLATE_H_
#define CRX_REFLECTION_TREFLECTIONOBJECTCASTSTEMPLATE_H_

#include <CRX/Reflection/TReflectionObjectCasts.h>
#include <CRX/Reflection/TReflectionObject.h>

namespace crx {

namespace impl {

template<class CT>
class TReflectionObjectCastsPointerTemplate : public TReflectionObjectCasts {
	public:
	TReflectionObjectCastsPointerTemplate() {
		SetNativeTypeInfo( &typeid(CT) );
		m_xType = NType::Pointer;
	}
	virtual ~TReflectionObjectCastsPointerTemplate()=default;

	public:
	virtual TReflectionObject* Cast(std::any& data) override {
		return std::any_cast<CT>(data);
	}

	virtual void Cast(TReflectionObject* from, std::any& to) override {
		to = dynamic_cast<CT>(from);
	}

};

template<class CT>
class TReflectionObjectCastsTemplate : public TReflectionObjectCasts {
	public:
	TReflectionObjectCastsTemplate() {
		SetNativeTypeInfo( &typeid(CT) );
		m_xType = NType::Object;
	}
	virtual ~TReflectionObjectCastsTemplate()=default;

	public:
	virtual TReflectionObject* Cast(std::any& data) override {
		return std::any_cast<CT>(&data);
	}

	// суть проблемы - может быть pure virtual методы и тогда нельзя создать эксземпляр
	// нужно, конечно, через enable_if запилить, но как-то позже
	virtual void Cast(TReflectionObject* from, std::any& to) override {
		//to = make_any<CT>( *(dynamic_cast<CT*>(from)) );
	}

};


template<class CT>
class TReflectionObjectSharedPtrCastsTemplate : public TReflectionObjectCasts {
	public:
	typedef typename CT::element_type mtype;
	TReflectionObjectSharedPtrCastsTemplate() {
		SetNativeTypeInfo( &typeid(CT) );
		m_sWrapper = "std::shared_ptr";
		m_xType = NType::SmartPtr;
	}
	virtual ~TReflectionObjectSharedPtrCastsTemplate()=default;

	public:
	virtual TReflectionObject* Cast(std::any& data) override {
		return std::any_cast<CT&>(data).get();
	}

	virtual void Cast(TReflectionObject* from, std::any& to) override {
		if(!from) {
			to = std::make_any<CT>();
		} else {

			auto src = from->shared_from_this();
			to = std::dynamic_pointer_cast<mtype>(src);
		}
	}

};

template<class CT>
class TReflectionObjectWeakPtrCastsTemplate : public TReflectionObjectCasts {
	public:
	typedef typename CT::element_type mtype;
	TReflectionObjectWeakPtrCastsTemplate() {
		SetNativeTypeInfo( &typeid(CT) );
		m_sWrapper = "std::weak_ptr";
		m_xType = NType::SmartPtr;
	}
	virtual ~TReflectionObjectWeakPtrCastsTemplate()=default;

	public:
	virtual TReflectionObject* Cast(std::any& data) override {
		return std::any_cast<CT&>(data).lock().get();
	}

	virtual void Cast(TReflectionObject* from, std::any& to) override {
		if(!from) {
			to = std::make_any<CT>();
		} else {
			auto src = from->shared_from_this();
			auto shp = std::dynamic_pointer_cast<mtype>(src);
			to = std::make_any<std::weak_ptr<mtype>>(shp);
		}
	}

	virtual bool IsWeakPtr() override { return true; }
};

}//impl


}//crx




#endif /* REFLECTION_TREFLECTIONOBJECTCASTSTEMPLATE_H_ */
