# Makefile for push_notify

#### configuration begin ####

# Dovecot's header directory
DOVECOT_INC_PATH = /usr/include/dovecot
# Dovecot's plugin path
DOVECOT_PLUGIN_PATH = /usr/lib64/dovecot
# Dovecot's config path
DOVECOT_CONFIG_PATH = /etc/dovecot/conf.d 
# D

## usually no need to configure anything below this line ##

# plugin source & target name #
NOTIFY_PLUGIN_NAME = lib90_push_notify_plugin.so
REGISTRATION_PLUGIN_NAME = lib90_push_notify_registration_plugin.so

#### configuration end ####

SRCDIR = src
NOTIFY_SOURCE := $(SRCDIR)/push-notify-plugin.c
REGISTRATION_SOURCE := $(SRCDIR)/push-notify-registration-plugin.c

.PHONY: all build install configure clean

all: build

build: notify registration

notify: ${NOTIFY_PLUGIN_NAME}

registration: ${REGISTRATION_PLUGIN_NAME}

${NOTIFY_PLUGIN_NAME}: ${NOTIFY_SOURCE}
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) \
	      -fPIC -shared -Wall \
	      -I${DOVECOT_INC_PATH} \
	      -I${DOVECOT_INC_PATH}/src \
	      -I${DOVECOT_INC_PATH}/src/lib \
	      -I${DOVECOT_INC_PATH}/src/lib-storage \
	      -I${DOVECOT_INC_PATH}/src/lib-mail \
	      -I${DOVECOT_INC_PATH}/src/lib-imap \
	      -I${DOVECOT_INC_PATH}/src/lib-index \
	      -DHAVE_CONFIG_H \
	      $< -o $@

${REGISTRATION_PLUGIN_NAME}: ${REGISTRATION_SOURCE}
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) \
	      -fPIC -shared -Wall \
	      -I${DOVECOT_INC_PATH} \
	      -I${DOVECOT_INC_PATH}/src \
	      -I${DOVECOT_INC_PATH}/src/lib \
	      -I${DOVECOT_INC_PATH}/src/lib-storage \
	      -I${DOVECOT_INC_PATH}/src/lib-mail \
	      -I${DOVECOT_INC_PATH}/src/lib-imap \
	      -I${DOVECOT_INC_PATH}/src/lib-index \
	      -DHAVE_CONFIG_H \
	      $< -o $@

install: install_notify_plugin install_registration_plugin

install_notify_plugin: ${NOTIFY_PLUGIN_NAME}
	install -d ${DESTDIR}${DOVECOT_PLUGIN_PATH}
	install -m 0755 $< ${DESTDIR}${DOVECOT_PLUGIN_PATH}

install_registration_plugin: ${REGISTRATION_PLUGIN_NAME}
	install -d ${DESTDIR}${DOVECOT_PLUGIN_PATH}
	install -m 0755 $< ${DESTDIR}${DOVECOT_PLUGIN_PATH}

clean:
	$(RM) ${NOTIFY_PLUGIN_NAME} ${REGISTRATION_PLUGIN_NAME}

# EOF