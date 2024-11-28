/**************************************************************************************************************************
 *
 * RTL88x2CU userspace explicit SU TX beamforming for monitor mode
 * https://github.com/libc0607/rtl88x2cu-20230728
 * 
 * This re-implementation from scratch is a replacement of core/rtw_beamforming.c and hal/rtl8822c/rtl8822c_phy.c
 * and then exposes APIs via procfs to userspace applications (injection-based app -- mainly wifibroadcast and its variants).
 *
 *
 * The macro CONFIG_BEAMFORMING should be disabled to avoid any conflict. Instead, use CONFIG_BEAMFORMING_MONITOR.
 * Some function from original implementation was renamed with "_monitor" suffix so you know what it was.
 *
 * This is a *simplified*, standalone version: 
 *  - Only VHT SU explicit beamforming is supported, and I've removed all HT/VHT-MU related code;
 *  - Not related to AP/STA. Designed to use with monitor mode;
 *  - Uses SU TXBF index 0 reg set in 88x2e.
 *
 *
 *
 *
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
 *************************************************************************************************************************/
#define _RTL8822C_BF_MONITOR_C_

#include <hal_data.h>		/* HAL_DATA_TYPE */
#include "../hal_halmac.h"	/* rtw_halmac_phy_power_switch() */
#include "rtl8822c.h"

#ifdef CONFIG_BEAMFORMING_MONITOR

u8 bf_monitor_send_vht_ndpa(PADAPTER adapter, 
	u8* ra, 	// bfee_macaddr
	u8* ta, 	// bfer_macaddr
	u16 p_aid, 	// 12-bit
	u8 g_id,	// 0/63
	u8 seq,		// 0~63 seq
	enum channel_width bw
)
{
	struct xmit_priv		*pxmitpriv;
	struct mlme_ext_priv		*pmlmeext;
	struct xmit_frame		*pmgntframe;
	struct mlme_ext_info		*pmlmeinfo;
	struct ndpa_sta_info_monitor	sta_info;
	struct pkt_attrib		*attrib;
	struct rtw_ieee80211_hdr	*pwlanhdr;
	u8 *pframe;
	enum MGN_RATE txrate;
	u16 duration = 0;
	u8 aSifsTime = 0;
	u8 sequence = 0;

	pxmitpriv = &adapter->xmitpriv;
	pmlmeext = &adapter->mlmeextpriv;
	pmlmeinfo = &pmlmeext->mlmext_info;
	pmgntframe = alloc_mgtxmitframe(pxmitpriv);
	if (!pmgntframe) {
		RTW_ERR("%s: alloc mgnt frame fail!\n", __FUNCTION__);
		return _FALSE;
	}

	attrib = &pmgntframe->attrib;
	update_mgntframe_attrib(adapter, attrib);
	attrib->type = WIFI_MGT_TYPE;
	attrib->subtype = WIFI_NDPA;
	attrib->bwmode = bw;
	attrib->qsel = QSLT_MGNT;
	attrib->rate = (u8)ODM_MGN_VHT2SS_MCS0;
	attrib->bf_pkt_type = 0;
	attrib->txbf_g_id = g_id;
	attrib->txbf_p_aid = p_aid;
	attrib->retry_ctrl = _FALSE;

	_rtw_memset(pmgntframe->buf_addr, 0, TXDESC_OFFSET + WLANHDR_OFFSET);
	pframe = pmgntframe->buf_addr + TXDESC_OFFSET;
	pwlanhdr = (struct rtw_ieee80211_hdr *)pframe;
	pwlanhdr->frame_ctl = 0;
	set_frame_sub_type(pframe, attrib->subtype);
	
	aSifsTime = 16;
	duration = 2 * aSifsTime + 44;
	if (bw == CHANNEL_WIDTH_80)
		duration += 40;
	else if (bw == CHANNEL_WIDTH_40)
		duration += 87;
	else
		duration += 180;
	set_duration(pframe, duration);

	/* Receiver Address (bfee)*/
	_rtw_memcpy(pwlanhdr->addr1, ra, ETH_ALEN);
	/* Transmitter Address (bfer)*/
	_rtw_memcpy(pwlanhdr->addr2, ta, ETH_ALEN);

	/* Sounding Sequence, bit0~1 is reserved */
	sequence = (seq << 2) & 0x3f;
	_rtw_memcpy(pframe + 16, &sequence, 1);

	/* STA Info */
	sta_info.aid = p_aid;
	sta_info.feedback_type = 0;	/* "Feedback Type" set to 0 for SU */
	sta_info.nc_index = 1; /* "Nc Index" reserved if the Feedback Type field indicates SU */
	_rtw_memcpy(pframe + 17, (u8 *)&sta_info, 2);

	attrib->pktlen = 19;
	attrib->last_txcmdsz = attrib->pktlen;

	dump_mgntframe(adapter, pmgntframe);

	return _TRUE;
}

// only called when local act as beamformer
void bf_monitor_phydm_txbf_rfmode_8822c(PADAPTER adapter, u8 su_bfee_cnt)
{
	struct dm_struct *dm = (struct dm_struct *)GET_PDM_ODM(adapter);
	u8 i;

	if (dm->rf_type == RF_1T1R)
		return;
	RTW_INFO("%s: bfee_cnt=%u\n", __FUNCTION__, su_bfee_cnt);	
	if (su_bfee_cnt > 0 ) {
		/*Path A ==================*/
		/*RF mode table write enable*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, BIT(19), 0x1);
		/*Select RX mode*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x33, 0xF, 3);
		/*Set Table data*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x3e, 0x3, 0x2);
		/*Set Table data*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x3f, 0xfffff, 0x65AFF);
		/*RF mode table write disable*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, BIT(19), 0x0);
		/*Path B ==================*/
		/*RF mode table write enable*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, BIT(19), 0x1);
		/*Select RX mode*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x33, 0xF, 3);
		/*Set Table data*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x3f, 0xfffff, 0x996BF);
		/*Select Standby mode*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x33, 0xF, 1);
		/*Set Table data*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x3f, 0xfffff, 0x99230);
		/*RF mode table write disable*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, BIT(19), 0x0);
	} else {
	        // I don't know what here doing, but just fill in the initial values before operation
	  	/*Path A ==================*/
		/*RF mode table write enable*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, BIT(19), 0x1);
		/*Select RX mode*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x33, 0xF, 0);
		/*Set Table data*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x3e, 0x3, 0x3);
		/*Set Table data*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x3f, 0xfffff, 0x18);
		/*RF mode table write disable*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, BIT(19), 0x0);
                /*Path B ==================*/
		/*RF mode table write enable*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, BIT(19), 0x1);
		/*Select RX mode*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x33, 0xF, 0);
		/*Set Table data*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x3f, 0xfffff, 0x18);
		/*RF mode table write disable*/
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, BIT(19), 0x0);
	}

	/*@if Nsts > Nc, don't apply V matrix*/
	odm_set_bb_reg(dm, R_0x1e24, BIT(11), 1);

	if (su_bfee_cnt > 0) {
		/*@enable BB TxBF ant mapping register*/
		odm_set_bb_reg(dm, R_0x1e24, BIT(28) | BIT29, 0x2);
		odm_set_bb_reg(dm, R_0x1e24, BIT(30), 1);

		/* logic mapping */
		/* TX BF logic map and TX path en for Nsts = 1~2 */
		odm_set_bb_reg(dm, R_0x820, 0xff, 0x33);
		odm_set_bb_reg(dm, R_0x1e2c, 0xffff, 0x404);
		odm_set_bb_reg(dm, R_0x820, 0xffff0000, 0x33);
		odm_set_bb_reg(dm, R_0x1e30, 0xffff, 0x404);
	} else {
		/*@Disable BB TxBF ant mapping register*/
		odm_set_bb_reg(dm, R_0x1e24, BIT(28) | BIT29, 0x0);
		odm_set_bb_reg(dm, R_0x1e24, BIT(31), 0);
		/*@1SS~2ss A, AB*/
		odm_set_bb_reg(dm, R_0x820, 0xff, 0x31);
		odm_set_bb_reg(dm, R_0x1e2c, 0xffff, 0x400);
		// I don't know what here doing, but just fill in the initial values before operation
		odm_set_bb_reg(dm, R_0x820, 0xffff0000, 0x1111);
		odm_set_bb_reg(dm, R_0x1e30, 0xffff, 0xe4e4);
	}
}

void bf_monitor_sounding_config(PADAPTER adapter, 
        u8 bf_en, 
	enum channel_width bw
)
{
	u32 txbf_ctrl;

	txbf_ctrl = rtw_read32(adapter, REG_TXBF_CTRL_8822C);
	txbf_ctrl &= ~(BIT_R_TXBF0_20M_8822C|BIT_R_TXBF0_40M_8822C|BIT_R_TXBF0_80M_8822C);
        if (bf_en) {
              switch (bw) {
	      default:
	      case CHANNEL_WIDTH_80:
		      txbf_ctrl |= BIT_R_TXBF0_80M_8822C;
		      fallthrough;
	      case CHANNEL_WIDTH_40:
		      txbf_ctrl |= BIT_R_TXBF0_40M_8822C;
		      fallthrough;
	      case CHANNEL_WIDTH_20:
		      txbf_ctrl |= BIT_R_TXBF0_20M_8822C;
		      break;
	      }
	      txbf_ctrl |= BIT_DIS_NDP_BFEN_8822C;
        }
	rtw_write32(adapter, REG_TXBF_CTRL_8822C, txbf_ctrl);
}

// local: bfee, remote: bfer
void bf_monitor_config_beamformer(PADAPTER adapter,
	u8* bfer_macaddr,
	u8 g_id	
)
{
	/* Beamforming */
	u8 nc_index = 0, nr_index = 0;
	u8 grouping = 0, codebookinfo = 0, coefficientsize = 0;
	u32 addr_bfer_info, addr_csi_rpt;
	u32 csi_param;
	u32 tmp6dc = 0;
	u8 csi_rsc = 0x0;
	/* Misc */
	u8 i;

	RTW_INFO("%s: Config SU BFer entry HW setting\n", __FUNCTION__);

	addr_bfer_info = REG_ASSOCIATED_BFMER0_INFO_8822C;
	addr_csi_rpt = REG_TX_CSI_RPT_PARAM_BW20_8822C;

	/* Sounding protocol control */
	rtw_write8(adapter, REG_SND_PTCL_CTRL_8822C, 0xDB);

	/* MAC address/Partial AID of Beamformer */
	// I know, but ... where's the p_aid? 
	for (i = 0; i < ETH_ALEN; i++)
		rtw_write8(adapter, addr_bfer_info+i, bfer_macaddr[i]);

	/* CSI report parameters of Beamformer */
	nc_index = GET_HAL_RX_NSS(adapter)-1;
	nr_index = 1;		// Number of Sounding Dimensions
	grouping = 0;		// no grouping
	codebookinfo = 1; 	// vht only
	coefficientsize = 3;
	csi_param = (u16)((coefficientsize<<10)|(codebookinfo<<8)|(grouping<<6)|(nr_index<<3)|(nc_index));
	rtw_write16(adapter, addr_csi_rpt, csi_param);
	RTW_INFO("%s: nc=%d nr=%d group=%d codebookinfo=%d coefficientsize=%d csi=0x%04x\n",
		 __FUNCTION__, nc_index, nr_index, grouping, codebookinfo, coefficientsize, csi_param);

	/* ndp_rx_standby_timer */
	rtw_write8(adapter, REG_SND_PTCL_CTRL_8822C+3, 0x70);
	
	// BIT_CSI_GID_SEL_8822C
	tmp6dc = (rtw_read32(adapter, REG_BBPSF_CTRL_8822C) | BIT(30) | (csi_rsc << 13));
	tmp6dc = (g_id==63)? (tmp6dc|BIT(12)): (tmp6dc&(~BIT(12)));
	rtw_write32(adapter, REG_BBPSF_CTRL_8822C, tmp6dc);
	
	rtw_write32(adapter, REG_CSI_RRSR_8822C, 0x550);
}

// local: bfer, remote: bfee
void bf_monitor_config_beamformee(PADAPTER adapter, 
		u16 p_aid	// bfee's, 12bit
)
{
	struct mlme_priv *mlme;
	u32 val32;

	RTW_INFO("%s: Config SU BFee entry HW setting\n", __FUNCTION__);
	mlme = &adapter->mlmepriv;

	bf_monitor_phydm_txbf_rfmode_8822c(adapter, 1);

	/* P_AID of Beamformee & enable NDPA transmission & enable NDPA interrupt */
	val32 = rtw_read32(adapter, REG_TXBF_CTRL_8822C);
	val32 = BIT_SET_R_TXBF0_AID_8822C(val32, p_aid);
	val32 &= ~(BIT_R_TXBF0_20M_8822C | BIT_R_TXBF0_40M_8822C | BIT_R_TXBF0_80M_8822C);
	val32 |= BIT_R_EN_NDPA_INT_8822C | BIT_USE_NDPA_PARAMETER_8822C | BIT_R_ENABLE_NDPA_8822C;
	rtw_write32(adapter, REG_TXBF_CTRL_8822C, val32);

	/* CSI report parameters of Beamformee */
	val32 = rtw_read32(adapter, REG_ASSOCIATED_BFMEE_SEL_8822C);
	val32 = BIT_SET_AID0_8822C(val32, p_aid);
	val32 |= BIT_TXUSER_ID0_8822C;

	/* unknown? */
	val32 &= 0x03FFFFFF;
	val32 |= 0x60000000;

	rtw_write32(adapter, REG_ASSOCIATED_BFMEE_SEL_8822C, val32);
}

void bf_monitor_sounding_reset(PADAPTER adapter)
{
	u8 i;
	u32 mu_tx_ctl;

	rtw_write8(adapter, REG_TXBF_CTRL_8822C+3, 0);

	mu_tx_ctl = rtw_read32(adapter, REG_MU_TX_CTL_8822C);
	for (i=0; i<6; i++) {
		mu_tx_ctl = BIT_SET_R_MU_TAB_SEL_8822C(mu_tx_ctl, i);
		rtw_write32(adapter, REG_MU_TX_CTL_8822C, mu_tx_ctl);
		/* set MU STA gid valid table */
		rtw_write32(adapter, REG_MU_STA_GID_VLD_8822C, 0);
	}

	/* Disable TxMU PPDU */
	mu_tx_ctl &= ~BIT_R_EN_MU_MIMO_8822C;
	rtw_write32(adapter, REG_MU_TX_CTL_8822C, mu_tx_ctl);
}

void bf_monitor_reset_beamformer(PADAPTER adapter)
{
	rtw_write32(adapter, REG_ASSOCIATED_BFMER0_INFO_8822C, 0);
	rtw_write16(adapter, REG_ASSOCIATED_BFMER0_INFO_8822C+4, 0);
	rtw_write16(adapter, REG_TX_CSI_RPT_PARAM_BW20_8822C, 0);

	RTW_INFO("%s: Clear SU BFer entry 0 HW setting\n", __FUNCTION__);
}

void bf_monitor_reset_beamformee(PADAPTER adapter)
{

	u32 txbf_ctrl, bfmee_sel;

	/* Force disable sounding config */
	bf_monitor_sounding_config(adapter, 0, CHANNEL_WIDTH_80);
	
	/* clear P_AID */
	txbf_ctrl = rtw_read32(adapter, REG_TXBF_CTRL_8822C);
	bfmee_sel = rtw_read32(adapter, REG_ASSOCIATED_BFMEE_SEL_8822C);

	txbf_ctrl = BIT_SET_R_TXBF0_AID_8822C(txbf_ctrl, 0);
	txbf_ctrl &= ~(BIT_R_TXBF0_20M_8822C | BIT_R_TXBF0_40M_8822C | BIT_R_TXBF0_80M_8822C);

	bfmee_sel = BIT_SET_AID0_8822C(bfmee_sel, 0);
	bfmee_sel &= ~BIT_TXUSER_ID0_8822C;

	txbf_ctrl |= BIT_R_EN_NDPA_INT_8822C | BIT_USE_NDPA_PARAMETER_8822C | BIT_R_ENABLE_NDPA_8822C;
	rtw_write32(adapter, REG_TXBF_CTRL_8822C, txbf_ctrl);
	rtw_write32(adapter, REG_ASSOCIATED_BFMEE_SEL_8822C, bfmee_sel);

	RTW_INFO("%s: Clear SU BFee entry 0 HW setting\n", __FUNCTION__);
}

void bf_monitor_phy_reset_rtl8822c(PADAPTER adapter)
{
	u8 i;
	u32 val32;

	RTW_INFO("%s\n", __FUNCTION__);
	
	/* Reset MU BFer entry setting */
	/* Clear validity of MU STA0 and MU STA1 */
	val32 = rtw_read32(adapter, REG_MU_TX_CTL_8822C);
	val32 = BIT_SET_R_MU_TABLE_VALID_8822C(val32, 0);
	rtw_write32(adapter, REG_MU_TX_CTL_8822C, val32);

	/* Reset SU BFer entry setting */
	rtw_write32(adapter, REG_ASSOCIATED_BFMER0_INFO_8822C, 0);
	rtw_write16(adapter, REG_ASSOCIATED_BFMER0_INFO_8822C+4, 0);
	rtw_write16(adapter, REG_TX_CSI_RPT_PARAM_BW20_8822C, 0);
	rtw_write32(adapter, REG_ASSOCIATED_BFMER1_INFO_8822C, 0);
	rtw_write16(adapter, REG_ASSOCIATED_BFMER1_INFO_8822C+4, 0);
	rtw_write16(adapter, REG_TX_CSI_RPT_PARAM_BW20_8822C+2, 0);

	/* Force disable sounding */
	bf_monitor_sounding_reset(adapter);

	/* Config RF mode */
	bf_monitor_phydm_txbf_rfmode_8822c(adapter, 0);

	/* Disable sending NDPA & BF-rpt-poll to all BFee */
	for (i=0; i < 6; i++)
		rtw_write16(adapter, REG_WMAC_ASSOCIATED_MU_BFMEE2_8822C+(i*2), 0);

	/* set validity of MU STA */
	rtw_write32(adapter, REG_MU_TX_CTL_8822C, 0);

	/* Reset SU BFee entry setting */
	/* SU BF0 and BF1 */
	val32 = BIT_R_EN_NDPA_INT_8822C | BIT_USE_NDPA_PARAMETER_8822C | BIT_R_ENABLE_NDPA_8822C;
	rtw_write32(adapter, REG_TXBF_CTRL_8822C, val32);
	rtw_write32(adapter, REG_ASSOCIATED_BFMEE_SEL_8822C, 0);

	//RTW_INFO("-%s\n", __FUNCTION__);
}

void bf_monitor_reg_init(PADAPTER adapter)
{
	u8 v8;
	u32 v32;

	v32 = rtw_read32(adapter, REG_MU_TX_CTL_8822C);
	/* Enable P1 aggr new packet according to P0 transfer time */
	v32 |= BIT_R_MU_P1_WAIT_STATE_EN_8822C;
	/* MU Retry Limit */
	v32 = BIT_SET_R_MU_RL_8822C(v32, 0xA);
	/* Disable Tx MU-MIMO until sounding done */
	v32 &= ~BIT_R_EN_MU_MIMO_8822C;
	/* Clear validity of MU STAs */
	v32 = BIT_SET_R_MU_TABLE_VALID_8822C(v32, 0);
	rtw_write32(adapter, REG_MU_TX_CTL_8822C, v32);

	/* MU-MIMO Option as default value */
	v8 = BIT_WMAC_TXMU_ACKPOLICY_8822C(3);
	v8 |= BIT_WMAC_TXMU_ACKPOLICY_EN_8822C;
	rtw_write8(adapter, REG_MU_BF_OPTION_8822C, v8);
	/* MU-MIMO Control as default value */
	rtw_write16(adapter, REG_WMAC_MU_BF_CTL_8822C, 0);

	/* Set MU NDPA rate & BW source */
	/* 0x42C[30] = 1 (0: from Tx desc, 1: from 0x45F) */
	v8 = rtw_read8(adapter, REG_TXBF_CTRL_8822C+3);
	v8 |= (BIT_USE_NDPA_PARAMETER_8822C >> 24);
	rtw_write8(adapter, REG_TXBF_CTRL_8822C+3, v8);
	/* 0x45F[7:0] = 0x10 (Rate=OFDM_6M, BW20) */
	rtw_write8(adapter, REG_NDPA_OPT_CTRL_8822C, 0x10);

	/* Temp Settings */
	/* STA2's CSI rate is fixed at 6M */
	v8 = rtw_read8(adapter, 0x6DF);
	v8 = (v8 & 0xC0) | 0x4;
	rtw_write8(adapter, 0x6DF, v8);
}

void bf_monitor_leave(PADAPTER adapter)
{
	rtw_write8(adapter, REG_SND_PTCL_CTRL_8822C, 0xD8);
	bf_monitor_reset_beamformer(adapter);
	bf_monitor_reset_beamformee(adapter);
}

void bf_monitor_init(PADAPTER adapter,
    u8* bfer_macaddr, 
    u8  bfer_g_id,
    u16 bfee_p_aid
)
{
    bf_monitor_reg_init(adapter);
    bf_monitor_sounding_config(adapter, 0, CHANNEL_WIDTH_80);
    bf_monitor_config_beamformer(adapter, bfer_macaddr, bfer_g_id);
    bf_monitor_config_beamformee(adapter, bfee_p_aid);
    RTW_INFO("%s: Init BF, BFer: %02x:%02x:%02x:%02x:%02x:%02x, BFer G_ID=%u, BFee P_AID=%u\n", __FUNCTION__,
        bfer_macaddr[0], bfer_macaddr[1], bfer_macaddr[2], bfer_macaddr[3], bfer_macaddr[4], bfer_macaddr[5],
        bfer_g_id, bfee_p_aid);
}

void bf_monitor_reset(PADAPTER adapter)
{
    bf_monitor_leave(adapter);
    bf_monitor_phy_reset_rtl8822c(adapter);
    
    RTW_INFO("%s: Clear SU BF setting\n", __FUNCTION__);
}

void bf_monitor_send(PADAPTER adapter, 
    u8* bfer_macaddr, 
    u8* bfee_macaddr, 
    u16 p_aid,          // Partial AID, 12-bit
    u8 g_id,            // Group ID, 0 or 63
    u8 seq,             // 0~63, 6-bit
    enum channel_width bw
)
{
    RTW_INFO("%s: Send %s NDPA frame from BFer (%02x:%02x:%02x:%02x:%02x:%02x) to BFee (%02x:%02x:%02x:%02x:%02x:%02x), P_AID=%u, G_ID=%u, seq=%u, BW=%d\n", __FUNCTION__,
        "VHT",
        bfer_macaddr[0], bfer_macaddr[1], bfer_macaddr[2], bfer_macaddr[3], bfer_macaddr[4], bfer_macaddr[5],
        bfee_macaddr[0], bfee_macaddr[1], bfee_macaddr[2], bfee_macaddr[3], bfee_macaddr[4], bfee_macaddr[5],
        p_aid, g_id, seq, bw);
        bf_monitor_send_vht_ndpa(adapter, bfee_macaddr, bfer_macaddr, p_aid, g_id, seq, bw);
}

// here cbr = Compressed Beamforming Report
// To convert the frame into V matrix -- use anything (e.g. Wireshark) to capture the packet, then search the Github
// e.g. Vito-Swift/dpkt-80211BeamformingReport, or watalabo/WiPiCap
// or start from IEEE Std 802.11™‐2020 "9.4.1.29 Compressed Beamforming Report field" and "19.3.12.3.6 Compressed beamforming feedback matrix"
// I won't do the V matrix conversion here, and I HATE LINEAR ALGEBRA
void bf_monitor_print_cbr(PADAPTER adapter, struct seq_file *m)
{
    HAL_DATA_TYPE	*pHalData;
    struct csi_rpt_monitor * csi;
    u8 bw, i, j;
    s16 tmp_snr;
    
    pHalData= GET_HAL_DATA(adapter);
	csi = &(pHalData->csi_rpt_monitor);
	if (csi->bw == CHANNEL_WIDTH_80)
	    bw = 80;
	else if (csi->bw == CHANNEL_WIDTH_40)
	    bw = 40;
	else 
	    bw = 20;

    RTW_PRINT_SEL(m, "Nc Index: %hhu Columns\n", (csi->nc)+1);
    RTW_PRINT_SEL(m, "Nr Index: %hhu Rows\n", (csi->nr)+1);
    RTW_PRINT_SEL(m, "Channel Width: %hhu MHz\n", bw);
    RTW_PRINT_SEL(m, "Grouping (Ng): %hhu\n", csi->ng);
    RTW_PRINT_SEL(m, "Codebook Information: %hhu\n", csi->codebook);
    RTW_PRINT_SEL(m, "Sounding Dialog Token: %hhu\n", csi->token);
    for (i=0; i<(csi->nc+1); i++) {
        // tmp_snr: 0.01dB unit
        tmp_snr = (csi->snr[i]*25) +2200;
        RTW_PRINT_SEL(m, "Average Signal to Noise Ratio (NDP's, seen from remote) - Stream %hhu: %d mBm\n", i, tmp_snr);
    }
    for (i=0; i<(csi->nc+1); i++) {
        RTW_PRINT_SEL(m, "CBR Frame RSSI %hhu: %d dBm, SNR %hhu: %d dB, EVM %hhu: -%d dB\n", i, csi->rx_pwr[i], i, csi->rx_snr[i], i, csi->rx_evm[i]);
    }
    RTW_PRINT_SEL(m, "CSI Matrix Len: %hu\n", csi->csi_matrix_len);
    /*
    RTW_PRINT_SEL(m, "CSI Matrix: \n");
    for (i=0; i<(csi->csi_matrix_len/16)+1; i++) {
        for (j=0; j<16; j++) {
            if (i*16+j == csi->csi_matrix_len)
                break;
            RTW_PRINT_SEL(m, "%02x ", csi->csi_matrix[i*16+j]);
        }
        RTW_PRINT_SEL(m, "\n");
    }
    RTW_PRINT_SEL(m, "\n");
    */
}

void bf_monitor_print_conf(PADAPTER adapter, struct seq_file *m)
{
    u32 i, txbf_ctrl, tmp6dc, addr_bfer_info, bfee_sel;
    u8 mac[ETH_ALEN];
    
    txbf_ctrl = rtw_read32(adapter, REG_TXBF_CTRL_8822C);
    
    addr_bfer_info = REG_ASSOCIATED_BFMER0_INFO_8822C;
    for (i = 0; i < ETH_ALEN; i++)
        mac[i] = rtw_read8(adapter, addr_bfer_info+i);
    tmp6dc = rtw_read32(adapter, REG_BBPSF_CTRL_8822C); // BIT(12)=63, ~BIT(12)=0

    bfee_sel = rtw_read32(adapter, REG_ASSOCIATED_BFMEE_SEL_8822C);
	
    RTW_PRINT_SEL(m, "Remote MAC address (When remote as beamformer): %02x:%02x:%02x:%02x:%02x:%02x\n", 
                                                            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    RTW_PRINT_SEL(m, "Remote G_ID (When remote as beamformer): %u\n", (tmp6dc&BIT(12))? 63: 0);
    RTW_PRINT_SEL(m, "REG_TXBF_CTRL_8822C.TXBF0_80M_8822C: %u\n", txbf_ctrl & BIT_R_TXBF0_80M_8822C? 1: 0);
    RTW_PRINT_SEL(m, "REG_TXBF_CTRL_8822C.TXBF0_40M_8822C: %u\n", txbf_ctrl & BIT_R_TXBF0_40M_8822C? 1: 0);
    RTW_PRINT_SEL(m, "REG_TXBF_CTRL_8822C.TXBF0_20M_8822C: %u\n", txbf_ctrl & BIT_R_TXBF0_20M_8822C? 1: 0);
    RTW_PRINT_SEL(m, "REG_TXBF_CTRL_8822C.AID0: %u\n", BIT_GET_R_TXBF0_AID_8822C(txbf_ctrl));
    RTW_PRINT_SEL(m, "REG_TXBF_CTRL_8822C.EN_NDPA_INT: %u\n", txbf_ctrl & BIT_R_EN_NDPA_INT_8822C? 1: 0);
    RTW_PRINT_SEL(m, "REG_TXBF_CTRL_8822C.USE_NDPA_PARAMETER: %u\n", txbf_ctrl & BIT_USE_NDPA_PARAMETER_8822C? 1: 0);
    RTW_PRINT_SEL(m, "REG_TXBF_CTRL_8822C.ENABLE_NDPA: %u\n", txbf_ctrl & BIT_R_ENABLE_NDPA_8822C? 1: 0);
    RTW_PRINT_SEL(m, "REG_ASSOCIATED_BFMEE_SEL_8822C.AID0: %u\n", BIT_GET_AID0_8822C(bfee_sel));

}

// Set REG_TXBF_CTRL_8822C.TXBF0_*0M_8822C according to CBR feedback
void bf_monitor_enable_txbf(PADAPTER adapter, bool en)
{
	HAL_DATA_TYPE *pHalData;
	struct csi_rpt_monitor *csi;
	
	pHalData = GET_HAL_DATA(adapter);
	csi = &(pHalData->csi_rpt_monitor);
        
        // Only set enable when we got any CSI matrix 
        if (csi->csi_matrix_len > 0 && en) {
            bf_monitor_sounding_config(adapter, 1, (enum channel_width)csi->bw);
        } else {
             bf_monitor_sounding_config(adapter, 0, CHANNEL_WIDTH_80);
        }
        
}

// RX hooks 

// called by recv_frame_monitor() when beamformer get "VHT compressed beamforming" packet (feedback) from beamformee
u32 bf_monitor_get_report_packet(PADAPTER adapter, union recv_frame *precv_frame)
{
	u32 ret = _SUCCESS;
	u8 *pframe;
	u32 frame_len;
	u8 *ta;
	u8 *frame_body;
	u8 category, action;
	u8 *pMIMOCtrlField;
	u8 *pCSIMatrix;
	u8 Nc = 0, Nr = 0, CH_W = 0, Ng = 0, CodeBook = 0;
	u16 CSIMatrixLen = 0;
	u32 i, j;
	struct csi_rpt_monitor * csi;
	HAL_DATA_TYPE	*pHalData;
	struct rtw_ieee80211_hdr_3addr * ieeehdr;
	u8 bfer_mac_readout[ETH_ALEN];
	
	pframe = precv_frame->u.hdr.rx_data;
	frame_len = precv_frame->u.hdr.len;
	frame_body = pframe + sizeof(struct rtw_ieee80211_hdr_3addr);
	category = frame_body[0];
	action = frame_body[1];
	pHalData= GET_HAL_DATA(adapter);
	csi = &(pHalData->csi_rpt_monitor);
	ieeehdr = (struct rtw_ieee80211_hdr_3addr *) precv_frame->u.hdr.rx_data;

	if (!((category == RTW_WLAN_CATEGORY_VHT) && (action == RTW_WLAN_ACTION_VHT_COMPRESSED_BEAMFORMING))) {
	        return ret;
	} 

	// CHECK THE CBR FRAME'S DEST ADDR!
	// Why didn't the stock driver check that? It'll report all CBR in air!
	if (_rtw_memcmp(adapter_mac_addr(adapter), ieeehdr->addr1, ETH_ALEN) == _FALSE) {
	        return ret;
	}

	pMIMOCtrlField = pframe + 26;
	csi->nc = (*pMIMOCtrlField) & 0x7;
	csi->nr = ((*pMIMOCtrlField) & 0x38) >> 3;
	csi->bw =  (((*pMIMOCtrlField) & 0xC0) >> 6);
	csi->ng = (*(pMIMOCtrlField+1)) & 0x3;
	csi->codebook = ((*(pMIMOCtrlField+1)) & 0x4) >> 2;
        csi->token =  ((*(pMIMOCtrlField+2)) & 0xfc) >> 2;
        for (i=0; i<Nc+1; i++) {
            csi->snr[i] = (*(pMIMOCtrlField+3+i));
            csi->rx_snr[i] = precv_frame->u.hdr.attrib.phy_info.rx_snr[i];
            csi->rx_pwr[i] = precv_frame->u.hdr.attrib.phy_info.rx_pwr[i];
            csi->rx_evm[i] = precv_frame->u.hdr.attrib.phy_info.rx_mimo_evm_dbm[i];
        }
	/*
	 * 24+(1+1+3)+2
	 * ==> MAC header+(Category+ActionCode+MIMOControlField)+SNR(Nc=2)
	 */
	CSIMatrixLen = frame_len - 26 - 3 - (Nc+1) - 4; // 4=crc
	csi->csi_matrix_len = CSIMatrixLen;

	// todo: bug here, and... it's useless
	//pCSIMatrix = pMIMOCtrlField + 3 + (Nc+1);
	// _rtw_memcpy(csi->csi_matrix, pCSIMatrix, CSIMatrixLen);

	RTW_INFO("BF_MONITOR %s: local is BFer (%02x:%02x:%02x:%02x:%02x:%02x), got compressed beamforming report from BFee (%02x:%02x:%02x:%02x:%02x:%02x), Nc=%d, Nr=%d, CH_W=%d, Ng=%d, CodeBook=%d\n",
	        __FUNCTION__,
	        ieeehdr->addr1[0], ieeehdr->addr1[1], ieeehdr->addr1[2], ieeehdr->addr1[3], ieeehdr->addr1[4], ieeehdr->addr1[5],
	        ieeehdr->addr2[0], ieeehdr->addr2[1], ieeehdr->addr2[2], ieeehdr->addr2[3], ieeehdr->addr2[4], ieeehdr->addr2[5],
	        csi->nc, csi->nr, csi->bw, csi->ng, csi->codebook);

	return ret;
}

// called by process_c2h_event()
void bf_monitor_c2h_snd_txbf(PADAPTER adapter, u8 *buf, u8 buf_len)
{
	u8 res;
	HAL_DATA_TYPE	*pHalData;
	
	pHalData= GET_HAL_DATA(adapter);
	
	res = C2H_SND_TXBF_GET_SND_RESULT(buf) ? _TRUE : _FALSE;
	
	if (res == _TRUE) {
	    RTW_INFO("+%s: sounding success!\n", __FUNCTION__);
        } else {
	    RTW_INFO("+%s: sounding fail!\n", __FUNCTION__);
        }
}

#endif /* CONFIG_BEAMFORMING_MONITOR */



