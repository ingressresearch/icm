#!/bin/bash
echo Starting player names >> ~/icm/ingressgloballog.log
cd icm
/usr/bin/php updateplayernames.php 2>&1 > /dev/null
echo Ending player names >> ~/icm/ingressgloballog.log

