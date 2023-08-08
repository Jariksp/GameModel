#ifndef REFLECTION_MAKESHARED_H_
#define REFLECTION_MAKESHARED_H_

#include <CRX/Reflection/IVirtualConstructor.h>
#include <memory>

namespace crx {


template<typename _Tp, typename... _Args>
inline std::shared_ptr<_Tp> MakeShared(_Args&&... __args) {
	auto pp = std::make_shared<_Tp>( std::forward<_Args>(__args)... );

	auto vc = dynamic_cast<IVirtualConstructor*>(pp.get());
	if(vc) vc->VirtualConstructor();

	return pp;
}





}//crx




#endif /* REFLECTION_MAKESHARED_H_ */
