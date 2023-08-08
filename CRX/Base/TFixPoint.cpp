#include <CRX/Base/TFixPoint.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Reflection/TReflectionStringConverter.h>
#include <CRX/Reflection/TReflectionTypeFactoryTemplate.h>
#include <limits>
#include <CRX/Base/TMiscUtils.h>

namespace crx {

const TFixPoint TFixPoint::MAX = std::numeric_limits<int64_t>::max();
const TFixPoint TFixPoint::MIN = std::numeric_limits<int64_t>::min();

void TFixPoint::operator=(double d) { m_iData = llround(d*dPointBase); }


int64_t TFixPoint::Data() const { return m_iData; }


void TFixPoint::operator +=(double s) { m_iData += llround(s*dPointBase); }

void TFixPoint::operator -=(double s) { m_iData -= llround(s*dPointBase); }

void TFixPoint::operator *=(double s) {
	m_iData *= llround(s*dPointBase);
	m_iData /= PointBase;
}

void TFixPoint::operator /=(double s) {
	m_iData *= PointBase;
	m_iData /= llround(s*dPointBase);
}

bool TFixPoint::operator ==(double f) const { return m_iData==llround(f*dPointBase); }

bool TFixPoint::operator !=(double f) const { return m_iData!=llround(f*dPointBase); }

bool TFixPoint::operator <(double f) const { return m_iData<llround(f*dPointBase); }

bool TFixPoint::operator >(double f) const { return m_iData>llround(f*dPointBase); }

bool TFixPoint::operator <=(double f) const { return m_iData<=llround(f*dPointBase); }

bool TFixPoint::operator >=(double f) const { return m_iData>=llround(f*dPointBase); }

std::string TFixPoint::ToString() const {
	auto h = std::to_string( std::abs(m_iData/PointBase) );
	auto l = std::to_string( std::abs(m_iData%PointBase) );
	
	while(l.size()<3) l = std::string("0")+l;

	return (m_iData<0?"-":"")+h+"."+l;
}

std::string TFixPoint::ToString(unsigned decimalDigits, bool fixDigits, bool forceusePlus) const {
	static char buf[128];
	static char fixFmtPlus[] = "%+.*f";
	static char floatFmtPlus[] = "%+g";
	static char fixFmt[] = "%.*f";
	static char floatFmt[] = "%g";

	auto val = round(*this, decimalDigits).AsDouble();

	if(forceusePlus) {
		if(fixDigits) {
			sprintf(buf, fixFmtPlus, decimalDigits, val);
		} else {
			sprintf(buf, floatFmtPlus, val);
		}
	} else {
		if(fixDigits) {
			sprintf(buf, fixFmt, decimalDigits, val);
		} else {
			sprintf(buf, floatFmt, val);
		}
	}


	return buf;
}

TFixPoint TFixPoint::FromString(const std::string &s) {
	auto d = crx::to_double(s);
	return TFixPoint(d);
}

class TFixPointStringConverter : public crx::TReflectionStringConverter {
	public:
	TFixPointStringConverter() { SetNativeTypeInfo( &typeid(TFixPoint) ); }



	virtual void Set(std::any& out, const std::string& value) {
		out = TFixPoint::FromString(value);
	}

	virtual void Get(std::string& out, const std::any& value) {
		out = std::any_cast<const TFixPoint&>(value).ToString();
	}


};


void TFixPoint::InitReflection() {
	crx::TReflectionStringConverter::Add( new TFixPointStringConverter() );
	crx::impl::AddReflectionTypeFactoryTemplateIfNo<TFixPoint>();
}

TFixPoint TFixPoint::min(const TFixPoint& a, const TFixPoint& b) {
	return a.m_iData<b.m_iData?a:b;

}

// ------------------------------- //

TFixPoint abs(const TFixPoint& f) { return f<0?-f:f; }

//TFixPoint sqr(const TFixPoint& f) { return f*f; }

/*
TFixPoint sqrt(const TFixPoint& f) {
	double aa = double(f.Data())/f.PointBase;
	return TFixPoint( ::sqrt(aa) );
}
*/

TFixPoint sign(const TFixPoint& f) {
	if(f<0) return -1;
	else if(f>0) return +1;
	return 0;
}

TFixPoint round(const TFixPoint& f) {
	return crx::iround(double(f.Data())/f.PointBase);
}

TFixPoint round(const TFixPoint& f, unsigned ubase) {
	auto base = pow(10.0, double(ubase));
	return round( (double(f.Data())/f.PointBase)*base )/(int)base;
}

int iround(const TFixPoint& f) { return round(f).AsInt(); }



std::ostream& operator <<(std::ostream &st, const TFixPoint &rr) {
	st<<rr.ToString();
	return st;
}


TFixPoint RoundTo(const TFixPoint& f, int mustDiv) {
	return iround(std::floor( 0.5 + (f.AsDouble()/double(mustDiv)) ))*mustDiv;
}


} /* namespace crx */
