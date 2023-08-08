//#include <CRX/Base/TTCPClientSocket.h>
//#include <curl/curl.h>
//#include <CRX/Base/TDebug.h>
//
//namespace crx {
//
//static long s_iConnectionTimeout = 30;//secons
//
//TTCPClientSocket::TTCPClientSocket() {
//	m_pCURL = curl_easy_init();
//	m_pMulti = curl_multi_init();
//
//	curl_multi_add_handle( (CURLM*)m_pMulti, (CURL*)m_pCURL );
//}
//
//TTCPClientSocket::~TTCPClientSocket() {
//	auto curl = (CURL*)m_pCURL;
//	auto multi = (CURLM*)m_pMulti;
//
//	curl_multi_remove_handle(multi, curl);
//
//	curl_multi_cleanup(multi);
//	curl_easy_cleanup(curl);
//}
//
//
//TTCPClientSocket::NState TTCPClientSocket::State() {
//	CheckState();
//	return m_xState;
//}
//
//void TTCPClientSocket::CheckState() {
//	if(m_xState!=NState::Connecting) return;
//
//	int still_running;
//	auto mc = curl_multi_perform((CURLM*)m_pMulti, &still_running);
//
//	// это хуйня, конечно. Оно и в случае ошибки выдаст такой же результат
//	if(mc!=CURLM_OK) {
//		m_xState = NState::Error;
//		_LOG( "Connecting: "<<curl_multi_strerror(mc) );
//	} else if(still_running==0) {
//		long protocol;
//		curl_easy_getinfo((CURL*)m_pCURL, CURLINFO_PROTOCOL, &protocol);
//		m_xState = protocol==0?NState::Error:NState::Ready;
//	}
//
//
//}
//
//void TTCPClientSocket::Connect(const std::string &ip, int port) {
//	auto curl = (CURL*)m_pCURL;
//
//	std::string url = ip + std::string(":")+std::to_string(port);
//
//	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
//	curl_easy_setopt(curl, CURLOPT_TIMEOUT, s_iConnectionTimeout);
//
//
//	m_xState = NState::Connecting;
//	CheckState();
//}
//
//TTCPClientSocket::NCode TTCPClientSocket::Send(const void *data, unsigned size, unsigned& realSend) {
//	size_t sent = 0;
//	auto curl = (CURL*)m_pCURL;
//	auto res = curl_easy_send(curl, data, size, &sent);
//
//	realSend = (unsigned)sent;
//	return ParseResult(res, "Send");
//}
//
//
//TTCPClientSocket::NCode TTCPClientSocket::Recv(void *buffer, unsigned recvSize, unsigned& realRecv) {
//	auto curl = (CURL*)m_pCURL;
//	size_t nread = 0;
//	auto res = curl_easy_recv(curl, buffer, recvSize, &nread);
//
//	realRecv = (unsigned)nread;
//	return ParseResult(res, "Recv");
//}
//
//TTCPClientSocket::NCode TTCPClientSocket::ParseResult(unsigned code, const std::string& from) {
//	auto res = CURLcode(code);
//
//	if(res!=CURLE_OK) {
//		if(res==CURLE_AGAIN) {
//			return NCode::TryAgain;
//		}
//		_LOG( from<<": "<<curl_easy_strerror(res) );
//		m_xState = NState::Error;
//		return NCode::Error;
//	}
//
//	return NCode::OK;
//}
//
//
//} /* namespace crx */
