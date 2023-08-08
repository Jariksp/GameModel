//#include <curl/curl.h>
//#include <CRX/Base/TDebug.h>
//#include <stdio.h>
//#include <CRX/Base/TFile.h>
//#include <CRX/Base/THTTPRequest.h>
//#include <thread>
//
//namespace crx {
//
//THTTPRequest::THTTPRequest() {
//	m_pCURL = curl_easy_init();
//}
//
//THTTPRequest::~THTTPRequest() {
//	std::lock_guard<std::mutex> ll(m_xMutex);
//	curl_easy_cleanup( (CURL*)m_pCURL );
//}
//
//const std::string& THTTPRequest::Data() { return m_sData; }
//
//
//NCURLError THTTPRequest::LastError() { return m_xLastError; }
//
//void THTTPRequest::SaveDataToFile(const std::string &fileName) { m_sSaveToFile = fileName; }
//
//void THTTPRequest::SetPOSTData(const std::string& data) { m_sPostData = data; }
//
//static size_t CurlWrite_CallbackFunc_StdString(void* contents, size_t size, size_t nmemb, std::string* s) {
//	size_t newLength = size*nmemb;
//	s->append((char*)contents, newLength);
//	return newLength;
//}
//
//
//void THTTPRequest::SyncRequest(const std::string &url) {
//	m_sURL = url;
//	SyncRequest();
//}
//
//void THTTPRequest::Close() {
//	std::lock_guard<std::mutex> ll(m_xMutex);
//	m_bBreak = true;
//	curl_easy_reset((CURL*)m_pCURL);
//}
//
//void THTTPRequest::SyncRequest() {
//	auto curl = (CURL*)m_pCURL;
//
//	#define CHECK() if(m_bBreak) { m_xLastError = NNHTTPRequestError::Error; return; }
//
//	if(!curl) {
//		m_xLastError = NNHTTPRequestError::Error;
//		return;
//	}
//
//	CHECK();
//
//	m_xMutex.lock();
//
//	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
//
//	curl_easy_setopt( curl, CURLOPT_URL, m_sURL.c_str() );
//
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_sData);
//	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //remove this to disable verbose output
//	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
//
//
//	if(m_sPostData.size()) {
//		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_sPostData.c_str());
//		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, m_sPostData.size());
//	}
//
//
//
//	struct curl_slist *list = nullptr;
//	if(m_vHeaders.size()) {
//		for(auto& a:m_vHeaders) list = curl_slist_append(list, a.c_str());
//		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
//	}
//
//	m_xMutex.unlock();
//
//	CHECK();
//	std::this_thread::yield();// это немного замедляет общую работу, но позволяет гарантировано разбить проверку - если Close захватил мутекс
//	CHECK();
//
//	auto res = curl_easy_perform(curl);
//	if(list) curl_slist_free_all(list);
//
//	CHECK();
//
//	// Check for errors
//	if(res != CURLE_OK) {
//		_LOG("curl_easy_perform() failed: "<<curl_easy_strerror(res));
//		m_xLastError = NNHTTPRequestError::Error;
//	} else {
//		m_xLastError = NNHTTPRequestError::NoError;
//
//		if(m_sSaveToFile.size()) {
//			TFile fn;
//			fn.Create(m_sSaveToFile);
//			fn.Write(m_sData);
//		}
//
//	}
//}
//
//
//void THTTPRequest::SetURL(const std::string &url) { m_sURL = url; }
//
//void THTTPRequest::AddHeader(const std::string& header) { m_vHeaders.push_back(header); }
//
//}// crx
//
//
//
//
