#include <lwip/apps/http_client.h>
#include <lwip/arch.h>
#include <lwip/pbuf.h>
#include <pico/time.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/altcp.h"
#include "lwip/altcp_tls.h"

#define HOST "www.alphavantage.co"
#define URL_REQUEST "/query?function=GLOBAL_QUOTE&symbol=CRWV&apikey="

volatile bool req_done = false;

float parse_stock_price(struct pbuf *p) {
    /*
     Response shape:
        {
            "Global Quote": {
                "01. symbol": "CRWV",
                "02. open": "48.1000",
                "03. high": "51.0200",
                "04. low": "47.0575",
                "05. price": "48.7300",
                "06. volume": "90252210",
                "07. latest trading day": "2026-11-10",
                "08. previous close": "48.9800",
                "09. change": "-0.2500",
                "10. change percent": "-0.5104%"
            }
        }
    */

    char fbuf[32];
    u16_t idx = pbuf_strstr(p, "\"05. price\":");
    if (idx == 0xFFFF) {
        // Not found.
        return 0;
    }
    // Move to the start of the price float.
    idx += 14;
    u16_t offset = 0;
    char c;
    while ((c = (char)pbuf_get_at(p, idx)) != '"' && offset < 32) {
        fbuf[offset] = c;
        idx++;
        offset++;
    }

    char *end;
    // TODO: Better error handling?
    return strtof(fbuf, &end);
}

err_t req_recv_fn(void *arg, struct altcp_pcb *conn, struct pbuf *p, err_t err) {
    float price = parse_stock_price(p);
    printf("CRWV price: $%.2f\n", price);
    altcp_recved(conn, p->tot_len);
    pbuf_free(p);
    return 0;
}

void req_result_fn(void *arg, httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err) {
    printf("result %d len %u server_response %u err %d\n", httpc_result, rx_content_len, srv_res, err);
    req_done = true;
}

// Wrap altcp_tls_alloc to set ssl hostname (sni).
// Most web servers are behind load balancers / proxies / etc.
// SNI is needed so the correct cert is presented by the receiver.
static struct altcp_pcb *altcp_tls_alloc_sni(void *arg, u8_t ip_type) {
    struct altcp_tls_config *conf = (struct altcp_tls_config*)arg;
    struct altcp_pcb *pcb = altcp_tls_alloc(conf, ip_type);
    if (!pcb) {
        printf("Failed to allocate PCB\n");
        return NULL;
    }
    mbedtls_ssl_set_hostname(altcp_tls_context(pcb), HOST);
    return pcb;
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_USA)) {
        return -1;
    }

    // Station mode, i.e. allowing connecting to APs.
    // Must be enabled before trying to connect.
    cyw43_arch_enable_sta_mode();

    err_t err;
    if ((err = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))) {
        while(true) {
            printf("Failed to connect: %d\n", err);
            sleep_ms(1000);
        }
    }
    printf("Connection successful, will make request...\n");
    sleep_ms(5000);

    httpc_connection_t req_settings = {0};
    struct altcp_tls_config *tls_config = altcp_tls_create_config_client(NULL, 0);

    struct altcp_allocator_s tls_alloc = {
        .alloc = altcp_tls_alloc_sni,
        .arg = tls_config,
    };
    req_settings.altcp_allocator = &tls_alloc;
    req_settings.result_fn = req_result_fn;
    req_settings.headers_done_fn = NULL;

    if ((err = httpc_get_file_dns(HOST, 443, URL_REQUEST ALPHA_VANTAGE_API_KEY, &req_settings, req_recv_fn, NULL, NULL)) != ERR_OK) {
        while (true) {
            printf("HTTP request failed: %d\n", err);
            sleep_ms(1000);
        }
    }
    while (!req_done);

    altcp_tls_free_config(tls_config);

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }
}
