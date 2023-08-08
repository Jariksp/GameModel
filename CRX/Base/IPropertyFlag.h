#ifndef BASE_IPROPERTYFLAG_H_
#define BASE_IPROPERTYFLAG_H_

#include <memory>


namespace crx {

class IPropertyFlag {
	public:
	inline IPropertyFlag()=default;
	virtual ~IPropertyFlag()=default;

	public:
	virtual std::shared_ptr<IPropertyFlag> Clone()=0;
	virtual void Clear()=0;
	virtual void AddFlags(const std::shared_ptr<IPropertyFlag>& f)=0;
	virtual bool BitwiseAnd(const std::shared_ptr<IPropertyFlag>& f) const=0;
	virtual bool BitwiseAnd(const IPropertyFlag* f) const=0;
};

}

#endif /* BASE_IPROPERTYFLAG_H_ */
