#include <string>
#include <map>


#pragma once

typedef struct SocketData
{
	unsigned int socket;
	unsigned long lastTime;
	SocketData()
	{
		socket = 0;
		lastTime = 0;
	}
};

class HttpConnect
{
private:
	std::string m_strGetResult;
public:
	HttpConnect();
	~HttpConnect();
	bool socketHttp(std::string host, std::string request, unsigned int port);
	void postData(std::string host, unsigned int port, std::string path, std::string post_content);
	void getData1_0(std::string host, unsigned int port, std::string path, std::string get_content);
	bool getData1_1(std::string host, unsigned int port, std::string path, std::string get_content);

	// ��ȡ����Ľ��
	std::string GetLastRequestResult();
	// socket�����
	std::map<std::string,SocketData> m_vSocket;
};

