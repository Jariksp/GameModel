#include <CRX/Reflection/IReflectionFunctionCallOperator.h>
#include <CRX/Reflection/TReflectionClassFunctionData.h>
#include <CRX/Reflection/TReflectionGlobals.h>

namespace crx {

TReflectionClassFunctionData::TReflectionClassFunctionData(const std::string &name, IReflectionFunctionCallOperator *op) {
	m_sName = name;
	m_pOperator = op;
}


void TReflectionClassFunctionData::Add(TReflectionClassFunctionData* fn) {
	TReflectionGlobals::GetInstance()->Add(fn->ClassTypeName(), fn);
}

TReflectionClassFunctionData* TReflectionClassFunctionData::Add(const std::string &publishAs, IReflectionFunctionCallOperator *op) {
	auto fn = new TReflectionClassFunctionData(publishAs, op);
	Add( fn );
	return fn;
}


const std::string& TReflectionClassFunctionData::Name() { return m_sName; }

IReflectionFunctionCallOperator* TReflectionClassFunctionData::Operator() { return m_pOperator; }

std::string TReflectionClassFunctionData::TypeName() {
	std::string s;
	const auto& vv = m_pOperator->ArgsTypeNames();
	for(const auto& a:vv) {
		if(s.size()) s += ",";
		s += a;
	}

	s = std::string("void(") + s + std::string(")");
	return s;
}


const std::string& TReflectionClassFunctionData::ClassTypeName() { return m_pOperator->ClassTypeName(); }

}//crx



