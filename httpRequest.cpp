#include "stdafx.h"
#include "httpRequest.h"

HTTPRequest::HTTPRequest(const std::string& url)
    : url(url)
{
    curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL");
    }
}

HTTPRequest::~HTTPRequest()
{
    curl_easy_cleanup(curl);
}

void HTTPRequest::post(const std::string& payload)
{
    std::map<std::string, std::string> headers = { {"Content-Type", "application/json"} };
    struct curl_slist* curlHeaders = NULL;
    for (const auto& header : headers) {
        curlHeaders = curl_slist_append(curlHeaders, (header.first + ": " + header.second).c_str());
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlHeaders);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        throw std::runtime_error("cURL request failed");
    }
}