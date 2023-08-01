#ifndef PROBE_H
#define PROBE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <esp_err.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <freertos/portmacro.h>

#include "common.h"


#define DEFAULT_PROBE_COUNT 128

typedef enum {
	ATTACK_PROBE_NONE,
	ATTACK_PROBE_UNDIRECTED,
	ATTACK_PROBE_DIRECTED_USER,
	ATTACK_PROBE_DIRECTED_SCAN
} probe_attack_t;

static const char* PROBE_TAG = "probe@GRAVITY";

extern char **user_ssids;
extern int user_ssid_count;
extern bool *attack_status;
extern bool *hop_defaults;

extern uint8_t probe_raw[];
extern uint8_t probe_response_raw[];

extern int PROBE_REQUEST_LEN;
extern int PROBE_SSID_OFFSET;
extern int PROBE_SRCADDR_OFFSET;
extern int PROBE_DESTADDR_OFFSET;
extern int PROBE_BSSID_OFFSET;
extern int PROBE_SEQNUM_OFFSET;
extern int PROBE_RESPONSE_LEN;
extern int PROBE_RESPONSE_DEST_ADDR_OFFSET;
extern int PROBE_RESPONSE_SRC_ADDR_OFFSET;
extern int PROBE_RESPONSE_BSSID_OFFSET;
extern int PROBE_RESPONSE_PRIVACY_OFFSET;
extern int PROBE_RESPONSE_SSID_OFFSET;
extern int PROBE_RESPONSE_GROUP_CIPHER_OFFSET;
extern int PROBE_RESPONSE_PAIRWISE_CIPHER_OFFSET;
extern int PROBE_RESPONSE_AUTH_TYPE_OFFSET;

/*
 * This is the (currently unofficial) 802.11 raw frame TX API,
 * defined in esp32-wifi-lib's libnet80211.a/ieee80211_output.o
 *
 * This declaration is all you need for using esp_wifi_80211_tx in your own application.
 */
esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);

int probe_stop();
int probe_start(probe_attack_t type);

/* extern functions - defined in gravity.c */
int addSsid(char *ssid);
int rmSsid(char *ssid);
int countSsid();
char **lsSsid();
char *generate_random_ssid();

#endif