#ifndef __PUSH_NOTIFY_COMMON_PLUGIN__H
#define __PUSH_NOTIFY_COMMON_PLUGIN__H

#include <stdint.h>

#define APS_VERSION "2"

#define PUSH_NOTIFY_PATH "/var/dovecot/push_notify"

struct module;

typedef struct msg_data_s {
	uint32_t msg;
	uint32_t pid;

	char d1[128];
	char d2[512];
	char d3[512];
	char d4[512];
} msg_data_t;

#endif