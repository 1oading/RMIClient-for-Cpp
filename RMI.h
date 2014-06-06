class MYINDENTIFYCLASS
{

};

struct RemoteFunctionStub
{
	void* mReturnValue;
	int mReturnValueLength;
	void* mPara;
	int mParaCount;
	int mStructLength;
	std::string mFunctionID;
};

template<
typename ReturnValue,
typename P1 = MYINDENTIFYCLASS,
typename P2 = MYINDENTIFYCLASS,
typename P3 = MYINDENTIFYCLASS>
class FunctionObject
{
public:
	FunctionObject()
	{
		mParaCount = 2;
	}

	ReturnValue operator() (const char* tRomateFunction, RMIClientBase* pClient, P1 p1, P2 p2)
	{
		ReturnValue r;
		mStub.mFunctionID = tRomateFunction;

		mStub.mReturnValue = (void*)&r;
		mStub.mReturnValueLength = sizeof(ReturnValue);

		mUnionList[0].p1 = p1;
		mUnionList[0].p2 = p2;

		mStub.mPara = (void*)mUnionList;
		mStub.mParaCount = mParaCount;
		mStub.mStructLength = sizeof(UNIONTYPE) * mParaCount - 1;

		pClient->CallRemoteFunction(&mStub);
		return r;
	}

private:

	int mParaCount;
	union UNIONTYPE {
		ReturnValue mReturnValue;
		P1 p1;
		P2 p2;
	};

	UNIONTYPE mUnionList[3];
	RemoteFunctionStub mStub;
};

class RMIClientBase
{
public:
	RMIClientBase();
	~RMIClientBase();
	bool Connect(const char* tRomateIP, int tRomatePort);
	void stopConnect();
	bool isAvailable();
	bool CallRemoteFunction(RemoteFunctionStub* stub);

private:
	bool sendMessage(void* pValue, int tSendLength);

	bool mConnected;
	int mPort;
	std::string mRemoteIP;

	WSADATA wsaData;
	sockaddr_in mRemoteAddr;
	SOCKET mRemoteSocket;
};
