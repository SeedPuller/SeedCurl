#include "seedcurl.h"
#include <iostream>

int myCurl::instance_counter = 0;

myCurl::myCurl(string u){
    url = u;
    filename = "/tmp/curltemp.txt";
    if (!instance_counter){
        curl_global_init(CURL_GLOBAL_ALL);
    }
    curl_handle = curl_easy_init();
    //curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    ++instance_counter;
}

myCurl::~myCurl(){
    curl_easy_cleanup(curl_handle);
    curl_easy_reset(curl_handle);
    --instance_counter;
    if (!instance_counter){
        curl_global_cleanup();        
    }
}

void myCurl::perform(){
    if(curl_handle && file){
        file = fopen(filename.c_str(), "wb");
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, file);
        res = curl_easy_perform(curl_handle);
    }
    fclose(file);
}

int myCurl::code(){
    perform();
    int resp = static_cast<int>(res);
    return resp;
}

string myCurl::response(int perf){
    if (!perf){
        return curl_easy_strerror(res);
    }
    perform();
    return curl_easy_strerror(res);
}

string myCurl::text(){
    perform();
    int resp = static_cast<int>(res);
    std::ifstream file4read(filename);
    string line, read;
    while (file4read >> line){
        read += line;
    }
    return read;
}

size_t myCurl::write_data(void *ptr, size_t size, size_t nmemb, void *stream){
  size_t written = fwrite(ptr, size, nmemb, static_cast<FILE *>(stream));
  return written;
}

void myCurl::setPostParam(map<string, string>& param){
    string params;
    for (auto& elem : param){
        params += elem.first + "=" + elem.second + "&";
    }
    params.pop_back();
    curl_easy_setopt(curl_handle, CURLOPT_COPYPOSTFIELDS, params.c_str());
}


void myCurl::setGetParam(map<string, string>& param){
    string params;
    for (auto& elem : param){
        params += elem.first + "=" + elem.second + "&";
    }
    params.pop_back();
    url += "?" + params;
}

string myCurl::getUrl() const{
    return url;
}

void myCurl::setFileNme(const string fname){
    filename = fname;
}

string myCurl::getFileName() const{
    return filename;
}
