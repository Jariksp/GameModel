#ifndef TBITSETPROPERTYFLAG_H_
#define TBITSETPROPERTYFLAG_H_

#include <CRX/Base/IPropertyFlag.h>
#include <bitset>
#include <stdint.h>
#include <sstream>
#include <vector>


namespace crx {

// этот класс конвертирует число в номер бита. например, у вас есть 10 каких-то propertys, которые могут в произвольном порядке
// быть у объекта. можно делать find по их вектору, а можно сделть вот такой вот cash и быстро чекать, какие есть
// а ещё у одного скила может быть несколько разных property
//
// _Nb кол-во 64 битных uint.
// fakeUnqeID - введен только для того, чтобы NNBitset одинакового размереа считались разными типами и нельзя было их смешать


template<unsigned _Nb, int fakeUnqeID>
class TBitsetPropertyFlag : public IPropertyFlag {
	public:
	inline TBitsetPropertyFlag() { _Clear(); }
	inline TBitsetPropertyFlag(unsigned a) { Set(a); }
	virtual ~TBitsetPropertyFlag()=default;

	public:
	inline void Set(unsigned a) {
		_Clear();
		_Set(a);
	}

	inline void AddSet(unsigned a) { _Set(a); }

	inline TBitsetPropertyFlag& operator=(unsigned a) { Set(a); return *this; }

	inline TBitsetPropertyFlag& operator=(const TBitsetPropertyFlag& aa) {
		for(unsigned i=0;i<_Nb;i++) m_xData[i] = aa.m_xData[i];
		return *this;
	}

	public:
	operator unsigned() const {
		for(unsigned i=0;i<_Nb;i++) {
			uint64_t aa = ( m_xData[i] & ~(m_xData[i]-1) );
			if(aa) {
				auto ll = log2_64(aa);
				return ll + (i*64) + 1;
			}
		}
		return 0;
	}

	operator int() const { return unsigned(*this); }
/*
	inline operator bool() const {
		for(unsigned i=0;i<_Nb;i++) if(m_xData[i]) return true;
		return false;
	}
*/
	public:
	inline bool operator==(int a) const { return int(*this)==a; }


	public:
	inline TBitsetPropertyFlag& operator&=(const TBitsetPropertyFlag& aa) {
		for(unsigned i=0;i<_Nb;i++) m_xData[i] &= aa.m_xData[i];
		return *this;
	}

	inline bool BitwiseAnd(const TBitsetPropertyFlag& aa) const {
		for(unsigned i=0;i<_Nb;i++) if( (m_xData[i]&aa.m_xData[i]) ) return true;
		return false;
	}


	inline TBitsetPropertyFlag& operator|=(const TBitsetPropertyFlag& aa) {
		for(unsigned i=0;i<_Nb;i++) m_xData[i] |= aa.m_xData[i];
		return *this;
	}


	inline bool operator==(const TBitsetPropertyFlag& aa) const {
		for(unsigned i=0;i<_Nb;i++) if(m_xData[i]!=aa.m_xData[i]) return false;
		return true;
	}

	inline bool operator!=(const TBitsetPropertyFlag& a) const { return !operator==(a); }

	inline bool operator<(const TBitsetPropertyFlag& a) const { return int(*this)<int(a); }

	TBitsetPropertyFlag operator~() const {
		TBitsetPropertyFlag aa;
		for(unsigned i=0;i<_Nb;i++) aa.m_xData[i] = ~m_xData[i];
		return aa;
	}



	inline TBitsetPropertyFlag& operator^=(const TBitsetPropertyFlag& aa) {
		for(unsigned i=0;i<_Nb;i++) m_xData[i] ^= aa.m_xData[i];
		return *this;
	}

	std::vector<unsigned> AllSetted() const {
		std::vector<unsigned> vv;
		for(unsigned i=0;i<_Nb;i++) {
			uint64_t a = 1;
			for(unsigned k=0;k<64;k++) {
				if( (m_xData[i]&a) ) vv.push_back( i*64 + k + 1);
				a = a<<1;
			}
		}//for
		return vv;
	}


	public:
	uint64_t* Data() { return m_xData; }

	public:
	virtual void Clear() override { _Clear(); }

	virtual void AddFlags(const std::shared_ptr<IPropertyFlag>& f) override {
		auto aa = std::dynamic_pointer_cast<TBitsetPropertyFlag>(f);
		if(aa) this->operator |=(*aa);
	}

	virtual bool BitwiseAnd(const std::shared_ptr<IPropertyFlag>& f) const override {
		auto aa = std::dynamic_pointer_cast<TBitsetPropertyFlag>(f);
		return BitwiseAnd( *aa );
	}

	virtual bool BitwiseAnd(const IPropertyFlag* f) const override {
		auto aa = dynamic_cast<const TBitsetPropertyFlag*>(f);
		return BitwiseAnd(*aa);
	}

	virtual std::shared_ptr<IPropertyFlag> Clone() override {
		auto cc = std::make_shared<TBitsetPropertyFlag>();
		for(unsigned i=0;i<_Nb;i++) cc->m_xData[i] = m_xData[i];
		return cc;
	}



	protected:
	inline void _Set(unsigned a) {
		if(a==0) return;
		a--;
		m_xData[a/64] |= uint64_t(1)<<(a%64);
	}

	inline void _Clear() {
		for(unsigned i=0;i<_Nb;i++) m_xData[i] = 0;
	}

	static int log2_64 (uint64_t value) {
		static const int tab64[64] = {
		    63,  0, 58,  1, 59, 47, 53,  2,
		    60, 39, 48, 27, 54, 33, 42,  3,
		    61, 51, 37, 40, 49, 18, 28, 20,
		    55, 30, 34, 11, 43, 14, 22,  4,
		    62, 57, 46, 52, 38, 26, 32, 41,
		    50, 36, 17, 19, 29, 10, 13, 21,
		    56, 45, 25, 31, 35, 16,  9, 12,
		    44, 24, 15,  8, 23,  7,  6,  5
		};

	    value |= value >> 1;
	    value |= value >> 2;
	    value |= value >> 4;
	    value |= value >> 8;
	    value |= value >> 16;
	    value |= value >> 32;
	    return tab64[((uint64_t)((value - (value >> 1))*0x07EDD5E59A4E28C2)) >> 58];
	}



	protected:
	uint64_t m_xData[_Nb];


};

/*
template<unsigned _Nb, int fakeUnqeID>
inline TBitsetPropertyFlag<_Nb, fakeUnqeID> operator&(const TBitsetPropertyFlag<_Nb,fakeUnqeID>& __x, const TBitsetPropertyFlag<_Nb,fakeUnqeID>& __y) {
	TBitsetPropertyFlag<_Nb,fakeUnqeID> __result(__x);
	__result &= __y;
	return __result;
}
*/

template<unsigned _Nb, int fakeUnqeID>
inline bool operator&(const TBitsetPropertyFlag<_Nb,fakeUnqeID>& __x, const TBitsetPropertyFlag<_Nb,fakeUnqeID>& __y) {
	return __x.BitwiseAnd(__y);
}


template<unsigned _Nb, int fakeUnqeID>
inline TBitsetPropertyFlag<_Nb,fakeUnqeID> operator|(const TBitsetPropertyFlag<_Nb,fakeUnqeID>& __x, const TBitsetPropertyFlag<_Nb,fakeUnqeID>& __y) {
	TBitsetPropertyFlag<_Nb,fakeUnqeID> __result(__x);
	__result |= __y;
	return __result;
}

template<unsigned _Nb, int fakeUnqeID>
std::ostream& operator<<(std::ostream& st, const TBitsetPropertyFlag<_Nb,fakeUnqeID>& rr) {
	std::string ss;
	auto vv = rr.AllSetted();
	for(auto a:vv) ss += std::to_string(a) + std::string(";");
	st<<ss;
	return st;
}

}

#endif
