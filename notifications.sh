#!/bin/bash
echo Starting notifications >> ~/icm/ingressgloballog.log
cd icm
/usr/bin/php notification.php 2>&1 > ./notifications.log
echo Ending notifications >> ~/icm/ingressgloballog.log

