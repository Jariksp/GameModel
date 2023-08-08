#include <CRX/Base/TDateInterval.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CRX/Base/TDebug.h>


namespace crx {

TDateInterval::TDateInterval(const std::string& s) { Set(s); }
TDateInterval::TDateInterval(unsigned secs) { Set(secs); }

void TDateInterval::Set(const std::string& s) {
	unsigned ss = 0;
	if( s.size() ) {
		sscanf( s.c_str(), "%u",  &ss );
		char c = tolower( s[s.size()-1] );
		if(c=='h') ss *= 3600;
		else if(c=='m') ss *= 60;
	}//if
	
	m_uSecs = ss;
}

void TDateInterval::Set(unsigned secs) { m_uSecs = secs; }

unsigned TDateInterval::Get() const { return m_uSecs; }

}
