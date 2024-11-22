/******************************************************************************
 *
 * RTL88x2CU userspace explicit SU beamforming for monitor mode
 * https://github.com/libc0607/rtl88x2cu-20230728
 * 
 * This re-implementation from scratch is a replacement of core/rtw_beamforming.c and hal/rtl8822c/rtl8822c_phy.c
 * and then exposes APIs via procfs to userspace applications (injection-based app -- mainly wfb-ng and its variants).
 *
 *
 * The macro CONFIG_BEAMFORMING should be disabled to avoid any conflict. Instead, use CONFIG_BEAMFORMING_MONITOR.
 * Some function from original implementation was renamed with "_monitor" suffix so you know what it was.
 *
 * This is a *simplified*, standalone version: 
 *  - Only VHT SU explicit beamforming is supported, and I've removed all HT/VHT-MU related code;
 *  - Not involving anything related to AP/STA. Designed to use with monitor mode;
 *  - Uses TXBF index 0 reg set.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/

#ifdef CONFIG_BEAMFORMING_MONITOR
/* REG_TXBF_CTRL		(Offset 0x42C) */
#define BITS_R_TXBF1_AID_8822C			(BIT_MASK_R_TXBF1_AID_8822C << BIT_SHIFT_R_TXBF1_AID_8822C)
#define BIT_CLEAR_R_TXBF1_AID_8822C(x)		((x) & (~BITS_R_TXBF1_AID_8822C))
#define BIT_SET_R_TXBF1_AID_8822C(x, v)		(BIT_CLEAR_R_TXBF1_AID_8822C(x) | BIT_R_TXBF1_AID_8822C(v))

#define BITS_R_TXBF0_AID_8822C			(BIT_MASK_R_TXBF0_AID_8822C << BIT_SHIFT_R_TXBF0_AID_8822C)
#define BIT_CLEAR_R_TXBF0_AID_8822C(x)		((x) & (~BITS_R_TXBF0_AID_8822C))
#define BIT_SET_R_TXBF0_AID_8822C(x, v)		(BIT_CLEAR_R_TXBF0_AID_8822C(x) | BIT_R_TXBF0_AID_8822C(v))

/* REG_NDPA_OPT_CTRL		(Offset 0x45F) */
#define BITS_R_NDPA_BW_8822C			(BIT_MASK_R_NDPA_BW_8822C << BIT_SHIFT_R_NDPA_BW_8822C)
#define BIT_CLEAR_R_NDPA_BW_8822C(x)		((x) & (~BITS_R_NDPA_BW_8822C))
#define BIT_SET_R_NDPA_BW_8822C(x, v)		(BIT_CLEAR_R_NDPA_BW_8822C(x) | BIT_R_NDPA_BW_8822C(v))

/* REG_ASSOCIATED_BFMEE_SEL	(Offset 0x714) */
#define BITS_AID1_8822C				(BIT_MASK_AID1_8822C << BIT_SHIFT_AID1_8822C)
#define BIT_CLEAR_AID1_8822C(x)			((x) & (~BITS_AID1_8822C))
#define BIT_SET_AID1_8822C(x, v)		(BIT_CLEAR_AID1_8822C(x) | BIT_AID1_8822C(v))

#define BITS_AID0_8822C				(BIT_MASK_AID0_8822C << BIT_SHIFT_AID0_8822C)
#define BIT_CLEAR_AID0_8822C(x)			((x) & (~BITS_AID0_8822C))
#define BIT_SET_AID0_8822C(x, v)		(BIT_CLEAR_AID0_8822C(x) | BIT_AID0_8822C(v))

/* REG_SND_PTCL_CTRL		(Offset 0x718) */
#define BIT_VHTNDP_RPTPOLL_CSI_STR_OFFSET_SEL_8822C	BIT(15)

/* REG_MU_TX_CTL		(Offset 0x14C0) */
#define BIT_R_MU_P1_WAIT_STATE_EN_8822C		BIT(16)

#define BIT_SHIFT_R_MU_RL_8822C			12
/* #define BIT_MASK_R_MU_RL_8822C			0xF */
#define BITS_R_MU_RL_8822C			(BIT_MASK_R_MU_RL_8822C << BIT_SHIFT_R_MU_RL_8822C)
#define BIT_R_MU_RL_8822C(x)			(((x) & BIT_MASK_R_MU_RL_8822C) << BIT_SHIFT_R_MU_RL_8822C)
#define BIT_CLEAR_R_MU_RL_8822C(x)		((x) & (~BITS_R_MU_RL_8822C))
#define BIT_SET_R_MU_RL_8822C(x, v)		(BIT_CLEAR_R_MU_RL_8822C(x) | BIT_R_MU_RL_8822C(v))

#define BIT_SHIFT_R_MU_TAB_SEL_8822C		8
#define BIT_MASK_R_MU_TAB_SEL_8822C		0x7
#define BITS_R_MU_TAB_SEL_8822C			(BIT_MASK_R_MU_TAB_SEL_8822C << BIT_SHIFT_R_MU_TAB_SEL_8822C)
#define BIT_R_MU_TAB_SEL_8822C(x)		(((x) & BIT_MASK_R_MU_TAB_SEL_8822C) << BIT_SHIFT_R_MU_TAB_SEL_8822C)
#define BIT_CLEAR_R_MU_TAB_SEL_8822C(x)		((x) & (~BITS_R_MU_TAB_SEL_8822C))
#define BIT_SET_R_MU_TAB_SEL_8822C(x, v)	(BIT_CLEAR_R_MU_TAB_SEL_8822C(x) | BIT_R_MU_TAB_SEL_8822C(v))

#define BIT_R_EN_MU_MIMO_8822C			BIT(7)

#define BITS_R_MU_TABLE_VALID_8822C		(BIT_MASK_R_MU_TABLE_VALID_8822C << BIT_SHIFT_R_MU_TABLE_VALID_8822C)
#define BIT_CLEAR_R_MU_TABLE_VALID_8822C(x)	((x) & (~BITS_R_MU_TABLE_VALID_8822C))
#define BIT_SET_R_MU_TABLE_VALID_8822C(x, v)	(BIT_CLEAR_R_MU_TABLE_VALID_8822C(x) | BIT_R_MU_TABLE_VALID_8822C(v))

/* REG_WMAC_MU_BF_CTL		(Offset 0x1680) */
#define BITS_WMAC_MU_BFRPTSEG_SEL_8822C			(BIT_MASK_WMAC_MU_BFRPTSEG_SEL_8822C << BIT_SHIFT_WMAC_MU_BFRPTSEG_SEL_8822C)
#define BIT_CLEAR_WMAC_MU_BFRPTSEG_SEL_8822C(x)		((x) & (~BITS_WMAC_MU_BFRPTSEG_SEL_8822C))
#define BIT_SET_WMAC_MU_BFRPTSEG_SEL_8822C(x, v)	(BIT_CLEAR_WMAC_MU_BFRPTSEG_SEL_8822C(x) | BIT_WMAC_MU_BFRPTSEG_SEL_8822C(v))

#define BITS_WMAC_MU_BF_MYAID_8822C		(BIT_MASK_WMAC_MU_BF_MYAID_8822C << BIT_SHIFT_WMAC_MU_BF_MYAID_8822C)
#define BIT_CLEAR_WMAC_MU_BF_MYAID_8822C(x)	((x) & (~BITS_WMAC_MU_BF_MYAID_8822C))
#define BIT_SET_WMAC_MU_BF_MYAID_8822C(x, v)	(BIT_CLEAR_WMAC_MU_BF_MYAID_8822C(x) | BIT_WMAC_MU_BF_MYAID_8822C(v))

/* REG_WMAC_ASSOCIATED_MU_BFMEE7	(Offset 0x168E) */
#define BIT_STATUS_BFEE7_8822C			BIT(10)

struct ndpa_sta_info_monitor {
	u16 aid:12;
	u16 feedback_type:1;
	u16 nc_index:3;
};

struct csi_rpt_monitor {
	u8 nc;
	u8 nr;
	enum channel_width bw;
	u8 ng;
	u8 codebook;
	u8 token;
	s8 rx_pwr[4];   // per-path's pwdb
	s8 rx_snr[4];   // per-path's SNR
	u8 rx_evm[4];   // per-path's EVM
	s8 snr[4];      // [-128,127], map to [-10dB,+53.75dB]
	u16 csi_matrix_len; // 80m=293(292.5)/234sc, 40m=135/108sc, 20m=65/52sc, 6bit_phi+4bit_psi
	//u8 csi_matrix[293];  // useless
};


// For PROCFS

void    bf_monitor_init(PADAPTER adapter, u8* bfer_macaddr, u8 bfer_g_id, u16 bfee_p_aid);
void    bf_monitor_reset(PADAPTER adapter);
void    bf_monitor_send(PADAPTER adapter, u8* bfer_macaddr, u8* bfee_macaddr, u16 p_aid, u8 g_id, u8 seq, enum channel_width bw);
void    bf_monitor_print_cbr(PADAPTER adapter, struct seq_file *m);
void    bf_monitor_print_conf(PADAPTER adapter, struct seq_file *m);
void    bf_monitor_enable_txbf(PADAPTER adapter, bool en);


// Internal APIs

u8     bf_monitor_send_vht_ndpa(PADAPTER adapter, u8* ra, u8* ta, u16 p_aid, u8 g_id, u8 seq, enum channel_width bw);
void   bf_monitor_reg_init(PADAPTER adapter);
void   bf_monitor_phydm_txbf_rfmode_8822c(PADAPTER adapter, u8 su_bfee_cnt);
void   bf_monitor_phy_reset_rtl8822c(PADAPTER adapter);

void   bf_monitor_sounding_config(PADAPTER adapter, u8 bf_en, enum channel_width bw);
void   bf_monitor_sounding_reset(PADAPTER adapter);
void   bf_monitor_leave(PADAPTER adapter);

void   bf_monitor_config_beamformer(PADAPTER adapter, u8* bfer_macaddr, u8 g_id);
void   bf_monitor_config_beamformee(PADAPTER adapter, u16 p_aid);
void   bf_monitor_reset_beamformer(PADAPTER adapter);
void   bf_monitor_reset_beamformee(PADAPTER adapter);



// Hooks for RX

// called by OnAction_vht() when beamformer get "VHT compressed beamforming" packet (feedback) from beamformee
u32     bf_monitor_get_report_packet(PADAPTER adapter, union recv_frame *precv_frame);
// called in validate_recv_ctrl_frame(), when beamformee get NDPA packet
//void    bf_monitor_get_ndpa_packet(PADAPTER adapter, union recv_frame *precv_frame);
// called by process_c2h_event(), for firmware reporting sounding success/fail
void    bf_monitor_c2h_snd_txbf(PADAPTER adapter, u8 *buf, u8 buf_len);



#endif /*CONFIG_BEAMFORMING_MONITOR*/

