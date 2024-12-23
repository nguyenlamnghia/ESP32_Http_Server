#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"

// #define WIFI_SSID "Nguyen Lam Nghia"
// #define WIFI_PASS "123456781"

#define WIFI_SSID "NGHIA"
#define WIFI_PASS "123123123"

// #define WIFI_SSID "Hust_D3,D5"
// #define WIFI_PASS ""

static const char *TAG = "HTTP_SERVER";

/* Wi-Fi event handler */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Retrying to connect to the AP...");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG, "Got IP: %s", ip4addr_ntoa(&event->ip_info.ip));
    }
}

/* HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req) {
    char response[] = "Hello, this is ESP32!";
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

/* HTTP POST handler */
static esp_err_t message_post_handler(httpd_req_t *req) {
    char content[100];
    int content_len = httpd_req_recv(req, content, sizeof(content));
    if (content_len <= 0) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    content[content_len] = '\0'; // Ensure null-termination
    ESP_LOGI(TAG, "Received message: %s", content);

    char response[] = "Message received!";
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

/* URI handler registration */
static httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t hello_uri = {
            .uri = "/hello",
            .method = HTTP_GET,
            .handler = hello_get_handler,
            .user_ctx = NULL
        };

        httpd_uri_t message_uri = {
            .uri = "/message",
            .method = HTTP_POST,
            .handler = message_post_handler,
            .user_ctx = NULL
        };

        httpd_register_uri_handler(server, &hello_uri);
        httpd_register_uri_handler(server, &message_uri);

        return server;
    }

    ESP_LOGI(TAG, "Failed to start web server!");
    return NULL;
}

/* Wi-Fi initialization */
void wifi_init(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
}

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi and start web server
    wifi_init();
    start_webserver();
}
