#ifndef CRX_TFILE_H_H_H_
#define CRX_TFILE_H_H_H_

#include <string>
#include <vector>
#include <fstream>


namespace crx {

class TFile {
	public:
	TFile() = default;
	TFile(const std::string& fileName, bool openReadOnly=false);
	virtual  ~TFile();

	public:
	void Create(const std::string& fileName);//create new file and open it
	void Open(const std::string& fileName, bool openReadOnly=false);
	void Close();
	void Unlink();
	const std::string& Name() const;

	public:
	static void Rename(const std::string& oldName, const std::string& newName);
	static void Unlink(const std::string& fileName);
	static void CreateDirectory(const std::string& dirName);
	static bool IsExists(const std::string& fileName);
	static bool IsDirectory(const std::string& fileName);
	static bool IsFile(const std::string& fileName);
	static bool LS(const std::string& dir, std::vector<std::string>& list);//return list of directory
	static std::vector<std::string> LS(const std::string& dir);
	static std::string PrepareFileName(const std::string& fn);//replace all \ to / and remove last / if exists
	
	public:
	uint64_t Size() const;
	uint64_t AvailableSize();

	void Seek(uint64_t newPos, bool fromBegin=true);
	uint64_t Tell();

	unsigned Read(void* ptr, uint64_t size);
	unsigned Write(const void* ptr, uint64_t size);

	unsigned Read(std::string&);//read all from current pos to end
	unsigned Write(const std::string&);//write all string


	void Truncate(uint64_t newSize=0);
	bool IsEof();



	protected:
	std::string m_sName;
	std::fstream m_xFs;


	private://can't be copyend
	TFile(const TFile&);
	void operator=(const TFile&);
	
};

}//crx



#endif
