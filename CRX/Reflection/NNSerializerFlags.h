#ifndef REFLECTION_NNSERIALIZERFLAGS_H_
#define REFLECTION_NNSERIALIZERFLAGS_H_

namespace crx {

namespace NNSerializerFlags {
	enum NSerializerFlags {
		None=0x0,
		Zip=0x1,
		Base64=0x2,
		ZipBase64=0x3
	};
};

typedef NNSerializerFlags::NSerializerFlags NSerializerFlags;


}//crx

// пришлось вынести из неймспейса, иначе проблемы с видимостью в других нейсмспесах
inline crx::NSerializerFlags operator | (crx::NSerializerFlags lhs, crx::NSerializerFlags rhs) {
	return crx::NSerializerFlags( unsigned(lhs) | unsigned(rhs) );
}

inline crx::NSerializerFlags& operator |= (crx::NSerializerFlags& lhs, crx::NSerializerFlags rhs) {
    lhs = lhs | rhs;
    return lhs;
}



#endif /* REFLECTION_NNSERIALIZERFLAGS_H_ */
