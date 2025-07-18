/******************************************************************************
 *
 * Copyright(c) 2007 - 2019 Realtek Corporation.
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
#ifndef __RTW_MLME_EXT_H_
#define __RTW_MLME_EXT_H_


/*
 * SURVEY_TO: dwell time (ms) for passive channel scan or other scan functions
 * in addition to normal scan.
 * ACTIVE_CH_SURVEY_TO: dwell time (ms) for active scan channel in nomal scan.
 */
#define SURVEY_TO		(100)
#define ACTIVE_CH_SURVEY_TO	(50)

#define REAUTH_TO		(300) /* (50) */
#define REASSOC_TO		(300) /* (50) */
/* #define DISCONNECT_TO	(3000) */
#define ADDBA_TO			(2000)

#define LINKED_TO (1) /* unit:2 sec, 1x2 = 2 sec */

#define REAUTH_LIMIT	(4)
#define REASSOC_LIMIT	(4)
#define READDBA_LIMIT	(2)

#ifdef CONFIG_GSPI_HCI
	#define ROAMING_LIMIT	5
#else
	#define ROAMING_LIMIT	8
#endif

/*net_type, pmlmeinfo->state*/
#define _HW_STATE_NOLINK_	0x00
#define _HW_STATE_ADHOC_	0x01
#define _HW_STATE_STATION_	0x02
#define _HW_STATE_AP_		0x03
#define _HW_STATE_MONITOR_	0x04

#define	WIFI_FW_NULL_STATE		_HW_STATE_NOLINK_
#define	WIFI_FW_STATION_STATE		_HW_STATE_STATION_
#define	WIFI_FW_AP_STATE		_HW_STATE_AP_
#define	WIFI_FW_ADHOC_STATE		_HW_STATE_ADHOC_

#define	WIFI_FW_PRE_LINK		0x00000800
#define	WIFI_FW_AUTH_NULL		0x00000100
#define	WIFI_FW_AUTH_STATE		0x00000200
#define	WIFI_FW_AUTH_SUCCESS		0x00000400

#define	WIFI_FW_ASSOC_STATE		0x00002000
#define	WIFI_FW_ASSOC_SUCCESS		0x00004000

#define	WIFI_FW_LINKING_STATE		(WIFI_FW_AUTH_NULL | WIFI_FW_AUTH_STATE | WIFI_FW_AUTH_SUCCESS | WIFI_FW_ASSOC_STATE)


#define		_1M_RATE_	0
#define		_2M_RATE_	1
#define		_5M_RATE_	2
#define		_11M_RATE_	3
#define		_6M_RATE_	4
#define		_9M_RATE_	5
#define		_12M_RATE_	6
#define		_18M_RATE_	7
#define		_24M_RATE_	8
#define		_36M_RATE_	9
#define		_48M_RATE_	10
#define		_54M_RATE_	11

/********************************************************
MCS rate definitions
*********************************************************/
#define MCS_RATE_1R	(0x000000ff)
#define MCS_RATE_2R	(0x0000ffff)
#define MCS_RATE_3R	(0x00ffffff)
#define MCS_RATE_4R	(0xffffffff)
#define MCS_RATE_2R_13TO15_OFF	(0x00001fff)


extern unsigned char RTW_WPA_OUI[];
extern unsigned char WMM_OUI[];
extern unsigned char WPS_OUI[];
extern unsigned char WFD_OUI[];
extern unsigned char P2P_OUI[];
extern unsigned char MULTI_AP_OUI[];

extern unsigned char WMM_INFO_OUI[];
extern unsigned char WMM_PARA_OUI[];

typedef enum _HT_IOT_PEER {
	HT_IOT_PEER_UNKNOWN			= 0,
	HT_IOT_PEER_REALTEK			= 1,
	HT_IOT_PEER_REALTEK_92SE		= 2,
	HT_IOT_PEER_BROADCOM		= 3,
	HT_IOT_PEER_RALINK			= 4,
	HT_IOT_PEER_ATHEROS			= 5,
	HT_IOT_PEER_CISCO				= 6,
	HT_IOT_PEER_MERU				= 7,
	HT_IOT_PEER_MARVELL			= 8,
	HT_IOT_PEER_REALTEK_SOFTAP 	= 9,/* peer is RealTek SOFT_AP, by Bohn, 2009.12.17 */
	HT_IOT_PEER_SELF_SOFTAP 		= 10, /* Self is SoftAP */
	HT_IOT_PEER_AIRGO				= 11,
	HT_IOT_PEER_INTEL				= 12,
	HT_IOT_PEER_RTK_APCLIENT		= 13,
	HT_IOT_PEER_REALTEK_81XX		= 14,
	HT_IOT_PEER_REALTEK_WOW		= 15,
	HT_IOT_PEER_REALTEK_JAGUAR_BCUTAP = 16,
	HT_IOT_PEER_REALTEK_JAGUAR_CCUTAP = 17,
	HT_IOT_PEER_MAX				= 18
} HT_IOT_PEER_E, *PHTIOT_PEER_E;


typedef enum _RT_HT_INF0_CAP {
	RT_HT_CAP_USE_TURBO_AGGR = 0x01,
	RT_HT_CAP_USE_LONG_PREAMBLE = 0x02,
	RT_HT_CAP_USE_AMPDU = 0x04,
	RT_HT_CAP_USE_WOW = 0x8,
	RT_HT_CAP_USE_SOFTAP = 0x10,
	RT_HT_CAP_USE_92SE = 0x20,
	RT_HT_CAP_USE_88C_92C = 0x40,
	RT_HT_CAP_USE_AP_CLIENT_MODE = 0x80,	/* AP team request to reserve this bit, by Emily */
} RT_HT_INF0_CAPBILITY, *PRT_HT_INF0_CAPBILITY;

typedef enum _RT_HT_INF1_CAP {
	RT_HT_CAP_USE_VIDEO_CLIENT = 0x01,
	RT_HT_CAP_USE_JAGUAR_BCUT = 0x02,
	RT_HT_CAP_USE_JAGUAR_CCUT = 0x04,
} RT_HT_INF1_CAPBILITY, *PRT_HT_INF1_CAPBILITY;

struct mlme_handler {
	unsigned int   num;
	char *str;
	unsigned int (*func)(_adapter *padapter, union recv_frame *precv_frame);
};

struct action_handler {
	unsigned int   num;
	char *str;
	unsigned int (*func)(_adapter *padapter, union recv_frame *precv_frame);
};

enum SCAN_STATE {
	SCAN_DISABLE = 0,
	SCAN_START = 1,
	SCAN_PS_ANNC_WAIT = 2,
	SCAN_ENTER = 3,
	SCAN_PROCESS = 4,

	/* backop */
	SCAN_BACKING_OP = 5,
	SCAN_BACK_OP = 6,
	SCAN_LEAVING_OP = 7,
	SCAN_LEAVE_OP = 8,

	/* SW antenna diversity (before linked) */
	SCAN_SW_ANTDIV_BL = 9,

	/* legacy p2p */
	SCAN_TO_P2P_LISTEN = 10,
	SCAN_P2P_LISTEN = 11,

	SCAN_COMPLETE = 12,
	SCAN_STATE_MAX,
};

const char *scan_state_str(u8 state);

enum ss_backop_flag {
	SS_BACKOP_EN = BIT0, /* backop when linked */
	SS_BACKOP_EN_NL = BIT1, /* backop even when no linked */

	SS_BACKOP_PS_ANNC = BIT4,
	SS_BACKOP_TX_RESUME = BIT5,
};

struct ss_res {
	u8 state;
	u8 next_state; /* will set to state on next cmd hdl */
	int	bss_cnt;
	u8 activate_ch_cnt;
	int	channel_idx;
	u8 force_ssid_scan;
	int	scan_mode;
	u16 scan_ch_ms;
	u32 scan_timeout_ms;
	u8 rx_ampdu_accept;
	u8 rx_ampdu_size;
	u8 igi_scan;
	u8 igi_before_scan; /* used for restoring IGI value without enable DIG & FA_CNT */
#ifdef CONFIG_SCAN_BACKOP
	u8 backop_flags_sta; /* policy for station mode*/
	#ifdef CONFIG_AP_MODE
	u8 backop_flags_ap; /* policy for ap mode */
	#endif
	#ifdef CONFIG_RTW_MESH
	u8 backop_flags_mesh; /* policy for mesh mode */
	#endif
	u8 backop_flags; /* per backop runtime decision */
	u8 scan_cnt;
	u8 scan_cnt_max;
	systime backop_time; /* the start time of backop */
	u16 backop_ms;
#endif
#if defined(CONFIG_ANTENNA_DIVERSITY) || defined(DBG_SCAN_SW_ANTDIV_BL)
	u8 is_sw_antdiv_bl_scan;
#endif
	u8 ssid_num;
	u8 ch_num;
	NDIS_802_11_SSID ssid[RTW_SSID_SCAN_AMOUNT];
	struct rtw_ieee80211_channel ch[RTW_CHANNEL_SCAN_AMOUNT];

	u32 token; 	/* 0: use to identify caller */
	u16 duration;	/* 0: use default */
	u8 igi;		/* 0: use defalut */
	u8 bw;		/* 0: use default */

	bool acs; /* aim to trigger channel selection when scan done */
};

#ifdef CONFIG_TDLS
enum TDLS_option {
	TDLS_ESTABLISHED = 1,
	TDLS_ISSUE_PTI,
	TDLS_CH_SW_RESP,
	TDLS_CH_SW_PREPARE,
	TDLS_CH_SW_START,
	TDLS_CH_SW_TO_OFF_CHNL,
	TDLS_CH_SW_TO_BASE_CHNL_UNSOLICITED,
	TDLS_CH_SW_TO_BASE_CHNL,
	TDLS_CH_SW_END_TO_BASE_CHNL,
	TDLS_CH_SW_END,
	TDLS_RS_RCR,
	TDLS_TEARDOWN_STA,
	TDLS_TEARDOWN_STA_TOOFAR,
	TDLS_TEARDOWN_STA_NO_WAIT,
	TDLS_TEARDOWN_STA_LOCALLY,
	TDLS_TEARDOWN_STA_LOCALLY_POST,
	maxTDLS,
};

#endif /* CONFIG_TDLS */

/*
 * Usage:
 * When one iface acted as AP mode and the other iface is STA mode and scanning,
 * it should switch back to AP's operating channel periodically.
 * Parameters info:
 * When the driver scanned RTW_SCAN_NUM_OF_CH channels, it would switch back to AP's operating channel for
 * RTW_BACK_OP_CH_MS milliseconds.
 * Example:
 * For chip supports 2.4G + 5GHz and AP mode is operating in channel 1,
 * RTW_SCAN_NUM_OF_CH is 8, RTW_BACK_OP_CH_MS is 300
 * When it's STA mode gets set_scan command,
 * it would
 * 1. Doing the scan on channel 1.2.3.4.5.6.7.8
 * 2. Back to channel 1 for 300 milliseconds
 * 3. Go through doing site survey on channel 9.10.11.36.40.44.48.52
 * 4. Back to channel 1 for 300 milliseconds
 * 5. ... and so on, till survey done.
 */
#if defined(CONFIG_ATMEL_RC_PATCH)
	#define RTW_SCAN_NUM_OF_CH 2
	#define RTW_BACK_OP_CH_MS 200
#elseif defined(CONFIG_CUSTOMER_EZVIZ_CHIME2)
	#define RTW_SCAN_NUM_OF_CH 1
	#define RTW_BACK_OP_CH_MS 200
#else
	#define RTW_SCAN_NUM_OF_CH 3
	#define RTW_BACK_OP_CH_MS 400
#endif

#define RTW_IP_ADDR_LEN 4
#define RTW_IPv6_ADDR_LEN 16

struct mlme_ext_info {
	u32	state;
#ifdef CONFIG_MI_WITH_MBSSID_CAM
	u8	hw_media_state;
#endif
	u32	reauth_count;
	u32	reassoc_count;
	u32	link_count;
	u32	auth_seq;
	u32	auth_algo;	/* 802.11 auth, could be open, shared, auto */
	u16 auth_status;
	u32	authModeToggle;
	u32	enc_algo;/* encrypt algorithm; */
	u32	key_index;	/* this is only valid for legendary wep, 0~3 for key id. */
	u32	iv;
	u8	chg_txt[128];
	u16	aid;
	u16	bcn_interval;
	u16	capability;
	u8	assoc_AP_vendor;
	u8	slotTime;
	u8	preamble_mode;
	u8	WMM_enable;
	u8	ERP_enable;
	u8	ERP_IE;
	u8	HT_enable;
	u8	HT_caps_enable;
	u8	HT_info_enable;
	u8	HT_protection;
	u8	turboMode_cts2self;
	u8	turboMode_rtsen;
	u8	SM_PS;
	u8	agg_enable_bitmap;
	u8	ADDBA_retry_count;
	u8	candidate_tid_bitmap;
	u8	dialogToken;
	/* Accept ADDBA Request */
	BOOLEAN bAcceptAddbaReq;
	u8	bwmode_updated;
	u8	hidden_ssid_mode;
	u8	VHT_enable;

	u8 ip_addr[RTW_IP_ADDR_LEN];
	u8 ip6_addr[RTW_IPv6_ADDR_LEN];

	struct ADDBA_request		ADDBA_req;
	struct WMM_para_element	WMM_param;
	struct HT_caps_element	HT_caps;
	struct HT_info_element		HT_info;
	WLAN_BSSID_EX			network;/* join network or bss_network, if in ap mode, it is the same to cur_network.network */
#ifdef ROKU_PRIVATE
	/*infra mode, store supported rates from AssocRsp*/
	NDIS_802_11_RATES_EX	SupportedRates_infra_ap;
	u8 ht_vht_received;/*ht_vht_received used to show debug msg BIT(0):HT BIT(1):VHT */
#endif /* ROKU_PRIVATE */

#ifdef CONFIG_WRITE_BCN_LEN_TO_FW
	u16 last_bcn_len;
#endif
};

void init_channel_list(_adapter *padapter);

int rtw_rfctl_init(_adapter *adapter);
void rtw_rfctl_deinit(_adapter *adapter);
void rtw_rfctl_update_op_mode(struct rf_ctl_t *rfctl, u8 ifbmp_mod, u8 if_op, u8 ifbmp_excl);

bool rtw_mlme_band_check(_adapter *adapter, const u32 ch);


enum {
	BAND_24G = BIT0,
	BAND_5G = BIT1,
};
void RTW_SET_SCAN_BAND_SKIP(_adapter *padapter, int skip_band);
void RTW_CLR_SCAN_BAND_SKIP(_adapter *padapter, int skip_band);
int RTW_GET_SCAN_BAND_SKIP(_adapter *padapter);

bool rtw_mlme_ignore_chan(_adapter *adapter, const u32 ch);

/* P2P_MAX_REG_CLASSES - Maximum number of regulatory classes */
#define P2P_MAX_REG_CLASSES 10

/* P2P_MAX_REG_CLASS_CHANNELS - Maximum number of channels per regulatory class */
#define P2P_MAX_REG_CLASS_CHANNELS 20

/* struct p2p_channels - List of supported channels */
struct p2p_channels {
	/* struct p2p_reg_class - Supported regulatory class */
	struct p2p_reg_class {
		/* reg_class - Regulatory class (IEEE 802.11-2007, Annex J) */
		u8 reg_class;

		/* channel - Supported channels */
		u8 channel[P2P_MAX_REG_CLASS_CHANNELS];

		/* channels - Number of channel entries in use */
		size_t channels;
	} reg_class[P2P_MAX_REG_CLASSES];

	/* reg_classes - Number of reg_class entries in use */
	size_t reg_classes;
};

struct p2p_oper_class_map {
	enum hw_mode {IEEE80211G, IEEE80211A} mode;
	u8 op_class;
	u8 min_chan;
	u8 max_chan;
	u8 inc;
	enum { BW20, BW40PLUS, BW40MINUS } bw;
};

struct mlme_ext_priv {
	_adapter	*padapter;
	u8	mlmeext_init;
	ATOMIC_T		event_seq;
	u16	mgnt_seq;
#ifdef CONFIG_IEEE80211W
	u16	sa_query_seq;
#endif
	/* struct fw_priv 	fwpriv; */

	unsigned char	cur_channel;
	unsigned char	cur_bwmode;
	unsigned char	cur_ch_offset;/* PRIME_CHNL_OFFSET */
	unsigned char	cur_wireless_mode;	/* NETWORK_TYPE */

	unsigned char	basicrate[NumRates];
	unsigned char	datarate[NumRates];
#ifdef CONFIG_80211N_HT
	unsigned char default_supported_mcs_set[16];
#endif

	struct ss_res		sitesurvey_res;
#ifdef CONFIG_RTW_ROAM_QUICKSCAN
	u8      quickscan_next;
	u8      roam_ch_num;
	struct  rtw_ieee80211_channel roam_ch[RTW_CHANNEL_SCAN_AMOUNT];
#endif
	struct mlme_ext_info	mlmext_info;/* for sta/adhoc mode, including current scanning/connecting/connected related info.
                                                      * for ap mode, network includes ap's cap_info */
	_timer		survey_timer;
	_timer		link_timer;

#ifdef CONFIG_RTW_REPEATER_SON
	_timer		rson_scan_timer;
#endif
#ifdef CONFIG_RTW_80211R
	_timer		ft_link_timer;
	_timer		ft_roam_timer;
#endif
#ifdef CONFIG_RTW_TOKEN_BASED_XMIT
	_timer		tbtx_xmit_timer;
	_timer		tbtx_token_dispatch_timer;
#endif

	systime last_scan_time;
	u8	scan_abort;
	u8 join_abort;

	u8	tx_rate; /* TXRATE when USERATE is set. */
	RATE_SECTION tx_rate_section; /* decided by tx_rate */

	u32	retry; /* retry for issue probereq */

	u64 TSFValue;
	u32 bcn_cnt;
	u32 last_bcn_cnt;
	u8 cur_bcn_cnt;/*2s*/
	u8 dtim;/*DTIM Period*/
#ifdef DBG_RX_BCN
	u8 tim[4];
#endif
#ifdef CONFIG_BCN_RECV_TIME
	u16 bcn_rx_time;
#endif
#ifdef CONFIG_AP_MODE
	unsigned char bstart_bss;
#endif

	/* recv_decache check for Action_public frame */
	u8 action_public_dialog_token;
	u16	 action_public_rxseq;

	/* #ifdef CONFIG_ACTIVE_KEEP_ALIVE_CHECK */
	u8 active_keep_alive_check;
	/* #endif */
#ifdef DBG_FIXED_CHAN
	u8 fixed_chan;
#endif

	u8 tsf_update_required:1;
	u8 en_hw_update_tsf:1; /* set hw sync bcn tsf register or not */
	systime tsf_update_pause_stime;
	u8 tsf_update_pause_factor; /* num of bcn intervals to stay TSF update pause status */
	u8 tsf_update_restore_factor; /* num of bcn interval to stay TSF update restore status */
#ifdef CONFIG_SUPPORT_STATIC_SMPS
	u8 ssmps_en;
	u16 ssmps_tx_tp_th;/*Mbps*/
	u16 ssmps_rx_tp_th;/*Mbps*/
	#ifdef DBG_STATIC_SMPS
	u8 ssmps_test;
	u8 ssmps_test_en;
	#endif
#endif
#ifdef CONFIG_CTRL_TXSS_BY_TP
	u8 txss_ctrl_en;
	u16 txss_tp_th;/*Mbps*/
	u8 txss_tp_chk_cnt;/*unit 2s*/
	bool txss_1ss;
	u8 txss_momi_type_bk;
#endif
#ifdef CONFIG_DFS
	_timer csa_timer;
#endif /* CONFIG_DFS */
};

struct support_rate_handler {
	u8 rate;
	bool basic;
	bool existence;
};

static inline u8 check_mlmeinfo_state(struct mlme_ext_priv *plmeext, sint state)
{
	if ((plmeext->mlmext_info.state & 0x03) == state)
		return _TRUE;

	return _FALSE;
}

void sitesurvey_set_offch_state(_adapter *adapter, u8 scan_state);

#define mlmeext_msr(mlmeext) ((mlmeext)->mlmext_info.state & 0x03)
#define mlmeext_scan_state(mlmeext) ((mlmeext)->sitesurvey_res.state)
#define mlmeext_scan_state_str(mlmeext) scan_state_str((mlmeext)->sitesurvey_res.state)
#define mlmeext_chk_scan_state(mlmeext, _state) ((mlmeext)->sitesurvey_res.state == (_state))
#define mlmeext_set_scan_state(mlmeext, _state) \
	do { \
		((mlmeext)->sitesurvey_res.state = (_state)); \
		((mlmeext)->sitesurvey_res.next_state = (_state)); \
		rtw_mi_update_iface_status(&((container_of(mlmeext, _adapter, mlmeextpriv)->mlmepriv)), 0); \
		/* RTW_INFO("set_scan_state:%s\n", scan_state_str(_state)); */ \
		sitesurvey_set_offch_state(container_of(mlmeext, _adapter, mlmeextpriv), _state); \
	} while (0)

#define mlmeext_scan_next_state(mlmeext) ((mlmeext)->sitesurvey_res.next_state)
#define mlmeext_set_scan_next_state(mlmeext, _state) \
	do { \
		((mlmeext)->sitesurvey_res.next_state = (_state)); \
		/* RTW_INFO("set_scan_next_state:%s\n", scan_state_str(_state)); */ \
	} while (0)

#ifdef CONFIG_SCAN_BACKOP
#define mlmeext_scan_backop_flags(mlmeext) ((mlmeext)->sitesurvey_res.backop_flags)
#define mlmeext_chk_scan_backop_flags(mlmeext, flags) ((mlmeext)->sitesurvey_res.backop_flags & (flags))
#define mlmeext_assign_scan_backop_flags(mlmeext, flags) \
	do { \
		((mlmeext)->sitesurvey_res.backop_flags = (flags)); \
		RTW_INFO("assign_scan_backop_flags:0x%02x\n", (mlmeext)->sitesurvey_res.backop_flags); \
	} while (0)

#define mlmeext_scan_backop_flags_sta(mlmeext) ((mlmeext)->sitesurvey_res.backop_flags_sta)
#define mlmeext_chk_scan_backop_flags_sta(mlmeext, flags) ((mlmeext)->sitesurvey_res.backop_flags_sta & (flags))
#define mlmeext_assign_scan_backop_flags_sta(mlmeext, flags) \
	do { \
		((mlmeext)->sitesurvey_res.backop_flags_sta = (flags)); \
	} while (0)
#else
#define mlmeext_scan_backop_flags(mlmeext) (0)
#define mlmeext_chk_scan_backop_flags(mlmeext, flags) (0)
#define mlmeext_assign_scan_backop_flags(mlmeext, flags) do {} while (0)

#define mlmeext_scan_backop_flags_sta(mlmeext) (0)
#define mlmeext_chk_scan_backop_flags_sta(mlmeext, flags) (0)
#define mlmeext_assign_scan_backop_flags_sta(mlmeext, flags) do {} while (0)
#endif /* CONFIG_SCAN_BACKOP */

#if defined(CONFIG_SCAN_BACKOP) && defined(CONFIG_AP_MODE)
#define mlmeext_scan_backop_flags_ap(mlmeext) ((mlmeext)->sitesurvey_res.backop_flags_ap)
#define mlmeext_chk_scan_backop_flags_ap(mlmeext, flags) ((mlmeext)->sitesurvey_res.backop_flags_ap & (flags))
#define mlmeext_assign_scan_backop_flags_ap(mlmeext, flags) \
	do { \
		((mlmeext)->sitesurvey_res.backop_flags_ap = (flags)); \
	} while (0)
#else
#define mlmeext_scan_backop_flags_ap(mlmeext) (0)
#define mlmeext_chk_scan_backop_flags_ap(mlmeext, flags) (0)
#define mlmeext_assign_scan_backop_flags_ap(mlmeext, flags) do {} while (0)
#endif /* defined(CONFIG_SCAN_BACKOP) && defined(CONFIG_AP_MODE) */

#if defined(CONFIG_SCAN_BACKOP) && defined(CONFIG_RTW_MESH)
#define mlmeext_scan_backop_flags_mesh(mlmeext) ((mlmeext)->sitesurvey_res.backop_flags_mesh)
#define mlmeext_chk_scan_backop_flags_mesh(mlmeext, flags) ((mlmeext)->sitesurvey_res.backop_flags_mesh & (flags))
#define mlmeext_assign_scan_backop_flags_mesh(mlmeext, flags) \
	do { \
		((mlmeext)->sitesurvey_res.backop_flags_mesh = (flags)); \
	} while (0)
#else
#define mlmeext_scan_backop_flags_mesh(mlmeext) (0)
#define mlmeext_chk_scan_backop_flags_mesh(mlmeext, flags) (0)
#define mlmeext_assign_scan_backop_flags_mesh(mlmeext, flags) do {} while (0)
#endif /* defined(CONFIG_SCAN_BACKOP) && defined(CONFIG_RTW_MESH) */

u32 rtw_scan_timeout_decision(_adapter *padapter);

void init_mlme_default_rate_set(_adapter *padapter);
int init_mlme_ext_priv(_adapter *padapter);
int init_hw_mlme_ext(_adapter *padapter);
void free_mlme_ext_priv(struct mlme_ext_priv *pmlmeext);
extern struct xmit_frame *alloc_mgtxmitframe(struct xmit_priv *pxmitpriv);
struct xmit_frame *alloc_mgtxmitframe_once(struct xmit_priv *pxmitpriv);

/* void fill_fwpriv(_adapter * padapter, struct fw_priv *pfwpriv); */
u8 judge_network_type(_adapter *padapter, unsigned char *rate, int ratelen);
void get_rate_set(_adapter *padapter, unsigned char *pbssrate, int *bssrate_len);
void set_mcs_rate_by_mask(u8 *mcs_set, u32 mask);
void UpdateBrateTbl(_adapter *padapter, u8 *mBratesOS);
void UpdateBrateTblForSoftAP(u8 *bssrateset, u32 bssratelen);
void change_band_update_ie(_adapter *padapter, WLAN_BSSID_EX *pnetwork, u8 ch);

void Set_MSR(_adapter *padapter, u8 type);


void rtw_set_external_auth_status(_adapter *padapter, const void *data, int len);

u8 rtw_get_oper_ch(_adapter *adapter);
void rtw_set_oper_ch(_adapter *adapter, u8 ch);
u8 rtw_get_oper_bw(_adapter *adapter);
void rtw_set_oper_bw(_adapter *adapter, u8 bw);
u8 rtw_get_oper_choffset(_adapter *adapter);
void rtw_set_oper_choffset(_adapter *adapter, u8 offset);
int rtw_get_oper_bchbw_by_hwband(struct dvobj_priv *dvobj, enum phl_band_idx band_idx
	, enum band_type *band, u8 *ch, u8 *bw, u8 *offset);
systime rtw_get_on_oper_ch_time(_adapter *adapter);
systime rtw_get_on_cur_ch_time(_adapter *adapter);

RTW_FUNC_2G_5G_ONLY void set_channel_bwmode(_adapter *padapter, unsigned char channel, unsigned char channel_offset, unsigned short bwmode);
void set_bch_bwmode(_adapter *padapter,
	struct _ADAPTER_LINK *padapter_link,
	enum band_type band,
	unsigned char channel,
	unsigned char channel_offset,
	unsigned short bwmode,
	u8 do_rfk);

void csa_timer_hdl(void *FunctionContext);

unsigned int decide_wait_for_beacon_timeout(unsigned int bcn_interval);

void _clear_cam_entry(_adapter *padapter, u8 entry);
void write_cam_from_cache(_adapter *adapter, u8 id);
void rtw_sec_cam_swap(_adapter *adapter, u8 cam_id_a, u8 cam_id_b);
void rtw_clean_dk_section(_adapter *adapter);
void rtw_clean_hw_dk_cam(_adapter *adapter);

/* modify both HW and cache */
void write_cam(_adapter *padapter, u8 id, u16 ctrl, u8 *mac, u8 *key, bool ext);
void clear_cam_entry(_adapter *padapter, u8 id);

/* modify cache only */
void write_cam_cache(_adapter *adapter, u8 id, u16 ctrl, u8 *mac, u8 *key);
void clear_cam_cache(_adapter *adapter, u8 id);

void invalidate_cam_all(_adapter *padapter);

void flush_all_cam_entry(_adapter *padapter);

BOOLEAN IsLegal5GChannel(PADAPTER Adapter, u8 channel);

void site_survey(_adapter *padapter, u8 survey_channel, RT_SCAN_TYPE ScanType);
u8 collect_bss_info(_adapter *padapter, union recv_frame *precv_frame, WLAN_BSSID_EX *bssid);
void update_network(WLAN_BSSID_EX *dst, WLAN_BSSID_EX *src, _adapter *padapter, bool update_ie);

u8 *get_my_bssid(WLAN_BSSID_EX *pnetwork);
u16 get_beacon_interval(WLAN_BSSID_EX *bss);

int is_client_associated_to_ap(_adapter *padapter);
int is_client_associated_to_ibss(_adapter *padapter);
int is_IBSS_empty(_adapter *padapter);

unsigned char check_assoc_AP(u8 *pframe, uint len);
void get_assoc_AP_Vendor(char *vendor, u8 assoc_AP_vendor);
#ifdef CONFIG_RTS_FULL_BW
void rtw_parse_sta_vendor_ie_8812(_adapter *adapter, struct sta_info *sta, u8 *tlv_ies, u16 tlv_ies_len);
#endif/*CONFIG_RTS_FULL_BW*/
#ifdef CONFIG_80211AC_VHT
void get_vht_bf_cap(u8 *pframe, uint len, struct vht_bf_cap *bf_cap);
#endif

int WMM_param_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs	pIE);
#ifdef CONFIG_WFD
void rtw_process_wfd_ie(_adapter *adapter, u8 *ie, u8 ie_len, const char *tag);
void rtw_process_wfd_ies(_adapter *adapter, u8 *ies, u8 ies_len, const char *tag);
#endif
void WMMOnAssocRsp(_adapter *padapter);
#ifdef CONFIG_RTW_TOKEN_BASED_XMIT
u8 rtw_is_tbtx_capabilty(u8 *p, u8 len);
#endif

void HT_caps_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
#ifdef ROKU_PRIVATE
void HT_caps_handler_infra_ap(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
#endif
void HT_info_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void HTOnAssocRsp(_adapter *padapter);

#ifdef ROKU_PRIVATE
void Supported_rate_infra_ap(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void Extended_Supported_rate_infra_ap(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
#endif

void ERP_IE_handler(_adapter *padapter, PNDIS_802_11_VARIABLE_IEs pIE);
void VCS_update(_adapter *padapter, struct sta_info *psta);
void	update_ldpc_stbc_cap(struct sta_info *psta);

#ifdef CONFIG_CHECK_SPECIFIC_IE_CONTENT
bool rtw_validate_value(u16 EID, u8 *p, u16 len);
#endif /* CONFIG_CHECK_SPECIFIC_IE_CONTENT */

bool is_hidden_ssid(char *ssid, int len);
bool hidden_ssid_ap(WLAN_BSSID_EX *snetwork);
void rtw_absorb_ssid_ifneed(_adapter *padapter, WLAN_BSSID_EX *bssid, u8 *pframe);

int rtw_get_bcn_keys(_adapter *adapter, u8 *whdr, u32 flen, struct beacon_keys *bcn_keys);
int rtw_get_bcn_keys_from_bss(WLAN_BSSID_EX *bss, struct beacon_keys *bcn_keys);
int rtw_update_bcn_keys_of_network(struct wlan_network *network);

int check_ielen(u8 *start, uint len);
int validate_bcn_and_probe_rsp_len(u8 *pframe, uint len);
void rtw_dump_bcn_keys(void *sel, struct beacon_keys *recv_beacon);
void rtw_bcn_key_err_fix(struct beacon_keys *cur, struct beacon_keys *recv);
bool rtw_bcn_key_compare(struct beacon_keys *cur, struct beacon_keys *recv);
int rtw_check_bcn_info(ADAPTER *Adapter, u8 *pframe, u32 packet_len);
void update_beacon_info(_adapter *padapter, u8 *pframe, uint len, struct sta_info *psta);
#if CONFIG_DFS
void process_csa_ie(_adapter *padapter, u8 *ies, uint ies_len);
#endif
void update_capinfo(PADAPTER Adapter, u16 updateCap);
void update_wireless_mode(_adapter *padapter);
void update_tx_basic_rate(_adapter *padapter, u8 modulation);
void update_sta_basic_rate(struct sta_info *psta, u8 wireless_mode);
int rtw_ies_get_supported_rate(u8 *ies, uint ies_len, u8 *rate_set, u8 *rate_num);
int rtw_elems_get_supported_rate(struct rtw_ieee802_11_elems *elems, u8 *rate_set, u8 *rate_num);

/* for sta/adhoc mode */
void update_sta_info(_adapter *padapter, struct sta_info *psta);
unsigned int update_basic_rate(unsigned char *ptn, unsigned int ptn_sz);
unsigned int update_supported_rate(unsigned char *ptn, unsigned int ptn_sz);
void Update_RA_Entry(_adapter *padapter, struct sta_info *psta);
void set_sta_rate(_adapter *padapter, struct sta_info *psta);

unsigned int receive_disconnect(_adapter *padapter, unsigned char *MacAddr, unsigned short reason, u8 locally_generated);

unsigned char get_highest_rate_idx(u64 mask);
unsigned char get_lowest_rate_idx_ex(u64 mask, int start_bit);
#define get_lowest_rate_idx(mask) get_lowest_rate_idx_ex(mask, 0)

int support_short_GI(_adapter *padapter, struct HT_caps_element *pHT_caps, u8 bwmode);
unsigned int is_ap_in_tkip(_adapter *padapter);
unsigned int is_ap_in_wep(_adapter *padapter);
unsigned int should_forbid_n_rate(_adapter *padapter);

enum eap_type parsing_eapol_packet(_adapter *padapter, u8 *key_payload, struct sta_info *psta, u8 trx_type);

bool _rtw_camctl_chk_cap(_adapter *adapter, u8 cap);
void _rtw_camctl_set_flags(_adapter *adapter, u32 flags);
void rtw_camctl_set_flags(_adapter *adapter, u32 flags);
void _rtw_camctl_clr_flags(_adapter *adapter, u32 flags);
void rtw_camctl_clr_flags(_adapter *adapter, u32 flags);
bool _rtw_camctl_chk_flags(_adapter *adapter, u32 flags);

struct sec_cam_bmp;
void dump_sec_cam_map(void *sel, struct sec_cam_bmp *map, u8 max_num);
void rtw_sec_cam_map_set(struct sec_cam_bmp *map, u8 id);
void rtw_sec_cam_map_clr_all(struct sec_cam_bmp *map);

bool _rtw_camid_is_gk(_adapter *adapter, u8 cam_id);
bool rtw_camid_is_gk(_adapter *adapter, u8 cam_id);
s16 rtw_camid_search(_adapter *adapter, u8 *addr, s16 kid, s8 gk);
s16 rtw_camid_alloc(_adapter *adapter, struct sta_info *sta, u8 kid, u8 gk, bool ext_sec, bool *used);
void rtw_camid_free(_adapter *adapter, u8 cam_id);
u8 rtw_get_sec_camid(_adapter *adapter, u8 max_bk_key_num, u8 *sec_key_id);

struct macid_bmp;
struct macid_ctl_t;
bool _rtw_macid_ctl_chk_cap(_adapter *adapter, u8 cap);
void dump_macid_map(void *sel, struct macid_bmp *map, u8 max_num);
bool rtw_macid_is_set(struct macid_bmp *map, u8 id);
void rtw_macid_map_clr(struct macid_bmp *map, u8 id);
bool rtw_macid_is_used(struct macid_ctl_t *macid_ctl, u8 id);
bool rtw_macid_is_bmc(struct macid_ctl_t *macid_ctl, u8 id);
u8 rtw_macid_get_iface_bmp(struct macid_ctl_t *macid_ctl, u8 id);
bool rtw_macid_is_iface_shared(struct macid_ctl_t *macid_ctl, u8 id);
bool rtw_macid_is_iface_specific(struct macid_ctl_t *macid_ctl, u8 id, _adapter *adapter);
s8 rtw_macid_get_ch_g(struct macid_ctl_t *macid_ctl, u8 id);
void rtw_alloc_macid(_adapter *padapter, struct sta_info *psta);
void rtw_release_macid(_adapter *padapter, struct sta_info *psta);
u8 rtw_search_max_mac_id(_adapter *padapter);
u8 rtw_macid_ctl_set_h2c_msr(struct macid_ctl_t *macid_ctl, u8 id, u8 h2c_msr);
void rtw_macid_ctl_set_bw(struct macid_ctl_t *macid_ctl, u8 id, u8 bw);
void rtw_macid_ctl_set_vht_en(struct macid_ctl_t *macid_ctl, u8 id, u8 en);
void rtw_macid_ctl_set_rate_bmp0(struct macid_ctl_t *macid_ctl, u8 id, u32 bmp);
void rtw_macid_ctl_set_rate_bmp1(struct macid_ctl_t *macid_ctl, u8 id, u32 bmp);
#ifdef CONFIG_PROTSEL_MACSLEEP
void rtw_macid_ctl_init_sleep_reg(struct macid_ctl_t *macid_ctl, u16 reg_ctrl, u16 reg_info);
void rtw_macid_ctl_init_drop_reg(struct macid_ctl_t *macid_ctl, u16 reg_ctrl, u16 reg_info);
#else
void rtw_macid_ctl_init_sleep_reg(struct macid_ctl_t *macid_ctl, u16 m0, u16 m1, u16 m2, u16 m3);
void rtw_macid_ctl_init_drop_reg(struct macid_ctl_t *macid_ctl, u16 m0, u16 m1, u16 m2, u16 m3);
#endif
void rtw_macid_ctl_init(struct macid_ctl_t *macid_ctl);
void rtw_macid_ctl_deinit(struct macid_ctl_t *macid_ctl);
u8 rtw_iface_bcmc_id_get(_adapter *padapter);
void rtw_iface_bcmc_id_set(_adapter *padapter, u8 mac_id);
#if defined(DBG_CONFIG_ERROR_RESET) && defined(CONFIG_CONCURRENT_MODE)
void rtw_iface_bcmc_sec_cam_map_restore(_adapter *adapter);
#endif
bool rtw_bmp_is_set(const u8 *bmp, u8 bmp_len, u8 id);
void rtw_bmp_set(u8 *bmp, u8 bmp_len, u8 id);
void rtw_bmp_clear(u8 *bmp, u8 bmp_len, u8 id);
bool rtw_bmp_not_empty(const u8 *bmp, u8 bmp_len);
bool rtw_bmp_not_empty_exclude_bit0(const u8 *bmp, u8 bmp_len);

#ifdef CONFIG_AP_MODE
bool rtw_tim_map_is_set(_adapter *padapter, const u8 *map, u8 id);
void rtw_tim_map_set(_adapter *padapter, u8 *map, u8 id);
void rtw_tim_map_clear(_adapter *padapter, u8 *map, u8 id);
bool rtw_tim_map_anyone_be_set(_adapter *padapter, const u8 *map);
bool rtw_tim_map_anyone_be_set_exclude_aid0(_adapter *padapter, const u8 *map);
#endif /* CONFIG_AP_MODE */

u32 report_join_res(_adapter *padapter, int aid_res, u16 status);
void report_survey_event(_adapter *padapter, union recv_frame *precv_frame);
void report_surveydone_event(_adapter *padapter, bool acs);
u32 report_del_sta_event(_adapter *padapter, unsigned char *MacAddr, unsigned short reason, bool enqueue, u8 locally_generated);
void report_add_sta_event(_adapter *padapter, unsigned char *MacAddr);
bool rtw_port_switch_chk(_adapter *adapter);
void report_wmm_edca_update(_adapter *padapter);

void beacon_timing_control(_adapter *padapter);
u8 chk_bmc_sleepq_cmd(_adapter *padapter);
extern u8 set_tx_beacon_cmd(_adapter *padapter, u8 flags);
unsigned int setup_beacon_frame(_adapter *padapter, unsigned char *beacon_frame);
void update_mgnt_tx_rate(_adapter *padapter, u8 rate);
void update_monitor_frame_attrib(_adapter *padapter, struct pkt_attrib *pattrib);
#ifdef CONFIG_RTW_MGMT_QUEUE
void update_mgntframe_subtype(_adapter *padapter, struct xmit_frame *pmgntframe);
#endif
void update_mgntframe_attrib(_adapter *padapter, struct pkt_attrib *pattrib);
void update_mgntframe_attrib_addr(_adapter *padapter, struct xmit_frame *pmgntframe);
s32 dump_mgntframe(_adapter *padapter, struct xmit_frame *pmgntframe);
s32 dump_mgntframe_and_wait(_adapter *padapter, struct xmit_frame *pmgntframe, int timeout_ms);
s32 dump_mgntframe_and_wait_ack(_adapter *padapter, struct xmit_frame *pmgntframe);
s32 dump_mgntframe_and_wait_ack_timeout(_adapter *padapter, struct xmit_frame *pmgntframe, int timeout_ms);

#ifdef CONFIG_P2P
int get_reg_classes_full_count(struct p2p_channels *channel_list);
void issue_probersp_p2p(_adapter *padapter, unsigned char *da);
void issue_p2p_provision_request(_adapter *padapter, u8 *pssid, u8 ussidlen, u8 *pdev_raddr);
void issue_p2p_GO_request(_adapter *padapter, u8 *raddr);
void issue_probereq_p2p(_adapter *padapter, u8 *da);
int issue_probereq_p2p_ex(_adapter *adapter, u8 *da, int try_cnt, int wait_ms);
void issue_p2p_invitation_response(_adapter *padapter, u8 *raddr, u8 dialogToken, u8 success);
void issue_p2p_invitation_request(_adapter *padapter, u8 *raddr);
#endif /* CONFIG_P2P */
void issue_beacon(_adapter *padapter, int timeout_ms);
void issue_probersp(_adapter *padapter, unsigned char *da, u8 is_valid_p2p_probereq);
void _issue_assocreq(_adapter *padapter, u8 is_assoc);
void issue_assocreq(_adapter *padapter);
void issue_reassocreq(_adapter *padapter);
void issue_asocrsp(_adapter *padapter, unsigned short status, struct sta_info *pstat, int pkt_type);
void issue_auth(_adapter *padapter, struct sta_info *psta, unsigned short status);
void issue_probereq(_adapter *padapter, const NDIS_802_11_SSID *pssid, const u8 *da);
s32 issue_probereq_ex(_adapter *padapter, const NDIS_802_11_SSID *pssid, const u8 *da, u8 ch, bool append_wps, int try_cnt, int wait_ms);
int issue_nulldata(_adapter *padapter, unsigned char *da, unsigned int power_mode, int try_cnt, int wait_ms);
int issue_qos_nulldata(_adapter *padapter, unsigned char *da, u16 tid, u8 ps, int try_cnt, int wait_ms);
int issue_disasoc(_adapter *adapter, unsigned char *da, unsigned short reason);
int issue_deauth(_adapter *padapter, unsigned char *da, unsigned short reason);
int issue_deauth_ex(_adapter *padapter, u8 *da, unsigned short reason, int try_cnt, int wait_ms);
void issue_addba_req(_adapter *adapter, unsigned char *ra, u8 tid);
void issue_addba_rsp(_adapter *adapter, unsigned char *ra, u8 tid, u16 status, u8 size);
u8 issue_addba_rsp_wait_ack(_adapter *adapter, unsigned char *ra, u8 tid, u16 status, u8 size, int try_cnt, int wait_ms);
void issue_del_ba(_adapter *adapter, unsigned char *ra, u8 tid, u16 reason, u8 initiator);
int issue_del_ba_ex(_adapter *adapter, unsigned char *ra, u8 tid, u16 reason, u8 initiator, int try_cnt, int wait_ms);
void issue_action_BSSCoexistPacket(_adapter *padapter);

#ifdef CONFIG_IEEE80211W
void issue_action_SA_Query(_adapter *padapter, unsigned char *raddr, unsigned char action, unsigned short tid, u8 key_type);
int issue_deauth_11w(_adapter *padapter, unsigned char *da, unsigned short reason, u8 key_type);
#endif /* CONFIG_IEEE80211W */
int issue_action_SM_PS(_adapter *padapter ,  unsigned char *raddr , u8 NewMimoPsMode);
int issue_action_SM_PS_wait_ack(_adapter *padapter, unsigned char *raddr, u8 NewMimoPsMode, int try_cnt, int wait_ms);

unsigned int send_delba_sta_tid(_adapter *adapter, u8 initiator, struct sta_info *sta, u8 tid, u8 force);
unsigned int send_delba_sta_tid_wait_ack(_adapter *adapter, u8 initiator, struct sta_info *sta, u8 tid, u8 force);

unsigned int send_delba(_adapter *padapter, u8 initiator, u8 *addr);
#ifdef CONFIG_AP_MODE
unsigned int send_beacon(_adapter *padapter);
#endif
void start_clnt_assoc(_adapter *padapter);
void start_clnt_auth(_adapter *padapter);
void start_clnt_join(_adapter *padapter);
void start_create_ibss(_adapter *padapter);

#if defined(CONFIG_LAYER2_ROAMING) && defined(CONFIG_RTW_80211K)
void rtw_roam_nb_discover(_adapter *padapter, u8 bfroce);
#endif

unsigned int OnAssocReq(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAssocRsp(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnProbeReq(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnProbeRsp(_adapter *padapter, union recv_frame *precv_frame);
unsigned int DoReserved(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnBeacon(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAtim(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnDisassoc(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAuth(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAuthClient(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnDeAuth(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction(_adapter *padapter, union recv_frame *precv_frame);

unsigned int on_action_spct(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_qos(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_dls(_adapter *padapter, union recv_frame *precv_frame);
#ifdef CONFIG_RTW_WNM
unsigned int on_action_wnm(_adapter *adapter, union recv_frame *rframe);
#endif

#define RX_AMPDU_ACCEPT_INVALID 0xFF
#define RX_AMPDU_SIZE_INVALID 0xFF

enum rx_ampdu_reason {
	RX_AMPDU_DRV_FIXED = 1,
	RX_AMPDU_BTCOEX = 2, /* not used, because BTCOEX has its own variable management */
	RX_AMPDU_DRV_SCAN = 3,
};
u8 rtw_rx_ampdu_size(_adapter *adapter);
bool rtw_rx_ampdu_is_accept(_adapter *adapter);
bool rtw_rx_ampdu_set_size(_adapter *adapter, u8 size, u8 reason);
bool rtw_rx_ampdu_set_accept(_adapter *adapter, u8 accept, u8 reason);
u8 rx_ampdu_apply_sta_tid(_adapter *adapter, struct sta_info *sta, u8 tid, u8 accept, u8 size);
u8 rx_ampdu_size_sta_limit(_adapter *adapter, struct sta_info *sta);
u8 rx_ampdu_apply_sta(_adapter *adapter, struct sta_info *sta, u8 accept, u8 size);
u16 rtw_rx_ampdu_apply(_adapter *adapter);

unsigned int OnAction_back(_adapter *padapter, union recv_frame *precv_frame);
unsigned int on_action_public(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_ft(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_ht(_adapter *padapter, union recv_frame *precv_frame);
#ifdef CONFIG_IEEE80211W
unsigned int OnAction_sa_query(_adapter *padapter, union recv_frame *precv_frame);
#endif /* CONFIG_IEEE80211W */
unsigned int on_action_rm(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_wmm(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_vht(_adapter *padapter, union recv_frame *precv_frame);
unsigned int OnAction_p2p(_adapter *padapter, union recv_frame *precv_frame);
#ifdef CONFIG_RTW_TOKEN_BASED_XMIT
unsigned int OnAction_tbtx_token(_adapter *padapter, union recv_frame *precv_frame);
#endif

#ifdef CONFIG_RTW_TOKEN_BASED_XMIT
void rtw_issue_action_token_req(_adapter *padapter, struct sta_info *pstat);
void rtw_issue_action_token_rel(_adapter *padapter);
#endif

void mlmeext_joinbss_event_callback(_adapter *padapter, int join_res);
void mlmeext_sta_del_event_callback(_adapter *padapter);
void mlmeext_sta_add_event_callback(_adapter *padapter, struct sta_info *psta);

int rtw_get_rx_chk_limit(_adapter *adapter);
void rtw_set_rx_chk_limit(_adapter *adapter, int limit);
void linked_status_chk(_adapter *padapter, u8 from_timer);

#define rtw_get_bcn_cnt(adapter)	(adapter->mlmeextpriv.cur_bcn_cnt)
#define rtw_get_bcn_dtim_period(adapter)	(adapter->mlmeextpriv.dtim)
void rtw_collect_bcn_info(_adapter *adapter);

void _linked_info_dump(_adapter *padapter);

void survey_timer_hdl(void *ctx);
#ifdef CONFIG_RTW_REPEATER_SON
void rson_timer_hdl(void *ctx);
#endif
void link_timer_hdl(void *ctx);
void addba_timer_hdl(void *ctx);
#ifdef CONFIG_RTW_TOKEN_BASED_XMIT
void rtw_tbtx_xmit_timer_hdl(void *ctx);
void rtw_tbtx_token_dispatch_timer_hdl(void *ctx);
#endif
#ifdef CONFIG_IEEE80211W
void sa_query_timer_hdl(void *ctx);
#endif /* CONFIG_IEEE80211W */
#if 0
void reauth_timer_hdl(_adapter *padapter);
void reassoc_timer_hdl(_adapter *padapter);
#endif

#define set_survey_timer(mlmeext, ms) \
	do { \
		/*RTW_INFO("%s set_survey_timer(%p, %d)\n", __FUNCTION__, (mlmeext), (ms));*/ \
		_set_timer(&(mlmeext)->survey_timer, (ms)); \
	} while (0)

#define set_link_timer(mlmeext, ms) \
	do { \
		/*RTW_INFO("%s set_link_timer(%p, %d)\n", __FUNCTION__, (mlmeext), (ms));*/ \
		_set_timer(&(mlmeext)->link_timer, (ms)); \
	} while (0)

bool rtw_is_cck_rate(u8 rate);
bool rtw_is_ofdm_rate(u8 rate);
bool rtw_is_basic_rate_cck(u8 rate);
bool rtw_is_basic_rate_ofdm(u8 rate);
bool rtw_is_basic_rate_mix(u8 rate);

extern int cckrates_included(unsigned char *rate, int ratelen);
extern int cckratesonly_included(unsigned char *rate, int ratelen);

extern void process_addba_req(_adapter *padapter, u8 *paddba_req, u8 *addr);

extern void update_TSF(struct mlme_ext_priv *pmlmeext, u8 *pframe, uint len);
extern void correct_TSF(_adapter *padapter, u8 mlme_state);
#ifdef CONFIG_BCN_RECV_TIME
void rtw_rx_bcn_time_update(_adapter *adapter, uint bcn_len, u8 data_rate);
#endif
extern u8 traffic_status_watchdog(_adapter *padapter, u8 from_timer);

void rtw_process_bar_frame(_adapter *padapter, union recv_frame *precv_frame);
void rtw_join_done_chk_ch(_adapter *padapter, int join_res);

int rtw_chk_start_clnt_join(_adapter *padapter, u8 *ch, u8 *bw, u8 *offset);

#ifdef RTW_BUSY_DENY_SCAN
#ifndef BUSY_TRAFFIC_SCAN_DENY_PERIOD
#ifdef CONFIG_RTW_ANDROID
#ifdef CONFIG_PLATFORM_ARM_SUN8I
	#define BUSY_TRAFFIC_SCAN_DENY_PERIOD	8000
#else
	#define BUSY_TRAFFIC_SCAN_DENY_PERIOD	12000
#endif
#else /* !CONFIG_ANDROID */
#define BUSY_TRAFFIC_SCAN_DENY_PERIOD	16000
#endif /* !CONFIG_ANDROID */
#endif /* !BUSY_TRAFFIC_SCAN_DENY_PERIOD */
#endif /* RTW_BUSY_DENY_SCAN */

void rtw_leave_opch(_adapter *adapter);
void rtw_back_opch(_adapter *adapter);

u8 join_cmd_hdl(_adapter *padapter, u8 *pbuf);
u8 disconnect_hdl(_adapter *padapter, u8 *pbuf);
u8 createbss_hdl(_adapter *padapter, u8 *pbuf);
#ifdef CONFIG_AP_MODE
u8 stop_ap_hdl(_adapter *adapter);
#endif
#ifdef CONFIG_RTW_TOKEN_BASED_XMIT
u8 tx_control_hdl(_adapter *adapter);
#endif
u8 setopmode_hdl(_adapter *padapter, u8 *pbuf);
u8 sitesurvey_cmd_hdl(_adapter *padapter, u8 *pbuf);
u8 setauth_hdl(_adapter *padapter, u8 *pbuf);
u8 setkey_hdl(_adapter *padapter, u8 *pbuf);
u8 set_stakey_hdl(_adapter *padapter, u8 *pbuf);
u8 set_assocsta_hdl(_adapter *padapter, u8 *pbuf);
u8 del_assocsta_hdl(_adapter *padapter, u8 *pbuf);
u8 add_ba_hdl(_adapter *padapter, unsigned char *pbuf);
u8 add_ba_rsp_hdl(_adapter *padapter, unsigned char *pbuf);

void rtw_ap_wep_pk_setting(_adapter *adapter, struct sta_info *psta);

u8 mlme_evt_hdl(_adapter *padapter, unsigned char *pbuf);
u8 chk_bmc_sleepq_hdl(_adapter *padapter, unsigned char *pbuf);
u8 tx_beacon_hdl(_adapter *padapter, unsigned char *pbuf);
u8 rtw_set_chbw_hdl(_adapter *padapter, u8 *pbuf);
u8 rtw_iqk_hdl(_adapter *padapter, unsigned char *pbuf);
u8 led_blink_hdl(_adapter *padapter, unsigned char *pbuf);
u8 set_csa_hdl(_adapter *padapter, unsigned char *pbuf);	/* Kurt: Handling DFS channel switch announcement ie. */
u8 set_ap_csa_hdl(_adapter *adapter, unsigned char *pbuf);
u8 bcn_control_hdl(_adapter *adapter, unsigned char *pbuf);
void rtw_update_mlme(_adapter *adapter, u8 *frame, u32 frame_len);
u8 rtw_csa_sta_update_cap_hdl(_adapter *adapter, unsigned char *pbuf);
u8 tdls_hdl(_adapter *padapter, unsigned char *pbuf);
u8 run_in_thread_hdl(_adapter *padapter, u8 *pbuf);
u8 rtw_write_bcnlen_hdl(_adapter *padapter, u8 *pbuf);
u8 rtw_reqtxrpt_cmd_hdl(_adapter *adapter, u8 *pbuf);

int rtw_sae_preprocess(_adapter *adapter, const u8 *buf, u32 len, u8 tx);

u32 rtw_desc_rate_to_bitrate(u8 bw, u8 rate_idx, u8 sgi);
u8 *build_supported_op_class_ie(_adapter *padapter, u8 *pbuf, int *pktlen);

#ifdef CONFIG_RTW_MULTI_AP
u8 rtw_get_ch_utilization(_adapter *adapter);
void rtw_ch_util_rpt(_adapter *adapter);
#endif

#define GEN_MLME_EXT_HANDLER(cmd, callback_func)	{.cmd_hdl = cmd, .callback = callback_func},

struct rtw_cmd {
	u8(*cmd_hdl)(_adapter *padapter, u8 *pbuf);
	void (*callback)(_adapter  *padapter, struct cmd_obj *cmd);
};

#ifdef _RTW_CMD_C_
#ifdef CONFIG_RTW_MESH
extern u8 rtw_mesh_set_plink_state_cmd_hdl(_adapter *adapter, u8 *parmbuf);
#else
u8 rtw_mesh_set_plink_state_cmd_hdl(_adapter *adapter, u8 *parmbuf) { return H2C_CMD_FAIL; };
#endif

struct rtw_cmd wlancmds[] = {
	GEN_MLME_EXT_HANDLER(join_cmd_hdl, NULL) /*CMD_JOINBSS*/
	GEN_MLME_EXT_HANDLER(disconnect_hdl, rtw_disassoc_cmd_callback) /*CMD_DISCONNECT*/
	GEN_MLME_EXT_HANDLER(createbss_hdl, NULL) /*CMD_CREATE_BSS*/
	GEN_MLME_EXT_HANDLER(setopmode_hdl, NULL) /*CMD_SET_OPMODE*/
	GEN_MLME_EXT_HANDLER(sitesurvey_cmd_hdl, rtw_survey_cmd_callback) /*CMD_SITE_SURVEY*/
	GEN_MLME_EXT_HANDLER(setauth_hdl, NULL) /*CMD_SET_AUTH*/
	GEN_MLME_EXT_HANDLER(setkey_hdl, NULL) /*CMD_SET_KEY*/
	GEN_MLME_EXT_HANDLER(set_stakey_hdl, rtw_setstaKey_cmdrsp_callback) /*CMD_SET_STAKEY*/
	GEN_MLME_EXT_HANDLER(add_ba_hdl, NULL) /*CMD_ADD_BAREQ*/
	GEN_MLME_EXT_HANDLER(rtw_set_chbw_hdl, NULL) /*CMD_SET_CHANNEL*/
	GEN_MLME_EXT_HANDLER(tx_beacon_hdl, NULL) /*CMD_TX_BEACON*/
	GEN_MLME_EXT_HANDLER(mlme_evt_hdl, NULL) /*CMD_SET_MLME_EVT*/
	GEN_MLME_EXT_HANDLER(rtw_drvextra_cmd_hdl, NULL) /*CMD_SET_DRV_EXTRA*/
	GEN_MLME_EXT_HANDLER(rtw_set_chplan_hdl, NULL) /*CMD_SET_CHANPLAN*/
	GEN_MLME_EXT_HANDLER(led_blink_hdl, NULL) /*CMD_LEDBLINK*/
	GEN_MLME_EXT_HANDLER(set_csa_hdl, NULL) /*CMD_SET_CHANSWITCH*/
	GEN_MLME_EXT_HANDLER(tdls_hdl, NULL) /*CMD_TDLS*/
	GEN_MLME_EXT_HANDLER(chk_bmc_sleepq_hdl, NULL) /*CMD_CHK_BMCSLEEPQ*/
	GEN_MLME_EXT_HANDLER(run_in_thread_hdl, NULL) /*CMD_RUN_INTHREAD*/
	GEN_MLME_EXT_HANDLER(add_ba_rsp_hdl, NULL) /*CMD_ADD_BARSP*/
	GEN_MLME_EXT_HANDLER(rm_post_event_hdl, NULL) /*CMD_RM_POST_EVENT*/
	GEN_MLME_EXT_HANDLER(rtw_mesh_set_plink_state_cmd_hdl, NULL) /*CMD_SET_MESH_PLINK_STATE*/
	GEN_MLME_EXT_HANDLER(rtw_iqk_hdl, NULL) /*CMD_DO_IQK*/
	GEN_MLME_EXT_HANDLER(rtw_get_chplan_hdl, rtw_get_chplan_callback) /* CMD_GET_CHANPLAN */
	GEN_MLME_EXT_HANDLER(rtw_write_bcnlen_hdl, NULL) /* CMD_WRITE_BCN_LEN */
	GEN_MLME_EXT_HANDLER(set_ap_csa_hdl, NULL) /* CMD_AP_CHANSWITCH */
	GEN_MLME_EXT_HANDLER(rtw_reqtxrpt_cmd_hdl, NULL) /* CMD_REQ_TXRPT */
	GEN_MLME_EXT_HANDLER(bcn_control_hdl, NULL) /* CMD_BCN_CONTROL */
	GEN_MLME_EXT_HANDLER(rtw_csa_sta_update_cap_hdl, NULL) /* CMD_BCN_CONTROL */
};
#endif

struct rtw_evt_header {
	u8 id;
	u8 seq;
	u16 len;
};

enum rtw_event_id {
	EVT_SURVEY, /*0*/
	EVT_SURVEY_DONE, /*1*/
	EVT_JOINBSS, /*2*/
	EVT_ADD_STA, /*3*/
	EVT_DEL_STA, /*4*/
	EVT_WMM_UPDATE, /*5*/
#ifdef CONFIG_IEEE80211W
	EVT_TIMEOUT_STA, /*6*/
#endif /* CONFIG_IEEE80211W */
#ifdef CONFIG_RTW_80211R
	EVT_FT_REASSOC, /*7*/
#endif
	EVT_ID_MAX
};
#ifdef _RTW_MLME_EXT_C_
static struct rtw_event wlanevents[] = {
	{sizeof(struct survey_event), &rtw_survey_event_callback}, /*EVT_SURVEY*/
	{sizeof(struct surveydone_event), &rtw_surveydone_event_callback}, /*EVT_SURVEY_DONE*/
	{sizeof(struct joinbss_event), &rtw_joinbss_event_callback}, /*EVT_JOINBSS*/
	{sizeof(struct stassoc_event), &rtw_stassoc_event_callback}, /*EVT_ADD_STA*/
	{sizeof(struct stadel_event), &rtw_stadel_event_callback}, /*EVT_DEL_STA*/
	{sizeof(struct wmm_event), &rtw_wmm_event_callback}, /*EVT_WMM_UPDATE*/
	#ifdef CONFIG_IEEE80211W
	{sizeof(struct stadel_event), &rtw_sta_timeout_event_callback}, /*EVT_TIMEOUT_STA*/
	#endif /* CONFIG_IEEE80211W */
	#if defined(CONFIG_IOCTL_CFG80211) && defined(CONFIG_RTW_80211R)
	{sizeof(struct stassoc_event), &rtw_ft_reassoc_event_callback}, /*EVT_FT_REASSOC*/
	#endif
};
#endif/* _RTW_MLME_EXT_C_ */
#endif
