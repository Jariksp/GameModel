#ifndef CRX_BASE__T3DIPOINT_H_
#define CRX_BASE__T3DIPOINT_H_

#include <sstream>
#include <CRX/Base/TBaseNumberPointClass.h>


namespace crx {

class T3DIPoint : public TBaseNumberPointClass<int, T3DIPoint> {
	public:
	inline T3DIPoint()=default;
	inline T3DIPoint(int x1, int y1, int z1=0) {
		x = x1; y = y1; z = z1;
	}

};

std::ostream& operator<<(std::ostream& st, const crx::T3DIPoint& rr);
std::istream& operator>>(std::istream& is, crx::T3DIPoint& dt);


}//crx



#endif /* BASE__T3DPOINT_H_ */
