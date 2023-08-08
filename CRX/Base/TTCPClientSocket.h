//#ifndef BASE_TTCPCLIENTSOCKET_H_
//#define BASE_TTCPCLIENTSOCKET_H_
//
//#include <string>
//
//
//namespace crx {
//
//// это обвязка вокруг libcurl, чтобы была возможность межплатформенно по TCP подключаться
//// libcurl вроде как на всех target платформах необходимых нам есть
//
//
//class TTCPClientSocket {
//	public:
//	TTCPClientSocket();
//	virtual ~TTCPClientSocket();
//
//	public:
//	enum class NCode {
//		OK,
//		TryAgain,
//		Error
//	};
//
//	enum class NState {
//		None,
//		Connecting,
//		Ready,
//		Error
//	};
//
//
//	public:
//	// коннет асинхронный, проверка выполняется в State
//	virtual void Connect(const std::string& ip, int port);
//	NState State();
//
//	// а вот Send/Recv асинхронные
//	virtual NCode Send(const void* data, unsigned size, unsigned& realSend);
//	virtual NCode Recv(void* buffer, unsigned recvSize, unsigned& realRecv);
//
//	protected:
//	virtual NCode ParseResult(unsigned code, const std::string& from);
//	virtual void CheckState();
//
//	protected:
//	void* m_pCURL = nullptr;
//	void* m_pMulti = nullptr;
//	NState m_xState = NState::None;
//};
//
//} /* namespace crx */
//
//#endif /* BASE_TTCPCLIENTSOCKET_H_ */
