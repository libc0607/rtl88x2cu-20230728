# rtl88x2cu-20230728
(WIP) RTL8812CU/RTL8822CU Linux Driver v5.15.8-52-20230728 FPV Mod  

Original driver from Realtek: [RTL8822CU_WiFi_linux_v5.15.8-52-g35e62e7c9_COEX20221215-3130.20230728.tar.gz](https://github.com/libc0607/rtl88x2cu-20230728/blob/main/doc/RTL8822CU_WiFi_linux_v5.15.8-52-g35e62e7c9_COEX20221215-3130.20230728.tar.gz)  
BL-M8812CU3 datasheet: [BL-M8812CU3_datasheet_V1.0.1.1.pdf](https://github.com/libc0607/rtl88x2cu-20230728/blob/main/doc/BL-M8812CU3_datasheet_V1.0.1.1.pdf)  

The RTL8812CU has fully integrated RF frontends (PA, LNA, and SPDT switch), so you will rarely see any RTL8812CU adaptor with PA/LNA (adding the PA means an additional SPDT switch). 
The internal PAs have really good performance (HT20, MCS7, ~16dBm TX, -28dB EVM), but it's still limited by the 3.3V power and poor heat dissipation.  
In a word -- usable, but not recommended for FPV. Use the blue square 8812EU instead.  

Tested:  
 - Build on kernel 6.5  
 - TX power unlocked (same as the 8812eu driver, ```iw set fixed```)  
 - RX in monitor mode  
 - TX injection in monitor mode, HT/VHT rates, and STBC/LDPC all work well  
 - Narrowband support in monitor mode, 5MHz(Yes it's working!)/10MHz bandwidth

Need test or can be further investigated:  
 - EDCCA patch  
 - \~4K Maximum MTU  
 - DKMS build script
 - ACK timeout (in procfs)
 - 10MHz BW in AP/STA mode (seems that the firmware needs to be replaced: [hal8822c_fw_10M.c](https://github.com/libc0607/rtl88x2cu-20230728/blob/main/hal/rtl8822c/hal8822c_fw_10M.c))
 - Thermal sensor default offset (the sensor readout code is working, but it needs a single-point calibration, so a default value needs to be set depending on some actual measurement)

Known bugs/issues:  
 - Short GI not working (not the radiotap header issue -- even when TXDESC is set by SET_TX_DESC_DATA_SHORT_8822C()). Maybe it's a firmware bug
 - Set txpower by ```iw``` higher than a certain (depends on the module, but significantly higher than the factory) value will cause tx to hang, and cannot be recovered unless manually reset the adaptor. Not sure if it's a power issue caused by poor PCB trace width inside the module (it's reasonable to not use an inner power plane for a ~300mA total chip...), some firmware bug, or even some analog silicon bug (latch-up?). Not caused by high temperature (water cooling tested, still), or any high TX traffic. Weird...
