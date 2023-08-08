#include <CRX/Base/TDirectoryData.h>
#include <CRX/Base/TStringUtils.h>
#include <CRX/Base/TDebug.h>
#include <math.h>
#include <CRX/Base/TVFSRoot.h>
#include <CRX/Base/TMiscUtils.h>

namespace crx {


std::shared_ptr<TDirectoryData> TDirectoryData::Find(const std::vector<std::string> &path) {
	if(path.empty()) return shared_from_this();

	auto vv = path;
	auto name = ToLowerCase( *vv.begin() );
	vv.erase(vv.begin());

	for(auto a:Subdirs) if(a->m_sName==name) {
		return a->Find(vv);
	}


	return nullptr;
}



std::shared_ptr<TDirectoryData> TDirectoryData::Load(const std::string &path) {
	auto root = std::make_shared<TDirectoryData>();

	root->_Load(path);

	return root;

}


void TDirectoryData::_Load(const std::string& path) {
	m_sPath = path;

	auto vfs = TVFSRoot::GetInstance();

	auto vv = vfs->FilesList(path);

	for(const auto& fn:vv) {
		auto pp = AssemblePath(path, fn);
		if( vfs->IsDirectory(pp) ) {

			auto dd = std::make_shared<TDirectoryData>();
			dd->m_xParent = shared_from_this();
			dd->m_sName = ToLowerCase(fn);

			dd->_Load(pp);
			Subdirs.push_back(dd);
		} else {
			Files.push_back(fn);
		}
	}//for
}

std::vector<std::string> TDirectoryData::FilesWithFullPath() {
	std::vector<std::string> vv;

	for(auto& a:Files) {
		vv.push_back( AssembleFilePath(m_sPath, a) );
	}

	return vv;
}

std::string TDirectoryData::RandomFileWithFullPath(const std::vector<std::string>& path) {
	auto dd = Find(path);
	if(!dd) return "";
	return dd->RandomFileWithFullPath();
}

const std::string& TDirectoryData::RandomFile() {
	if(Files.empty()) _ERROR("NoFiles in : "<<m_sPath);
	
	auto idx = (size_t)iround( double(Files.size())*double(rand())/double(RAND_MAX) );
	if(idx>=Files.size()) idx = Files.size()-1;
	return Files[idx];
}

std::string TDirectoryData::RandomFileWithFullPath() {
	return AssembleFilePath( m_sPath, RandomFile() );
}

const std::string& TDirectoryData::Name() { return m_sName; }

const std::string& TDirectoryData::Path() { return m_sPath; }

}//crx



