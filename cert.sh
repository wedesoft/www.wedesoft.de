#!/bin/sh
sudo certbot certonly --manual --preferred-challenges http
# cert.pem and privkey.pem at /etc/letsencrypt/live/www.wedesoft.de
