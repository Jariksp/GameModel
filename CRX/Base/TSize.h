#ifndef CRX_TSIZEH__H_H_
#define CRX_TSIZEH__H_H_

#include <sstream>


namespace crx {

class TSize {
	public:
	TSize() = default;
	TSize(int w1, int h1);

	public:
	void Set(int w1, int h1);
	void SetMax(const TSize& s);
	bool operator==(const TSize& p) const;
	void operator+=(const TSize& p);
	inline bool operator!=(const TSize& p) const { return !operator==(p); }

	public:
	int w = 0;
	int h = 0;

	inline int operator[](unsigned idx) const { return ((int*)this)[idx]; }
};

}//crx


std::ostream& operator<<(std::ostream& st, const crx::TSize& rr);







#endif
