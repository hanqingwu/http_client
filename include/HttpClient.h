#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include <iostream>
#include "include/curl/curl.h"

#define Z_DEBUG

using namespace std;


#ifdef _WIN32
typedef _off_t p_off_t;
#else
typedef off_t p_off_t;
#endif

typedef void(*progress_info_callback)(void *userdata, double downloadSpeed, double remainingTime, double progressPercentage);

typedef void(*event_callback)(int ret);



typedef struct
{
    void *sender;
	CURL *handle;
	progress_info_callback cb;
}Progress_User_Data;

typedef enum
{
	HTTP_REQUEST_OK = CURLE_OK,

	HTTP_REQUEST_ERROR = -999,

}Http_Client_Response;

class HttpClient
{
public:
	
	~HttpClient();

	static HttpClient *getInstance();
	static void destroyInstance();

    int HttpGet(const string requestURL, const string saveTo, void *sender, progress_info_callback cb, long timeout_s);
    
    int HttpGetNoBlock(const string requestURL, const string saveTo, void *sender, event_callback cb, int file_size);

    int HttpGetExit();


private:
	HttpClient();

	bool init();
	
	static size_t write_callback(char *buffer, size_t size, size_t nmemb, void *userdata);
    static int progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

	// Get the local file size
	p_off_t getLocalFileLength(string path);

	// Get the file size on the server
	double getDownloadFileLength(string url);

private:
	static HttpClient *instance;

	static double downloadFileLength;
	static curl_off_t resumeByte;

	// Call frequency of the callback function
	static time_t lastTime;

    static bool stopCurl;

    int mFileSize;
};

#endif		// __HTTPCLIENT_H__
