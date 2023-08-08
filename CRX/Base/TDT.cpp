#ifndef TDT__H_H
#define TDT__H_H

#include <CRX/Base/TMiscUtils.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Base/TDT.h>

namespace crx {

namespace TDebug {


TDT::TDT(const std::string& s) { Desc = s; Start(); }

TDT::~TDT() { if(!EndCalled) End(); }

void TDT::Start(const std::string& n2) {
	if(n2.size()) Desc = n2;
	Begin = crx::MicroTime();
	EndCalled = false;
}

uint64_t TDT::End() {
	auto a = crx::MicroTime() - Begin;
	_LOG( Desc<<": "<<unsigned(a)<<" micro secs\t"<<(double(a)/1000000.0)<<" sec" );
	EndCalled = true;
	return a;
}



}//TDebug
}//crx



#endif
