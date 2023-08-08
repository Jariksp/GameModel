#ifndef BASE_TBASENUMBERPOINTCLASS_H_
#define BASE_TBASENUMBERPOINTCLASS_H_

#include <CRX/Base/TBasePointClass.h>

namespace crx {

template<class TT, typename CT>
class TBaseNumberPointClass : public TBasePointClass<TT, CT> {
	public:
	inline TBaseNumberPointClass()=default;

	public:
	void operator*(TT d) {//operator*=
		this->x *= d;
		this->y *= d;
		this->z *= d;
 	}

	void operator*=(TT p) {
		this->x *= p;
		this->y *= p;
		this->z *= p;
	}

	void operator*=(const CT& p) {//operator+=
		this->x *= p.x;
		this->y *= p.y;
		this->z *= p.z;
	}

	void operator+=(const CT& p) {//operator+=
		this->x += p.x;
		this->y += p.y;
		this->z += p.z;
	}

	void operator-=(const CT& p) {//operator+=
		this->x -= p.x;
		this->y -= p.y;
		this->z -= p.z;
	}
	
	CT operator-(const CT& p) const {//operator-
		CT result = *((const CT*)this);
		result.x -= p.x;
		result.y -= p.y;
		result.z -= p.z;
		return result;
	}
	
	CT operator+(const CT& p) const {//operator+
		CT result = *((const CT*)this);
		result.x += p.x;
		result.y += p.y;
		result.z += p.z;
		return result;
	}

	TT Length2() const { return (this->x*this->x) + (this->y*this->y) + (this->z*this->z); }
	TT Distance2(const TBaseNumberPointClass<TT, CT>& pp) const {
		auto a = pp.x - this->x;
		auto b = pp.y - this->y;
		auto c = pp.z - this->z;
		return (a*a) + (b*b) + (c*c);
	}

};


}




#endif /* BASE_TBASENUMBERPOINTCLASS_H_ */
