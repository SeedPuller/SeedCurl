#ifndef MY_CURL_GUARD
#define MY_CURL_GUARD 1

#include <cstdlib>
#include <curl/curl.h>
#include <string>
#include <fstream>
#include <map>

using std::string;
using std::map;

#define SUCCESSS 200

class myCurl{
    public:
        myCurl(string u);
        ~myCurl();
        int code();
        string response(int perf = 0);
        string text();
        void setPostParam(map<string, string>& param);
        void setGetParam(map<string, string>& param);
        string getUrl() const;
        void setFileNme(const string fname);
        string getFileName() const;
        void perform();
    private:
        static int instance_counter;
        string url, filename;
        CURLcode res;
        CURL *curl_handle;
        FILE *file;
        static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
};

#endif