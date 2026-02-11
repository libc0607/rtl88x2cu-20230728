# rtl88x2cu
RTL8812CU/RTL8822CU Linux Driver v5.15.8.5-3 20240412 FPV Mod  
Download CC's pre-built OpenIPC firmware with this driver integrated [here](https://github.com/zhouruixi/OpenIPC-firmware), or Radxa Zero 3 Groundstation firmware [here](https://github.com/zhouruixi/SBC-GS)    

## Installation & Usage
See [libc0607/rtl88x2eu-20230815](https://github.com/libc0607/rtl88x2eu-20230815?tab=readme-ov-file#installation)

## Current status 
### Tested
 - Build on kernel 6.12  
 - TX power unlocked 
 - RX in monitor mode, 5/10/20/40/80MHz BW  
 - TX injection, 5/10/20MHz BW, HT/VHT rates, STBC/LDPC
 - DKMS build script
 - Thermal sensor
 - bf_monitor
 - Channel state scanning

### Need test
 - EDCCA patch  
 - \~4K Maximum MTU   
 - 10MHz BW in AP/STA mode (seems that the firmware needs to be replaced: [hal8822c_fw_10M.c](https://github.com/libc0607/rtl88x2cu-20230728/blob/main/hal/rtl8822c/hal8822c_fw_10M.c))  
 - Single tone output (/proc, single_tone)  
 - Disable CCA (/proc, dis_cca)

## Resources
Original driver from Realtek:  
[rtl88x2CU_WiFi_linux_v5.15.8-52-g35e62e7c9.20230728_COEX20221215-3130.tar.gz](https://github.com/user-attachments/files/20281949/rtl88x2CU_WiFi_linux_v5.15.8-52-g35e62e7c9.20230728_COEX20221215-3130.tar.gz)  
[rtl88x2CU_WiFi_linux_v5.15.8.5-3-g88098843f.20240412_COEX20221215-3130.tar.gz](https://github.com/user-attachments/files/20281946/rtl88x2CU_WiFi_linux_v5.15.8.5-3-g88098843f.20240412_COEX20221215-3130.tar.gz) (latest)  
Changelog (from Realtek): [ReleaseNotes.pdf](https://github.com/user-attachments/files/20281972/ReleaseNotes.pdf)  

## Open Source Hardware 
[BL-M8812CU3_datasheet_V1.0.1.1.pdf](https://github.com/user-attachments/files/20282045/BL-M8812CU3_datasheet_V1.0.1.1.pdf)  
[BL-M8812CU2_Datasheet_V1.0.pdf](https://github.com/user-attachments/files/25232595/BL-M8812CU2_Datasheet_V1.0.pdf)  
[RTL8812CU-CG_V4.3r03-20191113_.pdf](https://github.com/user-attachments/files/25232605/RTL8812CU-CG_V4.3r03-20191113_.pdf)  
[RTL8822CU_UM822C00.pdf](https://github.com/user-attachments/files/25232608/RTL8822CU_UM822C00.pdf)  


Schematic & PCB design (using the chip, not module):  
[libc0607/rtl88x2cu_rfpa5542_20mm_demo_v1p0](https://oshwhub.com/libc0607/rtl88x2cu_rfpa5542_20mm_demo_v1p0)  
![image](https://github.com/user-attachments/assets/f4dacddd-c369-4e84-b180-793ddcc076fe)  

[RTL8822CU Demo](https://oshwhub.com/vinvaa/88x2cu) by xi0n9Fpv  
![image](https://github.com/user-attachments/assets/cfd97b3d-322b-45d4-941c-8739e54030ac)  


TX power test: [JasonFreeLab/rtl8812cu](https://github.com/JasonFreeLab/rtl8812cu?tab=readme-ov-file#ant0_5825mhz_20m_11n_mcs1_mbm1700)  
(TL;DR: 23~24 dBm, MCS1)  
![image](https://github.com/user-attachments/assets/c2a1bb0a-37fd-4392-adba-c46d5187ba68)  

### Known bugs/issues
 - [Injection instability on 40/80MHz channels](https://github.com/libc0607/rtl88x2eu-20230815/issues/7): Seems a firmware bug, only can be solved by Realtek releasing some new firmware  
 - Injecting packet with short GI not working (not the radiotap header issue -- even when TXDESC is set by SET_TX_DESC_DATA_SHORT_8822C()). Maybe it's a firmware bug. But you should always use long GI on your drone so it doesn't matter
