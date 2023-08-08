#include <CRX/Base/TSize.h>
#include <algorithm>


namespace crx {

TSize::TSize(int w1, int h1) { w = w1; h = h1; }

void TSize::Set(int w1, int h1) { w = w1; h = h1; }

void TSize::SetMax(const TSize& s) {
	w = std::max(w, s.w);
	h = std::max(h, s.h);
}

bool TSize::operator==(const TSize& p) const { return w==p.w && h==p.h;  }
void TSize::operator+=(const TSize& p) { w += p.w; h += p.h; }

}//crx

std::ostream& operator<<(std::ostream& st, const crx::TSize& rr) {
	st<<"w="<<rr.w<<" h="<<rr.h;
	return st;
}

