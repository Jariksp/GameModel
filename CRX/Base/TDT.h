#ifndef CRX_TDT__H_H
#define CRX_TDT__H_H

#include <string>

namespace crx {

namespace TDebug {

struct TDT {
	TDT(const std::string& s);
	~TDT();

	void Start(const std::string& n2="");
	uint64_t End();

	protected:
	uint64_t Begin = 0;
	std::string Desc;
	bool EndCalled = false;
};

}//TDebug

}//crx




#endif
