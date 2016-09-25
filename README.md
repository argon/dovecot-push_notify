# push_notify for dovecot

Apple Mail (iOS) Push support.

Dovecot plugins which provide commands for iOS Mail.app to register for mail account push notifications

Works best with [push_notify][push_notify]

## Requirements

- Dovecot IMAP server
- MTA (e.g. postfix) configured to use dovecot LMTP for mail delivery
- APS Topic for Push certificates (i.e. `com.apple.mail.XServer.7462c88a-4849-cca3-fb17-7abcb77a8d76`)
- [push_notify][push_notify] running locally with `/var/dovecot/push_notify` socket accessible by dovecot user

## Installation

The included `Makefile` can build and install the plugins to `/usr/lib64/dovecot/`. This has been tested on Fedora. You may need to alter the paths for your machine.

```
$ make clean build install
```

## Configuration

Example (minimum) configuration is shown below.

**/etc/dovecot/conf.d/90-plugin.conf**

```
plugin {
  #setting_name = value
  push_notify_aps_topic = com.apple.mail.XServer.7462c88a-4849-cca3-fb17-7abcb77a8d76
}
```

**/etc/dovecot/conf.d/20-imap.conf**
```
protocol imap {
	[...]

	mail_plugins = $mail_plugins push_notify_registration notify push_notify
}
```

**/etc/dovecot/conf.d/**
```
protocol lmtp {
	[...]

	mail_plugins = $mail_plugins notify push_notify
}
```

### License

This work was derived from original work Open Sourced by Apple Inc.

[imap/cmd-x-apple-push-service.c](http://opensource.apple.com/source/dovecot/dovecot-293/dovecot/src/imap/cmd-x-apple-push-service.c)
[plugins/push-notify](http://opensource.apple.com/source/dovecot/dovecot-293/dovecot/src/plugins/push-notify/)

**Original code:**

> Copyright (c) 2010-2013 Apple Inc. All rights reserved.
> 
> Redistribution and use in source and binary forms, with or without
> modification, are permitted provided that the following conditions
> are met:
> 
> 1.  Redistributions of source code must retain the above copyright
> notice, this list of conditions and the following disclaimer.
> 2.  Redistributions in binary form must reproduce the above
> copyright notice, this list of conditions and the following
> disclaimer in the documentation and/or other materials provided
> with the distribution.
> 3.  Neither the name of Apple Inc. ("Apple") nor the names of its
> contributors may be used to endorse or promote products derived
> from this software without specific prior written permission.
> 
> THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND
> ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
> THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
> PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS
> CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
> SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
> LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
> USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
> ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
> OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
> OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
> SUCH DAMAGE.

**Subsequent modifications:**

> Copyright (c) 2012 - 2016 Andrew Naylor. All rights reserved.

[push_notify]: https://github.com/argon/push_notify