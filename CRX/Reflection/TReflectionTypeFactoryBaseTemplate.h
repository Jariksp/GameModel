#ifndef CRX_REFLECTION_TREFLECTIONTYPEFACTORYBASETEMPLATE_H_
#define CRX_REFLECTION_TREFLECTIONTYPEFACTORYBASETEMPLATE_H_

#include <CRX/Reflection/TReflectionTypeFactory.h>

namespace crx {

namespace impl {

template<class T, class EqualTo>
struct has_operator_equal_impl
{
    template<class U, class V>
    static auto test(U*) -> decltype(std::declval<U>() == std::declval<V>());
    template<typename, typename>
    static auto test(...) -> std::false_type;

    using type = typename std::is_same<bool, decltype(test<T, EqualTo>(0))>::type;
};

template<class T, class EqualTo = T>
struct has_operator_equal : has_operator_equal_impl<T, EqualTo>::type {};

template<class T>
bool CheckIsEqualIfHaveOpertorEqual(const T& a, const T& b, typename std::enable_if<has_operator_equal<T>::value>::type* = 0) {
    return a==b;
}

template<class T>
bool CheckIsEqualIfHaveOpertorEqual(const T& a, const T& b, typename std::enable_if<!has_operator_equal<T>::value>::type* = 0) {
    return false;
}


template<class CT>
class TReflectionTypeFactoryBaseTemplate : public TReflectionTypeFactory {
	public:
	TReflectionTypeFactoryBaseTemplate() { SetNativeTypeInfo( &typeid(CT) ); }
	virtual ~TReflectionTypeFactoryBaseTemplate()=default;

	public:
	virtual bool DoIsEqual(const std::any& a, const std::any& b) override {
		return CheckIsEqualIfHaveOpertorEqual<CT>( std::any_cast<const CT&>(a), std::any_cast<const CT&>(b) );
	}


};


}//impl



}//crx

#endif /* REFLECTION_TREFLECTIONTYPEFACTORYBASETEMPLATE_H_ */
