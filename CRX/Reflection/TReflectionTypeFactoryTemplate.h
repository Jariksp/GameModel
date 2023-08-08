#ifndef CRX_TAABASETYPEFACTORYTEMPLATE__H_H
#define CRX_TAABASETYPEFACTORYTEMPLATE__H_H

#include <CRX/Base/TDebug.h>
#include <CRX/Base/TMiscUtils.h>
#include <CRX/Reflection/TReflectionTypeFactoryBaseTemplate.h>
#include <type_traits>
#include <CRX/Reflection/MakeShared.h>

namespace crx {

namespace impl {

template<class CT>
class TReflectionTypeFactoryTemplate : public TReflectionTypeFactoryBaseTemplate<CT> {
	public:
	virtual std::any Create() override { return std::make_any<CT>(); }
};


template<class CT>
class TReflectionTypeFactoryPointerTemplate : public TReflectionTypeFactoryBaseTemplate<CT> {
	public:
	virtual std::any Create() override {
		typedef typename std::remove_pointer<CT>::type mtype;
		return std::make_any<CT>( new mtype() );

	}
};

template<class CT>
class TReflectionTypeFactorySharedPtrTemplate : public TReflectionTypeFactoryBaseTemplate<CT> {
	public:
	virtual std::any Create() override {
		typedef typename CT::element_type mtype;
		return std::make_any<CT>( MakeShared<mtype>() );
	}
};

template<class CT>
class TReflectionTypeFactoryWeakPtrTemplate : public TReflectionTypeFactoryBaseTemplate<CT> {
	public:
	virtual std::any Create() override {
		typedef typename CT::element_type mtype;
		return std::make_any<CT>( std::weak_ptr<mtype>() );
	}
};


template<class CT>
class TReflectionTypeFactoryAbstractTemplate : public TReflectionTypeFactoryBaseTemplate<CT> {
	public:
	virtual std::any Create() override {
		_ERROR("Can't create abstract object: "<<this->m_sTypeName);
		return std::any();
	}
};



template<typename T> struct is_shared_ptr : std::false_type {};
template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template<typename T> struct is_weak_ptr : std::false_type {};
template<typename T> struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};

// ---------------- NON SMART POINTER ------------------- //

template<class T, class = typename std::enable_if_t< (!std::is_pointer<T>::value && !is_shared_ptr<T>::value && !is_weak_ptr<T>::value && !std::is_abstract<T>::value)  >>
TReflectionTypeFactory* SelectTemplateFactoryByPointerNonPointerSharedPtrType() {
	return new TReflectionTypeFactoryTemplate<T>();
}


template<class T, class = typename std::enable_if_t<(std::is_pointer<T>::value && !std::is_abstract<typename std::remove_pointer<T>::type>::value)>, class = void>
TReflectionTypeFactory* SelectTemplateFactoryByPointerNonPointerSharedPtrType() {
	return new TReflectionTypeFactoryPointerTemplate<T>();
}

// ---------------- SHARED_PTR ------------------- //

template < typename T, typename std::enable_if< (std::is_abstract<T>::value), void** >::type = nullptr >
TReflectionTypeFactory* SelectTemplateFactoryBySharedPtrOrAbstractObject() {
	return new TReflectionTypeFactoryAbstractTemplate<std::shared_ptr<T>>();
}

template < typename T, typename std::enable_if< (!std::is_abstract<T>::value), void** >::type = nullptr >
TReflectionTypeFactory* SelectTemplateFactoryBySharedPtrOrAbstractObject() {
	return new TReflectionTypeFactorySharedPtrTemplate<std::shared_ptr<T>>();
}

// ---------------- WEAK_PTR ------------------- //

template < typename T, typename std::enable_if< (std::is_abstract<T>::value), void** >::type = nullptr >
TReflectionTypeFactory* SelectTemplateFactoryByWeakPtrOrAbstractObject() {
	return new TReflectionTypeFactoryWeakPtrTemplate<std::weak_ptr<T>>();
}

template < typename T, typename std::enable_if< (!std::is_abstract<T>::value), void** >::type = nullptr >
TReflectionTypeFactory* SelectTemplateFactoryByWeakPtrOrAbstractObject() {
	return new TReflectionTypeFactoryWeakPtrTemplate<std::weak_ptr<T>>();
}


// ------------------- SELECTOR BY TYPE ---------------- //

template < typename T, typename std::enable_if< (is_shared_ptr<T>::value), void** >::type = nullptr >
TReflectionTypeFactory* SelectTemplateFactoryByPointerNonPointerSharedPtrType() {
	typedef typename T::element_type mtype;
	return SelectTemplateFactoryBySharedPtrOrAbstractObject<mtype>();
}

template < typename T, typename std::enable_if< (is_weak_ptr<T>::value), void** >::type = nullptr >
TReflectionTypeFactory* SelectTemplateFactoryByPointerNonPointerSharedPtrType() {
	typedef typename T::element_type mtype;
	return SelectTemplateFactoryByWeakPtrOrAbstractObject<mtype>();
}

template < typename T, typename std::enable_if< (std::is_abstract<typename std::remove_pointer<T>::type>::value), void** >::type = nullptr >
TReflectionTypeFactory* SelectTemplateFactoryByPointerNonPointerSharedPtrType() {
	return new TReflectionTypeFactoryAbstractTemplate<T>();
}

// -------------- FACTORY ---------------------- //

template<class CT>
void AddReflectionTypeFactoryTemplateIfNo() {
	if( TReflectionTypeFactory::IsHave(GetTypeName<CT>()) ) return;
	TReflectionTypeFactory::Add( SelectTemplateFactoryByPointerNonPointerSharedPtrType<CT>() );
}

}//impl


}//crx


#endif
