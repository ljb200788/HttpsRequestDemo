// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <istream>
#include <ostream>
#include "HttpConnect.h"
#include "jsoncpp/json.h"
#include "Transcode.h"
#include "LUrlParser.h"
#include "response.h"
#include "httpresponseparser.h"
#include <windows.h>
#include "WininetHttp.h"
#include "curl\include\curl.h"

#include "ylog.h"

using namespace httpparser;
using namespace std;
using namespace LUrlParser;


#include "Thread.h"

class MyThread : public Thread
{
private:
	void run() 
	{
		std::cout<<"看起来不错哟,原来这就是线程类"<<endl;
	}

};

void testMyThread() {
	try {
		MyThread* p = new MyThread();
		p->start();
	}
	catch (std::exception e) {
		std::cout << e.what();
	}

}
char * ReadFileBuf(const char* pFilePath, long * nFileSize)
{

	FILE* pFile;
	fopen_s(&pFile, pFilePath, "rb");
	if (pFile == NULL)
		return NULL;

	_fseeki64(pFile, 0, SEEK_END);
	long nLen = ftell(pFile);
	*nFileSize = nLen;
	_fseeki64(pFile, 0, SEEK_SET);
	// 预留10空间作为越界保护
	char * pbuf = (char*)malloc(nLen + 10);
	if (pbuf)
	{
		memset(pbuf, 0, nLen + 10);
		nLen = fread(pbuf, 1, nLen, pFile);
	} while (0);
	fclose(pFile);
	return pbuf;
}

unsigned int Rand32(unsigned int min, unsigned int max)
{
	return (unsigned int)((double)rand() / (double)RAND_MAX*(max - min + 1) + min);
}


int parseResponse()
{

	std:string strGetResult;
	Response response;
	HttpResponseParser parser;

	HttpResponseParser::ParseResult res = parser.parse(response, strGetResult.c_str(), strGetResult.c_str() + sizeof(strGetResult.c_str()));

	if (res == HttpResponseParser::ParsingCompleted)
	{
		std::cout << response.inspect() << std::endl;
		return EXIT_SUCCESS;
	}
	else
	{
		std::cerr << "Parsing failed" << std::endl;
		return EXIT_FAILURE;
	}
}
std::string GetRamdomBoundary()
{
	std::string str = "-------------------------7";
	for (int i = 0; i<11; i++)
	{
		char ch[2] = { 0 };
		ch[0] = (char)Rand32('a', 'z');
		str.append(ch);
	}
	return str;
}

void test()
{
	clParseURL URL = clParseURL::ParseURL("https://Joh11n:D11ow@github.com:80/corporateshark/LUrlParser/?&query=ssl#q=frag");

	if (URL.IsValid())
	{
		cout << "Scheme    : " << URL.m_Scheme << endl;
		cout << "Host      : " << URL.m_Host << endl;
		cout << "Port      : " << URL.m_Port << endl;
		cout << "Path      : " << URL.m_Path << endl;
		cout << "Query     : " << URL.m_Query << endl;
		cout << "Fragment  : " << URL.m_Fragment << endl;
		cout << "User name : " << URL.m_UserName << endl;
		cout << "Password  : " << URL.m_Password << endl;
	}
	else
	{
		cout << "Parsing error: " << URL.m_ErrorCode << endl;
	}
}

string StringToUtf(string strValue)
{

	int nwLen = MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];//加上末尾'\0'
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), strValue.length(), pwBuf, nwLen);
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr = pBuf;
	delete[]pBuf;
	delete[]pwBuf;
	return retStr;

}

int main()
{

	//test();

	YLog log(YLog::INFO, "log.txt", YLog::ADD);

	CWininetHttp netHttp;
	std::string ret = netHttp.RequestJsonInfo("http://jsqing.cn/MarriageLawService/ListProvince?loginType=1", Hr_Post,
		"Content-Type:application/json;charset=utf-8", "{}");
	
	cout << "ret    : " << netHttp.UtfToGbk(ret.c_str()) << endl;
	log.W(__FILE__, __LINE__, YLog::INFO, "jsqing.cn", StringToUtf(ret));

	ret = netHttp.RequestJsonInfo("https://world.huanqiu.com/api/black_json", Hr_Get,
		"Content-Type:application/json;charset=utf-8", "{}");

	cout << "ret    : " << netHttp.UtfToGbk(ret.c_str()) << endl;

	
	log.W(__FILE__, __LINE__, YLog::INFO, "world.huanqiu.com", StringToUtf(ret));

	return 0;

	std::string g_strfile = "D:\\clipboard.png";
	std::string strFileName;
	std::string::size_type nPos = g_strfile.find_last_of("\\");
	if (nPos != std::string::npos)
	{
		strFileName = g_strfile.c_str() + nPos + 1;
	}

	std::string g_upload_url = "http://localhost:8080/spring-boot-package-war/uploadFile";
	//std::string g_upload_url ="http://jsqing.cn/MarriageLawServiceDocument/UploadContractFile";
	HttpConnect http;
	long g_filesize;

	std::string host;
	std::string path;
	std::size_t nPoslast = g_upload_url.find_last_of("/");
	std::size_t nHttp = g_upload_url.find_first_of("//");

	std::size_t nPosFist = g_upload_url.find_first_of("//");

	std::cout << nPoslast<<"-"<<nHttp <<"-"<< nPosFist << endl;

	///host.append(g_upload_url.c_str() + nHttp + 2, nPoslast - nHttp - 2);
	path.append(g_upload_url.c_str() + nPoslast);
	host.append(g_upload_url.c_str() + nHttp + 2, nPosFist- nHttp);
	//path.append(g_upload_url.c_str() + nHttp);


	clParseURL URL = clParseURL::ParseURL(g_upload_url);
	if (URL.IsValid())
	{
		host = URL.m_Host;
		path = "/"+URL.m_Path;
	}

	std::cout << host << endl;

	std::cout << path << endl;

	//return 0;
	
	long nFileSize = 0;
	char * pFileBuf = ReadFileBuf(g_strfile.c_str(), &nFileSize);
	g_filesize = nFileSize;

	int nCount = g_filesize / 65536;
	int nLeave = g_filesize % 65536;

	std::cout << g_filesize << endl;

	//std::cout << nCount << endl;

	//std::cout << nLeave << endl;

	

	for (int i = 0; i< nCount + 1; i++)
	{

		/////////////////////////////////////////////////////////////////
		std::string strBoundary = GetRamdomBoundary();

		std::string strConBoundary = "--" + strBoundary;

		std::stringstream  contentStram;

		contentStram << strConBoundary << "\r\n";
		contentStram << "Content-Disposition: form-data; name=\"file_size\"\r\n\r\n";
		contentStram << g_filesize << "\r\n";

		contentStram << strConBoundary << "\r\n";
		contentStram << "Content-Disposition: form-data; name=\"range\"\r\n\r\n";

		int nStart = 0;
		nStart = i * 65536;
		int nLast = 0;
		int nSendSize = 0;
		if (i != nCount)
		{
			nLast = nStart + 65535;
			nSendSize = 65536;
		}
		else
		{
			if (nLeave == 0)
				break;
			nLast = nStart + nLeave - 1;
			nSendSize = nLeave;
		}
		contentStram << nStart << "-" << nLast << "\r\n";

		contentStram << strConBoundary << "\r\n";
		contentStram << "Content-Disposition: form-data; name=\"file\"; filename=\"clipboard.png\"\r\n";
		//contentStram << "Content-Type: application/octet-stream\r\n\r\n";
		contentStram << "Content-Type:  image/png\r\n\r\n";

		std::string strContentData = contentStram.str();
		// 添加文件数据
		// ....
		strContentData.append(pFileBuf + nStart, nSendSize);
		strContentData.append("\r\n");
		strContentData.append(strConBoundary);
		strContentData.append("--\r\n\r\n");
		int nContentLen = strContentData.size();


		//////////////////////////////////////////////////////////////
		// C++ 模拟POST，multipart/form-data格式上传
		std::stringstream streamPost;
		streamPost << "POST "<< path << " HTTP/1.1\r\n";
		streamPost << "Accept: */*\r\n";
		streamPost << "Content-Type: multipart/form-data; boundary=" << strBoundary << "\r\n";
		//streamPost << "User-Agent: Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; WOW64; Trident/6.0)\r\n";
		streamPost << "Host: " << host << "\r\n";
		//streamPost << "DNT: 1\r\n";
		streamPost << "Connection: keep-alive\r\n";
		//streamPost << "Referer: http://www.iqiyi.com/upload?type=0&ufrm=web_iqiyi_uc\r\n";
		//streamPost << "Cache-Control: no-cache\r\n";
		streamPost << "Content-Length: " << nContentLen << "\r\n\r\n";

		std::string PostContent = streamPost.str();
		int nHeadSize = PostContent.size();
		PostContent.append(strContentData.c_str(), nContentLen);
		int nPostSize = PostContent.size();


		//std::cout << PostContent << endl;
		//std::cout << nPostSize << endl;

		//return 0;

		if (http.socketHttp(host, PostContent, atoi(URL.m_Port.c_str())) == false)
		{

			std::cout << "发包失败！" << endl;
			break;
		}
		else
		{
			std::string strGetResult = http.GetLastRequestResult();

			std::wstring wStr;
			Transcode::UTF8_to_Unicode(strGetResult.c_str(), strGetResult.size(), wStr);
			const wchar_t* pData = wStr.c_str();

			cout << StringToUtf(strGetResult )<< endl;

			YLog log(YLog::INFO, "log.txt", YLog::ADD);
			log.W(__FILE__, __LINE__, YLog::INFO, "dmp_path", StringToUtf(strGetResult));
			//cout << pData << endl;
			//OutputDebugString(pData);
		}
		// 避免上传过快
		//sleep(10);
	}
	if (pFileBuf)
	{
		free(pFileBuf);
		pFileBuf = NULL;
	}


	//testMyThread();

	Sleep(3000);


	//curl_global_init(CURL_GLOBAL_ALL);

    return 0;
}

