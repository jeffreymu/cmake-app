//
// Created by Zhou,Baochuan on 18/6/5.
//

#include "http.h"

using namespace work;

Http::Http()
{
    curl_global_init(CURL_GLOBAL_NOTHING);
}

Http::~Http()
{
    curl_global_cleanup();
}

size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream) {
    //cout << "----->reply" << endl;
    std::string* str = (std::string*)stream;
    //cout << *str << endl;
    (*str).append((char*)ptr, size * nmemb);
    return size * nmemb;
}
string Http::get(string url, unsigned int retries)
{
    string response;
    CURL *curl;
    struct curl_slist *headers = NULL;
    //headers = curl_slist_append(headers, "Content-Type: text/plain; charset=utf-8");

    curl = curl_easy_init() ;
    if (curl) {
        //curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &response);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0); // 传输超时
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2); // 连接超时
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
        CURLcode res = curl_easy_perform(curl);   // 执行

        // 重试
        while (res != CURLE_OK && --retries > 0) {
            res = curl_easy_perform(curl);   // 执行
        }

        curl_easy_cleanup(curl);
    }
    curl_slist_free_all(headers);
    return response;
}
