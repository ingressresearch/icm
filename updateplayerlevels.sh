#!/bin/bash
echo Starting player levels >> ~/icm/ingressgloballog.log
cd icm
/usr/bin/php updateplayerlevels.php 2>&1 > /dev/null
echo Ending player levels >> ~/icm/ingressgloballog.log

