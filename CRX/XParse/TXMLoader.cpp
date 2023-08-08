#include <CRX/XParse/TXMLoader.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Base/TVFSRoot.h>


namespace crx {


TXMLoader::TXMLoader(const std::string& name, const std::string& content) : TXMNode(name, content) {}


void TXMLoader::Load(const std::string& fileName) {
	auto aa = fileName.find(".cxm");
	auto con = TVFSRoot::GetInstance()->LoadWithIncludes(fileName);
	Parse(con, aa!=std::string::npos);
}


}//crx

