#ifndef CRX_TFixPoint_H_
#define CRX_TFixPoint_H_

#include <stdint.h>
#include <sstream>
#include <math.h>

namespace crx {

// суть проблемы - не нужны числа с плавающей запятой. но, ввиду конечной точности, в них может быть немного не то число, которые ты ожидаешь
// например 1+1=1.99999999, т.к. представление данных внутри двоичное. Поэтому есть вот такой вот класс с fixed-point арифметикой, который
// и решает подобные проблемы. Из минусов - остался всего 21 бит данных, так что больше 2млн число в него не положишь
// возможно, я разберусь, как правильно умножать/делить (из-за переполнения) 128 битные числа, тогда можно будет его расширить до 64-1-10 битов

class TFixPoint {
	public:
	TFixPoint()=default;

	static void InitReflection();

	public:
	// именно 1К, а не 1024, иначе там немного числа поплывут из-за окргуления и игрок может увидеть не то, что ожидал
	const int PointBase = 1000;
	const double dPointBase = 1000.0;

	static const TFixPoint MAX;
	static const TFixPoint MIN;

	public:
	inline TFixPoint(const TFixPoint& d) { m_iData = d.m_iData; }
	inline TFixPoint(int d) { m_iData = int64_t(d)*PointBase; }
	inline TFixPoint(int64_t v) { m_iData = v; }
	inline TFixPoint(double d) { m_iData = int64_t(d*dPointBase); }

	public:
	inline void operator=(const TFixPoint& d) { m_iData = d.m_iData; }
	inline void operator=(int d) { m_iData = int64_t(d)*PointBase; }
	void operator=(double d);

	public:
	inline double AsDouble() const { return double(m_iData)/dPointBase; }
	inline int AsInt() const { return int(m_iData/PointBase); }

	inline int FastInt() const { return int(m_iData>>10); }

	// ------------------------------------------- //
	public:
	inline void operator+=(const TFixPoint& s) { m_iData += s.m_iData; }
	inline void operator-=(const TFixPoint& s) { m_iData -= s.m_iData; }
	inline void operator*=(const TFixPoint& s) { m_iData *= s.m_iData; m_iData /= PointBase; }
	inline void operator/=(const TFixPoint& s) { m_iData *= PointBase; m_iData /= s.m_iData; }

	// ------------------------------------------- //
	public:
	inline void operator+=(int s) { m_iData += int64_t(s)*PointBase; }
	inline void operator-=(int s) { m_iData -= int64_t(s)*PointBase; }
	inline void operator*=(int s) { m_iData *= s; }
	inline void operator/=(int s) { m_iData /= s; }

	// ------------------------------------------- //
	public:
	void operator+=(double s);
	void operator-=(double s);
	void operator*=(double s);
	void operator/=(double s);

	// ------------------------------------------- //
	public:
	inline bool operator==(const TFixPoint& f) const { return m_iData==f.m_iData; }
	inline bool operator!=(const TFixPoint& f) const { return m_iData!=f.m_iData; }
	inline bool operator<(const TFixPoint& f) const { return m_iData<f.m_iData; }
	inline bool operator>(const TFixPoint& f) const { return m_iData>f.m_iData; }
	inline bool operator<=(const TFixPoint& f) const { return m_iData<=f.m_iData; }
	inline bool operator>=(const TFixPoint& f) const { return m_iData>=f.m_iData; }

	inline bool operator==(int f) const { return m_iData==int64_t(f)*PointBase; }
	inline bool operator!=(int f) const { return m_iData!=int64_t(f)*PointBase; }
	inline bool operator<(int f) const { return m_iData<int64_t(f)*PointBase; }
	inline bool operator>(int f) const { return m_iData>int64_t(f)*PointBase; }
	inline bool operator<=(int f) const { return m_iData<=int64_t(f)*PointBase; }
	inline bool operator>=(int f) const { return m_iData>=int64_t(f)*PointBase; }


	bool operator==(double f) const;
	bool operator!=(double f) const;
	bool operator<(double f) const;
	bool operator>(double f) const;
	bool operator<=(double f) const;
	bool operator>=(double f) const;


	public:
	inline TFixPoint operator+() const { return *this; }
	inline TFixPoint operator-() const { auto a = *this; a.m_iData = -a.m_iData; return a; }

	// ------------------------------------------- //
	public:
	inline TFixPoint operator*( const TFixPoint& b) const {
		TFixPoint c;
		c.m_iData = (m_iData * b.m_iData)/PointBase;
		return c;
	}

	inline TFixPoint operator/( const TFixPoint& b) const {
		TFixPoint c;
		c.m_iData = (m_iData*PointBase / b.m_iData);
		return c;
	}

	inline TFixPoint operator+( const TFixPoint& b) const {
		TFixPoint c;
		c.m_iData = (m_iData + b.m_iData);
		return c;
	}

	inline TFixPoint operator-( const TFixPoint& b) const {
		TFixPoint c;
		c.m_iData = (m_iData - b.m_iData);
		return c;
	}

	// ------------------------------------------- //
	public:
	inline TFixPoint operator*(int b) const {
		TFixPoint c;
		c.m_iData = m_iData * b;
		return c;
	}

	inline TFixPoint operator/(int b) const {
		TFixPoint c;
		c.m_iData = m_iData / b;
		return c;
	}

	inline TFixPoint operator+(int b) const {
		TFixPoint c;
		c.m_iData = (m_iData + b*PointBase);
		return c;
	}

	inline TFixPoint operator-(int b) const {
		TFixPoint c;
		c.m_iData = (m_iData - b*PointBase);
		return c;
	}


	// ------------------------------------------- //
	public:
	std::string ToString() const;
	static TFixPoint FromString(const std::string& s);
	std::string ToString(unsigned decimalDigits, bool fixDigits, bool forceusePlus) const;

	public:
	int64_t Data() const;

	public:
	static TFixPoint min(const TFixPoint& a, const TFixPoint& b);


	protected:
	int64_t m_iData = 0;
};

TFixPoint abs(const TFixPoint& f);
//TFixPoint sqr(const TFixPoint& f);
//inline TFixPoint sqrt(const TFixPoint& f) { return TFixPoint( ::sqrt(f.AsDouble()) ); }
TFixPoint sign(const TFixPoint& f);
TFixPoint round(const TFixPoint& f);
TFixPoint round(const TFixPoint& f, unsigned base);
std::ostream& operator<<(std::ostream& st, const TFixPoint& rr);

int iround(const TFixPoint& f);

TFixPoint RoundTo(const TFixPoint& f, int mustDiv);// округляет вверх, чтобы полученное число делилось нацело на mustDiv

} /* namespace crx */

#endif /* BASE_TFixPoint_H_ */
