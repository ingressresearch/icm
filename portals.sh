#!/bin/bash
echo Starting portals >> ~/icm/ingressgloballog.log
cd icm
/usr/bin/php map.php 2>&1 > portals.log
./updateplayerlevels.sh
./syncportals.sh
echo Ending portals >> ~/icm/ingressgloballog.log

