#ifndef CRX_TXMLOADERH_H_
#define CRX_TXMLOADERH_H_

#include <CRX/XParse/TXMNode.h>

namespace crx {

// загружает из VFS файлы, находит в них #include FILE_NAME и заменяет на содержимое FILE_NAME
// а после парсит исходя из file extention. Если оно .cxm - то как CXM, иначе как XM

class TXMLoader : public TXMNode {
	public:
	TXMLoader() = default;
	TXMLoader(const std::string& name, const std::string& content);
	virtual ~TXMLoader() = default;

	public:
	virtual void Load(const std::string& fileName);

};

}//crx

#endif
