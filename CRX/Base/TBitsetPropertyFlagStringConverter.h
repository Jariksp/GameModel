#ifndef BASE_TBITSETPROPERTYFLAGSTRINGCONVERTER_H_
#define BASE_TBITSETPROPERTYFLAGSTRINGCONVERTER_H_

#include <CRX/Base/TBitsetPropertyFlag.h>
#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Reflection/TReflectionTypeFactoryTemplate.h>

namespace crx {

template<unsigned _Nb, int fakeUnqeID>
std::string BitsetToString(const TBitsetPropertyFlag<_Nb,fakeUnqeID>& x) {
	auto vv = x.AllSetted();
	std::string s;
	for(auto a:vv) s += std::to_string(a) + std::string(";");
	return s;
}

template<unsigned _Nb, int fakeUnqeID>
TBitsetPropertyFlag<_Nb,fakeUnqeID> StringToBitset(const std::string& str) {
	TBitsetPropertyFlag<_Nb,fakeUnqeID> x;
	auto vv = crx::StrTok(str, ";");
	for(auto& a:vv) x.AddSet( crx::to_int(a) );
	return x;
}

template<unsigned _Nb, int fakeUnqeID>
class TBitsetPropertyFlagStringConverter : public crx::TReflectionStringConverter	{
	public:
	typedef TBitsetPropertyFlag<_Nb,fakeUnqeID> TBitsetType;
	TBitsetPropertyFlagStringConverter() { SetNativeTypeInfo( &typeid( TBitsetType ) ); }

	public:
	virtual void Set(std::any& out, const std::string& value) override {
		out = StringToBitset<_Nb, fakeUnqeID>(value);
	}

	virtual void Get(std::string& out, const std::any& value) override {
		const auto& vv = std::any_cast<const TBitsetType&>(value);
		out = BitsetToString<_Nb, fakeUnqeID>(vv);
	}

};

template<unsigned _Nb, int fakeUnqeID>
class TBitsetPropertyFlagSharedPtrStringConverter : public crx::TReflectionStringConverter	{
	public:
	typedef TBitsetPropertyFlag<_Nb,fakeUnqeID> TBitsetType;
	TBitsetPropertyFlagSharedPtrStringConverter() { SetNativeTypeInfo( &typeid( std::shared_ptr<TBitsetType> ) ); }

	public:
	virtual void Set(std::any& out, const std::string& value) override {
		auto a = std::make_shared<TBitsetType>();
		*a = StringToBitset<_Nb, fakeUnqeID>(value);;
		out = a;
	}

	virtual void Get(std::string& out, const std::any& value) override {
		auto vv =std:: any_cast<std::shared_ptr<TBitsetType>>(value);
		out = BitsetToString<_Nb, fakeUnqeID>(*vv);
	}

};


template<unsigned _Nb, int fakeUnqeID>
void AddBitsetPropertyFlagStringConverter(const TBitsetPropertyFlag<_Nb,fakeUnqeID>& tt) {
	typedef TBitsetPropertyFlag<_Nb,fakeUnqeID> TBitsetType;
	crx::impl::AddReflectionTypeFactoryTemplateIfNo<TBitsetType>();
	auto str_conv = new TBitsetPropertyFlagStringConverter<_Nb, fakeUnqeID>();
	crx::TReflectionStringConverter::Add(str_conv);
}

template<unsigned _Nb, int fakeUnqeID>
void AddBitsetPropertyFlagSharedPtrStringConverter(const std::shared_ptr<TBitsetPropertyFlag<_Nb,fakeUnqeID>>& tt) {
	typedef TBitsetPropertyFlag<_Nb,fakeUnqeID> TBitsetType;
	crx::impl::AddReflectionTypeFactoryTemplateIfNo<std::shared_ptr<TBitsetType>>();
	auto str_conv = new TBitsetPropertyFlagSharedPtrStringConverter<_Nb, fakeUnqeID>();
	crx::TReflectionStringConverter::Add(str_conv);
}


};



#endif /* PROTOTYPE_TBITSETPROPERTYFLAGSTRINGCONVERTER_H_ */
