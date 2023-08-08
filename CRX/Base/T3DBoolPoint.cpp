#include <CRX/Base/T3DBoolPoint.h>

namespace crx {


std::ostream& operator<<(std::ostream& st, const crx::T3DBoolPoint& rr) { return crx::OperatorToOS(st, rr); }
std::istream& operator>>(std::istream& is, crx::T3DBoolPoint& dt) { return crx::OperatorFromIS(is, dt); }


} /* namespace crx */
