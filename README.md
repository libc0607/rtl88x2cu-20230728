# rtl88x2cu-20230728
(WIP) RTL8812CU/RTL8822CU Linux Driver v5.15.8-52-20230728 FPV Mod  

Download pre-built OpenIPC firmware with this driver integrated [here](https://github.com/libc0607/openipc-firmware/releases/tag/latest) to help test.  

Original driver from Realtek: [RTL8822CU_WiFi_linux_v5.15.8-52-g35e62e7c9_COEX20221215-3130.20230728.tar.gz](https://github.com/libc0607/rtl88x2cu-20230728/blob/main/doc/RTL8822CU_WiFi_linux_v5.15.8-52-g35e62e7c9_COEX20221215-3130.20230728.tar.gz)  
BL-M8812CU3 datasheet: [BL-M8812CU3_datasheet_V1.0.1.1.pdf](https://github.com/libc0607/rtl88x2cu-20230728/blob/main/doc/BL-M8812CU3_datasheet_V1.0.1.1.pdf) -- A good replacement of BL-M8812AF1  

The RTL8812CU has fully integrated RF frontends (PA, LNA, and SPDT switch), so you will rarely see any RTL8812CU adaptor with PA/LNA (adding the PA means an additional SPDT switch).  
The internal PAs have really good performance (HT20, MCS7, ~16dBm TX, -28dB EVM), but it's still limited by the 3.3V power and poor heat dissipation.  
LB-LINK's support said the chip's output (using only internal PA) can reach 23 ~ 24dBm while injecting at 6Mbps, based on their measurement. So, if you need something between the big square RTL8812EU and the smaller square RTL8731BU...  


Tested:  
 - Build on kernel 6.8  
 - TX power unlocked (same as the 8812eu driver, ```iw wlan0 set txpower fixed <0~3150>```)  
 - RX in monitor mode  
 - TX injection in monitor mode, HT/VHT rates, and STBC/LDPC all work well  
 - Narrowband support in monitor mode, 5MHz/10MHz bandwidth  

Need test:  
 - EDCCA patch  
 - \~4K Maximum MTU  
 - DKMS build script  
 - ACK timeout (in procfs)  
 - 10MHz BW in AP/STA mode (seems that the firmware needs to be replaced: [hal8822c_fw_10M.c](https://github.com/libc0607/rtl88x2cu-20230728/blob/main/hal/rtl8822c/hal8822c_fw_10M.c))  
 - Thermal sensor default offset (the sensor readout code is working, but it needs a single-point calibration, so a default value needs to be set depending on some actual measurement)  
 - Single tone output (/proc, single_tone)  
 - Disable CCA (/proc, dis_cca)  

Known bugs/issues:  
 - Short GI not working (not the radiotap header issue -- even when TXDESC is set by SET_TX_DESC_DATA_SHORT_8822C()). Maybe it's a firmware bug. But you should always use long GI on your drone so it doesn't matter  
 - ~~Set txpower by ```iw``` higher than a certain (depends on the module, but significantly higher than the factory) value will cause tx to hang, and cannot be recovered unless manually reset the adaptor. Not sure if it's a power issue caused by poor PCB trace width inside the module (it's reasonable to not use an inner power plane for a \~300mA total chip...), some firmware bug, or even some analog silicon bug (latch-up?). Not caused by high temperature (water cooling tested, still), or any high TX traffic. Weird...~~
 - LB-LINK said the issue above can be a power failure, and a proper 3.3V power supply is needed. I was using 3.3V/1A, so I recommend at least 3.3V/2A  
