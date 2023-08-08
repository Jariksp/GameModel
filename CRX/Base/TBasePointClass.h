#ifndef BASE_TBASEPOINTCLASS_H_
#define BASE_TBASEPOINTCLASS_H_


#include <CRX/Base/TStringUtils.h>

namespace crx {

namespace NNPointCoord {
	enum NPointCoord {
		X=0,
		Y=1,
		Z=2,
		Size=3
	};
};
typedef NNPointCoord::NPointCoord NPointCoord;



template<class TT, typename CT>
class TBasePointClass {
	public:
	inline TBasePointClass()=default;


	public:
	void Set(TT inx, TT iny, TT inz) {
		x = inx;
		y = iny;
		z = inz;
	}

	public:
	TT x = 0;
	TT y = 0;
	TT z = 0;

	public:
	bool operator==(const CT& p) const {//operator==
		return p.x==x && p.y==y && p.z==z;
	}

	bool operator!=(const CT& p) const {//operator==
		return !(*((const CT*)this) == p);
		//return !(*((const CT*)this)) == p);
	}

	TT& operator[](int idx) { //operator[]
		TT* ptr = (TT*)this;
		return ptr[idx];
	}

	const TT& operator[](int idx) const { //operator[]
		const TT* ptr = (TT*)this;
		return ptr[idx];
	}


	public:
	std::string ToString() const {
		return std::to_string(x)+std::string(";")+std::to_string(y)+std::string(";")+std::to_string(z);
	}

	void FromString(const std::string& str) {
		auto vv = crx::StrTok(str, ";");
		for(unsigned i=0;i<vv.size();i++) {
			TT var;
			std::istringstream iss;
			iss.str(vv[i]);
			iss >> var;
			this->operator[](NPointCoord(i)) = var;
			//OperatorIndexGet(NPointCoord(i)) = var;
		}
	}

	public:
	template<class TR>
	TR CastTo() const {
		TR r;
		r.x = x;
		r.y = y;
		r.z = z;
		return r;
	}

	// это тупо, чтобы писать в for быстрее
	unsigned Size() const { return NNPointCoord::Size; }

};


};



#endif /* BASE_TBASEPOINTCLASS_H_ */
