#!/bin/bash
echo Starting COMM >> ~/icm/ingressgloballog.log
cd icm
/usr/bin/php ingress.php 2>&1 > ./loaddata.log
echo Ending COMM >> ~/icm/ingressgloballog.log

