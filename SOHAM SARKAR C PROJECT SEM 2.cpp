#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <json-c/json.h>

#define API_KEY "9b698d670eb0c396ebd0adc483febc66"
#define API_URL "http://api.weatherstack.com/current"https://api.openweathermap.org/data/2.5/weather?"q=%s&appid=%s&units=metric"

size_t write_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    return size * nitems;
}

int main() {
    CURL *curl;
    CURLcode res;
    char city[50];
    char url[150];
    struct json_object *root, *main, *weather, *description, *temp;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        printf("Error initializing curl.\n");
        return 1;
    }

    printf("Enter city name: ");
    scanf("%s", city);
    sprintf(url, API_URL, city, API_KEY);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        printf("Error: %s\n", curl_easy_strerror(res));
        return 1;
    }

    root = json_tokener_parse(curl_easy_strerror(res));
    if (!root) {
        printf("Error: Invalid JSON.\n");
        return 1;
    }

    main = json_object_object_get(root, "main");
    temp = json_object_object_get(main, "temp");
    weather = json_object_object_get(root, "weather");
    description = json_object_object_get(json_object_array_get_idx(weather, 0), "description");

    printf("Temperature in %s: %.2f°C\n", city, json_object_get_double(temp));
    printf("Weather condition: %s\n", json_object_get_string(description));

    json_object_put(root);
    curl_easy_cleanup(curl);

    return 0;
}
