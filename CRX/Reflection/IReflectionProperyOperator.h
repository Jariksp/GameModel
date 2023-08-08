#ifndef CRX_REFLECTION_IREFLECTIONPROPERYOPERATOR_H_
#define CRX_REFLECTION_IREFLECTIONPROPERYOPERATOR_H_

#include <any>
#include <CRX/Base/TMiscUtils.h>
#include <CRX/Reflection/TReflectionCommonTypeIdent.h>

namespace crx {

class TReflectionObject;

// -------------------- BASE ----------------------- //

class IReflectionProperyOperatorBase : public virtual TReflectionCommonTypeIdent {
	public:
	virtual ~IReflectionProperyOperatorBase()=default;
};


class IReflectionProperyOperatorGet : public virtual  IReflectionProperyOperatorBase {
	public:
	virtual ~IReflectionProperyOperatorGet()=default;
	virtual void Get(TReflectionObject* obj, std::any& copy_to)=0;
};

class IReflectionProperyOperatorSet : public virtual IReflectionProperyOperatorBase {
	public:
	virtual ~IReflectionProperyOperatorSet()=default;
	virtual void Set(TReflectionObject* obj, const std::any& copy_from)=0;
};


class IReflectionProperyLazyInitializationChecker {
	public:
	virtual ~IReflectionProperyLazyInitializationChecker()=default;
	virtual bool IsNullptr(TReflectionObject* obj)=0;
};





}//crx




#endif /* REFLECTION_IREFLECTIONPROPERYOPERATOR_H_ */
