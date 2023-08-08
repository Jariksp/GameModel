#ifndef CRX_REFLECTION_IREFLECTIONFUNCTIONCALLOPERATOR_H_
#define CRX_REFLECTION_IREFLECTIONFUNCTIONCALLOPERATOR_H_

#include <any>
#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <functional>
#include <CRX/Base/TMiscUtils.h>



namespace crx {

class TReflectionObject;

class IReflectionFunctionCallOperator {
	public:
	virtual ~IReflectionFunctionCallOperator()=default;

	public:
	const std::string& ClassTypeName() { return m_sClassTypeName; }
	const std::vector<std::string>& ArgsTypeNames() { return m_vArgsTypeNames; }

	public:
	virtual void Call(TReflectionObject* obj, std::vector<std::any>& agrs)=0;

	protected:
	std::string m_sClassTypeName;
	std::vector<std::string> m_vArgsTypeNames;
};



}//crx




#endif /* REFLECTION_IREFLECTIONFUNCTIONCALLOPERATOR_H_ */
