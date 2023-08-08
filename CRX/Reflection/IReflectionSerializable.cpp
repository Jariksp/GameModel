#include <CRX/Reflection/IReflectionSerializable.h>

namespace crx {

void IReflectionSerializable::AfterConstrictionInDeserializer() {}
void IReflectionSerializable::BeforLoadInDeserializer() {}
void IReflectionSerializable::AfterLoadInDeserializer() {}
void IReflectionSerializable::BeforSaveInSerializer() {}
void IReflectionSerializable::AfterSaveInSerializer() {}
void IReflectionSerializable::AfterAllLoadedInDeserializer() {}

} /* namespace crx */
