#! /bin/bash

iqn="iqn.2020-12.com.toshiba:tris-server"
target_ip=192.168.100.52
ip_port=$target_ip":3260"

    
sudo apt-get install open-iscsi
sudo /etc/init.d/open-iscsi start
sudo iscsiadm -m discovery -t st -p $target_ip
sudo iscsiadm -m node --targetname $iqn --portal $ip_port --op=update --name node.session.auth.authmethod --value=CHAP
sudo iscsiadm -m node --targetname $iqn --portal $ip_port --login