#ifndef REFLECTION_IVIRTUALCONSTRUCTOR_H_
#define REFLECTION_IVIRTUALCONSTRUCTOR_H_

namespace crx {

// если класс завернут в shared_ptr, то нельзя shared_ptr использоать из конструктора
// поэтому есть мой MakeShared, который вызывает std::make_shared, а потом пытается сделать каст и вызывает
// виртуальный констуктор

class IVirtualConstructor {
	public:
	IVirtualConstructor()=default;
	virtual ~IVirtualConstructor()=default;

	public:
	virtual void VirtualConstructor()=0;

};


}//crx




#endif /* REFLECTION_IVIRTUALCONSTRUCTOR_H_ */
