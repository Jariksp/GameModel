#ifndef CRX_TENUMTEMPLATE_H_
#define CRX_TENUMTEMPLATE_H_

#include <sstream>


namespace crx {

// от enum нельзя делать потомков. поэтому, чтбы расшрить энум в потомках, приходится вот так вот извращаться
// см. NSPotting как реализован



template<int fakeUnqeID>
class TEnumTempate {
	public:
	constexpr TEnumTempate()=default;
	constexpr TEnumTempate(int v) : Value(v) {}

	public:
	int Value = 0;

	public:
	typedef TEnumTempate<fakeUnqeID> TEnumTemplateType;

	bool operator !=(const TEnumTemplateType &a) const { return this->Value!=a.Value; }
	bool operator ==(const TEnumTemplateType &a) const { return this->Value==a.Value; }
	bool operator >(const TEnumTemplateType &a) const { return this->Value>a.Value; }
	bool operator <(const TEnumTemplateType &a) const { return this->Value<a.Value; }
	operator int() const { return Value; }


	TEnumTemplateType operator&(const TEnumTemplateType& a) {
		TEnumTemplateType t;
		t.Value = Value & a.Value;
		return t;
	}


};

template<int val>
std::istream& operator>>(std::istream& is, TEnumTempate<val>& dt) {
	is>>dt.Value;
	return is;
}

}//crx

template<int val>
std::ostream& operator<<(std::ostream& st, const crx::TEnumTempate<val>& rr) {
	st<<rr.Value;
	return st;
}


#endif /* MISC_TENUMTEMPLATE_H_ */
