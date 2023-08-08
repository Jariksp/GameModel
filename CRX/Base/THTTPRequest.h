//#ifndef CRX_BASE_THTTPREQUEST_H_
//#define CRX_BASE_THTTPREQUEST_H_
//
//#include <string>
//#include <vector>
//#include <mutex>
//
//
//namespace crx {
//
//namespace NNHTTPRequestError {
//	enum NHTTPRequestError {
//		NoError = 0,
//		Error = 1
//	};
//
//};
//
//typedef NNHTTPRequestError::NHTTPRequestError NCURLError;
//
//class THTTPRequest {
//	public:
//	THTTPRequest();
//	virtual ~THTTPRequest();
//
//	public:
//	void SetPOSTData(const std::string& data);
//	void SetURL(const std::string& url);
//	void AddHeader(const std::string& header);
//
//	public://block current thread
//	void SyncRequest(const std::string& url);
//	void SyncRequest();
//	void Close();
//
//
//	public:
//	const std::string& Data();
//	NCURLError LastError();
//	void SaveDataToFile(const std::string& fileName);
//
//
//	protected:
//	std::string m_sData;
//	NCURLError m_xLastError = NNHTTPRequestError::NoError;
//	std::string m_sSaveToFile;
//
//	protected:
//	std::string m_sPostData;
//	std::string m_sURL;
//	std::vector<std::string> m_vHeaders;
//
//	protected:
//	void* m_pCURL = nullptr;
//	bool m_bBreak = false;
//	std::mutex m_xMutex;
//};
//
//}// crx
//
//#endif /* BASE_THTTPREQUEST_H_ */