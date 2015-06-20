#ifndef __PUSH_NOTIFY_REGISTRATION_PLUGIN__H
#define __PUSH_NOTIFY_REGISTRATION_PLUGIN__H

#include "push-notify-common-plugin.h"

#include "imap-common.h"
#include "imap-id.h"

struct message_info {
	const char	*from;
	const char	*subj;
};

#ifdef DOVECOT_ABI_VERSION
const char *push_notify_registration_plugin_version = DOVECOT_ABI_VERSION;
#else
const char *push_notify_registration_plugin_version = DOVECOT_VERSION;
#endif

void push_notify_registration_plugin_init(struct module *module);
void push_notify_registration_plugin_deinit(void);

#endif