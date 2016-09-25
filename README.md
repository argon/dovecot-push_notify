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

[push_notify]: https://github.com/argon/push_notify