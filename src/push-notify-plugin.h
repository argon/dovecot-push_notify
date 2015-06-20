#ifndef __PUSH_NOTIFY_LDA_PLUGIN__H
#define __PUSH_NOTIFY_LDA_PLUGIN__H


#include "push-notify-common-plugin.h"

extern const char *push_notify_plugin_dependencies[];

#ifdef DOVECOT_ABI_VERSION
const char *push_notify_plugin_version = DOVECOT_ABI_VERSION;
#else
const char *push_notify_plugin_version = DOVECOT_VERSION;
#endif

void push_notify_plugin_init(struct module *module);
void push_notify_plugin_deinit(void);

#endif