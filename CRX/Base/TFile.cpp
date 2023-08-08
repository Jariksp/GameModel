#include <CRX/Base/TDebug.h>
#include <CRX/Base/TFile.h>
#include <CRX/Base/TStringUtils.h>

#include <filesystem>


namespace crx {

	/*Here everywhere for file and directory names is used 'std::filesystem::u8path()'
	It seems to the only way to make utf-8 work on both windows and linux
	The problem is that u8path deprecated in C++20
	It does't mean it would be deleted in C++20, but there would be another way to work with utf-8 paths
	In C++20 such construction would be available: 'std::filesysterm::path(u8"some/path")'
	So we can use std::filesystem::path(to_utf8(filepath) from TStringUtils.h when moving to C++20
	But it's no so that much necessary*/

TFile::TFile(const std::string& fn, bool ro) { Open(fn, ro); }
TFile::~TFile() { Close(); }

void TFile::Truncate(uint64_t size) {
	std::error_code ec;
	std::filesystem::resize_file(std::filesystem::u8path(m_sName), size, ec);
	if(ec) _ERROR("Truncate "<<m_sName<<" to size "<<size);
}


bool TFile::IsEof() { return !AvailableSize(); }


void TFile::Create(const std::string& fn) {
	Close();
	
	std::ofstream f(std::filesystem::u8path(fn));// We need file to be opened right after creating and available for reading and writing.
	f.close(); //std::ofsream allows only to write into file.
	m_sName = fn;
	if(!IsExists(m_sName)) _ERROR("Create file " << fn);
	Open(fn);//Here could be used std::fstream for writing and reading, but it doesn't create file. crx::TFile::Open(std::string fn) will open ii using std::fstream
}


void TFile::Open(const std::string& fn, bool ro) {
	Close();

	std::ios_base::openmode flag = ro?(std::ios::in | std::ios::binary):(std::ios::in | std::ios::out | std::ios::binary);

	m_xFs.open(std::filesystem::u8path(fn), flag);
	m_sName = fn;
	if(!m_xFs.is_open()) _ERROR("Open file " << fn);
}


void TFile::Rename(const std::string& old, const std::string& fn) {
	std::error_code ec;
	std::filesystem::rename(std::filesystem::u8path(old), std::filesystem::u8path(fn), ec);

	if(ec) _ERROR("Can't rename file "<< old<<" to " << fn);
}


void TFile::Unlink(const std::string& fn) {
	std::error_code ec;
	std::filesystem::remove_all(std::filesystem::u8path(fn), ec);

	if(ec) _ERROR("Remove in "<<fn);
}


bool TFile::IsExists(const std::string& fn) { return std::filesystem::exists(std::filesystem::u8path(fn)); }


bool TFile::IsDirectory(const std::string& fn1) {
	std::string fn = PrepareFileName(fn1);
	return std::filesystem::is_directory(std::filesystem::u8path(fn));
}


bool TFile::IsFile(const std::string& fn1) {
	auto fn = PrepareFileName(fn1);
	return std::filesystem::is_regular_file(std::filesystem::u8path(fn));
}

std::vector<std::string> TFile::LS(const std::string& dir) {
	std::vector<std::string> vv;
	LS(dir, vv);
	return vv;
}

bool TFile::LS(const std::string& fn, std::vector<std::string>& vv) {
	vv.clear();
	
	auto a = PrepareFileName(fn);

	auto filePath = std::filesystem::u8path(a);

	if(!IsExists(a)) return false;

	for(auto const& dir_entry : std::filesystem::directory_iterator{ filePath }) {
		std::string curObject = dir_entry.path().filename().u8string();

		if(curObject=="." || curObject=="..") continue;
		vv.push_back(curObject);
	}
	return true;

}


void TFile::CreateDirectory(const std::string& dn) {
	auto isCreated = std::filesystem::create_directories(std::filesystem::u8path(dn));
	if(!isCreated) _ERROR("Can't create directory '" <<dn<<"'");
}


void TFile::Close() {
	if(!m_xFs.is_open()) return;
	m_xFs.close();
	m_sName = "";
}


void TFile::Unlink() {
	auto s = m_sName;
	Close();
	if( s.empty() ) return;
	std::filesystem::remove(std::filesystem::u8path(s));
}


uint64_t TFile::Size() const { return (uint64_t)std::filesystem::file_size(std::filesystem::u8path(m_sName)); }
	

void TFile::Seek(uint64_t np, bool fb) {
	std::ios_base::seekdir a = (fb) ? std::ios::beg : std::ios::end;

	m_xFs.seekg(np, a);
	if(m_xFs.fail()) _ERROR("Seek");
}


uint64_t TFile::Tell()  { return m_xFs.tellg(); }


uint64_t TFile::AvailableSize() {
	return Size()-m_xFs.tellg();
}


unsigned TFile::Read(void* ptr, uint64_t size) {
	//if(!IsExists(m_sName)) _ERROR("Read from " << m_sName);

	m_xFs.read((char *)ptr, size);
	auto a = !m_xFs.badbit & !m_xFs.failbit & !m_xFs.eofbit;
	//if(a) _ERROR("Read from: "<<m_sName);
	return a;
}

unsigned TFile::Write(const void* ptr, uint64_t size) {
	//if(!IsExists(m_sName)) _ERROR("Write to " << m_sName);

	m_xFs.write((const char*)ptr, size);
	auto a = !m_xFs.badbit & !m_xFs.failbit;
	//if(a) _ERROR("Write to:" << m_sName);
	return a;
}


unsigned TFile::Read(std::string& s) {
	auto size = AvailableSize();
	s.resize(size);
	return Read( (void*)s.data(), size );
}

unsigned TFile::Write(const std::string& s) { return Write( s.data(), s.size() ); }


std::string TFile::PrepareFileName(const std::string& fn) {
	auto path = ReplaceAll(fn, "\\", "/");
	if(path[path.size()-1]=='/') path = path.substr(0, path.size()-1);
	
	if(path[path.size()-1]==':') path += "/";
	return path;
}


const std::string& TFile::Name() const { return m_sName; }

TFile::TFile(const TFile&) { _ERROR("TFile can't be copyed"); }
void TFile::operator=(const TFile&) { _ERROR("TFile can't be copyed"); }

}//crx
