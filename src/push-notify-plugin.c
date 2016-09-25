/*
 * Copyright (c) 2010-2011 Apple Inc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without  
 * modification, are permitted provided that the following conditions  
 * are met:
 * 
 * 1.  Redistributions of source code must retain the above copyright  
 * notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above  
 * copyright notice, this list of conditions and the following  
 * disclaimer in the documentation and/or other materials provided  
 * with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of its  
 * contributors may be used to endorse or promote products derived  
 * from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,  
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A  
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS  
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT  
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,  
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF  
 * SUCH DAMAGE.
 */

#include "imap-common.h"

#include "str.h"
#include "lib.h"

#include "mail-user.h"
#include "mail-storage.h"
#include "notify-plugin.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#include "push-notify-plugin.h"

// -----------------------------------------------------------------
//	push_notification ()

static void
push_notification(struct mail *mail)
{
	int	                debug           = 0;
	int	                notify_sock     = 0;
	const char         *sock_path       = PUSH_NOTIFY_PATH;
	ssize_t             rc              = 0;
	socklen_t           sock_len        = 0;
	struct sockaddr_un  sock_addr;
	struct msg_data_s   msg_data;

	const char *mailbox = mailbox_get_vname(mail->box);
	struct mail_storage *storage = mailbox_get_storage(mail->box);
	struct mail_user *user = mail_storage_get_user(storage);

	if (user->mail_debug) {
		debug = 1;
		i_info( "push-notify: push notification enabled" );
	}

	notify_sock = socket( AF_UNIX, SOCK_STREAM, 0 );
	if ( notify_sock < 0 ) {
		/* warn that connect failed but do not fail the plugin or message will not get delivered */
		i_warning( "push-notify: open socket: \"%s\" failed", sock_path );
		return;
	}

	sock_addr.sun_family = AF_UNIX;
	i_strocpy( sock_addr.sun_path, sock_path, sizeof(sock_addr.sun_path) );
	sock_len = sizeof(sock_addr.sun_family) + strlen(sock_addr.sun_path) + 1;
	rc = connect(notify_sock, (struct sockaddr *) &sock_addr,  sock_len);
	if ( rc < 0 ) {
		/* warn that connect failed but do not fail the plugin or message will not get delivered */
		i_warning( "push-notify: connect() to socket: \"%s\" failed: %m", sock_path );
		return;
	}

	memset( &msg_data, 0, sizeof( msg_data ) );
	msg_data.msg = 3;

	/* set user/account id */
	if ( user->username != NULL ) {
		i_strocpy( msg_data.d1, user->username, sizeof(msg_data.d1) );
		if (debug)
			i_info( "push-notify: notify: %s", msg_data.d1 );
	}

	if ( mailbox != NULL ) {
		i_strocpy( msg_data.d2, mailbox, sizeof(msg_data.d2) );
		if (debug)
			i_info( "push-notify: mailbox: %s", msg_data.d2);
	}

	rc = send(notify_sock, (void *)&msg_data, sizeof(struct msg_data_s), 0);
	if ( rc < 0 )
		i_warning( "push-notify: send() to socket: \"%s\" failed: %m", sock_path );

	if (debug)
		i_info("push-notify: data sent: %lu", rc);

	close(notify_sock);
} // push_notification

static void push_notify_mail_save(void *txn, struct mail *mail)
{
	push_notification(mail);
}

static void push_notify_mail_copy(void *txn, struct mail *src, struct mail *dst)
{
	push_notification(dst);
}

static const struct notify_vfuncs push_notify_vfuncs = {
	.mail_save = push_notify_mail_save,
	.mail_copy = push_notify_mail_copy
};

static struct notify_context *push_notify_ctx;

void push_notify_plugin_init (struct module *module ATTR_UNUSED)
{
	// Initialise storage layer
	push_notify_ctx = notify_register(&push_notify_vfuncs);
}

void push_notify_plugin_deinit (void)
{
	// Uninitialise storage layer
	notify_unregister(push_notify_ctx);
}

const char *push_notify_plugin_dependencies[] = { "notify", NULL };
