#include <curl/curl.h>
#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    auto* buffer = static_cast<std::string*>(userp);
    buffer->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

int main() {
    const char* api_key_env = std::getenv("GOOGLE_MAPS_API_KEY");
    if (!api_key_env || std::string(api_key_env).empty()) {
        std::cerr << "Error: GOOGLE_MAPS_API_KEY environment variable is not set.\n";
        return 1;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error: Failed to initialize libcurl.\n";
        return 1;
    }

    std::string address = "6120 Alvarado Pl, El Cerrito, CA";
    char* escaped_addr = curl_easy_escape(curl, address.c_str(), static_cast<int>(address.length()));
    char* escaped_key  = curl_easy_escape(curl, api_key_env, 0);

    std::string url = "https://maps.googleapis.com/maps/api/geocode/json?address=" +
                      std::string(escaped_addr ? escaped_addr : "") +
                      "&key=" + std::string(escaped_key ? escaped_key : "");

    curl_free(escaped_addr);
    curl_free(escaped_key);

    std::string response_body;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Request failed: " << curl_easy_strerror(res) << "\n";
        return 1;
    }

    try {
        json data = json::parse(response_body);
        std::string status = data.value("status", "UNKNOWN");
        if (status != "OK") {
            std::cerr << "API returned status: " << status << "\n";
            if (data.contains("error_message")) {
                std::cerr << "Details: " << data["error_message"] << "\n";
            }
            return 1;
        }

        const auto& loc = data["results"][0]["geometry"]["location"];
        double lat = loc["lat"].get<double>();
        double lng = loc["lng"].get<double>();

        std::cout << "Coordinates: " << lat << ", " << lng << "\n";
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}