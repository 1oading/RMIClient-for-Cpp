#include "MRI.h"

RMIClientBase::RMIClientBase()
{
	mConnected = false;
}

RMIClientBase::~RMIClientBase()
{
	stopConnect();
}

bool RMIClientBase::Connect(const char* tRomateIP, int tRomatePort)
{
	mRemoteIP = tRomateIP;
	mPort = tRomatePort;
	if (mConnected)
	{
		return false;
	}

	WSAStartup(MAKEWORD(1,1), &wsaData);

	mRemoteSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	mRemoteAddr.sin_addr.S_un.S_addr = inet_addr(tRomateIP);
	mRemoteAddr.sin_family = AF_INET;
	mRemoteAddr.sin_port = htons(tRomatePort);
	if (connect(mRemoteSocket, (SOCKADDR*)&mRemoteAddr, sizeof(SOCKADDR)))
	{
		WSACleanup();
		return false;
	}

	mConnected = true;
	return true;
}

void RMIClientBase::stopConnect()
{
	if (mConnected)
	{
		closesocket(mRemoteSocket);
	}
	WSACleanup();

	mConnected = false;
}

bool RMIClientBase::isAvailable()
{
	return mConnected;
}

bool RMIClientBase::CallRemoteFunction(RemoteFunctionStub* stub)
{
	int tStrLength = stub->mFunctionID.length();
	if (!sendMessage((void*)&tStrLength, sizeof(tStrLength)))
	{
		return false;
	}

	if (!sendMessage((void*)stub->mFunctionID.c_str(), stub->mFunctionID.length()))
	{
		return false;
	}

	if (!sendMessage((void*)stub->mParaCount, sizeof(stub->mParaCount)))
	{
		return false;
	}

	if (!sendMessage((void*)stub->mStructLength, sizeof(stub->mStructLength)))
	{
		return false;
	}

	if (!sendMessage((void*)stub->mPara, sizeof(stub->mPara)))
	{
		return false;
	}

	char* pReBuffer = new char[stub->mReturnValueLength];
	memset((void*)pReBuffer, 0, stub->mReturnValueLength);

	int nReCount = recvfrom(mRemoteSocket, pReBuffer, stub->mReturnValueLength, 0, NULL, NULL);
	if (nReCount == stub->mReturnValueLength)
	{
		memcpy(stub->mReturnValue, pReBuffer, nReCount);
		delete pReBuffer;
		return true;
	}
	else
	{
		delete pReBuffer;
		return false;
	}
}

bool RMIClientBase::sendMessage(void* pValue, int tSendLength)
{
	if (!mConnected)
	{
		return false;
	}
	if (send(mRemoteSocket, (char*)pValue, tSendLength, 0) < tSendLength)
	{
		return false;
	}
	return true;
}
