

const struct wpa_driver_ops wpa_driver_nl80211_ops = {
	.name = "nl80211",
	.desc = "Linux nl80211/cfg80211",
	.get_bssid = wpa_driver_nl80211_get_bssid,
	.get_ssid = wpa_driver_nl80211_get_ssid,
	.set_key = driver_nl80211_set_key,
	.scan2 = driver_nl80211_scan2,
	.sched_scan = wpa_driver_nl80211_sched_scan,
	.stop_sched_scan = wpa_driver_nl80211_stop_sched_scan,
	.get_scan_results2 = wpa_driver_nl80211_get_scan_results,
	.abort_scan = wpa_driver_nl80211_abort_scan,
	.deauthenticate = driver_nl80211_deauthenticate,
	.authenticate = driver_nl80211_authenticate,
	.associate = wpa_driver_nl80211_associate,
	.global_init = nl80211_global_init,////////////
}
const struct wpa_driver_ops *const wpa_drivers[] =
{
#ifdef CONFIG_DRIVER_NL80211
	&wpa_driver_nl80211_ops,
#endif /* CONFIG_DRIVER_NL80211 */
#ifdef CONFIG_DRIVER_MACSEC_QCA
	&wpa_driver_macsec_qca_ops,
#endif /* CONFIG_DRIVER_MACSEC_QCA */
}




static int wpa_supplicant_set_driver(struct wpa_supplicant *wpa_s,
				     const char *name)
{
	if (name == NULL) {
		/* default to first driver in the list */
		return select_driver(wpa_s, 0);
	}

	do {
		pos = os_strchr(driver, ',');
		if (pos)
			len = pos - driver;
		else
			len = os_strlen(driver);

		for (i = 0; wpa_drivers[i]; i++) {
			if (os_strlen(wpa_drivers[i]->name) == len &&
			    os_strncmp(driver, wpa_drivers[i]->name, len) ==
			    0) {
				/* First driver that succeeds wins */
				if (select_driver(wpa_s, i) == 0)
					return 0;
			}
		}

		driver = pos + 1;
	} while (pos);
}

static int wpas_init_driver(struct wpa_supplicant *wpa_s,
			    const struct wpa_interface *iface)
{
	const char *ifname, *driver, *rn;

	driver = iface->driver;
next_driver:
	if (wpa_supplicant_set_driver(wpa_s, driver) < 0)///////////////////////////
		return -1;

	wpa_s->drv_priv = wpa_drv_init(wpa_s, wpa_s->ifname);
	if (wpa_s->drv_priv == NULL) {
		const char *pos;
		pos = driver ? os_strchr(driver, ',') : NULL;
		if (pos) {
			wpa_dbg(wpa_s, MSG_DEBUG, "Failed to initialize "
				"driver interface - try next driver wrapper");
			driver = pos + 1;
			goto next_driver;
		}
		wpa_msg(wpa_s, MSG_ERROR, "Failed to initialize driver "
			"interface");
		return -1;
	}
	if (wpa_drv_set_param(wpa_s, wpa_s->conf->driver_param) < 0) {
		wpa_msg(wpa_s, MSG_ERROR, "Driver interface rejected "
			"driver_param '%s'", wpa_s->conf->driver_param);
		return -1;
	}

	ifname = wpa_drv_get_ifname(wpa_s);
	if (ifname && os_strcmp(ifname, wpa_s->ifname) != 0) {
		wpa_dbg(wpa_s, MSG_DEBUG, "Driver interface replaced "
			"interface name with '%s'", ifname);
		os_strlcpy(wpa_s->ifname, ifname, sizeof(wpa_s->ifname));
	}

	rn = wpa_driver_get_radio_name(wpa_s);
	if (rn && rn[0] == '\0')
		rn = NULL;

	wpa_s->radio = radio_add_interface(wpa_s, rn);
	if (wpa_s->radio == NULL)
		return -1;

	return 0;
}


static int wpa_supplicant_init_iface(struct wpa_supplicant *wpa_s,
				     const struct wpa_interface *iface)
{
	if (wpas_init_driver(wpa_s, iface) < 0)
		return -1; 
}

struct wpa_supplicant * wpa_supplicant_add_iface(struct wpa_global *global,
						 struct wpa_interface *iface,
						 struct wpa_supplicant *parent)
{
	wpa_supplicant_init_iface(wpa_s, &t_iface);
		
}















struct wpa_global * wpa_supplicant_init(struct wpa_params *params)
{
	// 1.注册全局的消息句柄
	#ifndef CONFIG_NO_WPA_MSG
	wpa_msg_register_ifname_cb(wpa_supplicant_msg_ifname_cb);
	#endif 
	// 2.注册eap相关
	ret = eap_register_methods();
	
	// 全局 global 分配空间
	struct wpa_global *global= os_zalloc(sizeof(*global));
	eloop_init();
	
	// 3.初始化 global 
	global->ctrl_iface = wpa_supplicant_global_ctrl_iface_init(global);
	{	// 下面是对wpa_supplicant_global_ctrl_iface_init 函数的分析
		struct ctrl_iface_global_priv *wpa_supplicant_global_ctrl_iface_init(struct wpa_global *global)	
		{
			struct ctrl_iface_global_priv *priv = os_zalloc(sizeof(*priv));
			priv->global = global;
			// 此时的global->params.ctrl_interface = /var/run/wpa_supplicantglobal
			if (global->params.ctrl_interface == NULL)
				return priv;
			
			// 下面是绑定socket
			wpas_global_ctrl_iface_open_sock(global, priv)
			{
				const char *ctrl = global->params.ctrl_interface; // /var/run/wpa_supplicantglobal
				priv->sock = socket(PF_UNIX, SOCK_DGRAM, 0);
				os_strlcpy(addr.sun_path, ctrl, sizeof(addr.sun_path));
				bind(priv->sock, (struct sockaddr *) &addr, sizeof(addr))
				
				// 重点：通过eloop监测 priv->sock 的事件
				// 回调函数为 wpa_supplicant_global_ctrl_iface_receive
				// 也就是假如有wpa_cli 发socket信息给 wpa_supplicant 进程，则处理函数就是
				// wpa_supplicant_global_ctrl_iface_receive
				eloop_register_read_sock(priv->sock,
					wpa_supplicant_global_ctrl_iface_receive,
					global, priv);
			}
		}
	}
	// 4.dbus 相关
	wpas_notify_supplicant_initialized(global);
	// 5和6.统计有多少个wpa_drivers，并分配 drv_count 个槽 
	//                   	|------||------|   
	// global->drv_priv指向 | void*|| void*|   一共drv_count个空的槽 
	//                   	|------||------|
	// 通过 查看 wpa_drivers 可知 wpa_drivers数组一共有两项  
	// 所以 global->drv_priv 此时指向两个空的void* 槽 
	//
	for (i = 0; wpa_drivers[i]; i++)
		global->drv_count++;
	global->drv_priv = os_calloc(global->drv_count, sizeof(void *));
}
//main.c
int main(int argc, char *argv[])
{
	//1. 解析启动参数
	// wpa_supplicant -g /var/run/wpa_supplicantglobal  -P /var/run/wpa_supplicant-global.pid -B
	for (;;) {
		c = getopt(argc, argv,
			   "b:Bc:C:D:de:f:g:G:hi:I:KLMm:No:O:p:P:qsTtuvW");
		switch (c) {
			case 'g':
				params.ctrl_interface = optarg;
			break;
			case 'P':
				os_free(params.pid_file);
				params.pid_file = os_rel2abs_path(optarg);
			break;
			case 'B':
				params.daemonize++;
			break;
			case 'N':
				iface_count++;
				iface = os_realloc_array(ifaces, iface_count,
							 sizeof(struct wpa_interface));
				if (iface == NULL)
					goto out;
				ifaces = iface;
				iface = &ifaces[iface_count - 1];
				os_memset(iface, 0, sizeof(*iface));
			break;
		}
	}		
	global = wpa_supplicant_init(&params);
	
	//2. wpa_supplicant -N 选项的时候下面才生效，由于我们启动 wpa_supplicant进程使用下面的命令
	// wpa_supplicant -g /var/run/wpa_supplicantglobal  -P /var/run/wpa_supplicant-global.pid #-B
	// 并没有添加-N 选项，下面就不考虑，至于什么时候调用wpa_supplicant_add_iface呢？
	// 当wpa_cli -g /var/run/wpa_supplicantglobal interface_add ath0 /var/run/wpa_supplicant-ath0.conf nl80211 /var/run/wpa_supplicant-ath0 ""
	// 使用这个命令的时候，才会通过 wpa_supplicant_global_ctrl_iface_receive 回调处理 interface_add 去
	// 调用 这个 wpa_supplicant_add_iface 接口 下面会分析
	
	
	for (i = 0; exitcode == 0 && i < iface_count; i++) {
		wpa_s  = wpa_supplicant_add_iface(global, &ifaces[i], NULL);
	}
	
}



static void wpa_supplicant_global_ctrl_iface_receive(int sock, void *eloop_ctx,
						     void *sock_ctx)
{
	// 读取 wpa_cli 数据
	res = recvfrom(sock, buf, CTRL_IFACE_MAX_LEN + 1, 0,
		       (struct sockaddr *) &from, &fromlen);

	// 处理数据
	reply_buf = wpa_supplicant_global_ctrl_iface_process(global, buf, &reply_len);
	{
		// 匹配关键字 INTERFACE_ADD
		if (os_strncmp(buf, "INTERFACE_ADD ", 14) == 0){
			// 到这里很清楚了，调用到了 wpa_supplicant_global_iface_add
			wpa_supplicant_global_iface_add(global, buf + 14);
		} 
		
	}
	
	// 发送处理结果给 wpa_cli
	sendto(sock, reply, reply_len, 0, (struct sockaddr *) &from,fromlen)
}

struct wpa_supplicant * wpa_supplicant_add_iface(struct wpa_global *global,struct wpa_interface *iface)
{
	struct wpa_supplicant *wpa_s;
	// 1.解析 interface_add 后面的参数，存放在 iface里面
	struct wpa_interface iface;
	do {
		//iface.confname = pos
		//iface.driver = pos
		//iface.ctrl_interface = pos
	} while (0);
	// 2. 新增接口
	wpa_s = wpa_supplicant_add_iface(global, &iface, NULL);
	{
		// 下面分析 wpa_supplicant_add_iface 接口
		struct wpa_supplicant * wpa_supplicant_add_iface(struct wpa_global *global,
						 struct wpa_interface *iface,
						 struct wpa_supplicant *parent)
		 {
			// 分配空间 wpa_s
			wpa_s = wpa_supplicant_alloc(parent);
			{
				wpa_s->global = global;
				// wpa_supplicant_init_iface为重要函数，下面单独用一节来分析它
				wpa_supplicant_init_iface(wpa_s, &t_iface)
				
				if (iface->p2p_mgmt == 0) {
					wpas_notify_iface_added(wpa_s)
					// 通过dbus通知外界有新的无线网络加入
					for (ssid = wpa_s->conf->ssid; ssid; ssid = ssid->next)
						wpas_notify_network_added(wpa_s, ssid);
				}
				// 设置链表
				/*
				*   插入前
				*    wpa_s       wpa_s      global
				*  |-------| 	|-------|	|-------|
				*  | next  |->  | next  |-> | ifaces|--|
			    *  |-------| 	|-------|	|-------|  |
				*     ^                                |
				*     |________________________________|
				*  
				*  插入后
				*    新的
				*    wpa_s       wpa_s       wpa_s      global
				*  |-------| 	|-------|	|-------|	|-------|
				*  | next  |->  | next  |-> | next  |-> | ifaces|--|
			    *  |-------| 	|-------|	|-------|	|-------|  |
				*     ^                                            |
				*     |____________________________________________|
				*/
				
				
				wpa_s->next    = global->ifaces;
				global->ifaces = wpa_s;
				
				return wpa_s;
			}
		 }				 

		
	}
	
	return wpa_s;
}


static int wpa_supplicant_init_iface(struct wpa_supplicant *wpa_s,
				     const struct wpa_interface *iface)
{
	//1.把 /var/run/wpa_supplicant-ath2.conf 配置文件解析
	wpa_s->conf = wpa_config_read(wpa_s->confname, NULL);
	//2.wpa_s里面的ifname 其实就是iface->ifname 即类似这样的 ath0 名字
	os_strlcpy(wpa_s->ifname, iface->ifname, sizeof(wpa_s->ifname));
	
	// 两个状态机暂时不分析
	eapol_sm_notify_portEnabled(wpa_s->eapol, false);
	eapol_sm_notify_portValid(wpa_s->eapol, false);
	
	//3.初始化驱动
	wpas_init_driver(wpa_s, iface)
	{ // static int wpas_init_driver(struct wpa_supplicant *wpa_s,const struct wpa_interface *iface)函数原型分析如下
		{
			const char *ifname, *driver, *rn;
			driver = iface->driver;// nl80211
			wpa_supplicant_set_driver(wpa_s, driver);
			{
				// 这里面会遍历wpa_drivers[i]数组，然后找到名字为driver 即80211 的一项
				//然后调用 static int select_driver(struct wpa_supplicant *wpa_s, int i)函数原型分析如下
				{
					// 比如如果传入的是nl80211 那么wpa_drivers[i]->global_init 就是 nl80211_global_init
					// 然后调用 nl80211_global_init 把返回值赋值给wpa_s->global_drv_priv
					global->drv_priv[i] = wpa_drivers[i]->global_init(global);
					{
						//static void * nl80211_global_init(void *ctx)
						{
							wpa_driver_nl80211_init_nl_global(global);
							{	//static int wpa_driver_nl80211_init_nl_global(struct nl80211_global *global)函数原型分析如下
								{
									// 通过eloop 检测底层驱动事件，处理函数为 process_global_event
										nl_cb_set(global->nl_cb, NL_CB_SEQ_CHECK, NL_CB_CUSTOM,no_seq_check, NULL);//置序列号检查函数为no_seq_check
										nl_cb_set(global->nl_cb, NL_CB_VALID,     NL_CB_CUSTOM,process_global_event, global);//置netlink消息回调处理函数
										/*
										将nl_event对应的socket注册到eloop中，回调函数为 wpa_driver_nl80211_event_receive，
										wpa_driver_nl80211_event_receive->调用 nl_recv_msg->调用 process_global_event。所以，我们只要关注process_global_event就可以了。
										*/
										nl80211_register_eloop_read(&global->nl_event,
													wpa_driver_nl80211_event_receive,
													global->nl_cb, 0);
								}
								
							}
						}
					}
					wpa_s->driver = wpa_drivers[i];
					wpa_s->global_drv_priv = global->drv_priv[i];
				}
			}
			wpa_s->drv_priv = wpa_drv_init(wpa_s, wpa_s->ifname);
			{//static inline void * wpa_drv_init(struct wpa_supplicant *wpa_s,const char *ifname)函数原型分析如下
				{   // 此时 .init2 = wpa_driver_nl80211_init, 参见wpa_driver_nl80211_ops 即可
					if (wpa_s->driver->init2)
					return wpa_s->driver->init2(wpa_s, ifname,//-----------	关注点2
									wpa_s->global_drv_priv);
					if (wpa_s->driver->init) {
						return wpa_s->driver->init(wpa_s, ifname);
					}
				}
			}
			wpa_drv_set_param(wpa_s, wpa_s->conf->driver_param);
			rn = wpa_driver_get_radio_name(wpa_s);
			wpa_s->radio = radio_add_interface(wpa_s, rn);
		}
		
	}
	
	wpa_supplicant_init_wpa(wpa_s);
	{
		// 主要是初始化里面的状态机
		struct wpa_sm_ctx *ctx;
		ctx = os_zalloc(sizeof(*ctx));
		if (ctx == NULL) {
			wpa_printf(MSG_ERROR, "Failed to allocate WPA context.");
			return -1;
		}
		// 注册了一些函数
		ctx->ctx = wpa_s;
		ctx->msg_ctx = wpa_s;
		ctx->set_state = _wpa_supplicant_set_state;
		ctx->get_state = _wpa_supplicant_get_state;
		ctx->deauthenticate = _wpa_supplicant_deauthenticate;
		ctx->reconnect = _wpa_supplicant_reconnect;
		ctx->set_key = wpa_supplicant_set_key;
		ctx->get_network_ctx = wpa_supplicant_get_network_ctx;
		ctx->get_bssid = wpa_supplicant_get_bssid;
		// 这里限于篇幅省略了些函数结口
		wpa_s->wpa = wpa_sm_init(ctx);
		 
	}
	
	// 下面是一些根据配置 设置参数
	//wpa_sm_set_ifname(wpa_s->wpa, wpa_s->ifname,wpa_s->bridge_ifname[0] ? wpa_s->bridge_ifname :NULL);
	//wpa_sm_set_fast_reauth(wpa_s->wpa, wpa_s->conf->fast_reauth);

	if (wpa_s->drv_flags & WPA_DRIVER_FLAGS_DEDICATED_P2P_DEVICE)
		wpa_s->p2p_mgmt = iface->p2p_mgmt;


	wpa_supplicant_driver_init(wpa_s);
	{
		
		
	}
	wpas_wps_init(wpa_s);
	{
		
	}
	wpa_s->ctrl_iface = wpa_supplicant_ctrl_iface_init(wpa_s);
	wpa_bss_init(wpa_s);
	wpas_rrm_reset(wpa_s);
	wpa_supplicant_set_default_scan_ies(wpa_s);
}








int process_global_event(struct nl_msg *msg, void *arg)
{
	struct nl80211_global *global = arg;
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
	struct nlattr *tb[NL80211_ATTR_MAX + 1];
	struct wpa_driver_nl80211_data *drv, *tmp;
	int ifidx = -1, wiphy_idx = -1, wiphy_idx_rx = -1;
	struct i802_bss *bss;
	u64 wdev_id = 0;
	int wdev_id_set = 0;
	int wiphy_idx_set = 0;

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		  genlmsg_attrlen(gnlh, 0), NULL);

	if (tb[NL80211_ATTR_IFINDEX])
		ifidx = nla_get_u32(tb[NL80211_ATTR_IFINDEX]);
	else if (tb[NL80211_ATTR_WDEV]) {
		wdev_id = nla_get_u64(tb[NL80211_ATTR_WDEV]);
		wdev_id_set = 1;
	} else if (tb[NL80211_ATTR_WIPHY]) {
		wiphy_idx_rx = nla_get_u32(tb[NL80211_ATTR_WIPHY]);
		wiphy_idx_set = 1;
	}

	dl_list_for_each_safe(drv, tmp, &global->interfaces,
			      struct wpa_driver_nl80211_data, list) {
		for (bss = drv->first_bss; bss; bss = bss->next) {
			if (wiphy_idx_set)
				wiphy_idx = nl80211_get_wiphy_index(bss);
			if ((ifidx == -1 && !wiphy_idx_set && !wdev_id_set) ||
			    ifidx == bss->ifindex ||
			    (wiphy_idx_set && wiphy_idx == wiphy_idx_rx) ||
			    (wdev_id_set && bss->wdev_id_set &&
			     wdev_id == bss->wdev_id)) {
				do_process_drv_event(bss, gnlh->cmd, tb);//////////////
				return NL_SKIP;
			}
		}
		wpa_printf(MSG_DEBUG,
			   "nl80211: Ignored event %d (%s) for foreign interface (ifindex %d wdev 0x%llx)",
			   gnlh->cmd, nl80211_command_to_string(gnlh->cmd),
			   ifidx, (long long unsigned int) wdev_id);
	}

	return NL_SKIP;
}


static void do_process_drv_event(struct i802_bss *bss, int cmd,
				 struct nlattr **tb)
{
	struct wpa_driver_nl80211_data *drv = bss->drv;
	int external_scan_event = 0;
	struct nlattr *frame = tb[NL80211_ATTR_FRAME];

	wpa_printf(MSG_DEBUG, "nl80211: Drv Event %d (%s) received for %s",
		   cmd, nl80211_command_to_string(cmd), bss->ifname);

	if (cmd == NL80211_CMD_ROAM &&
	    (drv->capa.flags & WPA_DRIVER_FLAGS_KEY_MGMT_OFFLOAD)) {
		/*
		 * Device will use roam+auth vendor event to indicate
		 * roaming, so ignore the regular roam event.
		 */
		wpa_printf(MSG_DEBUG,
			   "nl80211: Ignore roam event (cmd=%d), device will use vendor event roam+auth",
			   cmd);
		return;
	}

	if (drv->ap_scan_as_station != NL80211_IFTYPE_UNSPECIFIED &&
	    (cmd == NL80211_CMD_NEW_SCAN_RESULTS ||
	     cmd == NL80211_CMD_SCAN_ABORTED))
		nl80211_restore_ap_mode(bss);

	switch (cmd) {
	case NL80211_CMD_TRIGGER_SCAN:
		wpa_dbg(drv->ctx, MSG_DEBUG, "nl80211: Scan trigger");
		drv->scan_state = SCAN_STARTED;
		if (drv->scan_for_auth) {
			/*
			 * Cannot indicate EVENT_SCAN_STARTED here since we skip
			 * EVENT_SCAN_RESULTS in scan_for_auth case and the
			 * upper layer implementation could get confused about
			 * scanning state.
			 */
			wpa_printf(MSG_DEBUG, "nl80211: Do not indicate scan-start event due to internal scan_for_auth");
			break;
		}
		wpa_supplicant_event(drv->ctx, EVENT_SCAN_STARTED, NULL);//////////////
		break;
	case NL80211_CMD_START_SCHED_SCAN:
		wpa_dbg(drv->ctx, MSG_DEBUG, "nl80211: Sched scan started");
		drv->scan_state = SCHED_SCAN_STARTED;
		break;
	case NL80211_CMD_SCHED_SCAN_STOPPED:
		wpa_dbg(drv->ctx, MSG_DEBUG, "nl80211: Sched scan stopped");
		drv->scan_state = SCHED_SCAN_STOPPED;
		wpa_supplicant_event(drv->ctx, EVENT_SCHED_SCAN_STOPPED, NULL);//////////////
		break;
	case NL80211_CMD_NEW_SCAN_RESULTS:
		wpa_dbg(drv->ctx, MSG_DEBUG,
			"nl80211: New scan results available");
		if (drv->last_scan_cmd != NL80211_CMD_VENDOR)
			drv->scan_state = SCAN_COMPLETED;
		drv->scan_complete_events = 1;
		if (drv->last_scan_cmd == NL80211_CMD_TRIGGER_SCAN) {
			eloop_cancel_timeout(wpa_driver_nl80211_scan_timeout,
					     drv, drv->ctx);
			drv->last_scan_cmd = 0;
		} else {
			external_scan_event = 1;
		}
		send_scan_event(drv, 0, tb, external_scan_event);
		break;
	case NL80211_CMD_SCHED_SCAN_RESULTS:
		wpa_dbg(drv->ctx, MSG_DEBUG,
			"nl80211: New sched scan results available");
		drv->scan_state = SCHED_SCAN_RESULTS;
		send_scan_event(drv, 0, tb, 0);
		break;
	case NL80211_CMD_SCAN_ABORTED:
		wpa_dbg(drv->ctx, MSG_DEBUG, "nl80211: Scan aborted");
		if (drv->last_scan_cmd != NL80211_CMD_VENDOR)
			drv->scan_state = SCAN_ABORTED;
		if (drv->last_scan_cmd == NL80211_CMD_TRIGGER_SCAN) {
			/*
			 * Need to indicate that scan results are available in
			 * order not to make wpa_supplicant stop its scanning.
			 */
			eloop_cancel_timeout(wpa_driver_nl80211_scan_timeout,
					     drv, drv->ctx);
			drv->last_scan_cmd = 0;
		} else {
			external_scan_event = 1;
		}
		send_scan_event(drv, 1, tb, external_scan_event);
		break;
	case NL80211_CMD_AUTHENTICATE:
	case NL80211_CMD_ASSOCIATE:
	case NL80211_CMD_DEAUTHENTICATE:
	case NL80211_CMD_DISASSOCIATE:
	case NL80211_CMD_FRAME_TX_STATUS:
	case NL80211_CMD_UNPROT_DEAUTHENTICATE:
	case NL80211_CMD_UNPROT_DISASSOCIATE:
		mlme_event(bss, cmd, tb[NL80211_ATTR_FRAME],
			   tb[NL80211_ATTR_MAC], tb[NL80211_ATTR_TIMED_OUT],
			   tb[NL80211_ATTR_WIPHY_FREQ], tb[NL80211_ATTR_ACK],
			   tb[NL80211_ATTR_COOKIE],
			   tb[NL80211_ATTR_RX_SIGNAL_DBM],
			   tb[NL80211_ATTR_STA_WME],
			   tb[NL80211_ATTR_REQ_IE]);
		break;
	case NL80211_CMD_CONNECT:
	case NL80211_CMD_ROAM:
		mlme_event_connect(drv, cmd,//////////////
				   tb[NL80211_ATTR_STATUS_CODE],
				   tb[NL80211_ATTR_MAC],
				   tb[NL80211_ATTR_REQ_IE],
				   tb[NL80211_ATTR_RESP_IE],
				   tb[NL80211_ATTR_TIMED_OUT],
				   tb[NL80211_ATTR_TIMEOUT_REASON],
				   NULL, NULL, NULL,
				   tb[NL80211_ATTR_FILS_KEK],
				   NULL,
				   tb[NL80211_ATTR_FILS_ERP_NEXT_SEQ_NUM],
				   tb[NL80211_ATTR_PMK],
				   tb[NL80211_ATTR_PMKID]);
		break;
	case NL80211_CMD_CH_SWITCH_STARTED_NOTIFY:
		mlme_event_ch_switch(drv,
				     tb[NL80211_ATTR_IFINDEX],
				     tb[NL80211_ATTR_WIPHY_FREQ],
				     tb[NL80211_ATTR_WIPHY_CHANNEL_TYPE],
				     tb[NL80211_ATTR_CHANNEL_WIDTH],
				     tb[NL80211_ATTR_CENTER_FREQ1],
				     tb[NL80211_ATTR_CENTER_FREQ2],
				     0);
		break;
	case NL80211_CMD_CH_SWITCH_NOTIFY:
		mlme_event_ch_switch(drv,
				     tb[NL80211_ATTR_IFINDEX],
				     tb[NL80211_ATTR_WIPHY_FREQ],
				     tb[NL80211_ATTR_WIPHY_CHANNEL_TYPE],
				     tb[NL80211_ATTR_CHANNEL_WIDTH],
				     tb[NL80211_ATTR_CENTER_FREQ1],
				     tb[NL80211_ATTR_CENTER_FREQ2],
				     1);
		break;
	case NL80211_CMD_DISCONNECT:
		mlme_event_disconnect(drv, tb[NL80211_ATTR_REASON_CODE],
				      tb[NL80211_ATTR_MAC],
				      tb[NL80211_ATTR_DISCONNECTED_BY_AP]);
		break;
	case NL80211_CMD_MICHAEL_MIC_FAILURE:
		mlme_event_michael_mic_failure(bss, tb);
		break;
	case NL80211_CMD_JOIN_IBSS:
		mlme_event_join_ibss(drv, tb);
		break;
	case NL80211_CMD_REMAIN_ON_CHANNEL:
		mlme_event_remain_on_channel(drv, 0, tb);
		break;
	case NL80211_CMD_CANCEL_REMAIN_ON_CHANNEL:
		mlme_event_remain_on_channel(drv, 1, tb);
		break;
	case NL80211_CMD_NOTIFY_CQM:
		nl80211_cqm_event(drv, tb);
		break;
	case NL80211_CMD_REG_CHANGE:
	case NL80211_CMD_WIPHY_REG_CHANGE:
		nl80211_reg_change_event(drv, tb);
		break;
	case NL80211_CMD_REG_BEACON_HINT:
		nl80211_reg_beacon_hint_event(drv, tb);
		break;
	case NL80211_CMD_NEW_STATION:
		nl80211_new_station_event(drv, bss, tb);
		break;
	case NL80211_CMD_DEL_STATION:
		nl80211_del_station_event(drv, bss, tb);
		break;
	case NL80211_CMD_SET_REKEY_OFFLOAD:
		nl80211_rekey_offload_event(drv, tb);
		break;
	case NL80211_CMD_PMKSA_CANDIDATE:
		nl80211_pmksa_candidate_event(drv, tb);
		break;
	case NL80211_CMD_PROBE_CLIENT:
		nl80211_client_probe_event(drv, tb);
		break;
	case NL80211_CMD_TDLS_OPER:
		nl80211_tdls_oper_event(drv, tb);
		break;
	case NL80211_CMD_CONN_FAILED:
		nl80211_connect_failed_event(drv, tb);
		break;
	case NL80211_CMD_FT_EVENT:
		mlme_event_ft_event(drv, tb);
		break;
	case NL80211_CMD_RADAR_DETECT:
		nl80211_radar_event(drv, tb);
		break;
	case NL80211_CMD_STOP_AP:
		nl80211_stop_ap(drv, tb);
		break;
	case NL80211_CMD_VENDOR:
		nl80211_vendor_event(drv, tb);
		break;
	case NL80211_CMD_NEW_PEER_CANDIDATE:
		nl80211_new_peer_candidate(drv, tb);
		break;
	case NL80211_CMD_PORT_AUTHORIZED:
		nl80211_port_authorized(drv, tb);
		break;
	case NL80211_CMD_STA_OPMODE_CHANGED:
		nl80211_sta_opmode_change_event(drv, tb);
		break;
	case NL80211_CMD_UPDATE_OWE_INFO:
		mlme_event_dh_event(drv, bss, tb);
		break;
	case NL80211_CMD_UNPROT_BEACON:
		if (frame)
			mlme_event_unprot_beacon(drv, nla_data(frame),
						 nla_len(frame));
		break;
	case NL80211_CMD_CONTROL_PORT_FRAME_TX_STATUS:
		if (!frame)
			break;
		nl80211_control_port_frame_tx_status(drv,
						     nla_data(frame),
						     nla_len(frame),
						     tb[NL80211_ATTR_ACK],
						     tb[NL80211_ATTR_COOKIE]);
		break;
	default:
		wpa_dbg(drv->ctx, MSG_DEBUG, "nl80211: Ignored unknown event "
			"(cmd=%d)", cmd);
		break;
	}
}
void wpa_supplicant_event(void *ctx, enum wpa_event_type event,
			  union wpa_event_data *data)
{
  	case EVENT_SCAN_RESULTS:
			if (wpa_supplicant_event_scan_results(wpa_s, data))
			break; /* interface may have been removed */
}			  
static int wpa_supplicant_event_scan_results(struct wpa_supplicant *wpa_s,
					     union wpa_event_data *data)
{
 
}				
static int _wpa_supplicant_event_scan_results(struct wpa_supplicant *wpa_s,
					      union wpa_event_data *data,
					      int own_request, int update_only)
{

}