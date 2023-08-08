#ifndef BASE_T3DBOOLPOINT_H_
#define BASE_T3DBOOLPOINT_H_

#include <CRX/Base/TBasePointClass.h>

namespace crx {

class T3DBoolPoint : public TBasePointClass<bool, T3DBoolPoint> {
	public:
	inline T3DBoolPoint()=default;
	inline T3DBoolPoint(bool x1, bool y1, bool z1=false) {
		x = x1; y = y1; z = z1;
	}


};

std::ostream& operator<<(std::ostream& st, const crx::T3DBoolPoint& rr);
std::istream& operator>>(std::istream& is, crx::T3DBoolPoint& dt);


} /* namespace crx */

#endif /* BASE_T3DBOOLPOINT_H_ */
