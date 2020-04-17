
#include "HttpConnect.h"
#include <iostream>
#include <memory.h>
#ifdef WIN32
#include <winsock.h>
#include <tchar.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif // WIN32
#include <sstream>
using namespace std;

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

HttpConnect::HttpConnect()
{
#ifdef WIN32
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
}

HttpConnect::~HttpConnect()
{
	for (std::map<std::string,SocketData>::iterator it = m_vSocket.begin();it!=m_vSocket.end();it++)
	{
		SocketData socketData = it->second;
		closesocket(socketData.socket);
		it->second.socket =0;
	}
	m_vSocket.clear();
}

bool HttpConnect::socketHttp(std::string host, std::string request,unsigned int port)
{
	std::cout << port << endl;

	int sockfd;
	struct sockaddr_in address;
	struct hostent *server;

	SocketData socketData = m_vSocket[host];
	if (socketData.socket==0)
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);	
	}
	else 
	{
		sockfd = socketData.socket;
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	server = gethostbyname(host.c_str());

	if (server == 0)
	{
		return false;
	}

	memcpy((char *)&address.sin_addr.s_addr, (char*)server->h_addr, server->h_length);

	// ʹ��ѭ���򵥴���socket��ʱ�����������رգ���
	do 
	{
		int nRet = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
		if (nRet == 0)
			break;
		if (nRet == WSAEISCONN)
		{
			OutputDebugString(_T("The socket is already connected. "));
			break;
		}
		if(nRet == WSAEISCONN)
		{
			OutputDebugString(_T("A connect request was made on an already connected socket."));
		}
		else if (nRet == WSAETIMEDOUT)
		{
			// ���������رգ���Ҫ���½���socket
			OutputDebugString(_T("Attempt to connect timed out without establishing a connection."));
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			continue;
		}
		if (-1 == nRet)
		{
			TCHAR buf[256] = {0};
			DWORD dwError = WSAGetLastError();
			_stprintf_s(buf,_T("Connection error,Code %d"),dwError);
			OutputDebugString(buf);
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			continue;
		}
		break;
	} while (1);
	socketData.socket = sockfd;
	socketData.lastTime = GetTickCount();

	m_vSocket[host] = socketData;
#ifdef WIN32
	send(sockfd, request.c_str(), request.size(), 0);
#else
	write(sockfd, request.c_str(), request.size());
#endif
	// ע������ᵼ�����б������������
	// char buf[1024 * 1024] = { 0 };
	// �޸�Ϊ100K��Ϊ������
	char buf[1024 * 100] = { 0 };
	int offset = 0;
	int rc;


	bool bHttpHeadHead = false;
	bool bHttpHeadTail = false;

#ifdef WIN32
	while (rc = recv(sockfd, buf + offset, 1024, 0))
#else
	while (rc = read(sockfd, buf + offset, 1024))
#endif
	{
		offset += rc;
		// ����httpͷ��
		if (bHttpHeadHead == false)
		{
			const char * pHttp = strstr(buf, "HTTP/1.");
			if (pHttp)
				bHttpHeadHead = true;
		}
		// �ҵ�httpͷ������httpͷ����β
		if (bHttpHeadHead == true)
		{
			// Content-Length: 294 ��һ���У�����\r\n\r\nһ������
			const char * pContent = strstr(buf, "\r\n\r\n");
			if (pContent)
			{
				// ��һ��Copntent���ݾͽ��������
				if (rc < 1024)
					break;
				bHttpHeadTail = true;
			}
			// Httpβ���ҵ��ˣ�
			if (bHttpHeadTail)
			{
				if (pContent == NULL && rc < 1023)
					break;
			}
		}	
	}
#ifdef WIN32
	//closesocket(sockfd);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     	closesocket(sockfd);
#else
	close(sockfd);
#endif
	buf[offset] = 0;
#ifdef _DEBUG
	//cout << buf << std::endl;
#endif // _DEBUG
	// �������� 
	char *pFind = strstr(buf, "\r\n\r\n");
	if (pFind)
	{
		m_strGetResult.clear();
		if (memcmp(buf, "HTTP/1.0",8) == 0)
		{
			m_strGetResult.append(pFind+4);
		}
		else if (memcmp(buf, "HTTP/1.1",8) == 0)
		{
			char* pData = strstr(pFind+4,"\r\n"); // ��λ�������ݴ�С������
			if (pData)
			{
				m_strGetResult.append(pData);
			}
			else
			{
				m_strGetResult.append(pFind+4);
			}
		}
		else
		{
			m_strGetResult.append(pFind);
		}
	}
	return true;
}

void HttpConnect::postData(std::string host, unsigned int port, std::string path, std::string post_content)
{
	//POST����ʽ
	std::stringstream stream;
	stream << "POST " << path;
	stream << " HTTP/1.1\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Content-Type: application/x-www-form-urlencoded\r\n";
	stream << "Content-Length: " << post_content.length() << "\r\n\r\n";
	stream << post_content.c_str();
	socketHttp(host, stream.str(), port);
}

void HttpConnect::getData1_0(std::string host, unsigned int port, std::string path, std::string get_content)
{
	//GET����ʽ
	std::stringstream stream;
	stream << "GET " << path << "?" << get_content;
	stream << " HTTP/1.0\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Connection: close\r\n\r\n";
	socketHttp(host, stream.str(), port);
}

bool HttpConnect::getData1_1(std::string host, unsigned int port, std::string path, std::string get_content)
{
	//GET����ʽ
	std::stringstream stream;
	stream << "GET " << path << "?" << get_content;
	stream << " HTTP/1.1\r\n";
	stream << "Accept: */*\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Referer: http://www.iqiyi.com/upload?type=0&ufrm=web_iqiyi_uc\r\n";
	stream << "Connection: Keep-Alive\r\n\r\n";
	return socketHttp(host, stream.str(), port);
}

std::string HttpConnect::GetLastRequestResult()
{
	return m_strGetResult;
}
