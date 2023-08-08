#include <CRX/Base/TStringUtils.h>
#include <CRX/Base/TDebug.h>
#include <CRX/Base/TFile.h>
#include <CRX/Base/TVFSRoot.h>

namespace crx {

static std::string s_sIncludeTag("#include");

bool TVFSRoot::IsExists(const std::string& fn) { return fn.size() && TFile::IsExists( PrepareFileName(fn) ); }

bool TVFSRoot::IsDirectory(const std::string& fn) { return fn.size() && TFile::IsDirectory( PrepareFileName(fn) ); }

bool TVFSRoot::IsFile(const std::string& fn) { return fn.size() && TFile::IsFile( PrepareFileName(fn) ); }

bool TVFSRoot::IsFileExists(const std::string& fn) { return IsExists(fn) && IsFile(fn); }

bool TVFSRoot::IsDirectoryExists(const std::string& fn) { return IsExists(fn) && IsDirectory(fn); }

std::string TVFSRoot::FindExists(const std::vector<std::string>& vv) {
	for(const auto& fn:vv) if( IsExists(fn) ) return fn;
	return "";
}

std::vector<std::string> TVFSRoot::FilesList(const std::string& fn) {
	std::vector<std::string> vv;
	TFile::LS( PrepareFileName(fn), vv );
	return vv;
}



std::string TVFSRoot::LoadTxtFile(const std::string& fn) {
	TFile st( PrepareFileName(fn) );
	std::string str;
	st.Read(str);
	return str;
}

void TVFSRoot::SaveTxtFile(const std::string& infn, const std::string& data) {
	auto fn = PrepareFileName(infn);
	TFile file;
	file.Create(fn);
	file.Write(data);
}

void TVFSRoot::SaveTxtFile(const std::string& dir, const std::string& infn, const std::string& data) {
	if( !IsDirectoryExists(dir) ) TFile::CreateDirectory( PrepareFileName(dir) );
	
	auto fn = AssembleFilePath(dir, infn);
	SaveTxtFile(fn, data);
}

void TVFSRoot::SetLocale(const std::string& l) { m_sLocale = l; }

std::string TVFSRoot::PrepareVFSFileName(const std::string& infn) {
	if(infn.empty()) return infn;

	auto n = ReplaceAll(infn, "//", "/");
	n = ReplaceAll(n, "./", "/");
	if(n[0]!='/') n = std::string("/")+n;
	return n;
}

std::string TVFSRoot::PrepareFileName(const std::string& fn) {
	auto ss = AssembleFilePath(m_sBasePath, fn);
	if( m_sLocale.size() ) ss = ReplaceAll(ss, "%Locale%", m_sLocale );
	return ss;
}

void TVFSRoot::BasePath(const std::string& s) {
	m_sBasePath = s;
	if( m_sBasePath.size() && m_sBasePath[m_sBasePath.size()-1]!='/' ) m_sBasePath += '/';
}

const std::string& TVFSRoot::BasePath() { return m_sBasePath; }

std::string TVFSRoot::LoadWithIncludes(const std::string& fn, bool checkForComments) {
	auto str = LoadTxtFile(fn);

	auto relativePath = GetDirectoryPath(fn);
	size_t pos = 0;

	while(true) {
		pos = str.find( s_sIncludeTag, pos );
		if( pos==std::string::npos ) break;
		auto p2 = pos;

		for(;p2<str.size()&&str[p2]!='\n';p2++);

		std::string fileName = str.substr(pos, p2-pos);

		if(checkForComments) {
			auto p3 = pos;
			while(p3>0 && str[p3]!='\n') p3--;
			if(p3>0) p3++;
			auto comment = Trim(str.substr(p3, pos-p3));
			if(comment.find("//")==0) {
				pos = p2;
				continue;
			}
		}

		fileName = Trim( fileName.substr( s_sIncludeTag.size(), fileName.size() ) );

		if(fileName[0]!='/') fileName = relativePath + fileName;

		auto s1 = str.substr(0, pos);
		auto s2 = str.substr(p2+1);

		str = s1 + LoadWithIncludes(fileName, checkForComments) + s2;
	}
	return str;

}//LoadWithIncludes

std::string TVFSRoot::FindImage(const std::string dir, const std::string &fileName) {
	static std::vector<std::string> exts = {"jpg", "jpeg", "png"};
	return FindWithExtention(dir, fileName, exts);
}

std::string TVFSRoot::FindVideo(const std::string dir, const std::string &fileName) {
	static std::vector<std::string> exts = {"mp4", "avi"};
	return FindWithExtention(dir, fileName, exts);
}

std::string TVFSRoot::FindAudio(const std::string dir, const std::string &fileName) {
	static std::vector<std::string> exts = {"ogg", "wav"};
	return FindWithExtention(dir, fileName, exts);
}

std::string TVFSRoot::FindWithExtention(const std::string dir, const std::string &fileName, const std::vector<std::string> &exts) {
	auto vv = FilesList(dir);
	for(auto& full:vv) {
		if( full.find(fileName)==0 ) {
			auto ee = GetFileExtension(full);
			for(auto& ext:exts) if(ext==ee) return full;
		}
	}

	return "";
}

}// crx


