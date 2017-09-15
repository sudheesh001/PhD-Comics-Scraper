#include <curl/curl.h>

#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>

#include <algorithm>
#include "helper.h"

// TODO: Replace with cpp_redis? or any other inmemory DBs
std::vector <std::string> links;

// callback function writes data to a std::ostream
static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp) {
    if (userp) {
        std::ostream& os = *static_cast<std::ostream*>(userp);
        std::streamsize len = size * nmemb;
        if (os.write(static_cast<char*>(buf), len))
            return len;
    }

    return 0;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


CURLcode curl_read(const std::string& url, std::ostream& os, long timeout = 30) {
    CURLcode code(CURLE_FAILED_INIT);
    CURL* curl = curl_easy_init();

    if (curl) {
        if (CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
            && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
            && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
            && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
            && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
            && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str()))) {
            code = curl_easy_perform(curl);
        }
        curl_easy_cleanup(curl);
    }
    return code;
}

// Regex to look for image links within the website and fetch only the image links.
// A page might have multiple locations where these links are mentioned and matched by the regex/
// Push these to a global variable (erck!!) and remove duplicates.
void find_image_links(std::string s) {
    std::regex r("http://www.phdcomics.com/comics/archive/phd[0-9]+.gif");

    for(std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r);
                            i != std::sregex_iterator();
                            ++i ) {
        std::smatch m = *i;
        links.push_back(m.str());
    }
}

// This is a generic function that allows you to fetch the HTML content given a URL.
std::string curl_html_parse(const std::string &url) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::cout<<url<<std::endl;

    curl = curl_easy_init();
    if(curl) {
        // CURL mandates const std::string, therefore it's required to do c_str() for a variable std::string
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str(  ));
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}


int main(int argc, char** argv) {

    curl_global_init(CURL_GLOBAL_ALL);

    // TODO: Take range of IDs to query from argv. For now, lets just query this.
    for (int i=1; i<1000; i++) {
        std::string url = "http://phdcomics.com/comics/archive.php?comicid=" + std::to_string(i);
        std::cout<<"Parsing: " + url<<std::endl;
        std::string s = curl_html_parse(url);
        find_image_links(s);
        s = "";
    }

    remove_duplicates(links);


    std::cout<<"Links Obtained are"<<std::endl;
    for (int i=0; i < links.size(); i++) {
        std::vector<std::string> x = split(links[i], '/');
        std::string filename = x[x.size()-1];
        std::cout<<filename<<"   "<<links[i]<<std::endl;
        std::ofstream ofs(filename, std::ostream::binary);

        if (CURLE_OK == curl_read(links[i], ofs)) {
            // Image successfully written to file
        }
    }

    curl_global_cleanup();
}
