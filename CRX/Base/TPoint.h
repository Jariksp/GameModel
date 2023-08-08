#ifndef CRX_TPOINT_H_H_
#define CRX_TPOINT_H_H_

#include <sstream>


namespace crx {

class TDPoint;

class TPoint {
	public:
	TPoint() = default;
	TPoint(int x1, int y1);
	TPoint(const TDPoint& p);
	void operator=(const TDPoint& p);
	bool operator==(const TPoint& p) const;
	bool operator!=(const TPoint& p) const;
	bool operator<(const TPoint& p) const;
	TPoint operator+(const TPoint& p) const;
	TPoint operator-(const TPoint& p) const;
	void operator+=(const TPoint& p);

	void operator*=(const TPoint& p);

	public:
	int x = 0;
	int y = 0;

	inline const int& operator[](unsigned idx) const { return ((int*)this)[idx]; }
	inline int& operator[](unsigned idx) { return ((int*)this)[idx]; }

	public:
	void Set(int x1, int y1);
	void Set(const TPoint& p);
	void Set(const TDPoint& p);
	bool IsEqual(const TPoint& p) const;
	int Distance(const TPoint& p) const;

	public:
	std::string ToString() const;
	void FromString(const std::string& str);

};

}//crx

std::ostream& operator<<(std::ostream& st, const crx::TPoint& rr);

#endif
