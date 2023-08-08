#include <CRX/Base/T3DIPoint.h>
#include <CRX/Base/TDebug.h>

namespace crx {




std::ostream& operator<<(std::ostream& st, const crx::T3DIPoint& rr) { return crx::OperatorToOS(st, rr); }
std::istream& operator>>(std::istream& is, crx::T3DIPoint& dt) { return crx::OperatorFromIS(is, dt); }




}//crx





