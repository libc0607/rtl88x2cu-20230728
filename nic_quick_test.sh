#!/bin/sh

NIC=$1
CHANNEL=$2

echo "insmod"
rmmod 88x2cu
rmmod rtw88_8822cu
rmmod rtw88_8822c
rmmod rtw88_usb
rmmod rtw88_core
modprobe cfg80211
insmod 88x2cu.ko rtw_regd_src=1 rtw_tx_pwr_by_rate=0 rtw_tx_pwr_lmt_enable=0

echo "set $NIC monitor mode"
airmon-ng check kill
airmon-ng start $NIC

echo "set channel to $CHANNEL"
ifconfig $NIC down
iwconfig $NIC mode monitor channel $CHANNEL
ifconfig $NIC up
