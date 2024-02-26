// 管理帧调用过程
static int ol_ath_mgmt_rx_event_handler(ol_scn_t sc, u_int8_t *data, u_int32_t datalen)
{	 
	ol_ath_soc_softc_t *soc = (ol_ath_soc_softc_t *) sc;
    struct ieee80211com *ic;
    struct ieee80211_frame *wh;
    uint8_t *bufp;
    uint32_t len;
    wbuf_t wbuf;
    struct mgmt_rx_event_params rx_event = {0};
    struct ol_ath_softc_net80211 *scn;
    struct wmi_unified *wmi_handle;
    struct wlan_objmgr_pdev *pdev;
    enum wmi_target_type wmi_tgt_type;
    QDF_STATUS status;
	// 从psoc_obj 得到 wmi_handle 句柄
	wmi_handle = lmac_get_wmi_hdl(soc->psoc_obj);
	// 从data里面 解压事件 存放到 rx_event里面
	wmi_extract_mgmt_rx_params(wmi_handle, data, &rx_event, &bufp);
	
	//根据id 得到当前 psoc 下面的 pdev
	pdev = wlan_objmgr_get_pdev_by_id(soc->psoc_obj, PDEV_UNIT(
                                        rx_event.pdev_id), WLAN_MLME_SB_ID);
	
	scn = lmac_get_pdev_feature_ptr(pdev);
	ic = &scn->sc_ic;
	
	
	 /* Calculate the RSSI for WMI NON_TLV targets in the host as FW does not
     * have the support to send the RSSI value to the host through
     * WMI_MGMT_RX_EVENTID.  rss值相关
     */

    status = ol_ath_get_wmi_target_type(soc, &wmi_tgt_type);
	if (wmi_tgt_type == WMI_NON_TLV_TARGET)
        rx_event.rssi = rx_event.snr + scn->chan_nf;

    len = roundup(rx_event.buf_len, sizeof(u_int32_t));
    
	// 分配数据缓存 
	wbuf =  wbuf_alloc(ic->ic_osdev, WBUF_RX_INTERNAL, len);
	wbuf_init(wbuf, rx_event.buf_len);
	wh = (struct ieee80211_frame *)wbuf_header(wbuf);
	OS_MEMCPY(wh, bufp, rx_event.buf_len);
	// 进行下步处理
    ol_ath_mgmt_handler(pdev, scn, wbuf, wh, rx_event, false);
    {
		static inline QDF_STATUS mgmt_txrx_rx_handler(struct wlan_objmgr_psoc *psoc, qdf_nbuf_t nbuf,void *params)
		{
			struct wlan_lmac_if_mgmt_txrx_rx_ops *mgmt_rx_ops;

			mgmt_rx_ops = wlan_lmac_if_get_mgmt_txrx_rx_ops(psoc);

			if (mgmt_rx_ops && mgmt_rx_ops->mgmt_rx_frame_handler)
				return mgmt_rx_ops->mgmt_rx_frame_handler(psoc, nbuf, params);
				{   // 函数原型如下
					QDF_STATUS tgt_mgmt_txrx_rx_frame_handler(struct wlan_objmgr_psoc *psoc,qdf_nbuf_t buf,struct mgmt_rx_event_params *mgmt_rx_params)
					{
						struct mgmt_txrx_priv_psoc_context *mgmt_txrx_psoc_ctx;
						struct mgmt_rx_handler *rx_handler;
						enum mgmt_frame_type frm_type;
						// 得到对应的组件句柄 mgmt_txrx_psoc_ctx
						mgmt_txrx_psoc_ctx = (struct mgmt_txrx_priv_psoc_context *)wlan_objmgr_psoc_get_comp_private_obj(psoc,WLAN_UMAC_COMP_MGMT_TXRX);
						 
						// 根据帧类型得到 rx_handler 并把rx_handler下挂的所有节点 放到 rx_handler_head rx_handler_tail的链表中
						rx_handler = mgmt_txrx_psoc_ctx->mgmt_rx_comp_cb[frm_type];
						wlan_mgmt_txrx_rx_handler_list_copy(rx_handler,&rx_handler_head, &rx_handler_tail)
						
						// 根据MGMT_FRAME_TYPE_ALL得到 rx_handler 并把rx_handler下挂的所有节点 放到 rx_handler_head rx_handler_tail的链表中
						rx_handler = mgmt_txrx_psoc_ctx->mgmt_rx_comp_cb[MGMT_FRAME_TYPE_ALL];
						wlan_mgmt_txrx_rx_handler_list_copy(rx_handler,&rx_handler_head, &rx_handler_tail)
						
						// 遍历 rx_handler_head 下面所有的  rx_handler 调用 rx_handler->rx_cb 进行函数处理
						rx_handler = rx_handler_head;
						// 如果是 next的 都是copy的buf  直到最后一个
						
						while (rx_handler->next) {
							copy_buf = qdf_nbuf_clone(buf);

							if (!copy_buf) {
								rx_handler = rx_handler->next;
								continue;
							}

							rx_handler->rx_cb(psoc, peer, copy_buf,
										mgmt_rx_params, frm_type);
							rx_handler = rx_handler->next;
						}
						// 最后一个是原始buf
						rx_handler->rx_cb(psoc, peer, buf,
									mgmt_rx_params, frm_type);
									
					}
		{
				}	

			if (nbuf)
				qdf_nbuf_free(nbuf);

			return QDF_STATUS_E_NULL_VALUE;
		}
	}
}