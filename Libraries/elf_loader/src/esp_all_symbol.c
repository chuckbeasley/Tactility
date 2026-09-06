/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>

#include "private/elf_symbol.h"

/* Extern declarations from ELF symbol table */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
extern int esp_efuse_utility_count_once;
extern int esp_test_get_rx_mu_statistics;
extern int pwr_hal_set_beacon_filter_force_dump_limit;
extern int lv_style_set_shadow_spread;
extern int gpio_install_isr_service;
extern int lv_draw_sw_label;
extern int _ZTVSt23__codecvt_abstract_baseIwc10_mbstate_tE;
extern int r_ble_ll_conn_comp_event_send;
extern int lv_textarea_get_cursor_pos;
extern int uart_hal_set_txfifo_empty_thr;
extern int coex_schm_bt_a2dp_wifi_scan;
extern int service_instance_set_state;
extern int esp_ota_get_running_partition;
extern int uart_disable_rx_intr;
extern int esp_wifi_sta_get_ap_info_prof_pmk_internal;
extern int esp_now_send;
extern int lv_trigo_sin;
extern int tt_app_alertdialog_start;
extern int lv_indev_read;
extern int test_mimo_update_user_info;
extern int ethbroadcast;
extern int esp_partition_find;
extern int console_read;
extern int psa_key_agreement_raw_builtin;
extern int lv_font_get_glyph_dsc_fmt_txt;
extern int lv_draw_fill_dsc_init;
extern int ble_att_svr_start;
extern int lv_font_montserrat_18;
extern int phy_freq_i2c_num_addr;
extern int ppMapWaitTxq;
extern int r_ble_ll_hci_send_legacy_ext_adv_report;
extern int pmksa_cache_auth_get;
extern int ledc_set_fade_with_step;
extern int default_router_list;
extern int r_ble_ll_resolv_insert_eof_head;
extern int lv_color_format_get_size;
extern int httpd_sess_close_all;
extern int mpi_hal_set_mode;
extern int esp_sha512_driver_abort;
extern int lv_obj_get_event_count;
extern int mbedtls_ssl_tls12_get_preferred_hash_for_sig_alg;
extern int cnx_auth_timeout_process;
extern int ic_tx_pkt;
extern int _ZdlPvjSt11align_val_t;
extern int spi_flash_chip_winbond_page_program;
extern int is_fragmented_pkt;
extern int spi_flash_brownout_need_reset;
extern int phy_rxdc_est_min_new;
extern int lv_draw_deinit;
extern int lv_obj_set_style_flex_main_place;
extern int ble_store_config_persist_local_irk;
extern int ble_gattc_rx_read_group_type_adata;
extern int http_header_destroy;
extern int mbedtls_mpi_core_cond_swap;
extern int lvgl_keyboard_add_textarea;
extern int hostap_input;
extern int lvgl_hardware_keyboard_add_custom;
extern int eloop_run;
extern int preferences_opt_int32;
extern int ieee80211_tx_mgt_cb;
extern int vListInsert;
extern int gpio_config_as_analog;
extern int httpd_req_get_hdr_value_str;
extern int wifi_check_chan_param;
extern int mbedtls_gcm_auth_decrypt;
extern int wpa_auth_write_assoc_resp_owe;
extern int r_ble_ll_scan_rx_pkt_in_on_legacy;
extern int _ZTVSt17bad_function_call;
extern int esp_ocode_calib_init;
extern int uart_hal_rxfifo_rst;
extern int vswprintf;
extern int ic_update_sta_tsf;
extern int valid_key_length;
extern int r_ble_ll_scan_dup_update_ext;
extern int app_elf_sha256_str;
extern int phy_cal_param_track;
extern int mbedtls_x509_crt_parse_der;
extern int ble_ll_mem_env_p;
extern int r_ble_ll_scan_record_new_adv;
extern int phy_wifi_get_target_power;
extern int btwt_setup_dwell_timeout_fn_process;
extern int i2s_controller_get_config;
extern int r_ble_ll_utils_calc_access_addr;
extern int minitar_close;
extern int esp_netif_get_desc;
extern int tcp_trigger_input_pcb_close;
extern int esp_http_client_set_method;
extern int mbedtls_asn1_traverse_sequence_of;
extern int regdma_link_new_branch_addr_map;
extern int lv_cache_class_lru_rb_size;
extern int esp_wifi_vnd_lora_disable;
extern int esp_timer_delete;
extern int lv_obj_set_style_text_decor;
extern int g_coex_param;
extern int properties_file_open;
extern int gpio_descriptor_acquire;
extern int coex_schm_ble_mesh_config_bt_a2dp_wifi_scan;
extern int ble_gap_rx_l2cap_update_req;
extern int hmac_sha256_vector;
extern int pm_mac_enable_tsf_tbtt_soc_wakeup;
extern int mbedtls_ssl_decrypt_buf;
extern int phy_i2c_readReg_Mask;
extern int __stdio_flags;
extern int __atomic_fetch_and_8;
extern int httpd_register_uri_handler;
extern int mbedtls_aes_xts_free;
extern int sdspi_host_check_buffer_alignment;
extern int _g_esp_netif_netstack_default_wifi_ap;
extern int dragonfly_get_random_qr_qnr;
extern int ieee80211_output_do;
extern int esp_crypto_aes_gcm_encrypt_setup;
extern int ble_bb_cte_set_cte_ant_delay_tx_aod_1us;
extern int mac_tx_set_duration;
extern int lodepng_chunk_type_equals;
extern int ble_gap_repeat_pairing_event;
extern int bt_bb_v2_tx_set;
extern int pbuf_clone;
extern int phy_pwdet_tone_start;
extern int lv_style_set_arc_width;
extern int lv_area_increase;
extern int ip4_addr_isbroadcast_u32;
extern int r_ble_lll_scan_npl_restore;
extern int modem_clock_select_lp_clock_source;
extern int eb_space;
extern int mbedtls_ssl_read_version;
extern int ESP_EFUSE_ADC1_AVE_INITCODE_ATTEN2;
extern int coex_rom_osi_funcs_init;
extern int _ZTv0_n12_NSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED1Ev;
extern int i2c_master_device_change_address;
extern int ble_restore_our_sec_nvs;
extern int _ZSt12__to_chars_iIyESt15to_chars_resultPcS1_T_i;
extern int lv_obj_set_style_pad_left;
extern int ieee80211_search_node;
extern int wpa_supplicant_gtk_tx_bit_workaround;
extern int g_pm_cnt;
extern int esp_http_client_init;
extern int nvs_set_u8;
extern int hal_enable_sta_beacon_filter;
extern int handle_auth_sae;
extern int nan_ndp_inactivity_timeout_process;
extern int nimble_port_init;
extern int rc_set_per_conn_fix_rate;
extern int net80211_softap_funcs_init;
extern int ic_get_keep_alive_time;
extern int r_ble_lll_scan_ext_adv_init;
extern int _ZTVN2tt5MutexE;
extern int nvs_get_u8;
extern int phy_get_dcap_degen;
extern int bluetooth_midi_is_connected;
extern int r_ble_ll_adv_get_peer_rpa;
extern int __muldf3;
extern int phy_pbus_xpd_rx_on;
extern int esp_vfs_opendir;
extern int ble_att_svr_rx_read_blob;
extern int lv_label_ins_text;
extern int spi_timing_get_flash_timing_param;
extern int lv_font_get_glyph_static_bitmap;
extern int AUDIO_CODEC_TYPE;
extern int wpa_config_parse_string;
extern int esp_io_expander_gpio_wrapper_set_direction;
extern int empty_func_p_u32_r_void;
extern int ieee80211_cnx_attach;
extern int ieee80211_add_htcap_vendor;
extern int bb_agc_reg_update;
extern int lv_obj_scroll_to_y;
extern int _ZTVSt23_Sp_counted_ptr_inplaceIN2tt6PubSubIPvEESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int i2s_channel_get_info;
extern int r_ble_ll_hci_adv_mode_ext;
extern int mmu_hal_map_region;
extern int r_ble_ll_reset_sca;
extern int thread_get_return_code;
extern int multi_heap_get_info_impl;
extern int bt_bb_set_zb_tx_on_delay;
extern int __gesf2;
extern int _ZTISt8bad_cast;
extern int ic_init;
extern int r_hal_timer_read;
extern int lv_color_format_has_alpha;
extern int thread_set_affinity;
extern int spi_flash_hal_setup_read_suspend;
extern int _ZZNSt9__unicode13_Utf_iteratorIcDiPKcS2_NS_5_ReplEE12_M_read_utf8EvENKUlvE_clEv;
extern int ble_phy_module_env_p;
extern int i2c_controller_register8_set;
extern int lv_cache_class_lru_rb_count;
extern int ip_data;
extern int memp_free;
extern int ieee80211_regdomain_get_channel_bitmask;
extern int multi_heap_realloc_impl;
extern int pthread_mutex_trylock;
extern int wpa_set_bss;
extern int lora_can_receive;
extern int owe_build_assoc_req;
extern int sta_auth_shared;
extern int pm_enable_send_wake_null_on_channel;
extern int window_manager_stop;
extern int esp_bt_controller_init;
extern int lv_indev_set_long_press_time;
extern int ble_att_clt_rx_read_mult;
extern int esp_coex_common_semphr_give_wrapper;
extern int gdma_hal_reset;
extern int TRC_AMPDU_PER_UP_THRESHOLD;
extern int mbedtls_psa_mac_sign_setup;
extern int lv_draw_sw_mask_line_points_init;
extern int esp_wifi_ap_get_prof_pmk_internal;
extern int pvTaskIncrementMutexHeldCount;
extern int ble_gattc_deinit;
extern int io_mux_is_lp_io_in_use;
extern int lv_font_montserrat_10;
extern int tolower;
extern int psa_status_to_mbedtls;
extern int udp_pcbs;
extern int r_ble_lll_scan_aux_data_free;
extern int ieee80211_find_elem_match;
extern int gdma_hal_get_intr_status_reg;
extern int _ZTVNSt7__cxx118time_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEEE;
extern int lv_style_set_recolor_opa;
extern int backlight_set_brightness_default;
extern int esp_wifi_internal_on_coex_start;
extern int bt_bb_corr_thresh_aa_lc_en;
extern int phy_disable_low_rate;
extern int uart_hal_set_hw_flow_ctrl;
extern int coex_schm_ble_mesh_standby_bt_default_wifi_connecting;
extern int cache_hal_writeback_addr;
extern int ble_gatts_count_cfg;
extern int device_unlock;
extern int phy_tatget_pwr_band;
extern int nl_langinfo_l;
extern int mbedtls_ssl_encrypt_buf;
extern int hal_he_set_ersu_ack_rate;
extern int npl_freertos_funcs_get;
extern int r_ble_ll_misc_additional_options_set;
extern int pwr_hal_clear_mac_modem_state_wakeup_protect_signal;
extern int lv_image_get_pivot;
extern int vasprintf;
extern int lv_timer_get_paused;
extern int lv_textarea_set_cursor_pos;
extern int pm_enable_keep_alive_timer;
extern int hal_deinit;
extern int spi_flash_hal_setup_auto_suspend_mode;
extern int vTaskInternalSetTimeOutState;
extern int mbedtls_ecp_keypair_init;
extern int rc_disable_trc;
extern int lv_canvas_set_buffer;
extern int rewind;
extern int r_ble_lll_sched_env_init;
extern int tcp_rexmit;
extern int sys_check_timeouts;
extern int connect_scan_flag;
extern int phy_adc_rate_cal_rxdc;
extern int r_scan_duplicate_cache_refresh_set_time;
extern int phy_param;
extern int ieee80211_send_mgmt;
extern int _ZSt9use_facetISt5ctypeIwEERKT_RKSt6locale;
extern int spicommon_dma_chan_alloc;
extern int _timezone;
extern int g_wpa_pmk_caching_disabled;
extern int esp_transport_list_clean;
extern int close_pending;
extern int priv_config_opts_ptr;
extern int memp_SYS_TIMEOUT;
extern int ble_transport_to_hs_evt_impl;
extern int esp_psram_extram_test;
extern int esp_intr_noniram_enable;
extern int i2c_controller_register8_set_bits;
extern int esp_transport_ssl_set_client_key_data;
extern int wpa_sm_deinit;
extern int lodepng_compress_settings_init;
extern int driver_add;
extern int coex_schm_ble_mesh_standby_wifi_scan;
extern int psa_its_set;
extern int phy_get_rfcal_rxiq_data;
extern int psa_load_persistent_key;
extern int usb_hid_device_start;
extern int lv_draw_layer_create;
extern int hal_mac_fill_hwtxop;
extern int lv_color_black;
extern int tcp_rexmit_rto_prepare;
extern int esp_http_server_dispatch_event;
extern int lv_dropdown_get_selected_str;
extern int r_ble_ll_hci_ev_send_scan_timeout;
extern int spi_flash_op_lock;
extern int cJSON_ParseWithOpts;
extern int gpio_intr_enable;
extern int esp_crypto_base64_encode;
extern int sdmmc_flip_byte_order;
extern int dhcp_parse_extra_opts;
extern int lvgl_get_text_font_height;
extern int IP_EVENT;
extern int ble_gap_end_test_evt;
extern int gpio_sleep_set_direction;
extern int dhcp_coarse_tmr;
extern int pm_mesh_set_next_tbtt;
extern int lv_obj_set_style_layout;
extern int wpabuf_clear_free;
extern int isnanf;
extern int r_hal_timer_env_deinit;
extern int r_ble_ll_adv_rand_chan_from_mask;
extern int lv_chart_get_point_pos_by_id;
extern int mld6_input;
extern int f_getfree;
extern int memp_MLD6_GROUP;
extern int spi_hal_clear_intr_mask;
extern int r_ble_lll_conn_env_deinit;
extern int http_header_get_item;
extern int lv_timer_set_repeat_count;
extern int r_ble_phy_txpwr_set;
extern int heap_caps_check_add_region_allowed;
extern int ecc_hal_start_calc;
extern int r_ble_lll_adv_set_aux_data;
extern int udp_bind;
extern int _ZTVSt8numpunctIcE;
extern int phy_bb_cfo_cfg;
extern int lv_spinbox_class;
extern int r_ble_ll_arr_init;
extern int ble_hs_hci_evt_process;
extern int crypto_bignum_add;
extern int s_get_cache_state;
extern int xQueueIsQueueEmptyFromISR;
extern int trc_get_espnow_rate;
extern int g_pm;
extern int modem_clock_hal_enable_modem_private_fe_clock;
extern int cache_hal_resume;
extern int r_ble_ll_resolv_get_addr_pointer;
extern int pthread_include_pthread_rwlock_impl;
extern int wdev_is_data_in_rxlist;
extern int phy_pwdet_code_cal;
extern int lvgl_keyboard_enable;
extern int uart_controller_close;
extern int ble_ll_resolv_env_p;
extern int r_ble_lll_mmgmt_block_copy;
extern int i2c_controller_register16le_get;
extern int _ZSt9use_facetISt5ctypeIcEERKT_RKSt6locale;
extern int i2c_release_bus_handle;
extern int g_mmgmt_max_evt_block_cnt;
extern int bluetooth_get_radio_state;
extern int pm_update_modem_sleep_default_params;
extern int spi_flash_munmap;
extern int lv_bar_get_start_value;
extern int mbedtls_mpi_read_binary_le;
extern int r_ble_lll_adv_periodic_did_update;
extern int ppInstallKey;
extern int strcoll;
extern int ppRxFragmentProc;
extern int pp_process_hmac_waiting_txq;
extern int offchan_tx_progress_in;
extern int esp_sha256_driver_clone;
extern int itoa;
extern int r_filter_aux_duplicate_check_need_report;
extern int r_ble_ll_ctrl_rx_pdu;
extern int esp_netif_get_ip_info;
extern int sys_timeouts_sleeptime;
extern int ble_l2cap_init;
extern int lvgl_software_keyboard_set_content_area;
extern int spi_hal_hw_prepare_tx;
extern int phy_rxiq_opt;
extern int ble_att_clt_rx_write;
extern int crypto_ec_point_add;
extern int sae_derive_pt;
extern int lmac_record_txtime;
extern int lv_fs_tell;
extern int g_ble_ll_whitelist_deleted_irk_hdr;
extern int lv_style_set_pad_right;
extern int _lock_close;
extern int esp_wifi_get_hostap_private_internal;
extern int pmksa_cache_add_entry;
extern int r_ble_lll_sync_init;
extern int ble_hs_hci_rx_evt;
extern int tzname;
extern int lv_label_get_letter_on;
extern int lvgl_software_keyboard_hide;
extern int esp_aes_cipher_set_iv;
extern int r_ble_lll_scan_apply_multi_conn_slot;
extern int g_flash_guard_default_ops;
extern int wifi_gpio_debug;
extern int lv_draw_layer_init;
extern int lv_draw_arc_dsc_init;
extern int sta_csa_timer;
extern int ppCalTxAMPDULength;
extern int ieee80211_add_wme_param;
extern int lv_display_set_offset;
extern int __umoddi3;
extern int mbedtls_asn1_get_bitstring_null;
extern int r_ble_lll_conn_timeout_urgency_compare;
extern int sae_hash;
extern int r_ble_ll_df_conn_cte_rsp_enable;
extern int nimble_port_freertos_init;
extern int nd6_adjust_mld_membership;
extern int xRingbufferReceive;
extern int rcGet11BHighestRateIdx;
extern int esp_aes_crypt_ofb;
extern int mbedtls_ssl_get_mode_from_ciphersuite;
extern int r_ble_ll_whitelist_add;
extern int he_send_action_bsscolor_inuse;
extern int lv_obj_is_visible;
extern int lv_draw_sw_border;
extern int regdma_link_new_branch_continuous;
extern int ble_sm_gen_pair_rand;
extern int lodepng_inspect;
extern int eloop_cancel_timeout;
extern int phy_rfrx_gain_index;
extern int mbedtls_x509_get_sig_alg;
extern int wdev_set_promis_misc_pkt;
extern int nd6_reachability_hint;
extern int modff;
extern int iswalpha;
extern int lv_trigo_cos;
extern int rtc_clk_freq_to_period;
extern int esp_netif_set_ip_info;
extern int _ZTVSt23__codecvt_abstract_baseIDiDu10_mbstate_tE;
extern int ets_timer_done;
extern int _ZTv0_n12_NSdD0Ev;
extern int r_ble_hw_whitelist_check_in_wl;
extern int lwip_netconn_do_newconn;
extern int txop_max_list;
extern int r_ble_ll_adv_periodic_check_data_itvl;
extern int ble_controller_scan_duplicate_config;
extern int mbedtls_ssl_parse_finished;
extern int __retarget_lock_init;
extern int module_start;
extern int lv_switch_create;
extern int __gedf2;
extern int lv_style_set_shadow_offset_y;
extern int esp_vfs_fat_spiflash_mount_ro;
extern int r_ble_ll_hci_cmd_rx;
extern int r_ble_ll_conn_set_phy;
extern int esp_wifi_ap_set_group_mgmt_cipher_internal;
extern int lv_draw_glyph_dsc_init;
extern int pointer_get_mirror_x;
extern int lv_line_set_points_mutable;
extern int ble_hs_adv_set_fields;
extern int pm_disconnected_stop;
extern int lv_display_set_color_format;
extern int wifi_softap_set_config;
extern int he_twt_teardown_txcb;
extern int usb_msc_device_stop;
extern int file_system_add;
extern int lv_textarea_create;
extern int controller_mem_ptr_check;
extern int spi_bus_lock_acquire_end;
extern int owe_build_dhie;
extern int r_ble_ll_utils_remapped_channel;
extern int tt_app_fileselection_get_result_path;
extern int thread_set_stack_size;
extern int lv_pow;
extern int ble_hs_adv_find_field;
extern int mbedtls_psa_asymmetric_decrypt;
extern int aes_hal_read_iv;
extern int spi_flash_chip_generic_get_caps;
extern int _ZTISt9exception;
extern int g_cte_sample_limit;
extern int esp_transport_capture_errno;
extern int system_event_emit;
extern int ieee80211_sta_new_state;
extern int r_ble_ll_adv_env_init;
extern int lwip_getpeername;
extern int __ctzsi2;
extern int thread_start;
extern int ble_stack_disable;
extern int uart_wait_tx_done;
extern int r_ble_ll_sync_env_init;
extern int r_ble_ll_sync_list_add;
extern int esp_event_isr_post;
extern int r_ble_ll_sync_find;
extern int ieee80211_add_csa;
extern int phy_set_bttx_low_power;
extern int ble_bb_cte_set_cte_ant_switch_delay_rx_aoa_1us;
extern int r_ble_lll_conn_coex_dpc_update_on_event_scheduled;
extern int mbedtls_mpi_core_to_mont_rep;
extern int ble_freertos_co_pool;
extern int mbedtls_sha256_free;
extern int esp_elf_arch_relocate;
extern int esp_read_mac;
extern int esp_sha_set_mode;
extern int djb2_str;
extern int bundle_get_bool;
extern int timer_get_expiry_time;
extern int esp_coex_common_semphr_take_wrapper;
extern int phy_rx_11b_opt;
extern int lwip_ioctl;
extern int phy_write_rfpll_sdm;
extern int ieee80211_hostap_send_beacon_process;
extern int multi_heap_restore_minimum_free_bytes;
extern int lv_obj_set_style_margin_left;
extern int difftime;
extern int lora_remove_tx_callback;
extern int nan_ndp_setup_timeout_process;
extern int lv_draw_sw_mask_line_angle_init;
extern int r_ble_ll_ctrl_enc_allowed_pdu_rx;
extern int properties_file_for_each;
extern int esp_sha_512_t_init_hash;
extern int ieee80211_wme_standard_ac_to_esp_ac;
extern int module_resolve_symbol_global;
extern int i2s_del_channel;
extern int xTimerIsTimerActive;
extern int ble_gap_rx_rd_rem_sup_feat_complete;
extern int lv_obj_invalidate_area;
extern int esp_wifi_set_protocol;
extern int esp32_gpio_driver;
extern int r_ble_ll_mem_low_prio_src_set;
extern int r_ble_lll_adv_pdu_info_calculate;
extern int nd6_get_destination_mtu;
extern int mbedtls_aes_xts_init;
extern int ieee80211_raw_frame_sanity_check;
extern int bluetooth_remove_event_callback;
extern int action_q;
extern int set_rx_sense;
extern int r_ble_ll_arr_release;
extern int esp_netif_get_handle_from_ifkey;
extern int lv_arc_set_range;
extern int hal_init_tb_tx;
extern int s_wifi_stop_in_progress;
extern int hal_he_get_mplen_addr_end;
extern int esp_rsa_ds_release_ds_lock;
extern int mbedtls_net_free;
extern int uart_flush_input;
extern int ieee80211_find_ie_match;
extern int esp_sha1_driver_update;
extern int gdma_ahb_hal_clear_intr;
extern int app_manager_get_state;
extern int ieee80211_alloc_tx_buf;
extern int esp_task_wdt_impl_timer_allocate;
extern int rx11AXRate2AMPDULimit_update;
extern int __pow5Factor;
extern int _ZTVN2tt7service11displayidle24BouncingBallsScreensaverE;
extern int r_ble_ll_resolv_get_priv_addr;
extern int ble_hs_is_enabled;
extern int _i2s_hal_set_rx_clock;
extern int sys_arch_sem_wait;
extern int ulTaskGenericNotifyTake;
extern int vQueueSetQueueNumber;
extern int wdevProcessRxSucDataAll;
extern int ble_gap_rx_adv_report;
extern int pwm_is_inverted;
extern int nan_dp_search_node;
extern int __wrap_lv_dropdown_create;
extern int esp_timer_is_active;
extern int lv_display_get_physical_horizontal_resolution;
extern int r_ble_ll_adv_aux_calculate;
extern int sys_sem_new;
extern int btdm_broker_init;
extern int esp32c5_eco3_rom_ptr_init;
extern int sqrt;
extern int r_ble_lll_rxed_buffer_is_last_one;
extern int lv_anim_refr_now;
extern int phy_get_chan_power_offset;
extern int wlanif_input;
extern int lv_draw_sw_blend_color_to_l8;
extern int phy_get_dco_comp;
extern int r_ble_ll_conn_hci_read_transmit_power_level;
extern int __cxa_free_exception;
extern int lv_draw_sw_rgb565_swap;
extern int r_ble_lll_per_adv_coex_dpc_process;
extern int pwr_hal_set_mac_modem_beacon_miss_timeout;
extern int r_ble_ll_scan_sm_start;
extern int r_ble_ll_conn_sm_free_event;
extern int ieee80211_add_ulmu_pwrcap;
extern int ieee80211_ethbroadcast;
extern int ppRegisterTxCallback;
extern int mbedtls_hmac_drbg_seed_buf;
extern int lv_area_is_on;
extern int mbedtls_mpi_inv_mod_even_in_range;
extern int lmacTxFrame;
extern int esp_cache_unfreeze_ext_mem_cache;
extern int bundle_opt_int64;
extern int ble_gatts_peer_cl_sup_feat_update;
extern int ieee80211_add_xrates;
extern int phy_set_rxclk_en;
extern int eTaskGetState;
extern int itwt_setup_timeout_fn_process;
extern int lvgl_display_remove;
extern int wifi_funcs;
extern int strtok_r;
extern int nimble_mem_free;
extern int wpa_set_pmk;
extern int mbedtls_rsa_init;
extern int timer_set_callback_priority;
extern int ble_hs_sched_reset;
extern int wpa_add_kde;
extern int esp_deep_sleep_register_hook;
extern int lv_indev_scroll_throw_handler;
extern int ieee80211_add_heopr;
extern int wdev_process_beacon_filter;
extern int crypto_bignum_init;
extern int _ZTVSt13runtime_error;
extern int dns_gethostbyname_addrtype;
extern int ble_att_svr_rx_find_info;
extern int pm_set_sleep_min_active_time;
extern int _ZSt14__ostream_fillIcSt11char_traitsIcEEvRSt13basic_ostreamIT_T0_Ei;
extern int r_ble_ll_adv_sm_start;
extern int _ZTVSt18__moneypunct_cacheIwLb0EE;
extern int mem_calloc;
extern int lv_obj_add_style;
extern int g_btwt_num;
extern int ieee80211_phy_init;
extern int _ZSt19__throw_ios_failurePKci;
extern int sta_recv_assoc;
extern int esp_wifi_ap_get_max_sta_conn;
extern int mbedtls_to_psa_error;
extern int lv_obj_has_flag;
extern int r_exception_list_ble_mesh_pb_adv_link_id_table_add;
extern int pm_enable_disconnected_sleep_delay_timer;
extern int os_timer_arm;
extern int psa_generate_key_custom;
extern int esp_md5_hash_abort;
extern int _Z8init_mtkP6Device;
extern int _ZTVSt7num_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE;
extern int ieee80211_add_tie_param;
extern int npl_freertos_callout_mem_reset;
extern int spi_device_transmit;
extern int lv_obj_get_scrollbar_area;
extern int esp_test_tx_process_txq_state;
extern int lv_obj_get_content_coords;
extern int lv_checkbox_get_text;
extern int r_DB_hash_function_blob;
extern int ble_sm_peer_pair_rand;
extern int device_get_first_active_by_type;
extern int cnx_remove_from_blacklist;
extern int r_ble_lll_scan_should_resume_in_isr;
extern int _Znaj;
extern int esp_wifi_ap_get_sae_ext_config_internal;
extern int r_ble_ll_conn_prepare_tx_pdu;
extern int pbuf_copy_partial;
extern int esp_task_wdt_impl_timeout_triggered;
extern int regulatory_data;
extern int __ubsan_include;
extern int lv_bar_get_min_value;
extern int ieee80211_freedom_init;
extern int lv_indev_get_active_obj;
extern int psa_import_key;
extern int tsf_hal_set_modem_close_rf_delay_cycles;
extern int spi_flash_chip_generic_get_io_mode;
extern int lv_timer_get_time_until_next;
extern int r_ble_ll_scan_set_enable;
extern int r_ble_ll_resolv_list_rmv;
extern int _ZSt9use_facetISt8numpunctIwEERKT_RKSt6locale;
extern int ftm_responder_cleanup;
extern int r_ble_ll_adv_periodic_set_data;
extern int bluetooth_hid_device_is_connected;
extern int phy_pbus_rd_addr;
extern int app_manager_stop;
extern int mac_tx_set_plcp1;
extern int ecp_mul_restartable_internal;
extern int rtc_clk_set_cpu_switch_to_pll;
extern int wpa_sm_disassociate;
extern int pm_on_isr;
extern int _ZTVNSt7__cxx117collateIwEE;
extern int lvgl_software_keyboard_is_enabled;
extern int wifi_nvs_deinit;
extern int ieee80211_ccmp_decrypt;
extern int ieee80211_output_init;
extern int __wrap___register_frame;
extern int lv_draw_mask_rect_dsc_init;
extern int esp_random;
extern int mbedtls_mpi_exp_mod;
extern int i2s_alloc_dma_desc;
extern int WIFI_MESH_EVENT;
extern int ieee80211_parse_vhtopr;
extern int phy_pbus_set_dco;
extern int esp_crypto_aes_gcm_abort;
extern int lvgl_toolbar_add_text_button_action;
extern int phy_get_romfuncs;
extern int r_ble_ll_hci_send_dir_adv_report;
extern int lv_area_set_width;
extern int pm_enable_beacon_monitor_timer;
extern int mld6_joingroup;
extern int r_ble_ll_conn_update_new_conn_param;
extern int esp_err_to_name;
extern int nan_scan_timeout_process;
extern int gpio_output_disable;
extern int hostapd_config_free_bss;
extern int r_ble_lll_sleep_enable;
extern int ble_midi_get_active;
extern int r_ble_lll_sync_rx_pkt_in;
extern int r_ble_ll_set_ext_scan_params;
extern int xPortSetInterruptMaskFromISR;
extern int lv_obj_transform_point;
extern int http_utils_trim_whitespace;
extern int esp_http_client_set_post_field;
extern int ble_hs_hci_deinit;
extern int coex_schm_ble_mesh_traffic_bt_a2dp_wifi_conn;
extern int mmu_hal_paddr_to_vaddr;
extern int _Z30ble_scan_abort_name_resolutionP6Device;
extern int lv_fs_init;
extern int r_ble_ll_utils_calc_num_used_chans;
extern int r_ble_ll_conn_auth_pyld_timer_start;
extern int phy_ble_power_config;
extern int event_group_wait;
extern int lv_screen_load;
extern int lv_obj_scrollbar_invalidate;
extern int ccmp_decrypt;
extern int lv_label_set_long_mode;
extern int chm_is_at_home_channel;
extern int r_ble_lll_mmgmt_free_rx_buffer;
extern int lv_list_class;
extern int esp_psram_extram_add_to_heap_allocator;
extern int r_ble_lll_mmgmt_free_tx_buffer_hdr_chain;
extern int ledc_set_fade;
extern int r_ble_lll_conn_reset_link_state;
extern int st7796_driver;
extern int wifi_nvs_get_low_rate_enable;
extern int __cxa_begin_catch;
extern int pm_rx_beacon_process;
extern int mbedtls_threading_psa_globaldata_mutex;
extern int lv_inv_area;
extern int r_ble_ll_scan_is_inside_window;
extern int mbedtls_ssl_transform_init;
extern int adc2_wifi_release;
extern int tcp_slowtmr;
extern int ieee80211_adjust_2nd_chan;
extern int r_bt_rf_coex_hook_st_set;
extern int r_ble_lll_adv_recycle_adv_event_end;
extern int coex_schm_ble_mesh_traffic_bt_a2dp_wifi_connecting;
extern int vSemaphoreDeleteWithCaps;
extern int lodepng_decode_memory;
extern int scan_reset_cipher_and_akm;
extern int esp_sha256_driver_abort;
extern int lv_obj_get_index_by_type;
extern int lwip_read;
extern int r_bt_rf_coex_dft_pti_set;
extern int mbedtls_x509_oid_get_sig_alg;
extern int dhcp_supplied_address;
extern int r_ble_ll_ctrl_channel_class_info_update;
extern int spi_bus_lock_bg_check_dev_req;
extern int icmp_dest_unreach;
extern int esp_libc_timekeeping_get_realtime_us;
extern int modem_clock_hal_deselect_all_coex_lpclk_source;
extern int lvgl_keyboard_on_stop_lvgl;
extern int bundle_has_int32;
extern int rtc_gpio_init;
extern int _ZTVSt8time_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE;
extern int bt_agc_v2_set;
extern int hmac_md5_vector;
extern int crypto_ec_prime_len_bits;
extern int uart_driver_install;
extern int _double_byte;
extern int phy_band_i2c_set;
extern int httpd_uri_match_wildcard;
extern int r_ble_ll_ctrl_rx_conn_param_req;
extern int netif_list;
extern int uxTopUsedPriority;
extern int crypto_ec_get_order;
extern int esp_cmac_update;
extern int gpio_isr_handler_remove;
extern int wifi_nan_reg_rxcb;
extern int driver_get_device_type;
extern int xQueueCreateMutex;
extern int i2c_master_read_from_device;
extern int r_ble_lll_conn_coex_dpc_process;
extern int lv_color_hsv_to_rgb;
extern int pointer_get_swap_xy;
extern int r_ble_phy_rate_to_phy;
extern int properties_file_close;
extern int g_msys_ctrl_conn_mbuf_cnt;
extern int ieee80211_is_tx_allowed;
extern int ESP_EFUSE_RD_DIS_BLOCK_KEY2;
extern int _ZTVSt17__timepunct_cacheIwE;
extern int heap_caps_calloc;
extern int g_pm_cfg;
extern int g_mesh_self_organized;
extern int pwr_hal_set_mac_modem_state_wakeup_protect_early_time;
extern int service_instance_get_data;
extern int hmac_hal_next_block_normal;
extern int ieee80211_find_ie;
extern int sd_pwr_ctrl_set_io_voltage;
extern int esp_wifi_get_spp_attrubute_internal;
extern int _ZTVSd;
extern int scan_stack_enable;
extern int ieee80211_sta_disconnect;
extern int crypt_encrypt;
extern int gpio_matrix_output;
extern int uxQueueMessagesWaitingFromISR;
extern int r_ble_ll_task;
extern int esp_cpu_intr_get_desc;
extern int mbedtls_x509_crt_init;
extern int psa_verify_hash;
extern int r_ble_lll_sched_has_been_executed;
extern int _ZSt7getlineIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EES4_;
extern int esp_psram_init;
extern int lv_obj_init_draw_label_dsc;
extern int lv_dpx;
extern int ieee80211_deauth_construct;
extern int spi_bus_lock_touch;
extern int coex_hw_timer_set;
extern int nvs_get_i8;
extern int sms4;
extern int hal_set_wifi_default_pti;
extern int i2s_channel_register_event_callback;
extern int lv_strdup;
extern int pm_twt_on_tsf_timer;
extern int efuse_hal_get_major_chip_version;
extern int g_wdev_record_t1t4_cb;
extern int r_ble_ll_ctrl_txpwr_set;
extern int ble_midi_stop_keepalive;
extern int esp_sleep_pd_config;
extern int ieee80211_add_hecap;
extern int adiList_stack_initEnv;
extern int mspi_timing_enter_high_speed_mode;
extern int _ZTVNSt7__cxx119money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEE;
extern int lv_obj_set_style_transform_pivot_y;
extern int r_ble_ll_ctrl_datalen_upd_make;
extern int mbedtls_calloc;
extern int lora_set_parameter;
extern int usb_device_controller_begin_claim;
extern int rcUpdatePhyMode;
extern int phy_reg_init_new;
extern int lv_obj_move_to;
extern int r_ble_lll_scan_env_deinit;
extern int tkip;
extern int wDev_reset_bcnSendTick;
extern int lv_font_get_line_height;
extern int arr_stack_enableMultiConnVsCmd;
extern int _ZTSSt8bad_cast;
extern int lv_layer_reset;
extern int hal_set_sta_tsf_wakeup;
extern int esp_clk_tree_rc_fast_get_freq_hz;
extern int cJSON_Parse;
extern int ieee80211_btwt_teardown_txcb;
extern int lv_fs_drv_init;
extern int hal_enable_tsf_timer_wakeup;
extern int wdev_is_mgmt_hwdecr_enabled;
extern int cJSON_IsString;
extern int _ZZNKSt8__format14__formatter_fpIcE6formatIdNS_10_Sink_iterIcEEEENSt20basic_format_contextIT0_cE8iteratorET_RS7_ENKUlPcSB_E_clESB_SB_;
extern int lv_image_cache_is_enabled;
extern int http_parser_pause;
extern int pmu_sleep_disable_regdma_backup;
extern int __wrap_lv_button_create;
extern int mbedtls_ccm_set_lengths;
extern int esp_tls_internal_event_tracker_create;
extern int _ZSt22__verify_grouping_implPKcjS0_j;
extern int esp_flash_app_init_os_functions;
extern int pm_update_params;
extern int esp_wifi_send_mgmt_frm_internal;
extern int rv_core_critical_regs_restore;
extern int mbedtls_ecdsa_sign_det_ext;
extern int lv_indev_get_driver_data;
extern int wcslcpy;
extern int r_ble_lll_adv_sec_chan_pdu_make;
extern int esp_sha1_driver_abort;
extern int pp_register_net80211_tx_cb;
extern int phy_set_chan_interp;
extern int modem_clock_device_context;
extern int ble_gatts_reset;
extern int hid_report_map_mouse;
extern int ble_freertos_sem_pool;
extern int timer_free;
extern int r_ble_ll_calc_ticks_per_slot;
extern int multi_heap_get_info;
extern int esp_http_client_symbols;
extern int r_ble_hw_cte_get_max_buffer_number;
extern int mbedtls_md_setup;
extern int r_ble_ll_pdu_tx_cte_time_get;
extern int ble_store_overflow_event;
extern int lv_button_class;
extern int dns_gethostbyname_addrtype_n;
extern int __locale_names;
extern int spi_flash_encryption_hal_check;
extern int lv_draw_buf_goto_xy;
extern int g_he_max_apep_length_tab;
extern int _ZTVSt18__moneypunct_cacheIcLb0EE;
extern int pbuf_realloc;
extern int r_ble_ll_mtrack_reset;
extern int r_ble_lll_append_rx_buffer;
extern int supplicant_sta_conn_handler;
extern int pmu_hal_hp_set_sleep_modem_backup_disable;
extern int SDCARD_TYPE;
extern int hal_he_set_multi_bssid;
extern int lv_obj_set_style_grid_cell_row_pos;
extern int r_ble_ll_env_init;
extern int r_ble_ll_sync_reset;
extern int ble_att_clt_tx_read_blob;
extern int nvs_set_blob;
extern int phy_set_tx_gain_mem;
extern int pm_go_to_sleep;
extern int _ZTISt11logic_error;
extern int sta_reset_beacon_timeout;
extern int ppProcTxCallback;
extern int esp_http_client_cancel_request;
extern int adc_controller_read_raw;
extern int esp_test_get_rx_statistics;
extern int open;
extern int coex_schm_ble_mesh_traffic_wifi_connecting;
extern int s_tx_cacheq;
extern int lv_obj_scroll_to_view;
extern int crypto_bignum_addmod;
extern int POINTER_TYPE;
extern int http_header_get;
extern int bluetooth_serial_start;
extern int _stat_r;
extern int lv_anim_set_start_cb;
extern int wifi_destroy_nan;
extern int tcp_pcb_purge;
extern int xPortCheckValidListMem;
extern int r_ble_ll_conn_sm_find_available;
extern int r_ble_lll_scan_duration_period_timers_stop;
extern int esp_aes_crypt_cfb128;
extern int heap_caps_get_minimum_free_size;
extern int ieee80211_vht_get_vhtcap_ie;
extern int lv_malloc;
extern int uECC_vli_modMult_fast;
extern int r_ble_ll_modify_sca_action;
extern int esp_supplicant_deinit;
extern int r_ble_ll_scan_dup_check_ext;
extern int __dtox_engine;
extern int spi_flash_op_unlock;
extern int r_ble_ll_conn_calc_reliable_rssi;
extern int r_ble_ll_resolv_get_rpa_tmo;
extern int _ZTVN3nvs12NVSPartitionE;
extern int phy_set_rx_gain_table;
extern int mbedtls_rsa_rsassa_pss_verify_ext;
extern int psa_asymmetric_decrypt;
extern int ff_disk_write;
extern int pwr_hal_set_mac_modem_tbtt_auto_period_disable;
extern int netconn_prepare_delete;
extern int isdigit;
extern int _ZTVNSt7__cxx117collateIcEE;
extern int __double_computePow5;
extern int phy_get_target_pwr;
extern int r_ble_ll_mem_set;
extern int xPortCheckValidTCBMem;
extern int pm_on_tbtt;
extern int gpio_set_intr_type;
extern int ieee80211_ftm_attach;
extern int pm_beacon_offset_add_total_counter;
extern int spi_flash_check_and_flush_cache;
extern int hal_clr_itwt_pti;
extern int lwip_netconn_is_deallocated_msg;
extern int bluetooth_serial_read;
extern int tt_app_fileselection_start_for_existing_file;
extern int esp32_spi_driver;
extern int x_side_default;
extern int ieee80211_crypto_bip_encrypt_with_key;
extern int r_ble_ll_sync_terminate;
extern int coex_status_get;
extern int ip4_output_if_src;
extern int r_ble_ll_utils_calc_window_widening;
extern int esp_ble_register_bb_funcs;
extern int esp_efuse_utility_get_number_of_items;
extern int _ZGVNSt7collateIwE2idE;
extern int esp_wifi_scan_get_ap_records;
extern int etharp_request;
extern int lv_draw_label_iterate_characters;
extern int ble_bb_cte_set_cte_ant_switch_delay_rx_aod_2us_2m;
extern int ble_hs_id_use_addr;
extern int r_ble_ll_resolv_peer_rpa_any;
extern int esp_test_tx_process_complete;
extern int pmksa_cache_auth_deinit;
extern int gdma_ahb_hal_is_tx_link_switch_event_supported;
extern int os_msys_init;
extern int r_hal_timer_start_at;
extern int hal_mac_set_tb_max_pwr;
extern int spi_bus_lock_bg_req_exist;
extern int lmacProcessTxError;
extern int ble_hs_pvcy_irk_deinit;
extern int cnx_remove_all_rc;
extern int r_ble_ll_ctrl_proc_unk_rsp;
extern int phy_pbus_set_rxgain;
extern int _ZSt4swapIN2tt3app6apphub11AppHubEntryEENSt9enable_ifIXsrSt6__and_IJSt6__not_ISt15__is_tuple_likeIT_EESt21is_move_constructibleIS8_ESt18is_move_assignableIS8_EEE5valueEvE4typeERS8_SI_;
extern int ble_gatts_bonding_restored;
extern int wifi_auto_scan_set_paused;
extern int getenv;
extern int auth_sae_queue;
extern int r_ble_ll_conn_hci_disconnect_cmd;
extern int pm_coex_restart_timeout_process;
extern int mmgmt_enableRxbufOptFeature;
extern int wpa_sta_connect;
extern int lv_ll_get_tail;
extern int strlcpy;
extern int device_stop;
extern int esp_wifi_sta_get_ie;
extern int ble_att_clt_rx_read_blob;
extern int lv_image_get_src;
extern int r_ble_ll_resolv_set_local_rpa;
extern int lmacEndFrameExchangeSequence;
extern int _ZTSSt16invalid_argument;
extern int adc_hal_set_calibration_param;
extern int _ZTV11WL_Ext_Safe;
extern int phy_chip_set_chan;
extern int ieee80211_has_ht40_bss;
extern int _ZZNSt8__format15__formatter_intIcE11_M_do_parseERSt26basic_format_parse_contextIcENS_10_Pres_typeEENKUlvE0_clEv;
extern int _ZGVNSt8numpunctIwE2idE;
extern int wdev_bank_load;
extern int ble_store_write_rpa_rec;
extern int nus_tx_handle;
extern int esp_transport_list_init;
extern int lv_obj_init_draw_arc_dsc;
extern int empty_func_p_pconfg_r_void;
extern int esp_backtrace_print;
extern int display_get_gap_x;
extern int i2s_free_dma_desc;
extern int ppRegisterPromisRxCallback;
extern int pm_pend_noise_check;
extern int os_get_time;
extern int ble_sm_alg_ecc_init;
extern int ieee80211_proto_attach;
extern int r_ble_ll_set_public_addr;
extern int ble_hid_switch_profile;
extern int cnx_rc_update_age;
extern int mpi_hal_start_op;
extern int r_ble_ll_ctrl_proc_start;
extern int r_mem_split_frag;
extern int phy_pbus_debugmode;
extern int wDev_GetNoiseFloor;
extern int toupper;
extern int temperature_sensor_hal_get_raw_value;
extern int r_ble_ll_sync_list_get_free;
extern int gpio_pulldown_dis;
extern int g_pm_twt;
extern int i2s_hal_tdm_set_rx_slot;
extern int xTaskGetCurrentTaskHandle;
extern int mbedtls_ecp_get_type;
extern int ic_set_rx_policy;
extern int lwip_init;
extern int r_ble_ll_sync_sm_clear;
extern int lv_textarea_class;
extern int coex_schm_ble_mesh_traffic_bt_piscan_wifi_scan;
extern int _ZTSSt20bad_array_new_length;
extern int _ZTVNSt7__cxx118messagesIcEE;
extern int esp_elf_request;
extern int phy_pbus_xpd_dpd_path;
extern int r_ble_ll_sync_check_cte_type_allowed;
extern int esp_libc_include_init_funcs;
extern int pvTaskGetThreadLocalStoragePointer;
extern int gpio_hal_iomux_out;
extern int sdmmc_select_driver_strength;
extern int esp_sha_hash_setup;
extern int r_ble_ll_calc_session_key;
extern int bt_bb_set_max_gain;
extern int lv_event_send;
extern int _ZSt24__throw_out_of_range_fmtPKcz;
extern int system;
extern int lv_style_builtin_prop_flag_lookup_table;
extern int pau_hal_set_regdma_work_timeout;
extern int wifi_set_rx_policy;
extern int mbedtls_eckey_info;
extern int r_exception_list_ble_mesh_filter_disable;
extern int nimble_port_stop;
extern int psa_sign_message_builtin;
extern int atanhf;
extern int mbedtls_mpi_inv_mod;
extern int _ZTVSt10moneypunctIwLb0EE;
extern int esp_libc_init;
extern int ieee80211_register_hostap_timer;
extern int _ZZNSt8__format15__formatter_strIcE5parseERSt26basic_format_parse_contextIcEENKUlvE0_clEv;
extern int gpio_descriptor_remove_callback;
extern int file_mutex_lock;
extern int esp_time_impl_get_boot_time;
extern int esp_event_handler_instance_unregister_with;
extern int phy_get_xtal_duty;
extern int spi_device_polling_start;
extern int ble_store_util_delete_peer;
extern int lv_style_set_pad_top;
extern int pm_go_to_wake;
extern int ieee80211_sta_connect;
extern int esp_libc_include_strncpy_impl;
extern int iswctype;
extern int coex_schm_external_coex_wifi_connecting;
extern int ble_gattc_rx_write_rsp;
extern int esp_netif_update_default_netif;
extern int r_ble_ll_ctrl_find_new_phy;
extern int r_bt_rf_coex_hook_init;
extern int _ZTIN10__cxxabiv115__forced_unwindE;
extern int __wrap___cxa_throw;
extern int crypto_ecdh_get_pubkey;
extern int ble_hs_conn_first;
extern int r_ble_ll_adv_periodic_estimate_data_itvl;
extern int r_ble_ll_hci_init_support_cmd_base_on_lmp_ver;
extern int _ZSt15__try_use_facetISt5ctypeIcEEPKT_RKSt6locale;
extern int ledc_periph_signal;
extern int lv_draw_sw_transform;
extern int r_ble_lll_sleep_env_init;
extern int tcp_close;
extern int hal_he_set_default_pe;
extern int r_os_msys_reset;
extern int spi_hal_enable_data_line;
extern int r_ble_ll_is_controller_busy;
extern int lv_draw_layer_alloc_buf;
extern int soc_get_available_memory_region_max_count;
extern int gpio_iomux_input;
extern int hal_crypto_set_key_entry;
extern int ble_l2cap_ctx;
extern int floor;
extern int os_mempool_unregister;
extern int _lock_acquire_recursive;
extern int r_ble_ll_df_connless_cte_tx_enable;
extern int EccPoint_isZero;
extern int phy_i2c_enter_critical;
extern int bundle_get_string;
extern int _Z19ble_set_scan_activeP6Deviceb;
extern int winWiden_stack_enableSetConstPeerScaVsCmd;
extern int coex_core_event_duration_get;
extern int app_manager_remove;
extern int display_set_gap;
extern int wlanif_init_ap;
extern int hal_timer_env_p;
extern int sync_stack_getBroker;
extern int ppReSendBar;
extern int esp_register_freertos_tick_hook_for_cpu;
extern int ic_register_promis_rx_cb;
extern int r_ble_phy_txpower_round;
extern int _ZTVNSt8__format9_Seq_sinkINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEE;
extern int _ZdaPv;
extern int esp_event_symbols;
extern int lv_obj_init_draw_line_dsc;
extern int npl_freertos_mutex_pend;
extern int lora_remove_rx_callback;
extern int httpd_req_get_hdr_value_len;
extern int _Z31getQrVersionForBinaryDataLengthjRi;
extern int ic_clear_data_ifx_tx_queue;
extern int hal_mac_disable_low_rate;
extern int spi_flash_hal_erase_chip;
extern int tsf_hal_get_tbtt_interval;
extern int ble_hs_hci_set_hci_version;
extern int key_mgr_hal_set_key_usage;
extern int ppProcessLifeTime;
extern int trc_get_80211_tx_rate;
extern int esp_transport_translate_error;
extern int pm_is_dream;
extern int ieee80211_vht_updateparams;
extern int esp_crypto_key_mgr_enable_periph_clk;
extern int xRingbufferCreateStatic;
extern int r_ble_ll_ctrl_conn_upd_make;
extern int lv_slider_set_value;
extern int esp_crypto_ecdsa_lock_acquire;
extern int esp_sha1_update;
extern int TxRxCxt;
extern int softap_choose_bss_color_without_conflict;
extern int spi_bus_get_max_transaction_len;
extern int _ZTVN2tt7service11displayidle18DisplayIdleServiceE;
extern int esp_psram_mspi_register_isr;
extern int hostap_auth_open;
extern int mbedtls_ccm_setkey;
extern int lv_snprintf;
extern int sdmmc_init_mmc_check_ext_csd;
extern int mbedtls_pk_wrap_psa;
extern int sdmmc_decode_cid;
extern int Cache_WriteBack_Addr;
extern int r_ble_lll_conn_append_tx_buffer;
extern int _ZTVSt11__timepunctIwE;
extern int phy_get_rx_pbus_freq;
extern int ble_midi_set_conn_handle;
extern int pmu_hp_system_retention_param_default;
extern int lv_group_focus_prev;
extern int esp_flash_noos_functions;
extern int hmac_md5;
extern int esp_sha_hash_update;
extern int esp_crypto_ds_lock_release;
extern int spi_flash_encryption_hal_disable;
extern int mpi_hal_write_to_mem_block;
extern int phy_stop_tx_tone;
extern int esp_vfs_include_nullfs_register;
extern int r_ble_lll_calc_us_convert_tick_unit;
extern int LORA_TYPE;
extern int xIsrStackBottom;
extern int g_pp_timer_info;
extern int esp_wifi_sta_prof_is_wapi_internal;
extern int ble_l2cap_sig_tx;
extern int phy_bias_reg_set;
extern int bluetooth_serial_get_device;
extern int uart_write_bytes;
extern int wctype;
extern int __retarget_lock_release_recursive;
extern int nan_dp_post_tx;
extern int __wrap__Unwind_Resume_or_Rethrow;
extern int ppSubFromAMPDU;
extern int wcsftime;
extern int multi_heap_register;
extern int pm_get_keep_alive_time;
extern int ble_hs_misc_own_addr_type_to_id;
extern int esp_vfs_closedir;
extern int chm_get_snd_chan_mode;
extern int r_ble_ll_adv_sm_free;
extern int esp_tls_conn_new_sync;
extern int ble_hs_misc_conn_chan_find;
extern int lv_obj_style_deinit;
extern int r_ble_lll_conn_sm_npl_init;
extern int wpa_auth_sta_associated;
extern int mbedtls_psa_ecp_generate_key;
extern int ble_l2cap_parse_hdr;
extern int r_ble_lll_scan_common_init;
extern int esp_crypto_ecc_lock_acquire;
extern int ic_get_80211_tx_rate_config;
extern int tcp_split_unsent_seg;
extern int uart_get_buffered_data_len;
extern int wl_mount;
extern int gpio_set_pull_mode;
extern int lvgl_pointer_add;
extern int lv_display_get_matrix_rotation;
extern int os_timer_disarm;
extern int npl_freertos_funcs_init;
extern int putwc_unlocked;
extern int esp_hmac_compute_opaque;
extern int http_utils_append_string;
extern int lv_style_set_text_color;
extern int uECC_vli_equal;
extern int gdma_hal_connect_peri;
extern int pp_timer_process;
extern int xQueueGiveMutexRecursive;
extern int mbedtls_aes_setkey_enc;
extern int acd_add;
extern int esp_flash_freq_unlimit_cb;
extern int __retarget_lock_acquire_recursive;
extern int r_esp_ble_msys_init;
extern int he_twt_information_resume;
extern int lv_obj_set_x;
extern int r_ble_ll_arr_postion_pick;
extern int g_wifi_improve_contention_ability;
extern int lv_obj_get_scroll_end;
extern int ext_funcs_ro;
extern int scan_add_bssid;
extern int r_os_mbuf_ctrl_mbuf_cnt_increase;
extern int __floatundidf;
extern int phy_bbpll_recal;
extern int lv_obj_get_scroll_left;
extern int crypto_ec_point_deinit;
extern int bluetooth_midi_get_device;
extern int lv_draw_sw_triangle;
extern int ESP_EFUSE_WR_DIS_BLOCK_KEY3;
extern int lv_color32_luminance;
extern int gps_model_to_string;
extern int rtc_gpio_is_valid_gpio;
extern int lmacProcessRxSucData;
extern int driver_unbind;
extern int ble_spp_get_active;
extern int netconn_connect;
extern int mld6_lookfor_group;
extern int _Z10app_ledgerv;
extern int ieee80211_is_ht_cipher;
extern int hal_mac_clr_txq_state;
extern int nvs_set_i32;
extern int ble_att_deinit;
extern int hal_mac_tsf_set_time;
extern int app_event_subscribe;
extern int esp_event_handler_unregister_with;
extern int sdmmc_send_cmd_set_relative_addr;
extern int g_hmac_cnt;
extern int mbedtls_ssl_hash_from_md_alg;
extern int psa_is_key_present_in_storage;
extern int wpa_parse_wpa_ie_rsnxe;
extern int r_ble_ll_scan_get_next_adv_prim_chan;
extern int phy_ant_dft_cfg;
extern int _ZGVNSt7num_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int r_ble_phy_env_init;
extern int ble_hs_get_rpa_timeout;
extern int lv_roller_label_class;
extern int esp_efuse_rtc_calib_get_init_code;
extern int lvgl_trackball_set_cursor_image;
extern int app_event_subscribe_with_app_id;
extern int ble_att_clt_rx_read_group_type;
extern int wifi_get_macaddr;
extern int hostap_recv_ctl;
extern int gpio_set_direction;
extern int coex_schm_bt_a2dp_paused_wifi_scan;
extern int vTaskStartScheduler;
extern int ieee80211_add_supportedchannels;
extern int ieee80211_is_vht_cipher;
extern int ptr_beacon_offset_funcs;
extern int tcp_tmr;
extern int lv_draw_buf_from_image;
extern int spi_flash_chip_winbond_probe;
extern int pm_on_isr_set_twt_target;
extern int r_ble_ll_pdu_max_tx_octets_get;
extern int __umul128;
extern int _Z21app_metadata_parse_v1RKSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_St4lessIS5_ESaISt4pairIKS5_S5_EEER11AppMetadata;
extern int ic_get_next_tbtt;
extern int r_ble_lll_adv_free_link_state;
extern int ic_enable_sniffer;
extern int esp_netif_get_handle_from_ifkey_unsafe;
extern int lv_obj_remove_state;
extern int httpd_sess_pending;
extern int r_ble_ll_conn_env_init;
extern int KEYBOARD_TYPE;
extern int bt_bb_cca_en;
extern int ble_sm_sc_io_action;
extern int hal_mac_get_txq_state;
extern int xQueueSemaphoreTake;
extern int gdma_ahb_hal_set_strategy;
extern int esp_rsa_ds_opaque_asymmetric_decrypt;
extern int lv_cache_entry_get_entry;
extern int rtc_clk_cal;
extern int phy_xtal_duty_set;
extern int i2c_controller_has_device_at_address;
extern int esp_nimble_enable;
extern int gpio_od_enable;
extern int lv_style_set_clip_corner;
extern int spi_flash_hal_gpspi_supports_direct_write;
extern int sae_clear_data;
extern int r_ble_ll_conn_hci_cancel_conn_complete_event;
extern int stpcpy;
extern int ieee80211_add_extcap;
extern int hostapd_get_hapd_data;
extern int esp_test_clr_rx_statistics;
extern int spi_flash_hal_gpspi_poll_cmd_done;
extern int pau_hal_regdma_link_count_config;
extern int sta_auth_open;
extern int pm_on_coex_schm_status_config;
extern int phy_i2c_master_mem_txcap;
extern int r_ble_vendor_hci_duplicate_set_param;
extern int mgd_probe_send_timeout_process;
extern int phy_track_pll;
extern int r_ble_lll_sync_transfer_sched;
extern int lwip_init_tcp_isn;
extern int wDev_IndicateBeaconMemoryFrame;
extern int lv_indev_set_display;
extern int s_tmp_itwt_id;
extern int coex_schm_ble_mesh_config_bt_piscan_wifi_connecting;
extern int lv_anim_delete_all;
extern int pbuf_alloc_reference;
extern int coex_schm_ble_mesh_config_wifi_connecting;
extern int esp_wifi_internal_tx;
extern int mbedtls_mpi_core_add;
extern int vEventGroupDelete;
extern int rtc_clk_cpu_freq_mhz_to_config;
extern int esp_clk_xtal_freq;
extern int lv_draw_buf_copy;
extern int r_ble_ll_adv_ext_set_param_v2;
extern int hal_he_set_ack_rate;
extern int _ZTVSt9money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE;
extern int esp_psram_extram_reserve_dma_pool;
extern int lv_display_get_default;
extern int esp_isr_names;
extern int memp_TCP_PCB_LISTEN;
extern int ic_disable_sniffer;
extern int dhcp_fine_timeout_cb;
extern int bluetooth_hid_device_get_device;
extern int FreeRTOS_openocd_params;
extern int usb_host_hid_subscribe;
extern int __mulPow5divPow2;
extern int _ZTVSt23_Sp_counted_ptr_inplaceIN2tt7service15ServiceManifestESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int phy_txtone_linear_pwr;
extern int gChmCxt;
extern int ieee80211_he_attach;
extern int r_ble_ll_hci_send_ext_adv_report;
extern int uart_hal_read_rxfifo;
extern int phy_chip_i2c_readReg;
extern int i2c_hal_master_init;
extern int pwr_hal_set_mac_modem_state_wakeup_protect_enable;
extern int spi_bus_lock_bg_clear_req;
extern int phy_module_enable;
extern int lv_buttonmatrix_set_button_ctrl_all;
extern int r_ble_ll_scan_aux_data_ref;
extern int r_ble_lll_sync_recycle_buffer;
extern int lv_obj_mark_layout_as_dirty;
extern int ic_reset_rx_ba;
extern int lv_draw_unit_draw_letter;
extern int lmacSetMuEDCAParam;
extern int r_ble_lll_scan_duration_period_timers_restart;
extern int ic_set_vif;
extern int esp_flash_chip_boya;
extern int lv_display_rotate_area;
extern int r_ble_ll_conn_sm_new;
extern int lv_keyboard_get_textarea;
extern int esp_transport_list_get_transport;
extern int esp_netif_receive;
extern int i2s_get_buf_size;
extern int lv_event_get_user_data;
extern int r_ble_lll_adv_periodic_set_data;
extern int bundle_put_string;
extern int esp_fp_print_backtrace;
extern int sta_try_sa_query_process;
extern int uECC_vli_mmod;
extern int lv_indev_set_driver_data;
extern int hal_crypto_clr_key_entry;
extern int xPortGetTickRateHz;
extern int _ZGVZ10app_ledgervE6ledger;
extern int phy_set_chan_reg;
extern int mbedtls_ccm_finish;
extern int wifi_configure_wake_window_process;
extern int netbuf_free;
extern int r_ble_lll_mmgmt_free_rx_buffer_hdr;
extern int tcpip_inpkt;
extern int audio_codec_get_capabilities;
extern int wpa_status_to_reason_code;
extern int esp_timer_impl_init_system_time;
extern int ble_ll_sync_env_p;
extern int ic_register_rx_cb;
extern int esp_lcd_new_panel_io_spi;
extern int mt_add_peer;
extern int wifi_api_lock;
extern int npl_freertos_event_deinit;
extern int spi_flash_hal_program_page;
extern int bluetooth_is_scanning;
extern int wdev_csi_hw_bug_check;
extern int mbedtls_ssl_write_client_hello;
extern int ble_hs_stop_init;
extern int esp_test_get_tx_statistics;
extern int phy_i2c_writeReg;
extern int r_ble_ll_hci_send_noop;
extern int r_ble_ll_arr_deinit;
extern int lv_color_brightness;
extern int div;
extern int esp_wifi_send_deauth_local;
extern int lv_cache_entry_get_size;
extern int ff_diskio_register_sdmmc;
extern int lv_spangroup_set_mode;
extern int lv_bar_set_start_value;
extern int lv_obj_clean;
extern int ble_l2cap_sig_reject_tx;
extern int phy_get_pwdet_offset_new;
extern int wifi_module_enable;
extern int sae_write_confirm;
extern int lv_obj_remove_local_style_prop;
extern int ic_reset_tbtt;
extern int lv_menu_cont_class;
extern int uart_vfs_dev_register;
extern int esp_intr_noniram_disable;
extern int spi_bus_lock_bg_request;
extern int bt_osi_mem_malloc_internal;
extern int r_ble_lll_sleep_rm_timer;
extern int esp_wifi_bt_power_domain_on;
extern int phy_get_romfunc_addr;
extern int ble_sm_sc_deinit;
extern int xEventGroupGetStaticBuffer;
extern int udp_new_ip_type;
extern int esp32_pwm_ledc_driver;
extern int ff_diskio_is_registered;
extern int ble_gattc_rx_read_blob_rsp;
extern int __kernel_cos;
extern int ble_hs_pvcy_set_our_irk;
extern int conn_stack_enable;
extern int hal_enable_sta_tbtt;
extern int wifi_nvs_get;
extern int display_get_resolution_x;
extern int phy_abs_temp;
extern int mbedtls_x509_crt_verify_restartable;
extern int mumimo_spatial_cfg_get_nsts;
extern int wdev_mac_special_reg_load;
extern int esp_libc_include_heap_impl;
extern int i2c_master_write_byte;
extern int lv_arc_set_mode;
extern int wifi_nvs_validate_sta_password;
extern int r_ble_ll_scan_backup_info_from_sm;
extern int ieee80211_regdomain_update_in_scan;
extern int r_ble_lll_sched_init;
extern int __lock___at_quick_exit_mutex;
extern int mbedtls_rsa_check_privkey;
extern int ble_lll_module_env_p;
extern int is_ndpa_to_dut;
extern int scan_profile_check;
extern int r_ble_ll_hci_vendor_register;
extern int vQueueDelete;
extern int r_ble_ll_conn_get_min_dura_required;
extern int phy_xtal_reg_set;
extern int pwm_get_period;
extern int spi_flash_chip_gd_suspend_cmd_conf;
extern int r_bt_rf_coex_cfg_set;
extern int mbedtls_pk_sign_restartable;
extern int _ZTISt12out_of_range;
extern int r_ble_lll_sched_insertion_end;
extern int he_send_action_twt_information;
extern int phy_rxiq_scale_set;
extern int gpio_pulldown_en;
extern int adiList_stack_listDump;
extern int httpd_is_sess_available;
extern int esp_libc_include_pthread_impl;
extern int apm_hal_set_master_sec_mode_all;
extern int spi_bus_lock_bg_check_dev_acq;
extern int xPortInIsrContext;
extern int r_ble_hci_trans_env_deinit;
extern int __bufio_flush;
extern int ble_store_config_persist_cccds;
extern int esp_coex_common_timer_done_wrapper;
extern int wcsnlen;
extern int s_update_cache_state;
extern int sdmmc_read_sectors_dma;
extern int set_assoc_ie;
extern int pm_extend_tbtt_adaptive_deattach;
extern int wpa_sm_set_ap_rsnxe;
extern int wpa_eapol_key_send;
extern int psa_its_get_info;
extern int lv_bar_create;
extern int phy_get_vco_init_new;
extern int _ZTISt9bad_alloc;
extern int ble_transport_to_ll_acl_impl;
extern int gdma_ahb_hal_set_priority;
extern int lv_list_get_button_text;
extern int esp_wifi_ap_get_prof_pairwise_cipher_internal;
extern int ieee80211_btwt_setup_txcb;
extern int esp_log_timestamp;
extern int xStreamBufferGenericCreateStatic;
extern int __fixsfdi;
extern int coex_schm_bt_default_wifi_connecting;
extern int _ZTv0_n12_NSoD0Ev;
extern int gdma_default_tx_isr;
extern int thread_free;
extern int ble_att_clt_rx_indicate;
extern int lv_draw_sw_blend_image_to_al88;
extern int phy_rxiq_set_reg;
extern int ble_gap_rx_data_len_change;
extern int esp_hmac_update_transparent;
extern int lv_bar_get_max_value;
extern int lwip_cyclic_timers;
extern int sinf;
extern int r_ble_lll_mmgmt_rxbuffer_allocate;
extern int rcTxUpdatePer;
extern int get_local_fd;
extern int mbedtls_asn1_free_named_data_list_shallow;
extern int lv_textarea_set_cursor_click_pos;
extern int bundle_put_int32;
extern int vTimerSetTimerID;
extern int mbedtls_mpi_core_montmul;
extern int ieee80211_amsdu_negotiate;
extern int esp_wifi_vnd_lora_enable;
extern int r_ble_hw_rng_start;
extern int __wrap_gpio_config;
extern int RGB_LED_TYPE;
extern int ds_hal_busy;
extern int ieee80211_output;
extern int lv_cache_destroy;
extern int r_ble_ll_conn_get_active_conn;
extern int mbedtls_mpi_sub_mpi;
extern int ble_ll_conn_subrate_req_llcp;
extern int esp_wifi_disconnect;
extern int backlight_get_max_brightness;
extern int lodepng_color_mode_copy;
extern int r_ble_phy_set_dev_address;
extern int netif_find;
extern int lv_span_stack_init;
extern int phy_txiq_set_reg;
extern int phy_get_i2c_read_mask_;
extern int g_espnow_user_oui;
extern int lv_text_encoded_prev;
extern int pm_enable_sta_disconnected_power_management;
extern int ESP_EFUSE_KEY_PURPOSE_1;
extern int hal_he_get_bss_color;
extern int esp_http_client_reset_redirect_counter;
extern int r_ble_ll_adv_ext_set_scan_rsp;
extern int esp_log_early_timestamp;
extern int scan_get_scan_id;
extern int coex_schm_external_coex_wifi_default;
extern int r_ble_ll_adv_reset;
extern int _ZZNSt13basic_filebufIcSt11char_traitsIcEE5closeEvEN14__close_sentryD2Ev;
extern int esp_libc_include_syscalls_impl;
extern int empty_func_p_u8_r_void;
extern int esp_test_baparas_support_amsdu;
extern int pp_create_task;
extern int sys_thread_sem_init;
extern int ble_hs_hci_ctx_free;
extern int _ZTv0_n12_NSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED0Ev;
extern int lv_obj_set_style_text_opa;
extern int _ZTVN3nvs19NVSPartitionManagerE;
extern int mbedtls_gcm_crypt_and_tag;
extern int ble_l2cap_sig_reject_invalid_cid_tx;
extern int r_ble_ll_conn_set_csa;
extern int ieee80211_send_action;
extern int lv_indev_get_point;
extern int r_ble_ll_hci_rd_local_supp_cmd;
extern int mbedtls_ssl_handshake_wrapup;
extern int file_mutex_get;
extern int esp_sleep_gpio_include;
extern int hal_he_set_ac_muedca_param;
extern int start_select;
extern int i2s_hal_init;
extern int r_ble_ll_adv_active_chanset_set_pri;
extern int cnx_get_authtype_strength;
extern int r_ble_lll_sched_deletion_begin;
extern int lv_draw_buf_create;
extern int r_ble_lll_mmgmt_global_block_free;
extern int lv_draw_sw_blend_color_to_i1;
extern int minmea_scan;
extern int _Z29tusbStartMassStorageWithFlashb;
extern int mbedtls_mpi_core_gcd_modinv_odd;
extern int raw_sendto;
extern int qrcode_getBufferSize;
extern int __fixunssfsi;
extern int lmacProcessShortRetryFail;
extern int phy_i2c_sdm_init;
extern int lv_obj_refr_pos;
extern int i2s_gpio_loopback_set;
extern int _ZGVNSt11__timepunctIwE2idE;
extern int r_ble_ll_mem_memblock_get_cb;
extern int lv_spangroup_get_mode;
extern int mbedtls_pk_sign;
extern int r_os_mbuf_len;
extern int pm_extend_tbtt_adaptive_stop;
extern int esf_buf_alloc;
extern int r_ble_ll_scan_parse_auxptr;
extern int r_ble_lll_recycle_overlapped_sch;
extern int lv_color_to_u32;
extern int spicommon_bus_alloc;
extern int rcClearCurStat;
extern int esp_http_client_get_chunk_length;
extern int _daylight;
extern int vTaskPrioritySet;
extern int minitar_handle_panic;
extern int systimer_ticks_to_us;
extern int esp_task_wdt_impl_timer_restart;
extern int psa_hash_compute;
extern int wifi_set_mode_process;
extern int iswspace;
extern int _Z23ble_hid_get_host_activeP6Device;
extern int RC_SetBasicRate;
extern int _mtvt_table;
extern int pm_funcs_init;
extern int esp_wifi_set_channel;
extern int coex_schm_ble_default_bt_idle_wifi_scan;
extern int esp_crypto_dpa_protection_startup;
extern int coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_connecting;
extern int r_DB_addr_list_remove;
extern int tsf_hal_set_tsf_time_deviation_sync_enable;
extern int phy_ant_bttx_cfg;
extern int mbedtls_base64_encode;
extern int lodepng_load_file;
extern int pm_mac_disable_tsf_tbtt_soc_wakeup;
extern int __floatsidf;
extern int esp_wifi_power_domain_on;
extern int pthread_mutex_init;
extern int pvPortMalloc;
extern int r_ble_lll_process_dequeued_sch;
extern int mbedtls_mpi_shrink;
extern int mbedtls_mpi_grow;
extern int lv_indev_wait_release;
extern int lv_draw_layer;
extern int coex_schm_ble_default_bt_a2dp_wifi_conn;
extern int rtc_isr_noniram_enable;
extern int crypto_ec_point_is_at_infinity;
extern int sleep_clock_system_retention_init;
extern int sha_hal_hash_block;
extern int lv_indev_get_group;
extern int soc_memory_types;
extern int lmacProcessTxopQComplete;
extern int bluetooth_hid_device_start;
extern int hal_disable_tsf_timer;
extern int iswblank;
extern int uxTimerGetReloadMode;
extern int ieee80211_auth_construct;
extern int mbedtls_psa_rsa_sign_hash;
extern int esp_test_clr_tx_statistics;
extern int ledc_set_duty_with_hpoint;
extern int phy_set_modem_flag;
extern int mbedtls_psa_ecp_load_public_part;
extern int r_os_mbuf_pack_chains;
extern int mbedtls_md_info_from_type;
extern int psa_cipher_decrypt;
extern int mbedtls_asn1_get_len;
extern int r_ble_ll_conn_hci_le_start_encrypt;
extern int ieee80211_twt_teardown_all;
extern int httpd_resp_set_hdr;
extern int ampdu_dispatch_all;
extern int esp_psram_impl_get_available_size;
extern int esp_crt_verify_callback;
extern int hal_init;
extern int esp_libc_timekeeping_set_realtime_us;
extern int usb_midi_device_is_connected;
extern int lv_image_decoder_set_open_cb;
extern int lv_obj_center;
extern int xTaskCreatePinnedToCore;
extern int r_ble_ll_arr_assign;
extern int phy_set_lb_txiq;
extern int cnx_sta_connect_cmd;
extern int dhcps_get_hostname_on_mac;
extern int get_phy_version_str;
extern int r_ble_ll_resolv_list_read_size;
extern int ble_sm_init;
extern int ble_hs_hci_init;
extern int rcP2P11NSchedTbl;
extern int phy_get_lock;
extern int phy_loop_clk_en;
extern int USB_MSC_DEVICE_TYPE;
extern int hostap_delete_ptk;
extern int gdma_hal_read_intr_status;
extern int phy_magtrk_scale_set;
extern int lodepng_clear_icc;
extern int mbedtls_ccm_auth_decrypt;
extern int UART_CONTROLLER_TYPE;
extern int npl_freertos_hw_enter_critical;
extern int MPI_BLOCK_BASES;
extern int device_for_each;
extern int mbedtls_md_finish;
extern int lv_font_set_kerning;
extern int lv_spinbox_set_cursor_pos;
extern int ff_sdmmc_set_disk_status_check;
extern int temperature_sensor_hal_get_degree;
extern int wpa_sm_alloc_eapol;
extern int esp_wifi_ap_deauth_internal;
extern int wpa_auth_set_rsn_selection;
extern int esp_set_time_from_rtc;
extern int cnx_node_leave;
extern int __moddi3;
extern int rtc_clk_cpu_freq_set_config;
extern int ieee80211_crypto_gmac_decrypt;
extern int gpio_config;
extern int _ZTv0_n12_NSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEED1Ev;
extern int netif_default;
extern int ble_hs_conn_can_alloc;
extern int phy_txiq_get_mis_pwr;
extern int g_mesh_is_started;
extern int mbedtls_ssl_cipher_to_psa;
extern int ieee80211_add_ie_vendor_esp_now;
extern int phy_linear_to_db_64bits;
extern int __dtoa_engine;
extern int vTaskGetSnapshot;
extern int mbedtls_ecp_muladd;
extern int rtc_dig_clk8m_disable;
extern int ieee80211_he_node_init;
extern int lv_obj_set_style_min_height;
extern int ic_register_net80211_tx_cb;
extern int minitar_read_entry;
extern int ieee80211_parse_he_mu_edca;
extern int wpa_deinit;
extern int __isinfd;
extern int esp_aes_intr_alloc;
extern int i2s_hal_tdm_set_tx_slot;
extern int test_parse_rx_mu_mimo;
extern int mbedtls_rsa_rsassa_pkcs1_v15_verify;
extern int audio_codec_set_mute;
extern int esp_wifi_get_user_init_flag_internal;
extern int r_rtc0_timer_handler;
extern int ble_hs_unlock;
extern int mbedtls_rsa_rsassa_pss_sign_ext;
extern int itwt_setup_dwell_timeout_fn_process;
extern int netif_ip6_addr_set_state;
extern int esp_mbedtls_read;
extern int ieee80211_rate_ref_init;
extern int r_ble_ll_conn_eff_max_tx_rx_time_get;
extern int sta_eapol_txdone_cb;
extern int ble_hs_hci_util_set_random_addr;
extern int nan_dw_start_process;
extern int bt_agc_target_set;
extern int hmac_sha256_kdf;
extern int hal_he_clr_multi_bssid;
extern int pm_update_at_next_beacon;
extern int r_ble_ll_scan_send_truncated;
extern int pbuf_remove_header;
extern int modem_clock_hal_set_ble_rtc_timer_divisor_value;
extern int g_beacon_idx;
extern int rsn_pmkid;
extern int ic_set_interface;
extern int _ZTIN9__gnu_cxx26__concurrence_unlock_errorE;
extern int mbedtls_cipher_free;
extern int lv_slider_class;
extern int r_ble_log_hci_log_hex;
extern int lv_obj_get_scroll_x;
extern int r_ble_lll_sync_new;
extern int rv_core_critical_regs_save;
extern int app_internal_loader_service_manifest;
extern int ieee80211_itwt_teardown;
extern int display_get_backlight;
extern int pmksa_cache_clear_current;
extern int psa_copy_key;
extern int r_ble_ll_ctrl_ptr_check_enabled;
extern int httpd_sess_get;
extern int i2s_hal_pdm_set_rx_slot;
extern int coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_conn;
extern int r_ble_lll_mmgmt_update_rxbuf_insert_ptr;
extern int wpa_sm_set_ap_rsne_override;
extern int lv_spinbox_increment;
extern int phy_freq_to_chan;
extern int ieee80211_parse_uora;
extern int coex_deinit;
extern int usb_hid_device_send_consumer;
extern int lv_theme_get_font_normal;
extern int ble_hs_process_rx_data_queue;
extern int lv_dropdown_open;
extern int http_message_needs_eof;
extern int esp_wifi_sta_get_prof_password_internal;
extern int psa_hash_clone;
extern int s_ble_ll_arr_env;
extern int r_ble_lll_sched_get_realtime_info;
extern int r_ble_hw_cte_iq_sample_num_get;
extern int mbedtls_md_hmac_setup;
extern int ic_del_rx_ba;
extern int tlsf_memalign_offs;
extern int lv_arc_align_obj_to_angle;
extern int wpa_supplicant_send_2_of_4;
extern int freertos_symbols;
extern int modem_clock_hal_set_clock_domain_icg_bitmap;
extern int ble_hs_id_ctx_free;
extern int hal_he_set_tid_bitmap;
extern int __ffssi2;
extern int USB_HOST_MIDI_TYPE;
extern int wifi_get_protocol_process;
extern int gpio_hal_matrix_in;
extern int esp_sha_write_digest_state;
extern int esp_aes_process_dma;
extern int gpio_descriptor_release;
extern int pointer_set_swap_xy;
extern int wifi_check_and_handle_rmac_timer_pending;
extern int raw_input;
extern int ieee80211_getmgtframe;
extern int adiList_stack_setEnable;
extern int r_ble_lll_adv_sm_event_init;
extern int lvgl_keyboard_add;
extern int mbedtls_x509_crt_free;
extern int mbedtls_gcm_update_ad;
extern int phy_close_rf;
extern int phy_i2c_sdm_en;
extern int i2c_acquire_bus_handle;
extern int esp_aes_cipher_decrypt_setup;
extern int sdmmc_card_print_info;
extern int r_ble_ll_adv_aux_calculate_payload;
extern int xEventGroupGetBitsFromISR;
extern int temperature_sensor_power_acquire;
extern int g_startup_fn;
extern int s_fix_rate;
extern int lv_obj_set_style_bg_image_src;
extern int mbedtls_rsa_parse_key;
extern int r_ble_ll_hci_chk_phy_masks;
extern int httpd_resp_set_status;
extern int scan_pm_offchan;
extern int esp_now_deinit;
extern int coex_schm_bt_page_wifi_scan;
extern int esp_test_rx_parse_mu;
extern int gpio_descriptor_disable_interrupt;
extern int lv_obj_get_ext_draw_size;
extern int _rv_core_critical_regs_restore;
extern int mbedtls_ecp_keypair_free;
extern int lv_obj_redraw;
extern int mbedtls_x509_get_alg;
extern int phy_pbus_rd;
extern int hal_he_init;
extern int lv_obj_get_scroll_snap_x;
extern int btdm_broker_detach;
extern int esp_cache_err_has_active_err;
extern int sdmmc_init_cid;
extern int sys_mutex_lock;
extern int esp_flash_chip_generic;
extern int ieee80211_send_nulldata;
extern int audio_codec_get_mute;
extern int ppEnqueueTxDone;
extern int mbedtls_psa_crypto_free;
extern int ceil;
extern int httpd_sess_init;
extern int mac_tx_set_hesig;
extern int psa_to_md_errors;
extern int mbedtls_ssl_renegotiate;
extern int lv_obj_get_child_count;
extern int __wrap___register_frame_table;
extern int ieee80211_phy_type_get;
extern int esp_test_clr_rx_mu_statistics;
extern int sta_get_owe_data;
extern int tcp_tw_pcbs;
extern int ble_uuid_length;
extern int r_ble_ll_adv_update_did;
extern int ble_l2cap_sig_hdr_parse;
extern int netif_set_default;
extern int r_ble_ll_adv_set_data_related_addr_change;
extern int mbedtls_mpi_add_abs;
extern int spi_bus_lock_wait_bg_done;
extern int __wrap_lv_obj_create;
extern int pm_disable_keep_alive_timer;
extern int wifi_nvs_reset_current_ap_info;
extern int bootloader_flash_execute_command_common;
extern int lv_draw_get_available_task;
extern int i2c_del_master_bus;
extern int USB_MIDI_DEVICE_TYPE;
extern int cnx_remove_rc;
extern int esp_http_client_set_user_data;
extern int mbedtls_mpi_get_bit;
extern int r_ble_lll_sync_reschedule;
extern int strspn;
extern int mbedtls_aes_crypt_cfb128;
extern int esp_vfs_select;
extern int esp_libc_include_reent_syscalls_impl;
extern int r_ble_ll_conn_update_eff_data_len;
extern int esp_aes_dma_done;
extern int app_scheduler_start;
extern int _ZTVSt7num_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE;
extern int mbedtls_mpi_core_cond_assign;
extern int xQueueGenericReset;
extern int _ZGVNSt8messagesIwE2idE;
extern int ble_ll_conn_subrate_set;
extern int pm_get_tbtt_count;
extern int esp_cache_freeze_ext_mem_cache;
extern int __retarget_lock_acquire;
extern int ble_random_env_p;
extern int r_ble_ll_conn_callout_env_init;
extern int ieee80211_decrypt_espnow_pkt;
extern int ble_store_config_conf_init;
extern int lv_arc_set_bg_start_angle;
extern int lv_draw_sw_mask_deinit;
extern int ieee80211_is_robust_mgmt_frm;
extern int phy_wifi_fbw_sel;
extern int phy_pwdet_ref_code;
extern int ble_hs_id_addr;
extern int charge_pump_enable;
extern int wifi_on_coex_start_process;
extern int _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc;
extern int os_sleep;
extern int r_ble_ll_sync_list_size;
extern int _ZTTSd;
extern int esp_lcd_panel_invert_color;
extern int rc11BSchedTbl;
extern int esp_test_rx_trs_count;
extern int lv_obj_report_style_change;
extern int chm_return_home_channel;
extern int esp_netif_get_ifkey;
extern int ccmp;
extern int phy_i2c_sar2_init_code;
extern int scan_parse_beacon;
extern int i2c_master_write_read_device;
extern int phy_bt_tx_gain_set;
extern int coex_pti_tab;
extern int esp_log_va;
extern int r_ble_ll_ctrl_enc_allowed_pdu_tx;
extern int phy_vht_support;
extern int r_ble_ll_disconn_comp_event_send;
extern int lv_group_send_data;
extern int ESP_EFUSE_WR_DIS_KEY_PURPOSE_1;
extern int mbedtls_ssl_write_alpn_ext;
extern int lv_draw_sw_grad_cleanup;
extern int r_ble_lll_sched_delete;
extern int r_ble_lll_adv_sm_deinit;
extern int r_ble_lll_adv_get_sync_info_field;
extern int hal_set_ofdma_sequence_pti;
extern int esp_partition_find_err;
extern int esp_cache_get_alignment;
extern int uart_hal_txfifo_rst;
extern int gettimeofday;
extern int ESP_EFUSE_ADC1_CH4_ATTEN0_INITCODE_DIFF;
extern int g_wifi_default_wpa_crypto_funcs;
extern int test_rx_process_complete_noeb;
extern int ble_gatts_indicate_fail_notconn;
extern int clk_hal_cpu_get_freq_hz;
extern int esp_http_client_flush_response;
extern int phy_wifi_agc_sat_gain;
extern int ble_att_svr_rx_read_mult;
extern int r_ble_ll_mem_byte_to_word_size;
extern int esp_http_client_delete_header;
extern int lora_transmit;
extern int mbedtls_pk_verify;
extern int _ZZNSt13basic_filebufIcSt11char_traitsIcEE5closeEvEN14__close_sentryD1Ev;
extern int ieee80211_encap_esfbuf_htc;
extern int esp_libc_include_assert_impl;
extern int esp_tls_plain_tcp_connect;
extern int pm_coex_pwr_update;
extern int r_ble_ll_sync_deinit;
extern int ieee80211_softap_add_bss_color_change_announcement;
extern int lvgl_devices_attach;
extern int wifi_init_process;
extern int r_ble_lll_conn_sched_next_anchor;
extern int hal_get_tx_min_pwr;
extern int r_ble_ll_ctrl_tx_done;
extern int wifi_station_get_ipv4_address;
extern int app_manager_start_with_parameters;
extern int audio_codec_get_native_channels;
extern int ble_att_svr_rx_notify;
extern int rtcio_hal_iomux_output;
extern int g_msys_num_get_cb;
extern int esp_tls_conn_read;
extern int mt_deinit;
extern int mem_trim;
extern int lv_style_set_prop;
extern int lv_obj_transform_point_array;
extern int lv_label_set_text_selection_end;
extern int icmp_input;
extern int strcat;
extern int ieee80211_set_phy_mode;
extern int set_bb_wdg;
extern int rcGetSched;
extern int wifi_is_scanning;
extern int r_ble_ll_sync_transfer_received;
extern int g_adiList_stack_envP;
extern int r_ble_lll_get_rxed_buffer;
extern int lv_ll_is_empty;
extern int igmp_leavegroup_netif;
extern int g_log_level;
extern int mbedtls_cipher_finish_padded;
extern int esp_sleep_enable_gpio_switch;
extern int ieee80211_send_probereq;
extern int psa_generate_random;
extern int bluetooth_serial_stop;
extern int coex_core_timer_idx_get;
extern int lv_display_is_invalidation_enabled;
extern int r_ble_lll_sync_recycle_sch_item;
extern int lv_fs_close;
extern int r_ble_ll_adv_sync_pdu_ext_hdr_make;
extern int esp32_sdspi_fs_free;
extern int esp_wifi_start;
extern int lv_color_darken;
extern int esp_wifi_sta_get_reset_nvs_pmk_internal;
extern int lv_area_get_height;
extern int lv_obj_set_style_pad_right;
extern int phy_force_txrx_off_new;
extern int httpd_query_key_value;
extern int r_ble_ll_conn_get_anchor;
extern int http_parser_settings_init;
extern int i2c_reset_tx_fifo;
extern int esp_intr_alloc_info;
extern int esp_lcd_panel_init;
extern int rtc_gpio_iomux_input;
extern int phy_fe_reg_init;
extern int hal_mac_tx_config_edca;
extern int esp_sleep_config_gpio_isolate;
extern int npl_freertos_sem_deinit;
extern int feof;
extern int esp_test_rx_ctrl;
extern int hci_stack_disable;
extern int mbedtls_mpi_core_check_zero_ct;
extern int ppSetBarRate;
extern int ESP_EFUSE_WR_DIS_KEY_PURPOSE_5;
extern int sys_thread_tcpip;
extern int _ZSt7getlineIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EE;
extern int rgb_led_pwm_driver;
extern int lv_tabview_set_tab_bar_size;
extern int hmac_hal_next_block_padding;
extern int phy_get_tcode;
extern int _ZTVN2tt7service7rtctime14RtcTimeServiceE;
extern int r_ble_lll_adv_set_new_adv_data;
extern int mbedtls_x509_crt_parse;
extern int memory_calloc_with_policy;
extern int pwr_hal_clear_mac_modem_rx_beacon_miss_counter;
extern int lv_spangroup_get_max_line_height;
extern int r_ble_lll_mmgmt_rxbuf_cnt_get;
extern int r_ble_ll_mem_msys_update;
extern int lmacProcessCollisions_task;
extern int esp_vfs_unregister_fs_with_id;
extern int esp_http_client_set_url;
extern int ble_hs_unlock_nested;
extern int phy_gen_rx_gain_table;
extern int __bufio_buffer_allocate_locked;
extern int r_ble_lll_scan_chk_resume;
extern int rgb_led_gpio_driver;
extern int i2c_hal_master_set_scl_timeout_val;
extern int udp_connect;
extern int key_mgr_wait_for_state;
extern int gdma_ahb_hal_connect_peri;
extern int sta_michael_mic_failure;
extern int mbedtls_ecp_point_write_binary;
extern int device_set_driver;
extern int esp_coex_adapter_register;
extern int vTimerSetReloadMode;
extern int rcLoRaSchedTbl;
extern int wifi_nvs_validate_phymode;
extern int esp_timer_impl_set_alarm_id;
extern int lv_timer_ready;
extern int spi_flash_init_lock;
extern int pmksa_cache_auth_create_entry;
extern int r_ble_ll_arr_hci_scheduling_len_set;
extern int ic_get_default_sched;
extern int esp_wifi_get_config;
extern int hal_enable_rx_statistics;
extern int r_ble_ll_resolv_get_list;
extern int r_ble_vendor_hci_duplicate_control;
extern int xQueueGenericCreate;
extern int esp_netif_get_hostname;
extern int module_remove;
extern int file_mutex_register;
extern int vfwprintf;
extern int aes_unwrap;
extern int _ZZNKSt7__cxx117collateIwE12do_transformEPKwS3_EN4_BufD2Ev;
extern int pm_start_twt;
extern int esp_aes_setkey;
extern int lv_cache_entry_get_ref;
extern int spi_bus_get_attr;
extern int ic_get_key;
extern int pthread_key_create;
extern int __retarget_lock_release;
extern int lv_point_set;
extern int pp_printf;
extern int _Z29tusbStartMassStorageWithSdmmcb;
extern int bt_bb_rx_set;
extern int esp_fp_generated_step;
extern int _ZTVNSt7__cxx118numpunctIwEE;
extern int r_osi_coex_funcs_p;
extern int phy_enable_low_rate;
extern int remove;
extern int vApplicationGetIdleTaskMemory;
extern int lv_bar_class;
extern int hmac_hal_write_block_512;
extern int scan_hci_sendAdvLostReportVsEvent;
extern int pm_set_next_tbtt;
extern int gdma_hal_stop;
extern int esp_crypto_mpi_lock_release;
extern int ESP_EFUSE_KEY1;
extern int rmdir;
extern int file_system_for_each;
extern int _ZSt15__try_use_facetISt10moneypunctIwLb0EEEPKT_RKSt6locale;
extern int lv_style_set_shadow_opa;
extern int phy_track_pll_init;
extern int wifi_add_event_callback;
extern int ble_hs_conn_foreach;
extern int ieee80211_send_setup;
extern int spi_bus_lock_bg_exit;
extern int esp_vfs_fat_unregister_path;
extern int coex_schm_ble_mesh_traffic_bt_piscan_wifi_connecting;
extern int gpio_isr_handler_add;
extern int panic_print_backtrace;
extern int ppTxPkt;
extern int extAdv_stack_enable;
extern int net80211_data_ptr_init;
extern int tcp_free_ooseq;
extern int lv_menu_back_button_is_root;
extern int hal_clear_rx_beacon_pti;
extern int r_ble_log_internal_x1;
extern int lv_style_init;
extern int gdma_ahb_hal_disconnect_all;
extern int lv_style_set_pad_column;
extern int mbedtls_x509_get_key_usage;
extern int sdspi_host_start_command;
extern int r_ble_ll_conn_module_init;
extern int pm_rx_data_process;
extern int s_keys;
extern int check_comeback_token;
extern int usb_hid_device_send_gamepad;
extern int lv_indev_get_next;
extern int audio_codec_write;
extern int esp_transport_ssl_set_client_cert_data_der;
extern int hostap_init;
extern int lv_obj_set_style_line_width;
extern int ic_add_extra_softap_rx_ba;
extern int esp_aes_crypt_ecb;
extern int chm_get_current_band;
extern int psa_export_public_key;
extern int tcp_rexmit_rto_commit;
extern int psa_generate_key;
extern int lv_display_send_event;
extern int r_get_be32;
extern int pm_send_probe_start;
extern int wpa_sm_init;
extern int lv_draw_buf_reshape;
extern int __wtime_am_pm;
extern int lv_display_get_next;
extern int lv_style_set_transform_height;
extern int r_ble_ll_mem_head_check;
extern int g_mmgmt_max_block_cnt;
extern int usb_hid_device_is_connected;
extern int mbedtls_rsa_parse_pubkey;
extern int expm1f;
extern int _Z25app_ledger_free_argumentsiPPc;
extern int ble_att_clt_tx_exec_write;
extern int i2c_master_start;
extern int pwrsave_flushq;
extern int ble_att_clt_tx_indicate;
extern int huk_hal_configure;
extern int r_ble_ll_ctrl_phy_tx_transition_get;
extern int esp_aes_gcm_update_ad;
extern int _ZTSSt12out_of_range;
extern int tlsf_fit_size;
extern int file_system_remove;
extern int g_ble_ll_conn_evt_dura;
extern int __fixdfdi;
extern int tcp_send_empty_ack;
extern int lv_arc_set_end_angle;
extern int r_scan_duplicate_cache_refresh_timer_start;
extern int chm_check_channel_is_valid;
extern int regi2c_exit_critical;
extern int lv_textarea_set_accepted_chars;
extern int lv_draw_sw_blend_image_to_argb8888;
extern int putc_unlocked;
extern int _ZTVNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEE;
extern int minitar_checksum_header;
extern int r_ble_ll_scan_check_periodic_sync;
extern int wcscmp;
extern int ieee80211_he_updatecaps;
extern int _Z22app_metadata_get_valueRKSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_St4lessIS5_ESaISt4pairIKS5_S5_EEERS9_RS5_;
extern int ieee80211_psq_drop_one_pkt;
extern int ble_ll_hci_env_p;
extern int esp_vfs_unregister_with_id;
extern int _ZSt7__writeIwESt19ostreambuf_iteratorIT_St11char_traitsIS1_EES4_PKS1_i;
extern int xTaskCheckForTimeOut;
extern int ESP_EFUSE_KEY5;
extern int adc_oneshot_hal_init;
extern int ieee802_11_parse_elems;
extern int xRingbufferCreate;
extern int mbedtls_md_hmac_starts;
extern int uECC_vli_set;
extern int r_os_mbuf_leadingspace;
extern int bluetooth_get_paired_peers;
extern int mbedtls_mpi_core_fill_random;
extern int _ZSt15__try_use_facetISt8numpunctIcEEPKT_RKSt6locale;
extern int mbedtls_psa_ffdh_generate_key;
extern int lv_obj_get_screen;
extern int crypto_bignum_rand;
extern int pm_register_connectionless_wake_window;
extern int ble_gattc_rx_read_type_complete;
extern int wifi_bt_common_module_enable;
extern int lwip_netconn_do_delconn;
extern int lv_color32_eq;
extern int heap_caps_aligned_calloc;
extern int hmac_hal_clean;
extern int r_ble_controller_enable;
extern int lv_indev_set_read_cb;
extern int zb_rx_v0_0_set;
extern int lv_display_remove_event_cb_with_user_data;
extern int get_suspend_sp_number;
extern int mbedtls_sha256;
extern int r_ble_ll_whitelist_deleted_irk_store;
extern int ble_hs_hci_add_avail_pkts;
extern int phy_txdc_cal_init;
extern int esp_aes_acquire_hardware;
extern int _he_twt_information_resume;
extern int ble_gap_slave_adv_reattempt;
extern int crypto_ec_point_to_bin;
extern int ppSelectTxFormat;
extern int phy_pbus_xpd_rx_off;
extern int gdma_ahb_hal_init;
extern int _Z14get_native_pinP14GpioDescriptor;
extern int setup_timer_param;
extern int _unlink_r;
extern int npl_freertos_eventq_put_to_front;
extern int r_ble_lll_ext_scan_coex_dpc_process;
extern int r_ble_lll_sched_pop_overlapped_sch;
extern int prefix_list;
extern int adc_oneshot_hal_convert;
extern int port_uxInterruptNesting;
extern int g_wdev_dbg_rx;
extern int memory_print_stats;
extern int lv_display_get_theme;
extern int mbedtls_ecp_point_cmp;
extern int esp_crypto_aes_enable_periph_clk;
extern int esp_vfs_fat_spiflash_mount_rw_wl;
extern int xEventGroupWaitBits;
extern int wifi_mesh_event_post;
extern int device_construct_add;
extern int lv_obj_set_style_text_letter_space;
extern int coex_schm_ble_mesh_traffic_bt_a2dp_wifi_scan;
extern int ble_bb_cte_set_cte_ant_switch_delay_rx_aod_1us;
extern int _putc2;
extern int ieee80211_get_tx_info_from_eb;
extern int device_try_lock;
extern int modem_clock_configure_wifi_status;
extern int wl_clear_ap_no_lr;
extern int esp_bt_controller_enable;
extern int xStreamBufferSpacesAvailable;
extern int wpabuf_free;
extern int empty_func_p_pvoid_r_void;
extern int lv_obj_set_scroll_dir;
extern int spi_flash_hal_disable_auto_suspend_mode;
extern int ble_hs_lock_nested;
extern int pwm_backlight_driver;
extern int rx11AXRate2AMPDULimit;
extern int __fpclassifyf;
extern int lv_draw_sw_mask_apply;
extern int lv_strcmp;
extern int hid_report_map_keyboard_consumer_mouse;
extern int os_timer_setfn;
extern int _ZTVSt12out_of_range;
extern int _ZSt15get_new_handlerv;
extern int getcwd;
extern int ieee80211_ampdu_start_age_timer;
extern int coex_disable;
extern int pointer_exit_sleep;
extern int xTaskGetCoreID;
extern int cnx_sta_color_change_fn;
extern int r_ble_lll_sched_unlock_safely;
extern int scan_set_pas_duration;
extern int empty_func_p_void_r_void;
extern int hal_mac_is_dma_enable;
extern int pm_beacon_monitor_tbtt_stop;
extern int i2c_master_stop;
extern int panic_soc_fill_info;
extern int gdma_del_channel;
extern int device_for_each_of_type;
extern int sdmmc_mmc_get_erase_timeout_ms;
extern int rcGetAmpduSched;
extern int __ledf2;
extern int esp_cache_err_int_init;
extern int phy_mac_enable_bb;
extern int lv_font_get_glyph_width;
extern int lv_strndup;
extern int mbedtls_psa_rsa_import_key;
extern int lv_display_flush_is_last;
extern int btdm_broker_env_init;
extern int esp_netif_free_rx_buffer;
extern int lv_menu_main_header_cont_class;
extern int lv_list_create;
extern int coex_schm_ble_mesh_traffic_bt_conn_wifi_conn;
extern int r_ble_log_internal_hex;
extern int lv_anim_set_reverse_delay;
extern int r_ble_lll_mmgmt_get_mempool_size;
extern int task_wdt_timeout_abort;
extern int wpa_auth_gen_wpa_ie;
extern int esp32_i2c_master_driver;
extern int ic_del_ptk_and_gtk_alg;
extern int ble_bb_cte_set_cte_samp_limit_samp;
extern int hal_vhtdlmu_set_membership_status;
extern int pmksa_cache_flush;
extern int tsf_hal_set_tbtt_modem_wakeup_enable;
extern int esp_event_loop_run;
extern int adiList_stack_listAdd;
extern int r_ble_lll_sync_coex_dpc_update;
extern int uart_controller_read_byte;
extern int npl_freertos_sem_get_count;
extern int wifi_create_softap;
extern int ble_ll_conn_subrate_req_hci;
extern int USB_HOST_HID_TYPE;
extern int gpio_reset_pin;
extern int ieee80211_setup_phy_mode;
extern int __wrap_gpio_set_direction;
extern int esp_wifi_sta_get_mgmt_group_cipher;
extern int setenv;
extern int ic_ebuf_recycle_tx;
extern int ble_store_full_event;
extern int rgb_led_set_color;
extern int hal_crypto_disable;
extern int lmacDisableTransmit;
extern int esp_hmac_setup_opaque;
extern int psa_allocate_buffer_to_slot;
extern int USB_HID_DEVICE_TYPE;
extern int bundle_opt_int32;
extern int r_ble_ll_customize_peer_sca_set;
extern int ble_store_read;
extern int wpa_crypto_funcs;
extern int strcasecmp;
extern int sync_stack_disable;
extern int nan_call_ndp_confirm_cb;
extern int ampdu_dispatch;
extern int extAdv_stack_disable;
extern int load_kern;
extern int r_ble_ll_adv_rpa_timeout;
extern int _kill_r;
extern int netconn_write_vectors_partly;
extern int esp_transport_connect_async;
extern int _ZSt20__throw_length_errorPKc;
extern int cJSON_IsNumber;
extern int lv_color_hex;
extern int esp_sha_hash_clone;
extern int phy_rfpll_set_freq;
extern int lv_arc_set_start_angle;
extern int lv_textarea_set_password_show_time;
extern int esp_cache_suspend_ext_mem_cache;
extern int lv_lodepng_init;
extern int __assert_func;
extern int lv_obj_get_content_width;
extern int dh_groups_get;
extern int __wrap_lv_switch_create;
extern int r_ble_ll_callout_env_init;
extern int r_ble_hw_periodiclist_rmv;
extern int ble_sm_enc_initiate;
extern int _ZZNSt8__format14__formatter_fpIcE5parseERSt26basic_format_parse_contextIcEENKUlvE0_clEv;
extern int os_memdup;
extern int mbedtls_mutex_unlock;
extern int esp_netif_action_connected;
extern int _ZGVNSt7__cxx118numpunctIwE2idE;
extern int lv_obj_create;
extern int make_hw_cmd;
extern int device_has_active_by_type;
extern int psa_parse_key_data_from_storage;
extern int ble_gatts_tx_notifications;
extern int lv_obj_get_content_height;
extern int ble_bb_cte_set_cte_ant_switch_delay_rx_aoa_2us;
extern int hostap_query_mac_in_list;
extern int mbedtls_psa_rsa_load_representation;
extern int esp_md5_hash_finish;
extern int r_ble_lll_mmgmt_alloc_rx_buffer_and_hdr;
extern int nvs_open;
extern int chm_get_channel_index;
extern int r_ble_ll_hci_ev_sca_update;
extern int lv_obj_set_style_bg_image_opa;
extern int sntp_sync_time;
extern int lv_label_set_text_static;
extern int test_tx_fail_statistics;
extern int lv_indev_read_timer_cb;
extern int npl_freertos_event_get_arg;
extern int r_os_msys_get_pkthdr;
extern int i2c_master_read;
extern int r_ble_ll_resolv_enable_cmd;
extern int netif_add;
extern int lv_display_get_original_horizontal_resolution;
extern int esp_crosscore_int_send_yield;
extern int r_ble_ll_sync_send_per_adv_rpt;
extern int npl_freertos_get_time_forever;
extern int ADC_CONTROLLER_TYPE;
extern int hal_set_sta_tbtt_interval;
extern int lv_style_set_outline_color;
extern int mbedtls_asn1_get_alg_null;
extern int heap_caps_malloc_base;
extern int _ZTVSt15_Sp_counted_ptrIPN2tt7service11displayidle18DisplayIdleServiceELN9__gnu_cxx12_Lock_policyE1EE;
extern int ieee80211_recycle_cache_eb;
extern int ieee80211_alloc_challenge;
extern int pwr_hal_set_mac_modem_beacon_miss_intr_enable;
extern int xTaskGenericNotifyWait;
extern int panicHandler;
extern int phy_freq_reg_init_new;
extern int phy_bt_get_tx_gain_new;
extern int regdma_link_stats;
extern int wifi_softap_start;
extern int psa_hash_setup;
extern int r_ble_hci_trans_cfg_hs;
extern int lv_cache_release;
extern int psa_sign_hash;
extern int gdma_get_group_channel_id;
extern int __cxa_guard_release;
extern int ieee80211_add_vhtcap;
extern int scan_parse_ht2040_coex;
extern int cipher_type_map_public_to_supp;
extern int r_priv_sdk_config_options_init;
extern int nvs_set_u16;
extern int wpa_ap_get_peer_spp_msg;
extern int hmac_sha1;
extern int spi_flash_hal_supports_direct_write;
extern int mbedtls_ssl_handshake_server_step;
extern int lv_draw_border_dsc_init;
extern int key_mgr_hal_set_xts_aes_key_len;
extern int strtoul;
extern int lv_span_set_text;
extern int esp_transport_tcp_set_keep_alive;
extern int mbedtls_gcm_update;
extern int gdma_hal_deinit;
extern int gdma_hal_clear_intr;
extern int lwip_getsockname;
extern int ble_att_svr_rx_read_mult_var;
extern int r_ble_ll_adv_periodic_set_info_transfer;
extern int r_ble_ll_adv_env_deinit;
extern int mac_tx_set_htsig;
extern int spi_flash_chip_generic_read_reg;
extern int lv_obj_set_style_line_opa;
extern int pmu_hp_system_analog_param_default;
extern int _lseek_r;
extern int mbedtls_psa_mac_sign_finish;
extern int mbedtls_ssl_handshake_free;
extern int r_ble_ll_adv_sm_find_configured;
extern int esp_timer_impl_init;
extern int r_bt_rf_coex_coded_txrx_time_upper_lim;
extern int pm_noise_check;
extern int psa_key_derivation_set_capacity;
extern int r_ble_ll_init_get_conn_comp_ev;
extern int __log10Pow2;
extern int tcp_listen_with_backlog_and_err;
extern int esp_mbedtls_cleanup;
extern int _ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE;
extern int sdmmc_send_cmd_go_idle_state;
extern int ip4addr_ntoa_r;
extern int pm_get_sleep_type;
extern int lvgl_module;
extern int r_ble_ll_arr_hci_enable;
extern int lv_text_encoded_get_byte_id;
extern int pm_incr_twt_active_cnt;
extern int lv_obj_set_ext_click_area;
extern int pcl_stack_enableSetRssiThreshVsCmd;
extern int lv_keyboard_set_textarea;
extern int spi_flash_chip_generic_get_write_protect;
extern int coex_core_pti_get;
extern int tcp_shutdown;
extern int ble_gap_adv_active;
extern int pmu_lp_system_init;
extern int esp_time_impl_get_time_since_boot;
extern int mbedtls_mpi_swap;
extern int ble_hs_pvcy_remove_entry;
extern int ieee80211_align_eb;
extern int uxTaskGetStackHighWaterMark;
extern int mkdir;
extern int spi_flash_init_chip_state;
extern int test_rx_parse_mumimo;
extern int r_ble_hw_cte_get_sampled_iq_data;
extern int npl_freertos_mutex_deinit;
extern int r_ble_lll_conn_env_deinit_for_sm;
extern int nvs_get_blob;
extern int psa_save_persistent_key;
extern int r_ble_ll_hci_ev_databuf_overflow;
extern int ble_gap_conn_broken;
extern int atol;
extern int esp_wifi_clear_default_wifi_driver_and_handlers;
extern int r_ble_lll_init_pre_process;
extern int lv_spangroup_refresh;
extern int console_access;
extern int esp_crypto_sha_aes_lock_release;
extern int phy_set_mac_data;
extern int adv_stack_initEnv;
extern int r_ble_lll_adv_set_new_scan_rsp_data;
extern int lv_cache_drop_all;
extern int r_ble_ll_hci_info_params_cmd_proc;
extern int ble_hci_trans_env_p;
extern int r_hal_timer_started;
extern int usb_midi_device_set_name;
extern int r_ble_ll_resolve_set_priv_mode;
extern int __udivdi3;
extern int pmksa_cache_auth_init;
extern int lv_draw_add_task;
extern int etharp_query;
extern int mbedtls_x509_crt_profile_suiteb;
extern int bundle_has_string;
extern int r_ble_lll_df_ring_buffer_init;
extern int spi_flash_chip_generic_erase_sector;
extern int ble_hs_id_reset;
extern int r_filter_duplicate_mode_disable;
extern int lv_spinbox_set_digit_format;
extern int sae_ecc_prime_len_2_hash_len;
extern int cnx_sta_associated;
extern int RTC_TYPE;
extern int lv_spinbox_set_step;
extern int gpio_iomux_output;
extern int esp_timer_create;
extern int wDev_record_ftm_data;
extern int ble_gap_preempt_done;
extern int r_filter_duplicate_addr_check_in_table;
extern int mt_init;
extern int __retarget_lock_close_recursive;
extern int preferences_put_bool;
extern int r_ble_ll_adv_hci_set_random_addr;
extern int r_ble_ll_scan_reset;
extern int mbedtls_ecc_group_from_psa;
extern int _ZTVN3nvs15NVSHandleSimpleE;
extern int esp_lcd_panel_io_tx_param;
extern int aes_128_cbc_encrypt;
extern int sae_parse_commit;
extern int hmac_sha256;
extern int camera_open;
extern int _ZdlPv;
extern int audio_codec_get_input_gain_multiplier;
extern int ble_hs_stop;
extern int lv_display_get_driver_data;
extern int phy_chip_i2c_readReg_org;
extern int ieee80211_add_ie_vendor_esp_manufacturer;
extern int http_parser_url_init;
extern int r_ble_lll_adv_coex_dpc_pti_get;
extern int _strerror_r;
extern int r_ble_hw_cte_buffer_owner_set;
extern int r_ble_ll_ctrl_channel_class_reporting_make;
extern int mbedtls_ssl_ciphersuite_uses_ec;
extern int npl_freertos_time_ticks_to_ms;
extern int r_ble_lll_adv_alloc_and_copy_txbuf;
extern int coex_schm_bt_page_wifi_connecting;
extern int esp_panic_handler_enable_rtc_wdt;
extern int g_in_blacklist_flag;
extern int httpd_req_recv;
extern int __d_vfscanf;
extern int cipher_type_map_supp_to_public;
extern int f_rename;
extern int esp_log_level_get_timeout;
extern int rc_enable_trc;
extern int vPortClearInterruptMaskFromISR;
extern int mbedtls_ssl_parse_certificate;
extern int lv_obj_get_y2;
extern int lv_obj_event_base;
extern int app_manager_install_path_scan;
extern int ble_ll_conn_hci_subrate_req;
extern int esp_transport_ssl_set_cert_data;
extern int lv_textarea_set_password_mode;
extern int device_get;
extern int r_ble_hci_trans_buf_alloc;
extern int esp_log_writev;
extern int i2c_periph_signal;
extern int ieee80211_gpsq_init;
extern int ieee80211_better_rsn_pairwise_cipher;
extern int esp_wifi_set_ap_key_internal;
extern int _ZThn8_NSdD1Ev;
extern int esp_timer_stop;
extern int i2s_controller_write;
extern int lv_obj_style_apply_recolor;
extern int ble_ll_conn_hci_set_default_subrate;
extern int lvgl_port_init;
extern int thread_get_stack_space;
extern int ble_spp_set_conn_handle;
extern int ble_store_config_init;
extern int _ZTVSt15_Sp_counted_ptrIPN2tt7service9webserver16WebServerServiceELN9__gnu_cxx12_Lock_policyE1EE;
extern int esp_netif_is_netif_up;
extern int _fstat_r;
extern int module_stop;
extern int r_ble_phy_chan_to_freq;
extern int ap_sa_query_timeout_process;
extern int rtc_clk_slow_freq_get_hz;
extern int esp_transport_ssl_init;
extern int sae_clear_temp_data;
extern int ble_svc_gatt_init;
extern int _ZSt9use_facetISt11__timepunctIcEERKT_RKSt6locale;
extern int _Z21ble_start_advertisingP6DevicePK13ble_uuid128_t;
extern int esp_netif_attach_wifi_ap;
extern int esp_wifi_get_if_mac;
extern int lv_spinbox_set_range;
extern int gpio_backlight_driver;
extern int wpa_gen_rsnxe;
extern int lmacProcessCollision;
extern int rtc_set_time;
extern int ppRxProtoProc;
extern int he_twt_setup_event_post;
extern int wl_size;
extern int spi_device_acquire_bus;
extern int lv_layer_sys;
extern int lv_draw_dispatch;
extern int mac_tx_set_len;
extern int esp_clk_slowclk_cal_get;
extern int lv_label_get_text;
extern int r_ble_lll_sleep_env_deinit;
extern int __letf2;
extern int lv_tileview_class;
extern int mbedtls_pk_ecc_set_key;
extern int lv_mem_init;
extern int ble_hs_conn_deinit;
extern int wifi_osi_funcs_register;
extern int r_ble_ll_scan_update_aux_data;
extern int esp_vfs_write;
extern int create_new_bss_for_sa_query_failed_sta;
extern int lv_timer_reset;
extern int esp_efuse_rtc_calib_get_ver;
extern int coex_schm_bt_inq_wifi_conn;
extern int lv_refr_init;
extern int lv_display_set_draw_buffers;
extern int r_ble_ll_max_byte_get;
extern int coex_schm_init;
extern int MODEM_CLOCK_instance;
extern int vPortTCBPreDeleteHook;
extern int efuse_hal_flash_encryption_enabled;
extern int mbedtls_psa_key_agreement_ecdh;
extern int dispatcher_dispatch_timed;
extern int g_coex_adapter_funcs;
extern int bt_bb_v2_init_cmplx;
extern int esp_io_expander_gpio_wrapper_set_level;
extern int _ZTVSo;
extern int thread_set_priority;
extern int lv_style_set_pad_left;
extern int rcReachRetryLimit;
extern int esp_flash_init_main;
extern int httpd_sess_free_ctx;
extern int lvgl_trackball_add;
extern int pm_on_channel;
extern int lv_obj_get_event_dsc;
extern int pm_check_state;
extern int wpabuf_zeropad;
extern int wpa_sm_notify_assoc;
extern int esp_aes_gcm_setkey;
extern int lv_group_get_focused;
extern int lv_cubic_bezier;
extern int regdma_link_destroy;
extern int adc_set_hw_calibration_code;
extern int ieee80211_crypto_decap;
extern int r_ble_ll_adv_send_conn_comp_ev;
extern int sae_prepare_commit_pt;
extern int g_cnxMgr;
extern int pm_disable_disconnected_sleep_delay_timer;
extern int httpd_uri;
extern int sae_prepare_commit;
extern int bluetooth_serial_is_connected;
extern int r_ble_ll_conn_create;
extern int esp_psram_impl_get_physical_size;
extern int esp_bt_controller_deinit;
extern int r_ble_ll_adv_sync_calculate;
extern int ieee80211_add_ie_vendor_esp_head;
extern int sync_stack_initEnv;
extern int ble_att_svr_rx_read_type;
extern int hal_now;
extern int psa_driver_wrapper_get_key_buffer_size;
extern int r_ble_ll_mem_ptr_check;
extern int g_ble_ll_resolve_hdr;
extern int wDev_IndicateAmpdu;
extern int pm_twt_keep_alive_timeout;
extern int esp_http_client_write;
extern int r_ble_scan_advertise_filter_env_deinit;
extern int ble_random_enable_isr;
extern int tc_cmac_final;
extern int r_hal_timer_stop;
extern int esp_wifi_get_mode;
extern int ble_hs_pvcy_set_default_irk;
extern int r_ble_ll_ctrl_state_set;
extern int lv_image_decoder_post_process;
extern int ble_bb_cte_set_cte_en;
extern int lv_event_mark_deleted;
extern int ppCalSubFrameLength;
extern int register_chipv7_phy_init_param;
extern int sta_twt_null_data_max_retry_times_reached;
extern int sdmmc_io_reset;
extern int _ZSt7__writeIcESt19ostreambuf_iteratorIT_St11char_traitsIS1_EES4_PKS1_i;
extern int device_is_compatible;
extern int lv_event_get_current_target;
extern int bt_rf_coex_pti_dft_rom;
extern int vTaskGetInfo;
extern int cnx_sta_pm;
extern int mbedtls_ecp_point_read_binary;
extern int strncmp;
extern int vPortYieldFromISR;
extern int ieee80211_parse_rsn;
extern int ff_uni2oem;
extern int r_ble_ll_sync_est_event_failed;
extern int pwr_hal_set_beacon_filter_abort_length;
extern int __unordtf2;
extern int lv_display_create;
extern int spi_flash_chip_generic_read;
extern int mbedtls_md_init;
extern int mbedtls_ssl_update_out_pointers;
extern int r_ble_ll_set_default_privacy_mode;
extern int r_ble_ll_scan_init;
extern int lv_obj_style_apply_color_filter;
extern int r_ble_ll_scan_pre_process;
extern int ledc_timer_config;
extern int cJSON_GetArraySize;
extern int lv_display_set_rotation;
extern int r_ble_lll_scan_alloc_rxbuf;
extern int ble_uuid_u16;
extern int ieee80211_set_tim;
extern int hal_set_beamf_pti;
extern int mbedtls_mpi_size;
extern int ieee80211_mt_key_is_mask;
extern int ff_mutex_create;
extern int f_lseek;
extern int phy_rx_pkdet_num_set;
extern int r_filter_duplicate_mode_config;
extern int lv_style_set_height;
extern int pm_send_probe_stop;
extern int multi_heap_register_impl;
extern int xTimerPendFunctionCall;
extern int utoa;
extern int capture_tcp_transport_error;
extern int _ZSt10__pop_heapIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SI_SI_SL_;
extern int lv_atan2;
extern int power_supply_periph_signal;
extern int xPortcheckValidStackMem;
extern int empty_func_p_u8_u8_float_float_pparams_r_void;
extern int esp_http_client_is_complete_data_received;
extern int lvgl_keyboard_disable;
extern int npl_freertos_mutex_init;
extern int pwr_hal_set_beacon_filter_broadcast_wakeup_disable;
extern int lv_obj_set_style_transform_scale_x;
extern int g_cnx_probe_rc_list_cb;
extern int r_filter_aux_duplicate_hash_check_in_table;
extern int ieee80211_he_node_cleanup;
extern int ic_clear_interrupt_handler;
extern int _ZTVSt10moneypunctIcLb0EE;
extern int eloop_init;
extern int nan_send_sync_bcn_process;
extern int lora_get_parameter;
extern int i2c_master_write_to_device;
extern int rcAmpduLowerRate;
extern int phy_i2c_exit_critical;
extern int phy_rfrx_rxdc_cal_new;
extern int r_ble_ll_resolv_deinit;
extern int os_memblock_put_from_cb;
extern int wpa_supplicant_stop_countermeasures;
extern int ble_stack_enableVsEvents;
extern int systimer_hal_init;
extern int sta_support_auto_mode_connect_passive_channel_hidden_ap;
extern int r_ble_ll_adv_aux_pdu_make;
extern int r_ble_lll_mmgmt_direct_allocate_set;
extern int lv_color_over32;
extern int wpa_parse_wpa_ie;
extern int vRingbufferDeleteWithCaps;
extern int r_DB_addr_list_reset;
extern int pwr_hal_set_beacon_filter_unicast_wakeup_enable;
extern int lv_group_deinit;
extern int psa_cipher_finish;
extern int ble_att_clt_rx_mtu;
extern int r_ble_lll_conn_trans_ctrl_pdu;
extern int lv_group_init;
extern int lv_draw_sw_blend_image_to_rgb888;
extern int lv_group_focus_next;
extern int lv_delay_ms;
extern int modem_clock_module_disable;
extern int lv_label_cut_text;
extern int wpa_remove_ptk;
extern int tcp_process_refused_data;
extern int spi_flash_chip_gd_set_io_mode;
extern int ble_gatts_clt_cfg_access;
extern int pm_dream;
extern int r_os_mbuf_prepend;
extern int mbedtls_oid_get_md_hmac;
extern int lwip_close;
extern int pmu_lp_system_power_param_default;
extern int __negdf2;
extern int tc_cmac_setup;
extern int udp_init;
extern int xRingbufferReceiveUpToFromISR;
extern int _ZZN2tt7service9statusbar16StatusbarService13updateUsbIconEvENUlP10FileSystemPvE_4_FUNES4_S5_;
extern int r_ble_lll_sched_get_first_item;
extern int g_ble_single_env;
extern int is_lmac_idle;
extern int lv_strncpy;
extern int ESP_EFUSE_ADC1_AVE_INITCODE_ATTEN1;
extern int r_ble_ll_arr_pref_postion_get;
extern int phy_set_rx_gain_cal_iq;
extern int rcUpdateTxDone;
extern int ESP_EFUSE_KEY_PURPOSE_5;
extern int r_filter_exception_check_is_mesh_packet;
extern int phy_ant_wifirx_cfg;
extern int phy_en_hw_set_freq;
extern int ff_diskio_get_pdrv_card;
extern int phy_tx_cap_init;
extern int ic_get_espnow_rate;
extern int esp_http_client_get_and_clear_last_tls_error;
extern int lv_obj_set_style_height;
extern int i2s_channel_write;
extern int crypto_bignum_mod;
extern int __month_lengths;
extern int mbedtls_gcm_finish;
extern int ble_gap_adv_rsp_set_data;
extern int lmacProcessModemStateRxBeacon;
extern int test_rx_parse_nonmumimo;
extern int sae_ffc_prime_len_2_hash_len;
extern int cnx_bss_alloc;
extern int esp_cache_err_get_panic_info;
extern int _ZTVSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE;
extern int _ZTVSt23__codecvt_abstract_baseIDic10_mbstate_tE;
extern int rssi_saved;
extern int spi_flash_common_write_status_8b_wrsr2;
extern int esprv_int_set_vectored;
extern int dbg_hal_check_set_mplen_bitmap;
extern int r_ble_lll_adv_sched_remaining_pri_after;
extern int tcp_netif_ip_addr_changed;
extern int hmac_sha1_vector;
extern int esp_int_wdt_cpu_init;
extern int r_ble_phy_ramup_time_set;
extern int httpd_unregister_all_uri_handlers;
extern int _ZTVNSt7__cxx118messagesIwEE;
extern int bandmode2str;
extern int r_ble_lll_conn_reschedule_event;
extern int mbedtls_aes_init;
extern int rgb_led_disable;
extern int esp_crypto_mpi_enable_periph_clk;
extern int r_ble_lll_adv_sched_remove_and_restore;
extern int mbedtls_x509_crt_parse_cn_inet_pton;
extern int esp_create_mbedtls_handle;
extern int lv_display_set_render_mode;
extern int r_ble_ll_scan_add_scan_rsp_adv;
extern int memspi_host_program_page;
extern int cnx_check_bssid_in_blacklist;
extern int _ZTVSt7codecvtIDsc10_mbstate_tE;
extern int sta_is_wpa3_enabled;
extern int lv_color_luminance;
extern int _ZTVSt8numpunctIwE;
extern int __lesf2;
extern int spi_flash_hal_erase_block;
extern int ieee80211_amsdu_adjust_head;
extern int gpio_output_enable;
extern int esp_netif_set_old_ip_info;
extern int lv_rb_drop_node;
extern int lv_obj_set_style_transform_rotation;
extern int httpd_default_recv;
extern int g_mt_version;
extern int ieee80211_add_probe_resp_app_ies;
extern int _ZSt11make_uniqueIA_cENSt8__detail9_MakeUniqIT_E7__arrayEj;
extern int pmksa_cache_init;
extern int r_exception_list_need_report_for_mesh;
extern int bootloader_read_flash_id;
extern int r_ble_lll_adv_user_block_check;
extern int adc_oneshot_del_unit;
extern int mbedtls_psa_ecp_import_key;
extern int esp_nimble_deinit;
extern int ble_store_read_peer_sec;
extern int os_memblock_from;
extern int iswlower;
extern int tsf_hal_set_tbtt_soc_wakeup_disable;
extern int get_wpa_sm;
extern int module_resolve_symbol;
extern int ble_hs_enqueue_hci_event;
extern int netconn_err;
extern int g_allowed_groups;
extern int tt_symbol_resolver;
extern int esp_cpu_configure_region_protection;
extern int ble_sm_ioact_state;
extern int phy_dpd_rxdc_cal;
extern int lv_image_header_cache_is_enabled;
extern int sdmmc_send_cmd_send_csd;
extern int _read_r;
extern int ble_hs_conn_chan_find_by_scid;
extern int r_ble_ll_adv_sm_stop_limit_reached;
extern int coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_conn;
extern int mbedtls_ecdsa_can_do;
extern int mt_get_version;
extern int hmac_hal_start;
extern int BACKLIGHT_TYPE;
extern int ble_gap_disc_active;
extern int abs;
extern int lv_canvas_set_px;
extern int pm_beacon_offset_add_loss_counter;
extern int ppTxqEmpty;
extern int r_ble_lll_scan_recycling_is_out_of_order;
extern int uart_controller_open;
extern int xQueueGenericCreateStatic;
extern int r_ble_lll_conn_cth_flow_free_credit;
extern int spi_flash_hal_device_config;
extern int mspi_timing_enter_high_speed_early;
extern int battery_sense_driver;
extern int __unordsf2;
extern int r_ble_ll_update_max_tx_octets_phy_mode;
extern int lmacProcessLongFrameSuccess;
extern int esp_wifi_sta_update_ap_info_internal;
extern int ieee80211_setup_robust_mgmtframe;
extern int i2c_master_probe;
extern int esp_libc_include_strcmp_impl;
extern int KERNEL_SYMBOLS;
extern int mbedtls_ssl_get_ecp_group_id_from_tls_id;
extern int ble_att_svr_rx_write;
extern int _ZSt16__throw_bad_castv;
extern int display_mirror;
extern int hal_agreement_add_rx_ba;
extern int spi_flash_encryption_hal_destroy;
extern int inet_chksum_pseudo;
extern int _ZTVSt7codecvtIDsDu10_mbstate_tE;
extern int esp_app_get_elf_sha256;
extern int xQueueGiveFromISR;
extern int r_ble_lll_adv_sm_init;
extern int lv_display_is_double_buffered;
extern int dhcps_start;
extern int xQueueCreateCountingSemaphore;
extern int r_ble_lll_adv_aux_scannable_pdu_payload_len;
extern int esp_ptr_external_ram;
extern int r_ble_hw_driver_env_deinit;
extern int lv_style_set_rotary_sensitivity;
extern int tan;
extern int lora_add_state_callback;
extern int r_ble_ll_hci_ctlr_bb_cmd_proc;
extern int crypto_bignum_inverse;
extern int esp_rom_cvt;
extern int ble_gap_disc;
extern int ieee80211_getbcnframe;
extern int wifi_get_firmware_ops;
extern int mbedtls_ms_time;
extern int heap_caps_realloc;
extern int lv_image_decoder_open;
extern int uxTaskGetTaskNumber;
extern int rgb_led_enable;
extern int phytype2str;
extern int esp_hw_stack_guard_get_fired_cpu;
extern int power_supply_is_allowed_to_charge;
extern int mbedtls_ecc_group_to_psa;
extern int r_ble_lll_set_rxbuf_default_value;
extern int apply_z;
extern int __pow5bits;
extern int _ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE;
extern int esp_sha256_driver_update;
extern int f_mkdir;
extern int lv_calendar_get_showed_date;
extern int phy_get_data_sat;
extern int spi_bus_remove_device;
extern int sdmmc_wait_for_idle;
extern int coex_schm_bt_page_wifi_conn;
extern int phy_bb_cbw_chan_cfg;
extern int httpd_sess_process;
extern int _ZTVSt21__ctype_abstract_baseIwE;
extern int mbedtls_hmac_drbg_random;
extern int lvgl_get_shared_icon_font_height;
extern int mac_tx_set_vhtsig;
extern int __crash;
extern int hal_disable_softap_tsf;
extern int phy_get_tm7_new;
extern int _ZTVSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE1EE;
extern int ble_l2cap_sig_init;
extern int gpio_descriptor_add_callback;
extern int ble_hs_conn_alloc;
extern int mbedtls_rsa_rsassa_pkcs1_v15_sign;
extern int phy_band_change;
extern int sdmmc_init_host_frequency;
extern int lv_obj_get_self_width;
extern int r_ble_ll_conn_chk_csm_flags;
extern int ble_svc_gap_device_name;
extern int mbedtls_ssl_conf_ca_chain;
extern int esp_crypto_ds_enable_periph_clk;
extern int mbedtls_psa_asymmetric_encrypt;
extern int ble_bb_cte_set_cte_ant_switch_delay_rx_aoa_2us_2m;
extern int pp_register_config_cb;
extern int mbedtls_mpi_core_montmul_init;
extern int _ZSt9use_facetISt8numpunctIcEERKT_RKSt6locale;
extern int sdmmc_send_cmd_all_send_cid;
extern int wmemmove;
extern int phy_rfcal_rxiq;
extern int r_ble_ll_ctrl_start_enc_send;
extern int esp_flash_read_chip_id;
extern int spi_bus_deinit_lock;
extern int lv_obj_get_display;
extern int hostap_ht_operation_update;
extern int phy_get_chan_target_power;
extern int ble_gap_disc_cancel;
extern int ble_store_config_persist_our_secs;
extern int wl_erase_range;
extern int sta_rx_csa;
extern int ledc_channel_config;
extern int vQueueDeleteWithCaps;
extern int ble_restore_peer_sec_nvs;
extern int r_ble_ll_df_read_antenna_info;
extern int esp_crosscore_int_init;
extern int _ZZNSt18__moneypunct_cacheIcLb1EE8_M_cacheERKSt6localeEN11_Scoped_strC1ERKSs;
extern int xTaskGetSchedulerState;
extern int esp_flash_get_physical_size;
extern int r_ble_lll_conn_align_pld_cnt_with_rxbuffer;
extern int spi_hal_usr_is_done;
extern int mbedtls_mpi_free;
extern int phy_restart_cal;
extern int lv_indev_set_user_data;
extern int registered_heaps;
extern int ets_timer_disarm;
extern int ble_hs_mbuf_acl_pkt;
extern int mmu_hal_check_valid_ext_vaddr_region;
extern int tbtt_adaptive_setup;
extern int trackball_get_button_pressed;
extern int ieee80211_sta_tx_disconnect_cb;
extern int mbedtls_ct_memcmp;
extern int lv_obj_align_to;
extern int lwip_netconn_do_accepted;
extern int lv_rb_find;
extern int trc_calc_duration;
extern int mbedtls_mpi_mul_int;
extern int gdma_hal_start_with_desc;
extern int wcstoul;
extern int rx11ACRate2AMPDULimit;
extern int ble_gap_preempt_no_lock;
extern int ieee80211_process_bar_info;
extern int ble_att_svr_rx_mtu;
extern int r_ble_lll_scan_callout_env_deinit;
extern int ble_gattc_rx_read_group_type_complete;
extern int cnx_obss_scan;
extern int mbedtls_ssl_get_tls_id_from_ecp_group_id;
extern int sdmmc_init_rca;
extern int ble_gatts_notify;
extern int powf;
extern int _getpid_r;
extern int npl_freertos_funcs_deinit;
extern int sdmmc_send_cmd_send_scr;
extern int pp_hdrsize;
extern int gpio_descriptor_get_native_pin_number;
extern int _Z25app_metadata_copy_boundedPcjRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int dbg_lmac_ps_statis_reset;
extern int lv_indev_get_type;
extern int ble_hs_conn_chan_insert;
extern int scan_remove_bssid;
extern int mld6_tmr;
extern int ic_get_trc;
extern int phy_write_chan_freq;
extern int psa_to_ssl_errors;
extern int lv_color_make;
extern int raw_new_ip_type;
extern int r_ble_ll_df_connless_cte_rx_enable;
extern int audio_stream_set_volume;
extern int lwip_netconn_do_recv;
extern int phy_set_bb_wdg;
extern int ble_att_set_preferred_mtu;
extern int trc_tid_isTxAmpduOperational;
extern int r_ble_ll_whitelist_search;
extern int r_ble_ll_hci_event_send;
extern int esp_sha_release_hardware;
extern int ledc_hal_set_duty_int_part;
extern int mac_last_rxbuf_init;
extern int wep_encap;
extern int lv_keyboard_def_event_cb;
extern int __wrap_lv_textarea_create;
extern int config_is_cache_tx_buf_enabled;
extern int xRingbufferReceiveUpTo;
extern int gps_generic_module;
extern int rsn_cipher_put_suites;
extern int phy_rx_pkdet_dc_cal;
extern int lv_arc_get_knob_offset;
extern int ble_gap_terminate_with_conn;
extern int __gttf2;
extern int ble_gap_mtu_event;
extern int esp_transport_list_destroy;
extern int uECC_vli_bytesToNative;
extern int lv_arc_set_rotation;
extern int esp_tls_get_and_clear_error_type;
extern int _ZSt8to_charsPcS_d;
extern int r_ble_ll_chk_txrx_time;
extern int ieee80211_send_action_vendor_spec;
extern int hal_tsf_timer_set_target;
extern int rc11AXSchedTbl;
extern int tcp_input;
extern int coex_wifi_request;
extern int lv_obj_set_style_text_line_space;
extern int lv_display_get_event_dsc;
extern int pm_connectionless_wake_interval_timeout_process;
extern int lv_grid_init;
extern int esp_psram_check_ptr_addr;
extern int esp_fp_get_callers;
extern int ieee80211_add_htcap;
extern int coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_scan;
extern int tlsf_alloc_overhead;
extern int memspi_host_erase_block;
extern int lv_text_encoded_get_char_id;
extern int pp_coex_tx_request;
extern int lmacSetTxFrame;
extern int mbedtls_gcm_init;
extern int sdmmc_init_select_card;
extern int r_ble_ll_hci_ev_rd_rem_used_feat;
extern int ble_gap_reset_state;
extern int udp_new;
extern int app_install;
extern int pointer_set_mirror_x;
extern int ieee80211_ccmp_encrypt;
extern int lv_obj_set_style_grid_cell_y_align;
extern int lv_spangroup_set_align;
extern int r_ble_lll_scan_env_init;
extern int pm_funcs_deinit;
extern int ppCheckTxIdle;
extern int nvs_flash_erase_partition;
extern int _ZTVSt23_Sp_counted_ptr_inplaceIN2tt7service14ServiceContextESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int r_ble_ll_scan_initiator_start;
extern int mbedtls_pk_rsa_set_pubkey;
extern int _ZSt15__try_use_facetISt5ctypeIwEEPKT_RKSt6locale;
extern int lwip_select;
extern int ieee80211_pwrsave;
extern int esp_crypto_shared_gdma_start_axi_ahb;
extern int coex_schm_bt_piscan_wifi_connecting;
extern int lv_anim_set_completed_cb;
extern int iswgraph;
extern int mbedtls_ccm_free;
extern int _ZGVNSt10moneypunctIwLb0EE2idE;
extern int r_ble_lll_adv_alloc_sch_items;
extern int coex_schm_bt_default_wifi_scan;
extern int esp_test_tx_tb_complete;
extern int huk_hal_get_risk_level;
extern int ble_ll_rand_env_p;
extern int adc_apb_periph_free;
extern int phy_get_index_pwr;
extern int ble_gap_rx_phy_update_complete;
extern int he_twt_information_suspend;
extern int lv_textarea_set_placeholder_text;
extern int r_ble_lll_conn_timeout;
extern int ecdsa_hal_set_efuse_key;
extern int regdma_link_new_wait;
extern int lv_dropdown_get_option_index;
extern int esp_log_buffer_hex_internal;
extern int arr_hci_enable;
extern int mbedtls_oid_get_pk_alg;
extern int pthread_mutex_lock;
extern int ble_hs_lock;
extern int hal_he_reset_muedca_timer;
extern int mbedtls_oid_get_cipher_alg;
extern int esp_http_client_cleanup;
extern int wpa_neg_complete;
extern int bluetooth_serial_write;
extern int lv_timer_handler;
extern int esp_system_include_startup_funcs;
extern int r_ble_lll_adv_sched_next_pri_event;
extern int lwip_standard_chksum;
extern int r_ble_lll_env_init;
extern int ble_uuid_cmp;
extern int r_ble_ll_adv_flags_clear;
extern int _lock_init;
extern int hostap_handle_timer;
extern int set_gauss_coeff_1m;
extern int pm_sleep;
extern int memp_TCP_PCB;
extern int bootloader_flash_reset_chip;
extern int wDev_DiscardFrame;
extern int i2s_channel_disable;
extern int __getf2;
extern int wpa_sm_set_state;
extern int lv_draw_image_tiled_helper;
extern int r_ble_phy_get_actual_tx_time;
extern int is_esp_mesh_assoc;
extern int opendir;
extern int phy_bt_get_tx_tab_new;
extern int mld6_leavegroup_netif;
extern int trc_set_per_pkt_rate;
extern int pm_process_tim;
extern int memspi_host_erase_chip;
extern int esp_wifi_deauthenticate_internal;
extern int heap_caps_match;
extern int _ZTVSt18__moneypunct_cacheIwLb1EE;
extern int r_ble_ll_scan_enabled;
extern int _ZTTSt14basic_ofstreamIcSt11char_traitsIcEE;
extern int vTimerSetTimerNumber;
extern int ble_gatts_register_svcs;
extern int mbedtls_psa_get_random;
extern int ieee80211_alloc_action_vendor_spec;
extern int wl_is_ap_no_lr;
extern int ieee80211_decap;
extern int wifi_scan_start_process;
extern int ble_att_svr_rx_read_group_type;
extern int mbedtls_pk_verify_ext;
extern int ppCalFrameTimes;
extern int mbedtls_pk_info_from_type;
extern int TmpSTAAPCloseAP;
extern int bluetooth_hid_device_send_key;
extern int lvgl_ppa_is_supported;
extern int lv_obj_set_style_border_opa;
extern int lwip_netconn_do_getaddr;
extern int uxListRemove;
extern int coex_schm_bt_conn_wifi_conn;
extern int __ltsf2;
extern int _Z16esp_err_to_errori;
extern int wifi_scan;
extern int phy_pwdet_reg_init;
extern int sta_con_timer;
extern int ble_freertos_mutex_pool;
extern int gpio_sleep_set_pull_mode;
extern int cJSON_GetStringValue;
extern int ht_action_output;
extern int bundle_put_bool;
extern int nvs_commit;
extern int nd6_input;
extern int spi_bus_add_device;
extern int pbuf_header_force;
extern int flash_mmap_lock_init;
extern int pmu_hp_system_clock_param_default;
extern int _Z29ble_resolve_next_unnamed_peerP6Devicej;
extern int ble_hs_flow_deinit;
extern int lv_text_get_size;
extern int spi_flash_hal_gpspi_read;
extern int r_ble_ll_misc_options_set;
extern int _ZTVSt23__codecvt_abstract_baseIcc10_mbstate_tE;
extern int _ZSt14__add_groupingIcEPT_S1_S0_PKcjPKS0_S5_;
extern int scannum;
extern int lv_obj_add_state;
extern int esp_coex_common_malloc_internal_wrapper;
extern int crypto_bignum_legendre;
extern int hal_mac_rx_set_abort_frames_from_transbss;
extern int r_ble_hw_cte_get_timeline_num;
extern int device_add;
extern int _ZTVSt15_Sp_counted_ptrIPN2tt7service9statusbar16StatusbarServiceELN9__gnu_cxx12_Lock_policyE1EE;
extern int xPortInterruptedFromISRContext;
extern int spi_flash_encryption_hal_prepare;
extern int acosf;
extern int adc_channel_io_map;
extern int esp_psram_io_get_cs_io;
extern int app_manager_add;
extern int pm_mac_try_enable_modem_state;
extern int r_ble_ll_adv_sm_retrieve;
extern int r_ble_lll_sync_rx_process;
extern int window_manager_configure;
extern int os_mempool_ext_init;
extern int lv_anim_start;
extern int bluetooth_hid_device_send_gamepad;
extern int esp_time_impl_get_time;
extern int ecc_hal_enable_constant_time_point_mul;
extern int stat;
extern int coex_schm_ble_mesh_config_bt_a2dp_wifi_conn;
extern int tt_timezone_is_format_24_hour;
extern int r_ble_ll_scan_aux_data_unref;
extern int tcp_seg_free;
extern int mbedtls_psa_cipher_finish;
extern int lv_list_add_text;
extern int lv_iter_create;
extern int mbedtls_mpi_core_mla;
extern int bundle_has_int64;
extern int __cxa_atexit;
extern int loader_service_manifest;
extern int lv_image_decoder_add_to_cache;
extern int ieee80211_recv_bar;
extern int mbedtls_mpi_exp_mod_unsafe;
extern int mbedtls_rsa_write_key;
extern int lvgl_toolbar_add_image_button_action;
extern int esp_lcd_panel_del;
extern int spi_device_polling_end;
extern int r_ble_ll_hci_ev_datalen_chg;
extern int _ZGVNSt7__cxx118messagesIwE2idE;
extern int __gtsf2;
extern int bt_bb_v2_rx_set;
extern int gpio_controller_deinit_descriptors;
extern int pthread_once;
extern int esp_transport_poll_write;
extern int esp_tls_conn_write;
extern int lv_obj_set_style_grid_cell_x_align;
extern int mbedtls_ecp_group_load;
extern int esp32_adc_oneshot_driver;
extern int r_ble_lll_conn_update_buffer_hdr;
extern int _ZGVNSt7__cxx118numpunctIcE2idE;
extern int phy_get_cal_chan;
extern int lv_array_resize;
extern int hostap_deliver_data;
extern int g_heap_usage;
extern int ap_get_sta_internal;
extern int esp_test_clr_rx_error_occurs;
extern int r_ble_ll_init;
extern int mbedtls_sha256_starts;
extern int sqrtf;
extern int vTaskPlaceOnUnorderedEventList;
extern int wdev_process_tsf_timer;
extern int __divsf3;
extern int eap_client_get_eap_state;
extern int dns_init;
extern int r_ble_ll_mem_mbuf_get;
extern int display_get_mirror_y;
extern int empty_func_p_void_r_float;
extern int lv_obj_get_index;
extern int coex_pre_init;
extern int ble_gap_security_initiate;
extern int esp_wifi_internal_reg_netstack_buf_cb_local;
extern int cnx_start_obss_scan;
extern int I2S_CONTROLLER_TYPE;
extern int _ZGVNSt7__cxx118time_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int lv_canvas_fill_bg;
extern int ble_gatts_free_svcs;
extern int tcp_arg;
extern int lv_image_set_pivot;
extern int phy_open_fe_bb_clk;
extern int esp_wifi_register_eapol_txdonecb_internal;
extern int mbedtls_cipher_set_iv;
extern int fcntl;
extern int r_ble_ll_adv_ext_set_enable;
extern int spicommon_bus_initialize_io;
extern int sys_arch_mbox_fetch;
extern int httpd_start;
extern int wpa_sta_in_4way_handshake;
extern int lv_group_get_default;
extern int esp_vfs_fat_spiflash_mount;
extern int lv_dropdown_add_option;
extern int nan_transmit_action;
extern int r_ble_ll_conn_set_global_chanmap;
extern int bundle_has_bool;
extern int esp_rsa_ds_pad_v15_encode;
extern int ledc_hal_get_duty;
extern int backlight_set_brightness;
extern int camera_release_frame;
extern int vTaskRemoveFromUnorderedEventList;
extern int scan_stack_disable;
extern int g_mmgmt_mem_max_block_size;
extern int ble_store_key_from_value_local_irk;
extern int r_ble_ll_sync_phy_mode_to_hci;
extern int lv_draw_buf_flush_cache;
extern int _ZSt18uncaught_exceptionv;
extern int esp_ble_unregister_bb_funcs;
extern int mbedtls_x509_crt_check_extended_key_usage;
extern int lv_draw_task_get_draw_dsc;
extern int esp_sha1_driver_finish;
extern int r_ble_ll_rand_deinit;
extern int cnx_start_handoff_cb;
extern int spi_flash_common_set_io_mode;
extern int eapol_txcb;
extern int lseek;
extern int esp_netif_set_driver_config;
extern int esp_now_register_recv_cb;
extern int phy_i2c_paral_write_num;
extern int lv_obj_set_size;
extern int vht_max_apep_length;
extern int _ZTVN2tt7service13memorychecker20MemoryCheckerServiceE;
extern int __file_str_get;
extern int f_close;
extern int g_rts_threshold_bytes;
extern int wpa_install_key;
extern int wDev_remove_KeyEntry_all_cnx;
extern int lv_malloc_core;
extern int r_ble_lll_sleep_set_wakeup_timer;
extern int pm_set_beacon_duration;
extern int efuse_hal_read;
extern int ble_hs_hci_set_buf_sz;
extern int esp_rsa_ds_opaque_sign_hash_start;
extern int esp_tls_conn_new_async;
extern int lodepng_chunk_next;
extern int udp_sendto;
extern int isalnum;
extern int mbedtls_net_init;
extern int phy_set_channel_rfpll_freq_new;
extern int cnx_softap_csa;
extern int ble_hs_pvcy_our_irk;
extern int phy_pbus_xpd_tx_on;
extern int lv_tick_inc;
extern int lv_obj_style_state_compare;
extern int mbedtls_ssl_get_psa_curve_info_from_tls_id;
extern int r_ble_lll_adv_legacy_pri_chan_pdu_make;
extern int fminf;
extern int phy_rfcal_pwrctrl;
extern int hid_consumer_input_handle;
extern int lv_obj_set_style_pad_bottom;
extern int esp_elf_map_sym;
extern int _ZTVN2tt7service9statusbar16StatusbarServiceE;
extern int phy_get_xtal_code;
extern int r_ble_ll_sync_receive_enable;
extern int lodepng_state_init;
extern int ESP_EFUSE_RD_DIS_BLOCK_KEY3;
extern int _Z22ble_scan_clear_resultsP6Device;
extern int phy_wifi_11g_rate_chg;
extern int i2s_channel_init_std_mode;
extern int coex_schm_bt_sniff_sco_wifi_conn;
extern int mbedtls_rsa_pkcs1_decrypt;
extern int r_ble_ll_resolv_enabled;
extern int mbedtls_ssl_get_peer_cert;
extern int ble_att_set_peer_mtu;
extern int ble_store_util_count;
extern int phy_txpwr_cal_track_new;
extern int esp_supplicant_init;
extern int mac_tx_set_plcp2;
extern int efuse_hal_blk_version;
extern int r_ble_lll_sched_delay_according_to_priority;
extern int lv_draw_buf_create_ex;
extern int pm_start;
extern int lv_style_set_arc_color;
extern int nan_faw_end_process;
extern int esp_ecdsa_transparent_verify_hash;
extern int __subsf3;
extern int flash_mmap_lock_unfreeze;
extern int ble_uuid_deinit;
extern int lwip_shutdown;
extern int dispatcher_free;
extern int preferences_has_string;
extern int esp_efuse_mac_get_default;
extern int active_hid_rpt_map_len;
extern int wifi_nvs_compare_cfg_diff;
extern int esp_vfs_fat_register_cfg;
extern int lv_indev_active;
extern int xTaskResumeAll;
extern int mbedtls_rsa_pkcs1_verify;
extern int snprintf;
extern int vPortSetupTimer;
extern int nan_tx_action_frame;
extern int r_ble_lll_sched_check_remaining_entries;
extern int wpa_cipher_to_alg;
extern int phy_rf_cal_data_recovery;
extern int thread_alloc_full;
extern int bluetooth_fire_event;
extern int ble_sm_sc_oob_confirm;
extern int ppTxqUpdateBitmap;
extern int ieee80211_vnd_lora_ie_set;
extern int pwr_hal_set_mac_modem_beacon_miss_limit;
extern int _Z23ble_midi_start_internalP6Device;
extern int psa_cipher_set_iv;
extern int ble_gap_rx_rd_all_remote_feat;
extern int pm_tx_null_data_done_quick_wake_process;
extern int device_for_each_child;
extern int s_fragment;
extern int pm_parse_mbssid_element;
extern int mbedtls_mutex_unlock_ptr;
extern int ble_hs_is_parent_task;
extern int _Z23acquire_pin_or_set_nullRK11GpioPinSpectPP14GpioDescriptor;
extern int app_manager_get_topmost_instance_id;
extern int lv_checkbox_create;
extern int mbedtls_net_send;
extern int ieee80211_phy_mode_show;
extern int bt_bb_set_rx_sense;
extern int huk_hal_recharge_huk_memory;
extern int current_hid_profile;
extern int _ZTSSt9bad_alloc;
extern int mbedtls_asn1_write_int;
extern int spi_controller_unlock;
extern int __cxa_pure_virtual;
extern int mbedtls_mpi_sub_int;
extern int vQueueWaitForMessageRestricted;
extern int sntp_init;
extern int _ZTVN9__gnu_cxx26__concurrence_unlock_errorE;
extern int r_bt_rf_coex_cfg_get_default;
extern int ble_gattc_rx_read_mult_rsp;
extern int ble_hs_adv_parse_fields;
extern int esp_cmac_finish;
extern int r_ble_ll_acl_data_in;
extern int wifi_nvs_cfg_init;
extern int uECC_set_rng;
extern int ic_set_ac_muedca_param;
extern int r_ble_ll_ctrl_cal_path_loss_zone;
extern int r_ble_lll_sched_get_tail_of;
extern int panic_print_dec;
extern int phy_read_hw_noisefloor;
extern int _g_esp_netif_inherent_ap_config;
extern int trc_ctl;
extern int r_ble_lll_adv_alloc_cte_chain_mem;
extern int pp_timer_do_process;
extern int mbedtls_ecp_set_zero;
extern int hal_he_enable_dump_complete_hesigb;
extern int r_ble_ll_whitelist_read_size;
extern int ble_gatts_indicate_custom;
extern int esp_efuse_check_errors;
extern int rssi_index;
extern int _ZSt15__try_use_facetISt7codecvtIcc10_mbstate_tEEPKT_RKSt6locale;
extern int ble_ll_adv_env_p;
extern int gpio_descriptor_set_flags;
extern int _ZTVSt7codecvtIcc10_mbstate_tE;
extern int lv_obj_init_draw_rect_dsc;
extern int he_btwt_teardown_post_event;
extern int ieee80211_set_tx_desc;
extern int hal_mac_rate_autoack_init;
extern int wifi_set_promiscuous_callback;
extern int material_symbols_launcher_36;
extern int sha_hal_write_digest;
extern int ic_reset_extra_softap_rx_ba;
extern int minitar_read_header;
extern int r_ble_hw_rng_init;
extern int r_ble_lll_reset;
extern int r_ble_ll_ctrl_reject_ind_send;
extern int phy_wr_freq_mem;
extern int gdma_reset;
extern int phy_freq_num_get_data;
extern int wpa_use_akm_defined;
extern int lv_style_set_bg_opa;
extern int ESP_EFUSE_ADC1_CH2_ATTEN0_INITCODE_DIFF;
extern int r_ble_hw_whitelist_search;
extern int cnx_update_bss_more;
extern int hal_set_tx_min_pwr;
extern int i2c_cmd_link_delete;
extern int r_ble_ll_event_comp_pkts;
extern int r_os_mbuf_trailingspace;
extern int _ZTV9Partition;
extern int uart_param_config;
extern int __wrap__Unwind_Resume;
extern int uxTaskResetEventItemValue;
extern int lv_buttonmatrix_clear_button_ctrl_all;
extern int r_os_mbuf_ctrl_mbuf_cnt_decrease;
extern int lv_textarea_cursor_left;
extern int _ZZNKSt7__cxx117collateIcE12do_transformEPKcS3_EN4_BufD2Ev;
extern int __cxa_get_globals_fast;
extern int r_ble_hci_trans_init;
extern int ble_sm_cmd_get;
extern int bluetooth_pair;
extern int phy_en_pwdet;
extern int esp_security_init_include_impl;
extern int he_send_action_event_bsscolor;
extern int midi_chars;
extern int esp_wifi_internal_update_modem_sleep_default_params;
extern int settimeofday;
extern int mbedtls_ssl_validate_ciphersuite;
extern int ble_gattc_write_flat;
extern int lv_calendar_set_today_date;
extern int sha1_prf;
extern int lv_clamp_width;
extern int ppAssembleMicHdr;
extern int lv_chart_set_update_mode;
extern int esp_log_level_set;
extern int _ZGVNSt8numpunctIcE2idE;
extern int pm_on_coex_start;
extern int window_manager_create_ext;
extern int igmp_report_groups;
extern int esp_wifi_get_pmf_config_internal;
extern int coex_schm_process_restart;
extern int sdmmc_write_sectors_dma;
extern int ieee80211_ioctl_deinit;
extern int r_ble_lll_scan_schedule_aux;
extern int uECC_vli_sub;
extern int i2s_channel_try_to_constitute_duplex;
extern int ble_spp_set_active;
extern int lmac_stop_hw_txq;
extern int r_ble_ll_scan_get_ext_adv_report;
extern int lv_fs_get_drv;
extern int modem_clock_module_mac_reset;
extern int lv_list_button_class;
extern int pmksa_cache_deinit;
extern int aes_encrypt_init;
extern int esp_test_enable_rx_statistics;
extern int neighbor_cache;
extern int wifi_register_mgmt_frame;
extern int len_dh_ie;
extern int service_instance_construct;
extern int phy_get_fm_sar_dout;
extern int wpa_eapol_key_mic;
extern int sntp_get_sync_interval;
extern int esp_psram_mspi_mb;
extern int phy_wifi_set_tx_gain_new;
extern int esp_http_client_add_auth;
extern int r_ble_lll_mmgmt_block_size_get;
extern int g_ble_phy_mode_pkt_start_off_ro;
extern int mbedtls_mpi_core_bigendian_to_host;
extern int __wrap___deregister_frame;
extern int ledc_hal_set_range_number;
extern int camera_capture_jpeg;
extern int ppProcessTxQ;
extern int qr_version_binary_sizes;
extern int ieee80211_disassoc_construct;
extern int ic_set_bssid;
extern int gdma_connect;
extern int coex_schm_ble_default_bt_default_wifi_connecting;
extern int ieee80211_add_power_cap;
extern int uart_controller_flush_input;
extern int preferences_opt_int64;
extern int mbedtls_rsa_rsaes_pkcs1_v15_decrypt;
extern int r_exception_list_ble_mesh_addr_table_add;
extern int pm_get_connectionless_status;
extern int esp_wifi_ap_get_owe_config_internal;
extern int multi_heap_minimum_free_size_impl;
extern int driver_construct;
extern int lvgl_software_keyboard_destruct;
extern int lv_obj_set_grid_dsc_array;
extern int esp_wifi_deinit_internal;
extern int ieee80211_vnd_ie_set;
extern int empty_func_p_u8_u8_pu16_pfloat_r_void;
extern int pbuf_alloc;
extern int psram_exec_cmd;
extern int lv_draw_sw_blend_color_to_rgb565;
extern int r_ble_ll_adv_set_sched;
extern int ble_hs_hci_set_le_supported_feat;
extern int bootloader_init_mem;
extern int pm_is_sending_keep_alive;
extern int phy_set_pbus_reg;
extern int mbedtls_mpi_bitlen;
extern int _regi2c_impl_write;
extern int read;
extern int esp_ds_finish_sign;
extern int mbedtls_hmac_drbg_init;
extern int f_closedir;
extern int r_os_msys_unregister_block_num_get_cb;
extern int r_ble_lll_conn_slave_new;
extern int ieee80211_vht_updatecaps;
extern int xQueueIsQueueFullFromISR;
extern int raw_sendto_if_src;
extern int lv_fs_stdio_init;
extern int _ZTVSt16invalid_argument;
extern int app_manager_start;
extern int _ZZN2tt7service9statusbar16StatusbarService16updateSdCardIconEvENUlP10FileSystemPvE_4_FUNES4_S5_;
extern int mbedtls_mutex_init;
extern int phy_iq_swap_set;
extern int g_wifi_osi_funcs;
extern int mbedtls_asn1_write_tag;
extern int heap_caps_free;
extern int esp_ecc_point_verify;
extern int lv_menu_page_class;
extern int lv_async_call;
extern int r_ble_lll_sleep_init;
extern int _ZSt9use_facetISt10moneypunctIwLb0EEERKT_RKSt6locale;
extern int btwt_setup_dwell_timeout_fn;
extern int lv_font_get_default;
extern int ble_hs_timer_resched;
extern int tcp_recv;
extern int mbedtls_psa_cipher_decrypt;
extern int esp_http_client_set_authtype;
extern int ble_random_mode_init_exec_process_cb_get;
extern int psa_driver_wrapper_export_public_key;
extern int lv_obj_set_style_flex_grow;
extern int esp_transport_utils_ms_to_timeval;
extern int lodepng_encoder_settings_init;
extern int _ZTISt12length_error;
extern int hci_stack_initEnv;
extern int nimble_hid_device_api;
extern int phy_i2c_init1;
extern int gpio_set_level;
extern int psa_format_key_data_for_storage;
extern int lvgl_trackball_remove;
extern int i2s_controller_set_rx_tdm_config;
extern int mbedtls_mpi_core_exp_mod;
extern int esp_wifi_register_if_rxcb;
extern int wifi_is_started;
extern int _ZZNKSt7collateIcE12do_transformEPKcS2_EN4_BufD1Ev;
extern int mbedtls_pk_get_type;
extern int mld6_leavegroup;
extern int mbedtls_ssl_add_hs_hdr_to_checksum;
extern int phy_bb_bss_cbw40;
extern int esp_event_isr_post_to;
extern int __bufio_flush_locked;
extern int pm_mac_force_disable_modem_state;
extern int hostap_del_mac_info_from_list;
extern int ble_hs_conn_addrs;
extern int lv_menu_clear_history;
extern int lv_buttonmatrix_get_selected_button;
extern int bundle_free;
extern int mbedtls_mpi_add_int;
extern int cnx_remove_rc_except;
extern int spi_bus_init_lock;
extern int r_ble_ll_arr_pref_itvl_get;
extern int keyboard_is_present;
extern int aes_128_cbc_decrypt;
extern int ble_gatts_notify_custom;
extern int wpa_auth_uses_mfp;
extern int uart_hal_write_txfifo;
extern int esp32_uart_driver;
extern int regi2c_enter_critical;
extern int _ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_;
extern int uart_hal_tx_break;
extern int mbedtls_psa_ffdh_import_key;
extern int event_group_get;
extern int hal_agreement_add_extra_softap_rx_ba;
extern int r_ble_lll_conn_get_safe_instant_delta;
extern int g_ble_phy_chan_freq_ro;
extern int httpd_resp_send_chunk;
extern int phy_tx_magtrk_init;
extern int console_write;
extern int phy_set_iqcal_ckgen_code;
extern int r_ble_ll_adv_single_pkt_mode_get;
extern int islower;
extern int r_ble_ll_adv_scheduled;
extern int esp_wifi_internal_reg_netstack_buf_cb;
extern int hal_get_dump_ctrl_frame_cfg;
extern int lv_obj_init_draw_image_dsc;
extern int lv_draw_layer_go_to_xy;
extern int esp_event_loop_create_default;
extern int esp_phy_load_cal_and_init;
extern int r_ble_ll_sync_get_sm;
extern int esp_netif_stop;
extern int wifi_nvs_validate_sta_sae_identifier;
extern int ieee80211_ampdu_request;
extern int esp_http_client_close;
extern int mbedtls_mpi_lset;
extern int _ZTVSt9basic_iosIcSt11char_traitsIcEE;
extern int ble_gattc_read;
extern int r_ble_ll_adv_flags_set;
extern int ble_att_svr_reset;
extern int esp_mesh_map_deauth;
extern int spi_flash_chip_winbond_suspend_cmd_conf;
extern int psa_mac_abort;
extern int lv_obj_set_style_text_align;
extern int bsscolor_event_dump;
extern int ble_gattc_rx_find_info_complete;
extern int ip4_route_src_hook;
extern int lv_spinbox_create;
extern int esp32_spi_get_cs_pin;
extern int sys_sem_signal_isr;
extern int isgraph;
extern int mbedtls_ssl_send_alert_message;
extern int ieee80211_regdomain_ap_max_chan;
extern int npl_freertos_callout_reset;
extern int igmp_input;
extern int backlight_get_min_brightness;
extern int r_filter_duplicate_ad_type_config;
extern int _Z11init_uc6580P6Device;
extern int USB_DEVICE_CONTROLLER_TYPE;
extern int lv_canvas_set_draw_buf;
extern int r_get_le16;
extern int tcp_output;
extern int r_ble_get_npl_element_info;
extern int qrcode_initText;
extern int esp_transport_ssl_set_client_key_password;
extern int hal_set_sta_light_sleep_wake_ahead_time;
extern int i2s_controller_disable_direction;
extern int esp_now_init;
extern int _ZSt19piecewise_construct;
extern int ble_store_key_from_value_csfc;
extern int lv_textarea_delete_char;
extern int lv_dropdown_set_dir;
extern int wDev_Rxbuf_Init;
extern int phy_start_tx_tone_step;
extern int esp_http_client_get_content_length;
extern int psa_wipe_all_key_slots;
extern int lv_obj_set_style_bg_image_recolor_opa;
extern int pm_on_data_tx_done;
extern int ble_sm_sc_dhkey_check_exec;
extern int ip6_input;
extern int pbuf_try_get_at;
extern int lv_draw_sw_arc;
extern int lv_theme_get_color_primary;
extern int ampdu_process_multicast_address_qos_frame;
extern int wpa_sm_pmksa_cache_flush;
extern int wdev_mac_sleep;
extern int lv_bin_decoder_open;
extern int systimer_us_to_ticks;
extern int _powf;
extern int r_ble_ll_conn_ext_master_init;
extern int ic_set_trc;
extern int bt_agc_gain_max;
extern int psa_export_public_key_internal;
extern int i2c_master_bus_add_device;
extern int hal_mac_set_txq_invalid;
extern int i2s_channel_enable;
extern int hal_set_sta_tbtt_ahead_time;
extern int sdmmc_init_io_bus_width;
extern int esp_crypto_ecdsa_enable_periph_clk;
extern int spi_flash_chip_generic_config_host_io_mode;
extern int r_ble_ll_mem_alloc;
extern int lv_font_get_glyph_dsc;
extern int esp_mmu_map_get_max_consecutive_free_block_size;
extern int pm_on_mac_modem_beacon_miss;
extern int lv_font_glyph_release_draw_data;
extern int ppGetTaskHdl;
extern int lv_image_decoder_create;
extern int scan_check_hidden;
extern int lora_get_radio_state;
extern int wifi_copy_ap_record;
extern int wifi_nvs_validate_ap_chan;
extern int r_ble_lll_sched_insert_forcibly;
extern int uart_set_select_notif_callback;
extern int r_ble_ll_adv_first_chan;
extern int ble_svc_gap_deinit;
extern int r_ble_ll_hci_cte_select_iq_sample_data_auto;
extern int atexit;
extern int wifi_nvs_validate_ap_ssid;
extern int rtc_gpio_pullup_en;
extern int lvgl_trackball_set_settings;
extern int lv_label_create;
extern int esp_libc_include_strncmp_impl;
extern int fmin;
extern int ESP_EFUSE_WR_DIS_BLOCK_KEY0;
extern int lv_draw_create_unit;
extern int g_bss_color_collision_detection_enabled;
extern int esp_key_mgr_deactivate_key;
extern int mpi_hal_read_result_hw_op;
extern int tsf_hal_set_tbtt_rf_ctrl_enable;
extern int lmacProcessTxTimeout;
extern int lv_timer_get_user_data;
extern int ic_set_mac;
extern int localtime_r;
extern int _ZTVSt11__timepunctIcE;
extern int spi_bus_get_dma_ctx;
extern int esp_log_linked_list_clean;
extern int r_ble_ll_df_connless_cte_tx_set_param;
extern int r_ble_ll_scan_set_scan_params;
extern int r_ble_ll_conn_get_cur_phy_txpwr;
extern int ble_gatts_init;
extern int adv_stack_deinitEnv;
extern int mbedtls_rsa_opaque_info;
extern int npl_freertos_eventq_put;
extern int display_get_gap_y;
extern int pp_register_tx_cb;
extern int esp_mmu_unmap;
extern int hal_tsf_get_tbttstart;
extern int esp_test_enable_rx_mu_statistics;
extern int file_system_is_mounted;
extern int ieee80211_classify;
extern int lvgl_hardware_keyboard_remove_custom;
extern int r_ble_ll_deinit;
extern int lv_style_set_line_color;
extern int esp_libc_init_funcs;
extern int wifi_ipc_process;
extern int pm_force_soc_wakeup;
extern int psa_reset_key_attributes;
extern int esp_transport_ssl_skip_common_name_check;
extern int lv_ll_clear;
extern int ieee80211_protocol_attach;
extern int http_header_set;
extern int regularize_k;
extern int esp_transport_ssl_set_tls_version;
extern int _ZGVNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int adv_stack_enableScanReqRxdVsEvent;
extern int phy_i2c_bbpll_set;
extern int psa_generic_status_to_mbedtls;
extern int r_ble_phy_hw_state_check;
extern int dhcp_fine_tmr;
extern int bluetooth_disconnect;
extern int scan_bitmap_to_channel_list;
extern int adiList_stack_deinitEnv;
extern int psa_its_remove;
extern int psa_raw_key_agreement;
extern int rand_r;
extern int select;
extern int lv_group_set_default;
extern int lv_draw_dispatch_request;
extern int nimble_port_freertos_deinit;
extern int ble_att_clt_rx_prep_write;
extern int bt_bb_tx_cca_period;
extern int esp_aes_cipher_update;
extern int i2s_channel_read;
extern int grove_set_mode;
extern int esp_wifi_set_promiscuous;
extern int lv_indev_search_obj;
extern int mbedtls_md_free;
extern int uart_pattern_queue_reset;
extern int bt_rf_coex_pti_dft_p;
extern int lv_obj_set_style_text_outline_stroke_width;
extern int periph_rcc_acquire_enter;
extern int ieee80211_setup_pmf;
extern int r_ble_lll_scan_aux_data_flag_set;
extern int nonhe_clr_snd_ch_cfg;
extern int lv_style_set_max_height;
extern int esp_vfs_unregister_fs;
extern int wdev_funcs_init;
extern int mbedtls_mpi_write_binary_le;
extern int power_supply_supports_power_off;
extern int ieee80211_set_gtk;
extern int properties_file_has;
extern int esp_vfs_rename;
extern int sdmmc_write_sectors;
extern int ieee80211_ifdetach;
extern int coex_register_start_cb;
extern int ic_mac_init;
extern int ip4addr_aton;
extern int ble_store_delete_csfc;
extern int __wrap_gpio_set_level;
extern int mbedtls_asn1_sequence_free;
extern int task_event_group_destruct;
extern int ic_register_tx_cb;
extern int r_ble_hw_whitelist_rmv;
extern int omac1_aes_128_vector;
extern int esp_wifi_sta_get_prof_authmode_internal;
extern int sync_stack_deinitEnv;
extern int ic_set_bar_rate;
extern int mld6_joingroup_netif;
extern int ieee80211_regdomain_get_country;
extern int esp_aes_gcm_starts;
extern int lv_draw_dispatch_layer;
extern int mbedtls_ecdsa_sign_det_restartable;
extern int mbedtls_ssl_get_ciphersuite_sig_pk_psa_alg;
extern int __xpg_strerror_r;
extern int lv_image_set_src;
extern int ieee80211_set_max_rate;
extern int r_ble_ll_flush_pkt_queue;
extern int esp_event_handler_unregister;
extern int ethernet_output;
extern int ble_osi_coex_funcs_register;
extern int pm_twt_wake_up;
extern int _Z26ble_gap_disc_event_handlerP13ble_gap_eventPv;
extern int vPortFree;
extern int esp_brownout_init;
extern int spi_flash_hal_gpspi_device_config;
extern int coexist_printf;
extern int _lock_try_acquire_recursive;
extern int ble_gap_notify_rx_event;
extern int lv_cache_create;
extern int _Z13init_atgm336hP6Device;
extern int r_ble_ll_msys_mem_copy;
extern int _ZTTSt14basic_ifstreamIcSt11char_traitsIcEE;
extern int spi_flash_cache2phys;
extern int spi_flash_cache_enabled;
extern int minitar_open;
extern int wifi_get_promiscuous;
extern int xpt2046_softspi_module;
extern int ppRxPkt;
extern int asin;
extern int _exit;
extern int ieee80211_ap_sa_query_timeout;
extern int ic_register_timer_post_cb;
extern int spi_hal_cal_timing;
extern int sdspi_host_init;
extern int r_ble_ll_conn_created;
extern int phy_track_pll_deinit;
extern int ble_gap_authorize_event;
extern int wifi_sta_restore_default_mac;
extern int _ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE;
extern int pm_hmac_rx_data_process;
extern int device_get_driver;
extern int esp_http_client_perform;
extern int g_cte_ant_delay;
extern int hal_mac_is_txq_valid;
extern int httpd_queue_work;
extern int vEventGroupClearBitsCallback;
extern int esp_wifi_set_promiscuous_rx_cb;
extern int strncasecmp;
extern int sdmmc_decode_scr;
extern int r_ble_ll_ctrl_rx_version_ind;
extern int ble_controller_get_compile_version;
extern int esp_sha1_finish;
extern int _ZTVSt15_Sp_counted_ptrIPN2tt7service6espnow13EspNowServiceELN9__gnu_cxx12_Lock_policyE1EE;
extern int periph_rcc_release_exit;
extern int r_exception_list_ble_mesh_pb_adv_link_id_table_remove;
extern int r_ble_hw_rng_stop;
extern int phy_change_channel;
extern int ble_hs_misc_conn_chan_find_reqd;
extern int esp_timer_get_time;
extern int camera_get_height;
extern int rtc_time_get;
extern int advFilter_hci_configDuplicateExceptionList;
extern int __libc_fini_array;
extern int wdt_hal_init;
extern int lv_obj_set_height;
extern int periph_rcc_release_enter;
extern int gpio_descriptor_get_level;
extern int mbedtls_x509_get_ext;
extern int mbedtls_mpi_core_shift_l;
extern int r_ble_ll_hci_ev_conn_update;
extern int lv_obj_set_style_x;
extern int r_ble_ll_resolv_rpa_timer_cb;
extern int ieee80211_sta_scan;
extern int lv_event_get_layer;
extern int phy_rfpll_set_adc_rate;
extern int __wrap_lv_list_create;
extern int ic_ampdu_op;
extern int pm_on_sample_beacon;
extern int pm_enable_dream_timer;
extern int pm_mac_sleep;
extern int esp_register_shutdown_handler;
extern int esp_test_enable_tx_statistics;
extern int esp_log_linked_list_get_level;
extern int lv_display_get_color_format;
extern int r_ble_ll_conn_env_deinit;
extern int wpa_ap_remove;
extern int lv_spinbox_set_digit_step_direction;
extern int uart_controller_set_config;
extern int ble_att_svr_rx_exec_write;
extern int esp_efuse_get_field_size;
extern int wdev_mac_wakeup;
extern int gdma_ahb_hal_enable_etm_task;
extern int get_estimated_batime;
extern int crypto_ec_point_cmp;
extern int mbedtls_mpi_gcd_modinv_odd;
extern int lv_tabview_get_tab_bar;
extern int _ZZNKSt8__format14__formatter_fpIcE6formatIfNS_10_Sink_iterIcEEEENSt20basic_format_contextIT0_cE8iteratorET_RS7_ENKUlPcSB_E_clESB_SB_;
extern int NUS_SVC_UUID;
extern int esp_wifi_bt_power_domain_off;
extern int mbedtls_cipher_cmac_update;
extern int lv_obj_set_style_margin_top;
extern int mbedtls_pk_ecc_set_group;
extern int _ZSt15__try_use_facetISt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEEPKT_RKSt6locale;
extern int memp_TCPIP_MSG_API;
extern int esp_panic_handler;
extern int ecc_hal_read_verify_result;
extern int r_ble_lll_adv_update_sync_info_in_buffer;
extern int tcp_update_rcv_ann_wnd;
extern int mbedtls_psa_aead_abort;
extern int properties_file_get;
extern int ieee80211_copy_eb_header;
extern int esp_test_get_rx_error_occurs;
extern int r_ble_lll_sched_insert;
extern int wifi_create_queue;
extern int spi_flash_encryption_hal_enable;
extern int ieee80211_recv_action_register;
extern int ble_sm_num_procs;
extern int mbedtls_ssl_flush_output;
extern int lv_dropdown_close;
extern int lv_text_get_next_line;
extern int psa_free_key_slot;
extern int __ceil_log2pow5;
extern int ieee80211_crypto_encap;
extern int r_ble_ll_resolv_list_find;
extern int tcpip_try_callback;
extern int wpabuf_alloc_copy;
extern int sta_recv_ctl;
extern int isupper;
extern int nvs_flash_init_partition;
extern int putle32;
extern int phy_adc_rate_set;
extern int ppDequeueTxDone_Locked;
extern int udp_recv;
extern int tcp_input_pcb;
extern int _ZdlPvSt11align_val_t;
extern int i2s_platform_release_occupation;
extern int sdmmc_decode_ssr;
extern int esp_system_reset_modules_on_exit;
extern int lv_roller_class;
extern int r_ble_ll_ext_scan_parse_aux_ptr;
extern int lv_refr_deinit;
extern int lodepng_chunk_check_crc;
extern int phy_rate_to_index;
extern int ic_interface_enabled;
extern int app_manager_finish;
extern int nd6_tmr;
extern int pm_twt_set_target_tsf;
extern int bluetooth_connect;
extern int mbedtls_rsa_info;
extern int lv_arc_set_knob_offset;
extern int r_ble_ll_conn_check_itvl_data_len_is_match;
extern int r_ble_hci_trans_buf_free;
extern int pm_stop;
extern int esp_flash_init_default_chip;
extern int g_msys_ctrl_noconn_mbuf_cnt;
extern int esp_clk_tree_enable_src;
extern int rcUpdateAMPDUParam;
extern int lv_keyboard_class;
extern int spi_flash_chip_generic_read_unique_id;
extern int sdmmc_select_current_limit;
extern int ff_memfree;
extern int _ZGVNSt7__cxx119money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int tc_cmac_update;
extern int os_msys_deinit;
extern int ic_stop_hw_txq;
extern int ap_rxcb;
extern int vTaskMissedYield;
extern int app_paths_get_user_data_directory;
extern int ap_sta_hash_add;
extern int lv_obj_get_user_data;
extern int esp_vfs_access;
extern int _ZTVNSt7__cxx119money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEEE;
extern int ble_att_clt_tx_read_type;
extern int psa_its_get;
extern int r_ble_ll_ctrl_stop_rsp_timer;
extern int esp_wifi_is_if_ready_when_started;
extern int hci_transport_deinit;
extern int coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_scan;
extern int r_ble_ll_ctrl_channel_class_enable_make;
extern int r_exception_list_ble_mesh_filter_addr_add;
extern int r_ble_lll_adv_coex_dpc_process_sec;
extern int mbedtls_platform_zeroize;
extern int esp_reset_reason;
extern int lv_checkbox_class;
extern int expf;
extern int lvgl_toolbar_add_dropdown_action;
extern int __lock___malloc_recursive_mutex;
extern int ESP_EFUSE_MAC;
extern int adc_reset_lock_release;
extern int tcp_keepalive;
extern int ESP_EFUSE_WR_DIS_BLOCK_KEY4;
extern int r_ble_ll_mem_env_init;
extern int lv_obj_set_style_bg_color;
extern int pm_on_probe_resp_rx;
extern int st7796_module;
extern int esp_vfs_stat;
extern int config_get_wifi_task_stack_size;
extern int usb_hid_device_send_mouse;
extern int lv_indev_reset;
extern int mem_free;
extern int mbedtls_cipher_cmac_starts;
extern int lv_spinbox_get_rollover;
extern int _ZTVSt8time_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE;
extern int lv_tick_elaps;
extern int clk_hal_lp_slow_get_freq_hz;
extern int coex_schm_ble_mesh_traffic_bt_default_wifi_conn;
extern int wpa_sm_free_eapol;
extern int phy_get_max_pwr;
extern int esp_sha_read_digest_state;
extern int r_ble_phy_get_txdbm_by_level;
extern int dhcp_renew;
extern int esp32_ble_hid_device_driver;
extern int r_ble_ll_conn_auth_pyld_timer_cb;
extern int lv_ll_clear_custom;
extern int ieee80211_get_cipher_mic_len;
extern int bluetooth_hid_device_send_consumer;
extern int coex_core_disable;
extern int spi_flash_chip_generic_suspend_cmd_conf;
extern int _ZSt16__convert_from_vRKPiPciPKcz;
extern int wifi_init_completed;
extern int wifi_auto_scan_set_paused_function;
extern int esp_netif_action_disconnected;
extern int ble_sm_alg_g2;
extern int lv_obj_get_style_recolor_recursive;
extern int lvgl_trackball_settings_get_default;
extern int minmea_parse_gga;
extern int phy_iq_est_enable_new;
extern int access;
extern int mbedtls_rsa_validate_crt;
extern int r_ble_lll_df_ring_buffer_deinit;
extern int wifi_hw_start;
extern int mbedtls_rsa_deduce_crt;
extern int lv_spinbox_set_rollover;
extern int ampdu_free_rx_ba_index;
extern int ff_diskio_register_wl_partition;
extern int pm_beacon_timestamp_statistic;
extern int ble_hs_hci_acl_tx;
extern int gdma_ahb_hal_start_with_desc;
extern int lv_spangroup_add_span;
extern int mt_is_peer_exist;
extern int r_ble_lll_adv_periodic_stop;
extern int mbedtls_sha256_finish;
extern int wifi_station_save_ap_channel;
extern int netconn_accept;
extern int phy_txdc_cal_pwdet;
extern int fdopen;
extern int r_ble_ll_scan_ext_initiator_start;
extern int s_tbttstart;
extern int midi_io_handle;
extern int hal_random;
extern int coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_connecting;
extern int esp_register_npl_funcs;
extern int heap_caps_calloc_base;
extern int _ZSt20__throw_out_of_rangePKc;
extern int r_ble_ll_df_env_init;
extern int httpd_recv;
extern int phy_dpd_rxdc_cal_init;
extern int wifi_delete_queue;
extern int mbedtls_asn1_get_sequence_of;
extern int vTaskSetThreadLocalStoragePointer;
extern int esp_netif_start;
extern int _ZSt9use_facetISt10moneypunctIcLb0EEERKT_RKSt6locale;
extern int lv_style_set_pad_row;
extern int phy_chan_to_freq;
extern int mbedtls_mutex_init_ptr;
extern int r_ble_enable_bqb_test;
extern int lwip_inet_pton;
extern int pm_tx_done_itwt_process;
extern int ieee80211_vnd_lora_ie_size;
extern int lv_color_mix;
extern int hal_sniffer_enable;
extern int g_os_mempool_list;
extern int backlight_get_brightness;
extern int phy_xpd_rf;
extern int wifi_on_coex_schm_phase_process;
extern int mbedtls_cipher_base_lookup_table;
extern int empty_func_p_ptsf_u32_r_void;
extern int btwt_setup_timeout_fn_process;
extern int bluetooth_set_hid_host_active;
extern int spi_flash_hal_poll_cmd_done;
extern int phy_txcap_setting;
extern int lv_textarea_set_max_length;
extern int s_phy_get_max_pwr_new;
extern int uxTaskGetNumberOfTasks;
extern int frexpf;
extern int ieee80211_regdomain_is_dfs_channel;
extern int pm_beacon_offset_funcs_init;
extern int ieee80211_add_bss_max_idle;
extern int sta_retry_assoc;
extern int r_ble_lll_sleep_controller_sleep_en;
extern int ble_sm_slave_initiate;
extern int phy_get_power_db;
extern int send_deauth;
extern int file_system_set_owner;
extern int pm_noise_check_disable;
extern int comeback_token_hash;
extern int pm_off_channel;
extern int efuse_hal_chip_revision;
extern int display_get_resolution_y;
extern int tt_timezone_set;
extern int esp_aes_crypt_cbc;
extern int esp_crypto_common_clk_enable;
extern int pm_beacon_add_loss_counter;
extern int aes_hal_mode_init;
extern int lv_draw_rect;
extern int r_swap_buf;
extern int ble_gap_identity_event;
extern int lv_cache_entry_release_data;
extern int _ZTVSt7collateIcE;
extern int _ZStlsIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_St8_SetfillIS3_E;
extern int uart_disable_intr_mask;
extern int ppResortTxAMPDU;
extern int lv_buttonmatrix_set_selected_button;
extern int tsf_hal_set_modem_timer_wakeup_early_time;
extern int regdma_link_new_write;
extern int r_ble_ll_set_sync_transfer_params;
extern int lv_strlen;
extern int npl_freertos_sem_pend;
extern int gpio_hal_intr_disable;
extern int phy_bbtx_outfilter;
extern int lv_tabview_create;
extern int esp_coex_common_task_yield_from_isr_wrapper;
extern int r_ble_hw_set_connected_dev;
extern int ble_gap_adv_start;
extern int panic_print_registers;
extern int ppCheckTxRTS;
extern int BAROFDMSched;
extern int esp_mpi_mul_mpi_hw_op;
extern int g_ble_lll_sleep_env_p;
extern int r_ble_ll_hci_reset;
extern int pmksa_cache_add;
extern int asinhf;
extern int ble_att_clt_tx_read;
extern int lv_draw_sw_mask_angle_init;
extern int coex_schm_ble_mesh_config_wifi_conn;
extern int r_filter_duplicate_data_base_reset;
extern int r_ble_lll_conn_tx_process;
extern int lvgl_software_keyboard_construct;
extern int ispunct;
extern int wDevCheckBlockError;
extern int esp_flash_app_enable_os_functions;
extern int stdout;
extern int gdma_hal_disconnect_all;
extern int esp_md5_hash_compute;
extern int esf_buf_alloc_dynamic;
extern int mbedtls_mpi_cmp_abs;
extern int hal_init_tx_pwr;
extern int _ZGVNSt9money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int lv_strcat;
extern int esp_hmac_opaque_size_function;
extern int phy_wr_rx_gain_mem;
extern int r_ble_lll_mmgmt_allocated_flag_set;
extern int r_ble_ll_mem_actual_word_size_get;
extern int trc_init;
extern int mbedtls_rsa_private;
extern int lv_obj_remove_flag;
extern int freopen;
extern int ieee80211_set_bss_max_idle_period;
extern int display_get_color_format;
extern int _ZSt15__try_use_facetISt11__timepunctIcEEPKT_RKSt6locale;
extern int lv_obj_has_flag_any;
extern int _ZGVNSt7__cxx1110moneypunctIcLb1EE2idE;
extern int hal_he_set_power_save;
extern int rssi_margin;
extern int g_eb_list_desc;
extern int ieee80211_ht_deattach;
extern int s_itwt_id;
extern int ble_stack_deinitEnv;
extern int _g_esp_netif_soft_ap_ip;
extern int r_ble_lll_adv_periodic_start;
extern int lodepng_add_text;
extern int phy_bb_init;
extern int r_ble_ll_sync_filter_enabled;
extern int pthread_getspecific;
extern int r_ble_ll_df_conn_cte_req_enable;
extern int lvgl_ppa_get_or_create;
extern int lv_obj_enable_style_refresh;
extern int lv_obj_set_style_image_recolor;
extern int lwip_strnicmp;
extern int r_DB_check_addr_exist;
extern int phy_ckgen_5g_cal;
extern int lv_indev_set_group;
extern int npl_freertos_callout_set_arg;
extern int wifi_init_bss_color;
extern int cnx_beacon_timeout_process;
extern int esp_event_post_to;
extern int lv_obj_style_init;
extern int acoshf;
extern int regi2c_saradc_enable;
extern int wpa_res_to_status_code;
extern int _Z11init_ag33xxP6Device;
extern int console_fsync;
extern int vPortEnterCritical;
extern int spi_hal_hw_prepare_rx;
extern int lv_obj_set_style_base_dir;
extern int _ZTSSt12system_error;
extern int crypto_mbedtls_get_grp_id;
extern int lv_theme_get_color_secondary;
extern int coex_env;
extern int r_ble_ll_conn_is_dev_connected;
extern int xEventGroupCreateStatic;
extern int mt_register_recv_cb;
extern int wpa_supplicant_pairwise_gtk;
extern int esp_netif_sntp_deinit;
extern int module_is_started;
extern int lv_anim_set_user_data;
extern int esp_tls_internal_event_tracker_destroy;
extern int ieee80211_recv_action;
extern int sta_is_11r_enabled;
extern int mbedtls_cipher_crypt;
extern int tlsf_create;
extern int __bswapdi2;
extern int ble_gatts_peer_cl_sup_feat_get;
extern int aes_siv_decrypt;
extern int audio_stream_set_enabled;
extern int esp_cache_unfreeze_caches_enable_interrupts;
extern int usb_device_controller_get_active_class;
extern int xEventGroupCreateWithCaps;
extern int tcp_abort;
extern int vht_get_min_subframe_len;
extern int lv_timer_set_period;
extern int bluetooth_add_event_callback;
extern int r_ble_lll_sync_reset;
extern int r_ble_ll_adv_sm_init;
extern int bt_osi_mem_free_internal;
extern int esp_timer_init_include_func;
extern int lv_spangroup_class;
extern int hal_disable_sta_tsf;
extern int lv_arc_get_max_value;
extern int thread_join;
extern int esp_wifi_unset_appie_internal;
extern int phy_set_rx_gain_cal_dc;
extern int esp_clk_tree_initialize;
extern int coex_schm_ble_mesh_config_bt_conn_wifi_connecting;
extern int mbedtls_rsa_set_padding;
extern int heap_caps_add_region_with_caps;
extern int pwr_hal_set_beacon_filter_enable;
extern int crypto_ec_order_len;
extern int cnx_sta_color_change_fn_process;
extern int ESP_EFUSE_ADC1_CH1_ATTEN0_INITCODE_DIFF;
extern int pthread_mutex_destroy;
extern int esp_mesh_quick_funcs;
extern int hal_crypto_init;
extern int ble_att_svr_read_handle;
extern int _ZSt11__make_heapIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SI_SL_;
extern int fabsf;
extern int ic_get_80211_tx_rate;
extern int ppAMPDU2Normal;
extern int xRingbufferSend;
extern int hal_wdev_timer_target_enable;
extern int sdmmc_init_sd_timing_tuning;
extern int esp_efuse_utility_fill_buff;
extern int rand_init;
extern int esp_crypto_ecdsa_lock_release;
extern int _ZTVSt9money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE;
extern int dhcp_release_and_stop;
extern int r_ble_lll_scan_period_timer_cb;
extern int r_ble_lll_mmgmt_alloc_buffer_hdr;
extern int ieee80211_is_he_cipher;
extern int r_ble_ll_ctrl_proc_with_instant_initiated;
extern int lv_theme_default_deinit;
extern int esp_clk_utils_mspi_speed_mode_sync_after_cpu_freq_switching;
extern int lv_obj_has_state;
extern int _ZTVNSt7__cxx1110moneypunctIcLb0EEE;
extern int r_ble_ll_conn_adjust_pyld_len;
extern int ic_trc_set_per_pkt_rate;
extern int lora_set_enabled;
extern int lv_chart_set_all_values;
extern int netconn_gethostbyname_addrtype_n;
extern int power_supply_supports_charge_control;
extern int esp_transport_ssl_set_common_name;
extern int lv_event_remove_all;
extern int tcp_send_fin;
extern int ecp_mul_restartable_internal_soft;
extern int r_ble_ll_resolv_gen_priv_addr;
extern int pm_on_tsf_timer;
extern int sar_periph_ctrl_adc_oneshot_power_acquire;
extern int ieee80211_setup_vht_rates;
extern int ble_gap_event_connect_call;
extern int netif_get_by_index;
extern int lv_indev_get_gesture_dir;
extern int psa_initialize_key_slots;
extern int udp_disconnect;
extern int multi_heap_aligned_alloc_impl_offs;
extern int phy_pbus_xpd_iq_path;
extern int esp_sha512_driver_finish;
extern int r_ble_lll_sync_schedule_chain;
extern int spi_bus_initialize;
extern int memspi_host_read_status_hs;
extern int lv_calendar_header_dropdown_class;
extern int uart_controller_is_open;
extern int trc_isTxAmpduOperational;
extern int os_ext_mempool_register_cb;
extern int _ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE;
extern int ic_set_beacon_int;
extern int lmacProcessCtsTimeout;
extern int lv_obj_set_style_image_recolor_opa;
extern int coex_rom_data_init;
extern int __subdf3;
extern int lv_menu_section_class;
extern int lv_memmove;
extern int r_sdkconfig_set_opts;
extern int netconn_write_partly;
extern int sdmmc_send_cmd_send_op_cond;
extern int regdma_link_new_branch_write;
extern int he_send_action_twt_setup;
extern int esp_wifi_ap_get_sta_aid_local;
extern int offchan_recv_action;
extern int cnx_update_bss;
extern int coex_enable;
extern int lv_event_get_key;
extern int lv_span_get_style;
extern int igmp_tmr;
extern int r_ble_ll_conn_send_txpwr_report;
extern int mbedtls_ccm_init;
extern int r_ble_ll_hci_vs_csa_set;
extern int _ZTVSt17__timepunct_cacheIcE;
extern int ble_svc_gap_device_name_set;
extern int hal_he_set_mmss_and_aid;
extern int scan_set_desChan;
extern int g_dynamic_cs;
extern int lv_text_set_text_vfmt;
extern int sdmmc_send_cmd_send_cid;
extern int pwr_hal_set_mac_modem_state_sleep_limit_exceeded_wakeup_disable;
extern int lv_draw_buf_align;
extern int esp_sync_timekeeping_timers;
extern int pp_delete_task;
extern int lv_obj_update_layout;
extern int _Z36app_metadata_is_valid_format_versionRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int mbedtls_cipher_info_from_type;
extern int r_priv_sdk_config_options_deinit;
extern int i2s_channel_tune_rate;
extern int netif_set_link_up;
extern int pthread_mutex_timedlock;
extern int periph_rcc_exit;
extern int r_bt_rf_coex_hook_deinit;
extern int lmacTxDone;
extern int pthread_internal_local_storage_destructor_callback;
extern int r_exception_list_ble_mesh_filter_addr_remove;
extern int esp_tls_internal_event_tracker_capture;
extern int ble_gatts_connection_broken;
extern int __powidf2;
extern int r_ble_lll_mmgmt_skip_rxbuf_alloc_set;
extern int i2c_master_bus_rm_device;
extern int _ZSt27__unguarded_partition_pivotIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEESI_SI_SI_SK_;
extern int esp_unregister_ext_funcs;
extern int lv_obj_set_style_text_outline_stroke_opa;
extern int lv_area_join;
extern int _ZnwjRKSt9nothrow_t;
extern int lv_roller_set_selected;
extern int sys_init;
extern int r_ble_lll_recycle_in_task;
extern int mbedtls_x509_crt_profile_default;
extern int esp_wifi_ap_get_transition_disable_internal;
extern int nan_start;
extern int phy_reset_ckgen;
extern int r_ble_ll_scan_rx_pkt_in_on_aux;
extern int ds_hal_set_key_source;
extern int mktime;
extern int xRingbufferCreateWithCaps;
extern int controller_is_inited;
extern int ble_store_read_local_irk;
extern int ble_bb_cte_set_cte_samp_limit_switch;
extern int cnx_auth_timeout;
extern int ieee80211_event_report_bsscolor_collision;
extern int ble_att_clt_tx_notify;
extern int r_ble_ll_hci_ev_conn_cte_req_failed;
extern int mbedtls_aes_crypt_xts;
extern int r_ble_ll_hci_ev_ltk_req;
extern int rtc_clk_8m_enable;
extern int r_ble_lll_adv_coex_dpc_process_pri;
extern int r_ble_ll_hci_ext_scan_set_enable;
extern int mbedtls_ssl_optimize_checksum;
extern int hal_mac_tx_set_ppdu;
extern int mbedtls_ssl_pk_sig_alg_from_sig;
extern int spi_bus_lock_acquire_start;
extern int spi_flash_timing_is_tuned;
extern int esp_coex_common_int_restore_wrapper;
extern int r_ble_lll_adv_priority_reset;
extern int r_os_mbuf_get_pkthdr;
extern int phy_dac_rate_set;
extern int esp_wifi_sta_disable_owe_trans_internal;
extern int esp_cmac_abort;
extern int ble_gap_terminate;
extern int __bswapsi2;
extern int send_wake_null_timeout_process;
extern int device_set_parent;
extern int _ZStssIcSt11char_traitsIcESaIcEEDTcl21__char_traits_cmp_catIT0_ELi0EEERKNSt7__cxx1112basic_stringIT_S3_T1_EESB_;
extern int wifi_set_promiscuous;
extern int gps_meshtastic_module;
extern int r_ble_lll_convert_timeline_num_to_sync_handle;
extern int pm_beacon_monitor_tbtt_allowed;
extern int uxQueueMessagesWaiting;
extern int lv_tileview_tile_class;
extern int ff_memalloc;
extern int ampdu_alloc_extra_softap_rx_ba_index;
extern int lv_buttonmatrix_clear_button_ctrl;
extern int lvgl_sliderbox_get_value;
extern int addba_timeout_process;
extern int lv_label_get_text_selection_end;
extern int vTaskDeleteWithCaps;
extern int pwr_hal_set_mac_modem_tbtt_auto_period_enable;
extern int heap_caps_realloc_base;
extern int key_mgr_hal_start;
extern int __floatsisf;
extern int esp_vfs_rmdir;
extern int r_ble_lll_adv_update_adv_scan_rsp_data;
extern int rtc_gpio_set_direction;
extern int r_os_mbuf_concat;
extern int mbedtls_pem_init;
extern int r_ble_lll_mmgmt_free_tx_buffer;
extern int memmove;
extern int audio_stream_write;
extern int esp_wifi_set_appie_internal;
extern int ble_store_config_delete;
extern int esp_lcd_panel_swap_xy;
extern int r_put_le16;
extern int lwip_netconn_do_send;
extern int audio_codec_open;
extern int phy_bt_filter_reg;
extern int wDev_ProcessBeaconMemoryRxSuc;
extern int esp_coex_common_semphr_delete_wrapper;
extern int regi2c_ctrl_write_reg;
extern int esp_wifi_ipc_internal;
extern int spi_flash_chip_generic_probe;
extern int esp_intr_get_cpu;
extern int mmu_hal_unmap_region;
extern int r_ble_ll_conn_hci_le_set_phy;
extern int xpt2046_softspi_read_valid_touch;
extern int lv_event_remove;
extern int spi_flash_chip_boya_probe;
extern int ble_att_svr_stop;
extern int esp_vfs_fat_sdspi_mount;
extern int ble_store_key_from_value_cccd;
extern int npl_freertos_event_run;
extern int ble_l2cap_is_mtu_req_sent;
extern int tlsf_malloc;
extern int r_ble_lll_mmgmt_free_link_state;
extern int cnx_do_handoff_internal;
extern int get_iav_key;
extern int rtc_gpio_deinit;
extern int esp_wifi_register_wpa3_ap_cb;
extern int sdspi_host_io_int_wait;
extern int pm_dream_timeout_process;
extern int mem_malloc;
extern int r_ble_lll_sched_deletion_end;
extern int esp_tls_conn_destroy;
extern int _ZThn8_NSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED0Ev;
extern int hal_mac_set_bssid;
extern int esp_md5_hash_setup;
extern int wifi_get_scan_results;
extern int r_ble_ll_usecs_to_ticks_round_up;
extern int esp_clk_tree_src_get_freq_hz;
extern int r_ble_lll_adv_coex_dpc_update_on_adv_start;
extern int fclose;
extern int ap_free_sta;
extern int __wrap__Unwind_DeleteException;
extern int phy_get_mac_addr;
extern int r_ble_lll_scan_alloc_aux_memory;
extern int ble_store_config_deinit;
extern int psa_key_derivation_setup;
extern int esp_coex_common_timer_disarm_wrapper;
extern int lv_draw_buf_has_flag;
extern int lv_obj_get_scrollbar_mode;
extern int esp_netif_is_valid_static_ip;
extern int ble_att_is_response_op;
extern int esp_vfs_unregister;
extern int key_mgr_hal_is_huk_valid;
extern int _ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS_;
extern int lv_draw_task_get_label_dsc;
extern int lora_find_first_registered_device;
extern int lv_array_at;
extern int tlsf_get_pool;
extern int lv_style_set_outline_width;
extern int esp_task_wdt_init;
extern int lv_indev_get_vect;
extern int sntp_set_sync_mode;
extern int ble_hs_tx_data;
extern int r_ble_ll_ctrl_len_proc;
extern int lv_obj_get_scroll_snap_y;
extern int lv_arc_get_angle_end;
extern int vTaskSetTaskNumber;
extern int ets_timer_arm_us;
extern int mbedtls_x509_get_serial;
extern int psa_aead_decrypt;
extern int __ashrdi3;
extern int timer_reset_with_interval;
extern int r_ble_lll_sched_insertion_begin;
extern int thread_alloc;
extern int lv_style_prop_lookup_flags;
extern int lv_textarea_set_text_selection;
extern int phy_set_pbus_mem_5g;
extern int esp_netif_attach;
extern int _ZGVNSt8time_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int r_ble_ll_ctrl_is_inited_check;
extern int offchan_send_action_tx_status;
extern int r_ble_lll_mmgmt_reset;
extern int __find_locale;
extern int lv_group_get_editing;
extern int __locale_mb_cur_max;
extern int ieee80211_alloc_proberesp;
extern int r_ble_ll_scan_get_addr_from_ext_adv;
extern int i2c_controller_write_register;
extern int phy_freq_i2c_data_write;
extern int esp_set_scan_ie;
extern int g_scan_chan_list;
extern int r_ble_lll_scan_init;
extern int wDev_disable_low_rate;
extern int _ZTVSt11logic_error;
extern int lvgl_get_ui_density;
extern int esp_wifi_ap_get_prof_password_internal;
extern int lv_obj_destruct;
extern int coex_init;
extern int lv_list_add_button;
extern int fread;
extern int esp_vfs_uart_get_vfs;
extern int esp_sha_hash_compute;
extern int r_ble_ll_ctrl_initiate_tx_pwr_ctrl_req;
extern int tcpip_send_msg_wait_sem;
extern int lodepng_convert;
extern int esp_partition_write_raw;
extern int lv_ll_ins_head;
extern int temperature_sensor_attributes;
extern int r_ble_lll_deinit;
extern int lv_obj_get_coords;
extern int lv_line_set_points;
extern int esp_log_cache_set_level;
extern int r_ble_lll_sync_coex_dpc_update_on_event_end;
extern int lv_chart_class;
extern int pm_c5_flash_freq_limit_init_include_func;
extern int esp_wifi_ap_get_prof_authmode_internal;
extern int wifi_ant_to_ant_type;
extern int esp_log_util_set_cache_enabled_cb;
extern int xQueueGetMutexHolder;
extern int lv_display_get_layer_sys;
extern int wifi_mesh_ps_duty_cycle_get_process;
extern int lodepng_info_init;
extern int wpa_deauthenticate;
extern int pm_coex_customized_wifi_time_slice;
extern int multi_heap_free_size_impl;
extern int ble_bb_cte_set_cte_ant_switch_delay_rx_aod_2us;
extern int netif_set_down;
extern int r_ble_ll_adv_next_chan;
extern int nvs_set_u32;
extern int DISPLAY_TYPE;
extern int lwip_netconn_do_listen;
extern int adc_apb_periph_claim;
extern int hostap_new_assoc_sta;
extern int pwr_hal_set_mac_modem_state_sleep_limit;
extern int xPortSysTickHandler;
extern int npl_freertos_callout_get_ticks;
extern int power_supply_set_quick_charge_enabled;
extern int uxTaskPriorityGet;
extern int r_ble_lll_sync_sched_priority_increase;
extern int r_ble_lll_mmgmt_set_resv_count;
extern int ppInitTxq;
extern int __addsf3;
extern int r_ble_lll_adv_recycle_sch_item;
extern int lmacDiscardAgedMSDU;
extern int dhcp_release;
extern int heap_caps_init;
extern int ic_update_modem_sleep_default_params;
extern int __ashldi3;
extern int pm_on_isr_sleep;
extern int __libc_init_array;
extern int haptic_start_playback;
extern int coex_schm_ble_mesh_config_bt_default_wifi_scan;
extern int r_ble_ll_get_npl_element_info;
extern int r_ble_ll_adv_conn_req_rxd;
extern int wifi_hmac_init;
extern int esp_vfs_read;
extern int ff_diskio_get_drive;
extern int _ZTVSt20bad_array_new_length;
extern int ledc_hal_clear_left_off_fade_param;
extern int multi_heap_aligned_alloc_offs;
extern int r_ble_ll_mtrack_initialized_set;
extern int sae_pk_base32_decode;
extern int ble_sm_timer;
extern int lvgl_get_statusbar_icon_font_height;
extern int _Z16init_mtk_pa1616sP6Device;
extern int __fixdfsi;
extern int scan_fill_wps_scan_ie;
extern int sdspi_host_get_real_freq;
extern int ftm_is_responder_supported;
extern int r_hal_timer_env_init;
extern int ble_gattc_rx_prep_write_rsp;
extern int mbedtls_asn1_get_tag;
extern int r_ble_lll_adv_alloc_memory;
extern int coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_connecting;
extern int psa_hash_finish;
extern int r_ble_bb_cte_sample_limit_get;
extern int mbedtls_pk_can_do_psa;
extern int crypto_bignum_exptmod;
extern int esp_mmu_paddr_find_caps;
extern int r_ble_lll_adv_init;
extern int r_ble_log_internal_x0;
extern int hostapd_send_eapol;
extern int ant_dft_cfg;
extern int i2s_periph_signal;
extern int wifi_deinit_in_caller_task;
extern int ble_gap_notify_tx_event;
extern int thread_get_current;
extern int lv_image_header_cache_init;
extern int pp_deattach;
extern int ble_gatts_send_next_indicate;
extern int xTaskGenericNotify;
extern int __atomic_load_8;
extern int phy_i2c_readReg;
extern int dhcps_delete;
extern int ble_svc_sps_reset;
extern int _Z15is_pin_invertedP14GpioDescriptor;
extern int phy_module_has_clock_bits;
extern int lv_draw_sw_letter;
extern int r_ble_ll_arr_env_init;
extern int rcGet11NHighestRateIdx;
extern int esp_transport_read;
extern int esp_panic_handler_disable_timg_wdts;
extern int ble_hs_misc_peer_addr_type_to_id;
extern int mbedtls_ssl_parse_sig_alg_ext;
extern int i2c_controller_read_register;
extern int ieee80211_decap_amsdu;
extern int sdmmc_init_card_hs_mode;
extern int lv_style_set_border_post;
extern int ble_att_chan_mtu;
extern int spi_device_get_trans_result;
extern int itwt_probe_rc_tx_cb;
extern int r_ble_hci_trans_env_init;
extern int mbedtls_psa_cipher_encrypt_setup;
extern int esp_vfs_fat_mount_initialized;
extern int wDev_remove_KeyEntry;
extern int _ZGVNSt7__cxx119money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int rc4_skip;
extern int r_advertise_filter_init;
extern int i2c_param_config;
extern int r_ble_ll_adv_get_local_rpa;
extern int r_ble_scan_advertise_filter_init;
extern int lv_draw_sw_blend_color_to_rgb888;
extern int r_ble_lll_scan_set_aux_data_at;
extern int wpa_get_ntp_timestamp;
extern int coex_schm_ble_default_bt_default_wifi_conn;
extern int set_client_config;
extern int noise_check_loop;
extern int tcpip_init;
extern int r_ble_ll_utils_csa2_prng;
extern int prvTaskCreateDynamicPinnedToCoreWithCaps;
extern int device_listener_notify;
extern int fopen;
extern int vsprintf;
extern int ip_addr_any_type;
extern int regdma_link_new_continuous;
extern int esp_supplicant_unset_all_appie;
extern int ieee80211_mt_key_clear_mask;
extern int cache_hal_freeze;
extern int pbuf_copy_partial_pbuf;
extern int cs_send_to_ctrl_sock;
extern int crypto_ec_get_b;
extern int tcp_accept;
extern int r_ble_lll_mmgmt_rxbuffer_recycle_empty_hdr;
extern int esp_sha1_driver_clone;
extern int mbedtls_ssl_ciphersuite_from_id;
extern int r_ble_ll_mem_move;
extern int wDev_isNANPktInValidSlot;
extern int coex_schm_bt_a2dp_wifi_connecting;
extern int ESP_EFUSE_KEY4;
extern int ieee80211_regdomain_min_chan;
extern int lv_obj_set_local_style_prop;
extern int modem_clock_hal_enable_ble_rtc_timer_clock;
extern int r_ble_ll_adv_aux_conn_rsp_pdu_make;
extern int lv_rb_init;
extern int scan_start;
extern int ble_store_key_from_value_rpa_rec;
extern int pwr_hal_set_beacon_filter_abort_enable;
extern int cnx_rc_search;
extern int bundle_put_int64;
extern int ieee80211_set_tx_pti;
extern int ieee80211_hostapd_beacon_txcb;
extern int _ZTVSt9bad_alloc;
extern int raw_connect;
extern int mbedtls_ssl_conf_renegotiation;
extern int lv_anim_set_exec_cb;
extern int esp_http_client_set_header;
extern int esp_transport_get_errno;
extern int lv_textarea_get_text;
extern int r_ble_ll_conn_module_reset;
extern int __d_vfprintf;
extern int r_ble_ll_env_deinit;
extern int esp_partition_find_first;
extern int POWER_SUPPLY_TYPE;
extern int ble_hs_conn_find_assert;
extern int r_ble_lll_sched_list_details_dump;
extern int lv_theme_get_from_obj;
extern int mbedtls_ecp_gen_privkey;
extern int lv_screen_load_anim;
extern int chm_cancel_op;
extern int phy_ant_need_update;
extern int wdev_set_promis;
extern int file_system_get_owner;
extern int spi_hal_init;
extern int r_ble_ll_hci_set_adv_data;
extern int tt_timezone_set_format_24_hour;
extern int xQueueCreateCountingSemaphoreStatic;
extern int _ZTVSt7codecvtIDic10_mbstate_tE;
extern int rcGetHighestRateIdx;
extern int log;
extern int ble_hs_hci_cmd_send_buf;
extern int phy_wifi_enable_set;
extern int nvs_flash_erase;
extern int ble_gattc_rx_err;
extern int ppRegisterRxCallback;
extern int ip6_route;
extern int TRC_AMPDU_PER_DOWN_THRESHOLD;
extern int r_ble_ll_sync_periodic_ind;
extern int pm_set_sleep_type;
extern int hid_gamepad_input_handle;
extern int ieee80211_node_pwrsave;
extern int tcp_recv_null;
extern int bluetooth_scan_stop;
extern int r_ble_ll_mem_memblock_put_cb;
extern int lv_area_is_point_on;
extern int lv_draw_sw_layer;
extern int hal_set_pri20_chan_offset;
extern int crypto_ec_point_mul;
extern int _ZSt19__throw_ios_failurePKc;
extern int scan_validate_owe_scenarios;
extern int mbedtls_ssl_finish_handshake_msg;
extern int spi_flash_hal_init;
extern int ecc_hal_write_verify_param;
extern int ledc_set_fade_step_and_start;
extern int lv_obj_area_is_visible;
extern int mbedtls_oid_get_oid_by_md;
extern int lv_timer_enable;
extern int esp_lcd_panel_disp_on_off;
extern int lv_color32_make;
extern int lv_rand_set_seed;
extern int _ZSt3hexRSt8ios_base;
extern int _ZSt9use_facetINSt7__cxx118numpunctIcEEERKT_RKSt6locale;
extern int wifi_softap_max_support_num;
extern int hal_he_disable_obss_narrow_bw_ru;
extern int get_vfs_for_path;
extern int rtc_dig_clk8m_enable;
extern int wifi_get_bss_color;
extern int r_ble_ll_arr_origin_update;
extern int _ZTV8WL_Flash;
extern int r_ble_hci_trans_deinit;
extern int esp_aes_cipher_decrypt;
extern int os_random;
extern int wDevMacSleep;
extern int uECC_generate_random_int;
extern int rcGet11GHighestRateIdx;
extern int lv_image_decoder_set_close_cb;
extern int r_ble_ll_scan_parse_ext_hdr;
extern int uECC_vli_modSub;
extern int esp_tls_get_platform_time;
extern int r_get_le64;
extern int lv_binfont_create;
extern int lv_timer_handler_set_resume_cb;
extern int lv_led_class;
extern int esp_lcd_panel_set_gap;
extern int _uart_set_pin6;
extern int xTimerGetExpiryTime;
extern int r_ble_hw_cte_set_max_buffer_number;
extern int wl_sector_size;
extern int rc_get_G6M_sched;
extern int gpio_descriptor_enable_interrupt;
extern int r_ble_ll_ctrl_rx_phy_update_ind;
extern int esp_transport_get_error_handle;
extern int _ZSt17__verify_groupingPKcjRKSs;
extern int esp_send_assoc_resp;
extern int acd_netif_ip_addr_changed;
extern int _ZTISt12system_error;
extern int uart_controller_get_available;
extern int lwip_freeaddrinfo;
extern int chmod;
extern int _ZTVSt15_Sp_counted_ptrIPN2tt7service5audio12AudioServiceELN9__gnu_cxx12_Lock_policyE1EE;
extern int pm_keep_alive_timeout_process;
extern int ble_hs_mbuf_att_pkt;
extern int lv_arc_set_angles;
extern int pm_coex_update_rx_beacon_pti;
extern int esp_vfs_select_triggered;
extern int cache_hal_is_cache_enabled;
extern int r_ble_ll_resolv_local_addr_rd;
extern int lv_color_16_16_mix;
extern int r_ble_lll_mmgmt_rxbuf_direct_alloc;
extern int r_ble_ll_scan_env_init;
extern int modem_clock_deselect_all_module_lp_clock_source;
extern int r_ble_ll_sync_cancel_complete_event;
extern int __bufio_close;
extern int lora_add_rx_callback;
extern int esp_vApplicationTickHook;
extern int r_ble_scan_advertise_filter_destroy;
extern int ieee80211_parse_hecap;
extern int ble_att_svr_ticks_until_tmo;
extern int r_ble_hw_whitelist_clear;
extern int ieee80211_parse_htc;
extern int esp_elf_free;
extern int esp_rom_vprintf;
extern int ble_sm_alg_gen_key_pair;
extern int gps_settings_remove_configuration_at;
extern int lvgl_port_lock;
extern int ic_set_ac_param;
extern int esp_crt_bundle_attach;
extern int r_ble_ll_adv_clear_all;
extern int _do_wifi_start;
extern int lv_list_text_class;
extern int netconn_listen_with_backlog;
extern int test_nonmimo_update_user_info;
extern int cs_create_ctrl_sock;
extern int bluetooth_start_advertising;
extern int ble_store_config_read;
extern int pwr_hal_set_beacon_filter_force_sync_enable;
extern int lv_obj_is_valid;
extern int preferences_opt_string;
extern int lv_obj_set_scrollbar_mode;
extern int __kernel_tan;
extern int _ZTVSt7codecvtIwc10_mbstate_tE;
extern int r_ble_ll_conn_update_new_phy;
extern int ESP_EFUSE_WR_DIS_KEY_PURPOSE_0;
extern int hal_mac_get_txq_complete;
extern int r_ble_ll_rand_prand_get;
extern int mpi_hal_write_m_prime;
extern int sdmmc_send_cmd_send_status;
extern int i2c_controller_read;
extern int hal_set_sta_beacon_filter;
extern int sha_hal_read_digest;
extern int r_ble_ll_hci_ev_phy_update;
extern int pm_deattach;
extern int lv_calendar_class;
extern int netif_invoke_ext_callback;
extern int r_ble_lll_conn_recycle_sch_item;
extern int phy_set_rx_pbus_freq;
extern int r_ble_lll_scan_rx_process;
extern int wifi_he_get_hetb_tid_bitmap;
extern int hal_he_set_bcast_ru;
extern int uart_hal_set_rx_timeout;
extern int esp_netif_get_flags;
extern int lv_group_remove_obj;
extern int r_ble_ll_ctrl_rx_pwr_ctrl_resp;
extern int ble_hs_conn_find_by_addr;
extern int psa_verify_hash_builtin;
extern int cnx_node_join;
extern int uECC_vli_cmp;
extern int vTaskSetThreadLocalStoragePointerAndDelCallback;
extern int ble_gap_rx_param_req;
extern int psa_unregister_read;
extern int mbedtls_base64_decode;
extern int r_ble_hw_get_static_addr;
extern int ceilf;
extern int pmu_hp_system_init;
extern int _ZGVNSt9money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int r_ble_phy_sleep_related_etm_check;
extern int ets_isr_mask;
extern int spi_flash_hal_gpspi_supports_direct_read;
extern int r_ble_ll_adv_sm_create;
extern int file_system_unmount;
extern int scan_stack_deinitEnv;
extern int esp_vfs_unlink;
extern int hal_crypto_enable;
extern int pm_update_next_tbtt;
extern int map_wifi_config_sae_pwe_to_supp;
extern int ppCheckTxHEAMPDUlength;
extern int g_wifi_menuconfig;
extern int ESP_EFUSE_WR_DIS_KEY_PURPOSE_4;
extern int _ZSt16__introsort_loopIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEEiNS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SI_SK_T1_;
extern int _ZSt7nothrow;
extern int resend_eapol_handle;
extern int r_ble_lll_linkstate_cte_config_init;
extern int wifi_nvs_validate_sta_listen_interval;
extern int mbedtls_ecdsa_verify;
extern int ieee80211_regdomain_max_chan;
extern int _Z25ble_spp_init_gatt_handlesP6Device;
extern int esp_wifi_set_storage;
extern int g_offchan_packet_lifetime;
extern int adv_stack_disable;
extern int pm_is_sleeping;
extern int s_vht_cap_bmfmee_nsts;
extern int rc5G11AXSchedTbl;
extern int esp_startup_start_app;
extern int lv_buttonmatrix_class;
extern int esp_wifi_get_event_mask;
extern int esp_http_client_set_redirection;
extern int esp_crypto_ecc_enable_periph_clk;
extern int mbedtls_md_hmac_update;
extern int mbedtls_ssl_write_version;
extern int ff_diskio_register;
extern int spi_bus_lock_bg_entry;
extern int GPIO_CONTROLLER_TYPE;
extern int cnx_connect_next_ap_timeout;
extern int vTaskSuspendAll;
extern int sleep_retention_find_link_by_id;
extern int wDev_Reset_TBTT;
extern int r_ble_ll_hci_ev_encrypt_chg;
extern int bt_rf_coex_cfg_cb;
extern int ESP_EFUSE_ADC1_CH0_ATTEN0_INITCODE_DIFF;
extern int gdma_hal_is_tx_link_switch_event_supported;
extern int wifi_module_disable;
extern int driver_destruct;
extern int r_ble_lll_adv_get_sec_pdu_len;
extern int trc_get_80211_tx_rate_config;
extern int itwt_setup_dwell_timeout_fn;
extern int wifi_wpa2_is_started;
extern int mbedtls_ssl_get_mode_from_transform;
extern int wdev_csi_rx_process;
extern int esp_wifi_sta_disable_wpa2_authmode_internal;
extern int ble_hs_deinit;
extern int _ZGVNSt7num_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int pwm_set_inverted;
extern int r_os_mbuf_copydata;
extern int npl_freertos_get_current_task_id;
extern int ble_att_clt_rx_exec_write;
extern int esp_tls_get_conn_sockfd;
extern int lv_textarea_cursor_up;
extern int pm_get_tx_blocks_retention_mask;
extern int ble_store_util_delete_all;
extern int r_os_mbuf_appendfrom;
extern int ble_gattc_rx_exec_write_rsp;
extern int netconn_free;
extern int xEventGroupCreate;
extern int ic_register_michael_mic_failure_cb;
extern int ieee80211_assoc_req_construct;
extern int esp_test_set_rx_error_occurs;
extern int sys_now;
extern int mac_tx_get_rts_rate;
extern int mbrtowc;
extern int r_ble_lll_adv_periodic_sch_cnt_required;
extern int esp_wifi_80211_tx;
extern int usb_hid_device_stop;
extern int towlower;
extern int r_ble_ll_conn_sm_npl_deinit;
extern int lv_obj_is_group_def;
extern int sdmmc_init_sd_driver_strength;
extern int os_mempool_clear;
extern int lv_point_swap;
extern int ieee80211_rfid_locp_recv;
extern int _ZTv0_n12_NSt14basic_ofstreamIcSt11char_traitsIcEED0Ev;
extern int sys_mbox_free;
extern int raw_bind_netif;
extern int r_ble_lll_scan_copy_into_mbuf;
extern int rcUpdateAckSnr;
extern int adiList_stack_listClear;
extern int ieee80211_get_nvs_he_dcm_max_constellation_tx;
extern int __wrap_lv_obj_set_flex_flow;
extern int ble_gattc_rx_read_rsp;
extern int vTaskPlaceOnEventList;
extern int preferences_has_bool;
extern int _ZSt13__int_to_charIwyEiPT_T0_PKS0_St13_Ios_Fmtflagsb;
extern int pp_register_timer_cb;
extern int crypto_bignum_rshift;
extern int esp_rsa_ds_pad_v15_unpad;
extern int bt_rf_coex_hooks_p;
extern int esp_lcd_panel_io_tx_color;
extern int lv_obj_get_group;
extern int mbedtls_cipher_info_from_values;
extern int pm_is_in_wifi_slice_threshold;
extern int lv_pct;
extern int esp_test_get_tx_tb_statistics;
extern int coex_core_release;
extern int wpa_crypto_funcs_init;
extern int esp_cpu_set_breakpoint;
extern int _ZTTSo;
extern int spi_flash_common_write_status_16b_wrsr;
extern int esp_libc_include_strcpy_impl;
extern int rc11GSchedTbl;
extern int r_ble_lll_conn_event_delete_and_reschedule;
extern int hci_transport_host_acl_tx;
extern int wpa_receive;
extern int r_ble_ll_df_init;
extern int lv_refr_get_disp_refreshing;
extern int ble_ll_conn_env_p;
extern int lv_pct_to_px;
extern int sae_process_commit;
extern int lv_obj_set_grid_cell;
extern int ppClearRxFragment;
extern int rtc_clk_freq_cal;
extern int r_ble_lll_conn_module_reset;
extern int bt_bb_tx_cca_fifo_empty;
extern int gdma_ahb_hal_stop;
extern int esp32_gpio_is_mspi_pin;
extern int xQueueGenericSend;
extern int spi_flash_hal_erase_sector;
extern int mbedtls_ssl_get_ciphersuite_sig_alg;
extern int r_ble_ll_sync_established;
extern int r_ble_ll_rxpdu_alloc;
extern int npl_freertos_callout_is_active;
extern int bt_bb_coex_config;
extern int esp_event_handler_unregister_with_internal;
extern int sys_arch_protect;
extern int ble_l2cap_sig_deinit;
extern int r_npl_funcs;
extern int ble_store_write_peer_sec;
extern int uart_hal_init;
extern int g_ble_ll_supp_cmds_ro;
extern int r_ble_ll_whitelist_clear;
extern int cnx_add_to_blacklist;
extern int __log10Pow5;
extern int esp_coex_common_env_is_chip_wrapper;
extern int ble_gatts_stop;
extern int nan_is_in_dw;
extern int esp_http_client_read_response;
extern int r_ble_hw_cte_reset;
extern int esp_fast_psk;
extern int pm_coex_separate_connectionless_window;
extern int hal_he_set_bss_color;
extern int ip6_addr_any;
extern int ic_txq_empty;
extern int vht_set_snd_ch_cfg;
extern int mbedtls_psa_mac_update;
extern int mbedtls_mpi_core_lt_ct;
extern int lv_obj_get_scroll_y;
extern int dragonfly_generate_scalar;
extern int phy_i2c_writeReg_Mask;
extern int gdma_ahb_hal_get_intr_status_reg;
extern int tsf_hal_set_tbtt_soc_wakeup_enable;
extern int mbedtls_mpi_set_bit;
extern int hal_get_tsf_time;
extern int pwr_hal_clear_mac_modem_beacon_miss_intr_filter;
extern int _ZGVNSt7num_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int time;
extern int camera_close;
extern int _ZTVSt23__codecvt_abstract_baseIDsc10_mbstate_tE;
extern int __flockfile_init;
extern int lv_obj_set_style_text_outline_stroke_color;
extern int hal_mac_tx_set_cca;
extern int ieee80211_encap_esfbuf;
extern int __wrap__Unwind_RaiseException;
extern int _ZTVSt10moneypunctIwLb1EE;
extern int phy_get_freq_init;
extern int lv_keyboard_create;
extern int rc11NSchedTbl;
extern int timer_start;
extern int nan_get_clust_id;
extern int _ZTVSt15_Sp_counted_ptrIPN2tt7service7rtctime14RtcTimeServiceELN9__gnu_cxx12_Lock_policyE1EE;
extern int esp_wifi_sta_get_config_sae_pk_internal;
extern int gdma_ahb_hal_set_weight;
extern int audio_codec_close;
extern int ble_sm_enc_key_refresh_rx;
extern int r_ble_phy_get_min_txpwr_dbm;
extern int ble_att_is_request_op;
extern int ESP_EFUSE_KEY0;
extern int r_ble_lll_get_txed_buffer;
extern int ble_uuid_to_mbuf;
extern int he_twt_teardown_post_event;
extern int xTaskPriorityDisinherit;
extern int sdmmc_send_cmd_read_ocr;
extern int ble_sm_enc_change_rx;
extern int lwip_fcntl;
extern int gpio_get_level;
extern int _ZZNSt19_Sp_make_shared_tag5_S_tiEvE5__tag;
extern int owe_process_assoc_resp;
extern int phy_init_data;
extern int lv_obj_get_child_count_by_type;
extern int esp_mbedtls_write;
extern int wifi_nan_set_config_local;
extern int __wrap_lv_list_add_button;
extern int lv_obj_scroll_by_raw;
extern int coex_core_status_get;
extern int trc_SetTxAmpduState;
extern int r_ble_lll_adv_txbuf_adi_field_update;
extern int pwm_enable;
extern int in_rssi_adjust;
extern int ieee80211_add_countryie;
extern int fgetc;
extern int iswxdigit;
extern int phy_i2cmst_reg_init;
extern int phy_agc_reg_init_new;
extern int sae_write_commit;
extern int pmksa_cache_get_opportunistic;
extern int mbedtls_mpi_cmp_int;
extern int display_get_mirror_x;
extern int uart_flush;
extern int raw_new;
extern int stderr;
extern int coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_connecting;
extern int cache_hal_suspend;
extern int tcp_active_pcbs;
extern int s_fix_rate_mask;
extern int ppCheckIsConnTraffic;
extern int I8080_CONTROLLER_TYPE;
extern int wDev_Rxbuf_Deinit;
extern int minitar_read_contents_to_file;
extern int pwr_hal_get_mac_modem_state_sleep_limit_exceeded_status;
extern int esp_wifi_sta_pmf_enabled;
extern int g_wdev_csi_rx;
extern int i2s_init_dma_intr;
extern int r_ble_ll_is_addr_empty;
extern int netif_init;
extern int esp32_grove_driver;
extern int _ZSt9uppercaseRSt8ios_base;
extern int dns_gethostbyname;
extern int r_ble_phy_freq_to_chan;
extern int hal_sniffer_disable;
extern int r_ble_lll_scan_resume_aux_scan;
extern int he_data_bits_per_sym;
extern int mbedtls_cipher_init;
extern int ic_deinit;
extern int crypto_ec_prime_len;
extern int esp_sha_hash_finish;
extern int wpa_parse_kde_ies;
extern int etharp_output;
extern int aes_hal_transform_dma_start;
extern int ieee80211_mlme_connect_bss;
extern int _ZGVNSt7__cxx119money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int ble_l2cap_prepend_hdr;
extern int event_group_clear;
extern int uart_enable_tx_intr;
extern int hal_vht_enable_bwsignaling_rts;
extern int cnx_rc_update_state_metric;
extern int _g_esp_netif_netstack_default_wifi_sta;
extern int r_ble_lll_sleep_wake_up_ahead_check;
extern int ccmp_encap;
extern int wifi_station_start;
extern int ble_hs_log_flat_buf;
extern int esp_http_client_prepare;
extern int ieee80211_psq_send_one_pkt;
extern int _vector_table;
extern int phy_pa_dc_code_set;
extern int crypto_bignum_sqrmod;
extern int r_ble_lll_adv_calculate_aux_duration;
extern int sdmmc_enable_hs_mode_and_check;
extern int r_ble_ll_ctrl_rx_phy_req;
extern int lv_obj_is_editable;
extern int netconn_bind;
extern int lv_display_set_buffers;
extern int lv_obj_set_style_outline_pad;
extern int r_ble_ll_sync_parse_ext_hdr;
extern int r_ble_ll_conn_slave_start;
extern int lv_event_push;
extern int wdev_funcs_deinit;
extern int r_ble_lll_scan_recycle_sch_item;
extern int mbedtls_symbols;
extern int coex_schm_ble_mesh_standby_bt_a2dp_wifi_conn;
extern int lv_dropdown_class;
extern int r_ble_ll_adv_set_scan_rsp_data;
extern int BasicOFDMSched;
extern int ppRegressAmpdu;
extern int esp_sha_hash_abort;
extern int pm_send_sleep_null_cb;
extern int esp_efuse_utility_read_reg;
extern int s_is_6m;
extern int lmacProcessAllTxTimeout;
extern int esp_wifi_sta_prof_is_wpa2_internal;
extern int s_hci_stack_vsEventsMask;
extern int esp_netif_sntp_renew_servers;
extern int __bufio_setdir_locked;
extern int _ZZNSt18__moneypunct_cacheIwLb0EE8_M_cacheERKSt6localeEN11_Scoped_strC2ERKSbIwSt11char_traitsIwESaIwEE;
extern int r_ble_lll_sleep_rtc_to_ticks;
extern int wl_read;
extern int etharp_input;
extern int lodepng_get_raw_size;
extern int mspi_timing_psram_tuning;
extern int r_ble_ll_conn_sm_fetch_new;
extern int sha256_prf;
extern int lvgl_module_symbols;
extern int esp_sha_block;
extern int phy_get_pkdet_data;
extern int lv_image_src_get_type;
extern int __eqsf2;
extern int xTimerCreate;
extern int mbedtls_mpi_mod_mpi;
extern int _ZTVN2tt7service11development18DevelopmentServiceE;
extern int wifi_calloc;
extern int r_ble_ll_resolv_gen_rpa;
extern int lmacGetTxFrame;
extern int key_mgr_hal_continue;
extern int pm_is_force_return_home_chan;
extern int r_ble_rtc_wake_up_cpu_set;
extern int modem_clock_hal_select_wifi_lpclk_source;
extern int ble_hs_id_set_pub;
extern int btowc;
extern int gcc_soft_float_symbols;
extern int r_ble_ll_ctrl_proc_rsp_timer_cb;
extern int lv_display_get_layer_top;
extern int fileno;
extern int ble_gap_connect;
extern int esp_transport_ssl_crt_bundle_attach;
extern int dup_binstr;
extern int arr_hci_setSchedLen;
extern int ble_gattc_read_by_uuid;
extern int vTaskResume;
extern int bluetooth_hid_device_send_keyboard;
extern int ieee80211_update_bandwidth;
extern int sdspi_host_set_card_clk;
extern int r_ble_lll_df_env_init;
extern int esp_test_disable_rx_statistics;
extern int ieee80211_pm_tx_null_process;
extern int lv_buttonmatrix_set_ctrl_map;
extern int pm_on_coex_schm_process_restart;
extern int register_chipv7_phy;
extern int regdma_link_init;
extern int _ZTVNSt8ios_base7failureB5cxx11E;
extern int phy_freq_mem_data;
extern int ble_bb_cte_set_cte_samp_limit_ref;
extern int mbedtls_asn1_get_bool;
extern int uECC_valid_public_key;
extern int lv_obj_scroll_to_x;
extern int temp_sensor_get_raw_value;
extern int __bufio_get;
extern int ppSearchTxQueue;
extern int r_ble_lll_adv_reset;
extern int sdmmc_send_cmd_crc_on_off;
extern int __cxa_guard_abort;
extern int module_ensure_started;
extern int esp_wifi_set_config;
extern int memory_free;
extern int gpio_descriptor_get_owner_type;
extern int coex_core_pre_init;
extern int esp_wifi_sta_get_prof_ssid_internal;
extern int __lock___libc_recursive_mutex;
extern int __popcountsi2;
extern int sdmmc_io_rw_direct;
extern int heap_caps_get_allocated_size;
extern int mbedtls_mpi_fill_random;
extern int rtc_clk_slow_src_set;
extern int multi_heap_aligned_free;
extern int coex_schm_interval_set;
extern int ble_l2cap_sig_conn_broken;
extern int uECC_secp256r1;
extern int r_ble_log_reset_buf_index_flag;
extern int __time_weekday;
extern int vprintf;
extern int cnx_bss_init;
extern int r_ble_lll_sleep_is_enabled;
extern int write;
extern int lv_slider_get_mode;
extern int hal_agreement_del_extra_softap_rx_ba;
extern int memp_RAW_PCB;
extern int g_wdev_record_t2t3_cb;
extern int r_ble_lll_mmgmt_alloc_sch;
extern int _ZGVNSt7__cxx1110moneypunctIcLb0EE2idE;
extern int esp_intr_enable;
extern int __cxx_eh_arena_size_get;
extern int esp_reset_reason_set_hint;
extern int r_ble_ll_conn_hcc_params_set_fallback;
extern int mbedtls_mpi_shift_r;
extern int coex_schm_ble_mesh_config_bt_a2dp_wifi_connecting;
extern int lv_color_lighten;
extern int i2s_output_gpio_revoke;
extern int _ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EEPKS5_OS8_;
extern int ecc_hal_read_mul_result;
extern int _regi2c_impl_write_mask;
extern int wpa_auth_for_each_sta;
extern int __cxx_init_dummy;
extern int ble_gap_conn_find;
extern int ic_del_key;
extern int _findenv;
extern int lv_spinbox_set_value;
extern int wifi_rf_phy_enable;
extern int r_ble_ll_chk_txrx_octets;
extern int esp_md5_hash_clone;
extern int esp_transport_ssl_set_ds_data;
extern int uECC_vli_modAdd;
extern int r_ble_ll_set_random_addr;
extern int wpa_sm_set_ap_rsn_ie;
extern int lv_realloc_core;
extern int pm_twt_wake_done;
extern int mbedtls_sha256_update;
extern int _cosf;
extern int task_event_group_claim_bit;
extern int lv_indev_find_scroll_obj;
extern int wifi_get_rand_macaddr;
extern int r_ble_lll_adv_reschedule_event;
extern int lv_obj_class_create_obj;
extern int coex_schm_curr_phase_get;
extern int base_stack_enable;
extern int mmu_hal_ctx_init;
extern int spi_flash_guard_get;
extern int crypto_bignum_is_odd;
extern int crypto_bignum_sub;
extern int _ZSt8_DestroyIPSt6vectorIhSaIhEEEvT_S4_;
extern int intr_handler_get;
extern int uart_hal_set_rxfifo_full_thr;
extern int pwr_hal_set_mac_modem_state_sleep_limit_exceeded_wakeup_enable;
extern int r_hal_timer_convert_to_u32;
extern int r_ble_ll_conn_adj_data_len_param;
extern int mbedtls_ecp_point_free;
extern int tcp_zero_window_probe;
extern int wifi_mode_set;
extern int bluetooth_unpair;
extern int hid_report_map_gamepad;
extern int cnx_node_is_existing;
extern int wdev_mac_special_reg_store;
extern int esp_vfs_lseek;
extern int setvbuf;
extern int ethip6_output;
extern int r_ble_ll_scan_set_peer_rpa;
extern int lv_arc_rotate_obj_to_angle;
extern int r_ble_ll_ctrl_conn_param_reply;
extern int r_ble_hw_get_cte_packet_rx_rate_from_buffer;
extern int gdma_set_weight;
extern int sdmmc_fix_host_flags;
extern int esp_efuse_rtc_calib_get_chan_compens;
extern int wifi_nvs_validate_ap_password;
extern int esp_owe_build_assoc_resp_dhie;
extern int ble_gattc_disc_all_svcs;
extern int __isnanf;
extern int nimble_serial_api;
extern int wifi_txq_empty;
extern int _ZTVSt17_Sp_counted_arrayISaIhELN9__gnu_cxx12_Lock_policyE1EE;
extern int nan_get_tsf;
extern int ppCertSetRate;
extern int lv_fs_seek;
extern int memp_ND6_QUEUE;
extern int r_ble_lll_conn_coex_dpc_update;
extern int lv_display_delete_event;
extern int asprintf;
extern int mbedtls_ssl_update_handshake_status;
extern int rc11ASchedTbl;
extern int r_ble_hw_cte_set_sample_ring_buffer_ptr;
extern int i2s_get_source_clk_freq;
extern int owe_deinit;
extern int ieee80211_regdomain_ap_min_chan;
extern int dns_mquery_v6group;
extern int hal_init_bf;
extern int esp_event_handler_register;
extern int trc_set_bf_report_rate;
extern int wpabuf_put;
extern int rtc_clk_cpu_freq_set_config_fast;
extern int ESP_EFUSE_USER_DATA_MAC_CUSTOM;
extern int psa_destroy_persistent_key;
extern int wifi_scan_stop_process;
extern int esp_newlib_locks_init;
extern int uart_disable_tx_intr;
extern int regi2c_ctrl_write_reg_mask;
extern int lv_async_call_cancel;
extern int ieee80211_free_beacon_eb;
extern int _ZnajRKSt9nothrow_t;
extern int iscntrl;
extern int esp_newlib_init;
extern int r_ble_ll_hci_env_init;
extern int ieee80211_set_key;
extern int r_ble_ll_mtrack_initialized_get;
extern int lv_anim_set_deleted_cb;
extern int phy_i2c_pkdet_set;
extern int lv_display_refr_timer;
extern int mpi_hal_clear_interrupt;
extern int lv_anim_init;
extern int esp_event_loop_delete;
extern int mbedtls_asn1_get_int;
extern int ble_gap_conn_cancel;
extern int mbedtls_ssl_init;
extern int isatty;
extern int phy_rx_sig_pwr_sub;
extern int r_ble_lll_conn_chk_wthr_adj_max_evt_time;
extern int destination_cache;
extern int lmacDiscardMSDU;
extern int btwt_setup_timer;
extern int wifi_nvs_init;
extern int hid_report_map_keyboard_consumer_len;
extern int ieee80211_btwt_teardown;
extern int wpa_auth_sta_init;
extern int esp_wifi_destroy_if_driver;
extern int ble_l2cap_rx;
extern int r_ble_lll_adv_sched_periodic;
extern int minmea_sentence_id;
extern int wl_write;
extern int esp_vfs_mkdir;
extern int pthread_include_pthread_impl;
extern int spi_flash_chip_winbond_get_caps;
extern int ble_hid_get_active;
extern int ble_store_config_write;
extern int ic_disable_rx;
extern int ccmp_encrypt;
extern int multi_heap_malloc;
extern int bluetooth_midi_send;
extern int r_advertise_filter_env_init;
extern int r_ble_lll_conn_sched_conflict_handler;
extern int ieee80211_setup_ratetable;
extern int lv_color_premultiply;
extern int r_ble_hw_driver_env_init;
extern int ff_disk_ioctl;
extern int g_panic_abort_details;
extern int mbedtls_ssl_pend_fatal_alert;
extern int closedir;
extern int r_ble_ll_mem_env_deinit;
extern int spi_flash_hal_gpspi_common_command;
extern int r_ble_ll_ctrl_enc_allowed_pdu;
extern int esp_timer_get_expiry_time;
extern int ble_hs_mbuf_l2cap_pkt;
extern int esp_newlib_time_init;
extern int ppRecordBarRRC;
extern int pwr_hal_get_mac_modem_beacon_miss_limit_exceeded_status;
extern int esp_libc_init_global_stdio;
extern int lv_obj_align;
extern int sdmmc_read_sectors;
extern int nvs_flash_init;
extern int esp_crypto_hmac_lock_acquire;
extern int g_startup_time;
extern int lv_obj_style_create_transition;
extern int esp_sha512_driver_compute;
extern int preferences_put_int64;
extern int mbedtls_mpi_gen_prime;
extern int r_ble_lll_df_env_deinit;
extern int esp_test_tx_count_retry;
extern int hal_mac_init;
extern int pm_noise_check_process;
extern int r_ble_hci_trans_hs_acl_tx;
extern int lv_obj_remove_event;
extern int audio_stream_open_input;
extern int coex_hw_timer_enable;
extern int esp_libc_include_memcmp_impl;
extern int lv_style_set_border_opa;
extern int lv_display_trigger_activity;
extern int get_channel_max_bandwidth;
extern int lv_chart_get_series_next;
extern int r_ble_ll_conn_store_received_rssi;
extern int ieee80211_update_channel;
extern int r_ble_lll_timer_current_tick_get;
extern int ledc_hal_get_clk_cfg;
extern int system_event_callback_add;
extern int lv_group_get_obj_count;
extern int mbedtls_psa_rsa_verify_hash;
extern int service_manager_find_instance;
extern int mbedtls_x509_crt_is_revoked;
extern int hal_he_bsr_init;
extern int gdma_hal_enable_burst;
extern int wifi_set_protocol_process;
extern int esf_buf_alloc_dynamic_default_handler;
extern int sha256_vector;
extern int display_get_frame_buffer_count;
extern int lv_arc_get_rotation;
extern int mbedtls_ssl_config_free;
extern int r_ble_ll_sync_next_event;
extern int wifi_bt_common_module_disable;
extern int esp_event_handler_instance_register;
extern int i2c_master_read_byte;
extern int device_set_driver_data;
extern int r_ble_lll_current_rx_info_get;
extern int sdmmc_mmc_decode_csd;
extern int timer_set_pending_callback;
extern int _ZSt17__throw_bad_allocv;
extern int __lttf2;
extern int ieee80211_recv_action_vendor_spec;
extern int r_ble_ll_adv_sm_alloc;
extern int netconn_getaddr;
extern int bluetooth_scan_start;
extern int f_stat;
extern int aes_gcm_ae;
extern int ble_random_pa_env_p;
extern int r_ble_rtc_wake_up_cpu_clr;
extern int coex_schm_ble_mesh_traffic_bt_conn_wifi_connecting;
extern int r_ble_lll_sched_search_and_delete;
extern int r_ble_lll_adv_legacy_pdu_make;
extern int spicommon_dma_desc_alloc;
extern int usb_device_controller_release;
extern int phy_dc_iq_est_new;
extern int spi_flash_chip_winbond_erase_sector;
extern int sdspi_crc7;
extern int coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_scan;
extern int phy_11p_set;
extern int rate2str;
extern int device_start;
extern int lvgl_pointer_get_calibration;
extern int fmaxf;
extern int r_ble_lll_scan_no_rxbuf_left;
extern int psa_mac_verify_finish;
extern int esp_transport_destroy;
extern int esp_cpu_compare_and_set;
extern int esp_log_impl_lock_timeout;
extern int r_ble_ll_scan_dup_new;
extern int esp_event_handler_register_with;
extern int iswprint;
extern int hostapd_setup_wpa_psk;
extern int r_ble_lll_adv_sched_exception_handle;
extern int npl_freertos_event_init;
extern int lora_add_tx_callback;
extern int sar_periph_ctrl_adc_reset;
extern int ble_l2cap_sig_create_chan;
extern int esp_deep_sleep_register_phy_hook;
extern int wpa_sm_mlme_setprotection;
extern int lv_event_get_scroll_anim;
extern int service_manager_get_state;
extern int ESP_EFUSE_KEY_PURPOSE_2;
extern int r_ble_ll_mem_msys_alloc_check;
extern int __issignaling;
extern int r_ble_lll_adv_make_done;
extern int chm_get_band_from_chan;
extern int rcLoRaRate2SchedIdx;
extern int coex_schm_get_phase_by_idx;
extern int esp_wifi_init;
extern int mbedtls_ssl_update_in_pointers;
extern int tc_aes_encrypt;
extern int lv_event_add;
extern int sys_mbox_post;
extern int lv_area_get_width;
extern int esp_http_client_get_errno;
extern int esp_test_disable_rx_mu_statistics;
extern int sdmmc_allocate_aligned_buf;
extern int tsf_hal_set_tbtt_rf_ctrl_wait_cycles;
extern int r_ble_ll_sync_send_sync_ind;
extern int keyboard_read_key;
extern int cache_hal_get_cache_line_size;
extern int acd_remove;
extern int sdspi_host_init_device;
extern int r_ble_lll_adv_sm_reset;
extern int nan_send_action_process;
extern int wpa_sm_rsn_overriding_supported;
extern int mbedtls_ct_memcpy_if;
extern int npl_freertos_eventq_deinit;
extern int mt_send;
extern int lv_timer_delete;
extern int lv_indev_get_scroll_obj;
extern int _Z8tusbStopv;
extern int _esp_error_check_failed;
extern int esp_get_free_internal_heap_size;
extern int r_ble_ll_sync_info_event;
extern int phy_iq_est_disable;
extern int phy_i2c_clk_sel;
extern int r_ble_lll_sched_delete_by_sm_num_and_type;
extern int r_ble_ll_is_rpa;
extern int lv_spinbox_step_next;
extern int mbedtls_pk_setup;
extern int esp_flash_registered_chips;
extern int i2c_controller_register8_get;
extern int cache_hal_init;
extern int esp_wifi_opr_bss_color;
extern int ppCalDeliNum;
extern int pwm_is_enabled;
extern int esp_vApplicationIdleHook;
extern int httpd_req_handle_err;
extern int r_ble_lll_scan_alloc_txbuf;
extern int coex_core_enable;
extern int ic_create_wifi_task;
extern int esp_wifi_stop;
extern int i2c_controller_register16le_set;
extern int ble_gap_timer;
extern int priv_config_opts_ro;
extern int sae_deinit_pt;
extern int lmacDiscardFrameExchangeSequence;
extern int esp_transport_ssl_set_cert_data_der;
extern int pvTimerGetTimerID;
extern int phy_freq_i2c_write_set;
extern int coex_schm_bt_conn_wifi_connecting;
extern int sta_update_mbssid;
extern int r_exception_list_ble_mesh_pb_adv_link_id_table_is_empty;
extern int lv_layout_init;
extern int mbedtls_x509_get_subject_alt_name;
extern int r_ble_ll_mem_msys_insert;
extern int __nesf2;
extern int coex_core_init;
extern int esp_rsa_ds_opaque_sign_hash_abort;
extern int bundle_opt_string;
extern int gdma_default_rx_isr;
extern int wpa_config_profile;
extern int lv_image_class;
extern int ic_register_timer_cb;
extern int localtime;
extern int lvgl_toolbar_add_spinner_action;
extern int acd_start;
extern int r_ble_ll_sync_reserve;
extern int esp_http_client_set_username;
extern int lv_image_decoder_close;
extern int r_ble_ll_conn_sm_retrieve;
extern int pp_stop_sw_txq;
extern int lv_spinbox_get_step;
extern int hci_stack_vsEventIsEnabled;
extern int trc_update_def_rate;
extern int ble_bb_cte_get_cte_samp_limit_switch;
extern int _ZSt21__throw_runtime_errorPKc;
extern int audio_stream_set_change_callback;
extern int __floatdidf;
extern int wpa_config_assoc_ie;
extern int wifi_set_default_ssid;
extern int r_ble_ll_scan_get_local_rpa;
extern int lv_obj_set_content_height;
extern int _ZGVNSt11__timepunctIcE2idE;
extern int misc_nvs_deinit;
extern int lv_slider_create;
extern int nan_faw_start_process;
extern int coex_core_request;
extern int mbedtls_gcm_free;
extern int __wrap___cxa_call_unexpected;
extern int coex_pti_get;
extern int ESP_EFUSE_ADC1_CH5_ATTEN0_INITCODE_DIFF;
extern int esp_netif_action_start;
extern int _lock_close_recursive;
extern int mbedtls_rsa_check_pubkey;
extern int lvgl_ppa_supports_color_format;
extern int esp_btbb_enable;
extern int tcp_poll;
extern int bundle_get_int32;
extern int esp32_sdspi_fs_alloc;
extern int coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_conn;
extern int lv_strnlen;
extern int phy_set_rx_comp_new;
extern int scan_get_type;
extern int lv_obj_set_style_border_color;
extern int cnx_add_rc;
extern int xTaskCreateStaticPinnedToCore;
extern int r_ble_ll_conn_cth_flow_error_fn;
extern int esp_hw_stack_guard_get_pc;
extern int esp_netif_up;
extern int lv_timer_pause;
extern int r_ble_ll_hci_acl_rx;
extern int ieee80211_mt_key_set_mask;
extern int xTaskIncrementTick;
extern int lv_event_get_dsc;
extern int ble_hs_conn_find;
extern int phy_write_pbus_mem;
extern int crypto_ec_point_from_bin;
extern int lvgl_devices_detach;
extern int coex_schm_ble_mesh_traffic_bt_default_wifi_connecting;
extern int lv_calendar_set_month_shown;
extern int r_ble_ll_mem_msys_get_block_num;
extern int ppProcTxDone;
extern int spi_bus_lock_register_dev;
extern int esp_netif_dhcps_stop;
extern int sdmmc_send_app_cmd;
extern int r_put_le32;
extern int r_ble_ll_sync_transfer;
extern int http_header_clean;
extern int esp_sha512_driver_update;
extern int pm_handle_tbtt_interval;
extern int adc_calc_hw_calibration_code;
extern int audio_codec_get_volume;
extern int lv_dropdown_get_selected;
extern int _ZTVNSt7__cxx1110moneypunctIcLb1EEE;
extern int _ZZNKSt7collateIwE12do_transformEPKwS2_EN4_BufD2Ev;
extern int xTimerPendFunctionCallFromISR;
extern int npl_freertos_time_delay;
extern int panic_print_str;
extern int TRACKBALL_TYPE;
extern int tcp_pcb_lists;
extern int GROVE_TYPE;
extern int heap_caps_realloc_default;
extern int pwm_disable;
extern int ieee80211_setup_rates;
extern int ble_sm_sc_public_key_rx;
extern int ieee80211_hostap_attach;
extern int mbedtls_md_get_size;
extern int ble_ll_env_p;
extern int gdma_ahb_hal_enable_intr;
extern int ieee80211_crypto_aes_128_cmac_decrypt;
extern int app_scheduler_current_app_id;
extern int ESP_EFUSE_MAC_EXT;
extern int lv_draw_task_get_fill_dsc;
extern int lvgl_sliderbox_set_value;
extern int mbedtls_ssl_read;
extern int modem_clock_module_bits_get;
extern int lv_mem_deinit;
extern int pp_coex_tx_release;
extern int r_ble_ll_adv_aux_pdu_ext_hdr_make;
extern int power_supply_supports_property;
extern int _ZSt8to_charsPcS_fSt12chars_formati;
extern int ble_att_clt_tx_find_info;
extern int lodepng_decoder_settings_init;
extern int mbedtls_cipher_info_from_psa;
extern int spi_periph_signal;
extern int round;
extern int lv_obj_get_parent;
extern int r_ble_lll_mmgmt_assign_user_block;
extern int lwip_hook_ip6_input;
extern int lv_dropdown_set_selected_highlight;
extern int MEMORY_POLICY_DEFAULT;
extern int lwip_netconn_do_disconnect;
extern int g_adv_stack_envP;
extern int esp_io_expander_gpio_wrapper_reset_pin;
extern int lv_textarea_get_max_length;
extern int panic_abort;
extern int mbedtls_psa_mac_verify_finish;
extern int r_ble_ll_adv_sync_get_pdu_len;
extern int tzset;
extern int ble_att_clt_tx_read_group_type;
extern int npl_freertos_mempool_deinit;
extern int xQueueTakeMutexRecursive;
extern int lv_draw_finalize_task_creation;
extern int lv_color_white;
extern int g_lmac_cnt;
extern int ip4_output_if;
extern int app_esp32_module;
extern int pm_wake_done;
extern int esp_http_client_get_transport_type;
extern int ieee80211_set_sta_gtk_index;
extern int r_ble_ll_hci_set_scan_rsp_data;
extern int r_ble_ll_hci_deinit;
extern int coex_bt_release;
extern int conn_hci_sendChanMapUpdCompVsEvent;
extern int chm_init;
extern int esp_now_add_peer;
extern int r_ble_lll_conn_end;
extern int ble_hid_get_conn_handle;
extern int scan_get_apnum;
extern int ledc_set_fade_with_time;
extern int esp_clk_slowclk_cal_set;
extern int _ZSteqIhLj6EEbRKSt5arrayIT_XT0_EES4_;
extern int lv_style_set_arc_rounded;
extern int mbedtls_ssl_conf_authmode;
extern int sta_sa_query_process_timeout;
extern int crypto_ec_key_parse_pub;
extern int twtsetupcmd_str;
extern int ble_sm_sc_dhkey_check_rx;
extern int lv_bar_get_value;
extern int esp_efuse_utility_check_errors;
extern int ledc_stop;
extern int pm_offchan_ready_or_defer;
extern int tcp_enqueue_flags;
extern int r_ble_lll_conn_env_init_for_sm;
extern int sys_thread_new;
extern int hal_mac_set_addr;
extern int wpa_sta_cur_pmksa_matches_akm;
extern int ble_sm_sc_oob_data_check;
extern int mbedtls_ssl_prepare_handshake_record;
extern int xIsrStack;
extern int r_ble_ll_get_tx_pwr_compensation;
extern int pointer_set_mirror_y;
extern int spi_flash_chip_generic_page_program;
extern int lv_dropdown_set_selected;
extern int hal_he_set_ersu;
extern int lwip_netconn_do_close;
extern int usb_midi_is_connected;
extern int device_is_ready;
extern int netif_ip6_addr_set;
extern int lvgl_spinner_create;
extern int ledc_timer_pause;
extern int httpd_req_delete;
extern int ble_hs_hci_evt_acl_process;
extern int ieee80211_parse_wpa;
extern int coex_schm_bt_inq_wifi_scan;
extern int ieee80211_ampdu_enable;
extern int thread_get_state;
extern int lv_display_get_vertical_resolution;
extern int free_bss_info;
extern int esp_wifi_wpa_ptk_init_done_internal;
extern int mbedtls_rsa_rsaes_oaep_decrypt;
extern int r_ble_ll_conn_hci_rd_rssi;
extern int ppHEAMPDU2Normal;
extern int r_ble_ll_conn_hci_set_path_loss_en;
extern int r_os_mbuf_free_chain;
extern int hostap_deinit;
extern int ic_set_sleep_min_active_time;
extern int lv_display_enable_invalidation;
extern int esp_sha1_starts;
extern int npl_freertos_mutex_release;
extern int lvgl_toolbar_add_switch_action;
extern int wdev_process_tbtt;
extern int lv_event_set_ext_draw_size;
extern int ble_bb_cte_get_cte_samp_limit_ref;
extern int i2c_master_write;
extern int kernel_init;
extern int npl_freertos_time_ticks_to_ms32;
extern int _ZSt8to_charsPcS_e;
extern int tcp_backlog_accepted;
extern int wpa_cipher_valid_pairwise;
extern int ic_set_sta;
extern int he_twt_information_event_post;
extern int lv_bar_set_mode;
extern int mbedtls_md_update;
extern int _set;
extern int ble_hs_cfg;
extern int ble_store_delete;
extern int log1pf;
extern int ieee80211_close_all_twt_sessions;
extern int uart_is_driver_installed;
extern int xEventGroupSetBitsFromISR;
extern int inet_chksum;
extern int pbuf_cat;
extern int sdmmc_mmc_decode_cid;
extern int ptr_check_val;
extern int __adddf3;
extern int get_vfs_for_index;
extern int r_ble_ll_conn_update_safe_data_len;
extern int memspi_host_flush_cache;
extern int r_ble_ll_adv_pdu_make;
extern int lv_tabview_get_tab_active;
extern int he_twt_information_txcb;
extern int ledc_get_freq;
extern int ledc_set_pin;
extern int r_ble_ll_adv_can_chg_whitelist;
extern int r_ble_lll_get_npl_element_info;
extern int g_wpa3_hostap_auth_api_lock;
extern int r_ble_ll_hci_le_encrypt;
extern int esp_flash_chip_driver_initialized;
extern int coex_dbg_output;
extern int pxPortInitialiseStack;
extern int mbedtls_strerror;
extern int ic_set_rx_policy_ubssid_check;
extern int mbedtls_ssl_set_hostname;
extern int ff_diskio_get_pdrv_cnt_card;
extern int ff_disk_read;
extern int r_ble_lll_sched_delete_by_type;
extern int lv_obj_set_style_bg_image_recolor;
extern int phy_rx_filter_mode;
extern int lv_obj_set_style_text_color;
extern int he_preamble_su;
extern int r_hal_timer_read_tick;
extern int r_ble_lll_adv_stop;
extern int _ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESt6vectorIS7_SaIS7_EEEESD_PS7_ET1_T_T0_SF_;
extern int r_ble_ll_ctrl_le_pwr_change_ind_make;
extern int wlanif_init_sta;
extern int coex_schm_bt_idle_wifi_connecting;
extern int call_start_cpu0;
extern int ble_random_mode_pa_exec_process_cb_get;
extern int r_ble_ll_adv_put_syncinfo;
extern int ESP_EFUSE_ADC1_AVE_INITCODE_ATTEN0;
extern int lvgl_port_unlock;
extern int service_instance_get_state;
extern int esp_netif_create_default_wifi_sta;
extern int nan_dp_delete_peer;
extern int lv_malloc_zeroed;
extern int esp_ecdsa_transparent_verify_hash_abort;
extern int console_fstat;
extern int test_rx_parse_nonmumimo_complete_sigb;
extern int s_btwt_id_bitmap;
extern int __retarget_lock_init_recursive;
extern int lv_obj_remove_event_cb;
extern int extAdv_stack_deinitEnv;
extern int r_scan_duplicate_cache_refresh_cb;
extern int lv_menu_sidebar_header_cont_class;
extern int ble_hs_pvcy_set_mode;
extern int intr_handler_set;
extern int apm_hal_enable_ctrl_filter_all;
extern int acd_arp_reply;
extern int heap_caps_malloc_extmem_enable;
extern int coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_scan;
extern int ieee80211_amsdu_length_check;
extern int sntp_get_sync_status;
extern int mbedtls_aes_crypt_cbc;
extern int hal_disable_sta_beacon_filter;
extern int ble_hs_gap_svc_ctx;
extern int psa_custom_key_parameters_are_default;
extern int crypto_bignum_init_uint;
extern int phy_open_i2c_xpd;
extern int mbedtls_ssl_set_bio;
extern int lv_draw_buf_align_ex;
extern int pm_enable_twt_keep_alive_timer;
extern int mbedtls_rsa_validate_params;
extern int ble_l2cap_chan_free;
extern int r_ble_lll_conn_alloc_memory;
extern int netconn_shutdown;
extern int ieee80211_find_ext_ie;
extern int console_close;
extern int r_filter_duplicate_check_need_report;
extern int __atof_engine;
extern int lv_dropdown_set_options;
extern int hid_report_map_keyboard_consumer;
extern int ampdu_alloc_rx_ba_index;
extern int ethzero;
extern int r_filter_duplicate_set_ring_list_max_num;
extern int ieee80211_getcapinfo;
extern int r_ble_lll_adv_set_periodic_acad_chM;
extern int rtc_clk_cpu_freq_get_config;
extern int lv_image_create;
extern int phy_wifi_get_tx_tab_new;
extern int ieee80211_beacon_alloc;
extern int wifi_hw_stop;
extern int r_ble_ll_hci_init;
extern int mbedtls_mpi_exp_mod_soft;
extern int mbedtls_rsa_write_pubkey;
extern int pwr_hal_clear_mac_modem_rx_beacon_sleep_counter;
extern int ble_att_svr_tx_error_rsp;
extern int esp_wpa3_free_sae_data;
extern int lv_point_array_transform;
extern int psa_mac_sign_finish;
extern int g_mesh_topology;
extern int lv_dropdown_set_symbol;
extern int vRingbufferReturnItem;
extern int ic_update_trc_params;
extern int wifi_nvs_get_sta_listen_interval;
extern int chm_start_op;
extern int lv_area_move;
extern int lv_draw_dispatch_wait_for_request;
extern int esp_send_sae_auth_reply;
extern int mbedtls_psa_ecdsa_sign_hash;
extern int pbkdf2_sha1;
extern int PAU_instance;
extern int lv_draw_sw_mask_radius_init;
extern int _write_r;
extern int _ZTISt17bad_function_call;
extern int esp_time_impl_set_boot_time;
extern int pm_is_twt_awake;
extern int minitar_align_up_to_block_size;
extern int ble_gatts_indicate;
extern int r_ble_ll_ch_map_update_timer_cb;
extern int lv_font_get_glyph_bitmap;
extern int _Z11release_pinPP14GpioDescriptor;
extern int r_ble_lll_scan_abort_aux_sched;
extern int window_manager_await_state_change;
extern int xQueueGenericGetStaticBuffers;
extern int adiList_stack_disable;
extern int __lock___atexit_recursive_mutex;
extern int xTimerCreateStatic;
extern int esp32_sdspi_driver;
extern int ppCalTxHESMPDULength;
extern int esp_wifi_get_sta_key_internal;
extern int ESP_EFUSE_RD_DIS_BLOCK_KEY0;
extern int rv_core_critical_regs_frame;
extern int ds_hal_start_sign;
extern int lv_rb_remove_node;
extern int r_ble_lll_sync_stop;
extern int wpa_validate_wpa_ie;
extern int xTimerCreateTimerTask;
extern int r_ble_lll_conn_module_init;
extern int app_manager_find_manifest;
extern int r_ble_lll_adv_sec_chan_sch_cnt_get;
extern int crypto_ec_point_init;
extern int reachable_time;
extern int nvs_get_u16;
extern int lv_obj_scroll_to;
extern int __gettzinfo;
extern int mt_set_pmk;
extern int lv_tick_get;
extern int ic_set_interrupt_handler;
extern int abort_expect_void;
extern int udp_sendto_if;
extern int sys_timeout;
extern int esp_intr_free;
extern int mbedtls_ssl_parse_server_name_ext;
extern int ble_gap_unpair_oldest_except;
extern int __time_month;
extern int paths_get_user_data_path;
extern int _panic_handler;
extern int fgets;
extern int timer_alloc;
extern int mbedtls_asn1_write_mpi;
extern int phy_bb_bss_cbw40_dig;
extern int ppCalTxVHTSMPDULength;
extern int r_ble_lll_adv_ext_estimate_data_itvl;
extern int lv_draw_get_next_available_task;
extern int he_recv_action_event_bsscolor;
extern int i2s_check_set_mclk;
extern int properties_file_set;
extern int ble_hs_id_gen_rnd;
extern int _ZTVSt23_Sp_counted_ptr_inplaceIN2tt6PubSubINS0_7service9webserver14WebServerEventEEESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int sdmmc_io_send_op_cond;
extern int rv_utils_dbgr_is_attached;
extern int mbedtls_mpi_read_binary;
extern int lv_draw_buf_set_flag;
extern int lv_theme_simple_deinit;
extern int hal_he_set_bf_report_rate;
extern int itwt_information_timer;
extern int service_paths_get_user_data_path;
extern int extension_advfilter_module_env_p;
extern int atan;
extern int lv_timer_periodic_handler;
extern int lvgl_toolbar_create;
extern int esp_wifi_connect_internal;
extern int ieee80211_add_probe_req_ies;
extern int wpa_mic_len;
extern int g_beacon_eb;
extern int pm_local_tsf_process;
extern int mbedtls_psa_rsa_export_public_key;
extern int coex_schm_ble_mesh_traffic_wifi_scan;
extern int ff_disk_initialize;
extern int r_ble_ll_whitelist_deleted_irk_rmv;
extern int ieee80211_softap_add_uora_parameter;
extern int cfree;
extern int mpi_hal_set_search_position;
extern int panic_arch_fill_info;
extern int lv_chart_set_point_count;
extern int esp_ble_controller_info_capture;
extern int sha256_prf_bits;
extern int ble_hs_mbuf_pullup_base;
extern int esp_sha_dma;
extern int crypto_ec_init;
extern int device_get_by_name;
extern int ferror;
extern int lv_dropdown_get_dir;
extern int netconn_join_leave_group;
extern int mbedtls_ssl_verify_certificate;
extern int lv_arc_set_bg_end_angle;
extern int ble_spp_get_conn_handle;
extern int r_ble_ll_sync_lost_event;
extern int _ZSt11make_sharedIA_hESt10shared_ptrIT_Ej;
extern int nvs_erase_key;
extern int minitar_validate_header;
extern int lv_image_decoder_set_info_cb;
extern int lv_obj_allocate_spec_attr;
extern int r_ble_lll_scan_get_earliest_start_time;
extern int hal_mac_tsf_reset;
extern int esp_netif_down;
extern int r_hal_rtc_irq_handler;
extern int driver_construct_add;
extern int lv_spinbox_decrement;
extern int pm_tx_data_done_process;
extern int pmksa_cache_free_entry;
extern int esp_netif_init;
extern int mbedtls_mutex_lock_ptr;
extern int phy_rfcal_txiq;
extern int lv_buttonmatrix_get_button_text;
extern int coex_schm_bt_piscan_wifi_conn;
extern int lv_arc_get_min_value;
extern int rcGet11AXHighestRateIdx;
extern int lwip_write;
extern int heap_caps_get_free_size;
extern int isnan;
extern int esp_pbuf_allocate;
extern int phy_chan_freq_hw_init;
extern int iswupper;
extern int asinf;
extern int minmea_check;
extern int memspi_host_write_data_slicer;
extern int __kernel_sin;
extern int lv_text_encoded_size;
extern int adc_channel_read_raw;
extern int httpd_sess_set_descriptors;
extern int dhcp_start;
extern int phy_xtal_duty_cal;
extern int cnx_obss_scan_done_cb;
extern int uart_vfs_include_dev_init;
extern int esp_crypto_hmac_lock_release;
extern int ESP_EFUSE_RD_DIS_BLOCK_KEY4;
extern int esp_transport_list_add;
extern int __clzsi2;
extern int r_ble_lll_sched_env_deinit;
extern int r_ble_ll_hci_ev_le_csa;
extern int r_ble_lll_scan_npl_store;
extern int wDev_IndicateFrame;
extern int lora_can_transmit;
extern int ieee80211_ht_attach;
extern int __retarget_lock_try_acquire_recursive;
extern int vTaskSuspend;
extern int uECC_valid_point;
extern int mbedtls_ecp_muladd_restartable;
extern int device_destruct;
extern int udp_remove;
extern int phy_set_loopback_gain;
extern int sscanf;
extern int r_ble_ll_utils_verify_aa;
extern int unregister_ieee80211_action_vendor_get_key_cb;
extern int ble_sm_inject_io;
extern int sys_sem_free;
extern int mbedtls_free;
extern int esp_transport_set_errors;
extern int lv_ll_get_len;
extern int _Z15gps_ledger_syncv;
extern int ieee80211_parse_htcap;
extern int phy_chip_set_chan_ana;
extern int r_ble_ll_sync_est_event_success;
extern int lv_group_create;
extern int r_ble_ll_customize_peer_sca_get;
extern int pm_mac_disable_tsf_tbtt_modem_wakeup;
extern int esp_wifi_create_if_driver;
extern int esp_libc_include_memmove_impl;
extern int ic_trc_update_def_rate;
extern int ff_mutex_give;
extern int ble_sm_alg_f6;
extern int event_group_set;
extern int esp_clk_tree_xtal32k_get_freq_hz;
extern int ble_sm_alg_encrypt;
extern int mbedtls_ssl_handshake_wrapup_free_hs_transform;
extern int hal_sniffer_set_promis_misc_pkt;
extern int vPortAssertIfInISR;
extern int psa_get_key_attributes;
extern int lv_textarea_get_label;
extern int _regi2c_impl_read_mask;
extern int r_ble_ll_sync_send_truncated_per_adv_rpt;
extern int wDev_Set_Beacon_Int;
extern int r_ble_ll_conn_tx_pkt_in;
extern int lodepng_decompress_settings_init;
extern int r_ble_ll_adv_put_aux_ptr;
extern int esp_event_loop_delete_default;
extern int tcp_bound_pcbs;
extern int lv_cache_entry_init;
extern int hal_crypto_get_key_entry;
extern int aes_ctr_encrypt;
extern int sleep_retention_module_init;
extern int pp_unregister_tx_cb;
extern int i2s_hal_set_tx_clock;
extern int r_ble_ll_scan_adv_decode_addr;
extern int r_advertise_filter_env_deinit;
extern int clk_hal_xtal_get_freq_mhz;
extern int minitar_parse_metadata_from_tar_header;
extern int r_ble_lll_adv_reset_periodic_link_state;
extern int cal_ap_bandwith;
extern int ble_sm_sc_init;
extern int lv_bin_decoder_get_area;
extern int lv_obj_move_to_index;
extern int preferences_open;
extern int wifi_event_post;
extern int pm_coex_schm_overall_period_get;
extern int memp_TCPIP_MSG_INPKT;
extern int i2s_channel_reconfig_std_clock;
extern int esp_flash_write_encrypted;
extern int r_filter_duplicate_addr_ring_list_add;
extern int stl_symbols;
extern int scan_stack_initEnv;
extern int esp_gpio_is_reserved;
extern int lv_arc_get_bg_angle_end;
extern int r_ble_hw_cte_buffer_owner_clear;
extern int esp_crypto_sha_enable_periph_clk;
extern int esp_crypto_aes_gcm_decrypt;
extern int mbedtls_x509_get_rsassa_pss_params;
extern int ble_att_tx_with_conn;
extern int mbedtls_ssl_handle_pending_alert;
extern int ieee80211_send_proberesp;
extern int mbedtls_mpi_core_write_be;
extern int string_symbols;
extern int band2str;
extern int crypto_ecdh_deinit;
extern int XYcZ_add;
extern int httpd_sess_clear_ctx;
extern int esp_log;
extern int regi2c_saradc_disable;
extern int rtc_isr_noniram_disable;
extern int lv_obj_get_scroll_dir;
extern int lmacIsIdle;
extern int esp_mpi_enable_hardware_hw_op;
extern int esp_cpu_wait_for_intr;
extern int I2C_CONTROLLER_TYPE;
extern int phy_byte_to_word;
extern int wifi_menuconfig_init;
extern int lv_obj_is_layout_positioned;
extern int lv_point_from_precise;
extern int r_ble_ll_scan_deinit;
extern int __clzdi2;
extern int usb_msc_eject;
extern int esp_timer_impl_early_init;
extern int phy_txiq_cal_init;
extern int _ZTVSt23__codecvt_abstract_baseIDsDu10_mbstate_tE;
extern int esp_vfs_readdir;
extern int lv_tabview_get_tab_count;
extern int lv_timer_set_user_data;
extern int httpd_sess_close_lru;
extern int port_xSchedulerRunning;
extern int __unorddf2;
extern int os_get_random;
extern int hal_attenna_init;
extern int _lock_init_recursive;
extern int ble_att_clt_tx_prep_write;
extern int strncpy;
extern int lmacProcessCollisions;
extern int ds_hal_start;
extern int coex_schm_ble_mesh_config_bt_sniff_sco_wifi_scan;
extern int _ZTVSt12length_error;
extern int sys_arch_unprotect;
extern int spi_flash_set_erasing_flag;
extern int __wrap___cxa_allocate_exception;
extern int __mulPow5InvDivPow2;
extern int tsf_hal_set_tbtt_intr_disable;
extern int conn_stack_enableChanMapUpdCompVsEvent;
extern int spi_flash_chip_generic_yield;
extern int lmacAdjustTimestamp;
extern int lv_anim_set_delay;
extern int nan_ndc_start_process;
extern int trc_deinit;
extern int audio_stream_read;
extern int ble_sm_sc_public_key_exec;
extern int adv_hci_clearLegacyAdv;
extern int __cxa_guard_dummy;
extern int bundle_clone;
extern int npl_funcs;
extern int strndup;
extern int r_ble_phy_max_data_pdu_pyld;
extern int crypto_bignum_mulmod;
extern int display_init;
extern int coex_schm_ble_mesh_standby_wifi_conn;
extern int esp_log_cache_get_level;
extern int service_manager_remove;
extern int audio_stream_set_mute;
extern int ble_hs_hci_set_hci_supported_cmd;
extern int i2c_select_periph_clock;
extern int esp_crypto_aes_gcm_decrypt_setup;
extern int pm_disconnected_sleep_delay_timeout_process;
extern int lvgl_toolbar_set_nav_action;
extern int ic_set_he_rts_threshold_bytes_tab;
extern int tsf_hal_set_tbtt_modem_wakeup_disable;
extern int mbedtls_mpi_mul_mpi;
extern int _Z26app_metadata_is_valid_nameRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int mbedtls_aes_xts_setkey_dec;
extern int ant_rx_cfg;
extern int mbedtls_ecdsa_opaque_info;
extern int tcp_connect;
extern int cJSON_IsArray;
extern int esp_coex_common_clk_slowclk_cal_get_wrapper;
extern int wpa_sm_set_assoc_rsnxe;
extern int eloop_register_timeout;
extern int usb_host_hid_is_connected;
extern int r_ble_ll_ctrl_sca_req_rsp_make;
extern int spicommon_cs_initialize;
extern int ble_store_key_from_value_sec;
extern int get_max_conts_miss_beacon_sp_count;
extern int phy_tx_pwctrl_init_new;
extern int ethernet_input;
extern int lvgl_ppa_delete;
extern int coex_schm_bt_sniff_sco_wifi_connecting;
extern int lv_switch_class;
extern int mbedtls_psa_cipher_set_iv;
extern int esp_ble_controller_lib_check;
extern int lv_scale_class;
extern int ble_gattc_rx_find_info_idata;
extern int lv_ll_move_before;
extern int lv_anim_path_ease_out;
extern int assoc_ie_buf;
extern int dtm_hci_connlessIqEvtReport;
extern int hal_he_bssid_deinit;
extern int service_paths_get_user_data_directory;
extern int mbedtls_mpi_sub_abs;
extern int _Z34app_metadata_is_valid_version_codeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int _ZTVNSt7__cxx1110moneypunctIwLb0EEE;
extern int hal_get_time_to_sta_next_tbtt;
extern int esp_wifi_ap_notify_node_sae_auth_done;
extern int r_ble_ll_scan_hci_set_adv_report_flow_ctrl;
extern int coex_schm_unlock;
extern int r_ble_hw_encrypt_block;
extern int wDev_BeaconMemory_Init;
extern int mbedtls_ct_memcpy_offset;
extern int tcp_init;
extern int retrans_timer;
extern int ble_att_clt_tx_find_type_value;
extern int lv_label_is_char_under_pos;
extern int r_ble_ll_channel_assess;
extern int phy_is_low_rate_enabled;
extern int r_ble_ll_scan_dup_check_legacy;
extern int os_mempool_flags_set;
extern int g_exc_frames;
extern int ble_random_adv_env_p;
extern int r_filter_duplicate_data_base_deinit;
extern int hal_enable_tsf_timer;
extern int lv_draw_sw_blend_color_to_al88;
extern int xRingbufferGetCurFreeSize;
extern int ieee80211_parse_mbssid;
extern int _i2c_hal_init;
extern int r_ble_ll_resolv_env_init;
extern int lv_style_transition_dsc_init;
extern int lvgl_is_running;
extern int psa_validate_unstructured_key_bit_size;
extern int bluetooth_get_device_name;
extern int r_ble_ll_qa_enable;
extern int r_ble_controller_deinit;
extern int lv_textarea_get_password_bullet;
extern int r_ble_ll_resolv_set_rpa_tmo;
extern int esp_heap_adjust_alignment_to_hw;
extern int mbedtls_mpi_random;
extern int lv_dropdownlist_class;
extern int wifi_scan_restore_random_mac;
extern int r_ble_lll_sync_delete_and_resch;
extern int uart_controller_read_until;
extern int esp_clk_cpu_freq;
extern int npl_freertos_callout_deinit;
extern int r_ble_ll_sync_list_search;
extern int _ZTVN2tt7service11displayidle18MystifyScreensaverE;
extern int ble_mqueue_deinit;
extern int lv_indev_get_scroll_dir;
extern int esp_coex_common_semphr_create_wrapper;
extern int coex_schm_external_coex_wifi_default_rxonly;
extern int r_ble_lll_adv_sched_priority_increase;
extern int r_ble_ll_scan_refresh_nrpa;
extern int esp_psram_mspi_mb_init;
extern int tc_cmac_erase;
extern int _ZGVNSt7__cxx118messagesIcE2idE;
extern int gpio_controller_get_controller_context;
extern int lv_obj_get_click_area;
extern int lv_group_set_editing;
extern int ESP_EFUSE_WR_DIS_BLOCK_KEY1;
extern int hal_mac_ftm_get_t3;
extern int esp_phy_update_country_info;
extern int ip4_route_src;
extern int phy_get_rx_sig_pwr;
extern int _ZTVNSt7__cxx119money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEEE;
extern int lv_obj_get_self_height;
extern int lv_bar_get_mode;
extern int pmu_hp_system_digital_param_default;
extern int esp_event_handler_instance_unregister;
extern int i2c_master_transmit_receive;
extern int rcAttach;
extern int _Z9gps_probeP6Device;
extern int __kernel_tanf;
extern int strxfrm;
extern int service_manager_start;
extern int mbedtls_ssl_set_calc_verify_md;
extern int ble_sm_connection_broken;
extern int r_ble_lll_adv_data_cache;
extern int esp_now_get_version;
extern int pm_parse_beacon;
extern int ble_random_init_env_p;
extern int ble_gattc_disc_all_dscs;
extern int phy_set_chan_misc_new;
extern int r_ble_ll_adv_set_random_addr;
extern int vRingbufferReturnItemFromISR;
extern int spi_hal_user_start;
extern int r_hal_timer_set_exp_tick;
extern int mbedtls_ecp_check_privkey;
extern int __fixunsdfdi;
extern int bundle_opt_bool;
extern int __lock___arc4random_mutex;
extern int mbedtls_psa_aead_encrypt;
extern int lv_obj_get_transformed_area;
extern int expm1;
extern int __wtime_mon;
extern int esp_coex_common_int_disable_wrapper;
extern int esp_sha256_starts;
extern int _ZSt15__try_use_facetINSt7__cxx118numpunctIcEEEPKT_RKSt6locale;
extern int esp_wifi_connect;
extern int lv_style_set_outline_pad;
extern int pvalloc;
extern int esp_wifi_sta_connect_internal;
extern int btdm_broker_deinit;
extern int lv_obj_get_x;
extern int mbedtls_mpi_core_sub;
extern int memp_malloc;
extern int r_ble_lll_conn_pre_process;
extern int hal_he_set_mac_delay;
extern int spi_hal_fetch_result;
extern int phy_rxevm_reset_mem;
extern int dtm_hal_module_env_p;
extern int file_system_mount;
extern int lv_draw_sw_blend_image_to_i1;
extern int ieee80211_get_key;
extern int adc_load_hw_calibration_chan_compens;
extern int ftm_is_initiator_supported;
extern int tcp_err;
extern int r_hal_timer_init;
extern int g_rmac_scan_saved_mac;
extern int spi_flash_chip_generic_wait_idle;
extern int etharp_remove_static_entry;
extern int ic_ebuf_alloc;
extern int lv_obj_set_style_max_height;
extern int __retarget_lock_close;
extern int scalbnf;
extern int i2s_channel_reconfig_std_slot;
extern int lodepng_crc32;
extern int httpd_default_send;
extern int uart_hal_set_parity;
extern int spicommon_dma_chan_free;
extern int ieee80211_add_dh_param;
extern int _ZTVSt7collateIwE;
extern int esp_log_util_cvt_hex;
extern int f_mkfs;
extern int spi_hal_deinit;
extern int g_offchan_ctx;
extern int ble_l2cap_deinit;
extern int ic_set_sleep_wait_broadcast_data_time;
extern int __cxa_end_catch;
extern int sae_pk_set_password;
extern int _ZSt13__int_to_charIcyEiPT_T0_PKS0_St13_Ios_Fmtflagsb;
extern int platform_esp32_module;
extern int unregister_ieee80211_action_vendor_spec_cb;
extern int esp_partition_write;
extern int r_ble_ll_hci_le_read_local_features;
extern int mpi_hal_enable_search;
extern int r_ble_lll_scan_process_all_recv_pkt;
extern int esp_crypto_sha_aes_lock_acquire;
extern int lv_tabview_set_tab_bar_position;
extern int esp_mesh_get_running_active_duty_cycle;
extern int ble_gap_preempt;
extern int itwt_stop_process;
extern int sinhf;
extern int sdspi_host_do_transaction;
extern int cnx_rc_update_rssi;
extern int hal_mac_dump_rx_evm;
extern int vTaskGenericNotifyGiveFromISR;
extern int hal_mac_deinit;
extern int lv_style_set_shadow_color;
extern int adc_oneshot_hal_setup;
extern int mbedtls_ecp_mul;
extern int app_paths_get_assets_path;
extern int lv_obj_set_user_data;
extern int modem_clock_hal_enable_modem_common_fe_clock;
extern int ble_store_read_csfc;
extern int mbedtls_ssl_md_alg_from_hash;
extern int mbedtls_ssl_session_init;
extern int r_ble_ll_event_tx_pkt;
extern int r_ble_lll_scan_npl_reset;
extern int esp_transport_init_foundation_transport;
extern int memp_FRAG_PBUF;
extern int r_ble_ll_conn_update_new_chan_map;
extern int ppCalTxopDur;
extern int pm_tbtt_process;
extern int app_module;
extern int r_ble_ll_hci_ev_connless_iq_report;
extern int lv_dropdown_set_text;
extern int pcTaskGetName;
extern int wpa_config_done;
extern int _ZTVSt23_Sp_counted_ptr_inplaceISt6atomicIiESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int esp_lcd_panel_io_del;
extern int hid_appearance;
extern int wpa_sm_set_key;
extern int mld6_stop;
extern int ppProcessWaitingQueue;
extern int uart_read_bytes;
extern int display_disp_on_off;
extern int hci_transport_host_callback_register;
extern int lv_spinbox_step_prev;
extern int rtc_gpio_pullup_dis;
extern int r_ble_ll_mem_msys_deinit;
extern int hal_wdev_timer_set_target;
extern int atof;
extern int __cxa_guard_acquire;
extern int ecc_hal_write_mul_param;
extern int ble_mqueue_get;
extern int ble_gattc_timer;
extern int wifi_get_ap_list_process;
extern int xQueueCreateWithCaps;
extern int fseeko;
extern int r_ble_lll_adv_coex_dpc_calc_pti_update_itvl;
extern int __isinff;
extern int phy_wifi_track_tx_power_new;
extern int lvgl_toolbar_configure;
extern int r_ble_ll_conn_hci_set_tx_power_enable;
extern int __double_computeInvPow5;
extern int djb2_data;
extern int g_rmac_scan_saved;
extern int pwr_hal_set_modem_state_rxend_exit_code;
extern int esp_clk_init;
extern int crypt_prewarm;
extern int lv_label_set_text;
extern int esp_bt_controller_disable;
extern int ble_sm_sc_confirm_exec;
extern int r_ble_ll_sync_rx_pkt_in;
extern int lvgl_software_keyboard_get_last;
extern int pm_beacon_monitor_tbtt_timeout_process;
extern int pau_regdma_set_entry_link_addr;
extern int lv_tabview_class;
extern int r_ble_lll_conn_env_init;
extern int r_ble_lll_df_recycle_cte_buffer_with_check;
extern int phy_rf_cal_data_backup;
extern int sae_pk_buf_shift_left_19;
extern int lvgl_port_deinit;
extern int npl_freertos_time_ms_to_ticks32;
extern int gdma_get_alignment_constraints;
extern int lodepng_state_cleanup;
extern int mbedtls_ssl_write_finished;
extern int pcl_hci_setRssiThresh;
extern int r_filter_duplicate_hash_check_in_table;
extern int usb_device_controller_allocate_interfaces;
extern int npl_freertos_callout_init;
extern int tcp_recved;
extern int dhcps_set_option_info;
extern int lmacEndRetryAMPDUFail;
extern int crypt_generate_iv;
extern int aes_hal_setkey;
extern int hal_agreement_del_rx_ba;
extern int esp_aes_init;
extern int xTimerGenericCommand;
extern int wpa3_hostap_post_evt;
extern int g_twdt_isr;
extern int memp_pools;
extern int net80211_printf;
extern int r_ble_ll_scan_dup_update_legacy;
extern int sdmmc_mmc_switch;
extern int coex_schm_ble_default_bt_a2dp_wifi_connecting;
extern int hid_kb_input_handle;
extern int phy_pbus_force_mode;
extern int ppRemoveHTC;
extern int wifi_get_init_state;
extern int ic_ebuf_recycle_rx;
extern int chm_set_home_channel;
extern int adiList_stack_enable;
extern int pm_stop_twt;
extern int r_ble_ll_resolv_list_reset;
extern int r_ble_ll_sync_check_failed;
extern int uECC_make_key;
extern int phy_pbus_reg_store;
extern int lv_obj_set_style_y;
extern int module_construct_add_start;
extern int ip_chksum_pseudo;
extern int spi_hal_setup_device;
extern int r_ble_ll_conn_callout_env_init_for_sm;
extern int r_ble_lll_sync_deinit;
extern int i2s_platform_acquire_occupation;
extern int r_ble_ll_hci_scan_set_enable;
extern int inet_chksum_pbuf;
extern int lvgl_get_launcher_icon_font;
extern int lv_cache_entry_delete;
extern int lv_obj_readjust_scroll;
extern int mbedtls_ssl_sig_from_pk_alg;
extern int mbedtls_ssl_reset_in_pointers;
extern int lv_palette_lighten;
extern int r_ble_ll_scan_common_init;
extern int regdma_link_new_branch_wait;
extern int ieee80211_user_ie_init;
extern int cJSON_Delete;
extern int lv_area_set;
extern int r_ble_ll_init_rx_pkt_in;
extern int esp_netif_get_io_driver;
extern int _Z19getUrlFromCrashDataB5cxx11v;
extern int crypt_module;
extern int _ZThn8_NSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED1Ev;
extern int r_DB_data_node_create;
extern int sdspi_host_io_int_enable;
extern int r_ble_ll_is_valid_own_addr_type;
extern int ble_gattc_rx_mtu;
extern int r_put_le64;
extern int ble_gattc_disc_all_chrs;
extern int cache_hal_unfreeze;
extern int gmtime_r;
extern int lv_draw_line_dsc_init;
extern int pm_coex_recalculate_wifi_time_slice;
extern int r_ble_ll_adv_set_adv_data;
extern int sae_pk_get_be19;
extern int lv_obj_set_style_outline_width;
extern int r_DB_data_list_destroy;
extern int httpd_recv_with_opt;
extern int _fcntl_r;
extern int phy_wifi_get_tx_gain;
extern int dragonfly_min_pwe_loop_iter;
extern int wpa_config_bss;
extern int esp_log_impl_unlock;
extern int sdmmc_init_mmc_read_ext_csd;
extern int r_ble_phy_init_registers;
extern int rtcio_hal_set_direction;
extern int lv_display_set_driver_data;
extern int sae_set_group;
extern int spi_flash_chip_boya_get_caps;
extern int mbedtls_ssl_get_max_out_record_payload;
extern int ble_hs_conn_delete_chan;
extern int esp_vfs_fstat;
extern int lv_textarea_add_text;
extern int esp_bt_rtc_slow_clk_select;
extern int r_ble_lll_conn_handle_get_by_sch;
extern int esp_hmac_verify_finish_opaque;
extern int r_ble_ll_hci_link_ctrl_cmd_proc;
extern int pm_on_data_tx;
extern int psa_key_derivation_input_key;
extern int lv_ll_init;
extern int coex_schm_ble_mesh_traffic_bt_conn_wifi_scan;
extern int wpa_auth_sta_get_pmksa;
extern int pm_twt_process;
extern int lv_dropdown_is_open;
extern int esp_aes_dma_start;
extern int r_ble_ll_adv_read_txpwr;
extern int esp_http_client_get_username;
extern int hci_stack_enableSetVsEvtMaskVsCmd;
extern int mbedtls_asn1_get_bitstring;
extern int f_write;
extern int _ZTVSt13basic_filebufIcSt11char_traitsIcEE;
extern int wifi_wps_is_started;
extern int udp_sendto_if_src;
extern int esp_partition_next;
extern int ieee80211_mt_key_is_mask_zero;
extern int _Z15getRtcCrashDatav;
extern int coex_schm_ble_mesh_standby_bt_a2dp_wifi_scan;
extern int rcSetBarRate;
extern int wifi_send_mgmt_frame;
extern int esp_cache_err_get_cpuid;
extern int r_ble_ll_scan_hci_update_adv_report_flow_ctrl;
extern int _Z22ble_spp_start_internalP6Device;
extern int httpd_sess_new;
extern int r_ble_ll_adv_periodic_enable;
extern int r_ble_cte_bb_update_config;
extern int pwr_hal_select_wifimac_regdma_link;
extern int pmksa_cache_get;
extern int ieee80211_crypto_bip_encrypt;
extern int lodepng_chunk_append;
extern int esp_unregister_shutdown_handler;
extern int ets_isr_unmask;
extern int ff_diskio_register_raw_partition;
extern int mbedtls_pk_sign_ext;
extern int lv_line_create;
extern int esp_transport_tcp_init;
extern int _ZTVSt15basic_streambufIcSt11char_traitsIcEE;
extern int ble_sm_incr_peer_sign_counter;
extern int ESP_EFUSE_WR_DIS_BLOCK_KEY5;
extern int gdma_ahb_hal_append;
extern int mbedtls_psa_ecdsa_verify_hash;
extern int file_mutex_unlock;
extern int BLUETOOTH_TYPE;
extern int getle32;
extern int r_ble_ll_scan_rx_pkt_in;
extern int mbedtls_rsa_gen_key;
extern int ble_lll_df_env_p;
extern int scan_reset_default_app_params;
extern int ieee80211_vht_node_init;
extern int pm_attach;
extern int cnx_assoc_timeout;
extern int twt_update_inactive_time_secs;
extern int lv_obj_set_style_opa;
extern int start_internal;
extern int ledc_set_duty;
extern int r_os_mbuf_cmpm;
extern int r_ble_phy_ccm_encrypt_block;
extern int esp32_ble_midi_driver;
extern int phy_get_rc_dout;
extern int i2c_controller_write_read;
extern int SysTickIsrHandler;
extern int bt_agc_gain_offset;
extern int ble_uuid_to_any;
extern int tcp_ticks;
extern int wpa_set_passphrase;
extern int r_ble_ll_conn_calc_itvl_ticks;
extern int ieee80211_amsdu_encap_check;
extern int _ZTVSt9money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE;
extern int bt_bb_tx_cca_set;
extern int r_exception_list_ble_mesh_data_base_clear;
extern int psa_key_derivation_abort;
extern int s_wifi_nvs;
extern int vApplicationGetTimerTaskMemory;
extern int adc_oneshot_new_unit;
extern int sys_mbox_trypost;
extern int lv_obj_set_style_grid_row_dsc_array;
extern int regdma_find_next_module_link_head;
extern int r_get_le32;
extern int phy_rx_dco_cal_1step_new;
extern int _ZTVSt5ctypeIwE;
extern int pm_enable_twt_keep_alive;
extern int wDev_ProcessRxData_NAN_Interface_Hook;
extern int hal_set_tb_pti;
extern int esp_ptr_dma_ext_capable;
extern int httpd_resp_set_type;
extern int r_ble_hw_cte_buffer_software_own;
extern int lvgl_arch_start;
extern int __kernel_cosf;
extern int ledc_set_fade_time_and_start;
extern int spi_device_release_bus;
extern int test_tx_succ_statistics;
extern int ble_att_svr_rx_indicate;
extern int bt_agc_gain_set;
extern int r_os_mbuf_pullup;
extern int tcp_alloc;
extern int ic_add_rx_ba;
extern int mbedtls_x509_oid_get_md_alg;
extern int r_ble_lll_current_tx_info_get;
extern int r_ble_ll_ctrl_path_loss_mon;
extern int mbedtls_ssl_get_ciphersuite_sig_pk_psa_usage;
extern int _ZTVSt8messagesIwE;
extern int rx11NRate2AMPDULimit;
extern int lv_timer_core_deinit;
extern int coex_schm_ble_mesh_traffic_wifi_conn;
extern int ble_hs_hci_util_rand;
extern int gpio_pullup_en;
extern int multi_heap_free_size;
extern int ble_hs_adv_parse_free;
extern int esp_mpi_disable_hardware_hw_op;
extern int r_ble_lll_sleep_time_sync_set;
extern int qrcode_getModule;
extern int lv_obj_delete;
extern int dispatcher_alloc;
extern int esp_chip_info;
extern int wcscoll;
extern int gdma_register_tx_event_callbacks;
extern int hal_mac_color_get_bitmap;
extern int ppUnregisterTxCallback;
extern int esp_sha512_starts;
extern int modem_clock_module_enable;
extern int lv_image_header_cache_drop;
extern int mbedtls_ssl_write_handshake_msg_ext;
extern int thread_set_state_callback;
extern int timer_reset;
extern int r_ble_ll_sync_update_anchor_check_by_adva;
extern int wifi_sta_reg_rxcb;
extern int esp_wifi_get_macaddr_internal;
extern int coex_schm_status_bit_set;
extern int phy_freq_mem_change_5g_;
extern int _ZTVSt12system_error;
extern int spi_flash_hal_common_command;
extern int mspi_timing_change_speed_mode_cache_safe;
extern int multi_heap_minimum_free_size;
extern int ip_addr_broadcast;
extern int hal_get_tx_pwr;
extern int _ZGVNSt10moneypunctIcLb0EE2idE;
extern int ieee80211_set_phy_bw;
extern int _ZTVSt23_Sp_counted_ptr_inplaceIN2tt6PubSubINS0_7service5audio10AudioEventEEESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int sae_check_confirm_pk;
extern int netif_poll;
extern int phy_temp_to_power_new;
extern int r_ble_lll_sleep_disable;
extern int hal_utils_calc_clk_div_frac_accurate;
extern int gcmp;
extern int f_fdisk;
extern int get_act_lp_dbias;
extern int ieee80211_ht_node_init;
extern int pm_beacon_misc_process;
extern int timer_stop;
extern int lv_array_deinit;
extern int get_vfs_upper_bound;
extern int lv_cache_entry_dec_ref;
extern int lv_cache_add;
extern int hal_set_rx_ack_pti;
extern int mbedtls_pk_ecc_set_pubkey;
extern int wpa_ap_get_wpa_ie;
extern int unsetenv;
extern int psa_asymmetric_encrypt;
extern int phy_get_dreg_1p1;
extern int r_ble_lll_mgmmt_rxbuf_adjust_check;
extern int buf_shift_right;
extern int httpd_req_get_url_query_str;
extern int hal_he_clr_tid_bitmap;
extern int nimble_midi_api;
extern int ic_del_key_all;
extern int sdmmc_send_cmd_select_card;
extern int tlsf_block_size;
extern int ets_timer_arm;
extern int tcp_write;
extern int psa_unregister_read_under_mutex;
extern int lv_memcmp;
extern int memspi_host_erase_sector;
extern int r_ble_lll_conn_insert_txbuf_ahead;
extern int test_mumimo_get_heltf_num;
extern int cache_hal_vaddr_to_cache_level_id;
extern int ble_hs_conn_free;
extern int mbedtls_aes_free;
extern int tcp_listen_pcbs;
extern int esp_wifi_is_wpa3_compatible_mode_enabled;
extern int hal_get_sta_tsf;
extern int pm_enable_active_timer;
extern int pm_is_open;
extern int esp_partition_get;
extern int r_ble_ll_conn_sm_alloc;
extern int coex_schm_external_coex_wifi_scan;
extern int r_ble_ll_mem_generic_data_init;
extern int uxQueueGetQueueNumber;
extern int unregister_fd;
extern int wpa_auth_add_sae_pmkid;
extern int esp_transport_tcp_set_interface_name;
extern int lv_buttonmatrix_set_button_ctrl;
extern int WIFI_TYPE;
extern int regdma_find_link_by_id;
extern int coex_schm_ble_mesh_standby_bt_conn_wifi_conn;
extern int wpa_parse_wpa_ie_wpa;
extern int display_reset;
extern int coex_schm_ble_mesh_standby_wifi_connecting;
extern int lora_get_modulation;
extern int lv_draw_arc;
extern int esp_netif_set_mac;
extern int esp_netif_add_to_list_unsafe;
extern int psa_destroy_key;
extern int wpa_supplicant_transition_disable;
extern int pm_on_twt_force_tx;
extern int pm_force_scan_unlock;
extern int spi_flash_chip_list_check;
extern int spi_flash_hal_suspend;
extern int coex_schm_bt_inq_wifi_connecting;
extern int _ZTVNSt7__cxx118numpunctIcEE;
extern int cnx_csa_fn;
extern int r_ble_lll_mmgmt_alloc_link_state;
extern int spi_flash_disable_cache;
extern int hal_mac_enable_low_rate;
extern int pm_disconnected_start;
extern int r_ble_lll_conn_superversion_timer_cb;
extern int spi_flash_enable_cache;
extern int esp_wifi_cert_vht_mcs8;
extern int iswcntrl;
extern int ppAssembleAMPDU;
extern int ble_gap_unpair;
extern int esp_cmac_compute;
extern int esp_wifi_init_internal;
extern int GPIO_HOLD_MASK;
extern int npl_freertos_sem_release;
extern int lv_text_unicode_to_encoded;
extern int ble_att_svr_prev_handle;
extern int pm_sleep_delay_timeout_process;
extern int lmacProcessTxComplete;
extern int lvgl_get_text_font;
extern int bluetooth_hid_device_stop;
extern int __bufio_seek;
extern int __bufio_setvbuf;
extern int spi_flash_chip_winbond_read;
extern int pm_tx_data_process;
extern int s_ni;
extern int r_ble_ll_ctrl_phy_update_proc_complete;
extern int hal_init_imrsp_power;
extern int phy_txdc_cal;
extern int bt_agc_restart_set;
extern int r_ble_ll_adv_ext_check_data_itvl;
extern int i2s_controller_set_config;
extern int mbedtls_md_hmac_finish;
extern int xTaskAbortDelay;
extern int esp_unregister_npl_funcs;
extern int netif_loop_output;
extern int btwt_setup_timeout_fn;
extern int esp_vfs_lwip_sockets_register;
extern int spi_flash_chip_gd_get_caps;
extern int _ZTVSt15_Sp_counted_ptrIPN2tt7service11development18DevelopmentServiceELN9__gnu_cxx12_Lock_policyE1EE;
extern int esp_pthread_set_cfg;
extern int lv_array_push_back;
extern int phy_get_i2c_mst0_mask;
extern int sae_derive_pwe_from_pt_ecc;
extern int memp_init;
extern int esp_log_set_vprintf;
extern int regdma_link_update_next;
extern int ieee80211_get_robustmgtframe;
extern int esp_mmu_map_reserve_block_with_caps;
extern int r_ble_ll_conn_set_ext_con_params;
extern int mbedtls_aes_crypt_ctr;
extern int wifi_ap_reg_rxcb;
extern int ampdu_dispatch_movement;
extern int unlink;
extern int __retarget_lock_try_acquire;
extern int esp_lcd_panel_draw_bitmap;
extern int chm_set_snd_chan;
extern int lv_timer_handler_run_in_period;
extern int esp_mspi_pin_reserve;
extern int ic_get_G6M_sched;
extern int _on_exit;
extern int lmacSetAcParam;
extern int lv_obj_get_scroll_top;
extern int coex_schm_bt_conn_wifi_scan;
extern int efuse_hal_get_disable_wafer_version_major;
extern int xQueueReceiveFromISR;
extern int r_hal_timer_set_cb;
extern int lv_style_set_recolor;
extern int esp_hmac_verify_finish_transparent;
extern int _ZSt25__unguarded_linear_insertIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops14_Val_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SK_;
extern int lv_obj_class_init_obj;
extern int app_scheduler_stop;
extern int esp_netif_tcpip_exec;
extern int esp_timer_start_periodic;
extern int r_ble_lll_scan_update_configuration;
extern int fsetpos;
extern int logf;
extern int regdomain_table;
extern int r_ble_ll_ctrl_rx_pwr_ctrl_req;
extern int r_ble_ll_read_supp_states;
extern int wifi_create_sta;
extern int btdm_broker_attach;
extern int i2s_new_channel;
extern int ble_store_util_status_rr;
extern int ieee80211_pwrsave_txcb;
extern int spi_flash_chip_gd_get_io_mode;
extern int lodepng_info_cleanup;
extern int ble_hs_conn_init;
extern int omac1_aes_128;
extern int netconn_alloc;
extern int phy_get_rate_fcc_index;
extern int esp_fill_random;
extern int chm_acquire_lock;
extern int btdm_broker_publish;
extern int lv_theme_default_init;
extern int mbedtls_ssl_list_ciphersuites;
extern int ble_random_mode_adv_exec_process_cb_get;
extern int r_ble_ll_adv_remove;
extern int pm_coex_schm_process;
extern int tcp_rexmit_fast;
extern int esp_hmac_import_key_opaque;
extern int spi_device_polling_transmit;
extern int phy_freq_set_reg;
extern int modem_clock_hal_select_coex_lpclk_source;
extern int wifi_get_access_point_state;
extern int _ZSt8_DestroyIPN2tt3app4chat13StoredMessageEEvT_S5_;
extern int ble_hs_startup_go;
extern int esp_http_client_delete_all_headers;
extern int r_ble_ll_ctrl_rx_channel_reporting_ind;
extern int sinh;
extern int esp_lcd_panel_reset;
extern int __atod_engine;
extern int r_ble_ll_ext_conn_create;
extern int lv_event_get_count;
extern int mbedtls_ssl_transform_free;
extern int _ZTVN2tt7service6espnow13EspNowServiceE;
extern int raw_send;
extern int ppDirectRecycleAmpdu;
extern int phy_wakeup_init;
extern int esp_partition_find_first_err;
extern int esp_transport_poll_read;
extern int wifi_set_channel;
extern int igmp_start;
extern int coex_schm_ble_mesh_standby_bt_piscan_wifi_connecting;
extern int ieee80211_he_deattach;
extern int coex_schm_bt_a2dp_wifi_conn;
extern int sdmmc_init_sd_wait_data_ready;
extern int _ZTVSt14basic_ofstreamIcSt11char_traitsIcEE;
extern int _ZZNKSt7__cxx117collateIwE12do_transformEPKwS3_EN4_BufD1Ev;
extern int misc_nvs_init;
extern int lv_obj_scroll_by_bounded;
extern int clear_ic_key;
extern int pp_register_michael_mic_failure_cb;
extern int uECC_vli_cmp_unsafe;
extern int ble_transport_free;
extern int xTaskGetTickCount;
extern int lv_indev_set_cursor;
extern int r_ble_ll_conn_hci_le_ltk_reply;
extern int __wtime_weekday;
extern int coex_schm_ble_mesh_config_bt_sniff_sco_wifi_conn;
extern int esp_aes_gcm_auth_decrypt;
extern int lv_indev_get_key;
extern int mac_tx_set_mplen;
extern int ble_hs_mbuf_from_flat;
extern int lv_obj_get_layer_type;
extern int hal_mac_deinit_twt_tx;
extern int ble_l2cap_tx;
extern int __fpclassifyd;
extern int mbedtls_cipher_values_from_psa;
extern int r_ble_lll_adv_periodic_txbuf_make;
extern int ble_hs_flow_startup;
extern int lv_text_get_width_with_flags;
extern int __cxa_get_globals;
extern int esp_coex_common_timer_setfn_wrapper;
extern int wifi_set_channel_process;
extern int i2c_master_cmd_begin;
extern int power_supply_set_allowed_to_charge;
extern int mbedtls_pk_verify_restartable;
extern int esp_reset_reason_get_hint;
extern int esp_crypto_ds_lock_acquire;
extern int i2s_channel_init_pdm_rx_mode;
extern int wDev_ftm_record_t2t3;
extern int pm_allow_tx;
extern int ic_set_keep_alive_time;
extern int sdmmc_card_init;
extern int dragonfly_sqrt;
extern int s_cache_hal_init_ctx;
extern int psa_hash_update;
extern int _ZTVN10__cxxabiv117__class_type_infoE;
extern int sdmmc_init_mmc_bus_width;
extern int lv_cache_entry_set_invalid;
extern int gdma_ahb_hal_set_burst_size;
extern int nd6_cleanup_netif;
extern int esp_intr_ptr_in_isr_region;
extern int app_event_unsubscribe;
extern int ble_sm_lgcy_random_rx;
extern int advFilter_hci_setDuplicateParam;
extern int r_ble_ll_conn_power_ctrl_param_init;
extern int heap_caps_malloc;
extern int nus_chars_with_handle;
extern int ieee80211_regdomain_update_in_connect;
extern int _cnx_start_connect_without_scan;
extern int dhcps_new;
extern int esp_wifi_get_mac;
extern int pm_save_tbtt_info;
extern int phy_txdc_cal_pwdet_init;
extern int app_manager_install_path_add;
extern int coex_wifi_release;
extern int lv_style_set_bg_grad_color;
extern int wifi_reset_mac;
extern int ppAdd2AMPDUTail;
extern int wDev_ftm_set_t1t4;
extern int fflush;
extern int esp_log_cache_clean;
extern int pm_tbtt_adaptive_update_target;
extern int ieee80211_add_vhtopr;
extern int ble_gatts_bonding_established;
extern int spi_flash_encryption_hal_done;
extern int lv_obj_remove_event_cb_with_user_data;
extern int test_rx_parse_mumimo_complete_sigb;
extern int lv_anim_resolve_speed;
extern int r_ble_phy_set_rx_pwr_compensation;
extern int r_ble_ll_adv_ext_set_adv_data;
extern int esp_sleep_sub_mode_config;
extern int phy_rfcal_txcap;
extern int esp_efuse_utility_process;
extern int memp_UDP_PCB;
extern int r_ble_ll_resolv_peer_addr_rd;
extern int r_ble_ll_rand_start;
extern int pm_tx_null_data_done_process;
extern int supplicant_sta_disconn_handler;
extern int _ZZNSt8__format14__write_paddedINS_10_Sink_iterIcEEcEET_S3_St17basic_string_viewIT0_St11char_traitsIS5_EENS_6_AlignEjDiENKUljRS2_E_clEjSA_;
extern int modem_clock_hal_get_clock_domain_icg_bitmap;
extern int r_ble_lll_scan_chk_resume_in_task;
extern int scan_cancel;
extern int r_ble_lll_sync_reset_link_state;
extern int lv_area_intersect;
extern int r_ble_ll_sync_list_empty;
extern int r_ble_lll_recycle_sch_entry;
extern int esp_netif_dhcps_start;
extern int esp_io_expander_gpio_wrapper_set_pull_mode;
extern int esp_mspi_register_isr;
extern int r_ble_ll_conn_hci_wr_auth_pyld_tmo;
extern int gpio_descriptor_get_flags;
extern int r_ble_ll_hci_ev_send_adv_set_terminated;
extern int _ZTVSt16__numpunct_cacheIcE;
extern int app_paths_get_user_data_path;
extern int phy_get_tone_sar_dout;
extern int spi_flash_chip_generic_write;
extern int r_ble_lll_inform_task_to_recycle_sch;
extern int __shiftright128;
extern int wmemset;
extern int esp_cmac_setup;
extern int hal_he_set_htc;
extern int iswpunct;
extern int lv_event_get_target_obj;
extern int psa_key_derivation_output_bytes;
extern int esp_flash_chip_winbond;
extern int pm_is_twt_active_cnt_zero;
extern int multi_heap_free;
extern int phy_get_power_atten;
extern int r_ble_lll_adv_periodic_sched_priority_increase;
extern int r_ble_ll_adv_set_adv_params;
extern int r_ble_lll_mmgmt_free_rx_buffer_hdr_chain;
extern int esp_transport_ssl_set_keep_alive;
extern int wdev_mac_reg_store;
extern int mbedtls_rsa_rsaes_oaep_encrypt;
extern int r_ble_ll_adv_ext_estimate_data_itvl;
extern int phy_cfg_tx_magtrk;
extern int mbedtls_pem_free;
extern int forced_memzero;
extern int hal_he_get_rx_trigger_cnt;
extern int mbedtls_ecp_mul_restartable;
extern int _ZZNSt18__moneypunct_cacheIwLb1EE8_M_cacheERKSt6localeEN11_Scoped_strC1ERKSbIwSt11char_traitsIwESaIwEE;
extern int mbedtls_ssl_get_verify_result;
extern int lv_event_get_rotary_diff;
extern int hid_report_map_mouse_len;
extern int ble_hs_atomic_conn_flags;
extern int _system_r;
extern int window_manager_start;
extern int esp_psram_is_initialized;
extern int pm_coex_go_to_sleep;
extern int mbedtls_aes_xts_setkey_enc;
extern int lv_obj_invalidate;
extern int r_ble_ll_ctrl_phy_update_cancel;
extern int gpio_descriptor_get_pin_number;
extern int ble_sm_sc_random_rx;
extern int ble_store_config_persist_csfcs;
extern int lv_cache_entry_is_invalid;
extern int usb_msc_device_get;
extern int r_ble_lll_mmgmt_release_user_block;
extern int spicommon_bus_free;
extern int g_mesh_init_ps_type;
extern int floorf;
extern int ble_att_svr_rx_find_type_value;
extern int lv_draw_buf_adjust_stride;
extern int xQueueGetMutexHolderFromISR;
extern int g_phy_cap_rx_stbc;
extern int ble_store_util_bonded_peers;
extern int esp_aes_cipher_encrypt;
extern int aes_decrypt_deinit;
extern int wDev_AppendRxBlocks;
extern int r_ble_ll_resolv_init;
extern int spi_hal_set_data_pin_idle_level;
extern int vRingbufferDelete;
extern int vTaskPriorityDisinheritAfterTimeout;
extern int scan_flush_all_tx_buf;
extern int pm_extend_tbtt_adaptive_instance;
extern int phy_fe_txrx_reset;
extern int ieee80211_encap_amsdu;
extern int coex_schm_all_default;
extern int xTaskPriorityInherit;
extern int hal_coex_enable_default_pti;
extern int ff_mutex_delete;
extern int lv_point_to_precise;
extern int lv_obj_set_style_outline_opa;
extern int preferences_opt_bool;
extern int r_ble_lll_sleep_enable_now;
extern int esp_test_tx_addba_request;
extern int EccPoint_compute_public_key;
extern int ble_bb_cte_get_cte_samp_limit_samp;
extern int i2c_reset_rx_fifo;
extern int display_invert_color;
extern int lv_canvas_create;
extern int lv_obj_set_pos;
extern int mbedtls_ssl_free;
extern int r_ble_ll_adv_rx_pkt_in;
extern int mbedtls_ssl_session_free;
extern int r_ble_ll_conn_callout_env_deinit_for_sm;
extern int chanSel_stack_enableSetCsaVsCmd;
extern int rcSetTxAmpduLimit;
extern int pm_extend_tbtt_adaptive_servo;
extern int lv_font_get_bitmap_fmt_txt;
extern int crypto_ecdh_set_peerkey;
extern int r_ble_lll_mmgmt_block_cnt_check;
extern int get_sublen_offset;
extern int r_ble_log_raw_export;
extern int r_ble_lll_adv_coex_dpc_update;
extern int psa_cipher_encrypt;
extern int _ZTVN2tt7service11displayidle20StackChanScreensaverE;
extern int ieee80211_rfid_locp_recv_reset;
extern int _ZSt13__int_to_charIwmEiPT_T0_PKS0_St13_Ios_Fmtflagsb;
extern int wpa_sm_rx_eapol;
extern int lv_draw_sw_blend_color_to_argb8888;
extern int coex_schm_lock;
extern int get_vfs_for_fd;
extern int hal_he_set_uora_parameter;
extern int register_fd;
extern int zb_rx_auto_sel;
extern int _ZStneIwSt11char_traitsIwEEbRKSt19istreambuf_iteratorIT_T0_ES7_;
extern int r_ble_hci_trans_ll_evt_tx;
extern int r_ble_ll_conn_hci_rd_rem_ver_cmd;
extern int r_ble_ll_conn_rollback_last_unmapped_chan;
extern int r_ble_lll_scan_restart;
extern int usb_device_controller_get;
extern int pmu_hal_hp_set_modem_active_backup_disable;
extern int _ZTSN9__gnu_cxx24__concurrence_lock_errorE;
extern int get_total_scan_time;
extern int hmac_hal_read_result_256;
extern int mbedtls_cipher_cmac_finish;
extern int esp_log_write;
extern int esp_test_tx_enab_statistics;
extern int uxTimerGetTimerNumber;
extern int lv_span_stack_deinit;
extern int he_send_action_twt_teardown;
extern int esp_elf_relocate;
extern int nimble_port_get_dflt_eventq;
extern int f_utime;
extern int rtc_gpio_iomux_output;
extern int esp_lcd_panel_mirror;
extern int nan_warmup_timeout_process;
extern int modf;
extern int cosf;
extern int ieee80211_assoc_resp_construct;
extern int _ZSteqIcSt11char_traitsIcESaIcEEbRKNSt7__cxx1112basic_stringIT_T0_T1_EESA_;
extern int r_ble_ll_hci_npl_init;
extern int phy_filter_dcap_set;
extern int mbedtls_asn1_get_mpi;
extern int cnx_node_alloc;
extern int lv_obj_set_style_grid_cell_column_pos;
extern int os_reltime_initialized;
extern int lv_draw_sw_get_blend_handler;
extern int ppDequeueRxq_Locked;
extern int _ZGVNSt8messagesIcE2idE;
extern int esp_elf_init;
extern int esp_partition_mmap;
extern int mspi_timing_enter_low_speed_early;
extern int esp_rsa_ds_opaque_signature_sign_hash;
extern int ble_hs_pvcy_ensure_started;
extern int esp_aes_gcm_finish;
extern int esp_int_wdt_init;
extern int environ;
extern int lv_timer_get_next;
extern int sdspi_crc16;
extern int ble_bb_cte_set_cte_ant_delay_tx_aod_2us;
extern int phy_enable_agc;
extern int lv_layer_init;
extern int qsort;
extern int hal_he_set_hw_qos_null_ra_to_trans;
extern int lv_obj_get_y_aligned;
extern int hal_cca_set_snd20_cfg;
extern int lv_style_set_width;
extern int app_get_install_path;
extern int ble_freertos_ev_pool;
extern int ff_wtoupper;
extern int raw_remove;
extern int gps_module;
extern int task_event_group_wait;
extern int tcp_free;
extern int pthread_include_pthread_local_storage_impl;
extern int _Z19ble_get_scan_activeP6Device;
extern int phy_txcap_comp;
extern int http_header_delete;
extern int wep;
extern int ble_hs_conn_timer;
extern int wifi_nvs_cfg_item_init;
extern int pbuf_copy;
extern int mbedtls_pk_can_do;
extern int r_ble_ll_resolv_whether_reset_rpa_timer;
extern int r_ble_phy_env_deinit;
extern int r_ble_lll_adv_pri_chan_txbuf_alloc_and_make;
extern int _ZSt6formatIJNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEES5_St19basic_format_stringIcJDpNSt13type_identityIT_E4typeEEEDpOS8_;
extern int pm_mac_wakeup;
extern int ppClearTxq;
extern int mbedtls_rsa_deduce_private_exponent;
extern int _Z11ble_get_ctxP6Device;
extern int device_put;
extern int _ZTVN2tt10ScopedLockE;
extern int pmu_sleep_enable_regdma_backup;
extern int wifi_he_get_hetb_tid_bawin;
extern int lv_style_set_text_line_space;
extern int _ZTINSt8ios_base7failureB5cxx11E;
extern int soc_memory_regions;
extern int wDevCtrl;
extern int r_ble_ll_conn_find_active_conn;
extern int rtc_clk_xtal_freq_get;
extern int r_ble_ll_ctrl_rx_reject_ind;
extern int rcP2P11GSchedTbl;
extern int i2c_hal_master_handle_rx_event;
extern int r_ble_ll_scan_send_adv_report;
extern int sys_mutex_unlock;
extern int ieee80211_ftm_detach;
extern int ieee80211_timer_process;
extern int r_ble_ll_ctrl_le_power_req_make;
extern int coex_schm_ble_default_bt_default_wifi_scan;
extern int wpabuf_resize;
extern int _ZTVN2tt7service11displayidle21MatrixRainScreensaverE;
extern int lv_dropdown_get_option_count;
extern int ieee80211_add_assoc_req_ies;
extern int lv_tabview_get_content;
extern int mbedtls_pk_get_psa_attributes;
extern int lv_style_set_border_width;
extern int hal_coex_pti_init;
extern int r_ext_funcs_p;
extern int g_cte_ant_switch_delay_2m;
extern int esp_timer_impl_get_time;
extern int hal_agreement_clr_extra_softap_rx_ba;
extern int phy_pbus_workmode;
extern int tsf_hal_set_tsf_time_deviation;
extern int aes_gmac;
extern int r_ble_ll_adv_periodic_send_sync_ind;
extern int pointer_read_data;
extern int scan_enter_oper_channel_handler;
extern int ff_mutex_take;
extern int r_ble_lll_mmgmt_set_rxbuf_cnt_in_need;
extern int hci_stack_enable;
extern int sta_is_wifi_ent_enabled;
extern int ieee80211_merge_mbssid_profile;
extern int pm_noise_check_enable;
extern int tkip_decap;
extern int coex_schm_ble_mesh_standby_bt_piscan_wifi_conn;
extern int r_ble_lll_adv_update_data_if_needed;
extern int lv_theme_get_font_large;
extern int lv_display_flush_ready;
extern int igmp_joingroup_netif;
extern int lwip_netconn_is_err_msg;
extern int esp_phy_modem_init;
extern int httpd_sess_get_free;
extern int _Z16gps_ledger_clearv;
extern int lv_buttonmatrix_set_one_checked;
extern int lvgl_arch_stop;
extern int qrcode_initBytes;
extern int ieee80211_is_bufferable_mmpdu;
extern int nvs_get_i32;
extern int raw_netif_ip_addr_changed;
extern int etharp_add_static_entry;
extern int lodepng_color_mode_init;
extern int sdmmc_io_enable_hs_mode;
extern int lv_image_decoder_get_info;
extern int vPortExitCritical;
extern int lv_color_format_get_bpp;
extern int psa_aead_encrypt;
extern int ble_att_init;
extern int sae_derive_pwe_from_pt_ffc;
extern int spi_flash_hal_supports_direct_read;
extern int lv_label_get_letter_pos;
extern int ll_task_h;
extern int phy_fe_adc_on;
extern int lv_style_set_text_align;
extern int ecc_hal_set_mode;
extern int esp_clk_utils_mspi_speed_mode_sync_before_cpu_freq_switching;
extern int lv_ll_ins_tail;
extern int g_phyFuns;
extern int r_ble_lll_convert_timeline_num_to_conn_handle;
extern int r_ble_ll_conn_hci_set_path_loss_report_param;
extern int mbedtls_md_starts;
extern int owe_process_assoc_req;
extern int start_cpu0;
extern int ieee80211_add_htinfo;
extern int r_ble_lll_mmgmt_env_deinit;
extern int esp_hmac_finish_transparent;
extern int lv_anim_set_repeat_count;
extern int wpa_write_rsnxe;
extern int ble_hs_atomic_conn_delete;
extern int wcrtomb;
extern int ldexp;
extern int lv_style_set_anim_duration;
extern int _ZGVNSt7__cxx1110moneypunctIwLb0EE2idE;
extern int crypto_ec_key_deinit;
extern int sync_stack_enable;
extern int crypto_ec_get_a;
extern int lvgl_pointer_remove;
extern int rtc_io_num_map;
extern int _ZSt9use_facetISt11__timepunctIwEERKT_RKSt6locale;
extern int __wrap___gxx_personality_v0;
extern int preferences_has_int32;
extern int r_get_max_skip;
extern int netif_set_link_down;
extern int test_parse_rx_mu_nonmimo;
extern int esf_buf_setup;
extern int npl_freertos_sem_init;
extern int r_swap_in_place;
extern int esp_wifi_set_sleep_min_active_time;
extern int os_memblock_put;
extern int lv_chart_add_series;
extern int lv_layout_apply;
extern int esp_ptr_executable;
extern int r_ble_lll_mmgmt_rxbuf_empty_check;
extern int lv_memcpy;
extern int mbedtls_ecp_check_pubkey_soft;
extern int bluetooth_find_first_ready_device;
extern int wifi_get_station_state;
extern int ccmp_decap;
extern int wpa_write_rsn_ie;
extern int g_wifi_nvs;
extern int lv_map;
extern int dhcps_pbuf_alloc;
extern int mbedtls_ccm_encrypt_and_tag;
extern int haptic_select_library;
extern int _ZSt11make_sharedIN2tt6PubSubINS0_7service5audio10AudioEventEEEJEESt10shared_ptrIT_EDpOT0_;
extern int timer_is_running;
extern int netconn_recv_tcp_pbuf_flags;
extern int r_ble_lll_sleep_timer_exp;
extern int _ZSt4swapI6direntENSt9enable_ifIXsrSt6__and_IJSt6__not_ISt15__is_tuple_likeIT_EESt21is_move_constructibleIS5_ESt18is_move_assignableIS5_EEE5valueEvE4typeERS5_SF_;
extern int esf_buf_alloc_default_handler;
extern int r_ble_lll_conn_cth_flow_alloc_credit;
extern int coex_schm_ble_mesh_traffic_bt_piscan_wifi_conn;
extern int usb_msc_device_is_connected;
extern int esp_netif_sntp_init;
extern int lv_theme_default_is_inited;
extern int aes_decrypt_init;
extern int lv_chart_set_axis_range;
extern int sdmmc_check_host_function_ptr_integrity;
extern int igmp_init;
extern int esp_hmac_update_opaque;
extern int r_ble_ll_conn_calc_dci;
extern int window_manager_create;
extern int r_ble_ll_adv_enabled;
extern int ampdu_dispatch_as_many_as_possible;
extern int if_ctrl;
extern int ble_midi_ensure_keepalive;
extern int BLUETOOTH_SERIAL_TYPE;
extern int lv_textarea_get_one_line;
extern int hostapd_get_psk;
extern int usb_midi_device_start;
extern int f_truncate;
extern int ic_delete_wifi_task;
extern int xQueueCreateMutexStatic;
extern int mac_tx_set_pti;
extern int r_hal_timer_deinit;
extern int g_wdev_is_nan_pkt_in_valid_slot_cb;
extern int __wrap_gpio_get_level;
extern int charge_pump_init;
extern int r_ble_log_reset;
extern int g_ble_ll_ctrl_pkt_lengths_ro;
extern int lv_style_set_border_color;
extern int _ZSt24__throw_invalid_argumentPKc;
extern int r_ble_lll_adv_sched_first_pri_event;
extern int pm_update_by_connectionless_status;
extern int display_has_capability;
extern int hal_set_rx_beacon_pti;
extern int mbedtls_mpi_core_sub_int;
extern int lv_textarea_delete_char_forward;
extern int ppClearAlldataTxq_ifx;
extern int multi_heap_get_allocated_size;
extern int r_ble_ll_resolv_clear_all_wl_bit;
extern int lv_obj_get_state;
extern int r_ble_ll_scan_sm_stop;
extern int nan_dw_end_process;
extern int lv_display_get_horizontal_resolution;
extern int chm_get_home_channel;
extern int base_stack_deinitEnv;
extern int rcUpdateTxDoneAmpdu2;
extern int ieee80211_set_appie;
extern int lvgl_toolbar_set_title;
extern int lvgl_pointer_set_calibration;
extern int mbedtls_pem_read_buffer;
extern int phy_set_pbus_mem;
extern int lv_draw_buf_dup_ex;
extern int psa_import_key_into_slot;
extern int crypto_ec_point_is_on_curve;
extern int lv_timer_get_idle;
extern int port_uxCoreStartupDone;
extern int r_ble_ll_phy_to_phy_mode;
extern int sta_bip_check;
extern int r_ble_hw_periodiclist_clear;
extern int iswdigit;
extern int _ZStneIcSt11char_traitsIcEEbRKSt19istreambuf_iteratorIT_T0_ES7_;
extern int bt_agc_recorrect_set;
extern int phy_txbbgain_to_index;
extern int os_mempool_init_internal;
extern int rom_flash_chip_dummy_hpm;
extern int spi_flash_chip_gd_detect_size;
extern int ieee80211_regdomain_policy;
extern int ieee80211_encap_keep_alive_data;
extern int tc_cmac_init;
extern int gpio_input_enable;
extern int lv_text_encoded_letter_next_2;
extern int _ZZNSt18__moneypunct_cacheIwLb0EE8_M_cacheERKSt6localeEN11_Scoped_strC1ERKSbIwSt11char_traitsIwESaIwEE;
extern int r_ble_ll_conn_cth_flow_is_enabled;
extern int mbedtls_mpi_core_uint_le_mpi;
extern int lv_chart_set_type;
extern int esp_netif_next_unsafe;
extern int r_ble_lll_df_ring_buffer_handler_done;
extern int wpa_attach;
extern int HAPTIC_TYPE;
extern int esp_wifi_sta_is_ap_notify_completed_rsne_internal;
extern int ieee80211_regdomain_check_channel;
extern int r_ble_ll_ctrl_phy_update_ind_make;
extern int mbedtls_mpi_core_get_mont_r2_unsafe;
extern int tcp_timer_needed;
extern int lv_image_decoder_deinit;
extern int r_ble_ll_adv_active_chanset_clear;
extern int wdev_process_mac_modem_beacon_miss;
extern int lv_draw_arc_get_area;
extern int app_paths_get_assets_directory;
extern int lv_text_get_encoded_length;
extern int ESP_EFUSE_KEY3;
extern int wifi_log;
extern int sdmmc_check_scr;
extern int usb_hid_device_send_keyboard;
extern int r_ble_lll_adv_sched_aux;
extern int r_esp_ble_msys_deinit;
extern int regdma_find_prev_module_link_tail;
extern int fmax;
extern int ieee80211_ifattach;
extern int tbtt_adaptive_data_alloc;
extern int esp_gpio_revoke;
extern int r_ble_ll_scan_classify_filter_init;
extern int esp_wifi_sta_prof_is_rsn_internal;
extern int mbedtls_ssl_check_timer;
extern int _gettimeofday_r;
extern int npl_freertos_set_controller_npl_info;
extern int sdspi_host_remove_device;
extern int nan_dp_get_own_ndi;
extern int lv_obj_get_scroll_bottom;
extern int tcp_backlog_delayed;
extern int spi_flash_mmap;
extern int lv_rb_destroy;
extern int http_utils_assign_string;
extern int coex_hw_timer_freq_set;
extern int extAdv_stack_initEnv;
extern int bt_bb_rx_filter_sel;
extern int vApplicationStackOverflowHook;
extern int r_ble_ll_arr_env_deinit;
extern int mbedtls_ssl_handle_message_type;
extern int lvgl_lock;
extern int f_read;
extern int mbedtls_pk_import_into_psa;
extern int phy_noise_floor_auto_set;
extern int phy_bt_track_tx_power_new;
extern int send_wake_null_timeout;
extern int ieee80211_vht_deattach;
extern int heap_caps_aligned_alloc_base;
extern int lora_remove_state_callback;
extern int r_ble_hw_resolv_list_set;
extern int etharp_tmr;
extern int scan_set_current_scan_times;
extern int coex_schm_ble_mesh_config_bt_piscan_wifi_scan;
extern int ieee80211_update_phy_country;
extern int pp_sig_cnt;
extern int r_ble_lll_adv_start;
extern int bzero;
extern int wDev_SetCurChannel;
extern int xRingbufferSendFromISR;
extern int coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_connecting;
extern int igmp_stop;
extern int g_ble_sca_ppm_tbl_ro;
extern int r_ble_lll_aux_scan_drop;
extern int io_mux_force_disable_lp_io_clock;
extern int r_ble_ll_callout_env_deinit;
extern int lv_obj_set_style_border_post;
extern int phy_dac_scale_set;
extern int gdma_ahb_hal_get_eof_desc_addr;
extern int wdev_bank_store;
extern int hal_set_rx_beacon_time;
extern int lv_style_remove_prop;
extern int r_ble_ll_support_features_init;
extern int lvgl_software_keyboard_activate;
extern int r_ble_ll_hw_err_timer_cb;
extern int translate_path;
extern int _ZTVSt8bad_cast;
extern int lv_obj_set_scroll_snap_x;
extern int mbedtls_ecp_copy;
extern int _ZTVSi;
extern int lv_anim_speed_clamped;
extern int crypto_bignum_to_bin;
extern int coex_schm_ble_mesh_standby_bt_piscan_wifi_scan;
extern int sys_mutex_new;
extern int crypto_ec_key_group;
extern int sdmmc_mmc_send_ext_csd_data;
extern int hid_mouse_input_handle;
extern int hal_vht_sta_set_aid;
extern int esp_intr_disable_source;
extern int esp_transport_close;
extern int ieee80211_send_action_register;
extern int spicommon_dma_rx_mb;
extern int modem_clock_hal_deselect_all_wifi_lpclk_source;
extern int audio_stream_get_volume;
extern int esp_log_is_tag_loggable;
extern int lvgl_software_keyboard_show;
extern int gps_get_model_name;
extern int ble_att_clt_rx_find_info;
extern int lv_ll_get_next;
extern int esp_wifi_deinit;
extern int phy_pbus_rx_dco_cal;
extern int ppCheckTxQIdle;
extern int he_preamble_ersu;
extern int r_ble_ll_resolv_set_peer_rpa;
extern int wDev_ProcessRxSucData;
extern int dhcp_cleanup;
extern int ble_gap_adv_set_fields;
extern int r_ble_ll_mem_free;
extern int r_ble_lll_df_get_max_cte_sample_times_num;
extern int ieee80211_recv_action_vendor_esp_now;
extern int esp_ds_start_sign;
extern int fmod;
extern int r_ble_ll_scan_classify_filter_aux_init;
extern int r_ble_lll_mmgmt_update_interval_tick;
extern int _g_esp_netif_inherent_sta_config;
extern int pm_enable_sleep_delay_timer;
extern int periph_rcc_enter;
extern int GetAccess;
extern int wifi_destroy_softap;
extern int mbedtls_eckeydh_info;
extern int ble_att_clt_tx_write_req;
extern int esp_log_set_default_level;
extern int lv_draw_sw_blend_image_to_rgb565;
extern int itwt_probe_timer;
extern int lv_timer_core_init;
extern int wDev_Crypto_Disable;
extern int pm_register_pm_tx_null_cb;
extern int system_event_callback_remove;
extern int r_os_msys_get;
extern int _ZSt9use_facetISt10moneypunctIcLb1EEERKT_RKSt6locale;
extern int iswalnum;
extern int lv_arc_get_value;
extern int lv_textarea_set_text;
extern int hal_he_set_packet_padding_us;
extern int lv_area_is_out;
extern int mbedtls_pkcs5_pbkdf2_hmac_ext;
extern int ESP_EFUSE_ADC1_CH3_ATTEN0_INITCODE_DIFF;
extern int coex_schm_ble_mesh_traffic_bt_default_wifi_scan;
extern int hmac_hal_write_one_block_512;
extern int BLUETOOTH_HID_DEVICE_TYPE;
extern int r_ble_phy_mode_to_rate;
extern int camera_get_frame;
extern int lvgl_get_launcher_icon_font_height;
extern int r_ble_lll_scan_duration_timer_cb;
extern int adc_lock_try_acquire;
extern int r_ble_lll_adv_get_data_len;
extern int ieee80211_process_btwt_setup_action;
extern int _ZTVSt23_Sp_counted_ptr_inplaceIN2tt3app13fileselection5StateESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int ds_hal_write_message;
extern int hal_mac_set_rxbuf_reload_use_hw_beacon_enable;
extern int pointer_enter_sleep;
extern int _Z34app_metadata_is_valid_version_nameRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int esp_transport_init;
extern int cplusplus_symbols;
extern int pm_beacon_monitor_tbtt_start;
extern int raw_bind;
extern int _ZTSSt17bad_function_call;
extern int lwip_netconn_do_join_leave_group;
extern int httpd_sess_trigger_close_;
extern int i2s_hal_std_set_tx_slot;
extern int ic_stop_sw_txq;
extern int esp_nimble_disable;
extern int netif_add_ext_callback;
extern int thread_set_name;
extern int wifi_sta_reg_eapol_txdone_cb;
extern int vli_mmod_fast_secp256r1;
extern int gpio_sleep_sel_dis;
extern int esp_sha_acquire_hardware;
extern int esp_coex_init_include_func;
extern int ble_sm_ltk_req_rx;
extern int __lock___sinit_recursive_mutex;
extern int phy_bbpll_cal;
extern int mbedtls_psa_ffdh_export_public_key;
extern int mbedtls_ecdsa_info;
extern int _ZTv0_n12_NSdD1Ev;
extern int ble_hs_conn_exists;
extern int g_sync_stack_envP;
extern int r_ble_lll_adv_scan_rsp_legacy_pri_chan_pdu_make;
extern int mbedtls_cipher_setup;
extern int r_ble_lll_conn_reset_sm;
extern int ESP_EFUSE_WR_DIS_KEY_PURPOSE_3;
extern int esp_nimble_init;
extern int mbedtls_cipher_setkey;
extern int s_wifi_api_lock;
extern int mbedtls_ssl_handshake_client_step;
extern int gpio_isr_register;
extern int ledc_set_freq;
extern int wpabuf_alloc;
extern int lv_obj_set_state;
extern int ieee80211_espnow_get_init_flag;
extern int ble_att_clt_rx_error;
extern int xTaskGenericNotifyFromISR;
extern int device_is_added;
extern int mbedtls_pk_parse_subpubkey;
extern int ieee80211_set_shortslottime;
extern int rcClearCurAMPDUSched;
extern int ieee80211_timer_do_process;
extern int sdmmc_do_timing_tuning;
extern int r_ble_ll_hci_le_cmd_proc;
extern int ht_set_snd_ch_cfg;
extern int r_ext_bb_funcs_p;
extern int r_ble_ll_conn_hci_param_nrr;
extern int xQueueCreateSet;
extern int esp_flash_default_chip;
extern int ampduTidState2str;
extern int adc_oneshot_config_channel;
extern int wifi_remove_event_callback;
extern int log_stack_enableLogsRelatedVsCmd;
extern int pp_default_event_handler;
extern int coex_schm_ble_default_bt_idle_wifi_conn;
extern int spi_bus_free;
extern int r_ble_controller_disable;
extern int r_ble_ll_ctrl_phy_from_phy_mask;
extern int r_ble_lll_mmgmt_alloc_tx_buffer;
extern int pm_disable_sleep_delay_timer;
extern int r_ble_ll_sync_list_clear;
extern int esp_netif_destroy;
extern int __time_am_pm;
extern int wDev_SnifferRxData;
extern int r_ble_ll_ctrl_rx_periodic_sync_ind;
extern int ble_hid_init_gatt;
extern int r_ble_lll_scan_callout_env_init;
extern int audio_stream_get_mute;
extern int npl_freertos_hw_is_in_critical;
extern int key_mgr_hal_is_key_deployment_valid;
extern int r_DB_data_list_add;
extern int ftm_initiator_cleanup;
extern int crypto_ec_key_verify_signature;
extern int psa_mac_verify_setup;
extern int regi2c_ctrl_read_reg_mask;
extern int esp_wifi_unregister_wpa_cb_internal;
extern int mbedtls_mpi_core_read_be;
extern int _global_interrupt_handler;
extern int aes_hal_transform_dma_finish;
extern int sae_pk_valid_password;
extern int esp_timer_impl_get_min_period_us;
extern int mbedtls_ssl_check_curve_tls_id;
extern int esp_wifi_internal_set_sta_ip;
extern int r_ble_ll_adv_ext_set_param;
extern int g_mmgmt_mem_cnt_in_need;
extern int fputwc;
extern int __floatdisf;
extern int mbedtls_rsa_pkcs1_sign;
extern int cnx_validate_owe_bss;
extern int ble_mqueue_init;
extern int wdt_hal_deinit;
extern int os_memblock_get;
extern int uart_controller_write_byte;
extern int phy_printf;
extern int i2c_new_master_bus;
extern int wdev_mac_reg_load;
extern int ble_midi_get_use_indicate;
extern int spi_flash_chip_generic_timeout;
extern int crypto_ec_point_invert;
extern int pm_mac_modem_params_rt_update;
extern int esp_aes_gcm_update;
extern int sdmmc_can_discard;
extern int heap_caps_aligned_alloc;
extern int lv_display_get_event_count;
extern int lv_dropdown_get_selected_highlight;
extern int dbg_hal_check_clr_mplen_bitmap;
extern int driver_remove_destruct;
extern int r_ble_ll_mem_copy;
extern int mbedtls_asn1_get_alg;
extern int r_ble_lll_ext_scan_coex_dpc_update;
extern int npl_freertos_eventq_is_empty;
extern int mbedtls_mpi_mod_int;
extern int mbedtls_oid_get_ec_grp;
extern int r_ble_phy_isr;
extern int xTaskGetCurrentTaskHandleForCore;
extern int r_ble_ll_adv_sm_get;
extern int lv_indev_scroll_handler;
extern int esp_wifi_sta_get_sae_identifier_internal;
extern int hw_critical_state_status;
extern int strtof;
extern int ble_gattc_write;
extern int pwr_hal_set_mac_modem_beacon_miss_limit_exceeded_wakeup_enable;
extern int pm_twt_set_target_wdev_time;
extern int r_ble_ll_sync_env_deinit;
extern int phy_i2c_txrate_init_new;
extern int ecdsa_hal_verify_signature;
extern int wifi_find_first_registered_device;
extern int g_wdev_set_t1t4_cb;
extern int lv_obj_get_child_by_type;
extern int esp_mac_addr_len_get;
extern int cJSON_GetObjectItemCaseSensitive;
extern int coex_schm_ble_mesh_config_bt_conn_wifi_conn;
extern int mbedtls_ssl_write_change_cipher_spec;
extern int r_filter_duplicate_data_ring_list_init;
extern int lwip_getsockopt_impl_ext;
extern int mbedtls_mpi_gcd;
extern int r_ble_ll_mem_msys_init;
extern int crypto_bignum_deinit;
extern int r_ble_lll_init;
extern int ppCheckTxConnTrafficIdle;
extern int ble_transport_alloc_cmd;
extern int psa_sign_hash_builtin;
extern int mspi_timing_config_set_flash_clock;
extern int ble_sm_proc_find;
extern int esp_libc_time_init;
extern int ble_hs_flow_connection_broken;
extern int gdma_hal_connect_mem;
extern int RC_GetBlockAckTime;
extern int chm_release_lock;
extern int sdmmc_send_cmd_set_bus_width;
extern int lwip_netconn_do_join_leave_group_netif;
extern int ieee80211_ampdu_reorder;
extern int r_ble_log_set_buf_index_flag;
extern int lv_anim_core_init;
extern int ic_get_next_bcnoffset;
extern int _ZTISt13runtime_error;
extern int npl_freertos_event_set_arg;
extern int phy_set_rf_freq_offset;
extern int r_ble_lll_mmgmt_rxbuffer_alloc_precheck;
extern int ble_gap_rx_conn_complete;
extern int lv_global;
extern int ledc_update_duty;
extern int r_ble_lll_sleep_deinit;
extern int node_remove_from_list;
extern int ESP_EFUSE_ADC1_AVE_INITCODE_ATTEN3;
extern int esp_efuse_read_field_blob;
extern int __kernel_rem_pio2f;
extern int esp_ptr_byte_accessible;
extern int mbedtls_rsa_pkcs1_encrypt;
extern int mbedtls_ssl_reset_out_pointers;
extern int multi_heap_free_impl;
extern int ble_gap_enc_event;
extern int _ZTVSt19_Sp_counted_deleterIPN2tt7service12ServicePathsESt14default_deleteIS2_ESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int abort;
extern int cipher_type_map_public_to_internal;
extern int __fixunssfdi;
extern int r_ble_ll_mem_arr_pool_init;
extern int lv_obj_remove_style;
extern int regdma_link_new_branch_addr_map_default;
extern int g_mt;
extern int esp32_sdspi_fs_api;
extern int ieee80211w_get_active_igtk_key_id;
extern int phy_set_freq_i2c_new;
extern int ppCalTkipMic;
extern int esp_mmu_map_init;
extern int ble_store_write_cccd;
extern int r_ble_lll_convert_timeline_num_to_dtm_handle;
extern int rc_get_trc_by_index;
extern int gpio_descriptor_set_level;
extern int wpa_ap_rx_eapol;
extern int _ZSt15__try_use_facetISt8numpunctIwEEPKT_RKSt6locale;
extern int esp_crypto_hmac_enable_periph_clk;
extern int s_pm_beacon_offset_config;
extern int root_module;
extern int ieee80211_regdomain_check_ap_channel;
extern int clear_bss_queue;
extern int r_ble_ll_ctrl_ptr_check_set;
extern int memcmp;
extern int vsnprintf;
extern int r_os_mbuf_adj;
extern int audio_codec_read;
extern int pxCurrentTCBs;
extern int lv_os_init;
extern int r_ble_ll_conn_num_comp_pkts_event_send;
extern int esp_tls_init;
extern int esp_tls_get_error_handle;
extern int _ZSt13__adjust_heapIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEEiS2_NS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SK_SK_T1_T2_;
extern int esp_system_abort;
extern int _ZTSSt12length_error;
extern int ds_hal_read_result;
extern int i2c_controller_register8_reset_bits;
extern int wifi_init_key;
extern int phy_rc_cal;
extern int spi_flash_common_read_status_16b_rdsr_rdsr2;
extern int r_ble_ll_resolv_reset;
extern int _ZSt15__try_use_facetISt10moneypunctIwLb1EEEPKT_RKSt6locale;
extern int lv_display_delete;
extern int _ZSt14__convert_to_vIdEvPKcRT_RSt12_Ios_IostateRKPi;
extern int pm_coex_tbtt_process;
extern int ieee80211_vht_attach;
extern int adc_lock_release;
extern int ieee80211_pwrsave_node_cleanup;
extern int s_eapol_txdone_cb;
extern int lv_event_stop_bubbling;
extern int ieee80211_encap_null_data;
extern int ecc_hal_is_calc_finished;
extern int lv_draw_buf_destroy;
extern int r_ble_lll_priority_value_get;
extern int scan_stack_enableAdvFlowCtrlVsCmd;
extern int r_ble_ll_hci_le_cmd_send_cmd_status;
extern int ieee80211_parse_obss_scan_param;
extern int __fixsfsi;
extern int r_ble_log_internal_x3;
extern int _ZTVNSt8__format19_Formatting_scannerINS_10_Sink_iterIcEEcEE;
extern int esp_clk_apb_freq;
extern int phy_set_rfpll_freq;
extern int base_stack_initEnv;
extern int r_ble_ll_resolv_list_add;
extern int r_ble_hw_list_details_dump;
extern int lv_dropdown_get_options;
extern int r_ble_ll_df_reset;
extern int mbedtls_mpi_init;
extern int app_event_emit_close;
extern int lvgl_pointer_get_default;
extern int _ZSt19__throw_logic_errorPKc;
extern int r_ble_ll_hci_cb_customize_peer_sca;
extern int udp_send;
extern int r_hal_rtc_intr_init;
extern int ppResumeTxAMPDU;
extern int ap_rx_cb;
extern int rc_get_default_sched;
extern int isprint;
extern int mbedtls_pk_get_key_type;
extern int esp_netif_action_got_ip;
extern int lv_cache_is_enabled;
extern int lv_menu_class;
extern int lvgl_sliderbox_add_value_changed_cb;
extern int phy_get_freq_mem_param;
extern int udp_bind_netif;
extern int lv_display_get_physical_vertical_resolution;
extern int mbedtls_ecp_point_init;
extern int r_ble_ll_conn_hci_set_data_len;
extern int lv_spangroup_create;
extern int ppFillAMPDUBar;
extern int lv_ll_get_head;
extern int r_ble_ll_conn_init_phy;
extern int mpi_hal_write_at_offset;
extern int crypto_ec_get_mbedtls_to_nist_group_id;
extern int tlsf_realloc;
extern int tt_init_tactility_c;
extern int g_mmgmt_mempool_size;
extern int sta_rx_cb;
extern int mbedtls_cipher_set_padding_mode;
extern int esp_flash_is_quad_mode;
extern int lv_color16_luminance;
extern int r_ble_ll_conn_create_cancel;
extern int clk_hal_soc_root_get_freq_mhz;
extern int sntp_setservername;
extern int r_filter_duplicate_check_mesh_special_in_table;
extern int aes_ccm_ad;
extern int lmacProcessTxSuccess;
extern int r_ble_ll_auth_pyld_tmo_event_send;
extern int tsf_hal_set_rx_beacon_abort_tsf_time_deviation_sync_enable;
extern int phy_freq_chan_en_sw;
extern int lv_obj_set_style_width;
extern int ppCalTxHEAMPDULength;
extern int r_ble_ll_set_host_feat;
extern int wpa_auth_uses_sae;
extern int spi_flash_chip_generic_erase_block;
extern int ic_register_config_cb;
extern int g_mesh_is_root;
extern int hostap_eapol_resend_process;
extern int sprintf;
extern int sta_auth_ft;
extern int ble_sm_lgcy_confirm_exec;
extern int itwt_probe_timeout_fn_process;
extern int ble_store_write_csfc;
extern int phy_set_tsens_pwr;
extern int lmacInitAc;
extern int lv_event_get_current_target_obj;
extern int r_ble_ll_rand_sample;
extern int ble_lll_sched_env_p;
extern int phy_rx_loop_cap_set;
extern int lv_tabview_add_tab;
extern int ieee80211_set_hmac_stop;
extern int ieee80211_mgmt_output;
extern int WIFI_EVENT;
extern int lv_arc_get_mode;
extern int _scan_build_chan_list;
extern int wifi_nvs_validate_ap_num;
extern int mpi_hal_interrupt_enable;
extern int esp_vfs_fat_register;
extern int r_ble_lll_mmgmt_free_tx_buffer_hdr;
extern int lv_textarea_cursor_right;
extern int ble_hs_stop_deinit;
extern int pm_set_keep_alive_time;
extern int ble_gap_reattempt_count;
extern int acos;
extern int mbedtls_rsa_rsaes_pkcs1_v15_encrypt;
extern int lv_obj_set_style_pad_row;
extern int ieee80211_regdomain_get_country_ie;
extern int _Z8gps_initP6Device8GpsModel;
extern int scan_set_act_duration;
extern int cs_free_ctrl_sock;
extern int esp_wifi_register_owe_cb;
extern int lwip_getaddrinfo;
extern int r_ble_lll_conn_module_deinit;
extern int r_ble_ll_hci_ev_tx_pwr_report;
extern int wifi_sta_connect_internal_process;
extern int ppSetInterface;
extern int _ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base;
extern int adc_oneshot_read;
extern int ble_gatts_start;
extern int r_DB_hash_list_remove;
extern int esp_vfs_close;
extern int _ZSt25__throw_bad_function_callv;
extern int ble_att_mtu_by_cid;
extern int rtos_int_exit;
extern int lv_ll_remove;
extern int _ZGVNSt8time_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int esp_test_rx_process_complete;
extern int s_itwt_flow_id_bitmap;
extern int tcp_rexmit_rto;
extern int r_ble_ll_resolv_rpa;
extern int r_ble_ll_hci_post_rd_p256_pubkey_cmp_evt;
extern int r_ble_lll_sched_restart_link;
extern int wifi_stop_process;
extern int ff_wl_handles;
extern int ppRecycleAmpdu;
extern int _ZSt17iostream_categoryv;
extern int _ZTVSt14basic_ifstreamIcSt11char_traitsIcEE;
extern int pm_beacon_offset_is_enabled;
extern int range_read_addr_blocks;
extern int wpa_ether_send;
extern int wifi_softap_process_bsscolor_collision;
extern int lv_draw_label;
extern int _ZTv0_n12_NSt14basic_ifstreamIcSt11char_traitsIcEED0Ev;
extern int r_ble_lll_sleep_timer_tick_get;
extern int xTaskGetIdleTaskHandleForCore;
extern int ieee80211_regdomain_chan_in_range;
extern int _ZGVNSt7__cxx1110moneypunctIwLb1EE2idE;
extern int lv_obj_remove_style_all;
extern int i2c_controller_register16be_set;
extern int r_os_msys_unregister;
extern int multi_heap_internal_lock;
extern int hal_he_get_mplen_addr_start;
extern int ble_hs_id_ensure_ctx;
extern int nan_stop;
extern int lmacStopTransmit;
extern int r_ble_ll_calc_offset_ticks_us_for_rampup;
extern int phy_chan14_mic_cfg_new;
extern int esp_io_expander_gpio_wrapper_configure_pin;
extern int gWpaSm;
extern int mbedtls_x509_get_sig;
extern int phy_dis_hw_set_freq_new;
extern int hci_driver_vhci_ops;
extern int esp_wifi_set_igtk_internal;
extern int gdma_stop;
extern int esp_clk_tree_lp_slow_get_freq_hz;
extern int esp_aes_gcm_crypt_and_tag;
extern int spi_flash_needs_reset_check;
extern int pmu_hal_hp_set_sleep_modem_backup_enable;
extern int __nedf2;
extern int lv_strcpy;
extern int module_add;
extern int mmu_hal_vaddr_to_paddr;
extern int _Z33app_fs_list_direct_subdirectoriesRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERSt6vectorIS4_SaIS4_EE;
extern int lv_calendar_header_arrow_class;
extern int mbedtls_oid_get_ec_grp_algid;
extern int ble_gap_rx_disconn_complete;
extern int esp_test_tx_count_timeout;
extern int esp_wifi_power_domain_off;
extern int pbuf_take;
extern int esp_mmu_vaddr_to_paddr;
extern int sys_arch_mbox_tryfetch;
extern int trcAmpduSetState;
extern int http_parser_parse_url;
extern int mbedtls_psa_rsa_export_key;
extern int ble_store_iterate;
extern int netif_remove_ext_callback;
extern int preferences_put_int32;
extern int ppRecycleRxPkt;
extern int r_ble_phy_disable;
extern int esp_phy_load_cal_data_from_nvs;
extern int _interrupt_handler;
extern int phy_xpd_tsens;
extern int ip_addr_any;
extern int wifi_create_nan;
extern int ble_hs_conn_find_by_idx;
extern int ic_mac_deinit;
extern int clearerr;
extern int lv_arc_get_angle_start;
extern int esp_wifi_internal_configure_wake_window;
extern int phymode2str;
extern int ieee80211_regdomain_update;
extern int r_ble_ll_sync_check_acad;
extern int xRingbufferReceiveFromISR;
extern int f_unlink;
extern int esp_flash_spi_init_include_func;
extern int ble_att_clt_rx_read_type;
extern int trackball_read_delta;
extern int lv_obj_calculate_ext_draw_size;
extern int lv_timer_resume;
extern int _ZTVSt5ctypeIcE;
extern int r_ble_lll_conn_master_new;
extern int mbedtls_ecdsa_sign_restartable;
extern int copy_arguments;
extern int _lock_release;
extern int i2s_hal_std_set_rx_slot;
extern int coshf;
extern int tsf_hal_set_tbtt_rf_ctrl_disable;
extern int pm_mac_enable_tsf_tbtt_modem_wakeup;
extern int lv_textarea_get_accepted_chars;
extern int lv_fs_deinit;
extern int sdmmc_can_trim;
extern int scan_inter_channel_timeout_process;
extern int ble_random_adjust_co_count;
extern int phy_rfrx_gain_cal;
extern int ieee80211_hostapd_ps_txcb;
extern int mbedtls_ssl_handshake_step;
extern int gdma_hal_enable_intr;
extern int ble_random_conn_env_p;
extern int hal_disable_sta_tbtt;
extern int g_sw_cte_buffer_ptr;
extern int pmu_hp_system_power_param_default;
extern int esp_psram_impl_enable;
extern int esf_buf_recycle;
extern int xEventGroupClearBitsFromISR;
extern int mbedtls_ecdsa_verify_restartable;
extern int npl_freertos_time_get;
extern int ble_store_read_our_sec;
extern int audio_stream_open_output;
extern int tcp_bind_netif;
extern int spi_flash_hal_configure_host_io_mode;
extern int ip6_options_add_hbh_ra;
extern int __truncdfsf2;
extern int lv_arc_class;
extern int mbedtls_gcm_setkey;
extern int g_panic_abort;
extern int __lock___dd_hash_mutex;
extern int pmu_lp_system_analog_param_default;
extern int bt_bb_tx_cca_fifo_count;
extern int net80211_funcs_deinit;
extern int lv_indev_get_display;
extern int _ZTVN2tt7service5audio12AudioServiceE;
extern int r_ble_ll_rand_data_get;
extern int __lock___tz_mutex;
extern int display_get_swap_xy;
extern int hal_mac_color_get_collide_bitmap;
extern int r_ble_controller_init;
extern int sdmmc_erase_sectors;
extern int modem_clock_hal_enable_wifipwr_clock;
extern int scan_add_probe_ssid;
extern int wpa_alloc_eapol;
extern int r_esp_ble_set_cca_enable;
extern int mbedtls_gcm_starts;
extern int r_ble_ll_adv_init;
extern int r_ble_ll_conn_hci_rd_auth_pyld_tmo;
extern int bt_set_rx_comp;
extern int int_array_len;
extern int phy_bt_txdc_cal;
extern int phy_freq_module_resetn;
extern int r_ble_lll_sched_remove_overtime_entries;
extern int lv_draw_sw_grad_color_calculate;
extern int _ZSt7vformatB5cxx11St17basic_string_viewIcSt11char_traitsIcEESt17basic_format_argsISt20basic_format_contextINSt8__format10_Sink_iterIcEEcEE;
extern int mbedtls_x509_parse_subject_alt_name;
extern int r_ble_lll_adv_event_is_destroyed;
extern int coex_schm_ble_default_bt_a2dp_wifi_scan;
extern int sdmmc_send_cmd_switch_func;
extern int audio_stream_close;
extern int _ZZNSt18__moneypunct_cacheIcLb0EE8_M_cacheERKSt6localeEN11_Scoped_strC1ERKSs;
extern int ble_gap_passkey_event;
extern int ble_hs_flow_init;
extern int ieee80211_get_spp;
extern int vListInsertEnd;
extern int get_vendor_ie;
extern int esp_crypto_aes_gcm_encrypt;
extern int strcasestr;
extern int phy_linear_to_db;
extern int power_supply_is_quick_charge_enabled;
extern int ets_timer_setfn;
extern int ble_att_tx;
extern int r_filter_duplicate_addr_ring_list_init;
extern int rtc_clk_32k_enable;
extern int ic_get_pp_hdl;
extern int ESP_EFUSE_KEY_PURPOSE_3;
extern int file_mutex_try_lock;
extern int regdma_link_set_write_wait_content;
extern int lv_bar_set_range;
extern int esp_aes_crypt_ctr;
extern int r_ble_ll_scan_dup_move_to_head;
extern int key_mgr_hal_get_state;
extern int device_get_first_by_type;
extern int mbedtls_pkcs5_pbes2_ext;
extern int ble_att_svr_find_by_handle;
extern int f_opendir;
extern int _ZSt28__throw_bad_array_new_lengthv;
extern int ieee80211_amsdu_send_check;
extern int scan_remove_probe_ssid;
extern int g_ble_phy_t_rxaddrdelay_ro;
extern int g_config_softap_on_dfs_channel;
extern int tt_app_fileselection_start_for_existing_or_new_file;
extern int strnlen;
extern int esp_http_client_get_header;
extern int raw_recv;
extern int lwip_setsockopt_impl_ext;
extern int hal_mac_is_low_rate_enabled;
extern int esp_ecc_point_multiply;
extern int wifi_set_appie_process;
extern int r_ble_lll_adv_rx_process;
extern int libcoexist_reversion_git;
extern int psa_validate_key_persistence;
extern int he_recv_action_muedca_reset;
extern int r_ble_lll_sched_deinit;
extern int r_ble_lll_conn_rx_process;
extern int console_open;
extern int lvgl_get_statusbar_icon_font;
extern int lv_style_get_prop;
extern int USB_HOST_MSC_TYPE;
extern int _open_r;
extern int ble_bb_cte_set_cte_ant_switch_delay_rx_aoa_1us_2m;
extern int lv_palette_darken;
extern int phy_bt_tx_gain_init_new;
extern int wpa_auth_sta_deinit;
extern int stdin;
extern int g_hci_stack_envP;
extern int esp_http_client_open;
extern int lv_draw_fill;
extern int esp_setup_newlib_syscalls;
extern int bluetooth_set_device_name;
extern int sntp_stop;
extern int ppDisableQueue;
extern int lv_obj_set_flex_grow;
extern int r_ble_lll_conn_unacked_ctrl_pdu_handle;
extern int sdmmc_init_sd_ssr;
extern int esp_flash_freq_limit_cb;
extern int wdev_csi_len_align;
extern int __global_locale;
extern int aes_hal_set_iv;
extern int r_ble_ll_conn_enqueue_pkt;
extern int lv_cache_entry_inc_ref;
extern int uECC_shared_secret;
extern int fseek;
extern int lv_color_to_u16;
extern int pm_beacon_monitor_timeout_process;
extern int pm_on_data_rx;
extern int r_os_msys_register_block_num_get_cb;
extern int esp_sha256_driver_finish;
extern int phy_rx_gain_force;
extern int esp_wifi_register_wpa3_cb;
extern int wpa_parse_wpa_ie_wrapper;
extern int ppDequeueTxQ;
extern int psa_crypto_init;
extern int pm_offchan_is_waiting;
extern int vTaskSetTimeOutState;
extern int register_ieee80211_action_vendor_spec_cb;
extern int lv_anim_path_ease_in_out;
extern int esp_phy_disable;
extern int BLUETOOTH_MIDI_TYPE;
extern int r_ble_lll_adv_update_aux_offset_in_ext;
extern int ic_register_pm_tx_null_cb;
extern int s_pm_beacon_offset;
extern int lv_cache_set_name;
extern int lv_canvas_class;
extern int driver_find_compatible;
extern int preferences_close;
extern int ieee80211_setup_rateset;
extern int sdmmc_mmc_enable_hs_mode;
extern int lv_style_set_bg_image_src;
extern int r_ble_ll_conn_hci_remote_read_transmit_power_level;
extern int exp;
extern int ledc_hal_set_fade_param;
extern int pm_set_beacon_filter;
extern int lv_obj_set_style_flex_cross_place;
extern int lv_obj_set_style_align;
extern int r_ble_lll_mmgmt_env_init;
extern int r_ble_ll_adv_sm_fetch_new;
extern int root_is_model;
extern int lv_menu_set_page;
extern int r_ble_lll_mmgmt_global_block_malloc;
extern int ble_sm_deinit;
extern int wps_get_wps_sm_cb;
extern int _ZSt8to_charsPcS_eSt12chars_formati;
extern int ble_gattc_rx_find_type_value_hinfo;
extern int esp_netif_get_nr_of_ifs;
extern int mbedtls_mpi_cmp_mpi;
extern int SPI_CONTROLLER_TYPE;
extern int rtc_clk_slow_src_get;
extern int display_get_frame_buffer;
extern int memp_ARP_QUEUE;
extern int tcp_fasttmr;
extern int ieee80211_ht_updatehtcap;
extern int lv_style_set_length;
extern int r_ble_ll_ctrl_channel_status_report_timer_cb;
extern int sin;
extern int spicommon_irqsource_for_host;
extern int _ZSt8to_charsPcS_dSt12chars_format;
extern int esp_vfs_fat_partition_drive;
extern int phy_txcal_debuge_mode_new;
extern int esp_wifi_internal_free_rx_buffer;
extern int lv_arc_create;
extern int phy_iq_corr_enable;
extern int phy_agc_max_gain_set;
extern int tanf;
extern int pwr_hal_set_mac_modem_beacon_miss_limit_exceeded_wakeup_disable;
extern int dts_modules;
extern int vfs_include_syscalls_impl;
extern int __bufio_put;
extern int udp_input;
extern int ieee80211_update_twt;
extern int bt_agc_detect_set;
extern int panic_restart;
extern int esp32_wifi_driver;
extern int nimble_port_deinit;
extern int uECC_vli_clear;
extern int mbedtls_ecp_check_pubkey;
extern int r_hal_timer_process;
extern int heap_caps_get_total_size;
extern int tcp_close_ext;
extern int pm_tx_null_data_done_twt_process;
extern int vhtcaps_maxRxMPDULen;
extern int lv_obj_get_height;
extern int phy_wait_freq_hw_hop_done;
extern int wep_decap;
extern int check_bss_queue;
extern int g_msys_pool_list;
extern int bootloader_flash_update_id;
extern int ieee80211_decap1;
extern int wpa_auth_pmksa_add2;
extern int lvgl_module_config;
extern int lv_textarea_set_one_line;
extern int phy_read_sar_dout;
extern int ble_gap_adv_rsp_set_fields;
extern int i2c_common_deinit_pins;
extern int __netf2;
extern int lv_obj_set_layout;
extern int ieee80211_scan_deattach;
extern int he_get_min_subframe_len;
extern int crypto_bignum_init_set;
extern int ieee80211_ioctl;
extern int _ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base;
extern int sdmmc_init_sd_current_limit;
extern int _Z24ble_schedule_adv_restartP6Devicey;
extern int r_ble_lll_mmgmt_get_next_rxbuf_reuse_sch_item;
extern int pmksa_cache_get_current;
extern int periph_rcc_acquire_exit;
extern int coex_core_register_start_cb;
extern int mbedtls_mutex_lock;
extern int lv_obj_class;
extern int r_ble_ll_conn_hci_param_rr;
extern int esp_sha512_driver_clone;
extern int g_extAdv_stack_envP;
extern int h_errno;
extern int ppTxProtoProc;
extern int cache_hal_invalidate_addr;
extern int ampdu_free_extra_softap_rx_ba_index;
extern int multi_heap_internal_unlock;
extern int r_ble_ll_conn_set_cur_phy_txpwr;
extern int scan_build_chan_list;
extern int lv_theme_default_get;
extern int lwip_gethostbyname;
extern int esp_psram_impl_get_cs_io;
extern int esp_transport_ssl_set_client_cert_data;
extern int mac_txrx_init;
extern int rand;
extern int sys_untimeout;
extern int i2s_channel_reconfig_std_gpio;
extern int _ZSt8to_charsPcS_eSt12chars_format;
extern int sntp_set_system_time;
extern int zb_rx_core_set;
extern int dhcp_network_changed_link_up;
extern int g_ic;
extern int he_recv_action_twt_information;
extern int _Z19app_fs_is_directoryRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int memchr;
extern int r_scan_duplicate_cache_refresh_timer_stop;
extern int spi_hal_push_tx_buffer;
extern int esp_efuse_startup_include_func;
extern int vTaskDelete;
extern int ble_gap_master_in_progress;
extern int lv_spinbox_get_value;
extern int tsf_hal_get_counter_value;
extern int lv_bin_decoder_init;
extern int esp_transport_ssl_set_interface_name;
extern int hostapd_setup_sae_pt;
extern int ieee80211_add_operating_mode_notification;
extern int device_construct;
extern int r_ble_ll_conn_module_deinit;
extern int lwip_netconn_do_connect;
extern int spi_flash_chip_generic_detect_size;
extern int he_recv_action_bsscolor_collision;
extern int _ZTv0_n12_NSoD1Ev;
extern int pm_offchan_execute_ready_cb;
extern int netconn_send;
extern int cache_error_msg;
extern int esp_flash_write;
extern int driver_bind;
extern int esp_perip_clk_init;
extern int esp_netif_dhcpc_get_status;
extern int psa_cipher_update;
extern int esp_rtc_get_time_us;
extern int lvgl_pointer_get_slot_index;
extern int mpi_hal_enable_constant_time;
extern int esp_vfs_null_register;
extern int ble_hs_atomic_first_conn_handle;
extern int phy_rfcal_data_check;
extern int uart_hal_set_data_bit_num;
extern int task_event_group_construct;
extern int mbedtls_psa_aead_decrypt;
extern int _ZTSN9__gnu_cxx26__concurrence_unlock_errorE;
extern int gdma_periph_signals;
extern int ble_gap_rx_update_complete;
extern int VolToPart;
extern int esp_key_mgr_activate_key;
extern int lv_realloc;
extern int strlcat;
extern int r_ble_log_hci_vs_cmd_proc;
extern int ble_gatts_rx_indicate_ack;
extern int spi_flash_common_read_status_8b_rdsr2;
extern int r_ble_ll_hci_ev_rem_conn_parm_req;
extern int lmacProcessShortFrameSuccess;
extern int r_ble_lll_adv_pre_process;
extern int sdmmc_init_sd_uhs1;
extern int add_mic_ie_bip;
extern int ble_sm_lgcy_io_action;
extern int pm_unregister_connectionless_wake_window;
extern int lmacRetryTxFrame;
extern int r_ble_lll_mmgmt_update_mem_last_ptr;
extern int atan2f;
extern int r_ble_ll_event_dbuf_overflow;
extern int wifi_deinit;
extern int i2c_hal_slave_init;
extern int mbedtls_ct_memmove_left;
extern int ble_hs_util_ensure_addr;
extern int wpa3_hostap_auth_deinit;
extern int _close_r;
extern int power_supply_power_off;
extern int crypto_ecdh_init;
extern int lv_textarea_get_text_selection;
extern int lv_binfont_destroy;
extern int r_ble_lll_sleep_instant_wakeup_check;
extern int lv_image_cache_drop;
extern int xpt2046_softspi_power_supply_driver;
extern int modem_clock_get_module_deps;
extern int xStreamBufferBytesAvailable;
extern int ble_store_write_our_sec;
extern int task_event_group_signal;
extern int gps_get_state;
extern int r_ble_ll_pdu_tx_time_get;
extern int chm_set_current_band;
extern int wifi_api_unlock;
extern int lv_refr_now;
extern int esp_vfs_register;
extern int _ZTSSt11logic_error;
extern int xTaskCreatePinnedToCoreWithCaps;
extern int ble_sm_pair_initiate;
extern int _ZZNKSt8__format14__formatter_fpIcE6formatIeNS_10_Sink_iterIcEEEENSt20basic_format_contextIT0_cE8iteratorET_RS7_ENKUlPcSB_E_clESB_SB_;
extern int pm_is_waked;
extern int wifi_nvs_validate_country;
extern int wpa_compare_rsn_ie;
extern int chm_deinit;
extern int r_ble_lll_conn_event_is_destroyed;
extern int phy_freq_band_reg_set;
extern int uxQueueSpacesAvailable;
extern int phy_bb_reg_init_new;
extern int sta_rx_eapol;
extern int _ctype_b;
extern int cnx_node_search;
extern int gScanStruct;
extern int lv_obj_get_scroll_right;
extern int isinf;
extern int ble_gatts_rx_indicate_rsp;
extern int s_wifi_task_hdl;
extern int esp_netif_transmit_wrap;
extern int __divdi3;
extern int r_ble_ll_conn_calc_closest_event_cntr;
extern int ble_uuid_init_from_att_mbuf;
extern int lv_buttonmatrix_set_button_width;
extern int phy_chan_vs_index;
extern int phy_freq_to_index;
extern int lv_theme_get_font_small;
extern int bt_bb_tx_cca_fifo_read;
extern int i2c_hal_master_handle_tx_event;
extern int thread_get_task_handle;
extern int lv_draw_image;
extern int pwr_hal_set_beacon_filter_frame_crc_state;
extern int phy_close_fe_bb_clk;
extern int printf_decode;
extern int lv_fs_open;
extern int lvgl_sliderbox_create;
extern int r_ble_ll_scan_move_window_to;
extern int ble_att_svr_prep_clear;
extern int esp_bt_controller_get_status;
extern int thread_set_main_function;
extern int xStreamBufferSend;
extern int pbuf_free_header;
extern int mbedtls_psa_rsa_generate_key;
extern int ble_store_write_local_irk;
extern int gpio_od_disable;
extern int i2s_gpio_check_and_set;
extern int lv_display_add_event_cb;
extern int hal_mac_txq_disable;
extern int gpio_get_io_config;
extern int _ZSt13__heap_selectIN9__gnu_cxx17__normal_iteratorIPtSt6vectorItSaItEEEENS0_5__ops15_Iter_less_iterEEvT_S9_S9_T0_;
extern int wpa_auth_pmksa_add_sae;
extern int esp_hw_stack_guard_get_bounds;
extern int i2c_cmd_link_create;
extern int pm_is_twt_start;
extern int mbedtls_pk_rsa_set_key;
extern int is_off_channel;
extern int esp_netif_is_netif_listed;
extern int coex_schm_ble_mesh_standby_bt_a2dp_wifi_connecting;
extern int aes_hal_transform_block;
extern int ieee80211_htinfo_update_chw;
extern int chip_disable;
extern int esp_aes_cipher_encrypt_setup;
extern int r_ble_ll_ctrl_terminate_start;
extern int ble_hid_set_active;
extern int esf_buf_recycle_default_handler;
extern int uart_periph_signal;
extern int lv_text_cut;
extern int adiList_stack_rmSidFromList;
extern int gpio_intr_disable;
extern int ieee80211_parse_extcap;
extern int r_ble_lll_sched_stop;
extern int pbuf_add_header;
extern int mbedtls_ecp_group_init;
extern int coex_schm_ble_mesh_config_bt_sniff_sco_wifi_connecting;
extern int pm_twt_set_state;
extern int hal_vhtdlmu_set_group_id;
extern int ble_gap_tx_test_evt;
extern int cJSON_GetArrayItem;
extern int r_DB_check_hash_exist;
extern int phy_set_txcap_reg;
extern int sdmmc_init_sd_blocklen;
extern int fmodf;
extern int wdevProcessModemStateRxBeacon;
extern int _ZTVSt23_Sp_counted_ptr_inplaceIN2tt6PubSubI9WifiEventEESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int haptic_set_waveform;
extern int memp_NETDB;
extern int cnx_obss_scan_timeout;
extern int lv_draw_buf_init_handlers;
extern int coex_schm_ble_default_bt_idle_wifi_connecting;
extern int nan_sm_handle_event;
extern int hal_mac_rx_set_policy;
extern int module_destruct;
extern int r_ble_ll_read_supp_features;
extern int r_ble_lll_mmgmt_free_buffer_hdr;
extern int pau_hal_set_regdma_wait_timeout;
extern int mbedtls_x509_get_subject_alt_name_ext;
extern int tsf_hal_set_tbtt_intr_enable;
extern int hci_transport_init;
extern int ble_sm_process_result;
extern int lv_buttonmatrix_set_map;
extern int netif_set_addr;
extern int lwip_netconn_do_gethostbyname;
extern int lv_obj_move_children_by;
extern int hal_tsf_clear_soc_wakeup_request;
extern int spi_bus_register_destroy_func;
extern int coex_schm_bt_a2dp_paused_wifi_conn;
extern int r_ble_lll_scan_stop;
extern int mesh_sta_auth_expire_time;
extern int dispatcher_consume_timed;
extern int r_ble_ll_scan_rx_pkt_in_restore_addr_data;
extern int _ZSt8to_charsPcS_f;
extern int he_send_action_bsscolor_collision;
extern int hal_vhtdlmu_clr_group_id;
extern int app_manager_get_topmost_app_id;
extern int wifi_process_bsscolor_collision;
extern int esp_vfs_select_triggered_isr;
extern int esp_lcd_panel_disp_sleep;
extern int r_exception_list_ble_mesh_filter_enable;
extern int wifi_nvs_set;
extern int lv_label_get_recolor;
extern int lv_display_get_screen_active;
extern int phy_chip_set_chan_offset;
extern int ble_gattc_rx_find_type_value_complete;
extern int ppProcTxSecFrame;
extern int wifi_softap_get_config;
extern int coex_schm_ble_mesh_config_bt_piscan_wifi_conn;
extern int phy_set_tsens_power;
extern int _ZGVNSt8time_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int r_ble_lll_sync_alloc_memory;
extern int r_ble_lll_sync_reset_sm;
extern int bundle_alloc;
extern int g_ble_ll_pdu_header_tx_time_ro;
extern int ble_transport_to_hs_acl_impl;
extern int isalpha;
extern int wifi_station_get_rssi;
extern int r_ble_lll_scan_npl_init;
extern int phy_rf_init;
extern int wpa_free_eapol;
extern int lv_obj_update_layer_type;
extern int lv_obj_refresh_style;
extern int phy_xtal_duty_cal_init;
extern int dts_devices;
extern int ble_gap_event_listener_unregister;
extern int haptic_stop_playback;
extern int lv_obj_add_event_cb;
extern int lv_draw_sw_init;
extern int _ZSt8to_charsPcS_fSt12chars_format;
extern int GPS_TYPE;
extern int memp_PBUF_POOL;
extern int cJSON_ParseWithLengthOpts;
extern int r_ble_lll_mmgmt_alloc_rx_buffer;
extern int key_mgr_hal_set_key_purpose;
extern int wpa_sm_drop_sa;
extern int regdma_link_new_addr_map_default;
extern int cnx_handshake_timeout;
extern int sdmmc_io_init_check_card_cap;
extern int sdmmc_io_init_read_card_cap;
extern int ble_store_delete_peer_sec;
extern int netbuf_delete;
extern int phy_freq_to_mbgain;
extern int mbedtls_x509_crt_parse_der_nocopy;
extern int sdmmc_init_sd_scr;
extern int ip6_frag;
extern int dhcp_append_extra_opts;
extern int ppCheckTxAMPDUlength;
extern int lwip_getsockopt;
extern int ble_random_mode_conn_exec_process_cb_get;
extern int phy_tx_pwctrl_init_cal_new;
extern int ppPrepareBarFrame;
extern int esp_register_freertos_idle_hook_for_cpu;
extern int esp_http_client_read;
extern int usb_midi_device_send;
extern int esp_vfs_fat_info;
extern int esp_sha1_driver_compute;
extern int ble_uuid_to_str;
extern int scan_enter_oper_channel_process;
extern int esp_aes_gcm_free;
extern int esp_supplicant_common_deinit;
extern int _ZTVN2tt7service9webserver16WebServerServiceE;
extern int mbedtls_pk_psa_rsa_sign_ext;
extern int mbedtls_ssl_conf_alpn_protocols;
extern int r_ble_hw_whitelist_add;
extern int esp_log_cache_add;
extern int s_nan_cb;
extern int phy_bb_dcmem_clr;
extern int lv_display_get_dpi;
extern int r_os_mbuf_copyinto;
extern int sdmmc_io_enable_int;
extern int ampdu_dispatch_upto;
extern int r_ble_rtc_wake_up_state_clr;
extern int ble_gap_pairing_complete_event;
extern int npl_freertos_callout_stop;
extern int ble_att_conn_chan_find;
extern int r_ble_ll_sync_on_list;
extern int rcGetDCMMaxRate;
extern int app_metadata_parse;
extern int pm_coex_schm_process_restart;
extern int nan_dp_node_exists;
extern int dbg_cnt_lmac_drop;
extern int aes_decrypt;
extern int _ZTSSt13runtime_error;
extern int r_ble_hci_trans_cfg_ll;
extern int lv_obj_set_align;
extern int lv_style_prop_get_default;
extern int sha_hal_hash_dma;
extern int bluetooth_hid_device_send_mouse;
extern int ieee80211_add_action_vendor_spec_esp;
extern int _ZTVSt9money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE;
extern int _ZGVNSt9money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int trc_update_ifx_phy_mode;
extern int esp_restart;
extern int r_ble_ll_whitelist_rmv;
extern int ble_gattc_init;
extern int _ZTVSt8ios_base;
extern int phy_clr_modem_flag;
extern int spi_bus_lock_get_dev_id;
extern int lv_anim_set_repeat_delay;
extern int r_ble_lll_scan_alloc_memory;
extern int lvgl_module_configure;
extern int lv_rb_insert;
extern int wifi_assert;
extern int _ZZNKSt7collateIcE12do_transformEPKcS2_EN4_BufD2Ev;
extern int audio_codec_set_volume;
extern int httpd_resp_send;
extern int lv_obj_stop_scroll_anim;
extern int __ffsdi2;
extern int color_change_timer;
extern int mbedtls_ct_zeroize_if;
extern int r_ble_ll_resolv_env_deinit;
extern int ble_gap_adv_stop;
extern int sys_mbox_new;
extern int lv_chart_set_next_value;
extern int gdma_register_rx_event_callbacks;
extern int phy_freq_correct;
extern int phy_set_chanfreq;
extern int rename;
extern int ic_set_current_channel;
extern int meshtastic_gps_driver;
extern int pm_coex_set_reconnect_policy;
extern int regdma_link_new_continuous_default;
extern int lv_color_mix32;
extern int spi_flash_hal_check_status;
extern int ble_ll_hci_ev_subrate_change;
extern int esp_tls_get_bytes_avail;
extern int psa_export_key;
extern int i2s_controller_reset;
extern int esp_wifi_auth_done_internal;
extern int spi_flash_encryption_hal_enable_pseudo_rounds;
extern int ap_no_lr;
extern int material_symbols_shared_16;
extern int spi_bus_lock_set_bg_control;
extern int sdmmc_init_host_bus_width;
extern int rc_get_trc;
extern int conn_stack_getBroker;
extern int sdmmc_init_ocr;
extern int getc_unlocked;
extern int tlsf_free;
extern int s_itwt_suspend_flow_id_bitmap;
extern int lv_style_set_transform_width;
extern int xTimerGetPeriod;
extern int esp_cache_sync_ops_exit_critical_section;
extern int ble_hs_start;
extern int dns_tmr;
extern int he_time_per_sym;
extern int coex_schm_bt_sniff_sco_wifi_scan;
extern int pp_timer_register_post_cb;
extern int pbuf_free_ooseq_pending;
extern int pm_is_twt_send_null;
extern int ble_att_clt_rx_read_mult_var;
extern int esp_wifi_scan_stop;
extern int lv_color_to_int;
extern int esp_task_wdt_add;
extern int __lshrdi3;
extern int pm_send_nullfunc;
extern int pmu_hal_hp_set_sleep_active_backup_disable;
extern int __ascii_wctomb;
extern int mpi_hal_wait_op_complete;
extern int lv_anim_set_reverse_duration;
extern int mbedtls_mpi_safe_cond_swap;
extern int pwr_hal_set_mac_modem_beacon_miss_intr_disable;
extern int ieee80211_setup_lr_rates;
extern int ble_sm_sc_ensure_ctx;
extern int esp_aes_cipher_abort;
extern int lv_text_get_width;
extern int netconn_tcp_recvd;
extern int ble_stack_enableVsCmds;
extern int lvgl_window_manager_module;
extern int multi_heap_set_lock;
extern int misc_nvs_load;
extern int r_ble_hw_get_public_addr;
extern int mbedtls_ssl_set_timer;
extern int pointer_get_mirror_y;
extern int r_DB_addr_list_add;
extern int esp_intr_disable;
extern int wifi_stop_sw_txq;
extern int phy_module_disable;
extern int mbedtls_ecp_is_zero;
extern int esp_flash_erase_region;
extern int mbedtls_mpi_core_add_if;
extern int g_deauth_mac_list;
extern int ble_hs_misc_restore_irks;
extern int lv_display_get_inactive_time;
extern int ieee80211w_get_igtk_from_keyidx;
extern int r_ble_ll_is_valid_random_addr;
extern int ieee80211_add_rates;
extern int sdmmc_decode_csd;
extern int dragonfly_is_quadratic_residue_blind;
extern int r_ble_ll_rand_env_init;
extern int RC_GetCtsTime;
extern int r_ble_ll_adv_periodic_set_param;
extern int ble_l2cap_sig_cmd_get;
extern int lmacRxDone;
extern int ble_hs_conn_insert;
extern int bin_clear_free;
extern int lv_event_get_code;
extern int ieee80211_construct_action_vendor_spec;
extern int spi_flash_hal_read;
extern int gpio_hal_hysteresis_soft_enable;
extern int spicommon_cs_free_io;
extern int spicommon_periph_free;
extern int _pow;
extern int hal_timer_update_by_rtc;
extern int lv_fs_read;
extern int mbedtls_x509_get_ns_cert_type;
extern int update_cs_thres;
extern int esp_transport_connect;
extern int igmp_joingroup;
extern int ble_freertos_evq_pool;
extern int phy_get_tm7;
extern int ESP_EFUSE_RD_DIS_BLOCK_KEY1;
extern int __cxa_throw_bad_array_new_length;
extern int memory_alloc_with_policy;
extern int esp_hmac_setup_transparent;
extern int wDev_enable_low_rate;
extern int wcsxfrm;
extern int lv_ll_ins_prev;
extern int _ZZNKSt7collateIwE12do_transformEPKwS2_EN4_BufD1Ev;
extern int _ZSt11_Hash_bytesPKvjj;
extern int ble_l2cap_sig_update;
extern int ieee80211_parse_vhtcap;
extern int wifi_lmac_init;
extern int xt_unhandled_exception;
extern int mbedtls_mutex_free;
extern int wpa_sm_set_pmk_from_pmksa;
extern int send_wake_null_timer;
extern int lv_obj_set_style_text_font;
extern int pm_disconnected_sleep;
extern int icmp6_input;
extern int pm_on_isr_twt_wake;
extern int xTimerGetTimerDaemonTaskHandle;
extern int _ZZNSt18__moneypunct_cacheIcLb1EE8_M_cacheERKSt6localeEN11_Scoped_strC2ERKSs;
extern int r_ble_ll_hci_is_le_event_enabled;
extern int device_is_constructed;
extern int esp_wifi_ap_get_gtk_rekeying_config_internal;
extern int lv_obj_set_style_margin_right;
extern int mbedtls_mpi_core_write_le;
extern int xTaskDelayUntil;
extern int ieee80211_empty_txq;
extern int r_os_cputime_usecs_to_ticks;
extern int lv_obj_refresh_self_size;
extern int pthread_mutex_unlock;
extern int lv_tabview_set_active;
extern int wpa_cipher_key_len;
extern int auth_build_token_req;
extern int lv_obj_set_style_grid_cell_column_span;
extern int mac_tx_set_plcp0;
extern int wpa_gen_wpa_ie;
extern int crypto_ec_get_prime;
extern int ieee80211_ioctl_init;
extern int lv_buttonmatrix_create;
extern int ble_att_svr_rx_read;
extern int hal_set_itwt_pti;
extern int mbedtls_psa_cipher_abort;
extern int r_ble_lll_mmgmt_alloc;
extern int lv_timer_set_cb;
extern int mbedtls_psa_mac_verify_setup;
extern int rx_evm_data_rate_mcs_coefficient;
extern int coex_schm_bt_idle_wifi_conn;
extern int rgb_led_get_color;
extern int ieee80211_ioctl_process;
extern int __atomic_fetch_or_8;
extern int advFilter_stack_enableDupExcListVsCmd;
extern int bt_bb_tx_pwr_table_get;
extern int mbedtls_ccm_update_ad;
extern int r_ble_ll_conn_sm_get;
extern int etharp_cleanup_netif;
extern int gdma_ahb_hal_connect_mem;
extern int pm_send_wake_null_cb;
extern int ble_bb_cte_set_cte_ant_delay_rx_aoa_2us;
extern int netconn_new_with_proto_and_callback;
extern int ble_stack_initEnv;
extern int g_timer_info;
extern int lv_dropdown_get_list;
extern int esp_netif_remove_from_list_unsafe;
extern int usb_midi_device_get;
extern int mbedtls_hmac_drbg_update;
extern int localeconv;
extern int ble_att_clt_rx_find_type_value;
extern int ds_hal_configure_iv;
extern int lvgl_display_add;
extern int esp_partition_erase_range;
extern int __mulsf3;
extern int lv_obj_tree_walk;
extern int esp_cache_freeze_caches_disable_interrupts;
extern int esp_mbedtls_verify_certificate;
extern int lv_obj_send_event;
extern int hal_mac_color_clr_bitmap;
extern int cnx_auth_done;
extern int r_ble_lll_env_deinit;
extern int nimble_port_run;
extern int usb_midi_set_callback;
extern int r_ble_ll_hci_ev_send_scan_req_recv;
extern int _ZTTNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEE;
extern int lv_draw_line;
extern int rcUpSched;
extern int r_ble_ll_conn_sm_free;
extern int wpa_pmk_to_ptk;
extern int tcp_active_pcbs_changed;
extern int esp_mspi_32bit_address_flash_feature_check;
extern int ble_hs_hci_util_handle_pb_bc_join;
extern int lv_style_set_text_font;
extern int r_ble_ll_conn_cth_flow_set_buffers;
extern int ESP_EFUSE_RD_DIS_BLOCK_KEY5;
extern int os_timer_done;
extern int lv_obj_set_style_transform_pivot_x;
extern int wifi_ap_pmf_enabled;
extern int phy_get_rom_ver;
extern int esp_timer_init;
extern int esp_vfs_fat_rawflash_mount;
extern int esp_vfs_register_fs;
extern int pm_connectionless_wake_window_timeout_process;
extern int lv_screen_active;
extern int mbedtls_threading_psa_rngdata_mutex;
extern int mbedtls_ssl_write_certificate;
extern int _ZSt15__try_use_facetISt11__timepunctIwEEPKT_RKSt6locale;
extern int hal_he_set_co_hosted_bss;
extern int r_ble_vendor_hci_duplicate_exception_list_config;
extern int esp_rsa_ds_opaque_import_key;
extern int i2s_controller_set_rx_pdm_config;
extern int mbedtls_hmac_drbg_random_with_add;
extern int mbedtls_ssl_fetch_input;
extern int r_ble_ll_ctrl_proc_init;
extern int memp_NETBUF;
extern int wctob;
extern int coex_schm_ble_mesh_config_bt_default_wifi_connecting;
extern int _sin;
extern int uart_controller_write_bytes;
extern int http_header_generate_string;
extern int ip4_route;
extern int r_ble_ll_adv_set_enable;
extern int he_recv_action_twt_teardown;
extern int ble_log_env;
extern int pm_sleep_for;
extern int _ZGVNSt10moneypunctIcLb1EE2idE;
extern int r_ble_ll_sync_info_offset_get;
extern int test_rx_process_complete_retry;
extern int ble_hs_hci_cmd_tx;
extern int camera_get_width;
extern int fgetpos;
extern int ble_gap_unpair_oldest_peer;
extern int rtc_clk_cpu_freq_set_xtal;
extern int _ZZNKSt8__format15__formatter_strIcE6formatINS_10_Sink_iterIcEEEET_St17basic_string_viewIcSt11char_traitsIcEERSt20basic_format_contextIS5_cEENKUlvE_clEv;
extern int phy_tsens_read_init;
extern int r_ll_assert;
extern int pwr_hal_set_beacon_filter_broadcast_wakeup_enable;
extern int spi_device_queue_trans;
extern int ff_oem2uni;
extern int mbedtls_mpi_add_mpi;
extern int i2s_controller_read;
extern int lv_bin_decoder_info;
extern int i2c_master_transmit;
extern int igmp_lookfor_group;
extern int _ZGVNSt7__cxx117collateIcE2idE;
extern int esp_stdio_register;
extern int BcnInterval;
extern int esp_wifi_use_supp_pmk_cache;
extern int pow;
extern int ble_sm_alg_f5;
extern int key_mgr_hal_write_assist_info;
extern int lv_anim_path_linear;
extern int r_ble_ll_rand;
extern int hid_report_map_gamepad_len;
extern int g_rssi_threshold_failure;
extern int g_i2s;
extern int esp_wifi_set_sleep_wait_broadcast_data_time;
extern int ble_mqueue_put;
extern int httpd_sess_delete_invalid;
extern int pwm_set_period;
extern int esp_vfs_fat_sdcard_unmount;
extern int esp_elf_malloc;
extern int gdma_ahb_hal_reset;
extern int wifi_realloc;
extern int trc_onAmpduOp;
extern int pmksa_cache_auth_add;
extern int usb_msc_device_start;
extern int dbg_lmac_init;
extern int npl_freertos_event_is_queued;
extern int coex_schm_env;
extern int pmksa_cache_auth_add_entry;
extern int esp_mbedtls_mem_free;
extern int ieee80211_get_ptk;
extern int phy_freq_get_i2c_data;
extern int lv_style_set_shadow_width;
extern int r_ble_hw_get_cte_packet_rssi_from_buffer;
extern int psa_to_pk_ecdsa_errors;
extern int npl_freertos_eventq_remove;
extern int _ZSteqIcSt11char_traitsIcESaIcEEbRKNSt7__cxx1112basic_stringIT_T0_T1_EEPKS5_;
extern int r_ble_phy_module_deinit;
extern int esp_sta_reset_rmac_process;
extern int esp_mbedtls_mem_calloc;
extern int r_ble_lll_adv_alloc_periodic_memory;
extern int _Z17ble_publish_eventP6Device7BtEvent;
extern int esp_wifi_sta_get_pairwise_cipher_internal;
extern int __popcountdi2;
extern int ble_hs_flow_track_data_mbuf;
extern int modem_clock_hal_deselect_all_ble_rtc_timer_lpclk_source;
extern int gpio_hal_intr_enable_on_core;
extern int r_ble_ll_conn_hci_le_ltk_neg_reply;
extern int lv_event_stop_processing;
extern int device_remove;
extern int mbedtls_ecp_group_free;
extern int lv_anim_set_values;
extern int httpd_req_new;
extern int mbedtls_mpi_lsb;
extern int esp_netif_netstack_buf_free;
extern int mbedtls_psa_mac_compute;
extern int uECC_vli_modInv;
extern int r_ble_lll_per_adv_coex_dpc_calc_pti_update_itvl;
extern int vSystimerSetup;
extern int esp_wifi_get_protocol;
extern int ieee802154_txon_delay_set;
extern int ble_hs_adv_parse;
extern int mbedtls_ssl_write;
extern int esp_log_vprint_func;
extern int strstr;
extern int phy_get_i2c_hostid_;
extern int g_wpa_supp;
extern int uart_controller_read_bytes;
extern int lv_obj_set_y;
extern int phy_ant_update;
extern int r_ble_ll_conn_calc_dci_csa1;
extern int _Z15tusbIsSupportedv;
extern int igmp_leavegroup;
extern int fstat;
extern int scan_get_owe_channel_info;
extern int bt_bb_set_corr_thresh_le;
extern int lv_group_add_obj;
extern int lmacRequestTxopQueue;
extern int app_event_emit;
extern int he_get_min_subframe_len_dcm;
extern int httpd_sess_delete;
extern int r_ble_ll_conn_process_conn_params;
extern int phy_bt_tx_pwctrl_init;
extern int heap_caps_enable_nonos_stack_heaps;
extern int ble_sm_alg_gen_dhkey;
extern int lv_event_pop;
extern int r_ble_lll_conn_txbuf_insert_after;
extern int g_conn_stack_envP;
extern int xPortStartScheduler;
extern int module_construct;
extern int r_ble_ll_df_conn_cte_rx_set_param;
extern int spicommon_dma_setup_priv_buffer;
extern int g_mmu_mem_regions;
extern int esp_intr_alloc;
extern int _ZSt14__convert_to_vIeEvPKcRT_RSt12_Ios_IostateRKPi;
extern int atan2;
extern int mpi_hal_calc_hardware_words;
extern int _ZSt14__convert_to_vIfEvPKcRT_RSt12_Ios_IostateRKPi;
extern int lv_menu_separator_class;
extern int psa_export_key_internal;
extern int lv_anim_set_duration;
extern int g_debug_le_private_key_ro;
extern int lv_color16_premultiply;
extern int cnx_sta_leave;
extern int mbedtls_md;
extern int r_ble_phy_module_init;
extern int _ZSt9use_facetISt10moneypunctIwLb1EEERKT_RKSt6locale;
extern int include_esp_phy_override;
extern int cnx_can_do_obss_scan;
extern int coex_hw_timer_tick_get;
extern int s_table;
extern int r_ble_ll_get_our_devaddr;
extern int wDev_ProcessFiq;
extern int memspi_host_read_data_slicer;
extern int conn_ll_getSuppMaxTxOctets;
extern int esp_http_client_set_auth_data;
extern int bt_bb_v2_version;
extern int ieee80211_add_he_mu_edca_parameter_set;
extern int esp_transport_ssl_enable_global_ca_store;
extern int _Z24app_metadata_is_valid_idRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int lv_theme_apply;
extern int aes_hal_wait_done;
extern int xRingbufferGetStaticBuffer;
extern int lmacProcessTxRtsError;
extern int ble_sm_ia_ra;
extern int lv_draw_sw_fill;
extern int esp_coex_common_spin_lock_create_wrapper;
extern int esp_transport_ssl_set_client_key_config;
extern int i2s_channel_init_tdm_mode;
extern int g_txop_queue_status;
extern int lv_obj_set_style_grid_column_dsc_array;
extern int task_event_group_release_bit;
extern int npl_freertos_eventq_get;
extern int mbedtls_hmac_drbg_reseed;
extern int lv_obj_refr_size;
extern int uart_hal_set_tx_idle_num;
extern int phy_i2c_paral_write;
extern int r_ble_ll_adv_rpa_update;
extern int r_ble_lll_adv_alloc_rxbuf;
extern int pbuf_put_at;
extern int coex_schm_register_callback;
extern int esp_phy_modem_deinit;
extern int esp_hmac_abort_transparent;
extern int _ZTv0_n12_NSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEED0Ev;
extern int _ZTVSt10moneypunctIcLb1EE;
extern int phy_rxiq_cover_mg_mp;
extern int sta_recv_mgmt;
extern int ble_gap_subscribe_event;
extern int mspi_timing_flash_tuning;
extern int http_header_set_format;
extern int r_ble_lll_alternative_tx_buffer_set;
extern int tcp_rst_netif;
extern int lv_area_get_size;
extern int lmacIsLongFrame;
extern int hex2byte;
extern int http_parser_execute;
extern int __find_charset;
extern int mbedtls_pk_set_pubkey_from_prv;
extern int mbedtls_psa_cipher_encrypt;
extern int lv_image_decoder_get_area;
extern int r_ble_ll_whitelist_deleted_irk_list_clear;
extern int scan_update_scan_history;
extern int coex_schm_ble_mesh_config_bt_default_wifi_conn;
extern int esp_wifi_sta_report_bsscolor_collision;
extern int device_get_child_count;
extern int lv_array_init;
extern int wDev_ftm_record_t1t4;
extern int lv_draw_task_get_border_dsc;
extern int ieee80211_setup_basic_htrates;
extern int ieee80211_add_htinfo_body;
extern int mbedtls_mpi_copy;
extern int r_ble_phy_get_max_txpwr_dbm;
extern int coex_schm_ble_mesh_standby_bt_default_wifi_conn;
extern int rsn_set_snonce_cookie;
extern int netif_ip6_addr_set_parts;
extern int window_manager_get_state;
extern int ble_single_clear_single_step;
extern int wpa_supplicant_key_neg_complete;
extern int g_esp_wifi_cmw500_stbc_rx_test_enabled;
extern int esp_http_client_set_timeout_ms;
extern int sdmmc_init_csd;
extern int r_ble_lll_scan_reset_link_state;
extern int phy_pbus_force_test;
extern int lv_obj_set_style_pad_top;
extern int hci_stack_setVsEventMask;
extern int esp_wifi_register_wpa_cb_internal;
extern int __log2pow5;
extern int phy_get_temp_init;
extern int mbedtls_ssl_handshake;
extern int lv_draw_sw_mask_init;
extern int lv_checkbox_set_text;
extern int lv_refr_set_disp_refreshing;
extern int gdma_new_ahb_channel;
extern int _ZSt15__try_use_facetISt10moneypunctIcLb1EEEPKT_RKSt6locale;
extern int mbedtls_hmac_drbg_free;
extern int r_mem_init_mbuf_pool;
extern int mbedtls_md_hmac_reset;
extern int esp_flash_app_init;
extern int ble_hs_id_set_rnd;
extern int esp_flash_get_size;
extern int lv_cache_entry_acquire_data;
extern int lv_obj_check_type;
extern int tanhf;
extern int wdev_data_init;
extern int mbedtls_sha256_init;
extern int NAN_NETWORK_ID;
extern int sleep_system_peripheral_dummy;
extern int mumimo_spatial_cfg_get_nsts_tot;
extern int lv_draw_image_dsc_init;
extern int phy_bb_txpwr_init;
extern int EccPoint_mult;
extern int ieee80211_add_htinfo_vendor;
extern int mbedtls_ecp_grp_id_list;
extern int tc_aes128_set_encrypt_key;
extern int lv_display_set_flush_cb;
extern int adc2_wifi_acquire;
extern int esp_netif_attach_wifi_station;
extern int phy_tx_paon_set;
extern int rtc_clk_fast_src_set;
extern int esp32_wifi_pinned_driver;
extern int lv_vsnprintf;
extern int __ascii_mbtowc;
extern int _ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_i;
extern int psa_wipe_key_slot;
extern int uart_hal_get_symb_len;
extern int io_mux_enable_lp_io_clock;
extern int lvgl_ppa_rotate;
extern int lv_draw_sw_deinit;
extern int ble_capture_info_user_handler;
extern int ble_gattc_rx_read_type_adata;
extern int r_ble_ll_hci_vendor_unregister;
extern int uart_controller_get_config;
extern int i2s_output_gpio_reserve;
extern int __rem_pio2;
extern int spi_controller_try_lock;
extern int r_os_mbuf_extend;
extern int r_ble_ll_sync_create;
extern int esp_intr_alloc_intrstatus;
extern int lv_display_get_screen_prev;
extern int lv_obj_set_style_outline_color;
extern int esp_hmac_compute_transparent;
extern int mbedtls_psa_cipher_update;
extern int pau_hal_set_regdma_entry_link_addr;
extern int phy_nrx_freq_set;
extern int g_wpa_config_changed;
extern int usb_device_controller_is_cdc_enabled;
extern int r_ble_ll_hci_ev_rd_rem_ver;
extern int mbedtls_asn1_write_len;
extern int lv_anim_set_early_apply;
extern int mbedtls_mutex_free_ptr;
extern int crypto_ec_point_compute_y_sqr;
extern int rtc_clk_apb_freq_get;
extern int pwm_set_duty;
extern int rcGetRate;
extern int ble_sm_sc_random_exec;
extern int nd6_find_route;
extern int ieee80211_get_regdomain;
extern int r_ble_ll_hci_ev_hw_err;
extern int mbedtls_zeroize_and_free;
extern int mbedtls_ssl_conf_ciphersuites;
extern int esp_mpi_exp_mpi_mod_hw_op;
extern int _ZStmiRKSt15_Deque_iteratorISt6vectorIhSaIhEERS2_PS2_ES7_;
extern int i2c_controller_write;
extern int lodepng_palette_clear;
extern int nan_get_config_local;
extern int service_manager_find_manifest;
extern int httpd_sess_enum;
extern int esp_hmac_calculate;
extern int r_ble_ll_ctrl_update_features;
extern int lv_line_class;
extern int _ZGVNSt8time_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int esp_partition_is_flash_region_writable;
extern int adv_stack_getBroker;
extern int wifi_station_connect;
extern int usb_hid_device_get;
extern int coex_schm_status_bit_clear;
extern int g_mac_sleep_en;
extern int ip4_output_if_opt_src;
extern int ieee80211_output_raw_process;
extern int r_ble_ll_arr_common_factor_get;
extern int __eqtf2;
extern int r_ble_ll_conn_chk_phy_upd_start;
extern int mbedtls_ssl_reset_checksum;
extern int phy_txcal_work_mode;
extern int mbedtls_pk_free;
extern int r_ble_ll_sync_init;
extern int memory_realloc_with_policy;
extern int _ZTv0_n12_NSiD0Ev;
extern int tlsf_size;
extern int httpd_stop;
extern int esp_wifi_internal_on_coex_schm_phase;
extern int _ZSt13get_terminatev;
extern int esp_test_tx_count_collision;
extern int pm_clear_wakeup_signal;
extern int coex_schm_status_bitmap_get;
extern int lv_spangroup_get_expand_height;
extern int wifi_nvs_commit;
extern int ble_l2cap_chan_alloc;
extern int r_ble_lll_adv_remove;
extern int global_hapd;
extern int bt_bb_tx_cca_fifo_reset;
extern int nd6_restart_netif;
extern int ble_gatts_conn_can_alloc;
extern int esp_rsa_ds_opaque_size_function;
extern int pvt_auto_dbias_init;
extern int r_ble_ll_hci_ev_conn_iq_report;
extern int ieee80211_vht_get_vhtflags;
extern int gdma_hal_set_weight;
extern int xTaskGetTickCountFromISR;
extern int lv_obj_set_style_line_color;
extern int lv_display_dpx;
extern int ieee80211_parse_spatial_reuse;
extern int esp_vfs_include_console_register;
extern int ieee80211_output_pending_eb;
extern int r__os_mbuf_copypkthdr;
extern int mbedtls_mpi_safe_cond_assign;
extern int wifi_station_disconnect;
extern int vPortYield;
extern int xEventGroupSetBits;
extern int r_os_mbuf_prepend_pullup;
extern int lv_obj_get_child;
extern int lv_text_encoded_next;
extern int lv_obj_scroll_by;
extern int wpa_get_key;
extern int esp_phy_enable;
extern int pm_twt_disallow_tx;
extern int gpio_controller_get_pin_count;
extern int _ZGVNSt7__cxx117collateIwE2idE;
extern int sae_group_allowed;
extern int ieee80211_parse_beacon;
extern int chm_end_op;
extern int ip6_select_source_address;
extern int lmacReachShortLimit;
extern int esp_crypto_ecc_lock_release;
extern int mbedtls_ssl_conf_own_cert;
extern int esp_crypto_mpi_lock_acquire;
extern int bt_agc_rssi_thresh;
extern int esp_test_disable_tx_statistics;
extern int app_scan_params;
extern int he_recv_action_twt_setup;
extern int r_os_mbuf_pool_init;
extern int psa_copy_key_material_into_slot;
extern int r_ble_lll_scan_recycle_buffer;
extern int esp_wifi_ap_get_prof_ap_ssid_internal;
extern int lv_color_eq;
extern int _ZThn8_NSdD0Ev;
extern int sar_periph_ctrl_adc_oneshot_power_release;
extern int ieee80211_sta_is_connected;
extern int ieee80211_freedom_deinit;
extern int r_os_mbuf_get;
extern int r_ble_ll_ctrl_rx_power_change_ind;
extern int wmemcpy;
extern int wlanif_init;
extern int wifi_set_config_process;
extern int esp_ecdsa_transparent_verify_hash_complete;
extern int preferences_has_int64;
extern int r_ble_lll_sync_coex_dpc_process;
extern int r_ble_hci_trans_ll_acl_tx;
extern int wpa_set_profile;
extern int lv_strlcpy;
extern int ftm_resp_ctx;
extern int wpa_sm_notify_disassoc;
extern int r_ble_ll_sync_transfer_get;
extern int phy_chip_i2c_writeReg;
extern int cnx_sta_scan_cmd;
extern int r_exception_list_ble_mesh_addr_table_is_empty;
extern int lv_obj_set_style_border_width;
extern int power_supply_get_property;
extern int lv_obj_set_style_margin_bottom;
extern int conn_stack_disable;
extern int _ZSt8to_charsPcS_dSt12chars_formati;
extern int wifi_nvs_validate_lr_info;
extern int esp_http_client_get_password;
extern int ble_svc_gap_init;
extern int r_ble_lll_adv_aux_pdu_make;
extern int sta_set_rmac_restart;
extern int coex_wifi_channel_set;
extern int esp_transport_write;
extern int r_ble_lll_sync_set_scan_link_state;
extern int wifi_sta_get_prof_password;
extern int esp_wifi_internal_reg_rxcb;
extern int mbedtls_ccm_starts;
extern int PWM_TYPE;
extern int spi_flash_hal_gpspi_configure_host_io_mode;
extern int lv_obj_set_style_flex_track_place;
extern int psa_key_derivation_input_bytes;
extern int esp_http_client_set_password;
extern int r_ble_ll_scan_whitelist_enabled;
extern int hal_disable_tsf_timer_wakeup;
extern int ble_hid_set_conn_handle;
extern int r_ble_ll_adv_deinit;
extern int _lock_try_acquire;
extern int wifi_is_stop_in_progress;
extern int i2c_driver_delete;
extern int ap_get_sta;
extern int r_ble_ll_scan_end_adv_evt;
extern int omac1_aes_vector;
extern int __wtime_wday;
extern int ppMapTxQueue;
extern int usb_hid_device_set_name;
extern int ble_sm_alg_aes_cmac;
extern int phy_get_chan_cap;
extern int hostap_recv_mgmt;
extern int uart_driver_delete;
extern int spi_flash_chip_generic_reset;
extern int tcpip_callback;
extern int pm_set_bss_max_idle;
extern int uECC_vli_testBit;
extern int esp_rsa_ds_opaque_set_session_timeout;
extern int phy_set_rx_sense;
extern int pwr_hal_set_mac_modem_tbtt_auto_period_interval;
extern int r_ble_ll_ctrl_rx_feature_rsp;
extern int esp_http_client_get_status_code;
extern int r_ble_ll_resolv_clear_all_adi_list_bit;
extern int ledc_timer_rst;
extern int lv_init;
extern int phy_ant_wifitx_cfg;
extern int gpio_hal_iomux_in;
extern int mbedtls_mpi_core_exp_mod_working_limbs;
extern int lv_arc_set_change_rate;
extern int spi_bus_lock_unregister_dev;
extern int r_ble_hw_get_cte_packet_freq_from_buffer;
extern int esp_io_expander_gpio_wrapper_get_level;
extern int wifi_sta_change_mac_internal;
extern int ESP_EFUSE_WR_DIS_BLOCK_KEY2;
extern int ble_att_svr_rx_signed_write;
extern int ds_hal_write_private_key_params;
extern int regdma_link_get_owner_bitmap;
extern int r_ble_lll_alternative_tx_buffer_get;
extern int ble_gattc_connection_broken;
extern int gpio_pullup_dis;
extern int wpa_use_aes_key_wrap;
extern int spi_flash_hal_resume;
extern int ap_try_sa_query_process;
extern int ble_store_read_cccd;
extern int ble_transport_ll_init;
extern int r_ble_ll_conn_process_tx_power_releated;
extern int dhcp_stop;
extern int esp_netif_set_mac_api;
extern int nimble_mem_calloc;
extern int f_open;
extern int r_ble_ll_scan_env_deinit;
extern int pm_set_sleep_wait_broadcast_data_time;
extern int nan_send_disc_bcn_process;
extern int nd6_get_next_hop_addr_or_queue;
extern int lv_spangroup_get_expand_width;
extern int r_ble_ll_conn_hci_pcl_rssi_threshold;
extern int _i2c_hal_set_bus_timing;
extern int s_itwt_resume_flow_id_bitmap;
extern int esp_netif_get_old_ip_info;
extern int sdmmc_send_cmd_set_blocklen;
extern int pmu_hal_hp_set_modem_active_backup_enable;
extern int cnx_csa_fn_process;
extern int lv_free_core;
extern int ant_tx_cfg;
extern int s_head;
extern int esp_aes_free;
extern int console_fcntl;
extern int lv_event_get_draw_task;
extern int lv_color24_luminance;
extern int device_get_driver_data;
extern int r_ble_ll_sync_list_remove;
extern int _ZSt14__add_groupingIwEPT_S1_S0_PKcjPKS0_S5_;
extern int esp_wifi_build_rsnxe;
extern int ble_sm_alg_c1;
extern int mbedtls_ccm_update;
extern int vfscanf;
extern int pmu_hal_hp_set_sleep_active_backup_enable;
extern int esp_intr_alloc_intrstatus_bind;
extern int audio_stream_is_supported;
extern int key_mgr_hal_set_key_generator_mode;
extern int gpio_hog_driver;
extern int cnx_node_remove;
extern int esp_mpi_mult_mpi_failover_mod_mult_hw_op;
extern int ieee80211_rsn_cipher_priority;
extern int phy_band_reg;
extern int empty_func_p_void_r_bool;
extern int ble_hs_hci_get_hci_supported_cmd;
extern int lv_style_set_pad_bottom;
extern int ble_gap_init;
extern int i2s_channel_preload_data;
extern int rutype2str;
extern int cnx_clear_blacklist;
extern int r_ble_lll_mmgmt_free_sch_chain;
extern int mac_list_lock;
extern int crypt_decrypt;
extern int display_swap_xy;
extern int ble_att_svr_register;
extern int ieee80211_post_hmac_tx;
extern int sntp_set_time_sync_notification_cb;
extern int ble_gatts_add_svcs;
extern int _Z25ble_start_advertising_hidP6Devicet;
extern int ungetc;
extern int esp_netif_get_event_id;
extern int cte_sample_select_index;
extern int hal_he_disable_rts_threshold;
extern int netconn_join_leave_group_netif;
extern int regdma_link_new_branch_continuous_default;
extern int tlsf_add_pool;
extern int spi_flash_chip_gd_probe;
extern int temperature_sensor_power_release;
extern int coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_conn;
extern int r_ble_ll_resolv_list_clr;
extern int ftell;
extern int coex_schm_curr_period_get;
extern int spacing2str;
extern int aes_encrypt_deinit;
extern int app_event_poll;
extern int is_use_muedca;
extern int app_main;
extern int http_header_init;
extern int hal_set_beamf_mt_pti;
extern int lvgl_get_shared_icon_font;
extern int phy_get_xtal_freq;
extern int acd_network_changed_link_down;
extern int ble_hs_init;
extern int ble_hs_hci_util_data_hdr_strip;
extern int multi_heap_malloc_impl;
extern int ieee80211_is_40mhz_valid_bw;
extern int lmacPostTxComplete;
extern int esp_vfs_fcntl_r;
extern int _ZTVNSt8__format14_Fixedbuf_sinkIcEE;
extern int read_hw_noisefloor;
extern int mbedtls_ssl_get_ciphersuite_sig_pk_alg;
extern int lv_label_set_text_fmt;
extern int _ZTVSt23_Sp_counted_ptr_inplaceIN2tt3app5files5StateESaIvELN9__gnu_cxx12_Lock_policyE1EE;
extern int aes_encrypt;
extern int r_ble_ll_conn_req_peer_sca;
extern int coex_schm_ble_mesh_standby_bt_conn_wifi_connecting;
extern int adv_stack_enable;
extern int esp_register_ext_funcs;
extern int wifi_ap_sta_sae_auth_done_process;
extern int mbedtls_pk_parse_public_key;
extern int gdma_disconnect;
extern int coex_bt_request;
extern int lv_menu_sidebar_cont_class;
extern int r_ble_hw_resolv_list_search;
extern int resolve_symbol;
extern int lv_indev_set_type;
extern int ble_att_svr_deinit;
extern int phy_rfcal_data_sub;
extern int wifi_get_radio_state;
extern int mbedtls_ssl_write_sig_alg_ext;
extern int mbedtls_net_recv;
extern int esp_mbedtls_handshake;
extern int lv_utils_bsearch;
extern int lv_arc_set_value;
extern int rtc_time_us_to_slowclk;
extern int chm_get_current_channel;
extern int esp_log_impl_lock;
extern int scan_start_handler;
extern int esp_sha_dma_start;
extern int ieee80211_setup_htrates;
extern int multi_heap_get_allocated_size_impl;
extern int ble_bb_cte_set_cte_ant_switch_delay_rx_aod_1us_2m;
extern int chip_enable;
extern int sdmmc_init_sd_if_cond;
extern int file_system_get_path;
extern int gpio_sleep_sel_en;
extern int r_ble_lll_sync_event_end;
extern int atanf;
extern int atoi;
extern int mbedtls_rsa_public;
extern int sha_hal_sha512_init_hash;
extern int get_fattime;
extern int ieee80211_sta_parse_he_color_change_announcent;
extern int esp_tls_get_and_clear_last_error;
extern int mbedtls_x509_crt_check_key_usage;
extern int mbedtls_ssl_parse_alpn_ext;
extern int r_ble_lll_adv_update_did;
extern int ble_hw_env_p;
extern int __file_str_put_alloc;
extern int lv_image_decoder_init;
extern int ieee80211_ampdu_age_all;
extern int lv_area_set_height;
extern int psa_to_pk_rsa_errors;
extern int ble_hs_id_copy_addr;
extern int g_wdev_csi_rx_ctx;
extern int dns_mquery_v4group;
extern int strpbrk;
extern int g_authmode_incompatible;
extern int _ZTv0_n12_NSt14basic_ofstreamIcSt11char_traitsIcEED1Ev;
extern int __rem_pio2f;
extern int r_ble_ll_ctrl_enc_req_make;
extern int service_manager_stop;
extern int mbedtls_mpi_inv_mod_odd;
extern int tcp_rst;
extern int chm_get_chan_info;
extern int device_get_type;
extern int readdir;
extern int _ZZNSt8__format19_Formatting_scannerINS_10_Sink_iterIcEEcE13_M_format_argEjENKUlRT_E_clIcEEDaS5_;
extern int r_ble_ll_scan_resume_from_init;
extern int r_ble_lll_conn_decrypt_cipher_data;
extern int r_ble_hw_driver_deinit;
extern int xQueueGenericSendFromISR;
extern int r_ble_lll_adv_invalid_hci_memory_free;
extern int esp_ble_hw_get_static_addr;
extern int esp_http_client_is_chunked_response;
extern int esp_timer_start_once;
extern int dhcps_set_new_lease_cb;
extern int lv_dropdown_clear_options;
extern int ieee80211_phy_deinit;
extern int _ZGVNSt10moneypunctIwLb1EE2idE;
extern int r_ble_ll_ctrl_proc_stop;
extern int esp_wifi_get_config_sae_pwe_h2e_internal;
extern int lv_menu_main_cont_class;
extern int hal_init_tb_power;
extern int set_gauss_coeff_2m;
extern int ble_att_svr_init;
extern int adc_hal_set_controller;
extern int esp_btbb_disable;
extern int mbedtls_rsa_get_len;
extern int pthread_symbols;
extern int npl_freertos_eventq_init;
extern int mac_tx_set_txop_q;
extern int lv_color_to_32;
extern int esp_vfs_fat_sdspi_sdcard_init;
extern int TRC_PER_IS_GOOD;
extern int lv_label_get_long_mode;
extern int psa_purge_key;
extern int esp_http_client_get_url;
extern int phy_bb_fsm_rst;
extern int itwt_probe_timeout_fn;
extern int hal_enable_sta_tsf;
extern int lv_slider_set_range;
extern int r_ble_lll_df_cte_set_pattern_config;
extern int r_ble_hw_cte_rx_pkt_state;
extern int lv_image_decoder_set_get_area_cb;
extern int lv_draw_sw_blend;
extern int lv_indev_get_state;
extern int rtc_clk_cpu_set_to_default_config;
extern int bt_bb_rx_dpo_set;
extern int r_ble_hw_cte_ring_buffer_index_clear;
extern int r_ble_ll_utils_calc_dci_csa2;
extern int pm_wake_up;
extern int phy_freq_i2c_mem_write;
extern int lv_group_delete;
extern int ble_store_write;
extern int r_ble_ll_ctrl_rx_conn_param_rsp;
extern int _lock_release_recursive;
extern int rom_flash_chip_dummy;
extern int ff_diskio_get_sector_size;
extern int err_to_errno;
extern int ledc_hal_set_duty_start;
extern int mbedtls_ssl_start_handshake_msg;
extern int wifi_rf_phy_disable;
extern int lv_arc_get_bg_angle_start;
extern int r_ble_ll_conn_cth_flow_process_cmd;
extern int ble_att_svr_rx_write_no_rsp;
extern int _Z26ble_midi_init_gatt_handlesP6Device;
extern int lv_indev_delete;
extern int xTaskRemoveFromEventList;
extern int coex_schm_bt_a2dp_paused_wifi_connecting;
extern int npl_freertos_mempool_init;
extern int temperature_sensor_hal_init;
extern int i2c_controller_write_register_array;
extern int lv_image_cache_init;
extern int net80211_funcs_init;
extern int mbedtls_ssl_setup;
extern int modem_clock_domain_icg_config;
extern int usb_midi_device_stop;
extern int phy_pwdet_sar2_init;
extern int wifi_destroy_sta;
extern int esp_elf_deinit;
extern int lv_display_get_layer_bottom;
extern int esp32_ble_serial_driver;
extern int r_ble_ll_ctrl_version_ind_make;
extern int phy_rxevm_init_cfg;
extern int hostap_handle_timer_process;
extern int power_supply_supports_quick_charge;
extern int ip6_output_if;
extern int ble_att_truncate_to_mtu;
extern int r_ble_lll_mmgmt_free_sch;
extern int ble_hs_hci_acl_tx_now;
extern int uECC_get_rng;
extern int esp_panic_handler_increment_entry_count;
extern int r_ble_lll_adv_last_txbuf_memory_size_calculate;
extern int r_ble_ll_df_deinit;
extern int conn_stack_deinitEnv;
extern int uxTaskPriorityGetFromISR;
extern int r_ble_lll_scan_deinit;
extern int ic_get_he_rts_threshold_bytes;
extern int r_ble_ll_conn_master_init;
extern int esp_http_client_get_post_field;
extern int r_ble_lll_scan_sched_large_aux_offset;
extern int ble_att_inc_tx_stat;
extern int esp_md5_hash_update;
extern int rtcio_hal_iomux_input;
extern int _ZTSNSt8ios_base7failureB5cxx11E;
extern int r_ble_lll_sched_update_sched_offset;
extern int lv_layout_deinit;
extern int esp32_spi_deselect_all_cs;
extern int wifi_ap_remove_sta_node_process;
extern int r_ble_lll_conn_calc_first_anchor_point;
extern int ieee80211_add_dsparams;
extern int lv_obj_add_flag;
extern int lv_obj_get_local_style_prop;
extern int gdma_config_transfer;
extern int lv_roller_set_options;
extern int sdmmc_get_status;
extern int device_get_parent;
extern int esp32_i2c_driver;
extern int spi_flash_enable_interrupts_caches_and_other_cpu;
extern int mbedtls_ecp_read_key;
extern int wpa_cb;
extern int wpa_cipher_to_suite;
extern int memp_IGMP_GROUP;
extern int r_ble_ll_sync_put_syncinfo;
extern int r_ble_phy_stats_reset;
extern int phy_init_clk_set;
extern int ip_input;
extern int ble_gap_master_connect_reattempt;
extern int frexpl;
extern int _ZGVNSt7__cxx118time_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE;
extern int wpa_deattach;
extern int hal_mac_workaround_vht_rx_no_ack;
extern int pbuf_clen;
extern int ieee80211_crypto_attach;
extern int ip6addr_aton;
extern int hal_enable_sta_btwt_tsf;
extern int r_ble_ll_conn_hci_rd_chan_map;
extern int lv_draw_buf_width_to_stride_ex;
extern int r_ble_ll_ctrl_conn_param_pdu_proc;
extern int mbedtls_x509_get_time;
extern int crypto_bignum_div;
extern int pm_extend_tbtt_adaptive_start;
extern int r_ble_ll_hci_env_deinit;
extern int r_ble_lll_sched_pop_executed_sch;
extern int soc_get_available_memory_regions;
extern int mbedtls_ssl_sig_from_pk;
extern int sys_sem_signal;
extern int r_ble_ll_hci_send_adv_report;
extern int strtoll;
extern int sae_check_confirm;
extern int r_ble_hw_cte_get_sample_ring_buffer_ptr;
extern int lv_obj_set_style_shadow_width;
extern int phy_vco_init_cal;
extern int audio_codec_get_native_sample_rate;
extern int r_ble_ll_conn_hci_chk_conn_params;
extern int lv_palette_main;
extern int esp_hmac_abort_opaque;
extern int lv_draw_sw_image;
extern int tkip_encap;
extern int f_sync;
extern int lv_buttonmatrix_get_map;
extern int psa_reserve_free_key_slot;
extern int r_ble_lll_sleep_reset;
extern int spi_hal_setup_trans;
extern int lv_textarea_clear_selection;
extern int ieee80211_psq_find_max_bss;
extern int getc;
extern int ble_store_config_vars;
extern int r__os_msys_find_pool;
extern int r_ble_lll_adv_sched_priority_adjust;
extern int npl_freertos_hw_exit_critical;
extern int device_lock;
extern int sta_is_bss_max_idle_enabled;
extern int r_ble_ll_adv_sm_stop_timeout;
extern int ieee80211_freedom_inside_cb;
extern int lv_obj_get_y;
extern int pbuf_take_at;
extern int xEventGroupSync;
extern int os_reltime_expired;
extern int esp_transport_ssl_set_alpn_protocol;
extern int ppTxFragmentProc;
extern int r_ble_ll_adv_vendor_hci_legacy_adv_clear;
extern int esp_libc_include_getentropy_impl;
extern int phy_i2c_correct;
extern int lv_obj_set_flex_flow;
extern int esp_event_handler_instance_register_with;
extern int r_ble_ll_resolv_clear_all_pl_bit;
extern int phy_set_most_tpw_new;
extern int driver_remove;
extern int r_os_mbuf_append;
extern int __negsf2;
extern int spicommon_dma_desc_setup_link;
extern int ble_att_clt_rx_read;
extern int nvs_get_u32;
extern int mbedtls_psa_ecp_export_public_key;
extern int lmacReleaseTxopQueue;
extern int cnx_handshake_timeout_process;
extern int lv_draw_buf_premultiply;
extern int esp_now_set_pmk;
extern int os_timer_arm_us;
extern int spi_flash_chip_generic_set_io_mode;
extern int coex_schm_bt_piscan_wifi_scan;
extern int ESP_EFUSE_KEY_PURPOSE_0;
extern int ieee80211_wme_updateparams;
extern int lv_checkbox_set_text_static;
extern int esp_mspi_pin_init;
extern int esp_wifi_internal_issue_disconnect;
extern int ble_hs_log_mbuf;
extern int wcscpy;
extern int wDev_Get_Next_TBTT;
extern int wifi_station_get_target_ssid;
extern int ble_hs_id_infer_auto;
extern int ip4_frag;
extern int phy_wait_rfpll_cal_end_new;
extern int heap_caps_malloc_prefer;
extern int grove_get_mode;
extern int esp_mspi_get_io;
extern int ppSearchTxframe;
extern int esp_restart_noos;
extern int cos;
extern int mbedtls_psa_mac_abort;
extern int r_ble_ll_scan_get_addr_data_from_legacy;
extern int r_ble_ll_mem_generic_data_deinit;
extern int xEventGroupClearBits;
extern int xpt2046_softspi_driver;
extern int sta_input;
extern int lv_style_set_radius;
extern int sdmmc_init_mmc_decode_cid;
extern int root_driver;
extern int r_ble_ll_hci_cmd_proc;
extern int phy_get_tx_gain_value;
extern int esp_set_assoc_ie;
extern int esp_mmu_map;
extern int __powisf2;
extern int r_ble_lll_recycle_sch_in_task;
extern int rcGet11ACHighestRateIdx;
extern int uECC_vli_nativeToBytes;
extern int phy_bt_set_tx_gain_new;
extern int lv_point_transform;
extern int ieee80211_parse_action;
extern int esp_cache_err_clear_active_err;
extern int _ZTSSt9exception;
extern int tcp_eff_send_mss_netif;
extern int esp_transport_set_async_connect_func;
extern int lv_dropdown_create;
extern int pm_disable_beacon_monitor_timer;
extern int spi_device_get_actual_freq;
extern int lv_anim_set_path_cb;
extern int spi_flash_chip_generic_set_write_protect;
extern int hostap_add_in_mac_list;
extern int ble_uuid_flat;
extern int r_ble_lll_per_adv_coex_dpc_update;
extern int ble_att_svr_rx_prep_write;
extern int lv_text_is_cmd;
extern int esp_supplicant_common_init;
extern int ip6_chksum_pseudo;
extern int wDev_SnifferRxAmpdu;
extern int mbedtls_ssl_check_cert_usage;
extern int dhcps_stop;
extern int phy_ant_btrx_cfg;
extern int pm_coex_update_wifi_time_slice_state;
extern int wifi_deinit_process;
extern int ledc_hal_init;
extern int device_get_first_by_compatible;
extern int r_ble_ll_set_default_sync_transfer_params;
extern int advFilter_hci_controlDuplicate;
extern int _ZTVN9__gnu_cxx24__concurrence_lock_errorE;
extern int httpd_unrecv;
extern int esp_transport_set_func;
extern int r_ble_lll_adv_update_random_addr;
extern int hal_mac_set_csi_cbw;
extern int sdmmc_init_sd_bus_width;
extern int esp_system_get_time;
extern int lv_draw_label_dsc_init;
extern int phy_i2c_rc_cal_set;
extern int lv_bar_set_value;
extern int esp_event_handler_register_with_internal;
extern int esp_wifi_get_wps_status_internal;
extern int mbedtls_mpi_core_from_mont_rep;
extern int ble_uuid_init_from_att_buf;
extern int phy_disable_agc;
extern int r_ble_hw_periodiclist_add;
extern int r_ble_ll_conn_cth_flow_enable;
extern int esp_wifi_register_mgmt_frame_internal;
extern int r_filter_duplicate_hash_ring_list_add;
extern int r_os_msys_register;
extern int lv_buttonmatrix_get_one_checked;
extern int gpio_func_sel;
extern int pm_allow_to_enable_bcn_filter;
extern int eb_txdesc_space;
extern int tcpip_api_call;
extern int display_disp_sleep;
extern int display_draw_bitmap;
extern int uECC_vli_isZero;
extern int sdmmc_init_spi_crc;
extern int _ZTV11WL_Ext_Perf;
extern int r_esp_ble_ll_set_public_addr;
extern int ble_hs_hci_get_hci_version;
extern int ndp_rxcb;
extern int isspace;
extern int ble_hs_pvcy_set_resolve_enabled;
extern int sdmmc_sd_get_erase_timeout_ms;
extern int lv_obj_set_style_flex_flow;
extern int lv_draw_buf_init;
extern int xswap;
extern int netbuf_alloc;
extern int i2s_hal_calc_mclk_precise_division;
extern int r_ble_lll_adv_recycle_periodic_event_end;
extern int ble_midi_get_conn_handle;
extern int mbedtls_aes_setkey_dec;
extern int esp_libc_timekeeping_adjtime_get_remaining_us;
extern int phy_tsens_temp_read;
extern int wpa_akm_to_suite;
extern int r_ble_ll_conn_hci_le_rd_phy;
extern int ieee80211_psq_init;
extern int r_ble_lll_sched_delete_by_sm_num;
extern int r_ble_lll_mmgmt_rxbuffer_reuse_unapply_set;
extern int lv_obj_get_style_opa_recursive;
extern int multi_heap_realloc;
extern int lv_obj_set_style_pad_column;
extern int task_event_group_wait_any;
extern int ble_hs_wakeup_tx;
extern int lv_cache_entry_get_data;
extern int esp_flash_chip_gd;
extern int ic_get_random;
extern int lv_event_get_target;
extern int pbuf_chain;
extern int mbedtls_ssl_config_init;
extern int i2c_master_receive;
extern int pbuf_free;
extern int frexp;
extern int lv_draw_sw_grad_get;
extern int _ZTTSi;
extern int r_os_mbuf_cmpf;
extern int hal_init_tb_ru_power;
extern int psa_free_persistent_key_data;
extern int ic_disable_crypto;
extern int _ZdlPvj;
extern int srand;
extern int adv_hci_sendScanReqRxdVsEvent;
extern int current_task_is_wifi_task;
extern int lv_obj_calculate_style_text_align;
extern int wifi_crypto_init;
extern int r_ble_hw_whitelist_update;
extern int psa_cipher_abort;
extern int rc_get_trc_default;
extern int lv_obj_scroll_to_view_recursive;
extern int hal_set_rx_active_pti;
extern int ledc_hal_ls_channel_update;
extern int regdma_link_new_addr_map;
extern int crypto_bignum_is_one;
extern int ppRemoveHEAMPDUflags;
extern int wpa_init;
extern int r_ble_ll_conn_hci_set_chan_class;
extern int pwr_hal_set_mac_modem_state_wakeup_protect_disable;
extern int esp_mbedtls_get_bytes_avail;
extern int spicommon_bus_free_io_cfg;
extern int r_ble_ll_tx_pkt_in;
extern int g_sa_query_mac_list;
extern int __ctzdi2;
extern int vTaskDelay;
extern int esp_wifi_sta_prof_is_wpa_internal;
extern int esp_transport_destroy_foundation_transport;
extern int _ZSt4sortIN9__gnu_cxx17__normal_iteratorIPtSt6vectorItSaItEEEEEvT_S7_;
extern int memspi_host_init_pointers;
extern int ble_sm_proc_can_advance;
extern int lv_anim_core_deinit;
extern int lv_bar_is_symmetrical;
extern int r_ble_ll_sync_enabled;
extern int r_filter_duplicate_data_base_init;
extern int pm_beacon_offset_funcs_empty_init;
extern int lv_image_set_rotation;
extern int http_should_keep_alive;
extern int coex_schm_ble_mesh_standby_bt_default_wifi_scan;
extern int heap_caps_malloc_default;
extern int sdmmc_send_cmd_num_of_written_blocks;
extern int r_ble_scan_advertise_filter_env_init;
extern int ble_hs_pvcy_add_entry;
extern int r_ble_ll_ctrl_chk_proc_start;
extern int lv_buttonmatrix_has_button_ctrl;
extern int __cxa_rethrow;
extern int lv_ll_get_prev;
extern int lv_memset;
extern int bluetooth_stop_advertising;
extern int esp_log_default_level;
extern int ieee80211_psq_is_buff_pkt;
extern int r_ble_lll_conn_reset;
extern int crypt_get_iv;
extern int coex_event_duration_get;
extern int __lock___sfp_recursive_mutex;
extern int sleep_retention_entries_create;
extern int mbedtls_mpi_core_read_le;
extern int wpa_cipher_valid_mgmt_group;
extern int esp_netif_new;
extern int lv_fs_drv_register;
extern int g_authmode_threshold_failure;
extern int lv_area_align;
extern int gpio_controller_init_descriptors;
extern int r_ble_lll_adv_periodic_set_new_data;
extern int ieee80211_ap_try_sa_query;
extern int mbedtls_ecp_write_key_ext;
extern int ble_hs_conn_remove;
extern int ble_stack_enable;
extern int lv_style_is_empty;
extern int __wrap_gpio_reset_pin;
extern int esp32_i2s_driver;
extern int _ZSt15__ostream_writeIcSt11char_traitsIcEEvRSt13basic_ostreamIT_T0_EPKS3_i;
extern int app_manager_start_for_result;
extern int pm_coex_slice_timeout_process;
extern int esp_wifi_disconnect_internal;
extern int i2c_driver_install;
extern int r_ble_lll_sched_check_overlap;
extern int app_manager_for_each_manifest;
extern int i2c_master_multi_buffer_transmit;
extern int mbedtls_psa_ffdh_key_agreement;
extern int lv_textarea_set_align;
extern int phy_txiq_cover;
extern int CAMERA_TYPE;
extern int ds_hal_finish;
extern int xTimerGetReloadMode;
extern int esp_wifi_sta_get_rssi;
extern int hal_he_bssid_init;
extern int hid_report_map_keyboard_consumer_mouse_len;
extern int lv_timer_set_auto_delete;
extern int wifi_set_promis_process;
extern int mbedtls_ssl_get_key_exchange_md_tls1_2;
extern int mbedtls_psa_cipher_decrypt_setup;
extern int chm_set_current_channel;
extern int pmksa_cache_set_current;
extern int lv_obj_refresh_ext_draw_size;
extern int r_sdkconfig_get_opts;
extern int sta_update_power_off_reset_flag;
extern int esp_wifi_scan_start;
extern int tt_app_selectiondialog_start;
extern int wDev_Insert_KeyEntry;
extern int hal_enable_tx_statistics;
extern int _ZGVNSt7collateIcE2idE;
extern int PMU_instance;
extern int esp_vfs_register_fd_range;
extern int test_get_nonmumimo_common;
extern int _ZZNKSt7__cxx117collateIcE12do_transformEPKcS3_EN4_BufD1Ev;
extern int vTaskPlaceOnEventListRestricted;
extern int lvgl_keyboard_remove;
extern int npl_freertos_event_reset;
extern int ipaddr_aton;
extern int _rename_r;
extern int netconn_delete;
extern int phy_bb_wdg_cfg;
extern int hal_crypto_is_key_valid;
extern int r_os_cputime_ticks_to_usecs;
extern int port_uxCriticalNesting;
extern int ieee80211_add_beacon_app_ies;
extern int tlsf_walk_pool;
extern int lv_obj_hit_test;
extern int _Znwj;
extern int bootloader_execute_flash_command;
extern int phy_i2c_master_reset;
extern int lv_layer_top;
extern int bt_bb_rx_correlator_set;
extern int adiList_stack_addAdiList;
extern int minmea_parse_rmc;
extern int NAN_STANDARD_BSSID;
extern int ieee80211_muedca_updateparams;
extern int gdma_ahb_hal_enable_burst;
extern int lv_anim_path_ease_in;
extern int r_ble_ll_hci_post_gen_dhkey_cmp_evt;
extern int ble_gap_deinit;
extern int lv_indev_scroll_throw_predict;
extern int tcp_new_ip_type;
extern int cnx_assoc_timeout_process;
extern int nvs_open_from_partition;
extern int esp_cache_sync_ops_enter_critical_section;
extern int esf_buf_setdown;
extern int ic_trc_update_ifx_phy_mode;
extern int mbedtls_ssl_parse_change_cipher_spec;
extern int r_ble_ll_conn_rx_data_pdu;
extern int hexstr2bin;
extern int port_uxOldInterruptState;
extern int r_ble_ll_addr_is_id;
extern int hci_stack_deinitEnv;
extern int itwt_information_timeout_fn;
extern int pp_attach;
extern int lv_style_reset;
extern int npl_freertos_os_started;
extern int ppEnqueueRxq;
extern int lodepng_get_bpp;
extern int ieee80211_he_updateparams;
extern int lv_area_diff;
extern int hmac_hal_configure;
extern int ic_get_ptk_alg;
extern int nvs_set_i8;
extern int hal_mac_rx_disable;
extern int npl_freertos_time_ms_to_ticks;
extern int coex_schm_bt_idle_wifi_scan;
extern int strdup;
extern int trc_onPPTxDone;
extern int _cos;
extern int r_ble_phy_init;
extern int lv_chart_refresh;
extern int __time_wday;
extern int r_ble_ll_adv_aux_scannable_pdu_make;
extern int esp_event_loop_create;
extern int hal_mac_tx_clr_mplen;
extern int _ZZ10app_ledgervE6ledger;
extern int r_ble_lll_conn_sm_new;
extern int wmemchr;
extern int http_auth_basic;
extern int ip6_output_if_src;
extern int phy_pbus_print;
extern int r_ble_ll_mtrack_update;
extern int vListInitialiseItem;
extern int lmacConfMib;
extern int r_ble_ll_conn_sm_npl_init;
extern int ble_sm_lgcy_random_exec;
extern int spi_hal_get_intr_mask;
extern int lv_draw_buf_width_to_stride;
extern int tsf_hal_get_time;
extern int ble_transport_hs_init;
extern int setlocale;
extern int gps_settings_add_configuration;
extern int modem_clock_hal_select_ble_rtc_timer_lpclk_source;
extern int r_ble_lll_conn_update_link_state;
extern int lv_draw_sw_mask_free_param;
extern int esp_get_free_heap_size;
extern int nvs_close;
extern int pbuf_alloced_custom;
extern int phy_pbus_clear_reg;
extern int ppGetTxframe;
extern int pm_beacon_miss_exceeded_wakeup_disabled;
extern int ESP_EFUSE_WR_DIS_KEY_PURPOSE_2;
extern int inc_byte_array;
extern int _ZTISt20bad_array_new_length;
extern int lora_set_modulation;
extern int esp_coex_common_timer_arm_us_wrapper;
extern int wpa_sta_clear_curr_pmksa;
extern int rtc_clk_32k_disable_external;
extern int lv_anim_path_step;
extern int lmacRecycleMPDU;
extern int esp_partition_main_flash_region_safe;
extern int aes_wrap;
extern int mspi_timing_config_set_psram_clock;
extern int _ZTIN9__gnu_cxx24__concurrence_lock_errorE;
extern int _ZTISt16invalid_argument;
extern int _tzset_unlocked;
extern int ble_gap_conn_active;
extern int r_ble_ll_adv_active_chanset_is_sec;
extern int ieee80211_gettid;
extern int g_wifi_event_mask;
extern int pointer_get_touched_points;
extern int _ZGVNSt9money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int phy_get_rf_cal_version;
extern int pthread_include_pthread_semaphore_impl;
extern int mac_rxbuf_init;
extern int r_ble_ll_adv_get_chan_num;
extern int r_ble_lll_adv_txbuf_pdu_info_get;
extern int r_ble_ll_sync_get_handle;
extern int lv_event_get_param;
extern int device_construct_add_start;
extern int lv_slider_get_value;
extern int regdma_find_module_link_tail;
extern int sdmmc_send_cmd_send_if_cond;
extern int audio_stream_get_enabled;
extern int offchan_in_progress;
extern int ppSelectNextQueue;
extern int ieee80211_is_lr_only;
extern int gpio_uninstall_isr_service;
extern int ieee80211_find_elem;
extern int tcp_segs_free;
extern int ppTask;
extern int lv_table_class;
extern int r_ble_lll_adv_set_scan_rsp_data;
extern int AUDIO_STREAM_TYPE;
extern int r_ble_lll_adv_update_aux_offset_in_buffer;
extern int lv_draw_sw_rotate;
extern int ble_gap_adv_set_data;
extern int lv_free;
extern int mbedtls_ssl_write_record;
extern int _ZTVSt7codecvtIDiDu10_mbstate_tE;
extern int r_ble_log_internal_x2;
extern int hci_transport_host_cmd_tx;
extern int hal_mac_txq_enable;
extern int ieee80211_parse_wapi;
extern int r_ble_ll_scan_can_chg_whitelist;
extern int r_ble_lll_adv_reset_link_state;
extern int lv_draw_sw_blend_image_to_l8;
extern int psa_hash_abort;
extern int psa_mac_sign_setup;
extern int lv_draw_sw_mask_cleanup;
extern int ic_get_addr;
extern int esp_http_client_request_send;
extern int ieee80211_scan_attach;
extern int r_hal_timer_task_start;
extern int swprintf;
extern int adc_oneshot_hal_channel_config;
extern int _copy;
extern int md5_vector;
extern int coex_schm_bt_default_wifi_conn;
extern int lv_label_class;
extern int hostapd_cleanup;
extern int btdm_broker_env_deinit;
extern int r_ble_lll_mmgmt_init;
extern int device_exists_of_type;
extern int _sinf;
extern int esp_vfs_open;
extern int wifi_start_process;
extern int generic_esp32c5_module;
extern int tcp_seg_copy;
extern int fscanf;
extern int r_ble_lll_scan_invoke_ll_process;
extern int i2c_controller_register16be_get;
extern int r_ble_lll_conn_get_empty_packet_rssi;
extern int lv_obj_set_content_width;
extern int memp_TCP_SEG;
extern int conn_stack_initEnv;
extern int rcGetDefaultHigestRateIdx;
extern int xSemaphoreCreateGenericWithCaps;
extern int tcp_pcb_remove;
extern int explicit_bzero;
extern int lv_cache_drop;
extern int lodepng_decode;
extern int tbtt_adaptive_servo;
extern int mbedtls_ssl_tls12_write_client_hello_exts;
extern int hal_crypto_mgmt_rx_enabled;
extern int ble_hs_hw_error;
extern int mbedtls_mpi_core_random;
extern int lv_flex_init;
extern int esf_buf_free_static;
extern int coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_scan;
extern int ble_sm_alg_s1;
extern int esp_wifi_ap_is_sta_sae_reauth_node;
extern int lv_font_has_static_bitmap;
extern int _ZTVSt18__moneypunct_cacheIcLb1EE;
extern int ble_uuid_init_from_buf;
extern int esp_wifi_ap_get_sta_aid;
extern int r_ble_phy_rate_to_mode;
extern int mbedtls_pk_init;
extern int bundle_get_int64;
extern int lv_indev_send_event;
extern int ble_store_config_persist_peer_secs;
extern int ht_get_min_subframe_len;
extern int lv_label_get_text_selection_start;
extern int spi_flash_guard_set;
extern int _ZTVNSt7__cxx118time_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEEE;
extern int pm_set_wifimac_regdma_link_selection;
extern int mt_fetch_peer;
extern int spi_controller_lock;
extern int xIsrStackTop;
extern int window_manager_remove;
extern int spi_flash_restore_cache;
extern int ble_lll_mmgmt_env_p;
extern int r_ble_ll_rand_env_deinit;
extern int dns_setserver;
extern int esp_app_desc;
extern int ble_att_create_chan;
extern int lv_display_set_default;
extern int _ZSt20__throw_system_errori;
extern int http_parser_init;
extern int fabs;
extern int phy_get_tsens_value;
extern int esp_flash_read;
extern int r_ble_lll_mmgmt_alloc_tx_buffer_and_hdr;
extern int phy_index_to_txbbgain;
extern int r_ble_lll_ext_scan_coex_dpc_update_on_start;
extern int _ZTVN2tt14RecursiveMutexE;
extern int wpa_michael_mic_failure;
extern int r_ble_ll_adv_active_chanset_set_sec;
extern int r_ble_lll_conn_recycle_buffer;
extern int ieee80211_ht_node_cleanup;
extern int mbedtls_mpi_core_shift_r;
extern int pm_beacon_offset_is_sampling;
extern int __extendsfdf2;
extern int lv_bin_decoder_close;
extern int vEventGroupSetBitsCallback;
extern int rcUpdateRate;
extern int lv_obj_set_parent;
extern int mbedtls_psa_ecp_load_representation;
extern int itwt_cal_conts_miss_beacon_sp_count;
extern int r_ble_ll_ctrl_start_rsp_timer;
extern int spi_flash_chip_winbond_erase_block;
extern int esp_intr_enable_source;
extern int ble_transport_ll_deinit;
extern int pwm_get_duty;
extern int lv_obj_set_scroll_snap_y;
extern int is_fid_setup;
extern int coex_hw_timer_disable;
extern int esp_wifi_set_keep_alive_time;
extern int rtos_int_enter;
extern int r_ble_ll_conn_next_event;
extern int mbedtls_mpi_core_bitlen;
extern int esp_clk_tree_lp_fast_get_freq_hz;
extern int __bufio_fill_locked;
extern int lv_chart_set_div_line_count;
extern int gdma_hal_set_priority;
extern int cnx_connect_next_ap_timeout_process;
extern int psa_cipher_decrypt_setup;
extern int tcp_next_iss;
extern int rcUpdateRxDone;
extern int coex_schm_ble_mesh_config_wifi_scan;
extern int ESP_EFUSE_KEY2;
extern int lv_group_focus_obj;
extern int netif_set_up;
extern int _ZTVNSt7__cxx1110moneypunctIwLb1EEE;
extern int esp_event_post;
extern int lv_deinit;
extern int g_spi_lock_main_flash_dev;
extern int phy_get_sar_sig_ref;
extern int apm_hal_set_master_sec_mode;
extern int r_ble_ll_scan_have_rxd_scan_rsp;
extern int lodepng_decode32;
extern int crypto_ec_deinit;
extern int scan_prefer_chan;
extern int tcpip_input;
extern int httpd_resp_send_err;
extern int esp_efuse_get_key_purpose;
extern int lv_draw_init;
extern int __kernel_sinf;
extern int mbedtls_ssl_get_output_max_frag_len;
extern int lv_obj_set_style_grid_cell_row_span;
extern int chm_end_op_timeout_process;
extern int lv_draw_sw_line;
extern int bt_rf_coex_cfg_p;
extern int pm_disconnected_wake;
extern int coex_schm_ble_mesh_config_bt_conn_wifi_scan;
extern int ledc_bind_channel_timer;
extern int esp_libc_include_memcpy_impl;
extern int brownout_hal_config;
extern int spi_flash_disable_interrupts_caches_and_other_cpu;
extern int mem_init;
extern int ic_set_he_min_subframe_len_tab;
extern int send_ap_probe;
extern int __floatunsisf;
extern int ieee80211_vht_node_cleanup;
extern int __eqdf2;
extern int pm_twt_keep_alive_timeout_process;
extern int ble_bb_cte_set_cte_ant_delay_rx_aoa_1us;
extern int spi_bus_lock_get_by_id;
extern int esp_mmu_map_virt;
extern int spi_flash_chip_generic_write_encrypted;
extern int regdma_find_module_link_head;
extern int esp_rtc_init;
extern int phy_ant_clr_update_flag;
extern int cnx_sta_connect_led_timer_cb;
extern int ic_is_mgmt_hwdecr_enabled;
extern int pthread_setspecific;
extern int service_instance_destruct;
extern int ble_hs_state_ctx;
extern int lv_image_buf_get_transformed_area;
extern int xTaskGenericNotifyStateClear;
extern int r_DB_hash_list_reset;
extern int uart_hal_set_stop_bits;
extern int wifi_connect_process;
extern int lmacInit;
extern int hostapd_config_clear_wpa_psk;
extern int esp_aes_release_hardware;
extern int lv_draw_sw_box_shadow;
extern int battery_sense_power_supply_driver;
extern int esp_partition_read_raw;
extern int usb_device_controller_claim;
extern int sta_auth_sae;
extern int wDev_Get_Next_BcnOffset;
extern int r_filter_duplicate_mode_enable;
extern int esp_aes_gcm_init;
extern int ldexpf;
extern int ieee80211_action_vendor_spec_attach;
extern int tcp_sent;
extern int lv_anim_delete;
extern int pm_extend_tbtt_adaptive_attach;
extern int r_ble_lll_adv_sec_chan_pdu_update;
extern int ledc_hal_set_hpoint;
extern int wifi_init_in_caller_task;
extern int ble_midi_set_active;
extern int _ZTVSt15_Sp_counted_ptrIPN2tt7service13memorychecker20MemoryCheckerServiceELN9__gnu_cxx12_Lock_policyE1EE;
extern int r_ble_lll_rxpdu_copy;
extern int pthread_include_pthread_cond_var_impl;
extern int hal_he_get_aid;
extern int coex_schm_interval_get;
extern int rsn_is_snonce_cookie;
extern int __lock___env_recursive_mutex;
extern int ieee80211_reg_netstack_buf_cb;
extern int mbedtls_rsa_free;
extern int memp_NETCONN;
extern int mspi_timing_get_psram_low_speed_freq_mhz;
extern int hal_he_set_bbrxhung_time;
extern int _Z14app_fs_is_fileRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int esp_system_get_time_resolution;
extern int ieee80211_beacon_construct;
extern int psa_get_and_lock_key_slot;
extern int material_symbols_statusbar_16;
extern int ble_transport_to_ll_cmd_impl;
extern int pm_disable_dream_timer;
extern int roundf;
extern int r_ble_lll_scan_start;
extern int _ZSt17__verify_groupingPKcjRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE;
extern int rtc_get_time;
extern int lmac_update_tx_statistic;
extern int lv_list_set_button_text;
extern int ble_gap_update_params;
extern int r_ble_lll_sched_run;
extern int esp_vfs_fat_format_drive;
extern int esp_libc_locks_init;
extern int mbedtls_mpi_is_prime_ext;
extern int ble_hs_hci_util_set_data_len;
extern int r_ble_ll_mtrack_length_set;
extern int gpio_hal_matrix_out;
extern int bluetooth_midi_stop;
extern int tlsf_create_with_pool;
extern int hal_mac_get_txq_pmd;
extern int r_ble_ll_hci_adv_set_enable;
extern int icmp6_param_problem;
extern int ieee80211_ht_updateparams;
extern int ip4_output_if_opt;
extern int lvgl_try_lock;
extern int ble_gap_rx_rd_rem_ver_info_complete;
extern int hal_set_sta_tbtt;
extern int __kernel_rem_pio2;
extern int spi_hal_cal_clock_conf;
extern int rtc_gpio_pulldown_dis;
extern int r_ble_ll_resolv_update_priv_addr;
extern int ieee80211_send_deauth_no_bss;
extern int memspi_host_set_write_protect;
extern int __wtime_month;
extern int _do_wifi_stop;
extern int lv_obj_get_style_prop;
extern int r_ble_lll_adv_txbuf_memory_alloc_and_reset;
extern int gf_double;
extern int isxdigit;
extern int esp_http_client_fetch_headers;
extern int lv_indev_enable;
extern int lmacMSDUAged;
extern int spi_flash_chip_generic_erase_chip;
extern int r_ble_ll_get_addr_type;
extern int panic_print_hex;
extern int phy_txpwr_correct_new;
extern int nd6_clear_destination_cache;
extern int MPI_OPERATIONS_REG;
extern int esp_task_wdt_print_triggered_tasks;
extern int r_ble_lll_adv_set_adv_data;
extern int sae_accept_sta;
extern int r_ble_ll_resolv_rpa_timer_reset;
extern int lv_draw_buf_init_with_default_handlers;
extern int psa_generate_key_internal;
extern int nl_langinfo;
extern int preferences_put_string;
extern int mld6_report_groups;
extern int lv_area_is_in;
extern int ble_ll_scan_env_p;
extern int esp_wifi_get_promiscuous;
extern int towupper;
extern int wpa_sm_set_ap_rsnxe_override;
extern int lv_textarea_add_char;
extern int lv_style_set_bg_color;
extern int esp_wifi_enable_sae_pk_only_mode_internal;
extern int lv_draw_mask_rect;
extern int lv_timer_create_basic;
extern int r_ble_lll_conn_master_common_init;
extern int lvgl_unlock;
extern int r_ble_lll_recycle_buffer_in_task;
extern int esp_wifi_set_mode;
extern int esp_phy_store_cal_data_to_nvs;
extern int gdma_hal_get_eof_desc_addr;
extern int pbuf_add_header_force;
extern int r_os_msys_mbuf_get_enhanced;
extern int esp_ecdsa_transparent_verify_hash_start;
extern int lmacProcessLongRetryFail;
extern int udp_netif_ip_addr_changed;
extern int log10;
extern int _ZTVN10__cxxabiv120__si_class_type_infoE;
extern int esp32_bluetooth_driver;
extern int r_ble_ll_is_valid_adv_mode;
extern int r_ble_ll_reset;
extern int r_ble_ll_whitelist_init;
extern int lv_style_set_transition;
extern int lv_obj_set_style_min_width;
extern int mbedtls_ssl_derive_keys;
extern int esp_vfs_register_common;
extern int esp_test_clr_tx_tb_statistics;
extern int netif_remove;
extern int uxTaskGetSystemState;
extern int pm_beacon_add_total_counter;
extern int lwip_netconn_do_bind;
extern int BcnSendTick;
extern int one_bits;
extern int lv_anim_set_var;
extern int r_ble_ll_ctrl_initiate_dle;
extern int _ZSt15__try_use_facetISt10moneypunctIcLb0EEEPKT_RKSt6locale;
extern int esp_netif_dhcpc_start;
extern int ble_gap_event_listener_register;
extern int r_ble_ll_adv_scan_req_rxd;
extern int r_ble_lll_conn_sm_npl_deinit;
extern int ppProcessRxPktHdr;
extern int ieee80211_add_assoc_resp_ies;
extern int f_readdir;
extern int valloc;
extern int r_ble_hw_whitelist_sort;
extern int gpio_matrix_input;
extern int ble_sm_tx;
extern int mspi_timing_enter_low_speed_mode;
extern int esp_wifi_internal_set_log_level;
extern int lv_dropdown_set_options_static;
extern int gps_settings_for_each_configuration;
extern int phy_i2c_master_cmd_mem_init;
extern int phy_set_txclk_en;
extern int mbedtls_x509_get_name;
extern int phy_pkdadc_set;
extern int sntp_setoperatingmode;
extern int spi_bus_lock_get_acquiring_dev;
extern int _ZZNSt9formatterIPKvcE5parseERSt26basic_format_parse_contextIcEENKUlvE0_clEv;
extern int mbedtls_x509_free_subject_alt_name;
extern int phy_param_track_tot;
extern int tt_timezone_get_code;
extern int uECC_vli_numBits;
extern int lv_obj_set_style_bg_opa;
extern int heap_caps_get_info;
extern int mbedtls_ecdsa_der_to_raw;
extern int mbedtls_aes_crypt_ofb;
extern int pvt_func_enable;
extern int usb_host_hid_unsubscribe;
extern int _ZGVNSt7__cxx119money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE;
extern int mbedtls_mpi_write_binary;
extern int mbedtls_ssl_check_curve;
extern int lv_anim_get;
extern int wifi_disconnect_process;
extern int bluetooth_set_radio_enabled;
extern int lv_draw_buf_clear;
extern int _Z13init_mtk_l76bP6Device;
extern int _ZSt9terminatev;
extern int r_ble_lll_conn_event_abort_in_first_rx_tx_pair;
extern int isinff;
extern int tcp_bind;
extern int mbedtls_psa_ecp_export_key;
extern int error_to_string;
extern int rcLowerSched;
extern int f_mount;
extern int efuse_hal_get_minor_chip_version;
extern int esp_sha256_driver_compute;
extern int netconn_recv_udp_raw_netbuf_flags;
extern int ble_sm_our_pair_rand;
extern int ble_att_cmd_prepare;
extern int lv_style_set_outline_opa;
extern int sys_thread_sem_get;
extern int lvgl_toolbar_clear_actions;
extern int wpa_sm_deauthenticate;
extern int r_ble_ll_scan_get_peer_rpa;
extern int s_microseconds_offset;
extern int mbedtls_md_error_from_psa;
extern int lv_obj_set_style_border_side;
extern int lv_style_set_line_width;
extern int phy_get_modem_flag;
extern int sdmmc_enter_higher_speed_mode;
extern int lv_slider_set_mode;
extern int wifi_set_rxcb_process;
extern int chm_mhz2num;
extern int _Z21app_metadata_parse_v2RKSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_St4lessIS5_ESaISt4pairIKS5_S5_EEER11AppMetadata;
extern int esp_task_wdt_reset;
extern int g_in_blacklist_scanned_again;
extern int mbedtls_threading_key_slot_mutex;
extern int active_hid_rpt_map;
extern int adv_stack_enableClearLegacyAdvVsCmd;
extern int cnx_connect_next_ap;
extern int __tzcalc_limits;
extern int mbedtls_mpi_div_mpi;
extern int icmp6_dest_unreach;
extern int _isatty_r;
extern int adc_reset_lock_acquire;
extern int ieee80211_regdomain_is_active_scan;
extern int esp_netif_create_default_wifi_ap;
extern int esp_task_wdt_impl_timer_feed;
extern int __file_str_put;
extern int i2c_hal_master_trans_start;
extern int lv_display_get_rotation;
extern int rc5G11ACSchedTbl;
extern int _i2c_hal_deinit;
extern int MIDI_SVC_UUID;
extern int esp_vfs_set_readonly_flag;
extern int ieee80211_alloc_deauth;
extern int r_ble_ll_conn_sm_active_insert;
extern int mbedtls_cipher_update;
extern int ieee80211_espnow_set_init_flag;
extern int _ZTVNSt7__cxx119money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEEE;
extern int mbedtls_ecdsa_raw_to_der;
extern int mbedtls_ssl_get_bytes_avail;
extern int pm_on_beacon_rx;
extern int wifi_station_stop;
extern int nvs_sec_provider_include_impl;
extern int ieee80211_regdomain_chan_num;
extern int panic_clear_active_interrupts;
extern int ip4_input;
extern int double_jacobian_default;
extern int esp_partition_read;
extern int ble_store_config_persist_rpa_recs;
extern int pwr_hal_set_beacon_filter_force_sync_limit;
extern int xQueueReceive;
extern int lwip_netconn_do_write;
extern int r_ble_ll_ctrl_chanmap_req_make;
extern int wpa_cipher_put_suites;
extern int bt_bb_set_le_tx_on_delay;
extern int phy_rxiq_get_mis;
extern int pm_scale_listen_interval;
extern int esp_cmac_verify_finish;
extern int lv_chart_create;
extern int mbedtls_ssl_conf_verify;
extern int spi_flash_hal_gpspi_check_status;
extern int wcslen;
extern int timestamp_id;
extern int wDev_record_ftm_data_local;
extern int lmacReachLongLimit;
extern int bluetooth_midi_start;
extern int __issignalingf;
extern int r_ble_ll_conn_hci_update;
extern int psa_pk_status_to_mbedtls;
extern int ieee80211_parse_wmeparams;
extern int ic_enable_rx;
extern int r_ble_hci_trans_hs_cmd_tx;
extern int wdev_set_promis_ctrl_pkt;
extern int lwip_hook_tcp_isn;
extern int sdmmc_init_io;
extern int sha_hal_set_mode;
extern int r_ble_ll_sync_cancel;
extern int _ZTv0_n12_NSt14basic_ifstreamIcSt11char_traitsIcEED1Ev;
extern int phy_wait_freq_set_busy_new;
extern int pp_gettid;
extern int sys_timeouts_init;
extern int r_ble_lll_sched_update_cur_entry;
extern int lv_textarea_cursor_down;
extern int r_ble_ll_df_conn_cte_tx_set_param;
extern int wpa_parse_wpa_ie_rsn;
extern int esp_gpio_reserve;
extern int __floatundisf;
extern int wpa3_hostap_auth_init;
extern int flash_mmap_lock_freeze;
extern int esp_log_linked_list_set_level;
extern int ble_att_cmd_get;
extern int pm_active_timeout_process;
extern int ieee80211_output_process;
extern int camera_set_rotation;
extern int scan_set_scan_id;
extern int he_twt_setup_txcb;
extern int mbedtls_x509_oid_get_x509_ext_type;
extern int ieee80211_psq_take_head;
extern int _ZTSN10__cxxabiv115__forced_unwindE;
extern int __wpa_send_eapol;
extern int phy_ant_init;
extern int uart_intr_config;
extern int lv_label_set_text_selection_start;
extern int lv_text_ins;
extern int lv_cache_acquire;
extern int r_ble_ll_ctrl_conn_param_pdu_make;
extern int esp_rsa_ds_opaque_sign_hash_complete;
extern int coex_schm_ble_mesh_standby_bt_conn_wifi_scan;
extern int gdma_ahb_hal_read_intr_status;
extern int memp_PBUF;
extern int reset_cs_thres;
extern int get_fd_entry;
extern int tt_timezone_get_name;
extern int ble_store_config_compare_bond_count;
extern int __time_mon;
extern int ppCalVHTDeliNum;
extern int service_manager_add;
extern int coex_core_deinit;
extern int lv_textarea_set_password_bullet;
extern int ic_del_extra_softap_rx_ba;
extern int get_act_hp_dbias;
extern int rcClearCurSched;
extern int psa_mac_update;
extern int mbedtls_psa_external_get_random;
extern int esp_aes_cipher_finish;
extern int __wrap___bufio_setvbuf;
extern int mbedtls_ssl_config_defaults;
extern int lv_obj_set_style_radius;
extern int ble_gattc_read_long;
extern int ble_sm_create_chan;
extern int _ZZNSt18__moneypunct_cacheIwLb1EE8_M_cacheERKSt6localeEN11_Scoped_strC2ERKSbIwSt11char_traitsIwESaIwEE;
extern int _ZSt13__int_to_charIcmEiPT_T0_PKS0_St13_Ios_Fmtflagsb;
extern int r_ble_ll_rand_init;
extern int r_ble_ll_conn_hci_read_rem_features;
extern int mbedtls_ssl_start_renegotiation;
extern int ieee80211_find_ext_elem;
extern int esp_http_client_get_user_data;
extern int esp_panic_handler_feed_wdts;
extern int lv_display_get_original_vertical_resolution;
extern int wifi_malloc;
extern int r_ble_lll_mmgmt_rxbuffer_free_all;
extern int ic_set_key;
extern int ESP_EFUSE_KEY_PURPOSE_4;
extern int esp_ip4addr_ntoa;
extern int ble_att_svr_rx_notify_multi;
extern int esp_wifi_set_sta_key_internal;
extern int _ZTVSt16__numpunct_cacheIwE;
extern int gdma_start;
extern int r_ble_lll_scan_process_aux_data;
extern int ap_sta_add;
extern int putc;
extern int pmu_init;
extern int r_ble_lll_mmgmt_register_rxbuf_cb;
extern int r_ble_lll_sched_delay_if_overlapped;
extern int g_espnow_lock;
extern int wifi_softap_cur_max_ap_bss_num;
extern int wpa_supplicant_process_1_of_4;
extern int mbedtls_pk_parse_key;
extern int ieee80211_is_support_rate;
extern int pbuf_ref;
extern int phy_write_gain_mem;
extern int rcGetSMPDURate;
extern int ble_l2cap_get_conn_handle;
extern int log10f;
extern int _ZTVSt8messagesIcE;
extern int r_ble_lll_mmgmt_get_rxbuf_management_sch_item;
extern int esp_rom_output_to_channels;
extern int lv_fs_get_ext;
extern int os_mempool_init;
extern int esp_transport_ssl_set_addr_family;
extern int mpi_hal_enable_hardware_hw_op;
extern int _ZTVSt7num_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE;
extern int register_ieee80211_action_vendor_get_key_cb;
extern int strcpy;
extern int i2c_common_set_pins;
extern int esp_transport_ssl_set_client_key_data_der;
extern int lvgl_keyboard_on_start_lvgl;
extern int esp_cache_msync;
extern int sdmmc_send_cmd;
extern int mbedtls_psa_crypto_init_include_impl;
extern int hal_set_sta_tsf;
extern int r_ble_ll_generate_dh_key_v1;
extern int adjtime;
extern int lv_obj_set_width;
extern int _ZTv0_n12_NSiD1Ev;
extern int lv_obj_set_flex_align;
extern int lv_draw_rect_dsc_init;
extern int ble_gatts_conn_init;
extern int npl_freertos_callout_remaining_ticks;
extern int r_ble_lll_scan_free_aux_memory;
extern int tcp_abandon;
extern int lv_draw_image_normal_helper;
extern int _ZTVSt8time_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE;
extern int _putc1;
extern int sdmmc_get_erase_timeout_ms;
extern int s_dp;
extern int ble_sm_alg_f4;
extern int vTaskSwitchContext;
extern int esp_hmac_finish_opaque;
extern int esp_efuse_is_flash_encryption_enabled;
extern int esf_buf_setup_static;
extern int app_uninstall;
extern int lv_indev_get_user_data;
extern int uart_get_selectlock;
extern int pm_get_null_max_tx_time;
extern int memspi_host_read_id_hs;
extern int hal_mac_set_rxq_policy;
extern int hci_stack_getBroker;
extern int hal_sniffer_rx_set_promis;
extern int _lock_acquire;
extern int pp_post;
extern int r_ble_ll_hci_ev_path_loss_threshold;
extern int mbedtls_pk_get_bitlen;
extern int __wrap_gpio_set_pull_mode;
extern int ble_store_delete_cccd;
extern int mbedtls_ecdsa_sign;
extern int phy_set_pbus_mem_2g;
extern int g_cte_ant_switch_delay_1m;
extern int r_ble_ll_hci_is_event_enabled;
extern int sta_get_beacon_timeout;
extern int mbedtls_aes_crypt_ecb;
extern int hal_mac_set_rx_beacon_memory_addr_map;
extern int lvgl_hardware_keyboard_is_available;
extern int service_instance_get_manifest;
extern int lv_indev_create;
extern int sha_hal_wait_idle;
extern int lv_obj_set_style_transform_scale_y;
extern int ble_gap_rx_test_evt;
extern int netconn_disconnect;
extern int ff_disk_status;
extern int panic_soc_check_pseudo_cause;
extern int r_ble_lll_mmgmt_free;
extern int itwt_information_timeout_fn_process;
extern int xTaskGetStaticBuffers;
extern int esp_psram_chip_init;
extern int esp_wifi_sta_set_reset_nvs_pmk_internal;
extern int xStreamBufferReceive;
extern int putwc;
extern int lv_arc_set_bg_angles;
extern int lv_draw_sw_mask_rect;
extern int esp_netif_action_stop;
extern int elf_find_sym_default;
extern int esp_mbedtls_conn_delete;
extern int ledc_timer_resume;
extern int _ZTVSt8time_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE;
extern int esp_netif_netstack_buf_ref;
extern int coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_conn;
extern int mbedtls_ssl_read_record;
extern int vListInitialise;
extern int rc_disable_trc_by_interface;
extern int _ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SI_SK_;
extern int __isnand;
extern int npl_funcs_ro;
extern int gdma_hal_set_burst_size;
extern int ic_obtain_key;
extern int aes_ccm_ae;
extern int adc_hal_calibration_init;
extern int spi_flash_hal_set_write_protect;
extern int lv_style_set_border_side;
extern int lmacProcessAckTimeout;
extern int phy_version_str;
extern int lv_event_get_indev;
extern int esp_libc_timekeeping_adjtime_apply;
extern int ppEmptyDelimiterLength;
extern int esp_sleep_clock_config;
extern int esp_wifi_sta_get_group_cipher_internal;
extern int lv_obj_get_width;
extern int _ZSt15__try_use_facetISt7num_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEEEPKT_RKSt6locale;
extern int soc_memory_region_count;
extern int _ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EEPKS5_RKS8_;
extern int crypto_bignum_cmp;
extern int esp_partition_iterator_release;
extern int r_ble_lll_scan_get_aux_data_at;
extern int cJSON_GetNumberValue;
extern int s_btdm_broker_env_p;
extern int mpi_hal_write_rinv;
extern int driver_is_compatible;
extern int esp_transport_set_default_port;
extern int esp_lcd_new_panel_st7796;
extern int ble_l2cap_sig_timer;
extern int _ZZNSt18__moneypunct_cacheIcLb0EE8_M_cacheERKSt6localeEN11_Scoped_strC2ERKSs;
extern int eloop_destroy;
extern int esp_rom_get_bootloader_offset;
extern int scalbn;
extern int mac_tx_set_tb;
extern int spicommon_periph_claim;
extern int ieee80211_parse_heopr;
extern int mbedtls_mpi_shift_l;
extern int netif_get_ip6_addr_match;
extern int mspi_hw_info;
extern int lv_obj_set_style_line_rounded;
extern int wDev_IndicateCtrlFrame;
extern int crypto_bignum_is_zero;
extern int esp_cache_resume_ext_mem_cache;
extern int ieee80211_action_vendor_reset_seq_state;
extern int xPortSwitchFlag;
extern int i2c_set_pin;
extern int r_exception_list_ble_mesh_addr_table_remove;
extern int wifi_send_raw_frame;
extern int hal_mac_tx_get_blockack;
extern int mbedtls_cipher_definitions;
extern int _ZStmiRKSt15_Deque_iteratorI10QueuedItemRS0_PS0_ES5_;
extern int wifi_softap_stop;
extern int lv_button_create;
extern int base_stack_disable;
extern int lv_timer_create;
extern int lv_font_montserrat_14;
extern int psa_remove_key_data_from_memory;
extern int pbuf_get_at;
extern int psa_cipher_encrypt_setup;
extern int httpd_req_get_url_query_len;
extern int hal_mac_tsf_get_time;
extern int pwr_hal_set_beacon_filter_force_dump_enable;
extern int ble_att_svr_find_by_uuid;
extern int pm_reset_twt_active_cnt;
extern int heap_caps_get_largest_free_block;
#pragma GCC diagnostic pop

/* Available ELF symbols table: g_customer_elfsyms */

const struct esp_elfsym g_customer_elfsyms[] = {
    ESP_ELFSYM_EXPORT(esp_efuse_utility_count_once),
    ESP_ELFSYM_EXPORT(esp_test_get_rx_mu_statistics),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_force_dump_limit),
    ESP_ELFSYM_EXPORT(lv_style_set_shadow_spread),
    ESP_ELFSYM_EXPORT(gpio_install_isr_service),
    ESP_ELFSYM_EXPORT(lv_draw_sw_label),
    ESP_ELFSYM_EXPORT(_ZTVSt23__codecvt_abstract_baseIwc10_mbstate_tE),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_comp_event_send),
    ESP_ELFSYM_EXPORT(lv_textarea_get_cursor_pos),
    ESP_ELFSYM_EXPORT(uart_hal_set_txfifo_empty_thr),
    ESP_ELFSYM_EXPORT(coex_schm_bt_a2dp_wifi_scan),
    ESP_ELFSYM_EXPORT(service_instance_set_state),
    ESP_ELFSYM_EXPORT(esp_ota_get_running_partition),
    ESP_ELFSYM_EXPORT(uart_disable_rx_intr),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_ap_info_prof_pmk_internal),
    ESP_ELFSYM_EXPORT(esp_now_send),
    ESP_ELFSYM_EXPORT(lv_trigo_sin),
    ESP_ELFSYM_EXPORT(tt_app_alertdialog_start),
    ESP_ELFSYM_EXPORT(lv_indev_read),
    ESP_ELFSYM_EXPORT(test_mimo_update_user_info),
    ESP_ELFSYM_EXPORT(ethbroadcast),
    ESP_ELFSYM_EXPORT(esp_partition_find),
    ESP_ELFSYM_EXPORT(console_read),
    ESP_ELFSYM_EXPORT(psa_key_agreement_raw_builtin),
    ESP_ELFSYM_EXPORT(lv_font_get_glyph_dsc_fmt_txt),
    ESP_ELFSYM_EXPORT(lv_draw_fill_dsc_init),
    ESP_ELFSYM_EXPORT(ble_att_svr_start),
    ESP_ELFSYM_EXPORT(lv_font_montserrat_18),
    ESP_ELFSYM_EXPORT(phy_freq_i2c_num_addr),
    ESP_ELFSYM_EXPORT(ppMapWaitTxq),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_send_legacy_ext_adv_report),
    ESP_ELFSYM_EXPORT(pmksa_cache_auth_get),
    ESP_ELFSYM_EXPORT(ledc_set_fade_with_step),
    ESP_ELFSYM_EXPORT(default_router_list),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_insert_eof_head),
    ESP_ELFSYM_EXPORT(lv_color_format_get_size),
    ESP_ELFSYM_EXPORT(httpd_sess_close_all),
    ESP_ELFSYM_EXPORT(mpi_hal_set_mode),
    ESP_ELFSYM_EXPORT(esp_sha512_driver_abort),
    ESP_ELFSYM_EXPORT(lv_obj_get_event_count),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_tls12_get_preferred_hash_for_sig_alg),
    ESP_ELFSYM_EXPORT(cnx_auth_timeout_process),
    ESP_ELFSYM_EXPORT(ic_tx_pkt),
    ESP_ELFSYM_EXPORT(_ZdlPvjSt11align_val_t),
    ESP_ELFSYM_EXPORT(spi_flash_chip_winbond_page_program),
    ESP_ELFSYM_EXPORT(is_fragmented_pkt),
    ESP_ELFSYM_EXPORT(spi_flash_brownout_need_reset),
    ESP_ELFSYM_EXPORT(phy_rxdc_est_min_new),
    ESP_ELFSYM_EXPORT(lv_draw_deinit),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_flex_main_place),
    ESP_ELFSYM_EXPORT(ble_store_config_persist_local_irk),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_read_group_type_adata),
    ESP_ELFSYM_EXPORT(http_header_destroy),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_cond_swap),
    ESP_ELFSYM_EXPORT(lvgl_keyboard_add_textarea),
    ESP_ELFSYM_EXPORT(hostap_input),
    ESP_ELFSYM_EXPORT(lvgl_hardware_keyboard_add_custom),
    ESP_ELFSYM_EXPORT(eloop_run),
    ESP_ELFSYM_EXPORT(preferences_opt_int32),
    ESP_ELFSYM_EXPORT(ieee80211_tx_mgt_cb),
    ESP_ELFSYM_EXPORT(vListInsert),
    ESP_ELFSYM_EXPORT(gpio_config_as_analog),
    ESP_ELFSYM_EXPORT(httpd_req_get_hdr_value_str),
    ESP_ELFSYM_EXPORT(wifi_check_chan_param),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_auth_decrypt),
    ESP_ELFSYM_EXPORT(wpa_auth_write_assoc_resp_owe),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_rx_pkt_in_on_legacy),
    ESP_ELFSYM_EXPORT(_ZTVSt17bad_function_call),
    ESP_ELFSYM_EXPORT(esp_ocode_calib_init),
    ESP_ELFSYM_EXPORT(uart_hal_rxfifo_rst),
    ESP_ELFSYM_EXPORT(vswprintf),
    ESP_ELFSYM_EXPORT(ic_update_sta_tsf),
    ESP_ELFSYM_EXPORT(valid_key_length),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_dup_update_ext),
    ESP_ELFSYM_EXPORT(app_elf_sha256_str),
    ESP_ELFSYM_EXPORT(phy_cal_param_track),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_parse_der),
    ESP_ELFSYM_EXPORT(ble_ll_mem_env_p),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_record_new_adv),
    ESP_ELFSYM_EXPORT(phy_wifi_get_target_power),
    ESP_ELFSYM_EXPORT(btwt_setup_dwell_timeout_fn_process),
    ESP_ELFSYM_EXPORT(i2s_controller_get_config),
    ESP_ELFSYM_EXPORT(r_ble_ll_utils_calc_access_addr),
    ESP_ELFSYM_EXPORT(minitar_close),
    ESP_ELFSYM_EXPORT(esp_netif_get_desc),
    ESP_ELFSYM_EXPORT(tcp_trigger_input_pcb_close),
    ESP_ELFSYM_EXPORT(esp_http_client_set_method),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_traverse_sequence_of),
    ESP_ELFSYM_EXPORT(regdma_link_new_branch_addr_map),
    ESP_ELFSYM_EXPORT(lv_cache_class_lru_rb_size),
    ESP_ELFSYM_EXPORT(esp_wifi_vnd_lora_disable),
    ESP_ELFSYM_EXPORT(esp_timer_delete),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_decor),
    ESP_ELFSYM_EXPORT(g_coex_param),
    ESP_ELFSYM_EXPORT(properties_file_open),
    ESP_ELFSYM_EXPORT(gpio_descriptor_acquire),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_a2dp_wifi_scan),
    ESP_ELFSYM_EXPORT(ble_gap_rx_l2cap_update_req),
    ESP_ELFSYM_EXPORT(hmac_sha256_vector),
    ESP_ELFSYM_EXPORT(pm_mac_enable_tsf_tbtt_soc_wakeup),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_decrypt_buf),
    ESP_ELFSYM_EXPORT(phy_i2c_readReg_Mask),
    ESP_ELFSYM_EXPORT(__stdio_flags),
    ESP_ELFSYM_EXPORT(__atomic_fetch_and_8),
    ESP_ELFSYM_EXPORT(httpd_register_uri_handler),
    ESP_ELFSYM_EXPORT(mbedtls_aes_xts_free),
    ESP_ELFSYM_EXPORT(sdspi_host_check_buffer_alignment),
    ESP_ELFSYM_EXPORT(_g_esp_netif_netstack_default_wifi_ap),
    ESP_ELFSYM_EXPORT(dragonfly_get_random_qr_qnr),
    ESP_ELFSYM_EXPORT(ieee80211_output_do),
    ESP_ELFSYM_EXPORT(esp_crypto_aes_gcm_encrypt_setup),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_delay_tx_aod_1us),
    ESP_ELFSYM_EXPORT(mac_tx_set_duration),
    ESP_ELFSYM_EXPORT(lodepng_chunk_type_equals),
    ESP_ELFSYM_EXPORT(ble_gap_repeat_pairing_event),
    ESP_ELFSYM_EXPORT(bt_bb_v2_tx_set),
    ESP_ELFSYM_EXPORT(pbuf_clone),
    ESP_ELFSYM_EXPORT(phy_pwdet_tone_start),
    ESP_ELFSYM_EXPORT(lv_style_set_arc_width),
    ESP_ELFSYM_EXPORT(lv_area_increase),
    ESP_ELFSYM_EXPORT(ip4_addr_isbroadcast_u32),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_npl_restore),
    ESP_ELFSYM_EXPORT(modem_clock_select_lp_clock_source),
    ESP_ELFSYM_EXPORT(eb_space),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_read_version),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_AVE_INITCODE_ATTEN2),
    ESP_ELFSYM_EXPORT(coex_rom_osi_funcs_init),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED1Ev),
    ESP_ELFSYM_EXPORT(i2c_master_device_change_address),
    ESP_ELFSYM_EXPORT(ble_restore_our_sec_nvs),
    ESP_ELFSYM_EXPORT(_ZSt12__to_chars_iIyESt15to_chars_resultPcS1_T_i),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_pad_left),
    ESP_ELFSYM_EXPORT(ieee80211_search_node),
    ESP_ELFSYM_EXPORT(wpa_supplicant_gtk_tx_bit_workaround),
    ESP_ELFSYM_EXPORT(g_pm_cnt),
    ESP_ELFSYM_EXPORT(esp_http_client_init),
    ESP_ELFSYM_EXPORT(nvs_set_u8),
    ESP_ELFSYM_EXPORT(hal_enable_sta_beacon_filter),
    ESP_ELFSYM_EXPORT(handle_auth_sae),
    ESP_ELFSYM_EXPORT(nan_ndp_inactivity_timeout_process),
    ESP_ELFSYM_EXPORT(nimble_port_init),
    ESP_ELFSYM_EXPORT(rc_set_per_conn_fix_rate),
    ESP_ELFSYM_EXPORT(net80211_softap_funcs_init),
    ESP_ELFSYM_EXPORT(ic_get_keep_alive_time),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_ext_adv_init),
    ESP_ELFSYM_EXPORT(_ZTVN2tt5MutexE),
    ESP_ELFSYM_EXPORT(nvs_get_u8),
    ESP_ELFSYM_EXPORT(phy_get_dcap_degen),
    ESP_ELFSYM_EXPORT(bluetooth_midi_is_connected),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_get_peer_rpa),
    ESP_ELFSYM_EXPORT(__muldf3),
    ESP_ELFSYM_EXPORT(phy_pbus_xpd_rx_on),
    ESP_ELFSYM_EXPORT(esp_vfs_opendir),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_read_blob),
    ESP_ELFSYM_EXPORT(lv_label_ins_text),
    ESP_ELFSYM_EXPORT(spi_timing_get_flash_timing_param),
    ESP_ELFSYM_EXPORT(lv_font_get_glyph_static_bitmap),
    ESP_ELFSYM_EXPORT(AUDIO_CODEC_TYPE),
    ESP_ELFSYM_EXPORT(wpa_config_parse_string),
    ESP_ELFSYM_EXPORT(esp_io_expander_gpio_wrapper_set_direction),
    ESP_ELFSYM_EXPORT(empty_func_p_u32_r_void),
    ESP_ELFSYM_EXPORT(ieee80211_cnx_attach),
    ESP_ELFSYM_EXPORT(ieee80211_add_htcap_vendor),
    ESP_ELFSYM_EXPORT(bb_agc_reg_update),
    ESP_ELFSYM_EXPORT(lv_obj_scroll_to_y),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceIN2tt6PubSubIPvEESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(i2s_channel_get_info),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_adv_mode_ext),
    ESP_ELFSYM_EXPORT(mmu_hal_map_region),
    ESP_ELFSYM_EXPORT(r_ble_ll_reset_sca),
    ESP_ELFSYM_EXPORT(thread_get_return_code),
    ESP_ELFSYM_EXPORT(multi_heap_get_info_impl),
    ESP_ELFSYM_EXPORT(bt_bb_set_zb_tx_on_delay),
    ESP_ELFSYM_EXPORT(__gesf2),
    ESP_ELFSYM_EXPORT(_ZTISt8bad_cast),
    ESP_ELFSYM_EXPORT(ic_init),
    ESP_ELFSYM_EXPORT(r_hal_timer_read),
    ESP_ELFSYM_EXPORT(lv_color_format_has_alpha),
    ESP_ELFSYM_EXPORT(thread_set_affinity),
    ESP_ELFSYM_EXPORT(spi_flash_hal_setup_read_suspend),
    ESP_ELFSYM_EXPORT(_ZZNSt9__unicode13_Utf_iteratorIcDiPKcS2_NS_5_ReplEE12_M_read_utf8EvENKUlvE_clEv),
    ESP_ELFSYM_EXPORT(ble_phy_module_env_p),
    ESP_ELFSYM_EXPORT(i2c_controller_register8_set),
    ESP_ELFSYM_EXPORT(lv_cache_class_lru_rb_count),
    ESP_ELFSYM_EXPORT(ip_data),
    ESP_ELFSYM_EXPORT(memp_free),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_get_channel_bitmask),
    ESP_ELFSYM_EXPORT(multi_heap_realloc_impl),
    ESP_ELFSYM_EXPORT(pthread_mutex_trylock),
    ESP_ELFSYM_EXPORT(wpa_set_bss),
    ESP_ELFSYM_EXPORT(lora_can_receive),
    ESP_ELFSYM_EXPORT(owe_build_assoc_req),
    ESP_ELFSYM_EXPORT(sta_auth_shared),
    ESP_ELFSYM_EXPORT(pm_enable_send_wake_null_on_channel),
    ESP_ELFSYM_EXPORT(window_manager_stop),
    ESP_ELFSYM_EXPORT(esp_bt_controller_init),
    ESP_ELFSYM_EXPORT(lv_indev_set_long_press_time),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_read_mult),
    ESP_ELFSYM_EXPORT(esp_coex_common_semphr_give_wrapper),
    ESP_ELFSYM_EXPORT(gdma_hal_reset),
    ESP_ELFSYM_EXPORT(TRC_AMPDU_PER_UP_THRESHOLD),
    ESP_ELFSYM_EXPORT(mbedtls_psa_mac_sign_setup),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_line_points_init),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_prof_pmk_internal),
    ESP_ELFSYM_EXPORT(pvTaskIncrementMutexHeldCount),
    ESP_ELFSYM_EXPORT(ble_gattc_deinit),
    ESP_ELFSYM_EXPORT(io_mux_is_lp_io_in_use),
    ESP_ELFSYM_EXPORT(lv_font_montserrat_10),
    ESP_ELFSYM_EXPORT(tolower),
    ESP_ELFSYM_EXPORT(psa_status_to_mbedtls),
    ESP_ELFSYM_EXPORT(udp_pcbs),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_aux_data_free),
    ESP_ELFSYM_EXPORT(ieee80211_find_elem_match),
    ESP_ELFSYM_EXPORT(gdma_hal_get_intr_status_reg),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx118time_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEEE),
    ESP_ELFSYM_EXPORT(lv_style_set_recolor_opa),
    ESP_ELFSYM_EXPORT(backlight_set_brightness_default),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_on_coex_start),
    ESP_ELFSYM_EXPORT(bt_bb_corr_thresh_aa_lc_en),
    ESP_ELFSYM_EXPORT(phy_disable_low_rate),
    ESP_ELFSYM_EXPORT(uart_hal_set_hw_flow_ctrl),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_default_wifi_connecting),
    ESP_ELFSYM_EXPORT(cache_hal_writeback_addr),
    ESP_ELFSYM_EXPORT(ble_gatts_count_cfg),
    ESP_ELFSYM_EXPORT(device_unlock),
    ESP_ELFSYM_EXPORT(phy_tatget_pwr_band),
    ESP_ELFSYM_EXPORT(nl_langinfo_l),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_encrypt_buf),
    ESP_ELFSYM_EXPORT(hal_he_set_ersu_ack_rate),
    ESP_ELFSYM_EXPORT(npl_freertos_funcs_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_misc_additional_options_set),
    ESP_ELFSYM_EXPORT(pwr_hal_clear_mac_modem_state_wakeup_protect_signal),
    ESP_ELFSYM_EXPORT(lv_image_get_pivot),
    ESP_ELFSYM_EXPORT(vasprintf),
    ESP_ELFSYM_EXPORT(lv_timer_get_paused),
    ESP_ELFSYM_EXPORT(lv_textarea_set_cursor_pos),
    ESP_ELFSYM_EXPORT(pm_enable_keep_alive_timer),
    ESP_ELFSYM_EXPORT(hal_deinit),
    ESP_ELFSYM_EXPORT(spi_flash_hal_setup_auto_suspend_mode),
    ESP_ELFSYM_EXPORT(vTaskInternalSetTimeOutState),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_keypair_init),
    ESP_ELFSYM_EXPORT(rc_disable_trc),
    ESP_ELFSYM_EXPORT(lv_canvas_set_buffer),
    ESP_ELFSYM_EXPORT(rewind),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_env_init),
    ESP_ELFSYM_EXPORT(tcp_rexmit),
    ESP_ELFSYM_EXPORT(sys_check_timeouts),
    ESP_ELFSYM_EXPORT(connect_scan_flag),
    ESP_ELFSYM_EXPORT(phy_adc_rate_cal_rxdc),
    ESP_ELFSYM_EXPORT(r_scan_duplicate_cache_refresh_set_time),
    ESP_ELFSYM_EXPORT(phy_param),
    ESP_ELFSYM_EXPORT(ieee80211_send_mgmt),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt5ctypeIwEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(spicommon_dma_chan_alloc),
    ESP_ELFSYM_EXPORT(_timezone),
    ESP_ELFSYM_EXPORT(g_wpa_pmk_caching_disabled),
    ESP_ELFSYM_EXPORT(esp_transport_list_clean),
    ESP_ELFSYM_EXPORT(close_pending),
    ESP_ELFSYM_EXPORT(priv_config_opts_ptr),
    ESP_ELFSYM_EXPORT(memp_SYS_TIMEOUT),
    ESP_ELFSYM_EXPORT(ble_transport_to_hs_evt_impl),
    ESP_ELFSYM_EXPORT(esp_psram_extram_test),
    ESP_ELFSYM_EXPORT(esp_intr_noniram_enable),
    ESP_ELFSYM_EXPORT(i2c_controller_register8_set_bits),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_client_key_data),
    ESP_ELFSYM_EXPORT(wpa_sm_deinit),
    ESP_ELFSYM_EXPORT(lodepng_compress_settings_init),
    ESP_ELFSYM_EXPORT(driver_add),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_wifi_scan),
    ESP_ELFSYM_EXPORT(psa_its_set),
    ESP_ELFSYM_EXPORT(phy_get_rfcal_rxiq_data),
    ESP_ELFSYM_EXPORT(psa_load_persistent_key),
    ESP_ELFSYM_EXPORT(usb_hid_device_start),
    ESP_ELFSYM_EXPORT(lv_draw_layer_create),
    ESP_ELFSYM_EXPORT(hal_mac_fill_hwtxop),
    ESP_ELFSYM_EXPORT(lv_color_black),
    ESP_ELFSYM_EXPORT(tcp_rexmit_rto_prepare),
    ESP_ELFSYM_EXPORT(esp_http_server_dispatch_event),
    ESP_ELFSYM_EXPORT(lv_dropdown_get_selected_str),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_send_scan_timeout),
    ESP_ELFSYM_EXPORT(spi_flash_op_lock),
    ESP_ELFSYM_EXPORT(cJSON_ParseWithOpts),
    ESP_ELFSYM_EXPORT(gpio_intr_enable),
    ESP_ELFSYM_EXPORT(esp_crypto_base64_encode),
    ESP_ELFSYM_EXPORT(sdmmc_flip_byte_order),
    ESP_ELFSYM_EXPORT(dhcp_parse_extra_opts),
    ESP_ELFSYM_EXPORT(lvgl_get_text_font_height),
    ESP_ELFSYM_EXPORT(IP_EVENT),
    ESP_ELFSYM_EXPORT(ble_gap_end_test_evt),
    ESP_ELFSYM_EXPORT(gpio_sleep_set_direction),
    ESP_ELFSYM_EXPORT(dhcp_coarse_tmr),
    ESP_ELFSYM_EXPORT(pm_mesh_set_next_tbtt),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_layout),
    ESP_ELFSYM_EXPORT(wpabuf_clear_free),
    ESP_ELFSYM_EXPORT(isnanf),
    ESP_ELFSYM_EXPORT(r_hal_timer_env_deinit),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_rand_chan_from_mask),
    ESP_ELFSYM_EXPORT(lv_chart_get_point_pos_by_id),
    ESP_ELFSYM_EXPORT(mld6_input),
    ESP_ELFSYM_EXPORT(f_getfree),
    ESP_ELFSYM_EXPORT(memp_MLD6_GROUP),
    ESP_ELFSYM_EXPORT(spi_hal_clear_intr_mask),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_env_deinit),
    ESP_ELFSYM_EXPORT(http_header_get_item),
    ESP_ELFSYM_EXPORT(lv_timer_set_repeat_count),
    ESP_ELFSYM_EXPORT(r_ble_phy_txpwr_set),
    ESP_ELFSYM_EXPORT(heap_caps_check_add_region_allowed),
    ESP_ELFSYM_EXPORT(ecc_hal_start_calc),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_set_aux_data),
    ESP_ELFSYM_EXPORT(udp_bind),
    ESP_ELFSYM_EXPORT(_ZTVSt8numpunctIcE),
    ESP_ELFSYM_EXPORT(phy_bb_cfo_cfg),
    ESP_ELFSYM_EXPORT(lv_spinbox_class),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_init),
    ESP_ELFSYM_EXPORT(ble_hs_hci_evt_process),
    ESP_ELFSYM_EXPORT(crypto_bignum_add),
    ESP_ELFSYM_EXPORT(s_get_cache_state),
    ESP_ELFSYM_EXPORT(xQueueIsQueueEmptyFromISR),
    ESP_ELFSYM_EXPORT(trc_get_espnow_rate),
    ESP_ELFSYM_EXPORT(g_pm),
    ESP_ELFSYM_EXPORT(modem_clock_hal_enable_modem_private_fe_clock),
    ESP_ELFSYM_EXPORT(cache_hal_resume),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_get_addr_pointer),
    ESP_ELFSYM_EXPORT(pthread_include_pthread_rwlock_impl),
    ESP_ELFSYM_EXPORT(wdev_is_data_in_rxlist),
    ESP_ELFSYM_EXPORT(phy_pwdet_code_cal),
    ESP_ELFSYM_EXPORT(lvgl_keyboard_enable),
    ESP_ELFSYM_EXPORT(uart_controller_close),
    ESP_ELFSYM_EXPORT(ble_ll_resolv_env_p),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_block_copy),
    ESP_ELFSYM_EXPORT(i2c_controller_register16le_get),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt5ctypeIcEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(i2c_release_bus_handle),
    ESP_ELFSYM_EXPORT(g_mmgmt_max_evt_block_cnt),
    ESP_ELFSYM_EXPORT(bluetooth_get_radio_state),
    ESP_ELFSYM_EXPORT(pm_update_modem_sleep_default_params),
    ESP_ELFSYM_EXPORT(spi_flash_munmap),
    ESP_ELFSYM_EXPORT(lv_bar_get_start_value),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_read_binary_le),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_periodic_did_update),
    ESP_ELFSYM_EXPORT(ppInstallKey),
    ESP_ELFSYM_EXPORT(strcoll),
    ESP_ELFSYM_EXPORT(ppRxFragmentProc),
    ESP_ELFSYM_EXPORT(pp_process_hmac_waiting_txq),
    ESP_ELFSYM_EXPORT(offchan_tx_progress_in),
    ESP_ELFSYM_EXPORT(esp_sha256_driver_clone),
    ESP_ELFSYM_EXPORT(itoa),
    ESP_ELFSYM_EXPORT(r_filter_aux_duplicate_check_need_report),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_pdu),
    ESP_ELFSYM_EXPORT(esp_netif_get_ip_info),
    ESP_ELFSYM_EXPORT(sys_timeouts_sleeptime),
    ESP_ELFSYM_EXPORT(ble_l2cap_init),
    ESP_ELFSYM_EXPORT(lvgl_software_keyboard_set_content_area),
    ESP_ELFSYM_EXPORT(spi_hal_hw_prepare_tx),
    ESP_ELFSYM_EXPORT(phy_rxiq_opt),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_write),
    ESP_ELFSYM_EXPORT(crypto_ec_point_add),
    ESP_ELFSYM_EXPORT(sae_derive_pt),
    ESP_ELFSYM_EXPORT(lmac_record_txtime),
    ESP_ELFSYM_EXPORT(lv_fs_tell),
    ESP_ELFSYM_EXPORT(g_ble_ll_whitelist_deleted_irk_hdr),
    ESP_ELFSYM_EXPORT(lv_style_set_pad_right),
    ESP_ELFSYM_EXPORT(_lock_close),
    ESP_ELFSYM_EXPORT(esp_wifi_get_hostap_private_internal),
    ESP_ELFSYM_EXPORT(pmksa_cache_add_entry),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_init),
    ESP_ELFSYM_EXPORT(ble_hs_hci_rx_evt),
    ESP_ELFSYM_EXPORT(tzname),
    ESP_ELFSYM_EXPORT(lv_label_get_letter_on),
    ESP_ELFSYM_EXPORT(lvgl_software_keyboard_hide),
    ESP_ELFSYM_EXPORT(esp_aes_cipher_set_iv),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_apply_multi_conn_slot),
    ESP_ELFSYM_EXPORT(g_flash_guard_default_ops),
    ESP_ELFSYM_EXPORT(wifi_gpio_debug),
    ESP_ELFSYM_EXPORT(lv_draw_layer_init),
    ESP_ELFSYM_EXPORT(lv_draw_arc_dsc_init),
    ESP_ELFSYM_EXPORT(sta_csa_timer),
    ESP_ELFSYM_EXPORT(ppCalTxAMPDULength),
    ESP_ELFSYM_EXPORT(ieee80211_add_wme_param),
    ESP_ELFSYM_EXPORT(lv_display_set_offset),
    ESP_ELFSYM_EXPORT(__umoddi3),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_bitstring_null),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_timeout_urgency_compare),
    ESP_ELFSYM_EXPORT(sae_hash),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_conn_cte_rsp_enable),
    ESP_ELFSYM_EXPORT(nimble_port_freertos_init),
    ESP_ELFSYM_EXPORT(nd6_adjust_mld_membership),
    ESP_ELFSYM_EXPORT(xRingbufferReceive),
    ESP_ELFSYM_EXPORT(rcGet11BHighestRateIdx),
    ESP_ELFSYM_EXPORT(esp_aes_crypt_ofb),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_mode_from_ciphersuite),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_add),
    ESP_ELFSYM_EXPORT(he_send_action_bsscolor_inuse),
    ESP_ELFSYM_EXPORT(lv_obj_is_visible),
    ESP_ELFSYM_EXPORT(lv_draw_sw_border),
    ESP_ELFSYM_EXPORT(regdma_link_new_branch_continuous),
    ESP_ELFSYM_EXPORT(ble_sm_gen_pair_rand),
    ESP_ELFSYM_EXPORT(lodepng_inspect),
    ESP_ELFSYM_EXPORT(eloop_cancel_timeout),
    ESP_ELFSYM_EXPORT(phy_rfrx_gain_index),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_sig_alg),
    ESP_ELFSYM_EXPORT(wdev_set_promis_misc_pkt),
    ESP_ELFSYM_EXPORT(nd6_reachability_hint),
    ESP_ELFSYM_EXPORT(modff),
    ESP_ELFSYM_EXPORT(iswalpha),
    ESP_ELFSYM_EXPORT(lv_trigo_cos),
    ESP_ELFSYM_EXPORT(rtc_clk_freq_to_period),
    ESP_ELFSYM_EXPORT(esp_netif_set_ip_info),
    ESP_ELFSYM_EXPORT(_ZTVSt23__codecvt_abstract_baseIDiDu10_mbstate_tE),
    ESP_ELFSYM_EXPORT(ets_timer_done),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSdD0Ev),
    ESP_ELFSYM_EXPORT(r_ble_hw_whitelist_check_in_wl),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_newconn),
    ESP_ELFSYM_EXPORT(txop_max_list),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_periodic_check_data_itvl),
    ESP_ELFSYM_EXPORT(ble_controller_scan_duplicate_config),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_parse_finished),
    ESP_ELFSYM_EXPORT(__retarget_lock_init),
    ESP_ELFSYM_EXPORT(module_start),
    ESP_ELFSYM_EXPORT(lv_switch_create),
    ESP_ELFSYM_EXPORT(__gedf2),
    ESP_ELFSYM_EXPORT(lv_style_set_shadow_offset_y),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_spiflash_mount_ro),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_cmd_rx),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_set_phy),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_set_group_mgmt_cipher_internal),
    ESP_ELFSYM_EXPORT(lv_draw_glyph_dsc_init),
    ESP_ELFSYM_EXPORT(pointer_get_mirror_x),
    ESP_ELFSYM_EXPORT(lv_line_set_points_mutable),
    ESP_ELFSYM_EXPORT(ble_hs_adv_set_fields),
    ESP_ELFSYM_EXPORT(pm_disconnected_stop),
    ESP_ELFSYM_EXPORT(lv_display_set_color_format),
    ESP_ELFSYM_EXPORT(wifi_softap_set_config),
    ESP_ELFSYM_EXPORT(he_twt_teardown_txcb),
    ESP_ELFSYM_EXPORT(usb_msc_device_stop),
    ESP_ELFSYM_EXPORT(file_system_add),
    ESP_ELFSYM_EXPORT(lv_textarea_create),
    ESP_ELFSYM_EXPORT(controller_mem_ptr_check),
    ESP_ELFSYM_EXPORT(spi_bus_lock_acquire_end),
    ESP_ELFSYM_EXPORT(owe_build_dhie),
    ESP_ELFSYM_EXPORT(r_ble_ll_utils_remapped_channel),
    ESP_ELFSYM_EXPORT(tt_app_fileselection_get_result_path),
    ESP_ELFSYM_EXPORT(thread_set_stack_size),
    ESP_ELFSYM_EXPORT(lv_pow),
    ESP_ELFSYM_EXPORT(ble_hs_adv_find_field),
    ESP_ELFSYM_EXPORT(mbedtls_psa_asymmetric_decrypt),
    ESP_ELFSYM_EXPORT(aes_hal_read_iv),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_get_caps),
    ESP_ELFSYM_EXPORT(_ZTISt9exception),
    ESP_ELFSYM_EXPORT(g_cte_sample_limit),
    ESP_ELFSYM_EXPORT(esp_transport_capture_errno),
    ESP_ELFSYM_EXPORT(system_event_emit),
    ESP_ELFSYM_EXPORT(ieee80211_sta_new_state),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_env_init),
    ESP_ELFSYM_EXPORT(lwip_getpeername),
    ESP_ELFSYM_EXPORT(__ctzsi2),
    ESP_ELFSYM_EXPORT(thread_start),
    ESP_ELFSYM_EXPORT(ble_stack_disable),
    ESP_ELFSYM_EXPORT(uart_wait_tx_done),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_env_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_list_add),
    ESP_ELFSYM_EXPORT(esp_event_isr_post),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_find),
    ESP_ELFSYM_EXPORT(ieee80211_add_csa),
    ESP_ELFSYM_EXPORT(phy_set_bttx_low_power),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_switch_delay_rx_aoa_1us),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_coex_dpc_update_on_event_scheduled),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_to_mont_rep),
    ESP_ELFSYM_EXPORT(ble_freertos_co_pool),
    ESP_ELFSYM_EXPORT(mbedtls_sha256_free),
    ESP_ELFSYM_EXPORT(esp_elf_arch_relocate),
    ESP_ELFSYM_EXPORT(esp_read_mac),
    ESP_ELFSYM_EXPORT(esp_sha_set_mode),
    ESP_ELFSYM_EXPORT(djb2_str),
    ESP_ELFSYM_EXPORT(bundle_get_bool),
    ESP_ELFSYM_EXPORT(timer_get_expiry_time),
    ESP_ELFSYM_EXPORT(esp_coex_common_semphr_take_wrapper),
    ESP_ELFSYM_EXPORT(phy_rx_11b_opt),
    ESP_ELFSYM_EXPORT(lwip_ioctl),
    ESP_ELFSYM_EXPORT(phy_write_rfpll_sdm),
    ESP_ELFSYM_EXPORT(ieee80211_hostap_send_beacon_process),
    ESP_ELFSYM_EXPORT(multi_heap_restore_minimum_free_bytes),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_margin_left),
    ESP_ELFSYM_EXPORT(difftime),
    ESP_ELFSYM_EXPORT(lora_remove_tx_callback),
    ESP_ELFSYM_EXPORT(nan_ndp_setup_timeout_process),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_line_angle_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_enc_allowed_pdu_rx),
    ESP_ELFSYM_EXPORT(properties_file_for_each),
    ESP_ELFSYM_EXPORT(esp_sha_512_t_init_hash),
    ESP_ELFSYM_EXPORT(ieee80211_wme_standard_ac_to_esp_ac),
    ESP_ELFSYM_EXPORT(module_resolve_symbol_global),
    ESP_ELFSYM_EXPORT(i2s_del_channel),
    ESP_ELFSYM_EXPORT(xTimerIsTimerActive),
    ESP_ELFSYM_EXPORT(ble_gap_rx_rd_rem_sup_feat_complete),
    ESP_ELFSYM_EXPORT(lv_obj_invalidate_area),
    ESP_ELFSYM_EXPORT(esp_wifi_set_protocol),
    ESP_ELFSYM_EXPORT(esp32_gpio_driver),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_low_prio_src_set),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_pdu_info_calculate),
    ESP_ELFSYM_EXPORT(nd6_get_destination_mtu),
    ESP_ELFSYM_EXPORT(mbedtls_aes_xts_init),
    ESP_ELFSYM_EXPORT(ieee80211_raw_frame_sanity_check),
    ESP_ELFSYM_EXPORT(bluetooth_remove_event_callback),
    ESP_ELFSYM_EXPORT(action_q),
    ESP_ELFSYM_EXPORT(set_rx_sense),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_release),
    ESP_ELFSYM_EXPORT(esp_netif_get_handle_from_ifkey),
    ESP_ELFSYM_EXPORT(lv_arc_set_range),
    ESP_ELFSYM_EXPORT(hal_init_tb_tx),
    ESP_ELFSYM_EXPORT(s_wifi_stop_in_progress),
    ESP_ELFSYM_EXPORT(hal_he_get_mplen_addr_end),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_release_ds_lock),
    ESP_ELFSYM_EXPORT(mbedtls_net_free),
    ESP_ELFSYM_EXPORT(uart_flush_input),
    ESP_ELFSYM_EXPORT(ieee80211_find_ie_match),
    ESP_ELFSYM_EXPORT(esp_sha1_driver_update),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_clear_intr),
    ESP_ELFSYM_EXPORT(app_manager_get_state),
    ESP_ELFSYM_EXPORT(ieee80211_alloc_tx_buf),
    ESP_ELFSYM_EXPORT(esp_task_wdt_impl_timer_allocate),
    ESP_ELFSYM_EXPORT(rx11AXRate2AMPDULimit_update),
    ESP_ELFSYM_EXPORT(__pow5Factor),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service11displayidle24BouncingBallsScreensaverE),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_get_priv_addr),
    ESP_ELFSYM_EXPORT(ble_hs_is_enabled),
    ESP_ELFSYM_EXPORT(_i2s_hal_set_rx_clock),
    ESP_ELFSYM_EXPORT(sys_arch_sem_wait),
    ESP_ELFSYM_EXPORT(ulTaskGenericNotifyTake),
    ESP_ELFSYM_EXPORT(vQueueSetQueueNumber),
    ESP_ELFSYM_EXPORT(wdevProcessRxSucDataAll),
    ESP_ELFSYM_EXPORT(ble_gap_rx_adv_report),
    ESP_ELFSYM_EXPORT(pwm_is_inverted),
    ESP_ELFSYM_EXPORT(nan_dp_search_node),
    ESP_ELFSYM_EXPORT(__wrap_lv_dropdown_create),
    ESP_ELFSYM_EXPORT(esp_timer_is_active),
    ESP_ELFSYM_EXPORT(lv_display_get_physical_horizontal_resolution),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_aux_calculate),
    ESP_ELFSYM_EXPORT(sys_sem_new),
    ESP_ELFSYM_EXPORT(btdm_broker_init),
    ESP_ELFSYM_EXPORT(esp32c5_eco3_rom_ptr_init),
    ESP_ELFSYM_EXPORT(sqrt),
    ESP_ELFSYM_EXPORT(r_ble_lll_rxed_buffer_is_last_one),
    ESP_ELFSYM_EXPORT(lv_anim_refr_now),
    ESP_ELFSYM_EXPORT(phy_get_chan_power_offset),
    ESP_ELFSYM_EXPORT(wlanif_input),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_color_to_l8),
    ESP_ELFSYM_EXPORT(phy_get_dco_comp),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_read_transmit_power_level),
    ESP_ELFSYM_EXPORT(__cxa_free_exception),
    ESP_ELFSYM_EXPORT(lv_draw_sw_rgb565_swap),
    ESP_ELFSYM_EXPORT(r_ble_lll_per_adv_coex_dpc_process),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_beacon_miss_timeout),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_sm_start),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_free_event),
    ESP_ELFSYM_EXPORT(ieee80211_add_ulmu_pwrcap),
    ESP_ELFSYM_EXPORT(ieee80211_ethbroadcast),
    ESP_ELFSYM_EXPORT(ppRegisterTxCallback),
    ESP_ELFSYM_EXPORT(mbedtls_hmac_drbg_seed_buf),
    ESP_ELFSYM_EXPORT(lv_area_is_on),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_inv_mod_even_in_range),
    ESP_ELFSYM_EXPORT(lmacTxFrame),
    ESP_ELFSYM_EXPORT(esp_cache_unfreeze_ext_mem_cache),
    ESP_ELFSYM_EXPORT(bundle_opt_int64),
    ESP_ELFSYM_EXPORT(ble_gatts_peer_cl_sup_feat_update),
    ESP_ELFSYM_EXPORT(ieee80211_add_xrates),
    ESP_ELFSYM_EXPORT(phy_set_rxclk_en),
    ESP_ELFSYM_EXPORT(eTaskGetState),
    ESP_ELFSYM_EXPORT(itwt_setup_timeout_fn_process),
    ESP_ELFSYM_EXPORT(lvgl_display_remove),
    ESP_ELFSYM_EXPORT(wifi_funcs),
    ESP_ELFSYM_EXPORT(strtok_r),
    ESP_ELFSYM_EXPORT(nimble_mem_free),
    ESP_ELFSYM_EXPORT(wpa_set_pmk),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_init),
    ESP_ELFSYM_EXPORT(timer_set_callback_priority),
    ESP_ELFSYM_EXPORT(ble_hs_sched_reset),
    ESP_ELFSYM_EXPORT(wpa_add_kde),
    ESP_ELFSYM_EXPORT(esp_deep_sleep_register_hook),
    ESP_ELFSYM_EXPORT(lv_indev_scroll_throw_handler),
    ESP_ELFSYM_EXPORT(ieee80211_add_heopr),
    ESP_ELFSYM_EXPORT(wdev_process_beacon_filter),
    ESP_ELFSYM_EXPORT(crypto_bignum_init),
    ESP_ELFSYM_EXPORT(_ZTVSt13runtime_error),
    ESP_ELFSYM_EXPORT(dns_gethostbyname_addrtype),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_find_info),
    ESP_ELFSYM_EXPORT(pm_set_sleep_min_active_time),
    ESP_ELFSYM_EXPORT(_ZSt14__ostream_fillIcSt11char_traitsIcEEvRSt13basic_ostreamIT_T0_Ei),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_start),
    ESP_ELFSYM_EXPORT(_ZTVSt18__moneypunct_cacheIwLb0EE),
    ESP_ELFSYM_EXPORT(mem_calloc),
    ESP_ELFSYM_EXPORT(lv_obj_add_style),
    ESP_ELFSYM_EXPORT(g_btwt_num),
    ESP_ELFSYM_EXPORT(ieee80211_phy_init),
    ESP_ELFSYM_EXPORT(_ZSt19__throw_ios_failurePKci),
    ESP_ELFSYM_EXPORT(sta_recv_assoc),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_max_sta_conn),
    ESP_ELFSYM_EXPORT(mbedtls_to_psa_error),
    ESP_ELFSYM_EXPORT(lv_obj_has_flag),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_pb_adv_link_id_table_add),
    ESP_ELFSYM_EXPORT(pm_enable_disconnected_sleep_delay_timer),
    ESP_ELFSYM_EXPORT(os_timer_arm),
    ESP_ELFSYM_EXPORT(psa_generate_key_custom),
    ESP_ELFSYM_EXPORT(esp_md5_hash_abort),
    ESP_ELFSYM_EXPORT(_Z8init_mtkP6Device),
    ESP_ELFSYM_EXPORT(_ZTVSt7num_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE),
    ESP_ELFSYM_EXPORT(ieee80211_add_tie_param),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_mem_reset),
    ESP_ELFSYM_EXPORT(spi_device_transmit),
    ESP_ELFSYM_EXPORT(lv_obj_get_scrollbar_area),
    ESP_ELFSYM_EXPORT(esp_test_tx_process_txq_state),
    ESP_ELFSYM_EXPORT(lv_obj_get_content_coords),
    ESP_ELFSYM_EXPORT(lv_checkbox_get_text),
    ESP_ELFSYM_EXPORT(r_DB_hash_function_blob),
    ESP_ELFSYM_EXPORT(ble_sm_peer_pair_rand),
    ESP_ELFSYM_EXPORT(device_get_first_active_by_type),
    ESP_ELFSYM_EXPORT(cnx_remove_from_blacklist),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_should_resume_in_isr),
    ESP_ELFSYM_EXPORT(_Znaj),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_sae_ext_config_internal),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_prepare_tx_pdu),
    ESP_ELFSYM_EXPORT(pbuf_copy_partial),
    ESP_ELFSYM_EXPORT(esp_task_wdt_impl_timeout_triggered),
    ESP_ELFSYM_EXPORT(regulatory_data),
    ESP_ELFSYM_EXPORT(__ubsan_include),
    ESP_ELFSYM_EXPORT(lv_bar_get_min_value),
    ESP_ELFSYM_EXPORT(ieee80211_freedom_init),
    ESP_ELFSYM_EXPORT(lv_indev_get_active_obj),
    ESP_ELFSYM_EXPORT(psa_import_key),
    ESP_ELFSYM_EXPORT(tsf_hal_set_modem_close_rf_delay_cycles),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_get_io_mode),
    ESP_ELFSYM_EXPORT(lv_timer_get_time_until_next),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_set_enable),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_list_rmv),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt8numpunctIwEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(ftm_responder_cleanup),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_periodic_set_data),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_is_connected),
    ESP_ELFSYM_EXPORT(phy_pbus_rd_addr),
    ESP_ELFSYM_EXPORT(app_manager_stop),
    ESP_ELFSYM_EXPORT(mac_tx_set_plcp1),
    ESP_ELFSYM_EXPORT(ecp_mul_restartable_internal),
    ESP_ELFSYM_EXPORT(rtc_clk_set_cpu_switch_to_pll),
    ESP_ELFSYM_EXPORT(wpa_sm_disassociate),
    ESP_ELFSYM_EXPORT(pm_on_isr),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx117collateIwEE),
    ESP_ELFSYM_EXPORT(lvgl_software_keyboard_is_enabled),
    ESP_ELFSYM_EXPORT(wifi_nvs_deinit),
    ESP_ELFSYM_EXPORT(ieee80211_ccmp_decrypt),
    ESP_ELFSYM_EXPORT(ieee80211_output_init),
    ESP_ELFSYM_EXPORT(__wrap___register_frame),
    ESP_ELFSYM_EXPORT(lv_draw_mask_rect_dsc_init),
    ESP_ELFSYM_EXPORT(esp_random),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_exp_mod),
    ESP_ELFSYM_EXPORT(i2s_alloc_dma_desc),
    ESP_ELFSYM_EXPORT(WIFI_MESH_EVENT),
    ESP_ELFSYM_EXPORT(ieee80211_parse_vhtopr),
    ESP_ELFSYM_EXPORT(phy_pbus_set_dco),
    ESP_ELFSYM_EXPORT(esp_crypto_aes_gcm_abort),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_add_text_button_action),
    ESP_ELFSYM_EXPORT(phy_get_romfuncs),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_send_dir_adv_report),
    ESP_ELFSYM_EXPORT(lv_area_set_width),
    ESP_ELFSYM_EXPORT(pm_enable_beacon_monitor_timer),
    ESP_ELFSYM_EXPORT(mld6_joingroup),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_update_new_conn_param),
    ESP_ELFSYM_EXPORT(esp_err_to_name),
    ESP_ELFSYM_EXPORT(nan_scan_timeout_process),
    ESP_ELFSYM_EXPORT(gpio_output_disable),
    ESP_ELFSYM_EXPORT(hostapd_config_free_bss),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_enable),
    ESP_ELFSYM_EXPORT(ble_midi_get_active),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_rx_pkt_in),
    ESP_ELFSYM_EXPORT(r_ble_ll_set_ext_scan_params),
    ESP_ELFSYM_EXPORT(xPortSetInterruptMaskFromISR),
    ESP_ELFSYM_EXPORT(lv_obj_transform_point),
    ESP_ELFSYM_EXPORT(http_utils_trim_whitespace),
    ESP_ELFSYM_EXPORT(esp_http_client_set_post_field),
    ESP_ELFSYM_EXPORT(ble_hs_hci_deinit),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_a2dp_wifi_conn),
    ESP_ELFSYM_EXPORT(mmu_hal_paddr_to_vaddr),
    ESP_ELFSYM_EXPORT(_Z30ble_scan_abort_name_resolutionP6Device),
    ESP_ELFSYM_EXPORT(lv_fs_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_utils_calc_num_used_chans),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_auth_pyld_timer_start),
    ESP_ELFSYM_EXPORT(phy_ble_power_config),
    ESP_ELFSYM_EXPORT(event_group_wait),
    ESP_ELFSYM_EXPORT(lv_screen_load),
    ESP_ELFSYM_EXPORT(lv_obj_scrollbar_invalidate),
    ESP_ELFSYM_EXPORT(ccmp_decrypt),
    ESP_ELFSYM_EXPORT(lv_label_set_long_mode),
    ESP_ELFSYM_EXPORT(chm_is_at_home_channel),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_rx_buffer),
    ESP_ELFSYM_EXPORT(lv_list_class),
    ESP_ELFSYM_EXPORT(esp_psram_extram_add_to_heap_allocator),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_tx_buffer_hdr_chain),
    ESP_ELFSYM_EXPORT(ledc_set_fade),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_reset_link_state),
    ESP_ELFSYM_EXPORT(st7796_driver),
    ESP_ELFSYM_EXPORT(wifi_nvs_get_low_rate_enable),
    ESP_ELFSYM_EXPORT(__cxa_begin_catch),
    ESP_ELFSYM_EXPORT(pm_rx_beacon_process),
    ESP_ELFSYM_EXPORT(mbedtls_threading_psa_globaldata_mutex),
    ESP_ELFSYM_EXPORT(lv_inv_area),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_is_inside_window),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_transform_init),
    ESP_ELFSYM_EXPORT(adc2_wifi_release),
    ESP_ELFSYM_EXPORT(tcp_slowtmr),
    ESP_ELFSYM_EXPORT(ieee80211_adjust_2nd_chan),
    ESP_ELFSYM_EXPORT(r_bt_rf_coex_hook_st_set),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_recycle_adv_event_end),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_a2dp_wifi_connecting),
    ESP_ELFSYM_EXPORT(vSemaphoreDeleteWithCaps),
    ESP_ELFSYM_EXPORT(lodepng_decode_memory),
    ESP_ELFSYM_EXPORT(scan_reset_cipher_and_akm),
    ESP_ELFSYM_EXPORT(esp_sha256_driver_abort),
    ESP_ELFSYM_EXPORT(lv_obj_get_index_by_type),
    ESP_ELFSYM_EXPORT(lwip_read),
    ESP_ELFSYM_EXPORT(r_bt_rf_coex_dft_pti_set),
    ESP_ELFSYM_EXPORT(mbedtls_x509_oid_get_sig_alg),
    ESP_ELFSYM_EXPORT(dhcp_supplied_address),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_channel_class_info_update),
    ESP_ELFSYM_EXPORT(spi_bus_lock_bg_check_dev_req),
    ESP_ELFSYM_EXPORT(icmp_dest_unreach),
    ESP_ELFSYM_EXPORT(esp_libc_timekeeping_get_realtime_us),
    ESP_ELFSYM_EXPORT(modem_clock_hal_deselect_all_coex_lpclk_source),
    ESP_ELFSYM_EXPORT(lvgl_keyboard_on_stop_lvgl),
    ESP_ELFSYM_EXPORT(bundle_has_int32),
    ESP_ELFSYM_EXPORT(rtc_gpio_init),
    ESP_ELFSYM_EXPORT(_ZTVSt8time_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE),
    ESP_ELFSYM_EXPORT(bt_agc_v2_set),
    ESP_ELFSYM_EXPORT(hmac_md5_vector),
    ESP_ELFSYM_EXPORT(crypto_ec_prime_len_bits),
    ESP_ELFSYM_EXPORT(uart_driver_install),
    ESP_ELFSYM_EXPORT(_double_byte),
    ESP_ELFSYM_EXPORT(phy_band_i2c_set),
    ESP_ELFSYM_EXPORT(httpd_uri_match_wildcard),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_conn_param_req),
    ESP_ELFSYM_EXPORT(netif_list),
    ESP_ELFSYM_EXPORT(uxTopUsedPriority),
    ESP_ELFSYM_EXPORT(crypto_ec_get_order),
    ESP_ELFSYM_EXPORT(esp_cmac_update),
    ESP_ELFSYM_EXPORT(gpio_isr_handler_remove),
    ESP_ELFSYM_EXPORT(wifi_nan_reg_rxcb),
    ESP_ELFSYM_EXPORT(driver_get_device_type),
    ESP_ELFSYM_EXPORT(xQueueCreateMutex),
    ESP_ELFSYM_EXPORT(i2c_master_read_from_device),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_coex_dpc_process),
    ESP_ELFSYM_EXPORT(lv_color_hsv_to_rgb),
    ESP_ELFSYM_EXPORT(pointer_get_swap_xy),
    ESP_ELFSYM_EXPORT(r_ble_phy_rate_to_phy),
    ESP_ELFSYM_EXPORT(properties_file_close),
    ESP_ELFSYM_EXPORT(g_msys_ctrl_conn_mbuf_cnt),
    ESP_ELFSYM_EXPORT(ieee80211_is_tx_allowed),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_RD_DIS_BLOCK_KEY2),
    ESP_ELFSYM_EXPORT(_ZTVSt17__timepunct_cacheIwE),
    ESP_ELFSYM_EXPORT(heap_caps_calloc),
    ESP_ELFSYM_EXPORT(g_pm_cfg),
    ESP_ELFSYM_EXPORT(g_mesh_self_organized),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_state_wakeup_protect_early_time),
    ESP_ELFSYM_EXPORT(service_instance_get_data),
    ESP_ELFSYM_EXPORT(hmac_hal_next_block_normal),
    ESP_ELFSYM_EXPORT(ieee80211_find_ie),
    ESP_ELFSYM_EXPORT(sd_pwr_ctrl_set_io_voltage),
    ESP_ELFSYM_EXPORT(esp_wifi_get_spp_attrubute_internal),
    ESP_ELFSYM_EXPORT(_ZTVSd),
    ESP_ELFSYM_EXPORT(scan_stack_enable),
    ESP_ELFSYM_EXPORT(ieee80211_sta_disconnect),
    ESP_ELFSYM_EXPORT(crypt_encrypt),
    ESP_ELFSYM_EXPORT(gpio_matrix_output),
    ESP_ELFSYM_EXPORT(uxQueueMessagesWaitingFromISR),
    ESP_ELFSYM_EXPORT(r_ble_ll_task),
    ESP_ELFSYM_EXPORT(esp_cpu_intr_get_desc),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_init),
    ESP_ELFSYM_EXPORT(psa_verify_hash),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_has_been_executed),
    ESP_ELFSYM_EXPORT(_ZSt7getlineIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EES4_),
    ESP_ELFSYM_EXPORT(esp_psram_init),
    ESP_ELFSYM_EXPORT(lv_obj_init_draw_label_dsc),
    ESP_ELFSYM_EXPORT(lv_dpx),
    ESP_ELFSYM_EXPORT(ieee80211_deauth_construct),
    ESP_ELFSYM_EXPORT(spi_bus_lock_touch),
    ESP_ELFSYM_EXPORT(coex_hw_timer_set),
    ESP_ELFSYM_EXPORT(nvs_get_i8),
    ESP_ELFSYM_EXPORT(sms4),
    ESP_ELFSYM_EXPORT(hal_set_wifi_default_pti),
    ESP_ELFSYM_EXPORT(i2s_channel_register_event_callback),
    ESP_ELFSYM_EXPORT(lv_strdup),
    ESP_ELFSYM_EXPORT(pm_twt_on_tsf_timer),
    ESP_ELFSYM_EXPORT(efuse_hal_get_major_chip_version),
    ESP_ELFSYM_EXPORT(g_wdev_record_t1t4_cb),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_txpwr_set),
    ESP_ELFSYM_EXPORT(ble_midi_stop_keepalive),
    ESP_ELFSYM_EXPORT(esp_sleep_pd_config),
    ESP_ELFSYM_EXPORT(ieee80211_add_hecap),
    ESP_ELFSYM_EXPORT(adiList_stack_initEnv),
    ESP_ELFSYM_EXPORT(mspi_timing_enter_high_speed_mode),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx119money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEE),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_transform_pivot_y),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_datalen_upd_make),
    ESP_ELFSYM_EXPORT(mbedtls_calloc),
    ESP_ELFSYM_EXPORT(lora_set_parameter),
    ESP_ELFSYM_EXPORT(usb_device_controller_begin_claim),
    ESP_ELFSYM_EXPORT(rcUpdatePhyMode),
    ESP_ELFSYM_EXPORT(phy_reg_init_new),
    ESP_ELFSYM_EXPORT(lv_obj_move_to),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_env_deinit),
    ESP_ELFSYM_EXPORT(tkip),
    ESP_ELFSYM_EXPORT(wDev_reset_bcnSendTick),
    ESP_ELFSYM_EXPORT(lv_font_get_line_height),
    ESP_ELFSYM_EXPORT(arr_stack_enableMultiConnVsCmd),
    ESP_ELFSYM_EXPORT(_ZTSSt8bad_cast),
    ESP_ELFSYM_EXPORT(lv_layer_reset),
    ESP_ELFSYM_EXPORT(hal_set_sta_tsf_wakeup),
    ESP_ELFSYM_EXPORT(esp_clk_tree_rc_fast_get_freq_hz),
    ESP_ELFSYM_EXPORT(cJSON_Parse),
    ESP_ELFSYM_EXPORT(ieee80211_btwt_teardown_txcb),
    ESP_ELFSYM_EXPORT(lv_fs_drv_init),
    ESP_ELFSYM_EXPORT(hal_enable_tsf_timer_wakeup),
    ESP_ELFSYM_EXPORT(wdev_is_mgmt_hwdecr_enabled),
    ESP_ELFSYM_EXPORT(cJSON_IsString),
    ESP_ELFSYM_EXPORT(_ZZNKSt8__format14__formatter_fpIcE6formatIdNS_10_Sink_iterIcEEEENSt20basic_format_contextIT0_cE8iteratorET_RS7_ENKUlPcSB_E_clESB_SB_),
    ESP_ELFSYM_EXPORT(lv_image_cache_is_enabled),
    ESP_ELFSYM_EXPORT(http_parser_pause),
    ESP_ELFSYM_EXPORT(pmu_sleep_disable_regdma_backup),
    ESP_ELFSYM_EXPORT(__wrap_lv_button_create),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_set_lengths),
    ESP_ELFSYM_EXPORT(esp_tls_internal_event_tracker_create),
    ESP_ELFSYM_EXPORT(_ZSt22__verify_grouping_implPKcjS0_j),
    ESP_ELFSYM_EXPORT(esp_flash_app_init_os_functions),
    ESP_ELFSYM_EXPORT(pm_update_params),
    ESP_ELFSYM_EXPORT(esp_wifi_send_mgmt_frm_internal),
    ESP_ELFSYM_EXPORT(rv_core_critical_regs_restore),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_sign_det_ext),
    ESP_ELFSYM_EXPORT(lv_indev_get_driver_data),
    ESP_ELFSYM_EXPORT(wcslcpy),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sec_chan_pdu_make),
    ESP_ELFSYM_EXPORT(esp_sha1_driver_abort),
    ESP_ELFSYM_EXPORT(pp_register_net80211_tx_cb),
    ESP_ELFSYM_EXPORT(phy_set_chan_interp),
    ESP_ELFSYM_EXPORT(modem_clock_device_context),
    ESP_ELFSYM_EXPORT(ble_gatts_reset),
    ESP_ELFSYM_EXPORT(hid_report_map_mouse),
    ESP_ELFSYM_EXPORT(ble_freertos_sem_pool),
    ESP_ELFSYM_EXPORT(timer_free),
    ESP_ELFSYM_EXPORT(r_ble_ll_calc_ticks_per_slot),
    ESP_ELFSYM_EXPORT(multi_heap_get_info),
    ESP_ELFSYM_EXPORT(esp_http_client_symbols),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_get_max_buffer_number),
    ESP_ELFSYM_EXPORT(mbedtls_md_setup),
    ESP_ELFSYM_EXPORT(r_ble_ll_pdu_tx_cte_time_get),
    ESP_ELFSYM_EXPORT(ble_store_overflow_event),
    ESP_ELFSYM_EXPORT(lv_button_class),
    ESP_ELFSYM_EXPORT(dns_gethostbyname_addrtype_n),
    ESP_ELFSYM_EXPORT(__locale_names),
    ESP_ELFSYM_EXPORT(spi_flash_encryption_hal_check),
    ESP_ELFSYM_EXPORT(lv_draw_buf_goto_xy),
    ESP_ELFSYM_EXPORT(g_he_max_apep_length_tab),
    ESP_ELFSYM_EXPORT(_ZTVSt18__moneypunct_cacheIcLb0EE),
    ESP_ELFSYM_EXPORT(pbuf_realloc),
    ESP_ELFSYM_EXPORT(r_ble_ll_mtrack_reset),
    ESP_ELFSYM_EXPORT(r_ble_lll_append_rx_buffer),
    ESP_ELFSYM_EXPORT(supplicant_sta_conn_handler),
    ESP_ELFSYM_EXPORT(pmu_hal_hp_set_sleep_modem_backup_disable),
    ESP_ELFSYM_EXPORT(SDCARD_TYPE),
    ESP_ELFSYM_EXPORT(hal_he_set_multi_bssid),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_grid_cell_row_pos),
    ESP_ELFSYM_EXPORT(r_ble_ll_env_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_reset),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_read_blob),
    ESP_ELFSYM_EXPORT(nvs_set_blob),
    ESP_ELFSYM_EXPORT(phy_set_tx_gain_mem),
    ESP_ELFSYM_EXPORT(pm_go_to_sleep),
    ESP_ELFSYM_EXPORT(_ZTISt11logic_error),
    ESP_ELFSYM_EXPORT(sta_reset_beacon_timeout),
    ESP_ELFSYM_EXPORT(ppProcTxCallback),
    ESP_ELFSYM_EXPORT(esp_http_client_cancel_request),
    ESP_ELFSYM_EXPORT(adc_controller_read_raw),
    ESP_ELFSYM_EXPORT(esp_test_get_rx_statistics),
    ESP_ELFSYM_EXPORT(open),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_wifi_connecting),
    ESP_ELFSYM_EXPORT(s_tx_cacheq),
    ESP_ELFSYM_EXPORT(lv_obj_scroll_to_view),
    ESP_ELFSYM_EXPORT(crypto_bignum_addmod),
    ESP_ELFSYM_EXPORT(POINTER_TYPE),
    ESP_ELFSYM_EXPORT(http_header_get),
    ESP_ELFSYM_EXPORT(bluetooth_serial_start),
    ESP_ELFSYM_EXPORT(_stat_r),
    ESP_ELFSYM_EXPORT(lv_anim_set_start_cb),
    ESP_ELFSYM_EXPORT(wifi_destroy_nan),
    ESP_ELFSYM_EXPORT(tcp_pcb_purge),
    ESP_ELFSYM_EXPORT(xPortCheckValidListMem),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_find_available),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_duration_period_timers_stop),
    ESP_ELFSYM_EXPORT(esp_aes_crypt_cfb128),
    ESP_ELFSYM_EXPORT(heap_caps_get_minimum_free_size),
    ESP_ELFSYM_EXPORT(ieee80211_vht_get_vhtcap_ie),
    ESP_ELFSYM_EXPORT(lv_malloc),
    ESP_ELFSYM_EXPORT(uECC_vli_modMult_fast),
    ESP_ELFSYM_EXPORT(r_ble_ll_modify_sca_action),
    ESP_ELFSYM_EXPORT(esp_supplicant_deinit),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_dup_check_ext),
    ESP_ELFSYM_EXPORT(__dtox_engine),
    ESP_ELFSYM_EXPORT(spi_flash_op_unlock),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_calc_reliable_rssi),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_get_rpa_tmo),
    ESP_ELFSYM_EXPORT(_ZTVN3nvs12NVSPartitionE),
    ESP_ELFSYM_EXPORT(phy_set_rx_gain_table),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_rsassa_pss_verify_ext),
    ESP_ELFSYM_EXPORT(psa_asymmetric_decrypt),
    ESP_ELFSYM_EXPORT(ff_disk_write),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_tbtt_auto_period_disable),
    ESP_ELFSYM_EXPORT(netconn_prepare_delete),
    ESP_ELFSYM_EXPORT(isdigit),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx117collateIcEE),
    ESP_ELFSYM_EXPORT(__double_computePow5),
    ESP_ELFSYM_EXPORT(phy_get_target_pwr),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_set),
    ESP_ELFSYM_EXPORT(xPortCheckValidTCBMem),
    ESP_ELFSYM_EXPORT(pm_on_tbtt),
    ESP_ELFSYM_EXPORT(gpio_set_intr_type),
    ESP_ELFSYM_EXPORT(ieee80211_ftm_attach),
    ESP_ELFSYM_EXPORT(pm_beacon_offset_add_total_counter),
    ESP_ELFSYM_EXPORT(spi_flash_check_and_flush_cache),
    ESP_ELFSYM_EXPORT(hal_clr_itwt_pti),
    ESP_ELFSYM_EXPORT(lwip_netconn_is_deallocated_msg),
    ESP_ELFSYM_EXPORT(bluetooth_serial_read),
    ESP_ELFSYM_EXPORT(tt_app_fileselection_start_for_existing_file),
    ESP_ELFSYM_EXPORT(esp32_spi_driver),
    ESP_ELFSYM_EXPORT(x_side_default),
    ESP_ELFSYM_EXPORT(ieee80211_crypto_bip_encrypt_with_key),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_terminate),
    ESP_ELFSYM_EXPORT(coex_status_get),
    ESP_ELFSYM_EXPORT(ip4_output_if_src),
    ESP_ELFSYM_EXPORT(r_ble_ll_utils_calc_window_widening),
    ESP_ELFSYM_EXPORT(esp_ble_register_bb_funcs),
    ESP_ELFSYM_EXPORT(esp_efuse_utility_get_number_of_items),
    ESP_ELFSYM_EXPORT(_ZGVNSt7collateIwE2idE),
    ESP_ELFSYM_EXPORT(esp_wifi_scan_get_ap_records),
    ESP_ELFSYM_EXPORT(etharp_request),
    ESP_ELFSYM_EXPORT(lv_draw_label_iterate_characters),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_switch_delay_rx_aod_2us_2m),
    ESP_ELFSYM_EXPORT(ble_hs_id_use_addr),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_peer_rpa_any),
    ESP_ELFSYM_EXPORT(esp_test_tx_process_complete),
    ESP_ELFSYM_EXPORT(pmksa_cache_auth_deinit),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_is_tx_link_switch_event_supported),
    ESP_ELFSYM_EXPORT(os_msys_init),
    ESP_ELFSYM_EXPORT(r_hal_timer_start_at),
    ESP_ELFSYM_EXPORT(hal_mac_set_tb_max_pwr),
    ESP_ELFSYM_EXPORT(spi_bus_lock_bg_req_exist),
    ESP_ELFSYM_EXPORT(lmacProcessTxError),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_irk_deinit),
    ESP_ELFSYM_EXPORT(cnx_remove_all_rc),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_proc_unk_rsp),
    ESP_ELFSYM_EXPORT(phy_pbus_set_rxgain),
    ESP_ELFSYM_EXPORT(_ZSt4swapIN2tt3app6apphub11AppHubEntryEENSt9enable_ifIXsrSt6__and_IJSt6__not_ISt15__is_tuple_likeIT_EESt21is_move_constructibleIS8_ESt18is_move_assignableIS8_EEE5valueEvE4typeERS8_SI_),
    ESP_ELFSYM_EXPORT(ble_gatts_bonding_restored),
    ESP_ELFSYM_EXPORT(wifi_auto_scan_set_paused),
    ESP_ELFSYM_EXPORT(getenv),
    ESP_ELFSYM_EXPORT(auth_sae_queue),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_disconnect_cmd),
    ESP_ELFSYM_EXPORT(pm_coex_restart_timeout_process),
    ESP_ELFSYM_EXPORT(mmgmt_enableRxbufOptFeature),
    ESP_ELFSYM_EXPORT(wpa_sta_connect),
    ESP_ELFSYM_EXPORT(lv_ll_get_tail),
    ESP_ELFSYM_EXPORT(strlcpy),
    ESP_ELFSYM_EXPORT(device_stop),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_ie),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_read_blob),
    ESP_ELFSYM_EXPORT(lv_image_get_src),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_set_local_rpa),
    ESP_ELFSYM_EXPORT(lmacEndFrameExchangeSequence),
    ESP_ELFSYM_EXPORT(_ZTSSt16invalid_argument),
    ESP_ELFSYM_EXPORT(adc_hal_set_calibration_param),
    ESP_ELFSYM_EXPORT(_ZTV11WL_Ext_Safe),
    ESP_ELFSYM_EXPORT(phy_chip_set_chan),
    ESP_ELFSYM_EXPORT(ieee80211_has_ht40_bss),
    ESP_ELFSYM_EXPORT(_ZZNSt8__format15__formatter_intIcE11_M_do_parseERSt26basic_format_parse_contextIcENS_10_Pres_typeEENKUlvE0_clEv),
    ESP_ELFSYM_EXPORT(_ZGVNSt8numpunctIwE2idE),
    ESP_ELFSYM_EXPORT(wdev_bank_load),
    ESP_ELFSYM_EXPORT(ble_store_write_rpa_rec),
    ESP_ELFSYM_EXPORT(nus_tx_handle),
    ESP_ELFSYM_EXPORT(esp_transport_list_init),
    ESP_ELFSYM_EXPORT(lv_obj_init_draw_arc_dsc),
    ESP_ELFSYM_EXPORT(empty_func_p_pconfg_r_void),
    ESP_ELFSYM_EXPORT(esp_backtrace_print),
    ESP_ELFSYM_EXPORT(display_get_gap_x),
    ESP_ELFSYM_EXPORT(i2s_free_dma_desc),
    ESP_ELFSYM_EXPORT(ppRegisterPromisRxCallback),
    ESP_ELFSYM_EXPORT(pm_pend_noise_check),
    ESP_ELFSYM_EXPORT(os_get_time),
    ESP_ELFSYM_EXPORT(ble_sm_alg_ecc_init),
    ESP_ELFSYM_EXPORT(ieee80211_proto_attach),
    ESP_ELFSYM_EXPORT(r_ble_ll_set_public_addr),
    ESP_ELFSYM_EXPORT(ble_hid_switch_profile),
    ESP_ELFSYM_EXPORT(cnx_rc_update_age),
    ESP_ELFSYM_EXPORT(mpi_hal_start_op),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_proc_start),
    ESP_ELFSYM_EXPORT(r_mem_split_frag),
    ESP_ELFSYM_EXPORT(phy_pbus_debugmode),
    ESP_ELFSYM_EXPORT(wDev_GetNoiseFloor),
    ESP_ELFSYM_EXPORT(toupper),
    ESP_ELFSYM_EXPORT(temperature_sensor_hal_get_raw_value),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_list_get_free),
    ESP_ELFSYM_EXPORT(gpio_pulldown_dis),
    ESP_ELFSYM_EXPORT(g_pm_twt),
    ESP_ELFSYM_EXPORT(i2s_hal_tdm_set_rx_slot),
    ESP_ELFSYM_EXPORT(xTaskGetCurrentTaskHandle),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_get_type),
    ESP_ELFSYM_EXPORT(ic_set_rx_policy),
    ESP_ELFSYM_EXPORT(lwip_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_sm_clear),
    ESP_ELFSYM_EXPORT(lv_textarea_class),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_piscan_wifi_scan),
    ESP_ELFSYM_EXPORT(_ZTSSt20bad_array_new_length),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx118messagesIcEE),
    ESP_ELFSYM_EXPORT(esp_elf_request),
    ESP_ELFSYM_EXPORT(phy_pbus_xpd_dpd_path),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_check_cte_type_allowed),
    ESP_ELFSYM_EXPORT(esp_libc_include_init_funcs),
    ESP_ELFSYM_EXPORT(pvTaskGetThreadLocalStoragePointer),
    ESP_ELFSYM_EXPORT(gpio_hal_iomux_out),
    ESP_ELFSYM_EXPORT(sdmmc_select_driver_strength),
    ESP_ELFSYM_EXPORT(esp_sha_hash_setup),
    ESP_ELFSYM_EXPORT(r_ble_ll_calc_session_key),
    ESP_ELFSYM_EXPORT(bt_bb_set_max_gain),
    ESP_ELFSYM_EXPORT(lv_event_send),
    ESP_ELFSYM_EXPORT(_ZSt24__throw_out_of_range_fmtPKcz),
    ESP_ELFSYM_EXPORT(system),
    ESP_ELFSYM_EXPORT(lv_style_builtin_prop_flag_lookup_table),
    ESP_ELFSYM_EXPORT(pau_hal_set_regdma_work_timeout),
    ESP_ELFSYM_EXPORT(wifi_set_rx_policy),
    ESP_ELFSYM_EXPORT(mbedtls_eckey_info),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_filter_disable),
    ESP_ELFSYM_EXPORT(nimble_port_stop),
    ESP_ELFSYM_EXPORT(psa_sign_message_builtin),
    ESP_ELFSYM_EXPORT(atanhf),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_inv_mod),
    ESP_ELFSYM_EXPORT(_ZTVSt10moneypunctIwLb0EE),
    ESP_ELFSYM_EXPORT(esp_libc_init),
    ESP_ELFSYM_EXPORT(ieee80211_register_hostap_timer),
    ESP_ELFSYM_EXPORT(_ZZNSt8__format15__formatter_strIcE5parseERSt26basic_format_parse_contextIcEENKUlvE0_clEv),
    ESP_ELFSYM_EXPORT(gpio_descriptor_remove_callback),
    ESP_ELFSYM_EXPORT(file_mutex_lock),
    ESP_ELFSYM_EXPORT(esp_time_impl_get_boot_time),
    ESP_ELFSYM_EXPORT(esp_event_handler_instance_unregister_with),
    ESP_ELFSYM_EXPORT(phy_get_xtal_duty),
    ESP_ELFSYM_EXPORT(spi_device_polling_start),
    ESP_ELFSYM_EXPORT(ble_store_util_delete_peer),
    ESP_ELFSYM_EXPORT(lv_style_set_pad_top),
    ESP_ELFSYM_EXPORT(pm_go_to_wake),
    ESP_ELFSYM_EXPORT(ieee80211_sta_connect),
    ESP_ELFSYM_EXPORT(esp_libc_include_strncpy_impl),
    ESP_ELFSYM_EXPORT(iswctype),
    ESP_ELFSYM_EXPORT(coex_schm_external_coex_wifi_connecting),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_write_rsp),
    ESP_ELFSYM_EXPORT(esp_netif_update_default_netif),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_find_new_phy),
    ESP_ELFSYM_EXPORT(r_bt_rf_coex_hook_init),
    ESP_ELFSYM_EXPORT(_ZTIN10__cxxabiv115__forced_unwindE),
    ESP_ELFSYM_EXPORT(__wrap___cxa_throw),
    ESP_ELFSYM_EXPORT(crypto_ecdh_get_pubkey),
    ESP_ELFSYM_EXPORT(ble_hs_conn_first),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_periodic_estimate_data_itvl),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_init_support_cmd_base_on_lmp_ver),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt5ctypeIcEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(ledc_periph_signal),
    ESP_ELFSYM_EXPORT(lv_draw_sw_transform),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_env_init),
    ESP_ELFSYM_EXPORT(tcp_close),
    ESP_ELFSYM_EXPORT(hal_he_set_default_pe),
    ESP_ELFSYM_EXPORT(r_os_msys_reset),
    ESP_ELFSYM_EXPORT(spi_hal_enable_data_line),
    ESP_ELFSYM_EXPORT(r_ble_ll_is_controller_busy),
    ESP_ELFSYM_EXPORT(lv_draw_layer_alloc_buf),
    ESP_ELFSYM_EXPORT(soc_get_available_memory_region_max_count),
    ESP_ELFSYM_EXPORT(gpio_iomux_input),
    ESP_ELFSYM_EXPORT(hal_crypto_set_key_entry),
    ESP_ELFSYM_EXPORT(ble_l2cap_ctx),
    ESP_ELFSYM_EXPORT(floor),
    ESP_ELFSYM_EXPORT(os_mempool_unregister),
    ESP_ELFSYM_EXPORT(_lock_acquire_recursive),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_connless_cte_tx_enable),
    ESP_ELFSYM_EXPORT(EccPoint_isZero),
    ESP_ELFSYM_EXPORT(phy_i2c_enter_critical),
    ESP_ELFSYM_EXPORT(bundle_get_string),
    ESP_ELFSYM_EXPORT(_Z19ble_set_scan_activeP6Deviceb),
    ESP_ELFSYM_EXPORT(winWiden_stack_enableSetConstPeerScaVsCmd),
    ESP_ELFSYM_EXPORT(coex_core_event_duration_get),
    ESP_ELFSYM_EXPORT(app_manager_remove),
    ESP_ELFSYM_EXPORT(display_set_gap),
    ESP_ELFSYM_EXPORT(wlanif_init_ap),
    ESP_ELFSYM_EXPORT(hal_timer_env_p),
    ESP_ELFSYM_EXPORT(sync_stack_getBroker),
    ESP_ELFSYM_EXPORT(ppReSendBar),
    ESP_ELFSYM_EXPORT(esp_register_freertos_tick_hook_for_cpu),
    ESP_ELFSYM_EXPORT(ic_register_promis_rx_cb),
    ESP_ELFSYM_EXPORT(r_ble_phy_txpower_round),
    ESP_ELFSYM_EXPORT(_ZTVNSt8__format9_Seq_sinkINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEE),
    ESP_ELFSYM_EXPORT(_ZdaPv),
    ESP_ELFSYM_EXPORT(esp_event_symbols),
    ESP_ELFSYM_EXPORT(lv_obj_init_draw_line_dsc),
    ESP_ELFSYM_EXPORT(npl_freertos_mutex_pend),
    ESP_ELFSYM_EXPORT(lora_remove_rx_callback),
    ESP_ELFSYM_EXPORT(httpd_req_get_hdr_value_len),
    ESP_ELFSYM_EXPORT(_Z31getQrVersionForBinaryDataLengthjRi),
    ESP_ELFSYM_EXPORT(ic_clear_data_ifx_tx_queue),
    ESP_ELFSYM_EXPORT(hal_mac_disable_low_rate),
    ESP_ELFSYM_EXPORT(spi_flash_hal_erase_chip),
    ESP_ELFSYM_EXPORT(tsf_hal_get_tbtt_interval),
    ESP_ELFSYM_EXPORT(ble_hs_hci_set_hci_version),
    ESP_ELFSYM_EXPORT(key_mgr_hal_set_key_usage),
    ESP_ELFSYM_EXPORT(ppProcessLifeTime),
    ESP_ELFSYM_EXPORT(trc_get_80211_tx_rate),
    ESP_ELFSYM_EXPORT(esp_transport_translate_error),
    ESP_ELFSYM_EXPORT(pm_is_dream),
    ESP_ELFSYM_EXPORT(ieee80211_vht_updateparams),
    ESP_ELFSYM_EXPORT(esp_crypto_key_mgr_enable_periph_clk),
    ESP_ELFSYM_EXPORT(xRingbufferCreateStatic),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_conn_upd_make),
    ESP_ELFSYM_EXPORT(lv_slider_set_value),
    ESP_ELFSYM_EXPORT(esp_crypto_ecdsa_lock_acquire),
    ESP_ELFSYM_EXPORT(esp_sha1_update),
    ESP_ELFSYM_EXPORT(TxRxCxt),
    ESP_ELFSYM_EXPORT(softap_choose_bss_color_without_conflict),
    ESP_ELFSYM_EXPORT(spi_bus_get_max_transaction_len),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service11displayidle18DisplayIdleServiceE),
    ESP_ELFSYM_EXPORT(esp_psram_mspi_register_isr),
    ESP_ELFSYM_EXPORT(hostap_auth_open),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_setkey),
    ESP_ELFSYM_EXPORT(lv_snprintf),
    ESP_ELFSYM_EXPORT(sdmmc_init_mmc_check_ext_csd),
    ESP_ELFSYM_EXPORT(mbedtls_pk_wrap_psa),
    ESP_ELFSYM_EXPORT(sdmmc_decode_cid),
    ESP_ELFSYM_EXPORT(Cache_WriteBack_Addr),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_append_tx_buffer),
    ESP_ELFSYM_EXPORT(_ZTVSt11__timepunctIwE),
    ESP_ELFSYM_EXPORT(phy_get_rx_pbus_freq),
    ESP_ELFSYM_EXPORT(ble_midi_set_conn_handle),
    ESP_ELFSYM_EXPORT(pmu_hp_system_retention_param_default),
    ESP_ELFSYM_EXPORT(lv_group_focus_prev),
    ESP_ELFSYM_EXPORT(esp_flash_noos_functions),
    ESP_ELFSYM_EXPORT(hmac_md5),
    ESP_ELFSYM_EXPORT(esp_sha_hash_update),
    ESP_ELFSYM_EXPORT(esp_crypto_ds_lock_release),
    ESP_ELFSYM_EXPORT(spi_flash_encryption_hal_disable),
    ESP_ELFSYM_EXPORT(mpi_hal_write_to_mem_block),
    ESP_ELFSYM_EXPORT(phy_stop_tx_tone),
    ESP_ELFSYM_EXPORT(esp_vfs_include_nullfs_register),
    ESP_ELFSYM_EXPORT(r_ble_lll_calc_us_convert_tick_unit),
    ESP_ELFSYM_EXPORT(LORA_TYPE),
    ESP_ELFSYM_EXPORT(xIsrStackBottom),
    ESP_ELFSYM_EXPORT(g_pp_timer_info),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_prof_is_wapi_internal),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_tx),
    ESP_ELFSYM_EXPORT(phy_bias_reg_set),
    ESP_ELFSYM_EXPORT(bluetooth_serial_get_device),
    ESP_ELFSYM_EXPORT(uart_write_bytes),
    ESP_ELFSYM_EXPORT(wctype),
    ESP_ELFSYM_EXPORT(__retarget_lock_release_recursive),
    ESP_ELFSYM_EXPORT(nan_dp_post_tx),
    ESP_ELFSYM_EXPORT(__wrap__Unwind_Resume_or_Rethrow),
    ESP_ELFSYM_EXPORT(ppSubFromAMPDU),
    ESP_ELFSYM_EXPORT(wcsftime),
    ESP_ELFSYM_EXPORT(multi_heap_register),
    ESP_ELFSYM_EXPORT(pm_get_keep_alive_time),
    ESP_ELFSYM_EXPORT(ble_hs_misc_own_addr_type_to_id),
    ESP_ELFSYM_EXPORT(esp_vfs_closedir),
    ESP_ELFSYM_EXPORT(chm_get_snd_chan_mode),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_free),
    ESP_ELFSYM_EXPORT(esp_tls_conn_new_sync),
    ESP_ELFSYM_EXPORT(ble_hs_misc_conn_chan_find),
    ESP_ELFSYM_EXPORT(lv_obj_style_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_sm_npl_init),
    ESP_ELFSYM_EXPORT(wpa_auth_sta_associated),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ecp_generate_key),
    ESP_ELFSYM_EXPORT(ble_l2cap_parse_hdr),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_common_init),
    ESP_ELFSYM_EXPORT(esp_crypto_ecc_lock_acquire),
    ESP_ELFSYM_EXPORT(ic_get_80211_tx_rate_config),
    ESP_ELFSYM_EXPORT(tcp_split_unsent_seg),
    ESP_ELFSYM_EXPORT(uart_get_buffered_data_len),
    ESP_ELFSYM_EXPORT(wl_mount),
    ESP_ELFSYM_EXPORT(gpio_set_pull_mode),
    ESP_ELFSYM_EXPORT(lvgl_pointer_add),
    ESP_ELFSYM_EXPORT(lv_display_get_matrix_rotation),
    ESP_ELFSYM_EXPORT(os_timer_disarm),
    ESP_ELFSYM_EXPORT(npl_freertos_funcs_init),
    ESP_ELFSYM_EXPORT(putwc_unlocked),
    ESP_ELFSYM_EXPORT(esp_hmac_compute_opaque),
    ESP_ELFSYM_EXPORT(http_utils_append_string),
    ESP_ELFSYM_EXPORT(lv_style_set_text_color),
    ESP_ELFSYM_EXPORT(uECC_vli_equal),
    ESP_ELFSYM_EXPORT(gdma_hal_connect_peri),
    ESP_ELFSYM_EXPORT(pp_timer_process),
    ESP_ELFSYM_EXPORT(xQueueGiveMutexRecursive),
    ESP_ELFSYM_EXPORT(mbedtls_aes_setkey_enc),
    ESP_ELFSYM_EXPORT(acd_add),
    ESP_ELFSYM_EXPORT(esp_flash_freq_unlimit_cb),
    ESP_ELFSYM_EXPORT(__retarget_lock_acquire_recursive),
    ESP_ELFSYM_EXPORT(r_esp_ble_msys_init),
    ESP_ELFSYM_EXPORT(he_twt_information_resume),
    ESP_ELFSYM_EXPORT(lv_obj_set_x),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_postion_pick),
    ESP_ELFSYM_EXPORT(g_wifi_improve_contention_ability),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_end),
    ESP_ELFSYM_EXPORT(ext_funcs_ro),
    ESP_ELFSYM_EXPORT(scan_add_bssid),
    ESP_ELFSYM_EXPORT(r_os_mbuf_ctrl_mbuf_cnt_increase),
    ESP_ELFSYM_EXPORT(__floatundidf),
    ESP_ELFSYM_EXPORT(phy_bbpll_recal),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_left),
    ESP_ELFSYM_EXPORT(crypto_ec_point_deinit),
    ESP_ELFSYM_EXPORT(bluetooth_midi_get_device),
    ESP_ELFSYM_EXPORT(lv_draw_sw_triangle),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_BLOCK_KEY3),
    ESP_ELFSYM_EXPORT(lv_color32_luminance),
    ESP_ELFSYM_EXPORT(gps_model_to_string),
    ESP_ELFSYM_EXPORT(rtc_gpio_is_valid_gpio),
    ESP_ELFSYM_EXPORT(lmacProcessRxSucData),
    ESP_ELFSYM_EXPORT(driver_unbind),
    ESP_ELFSYM_EXPORT(ble_spp_get_active),
    ESP_ELFSYM_EXPORT(netconn_connect),
    ESP_ELFSYM_EXPORT(mld6_lookfor_group),
    ESP_ELFSYM_EXPORT(_Z10app_ledgerv),
    ESP_ELFSYM_EXPORT(ieee80211_is_ht_cipher),
    ESP_ELFSYM_EXPORT(hal_mac_clr_txq_state),
    ESP_ELFSYM_EXPORT(nvs_set_i32),
    ESP_ELFSYM_EXPORT(ble_att_deinit),
    ESP_ELFSYM_EXPORT(hal_mac_tsf_set_time),
    ESP_ELFSYM_EXPORT(app_event_subscribe),
    ESP_ELFSYM_EXPORT(esp_event_handler_unregister_with),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_set_relative_addr),
    ESP_ELFSYM_EXPORT(g_hmac_cnt),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_hash_from_md_alg),
    ESP_ELFSYM_EXPORT(psa_is_key_present_in_storage),
    ESP_ELFSYM_EXPORT(wpa_parse_wpa_ie_rsnxe),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_get_next_adv_prim_chan),
    ESP_ELFSYM_EXPORT(phy_ant_dft_cfg),
    ESP_ELFSYM_EXPORT(_ZGVNSt7num_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(r_ble_phy_env_init),
    ESP_ELFSYM_EXPORT(ble_hs_get_rpa_timeout),
    ESP_ELFSYM_EXPORT(lv_roller_label_class),
    ESP_ELFSYM_EXPORT(esp_efuse_rtc_calib_get_init_code),
    ESP_ELFSYM_EXPORT(lvgl_trackball_set_cursor_image),
    ESP_ELFSYM_EXPORT(app_event_subscribe_with_app_id),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_read_group_type),
    ESP_ELFSYM_EXPORT(wifi_get_macaddr),
    ESP_ELFSYM_EXPORT(hostap_recv_ctl),
    ESP_ELFSYM_EXPORT(gpio_set_direction),
    ESP_ELFSYM_EXPORT(coex_schm_bt_a2dp_paused_wifi_scan),
    ESP_ELFSYM_EXPORT(vTaskStartScheduler),
    ESP_ELFSYM_EXPORT(ieee80211_add_supportedchannels),
    ESP_ELFSYM_EXPORT(ieee80211_is_vht_cipher),
    ESP_ELFSYM_EXPORT(ptr_beacon_offset_funcs),
    ESP_ELFSYM_EXPORT(tcp_tmr),
    ESP_ELFSYM_EXPORT(lv_draw_buf_from_image),
    ESP_ELFSYM_EXPORT(spi_flash_chip_winbond_probe),
    ESP_ELFSYM_EXPORT(pm_on_isr_set_twt_target),
    ESP_ELFSYM_EXPORT(r_ble_ll_pdu_max_tx_octets_get),
    ESP_ELFSYM_EXPORT(__umul128),
    ESP_ELFSYM_EXPORT(_Z21app_metadata_parse_v1RKSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_St4lessIS5_ESaISt4pairIKS5_S5_EEER11AppMetadata),
    ESP_ELFSYM_EXPORT(ic_get_next_tbtt),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_free_link_state),
    ESP_ELFSYM_EXPORT(ic_enable_sniffer),
    ESP_ELFSYM_EXPORT(esp_netif_get_handle_from_ifkey_unsafe),
    ESP_ELFSYM_EXPORT(lv_obj_remove_state),
    ESP_ELFSYM_EXPORT(httpd_sess_pending),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_env_init),
    ESP_ELFSYM_EXPORT(KEYBOARD_TYPE),
    ESP_ELFSYM_EXPORT(bt_bb_cca_en),
    ESP_ELFSYM_EXPORT(ble_sm_sc_io_action),
    ESP_ELFSYM_EXPORT(hal_mac_get_txq_state),
    ESP_ELFSYM_EXPORT(xQueueSemaphoreTake),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_set_strategy),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_opaque_asymmetric_decrypt),
    ESP_ELFSYM_EXPORT(lv_cache_entry_get_entry),
    ESP_ELFSYM_EXPORT(rtc_clk_cal),
    ESP_ELFSYM_EXPORT(phy_xtal_duty_set),
    ESP_ELFSYM_EXPORT(i2c_controller_has_device_at_address),
    ESP_ELFSYM_EXPORT(esp_nimble_enable),
    ESP_ELFSYM_EXPORT(gpio_od_enable),
    ESP_ELFSYM_EXPORT(lv_style_set_clip_corner),
    ESP_ELFSYM_EXPORT(spi_flash_hal_gpspi_supports_direct_write),
    ESP_ELFSYM_EXPORT(sae_clear_data),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_cancel_conn_complete_event),
    ESP_ELFSYM_EXPORT(stpcpy),
    ESP_ELFSYM_EXPORT(ieee80211_add_extcap),
    ESP_ELFSYM_EXPORT(hostapd_get_hapd_data),
    ESP_ELFSYM_EXPORT(esp_test_clr_rx_statistics),
    ESP_ELFSYM_EXPORT(spi_flash_hal_gpspi_poll_cmd_done),
    ESP_ELFSYM_EXPORT(pau_hal_regdma_link_count_config),
    ESP_ELFSYM_EXPORT(sta_auth_open),
    ESP_ELFSYM_EXPORT(pm_on_coex_schm_status_config),
    ESP_ELFSYM_EXPORT(phy_i2c_master_mem_txcap),
    ESP_ELFSYM_EXPORT(r_ble_vendor_hci_duplicate_set_param),
    ESP_ELFSYM_EXPORT(mgd_probe_send_timeout_process),
    ESP_ELFSYM_EXPORT(phy_track_pll),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_transfer_sched),
    ESP_ELFSYM_EXPORT(lwip_init_tcp_isn),
    ESP_ELFSYM_EXPORT(wDev_IndicateBeaconMemoryFrame),
    ESP_ELFSYM_EXPORT(lv_indev_set_display),
    ESP_ELFSYM_EXPORT(s_tmp_itwt_id),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_piscan_wifi_connecting),
    ESP_ELFSYM_EXPORT(lv_anim_delete_all),
    ESP_ELFSYM_EXPORT(pbuf_alloc_reference),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_wifi_connecting),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_tx),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_add),
    ESP_ELFSYM_EXPORT(vEventGroupDelete),
    ESP_ELFSYM_EXPORT(rtc_clk_cpu_freq_mhz_to_config),
    ESP_ELFSYM_EXPORT(esp_clk_xtal_freq),
    ESP_ELFSYM_EXPORT(lv_draw_buf_copy),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_ext_set_param_v2),
    ESP_ELFSYM_EXPORT(hal_he_set_ack_rate),
    ESP_ELFSYM_EXPORT(_ZTVSt9money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE),
    ESP_ELFSYM_EXPORT(esp_psram_extram_reserve_dma_pool),
    ESP_ELFSYM_EXPORT(lv_display_get_default),
    ESP_ELFSYM_EXPORT(esp_isr_names),
    ESP_ELFSYM_EXPORT(memp_TCP_PCB_LISTEN),
    ESP_ELFSYM_EXPORT(ic_disable_sniffer),
    ESP_ELFSYM_EXPORT(dhcp_fine_timeout_cb),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_get_device),
    ESP_ELFSYM_EXPORT(FreeRTOS_openocd_params),
    ESP_ELFSYM_EXPORT(usb_host_hid_subscribe),
    ESP_ELFSYM_EXPORT(__mulPow5divPow2),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceIN2tt7service15ServiceManifestESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(phy_txtone_linear_pwr),
    ESP_ELFSYM_EXPORT(gChmCxt),
    ESP_ELFSYM_EXPORT(ieee80211_he_attach),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_send_ext_adv_report),
    ESP_ELFSYM_EXPORT(uart_hal_read_rxfifo),
    ESP_ELFSYM_EXPORT(phy_chip_i2c_readReg),
    ESP_ELFSYM_EXPORT(i2c_hal_master_init),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_state_wakeup_protect_enable),
    ESP_ELFSYM_EXPORT(spi_bus_lock_bg_clear_req),
    ESP_ELFSYM_EXPORT(phy_module_enable),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_set_button_ctrl_all),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_aux_data_ref),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_recycle_buffer),
    ESP_ELFSYM_EXPORT(lv_obj_mark_layout_as_dirty),
    ESP_ELFSYM_EXPORT(ic_reset_rx_ba),
    ESP_ELFSYM_EXPORT(lv_draw_unit_draw_letter),
    ESP_ELFSYM_EXPORT(lmacSetMuEDCAParam),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_duration_period_timers_restart),
    ESP_ELFSYM_EXPORT(ic_set_vif),
    ESP_ELFSYM_EXPORT(esp_flash_chip_boya),
    ESP_ELFSYM_EXPORT(lv_display_rotate_area),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_new),
    ESP_ELFSYM_EXPORT(lv_keyboard_get_textarea),
    ESP_ELFSYM_EXPORT(esp_transport_list_get_transport),
    ESP_ELFSYM_EXPORT(esp_netif_receive),
    ESP_ELFSYM_EXPORT(i2s_get_buf_size),
    ESP_ELFSYM_EXPORT(lv_event_get_user_data),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_periodic_set_data),
    ESP_ELFSYM_EXPORT(bundle_put_string),
    ESP_ELFSYM_EXPORT(esp_fp_print_backtrace),
    ESP_ELFSYM_EXPORT(sta_try_sa_query_process),
    ESP_ELFSYM_EXPORT(uECC_vli_mmod),
    ESP_ELFSYM_EXPORT(lv_indev_set_driver_data),
    ESP_ELFSYM_EXPORT(hal_crypto_clr_key_entry),
    ESP_ELFSYM_EXPORT(xPortGetTickRateHz),
    ESP_ELFSYM_EXPORT(_ZGVZ10app_ledgervE6ledger),
    ESP_ELFSYM_EXPORT(phy_set_chan_reg),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_finish),
    ESP_ELFSYM_EXPORT(wifi_configure_wake_window_process),
    ESP_ELFSYM_EXPORT(netbuf_free),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_rx_buffer_hdr),
    ESP_ELFSYM_EXPORT(tcpip_inpkt),
    ESP_ELFSYM_EXPORT(audio_codec_get_capabilities),
    ESP_ELFSYM_EXPORT(wpa_status_to_reason_code),
    ESP_ELFSYM_EXPORT(esp_timer_impl_init_system_time),
    ESP_ELFSYM_EXPORT(ble_ll_sync_env_p),
    ESP_ELFSYM_EXPORT(ic_register_rx_cb),
    ESP_ELFSYM_EXPORT(esp_lcd_new_panel_io_spi),
    ESP_ELFSYM_EXPORT(mt_add_peer),
    ESP_ELFSYM_EXPORT(wifi_api_lock),
    ESP_ELFSYM_EXPORT(npl_freertos_event_deinit),
    ESP_ELFSYM_EXPORT(spi_flash_hal_program_page),
    ESP_ELFSYM_EXPORT(bluetooth_is_scanning),
    ESP_ELFSYM_EXPORT(wdev_csi_hw_bug_check),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_client_hello),
    ESP_ELFSYM_EXPORT(ble_hs_stop_init),
    ESP_ELFSYM_EXPORT(esp_test_get_tx_statistics),
    ESP_ELFSYM_EXPORT(phy_i2c_writeReg),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_send_noop),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_deinit),
    ESP_ELFSYM_EXPORT(lv_color_brightness),
    ESP_ELFSYM_EXPORT(div),
    ESP_ELFSYM_EXPORT(esp_wifi_send_deauth_local),
    ESP_ELFSYM_EXPORT(lv_cache_entry_get_size),
    ESP_ELFSYM_EXPORT(ff_diskio_register_sdmmc),
    ESP_ELFSYM_EXPORT(lv_spangroup_set_mode),
    ESP_ELFSYM_EXPORT(lv_bar_set_start_value),
    ESP_ELFSYM_EXPORT(lv_obj_clean),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_reject_tx),
    ESP_ELFSYM_EXPORT(phy_get_pwdet_offset_new),
    ESP_ELFSYM_EXPORT(wifi_module_enable),
    ESP_ELFSYM_EXPORT(sae_write_confirm),
    ESP_ELFSYM_EXPORT(lv_obj_remove_local_style_prop),
    ESP_ELFSYM_EXPORT(ic_reset_tbtt),
    ESP_ELFSYM_EXPORT(lv_menu_cont_class),
    ESP_ELFSYM_EXPORT(uart_vfs_dev_register),
    ESP_ELFSYM_EXPORT(esp_intr_noniram_disable),
    ESP_ELFSYM_EXPORT(spi_bus_lock_bg_request),
    ESP_ELFSYM_EXPORT(bt_osi_mem_malloc_internal),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_rm_timer),
    ESP_ELFSYM_EXPORT(esp_wifi_bt_power_domain_on),
    ESP_ELFSYM_EXPORT(phy_get_romfunc_addr),
    ESP_ELFSYM_EXPORT(ble_sm_sc_deinit),
    ESP_ELFSYM_EXPORT(xEventGroupGetStaticBuffer),
    ESP_ELFSYM_EXPORT(udp_new_ip_type),
    ESP_ELFSYM_EXPORT(esp32_pwm_ledc_driver),
    ESP_ELFSYM_EXPORT(ff_diskio_is_registered),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_read_blob_rsp),
    ESP_ELFSYM_EXPORT(__kernel_cos),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_set_our_irk),
    ESP_ELFSYM_EXPORT(conn_stack_enable),
    ESP_ELFSYM_EXPORT(hal_enable_sta_tbtt),
    ESP_ELFSYM_EXPORT(wifi_nvs_get),
    ESP_ELFSYM_EXPORT(display_get_resolution_x),
    ESP_ELFSYM_EXPORT(phy_abs_temp),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_verify_restartable),
    ESP_ELFSYM_EXPORT(mumimo_spatial_cfg_get_nsts),
    ESP_ELFSYM_EXPORT(wdev_mac_special_reg_load),
    ESP_ELFSYM_EXPORT(esp_libc_include_heap_impl),
    ESP_ELFSYM_EXPORT(i2c_master_write_byte),
    ESP_ELFSYM_EXPORT(lv_arc_set_mode),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_sta_password),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_backup_info_from_sm),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_update_in_scan),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_init),
    ESP_ELFSYM_EXPORT(__lock___at_quick_exit_mutex),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_check_privkey),
    ESP_ELFSYM_EXPORT(ble_lll_module_env_p),
    ESP_ELFSYM_EXPORT(is_ndpa_to_dut),
    ESP_ELFSYM_EXPORT(scan_profile_check),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_vendor_register),
    ESP_ELFSYM_EXPORT(vQueueDelete),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_get_min_dura_required),
    ESP_ELFSYM_EXPORT(phy_xtal_reg_set),
    ESP_ELFSYM_EXPORT(pwm_get_period),
    ESP_ELFSYM_EXPORT(spi_flash_chip_gd_suspend_cmd_conf),
    ESP_ELFSYM_EXPORT(r_bt_rf_coex_cfg_set),
    ESP_ELFSYM_EXPORT(mbedtls_pk_sign_restartable),
    ESP_ELFSYM_EXPORT(_ZTISt12out_of_range),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_insertion_end),
    ESP_ELFSYM_EXPORT(he_send_action_twt_information),
    ESP_ELFSYM_EXPORT(phy_rxiq_scale_set),
    ESP_ELFSYM_EXPORT(gpio_pulldown_en),
    ESP_ELFSYM_EXPORT(adiList_stack_listDump),
    ESP_ELFSYM_EXPORT(httpd_is_sess_available),
    ESP_ELFSYM_EXPORT(esp_libc_include_pthread_impl),
    ESP_ELFSYM_EXPORT(apm_hal_set_master_sec_mode_all),
    ESP_ELFSYM_EXPORT(spi_bus_lock_bg_check_dev_acq),
    ESP_ELFSYM_EXPORT(xPortInIsrContext),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_env_deinit),
    ESP_ELFSYM_EXPORT(__bufio_flush),
    ESP_ELFSYM_EXPORT(ble_store_config_persist_cccds),
    ESP_ELFSYM_EXPORT(esp_coex_common_timer_done_wrapper),
    ESP_ELFSYM_EXPORT(wcsnlen),
    ESP_ELFSYM_EXPORT(s_update_cache_state),
    ESP_ELFSYM_EXPORT(sdmmc_read_sectors_dma),
    ESP_ELFSYM_EXPORT(set_assoc_ie),
    ESP_ELFSYM_EXPORT(pm_extend_tbtt_adaptive_deattach),
    ESP_ELFSYM_EXPORT(wpa_sm_set_ap_rsnxe),
    ESP_ELFSYM_EXPORT(wpa_eapol_key_send),
    ESP_ELFSYM_EXPORT(psa_its_get_info),
    ESP_ELFSYM_EXPORT(lv_bar_create),
    ESP_ELFSYM_EXPORT(phy_get_vco_init_new),
    ESP_ELFSYM_EXPORT(_ZTISt9bad_alloc),
    ESP_ELFSYM_EXPORT(ble_transport_to_ll_acl_impl),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_set_priority),
    ESP_ELFSYM_EXPORT(lv_list_get_button_text),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_prof_pairwise_cipher_internal),
    ESP_ELFSYM_EXPORT(ieee80211_btwt_setup_txcb),
    ESP_ELFSYM_EXPORT(esp_log_timestamp),
    ESP_ELFSYM_EXPORT(xStreamBufferGenericCreateStatic),
    ESP_ELFSYM_EXPORT(__fixsfdi),
    ESP_ELFSYM_EXPORT(coex_schm_bt_default_wifi_connecting),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSoD0Ev),
    ESP_ELFSYM_EXPORT(gdma_default_tx_isr),
    ESP_ELFSYM_EXPORT(thread_free),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_indicate),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_image_to_al88),
    ESP_ELFSYM_EXPORT(phy_rxiq_set_reg),
    ESP_ELFSYM_EXPORT(ble_gap_rx_data_len_change),
    ESP_ELFSYM_EXPORT(esp_hmac_update_transparent),
    ESP_ELFSYM_EXPORT(lv_bar_get_max_value),
    ESP_ELFSYM_EXPORT(lwip_cyclic_timers),
    ESP_ELFSYM_EXPORT(sinf),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_rxbuffer_allocate),
    ESP_ELFSYM_EXPORT(rcTxUpdatePer),
    ESP_ELFSYM_EXPORT(get_local_fd),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_free_named_data_list_shallow),
    ESP_ELFSYM_EXPORT(lv_textarea_set_cursor_click_pos),
    ESP_ELFSYM_EXPORT(bundle_put_int32),
    ESP_ELFSYM_EXPORT(vTimerSetTimerID),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_montmul),
    ESP_ELFSYM_EXPORT(ieee80211_amsdu_negotiate),
    ESP_ELFSYM_EXPORT(esp_wifi_vnd_lora_enable),
    ESP_ELFSYM_EXPORT(r_ble_hw_rng_start),
    ESP_ELFSYM_EXPORT(__wrap_gpio_config),
    ESP_ELFSYM_EXPORT(RGB_LED_TYPE),
    ESP_ELFSYM_EXPORT(ds_hal_busy),
    ESP_ELFSYM_EXPORT(ieee80211_output),
    ESP_ELFSYM_EXPORT(lv_cache_destroy),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_get_active_conn),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_sub_mpi),
    ESP_ELFSYM_EXPORT(ble_ll_conn_subrate_req_llcp),
    ESP_ELFSYM_EXPORT(esp_wifi_disconnect),
    ESP_ELFSYM_EXPORT(backlight_get_max_brightness),
    ESP_ELFSYM_EXPORT(lodepng_color_mode_copy),
    ESP_ELFSYM_EXPORT(r_ble_phy_set_dev_address),
    ESP_ELFSYM_EXPORT(netif_find),
    ESP_ELFSYM_EXPORT(lv_span_stack_init),
    ESP_ELFSYM_EXPORT(phy_txiq_set_reg),
    ESP_ELFSYM_EXPORT(phy_get_i2c_read_mask_),
    ESP_ELFSYM_EXPORT(g_espnow_user_oui),
    ESP_ELFSYM_EXPORT(lv_text_encoded_prev),
    ESP_ELFSYM_EXPORT(pm_enable_sta_disconnected_power_management),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY_PURPOSE_1),
    ESP_ELFSYM_EXPORT(hal_he_get_bss_color),
    ESP_ELFSYM_EXPORT(esp_http_client_reset_redirect_counter),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_ext_set_scan_rsp),
    ESP_ELFSYM_EXPORT(esp_log_early_timestamp),
    ESP_ELFSYM_EXPORT(scan_get_scan_id),
    ESP_ELFSYM_EXPORT(coex_schm_external_coex_wifi_default),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_reset),
    ESP_ELFSYM_EXPORT(_ZZNSt13basic_filebufIcSt11char_traitsIcEE5closeEvEN14__close_sentryD2Ev),
    ESP_ELFSYM_EXPORT(esp_libc_include_syscalls_impl),
    ESP_ELFSYM_EXPORT(empty_func_p_u8_r_void),
    ESP_ELFSYM_EXPORT(esp_test_baparas_support_amsdu),
    ESP_ELFSYM_EXPORT(pp_create_task),
    ESP_ELFSYM_EXPORT(sys_thread_sem_init),
    ESP_ELFSYM_EXPORT(ble_hs_hci_ctx_free),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED0Ev),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_opa),
    ESP_ELFSYM_EXPORT(_ZTVN3nvs19NVSPartitionManagerE),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_crypt_and_tag),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_reject_invalid_cid_tx),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_set_csa),
    ESP_ELFSYM_EXPORT(ieee80211_send_action),
    ESP_ELFSYM_EXPORT(lv_indev_get_point),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_rd_local_supp_cmd),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handshake_wrapup),
    ESP_ELFSYM_EXPORT(file_mutex_get),
    ESP_ELFSYM_EXPORT(esp_sleep_gpio_include),
    ESP_ELFSYM_EXPORT(hal_he_set_ac_muedca_param),
    ESP_ELFSYM_EXPORT(start_select),
    ESP_ELFSYM_EXPORT(i2s_hal_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_active_chanset_set_pri),
    ESP_ELFSYM_EXPORT(cnx_get_authtype_strength),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_deletion_begin),
    ESP_ELFSYM_EXPORT(lv_draw_buf_create),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_global_block_free),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_color_to_i1),
    ESP_ELFSYM_EXPORT(minmea_scan),
    ESP_ELFSYM_EXPORT(_Z29tusbStartMassStorageWithFlashb),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_gcd_modinv_odd),
    ESP_ELFSYM_EXPORT(raw_sendto),
    ESP_ELFSYM_EXPORT(qrcode_getBufferSize),
    ESP_ELFSYM_EXPORT(__fixunssfsi),
    ESP_ELFSYM_EXPORT(lmacProcessShortRetryFail),
    ESP_ELFSYM_EXPORT(phy_i2c_sdm_init),
    ESP_ELFSYM_EXPORT(lv_obj_refr_pos),
    ESP_ELFSYM_EXPORT(i2s_gpio_loopback_set),
    ESP_ELFSYM_EXPORT(_ZGVNSt11__timepunctIwE2idE),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_memblock_get_cb),
    ESP_ELFSYM_EXPORT(lv_spangroup_get_mode),
    ESP_ELFSYM_EXPORT(mbedtls_pk_sign),
    ESP_ELFSYM_EXPORT(r_os_mbuf_len),
    ESP_ELFSYM_EXPORT(pm_extend_tbtt_adaptive_stop),
    ESP_ELFSYM_EXPORT(esf_buf_alloc),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_parse_auxptr),
    ESP_ELFSYM_EXPORT(r_ble_lll_recycle_overlapped_sch),
    ESP_ELFSYM_EXPORT(lv_color_to_u32),
    ESP_ELFSYM_EXPORT(spicommon_bus_alloc),
    ESP_ELFSYM_EXPORT(rcClearCurStat),
    ESP_ELFSYM_EXPORT(esp_http_client_get_chunk_length),
    ESP_ELFSYM_EXPORT(_daylight),
    ESP_ELFSYM_EXPORT(vTaskPrioritySet),
    ESP_ELFSYM_EXPORT(minitar_handle_panic),
    ESP_ELFSYM_EXPORT(systimer_ticks_to_us),
    ESP_ELFSYM_EXPORT(esp_task_wdt_impl_timer_restart),
    ESP_ELFSYM_EXPORT(psa_hash_compute),
    ESP_ELFSYM_EXPORT(wifi_set_mode_process),
    ESP_ELFSYM_EXPORT(iswspace),
    ESP_ELFSYM_EXPORT(_Z23ble_hid_get_host_activeP6Device),
    ESP_ELFSYM_EXPORT(RC_SetBasicRate),
    ESP_ELFSYM_EXPORT(_mtvt_table),
    ESP_ELFSYM_EXPORT(pm_funcs_init),
    ESP_ELFSYM_EXPORT(esp_wifi_set_channel),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_idle_wifi_scan),
    ESP_ELFSYM_EXPORT(esp_crypto_dpa_protection_startup),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_connecting),
    ESP_ELFSYM_EXPORT(r_DB_addr_list_remove),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tsf_time_deviation_sync_enable),
    ESP_ELFSYM_EXPORT(phy_ant_bttx_cfg),
    ESP_ELFSYM_EXPORT(mbedtls_base64_encode),
    ESP_ELFSYM_EXPORT(lodepng_load_file),
    ESP_ELFSYM_EXPORT(pm_mac_disable_tsf_tbtt_soc_wakeup),
    ESP_ELFSYM_EXPORT(__floatsidf),
    ESP_ELFSYM_EXPORT(esp_wifi_power_domain_on),
    ESP_ELFSYM_EXPORT(pthread_mutex_init),
    ESP_ELFSYM_EXPORT(pvPortMalloc),
    ESP_ELFSYM_EXPORT(r_ble_lll_process_dequeued_sch),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_shrink),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_grow),
    ESP_ELFSYM_EXPORT(lv_indev_wait_release),
    ESP_ELFSYM_EXPORT(lv_draw_layer),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_a2dp_wifi_conn),
    ESP_ELFSYM_EXPORT(rtc_isr_noniram_enable),
    ESP_ELFSYM_EXPORT(crypto_ec_point_is_at_infinity),
    ESP_ELFSYM_EXPORT(sleep_clock_system_retention_init),
    ESP_ELFSYM_EXPORT(sha_hal_hash_block),
    ESP_ELFSYM_EXPORT(lv_indev_get_group),
    ESP_ELFSYM_EXPORT(soc_memory_types),
    ESP_ELFSYM_EXPORT(lmacProcessTxopQComplete),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_start),
    ESP_ELFSYM_EXPORT(hal_disable_tsf_timer),
    ESP_ELFSYM_EXPORT(iswblank),
    ESP_ELFSYM_EXPORT(uxTimerGetReloadMode),
    ESP_ELFSYM_EXPORT(ieee80211_auth_construct),
    ESP_ELFSYM_EXPORT(mbedtls_psa_rsa_sign_hash),
    ESP_ELFSYM_EXPORT(esp_test_clr_tx_statistics),
    ESP_ELFSYM_EXPORT(ledc_set_duty_with_hpoint),
    ESP_ELFSYM_EXPORT(phy_set_modem_flag),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ecp_load_public_part),
    ESP_ELFSYM_EXPORT(r_os_mbuf_pack_chains),
    ESP_ELFSYM_EXPORT(mbedtls_md_info_from_type),
    ESP_ELFSYM_EXPORT(psa_cipher_decrypt),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_len),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_le_start_encrypt),
    ESP_ELFSYM_EXPORT(ieee80211_twt_teardown_all),
    ESP_ELFSYM_EXPORT(httpd_resp_set_hdr),
    ESP_ELFSYM_EXPORT(ampdu_dispatch_all),
    ESP_ELFSYM_EXPORT(esp_psram_impl_get_available_size),
    ESP_ELFSYM_EXPORT(esp_crt_verify_callback),
    ESP_ELFSYM_EXPORT(hal_init),
    ESP_ELFSYM_EXPORT(esp_libc_timekeeping_set_realtime_us),
    ESP_ELFSYM_EXPORT(usb_midi_device_is_connected),
    ESP_ELFSYM_EXPORT(lv_image_decoder_set_open_cb),
    ESP_ELFSYM_EXPORT(lv_obj_center),
    ESP_ELFSYM_EXPORT(xTaskCreatePinnedToCore),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_assign),
    ESP_ELFSYM_EXPORT(phy_set_lb_txiq),
    ESP_ELFSYM_EXPORT(cnx_sta_connect_cmd),
    ESP_ELFSYM_EXPORT(dhcps_get_hostname_on_mac),
    ESP_ELFSYM_EXPORT(get_phy_version_str),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_list_read_size),
    ESP_ELFSYM_EXPORT(ble_sm_init),
    ESP_ELFSYM_EXPORT(ble_hs_hci_init),
    ESP_ELFSYM_EXPORT(rcP2P11NSchedTbl),
    ESP_ELFSYM_EXPORT(phy_get_lock),
    ESP_ELFSYM_EXPORT(phy_loop_clk_en),
    ESP_ELFSYM_EXPORT(USB_MSC_DEVICE_TYPE),
    ESP_ELFSYM_EXPORT(hostap_delete_ptk),
    ESP_ELFSYM_EXPORT(gdma_hal_read_intr_status),
    ESP_ELFSYM_EXPORT(phy_magtrk_scale_set),
    ESP_ELFSYM_EXPORT(lodepng_clear_icc),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_auth_decrypt),
    ESP_ELFSYM_EXPORT(UART_CONTROLLER_TYPE),
    ESP_ELFSYM_EXPORT(npl_freertos_hw_enter_critical),
    ESP_ELFSYM_EXPORT(MPI_BLOCK_BASES),
    ESP_ELFSYM_EXPORT(device_for_each),
    ESP_ELFSYM_EXPORT(mbedtls_md_finish),
    ESP_ELFSYM_EXPORT(lv_font_set_kerning),
    ESP_ELFSYM_EXPORT(lv_spinbox_set_cursor_pos),
    ESP_ELFSYM_EXPORT(ff_sdmmc_set_disk_status_check),
    ESP_ELFSYM_EXPORT(temperature_sensor_hal_get_degree),
    ESP_ELFSYM_EXPORT(wpa_sm_alloc_eapol),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_deauth_internal),
    ESP_ELFSYM_EXPORT(wpa_auth_set_rsn_selection),
    ESP_ELFSYM_EXPORT(esp_set_time_from_rtc),
    ESP_ELFSYM_EXPORT(cnx_node_leave),
    ESP_ELFSYM_EXPORT(__moddi3),
    ESP_ELFSYM_EXPORT(rtc_clk_cpu_freq_set_config),
    ESP_ELFSYM_EXPORT(ieee80211_crypto_gmac_decrypt),
    ESP_ELFSYM_EXPORT(gpio_config),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEED1Ev),
    ESP_ELFSYM_EXPORT(netif_default),
    ESP_ELFSYM_EXPORT(ble_hs_conn_can_alloc),
    ESP_ELFSYM_EXPORT(phy_txiq_get_mis_pwr),
    ESP_ELFSYM_EXPORT(g_mesh_is_started),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_cipher_to_psa),
    ESP_ELFSYM_EXPORT(ieee80211_add_ie_vendor_esp_now),
    ESP_ELFSYM_EXPORT(phy_linear_to_db_64bits),
    ESP_ELFSYM_EXPORT(__dtoa_engine),
    ESP_ELFSYM_EXPORT(vTaskGetSnapshot),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_muladd),
    ESP_ELFSYM_EXPORT(rtc_dig_clk8m_disable),
    ESP_ELFSYM_EXPORT(ieee80211_he_node_init),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_min_height),
    ESP_ELFSYM_EXPORT(ic_register_net80211_tx_cb),
    ESP_ELFSYM_EXPORT(minitar_read_entry),
    ESP_ELFSYM_EXPORT(ieee80211_parse_he_mu_edca),
    ESP_ELFSYM_EXPORT(wpa_deinit),
    ESP_ELFSYM_EXPORT(__isinfd),
    ESP_ELFSYM_EXPORT(esp_aes_intr_alloc),
    ESP_ELFSYM_EXPORT(i2s_hal_tdm_set_tx_slot),
    ESP_ELFSYM_EXPORT(test_parse_rx_mu_mimo),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_rsassa_pkcs1_v15_verify),
    ESP_ELFSYM_EXPORT(audio_codec_set_mute),
    ESP_ELFSYM_EXPORT(esp_wifi_get_user_init_flag_internal),
    ESP_ELFSYM_EXPORT(r_rtc0_timer_handler),
    ESP_ELFSYM_EXPORT(ble_hs_unlock),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_rsassa_pss_sign_ext),
    ESP_ELFSYM_EXPORT(itwt_setup_dwell_timeout_fn_process),
    ESP_ELFSYM_EXPORT(netif_ip6_addr_set_state),
    ESP_ELFSYM_EXPORT(esp_mbedtls_read),
    ESP_ELFSYM_EXPORT(ieee80211_rate_ref_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_eff_max_tx_rx_time_get),
    ESP_ELFSYM_EXPORT(sta_eapol_txdone_cb),
    ESP_ELFSYM_EXPORT(ble_hs_hci_util_set_random_addr),
    ESP_ELFSYM_EXPORT(nan_dw_start_process),
    ESP_ELFSYM_EXPORT(bt_agc_target_set),
    ESP_ELFSYM_EXPORT(hmac_sha256_kdf),
    ESP_ELFSYM_EXPORT(hal_he_clr_multi_bssid),
    ESP_ELFSYM_EXPORT(pm_update_at_next_beacon),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_send_truncated),
    ESP_ELFSYM_EXPORT(pbuf_remove_header),
    ESP_ELFSYM_EXPORT(modem_clock_hal_set_ble_rtc_timer_divisor_value),
    ESP_ELFSYM_EXPORT(g_beacon_idx),
    ESP_ELFSYM_EXPORT(rsn_pmkid),
    ESP_ELFSYM_EXPORT(ic_set_interface),
    ESP_ELFSYM_EXPORT(_ZTIN9__gnu_cxx26__concurrence_unlock_errorE),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_free),
    ESP_ELFSYM_EXPORT(lv_slider_class),
    ESP_ELFSYM_EXPORT(r_ble_log_hci_log_hex),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_x),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_new),
    ESP_ELFSYM_EXPORT(rv_core_critical_regs_save),
    ESP_ELFSYM_EXPORT(app_internal_loader_service_manifest),
    ESP_ELFSYM_EXPORT(ieee80211_itwt_teardown),
    ESP_ELFSYM_EXPORT(display_get_backlight),
    ESP_ELFSYM_EXPORT(pmksa_cache_clear_current),
    ESP_ELFSYM_EXPORT(psa_copy_key),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_ptr_check_enabled),
    ESP_ELFSYM_EXPORT(httpd_sess_get),
    ESP_ELFSYM_EXPORT(i2s_hal_pdm_set_rx_slot),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_conn),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_update_rxbuf_insert_ptr),
    ESP_ELFSYM_EXPORT(wpa_sm_set_ap_rsne_override),
    ESP_ELFSYM_EXPORT(lv_spinbox_increment),
    ESP_ELFSYM_EXPORT(phy_freq_to_chan),
    ESP_ELFSYM_EXPORT(ieee80211_parse_uora),
    ESP_ELFSYM_EXPORT(coex_deinit),
    ESP_ELFSYM_EXPORT(usb_hid_device_send_consumer),
    ESP_ELFSYM_EXPORT(lv_theme_get_font_normal),
    ESP_ELFSYM_EXPORT(ble_hs_process_rx_data_queue),
    ESP_ELFSYM_EXPORT(lv_dropdown_open),
    ESP_ELFSYM_EXPORT(http_message_needs_eof),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_prof_password_internal),
    ESP_ELFSYM_EXPORT(psa_hash_clone),
    ESP_ELFSYM_EXPORT(s_ble_ll_arr_env),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_get_realtime_info),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_iq_sample_num_get),
    ESP_ELFSYM_EXPORT(mbedtls_md_hmac_setup),
    ESP_ELFSYM_EXPORT(ic_del_rx_ba),
    ESP_ELFSYM_EXPORT(tlsf_memalign_offs),
    ESP_ELFSYM_EXPORT(lv_arc_align_obj_to_angle),
    ESP_ELFSYM_EXPORT(wpa_supplicant_send_2_of_4),
    ESP_ELFSYM_EXPORT(freertos_symbols),
    ESP_ELFSYM_EXPORT(modem_clock_hal_set_clock_domain_icg_bitmap),
    ESP_ELFSYM_EXPORT(ble_hs_id_ctx_free),
    ESP_ELFSYM_EXPORT(hal_he_set_tid_bitmap),
    ESP_ELFSYM_EXPORT(__ffssi2),
    ESP_ELFSYM_EXPORT(USB_HOST_MIDI_TYPE),
    ESP_ELFSYM_EXPORT(wifi_get_protocol_process),
    ESP_ELFSYM_EXPORT(gpio_hal_matrix_in),
    ESP_ELFSYM_EXPORT(esp_sha_write_digest_state),
    ESP_ELFSYM_EXPORT(esp_aes_process_dma),
    ESP_ELFSYM_EXPORT(gpio_descriptor_release),
    ESP_ELFSYM_EXPORT(pointer_set_swap_xy),
    ESP_ELFSYM_EXPORT(wifi_check_and_handle_rmac_timer_pending),
    ESP_ELFSYM_EXPORT(raw_input),
    ESP_ELFSYM_EXPORT(ieee80211_getmgtframe),
    ESP_ELFSYM_EXPORT(adiList_stack_setEnable),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sm_event_init),
    ESP_ELFSYM_EXPORT(lvgl_keyboard_add),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_free),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_update_ad),
    ESP_ELFSYM_EXPORT(phy_close_rf),
    ESP_ELFSYM_EXPORT(phy_i2c_sdm_en),
    ESP_ELFSYM_EXPORT(i2c_acquire_bus_handle),
    ESP_ELFSYM_EXPORT(esp_aes_cipher_decrypt_setup),
    ESP_ELFSYM_EXPORT(sdmmc_card_print_info),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_aux_calculate_payload),
    ESP_ELFSYM_EXPORT(xEventGroupGetBitsFromISR),
    ESP_ELFSYM_EXPORT(temperature_sensor_power_acquire),
    ESP_ELFSYM_EXPORT(g_startup_fn),
    ESP_ELFSYM_EXPORT(s_fix_rate),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_bg_image_src),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_parse_key),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_chk_phy_masks),
    ESP_ELFSYM_EXPORT(httpd_resp_set_status),
    ESP_ELFSYM_EXPORT(scan_pm_offchan),
    ESP_ELFSYM_EXPORT(esp_now_deinit),
    ESP_ELFSYM_EXPORT(coex_schm_bt_page_wifi_scan),
    ESP_ELFSYM_EXPORT(esp_test_rx_parse_mu),
    ESP_ELFSYM_EXPORT(gpio_descriptor_disable_interrupt),
    ESP_ELFSYM_EXPORT(lv_obj_get_ext_draw_size),
    ESP_ELFSYM_EXPORT(_rv_core_critical_regs_restore),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_keypair_free),
    ESP_ELFSYM_EXPORT(lv_obj_redraw),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_alg),
    ESP_ELFSYM_EXPORT(phy_pbus_rd),
    ESP_ELFSYM_EXPORT(hal_he_init),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_snap_x),
    ESP_ELFSYM_EXPORT(btdm_broker_detach),
    ESP_ELFSYM_EXPORT(esp_cache_err_has_active_err),
    ESP_ELFSYM_EXPORT(sdmmc_init_cid),
    ESP_ELFSYM_EXPORT(sys_mutex_lock),
    ESP_ELFSYM_EXPORT(esp_flash_chip_generic),
    ESP_ELFSYM_EXPORT(ieee80211_send_nulldata),
    ESP_ELFSYM_EXPORT(audio_codec_get_mute),
    ESP_ELFSYM_EXPORT(ppEnqueueTxDone),
    ESP_ELFSYM_EXPORT(mbedtls_psa_crypto_free),
    ESP_ELFSYM_EXPORT(ceil),
    ESP_ELFSYM_EXPORT(httpd_sess_init),
    ESP_ELFSYM_EXPORT(mac_tx_set_hesig),
    ESP_ELFSYM_EXPORT(psa_to_md_errors),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_renegotiate),
    ESP_ELFSYM_EXPORT(lv_obj_get_child_count),
    ESP_ELFSYM_EXPORT(__wrap___register_frame_table),
    ESP_ELFSYM_EXPORT(ieee80211_phy_type_get),
    ESP_ELFSYM_EXPORT(esp_test_clr_rx_mu_statistics),
    ESP_ELFSYM_EXPORT(sta_get_owe_data),
    ESP_ELFSYM_EXPORT(tcp_tw_pcbs),
    ESP_ELFSYM_EXPORT(ble_uuid_length),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_update_did),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_hdr_parse),
    ESP_ELFSYM_EXPORT(netif_set_default),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_set_data_related_addr_change),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_add_abs),
    ESP_ELFSYM_EXPORT(spi_bus_lock_wait_bg_done),
    ESP_ELFSYM_EXPORT(__wrap_lv_obj_create),
    ESP_ELFSYM_EXPORT(pm_disable_keep_alive_timer),
    ESP_ELFSYM_EXPORT(wifi_nvs_reset_current_ap_info),
    ESP_ELFSYM_EXPORT(bootloader_flash_execute_command_common),
    ESP_ELFSYM_EXPORT(lv_draw_get_available_task),
    ESP_ELFSYM_EXPORT(i2c_del_master_bus),
    ESP_ELFSYM_EXPORT(USB_MIDI_DEVICE_TYPE),
    ESP_ELFSYM_EXPORT(cnx_remove_rc),
    ESP_ELFSYM_EXPORT(esp_http_client_set_user_data),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_get_bit),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_reschedule),
    ESP_ELFSYM_EXPORT(strspn),
    ESP_ELFSYM_EXPORT(mbedtls_aes_crypt_cfb128),
    ESP_ELFSYM_EXPORT(esp_vfs_select),
    ESP_ELFSYM_EXPORT(esp_libc_include_reent_syscalls_impl),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_update_eff_data_len),
    ESP_ELFSYM_EXPORT(esp_aes_dma_done),
    ESP_ELFSYM_EXPORT(app_scheduler_start),
    ESP_ELFSYM_EXPORT(_ZTVSt7num_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_cond_assign),
    ESP_ELFSYM_EXPORT(xQueueGenericReset),
    ESP_ELFSYM_EXPORT(_ZGVNSt8messagesIwE2idE),
    ESP_ELFSYM_EXPORT(ble_ll_conn_subrate_set),
    ESP_ELFSYM_EXPORT(pm_get_tbtt_count),
    ESP_ELFSYM_EXPORT(esp_cache_freeze_ext_mem_cache),
    ESP_ELFSYM_EXPORT(__retarget_lock_acquire),
    ESP_ELFSYM_EXPORT(ble_random_env_p),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_callout_env_init),
    ESP_ELFSYM_EXPORT(ieee80211_decrypt_espnow_pkt),
    ESP_ELFSYM_EXPORT(ble_store_config_conf_init),
    ESP_ELFSYM_EXPORT(lv_arc_set_bg_start_angle),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_deinit),
    ESP_ELFSYM_EXPORT(ieee80211_is_robust_mgmt_frm),
    ESP_ELFSYM_EXPORT(phy_wifi_fbw_sel),
    ESP_ELFSYM_EXPORT(phy_pwdet_ref_code),
    ESP_ELFSYM_EXPORT(ble_hs_id_addr),
    ESP_ELFSYM_EXPORT(charge_pump_enable),
    ESP_ELFSYM_EXPORT(wifi_on_coex_start_process),
    ESP_ELFSYM_EXPORT(_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc),
    ESP_ELFSYM_EXPORT(os_sleep),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_list_size),
    ESP_ELFSYM_EXPORT(_ZTTSd),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_invert_color),
    ESP_ELFSYM_EXPORT(rc11BSchedTbl),
    ESP_ELFSYM_EXPORT(esp_test_rx_trs_count),
    ESP_ELFSYM_EXPORT(lv_obj_report_style_change),
    ESP_ELFSYM_EXPORT(chm_return_home_channel),
    ESP_ELFSYM_EXPORT(esp_netif_get_ifkey),
    ESP_ELFSYM_EXPORT(ccmp),
    ESP_ELFSYM_EXPORT(phy_i2c_sar2_init_code),
    ESP_ELFSYM_EXPORT(scan_parse_beacon),
    ESP_ELFSYM_EXPORT(i2c_master_write_read_device),
    ESP_ELFSYM_EXPORT(phy_bt_tx_gain_set),
    ESP_ELFSYM_EXPORT(coex_pti_tab),
    ESP_ELFSYM_EXPORT(esp_log_va),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_enc_allowed_pdu_tx),
    ESP_ELFSYM_EXPORT(phy_vht_support),
    ESP_ELFSYM_EXPORT(r_ble_ll_disconn_comp_event_send),
    ESP_ELFSYM_EXPORT(lv_group_send_data),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_KEY_PURPOSE_1),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_alpn_ext),
    ESP_ELFSYM_EXPORT(lv_draw_sw_grad_cleanup),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_delete),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sm_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_get_sync_info_field),
    ESP_ELFSYM_EXPORT(hal_set_ofdma_sequence_pti),
    ESP_ELFSYM_EXPORT(esp_partition_find_err),
    ESP_ELFSYM_EXPORT(esp_cache_get_alignment),
    ESP_ELFSYM_EXPORT(uart_hal_txfifo_rst),
    ESP_ELFSYM_EXPORT(gettimeofday),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_CH4_ATTEN0_INITCODE_DIFF),
    ESP_ELFSYM_EXPORT(g_wifi_default_wpa_crypto_funcs),
    ESP_ELFSYM_EXPORT(test_rx_process_complete_noeb),
    ESP_ELFSYM_EXPORT(ble_gatts_indicate_fail_notconn),
    ESP_ELFSYM_EXPORT(clk_hal_cpu_get_freq_hz),
    ESP_ELFSYM_EXPORT(esp_http_client_flush_response),
    ESP_ELFSYM_EXPORT(phy_wifi_agc_sat_gain),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_read_mult),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_byte_to_word_size),
    ESP_ELFSYM_EXPORT(esp_http_client_delete_header),
    ESP_ELFSYM_EXPORT(lora_transmit),
    ESP_ELFSYM_EXPORT(mbedtls_pk_verify),
    ESP_ELFSYM_EXPORT(_ZZNSt13basic_filebufIcSt11char_traitsIcEE5closeEvEN14__close_sentryD1Ev),
    ESP_ELFSYM_EXPORT(ieee80211_encap_esfbuf_htc),
    ESP_ELFSYM_EXPORT(esp_libc_include_assert_impl),
    ESP_ELFSYM_EXPORT(esp_tls_plain_tcp_connect),
    ESP_ELFSYM_EXPORT(pm_coex_pwr_update),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_deinit),
    ESP_ELFSYM_EXPORT(ieee80211_softap_add_bss_color_change_announcement),
    ESP_ELFSYM_EXPORT(lvgl_devices_attach),
    ESP_ELFSYM_EXPORT(wifi_init_process),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_sched_next_anchor),
    ESP_ELFSYM_EXPORT(hal_get_tx_min_pwr),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_tx_done),
    ESP_ELFSYM_EXPORT(wifi_station_get_ipv4_address),
    ESP_ELFSYM_EXPORT(app_manager_start_with_parameters),
    ESP_ELFSYM_EXPORT(audio_codec_get_native_channels),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_notify),
    ESP_ELFSYM_EXPORT(rtcio_hal_iomux_output),
    ESP_ELFSYM_EXPORT(g_msys_num_get_cb),
    ESP_ELFSYM_EXPORT(esp_tls_conn_read),
    ESP_ELFSYM_EXPORT(mt_deinit),
    ESP_ELFSYM_EXPORT(mem_trim),
    ESP_ELFSYM_EXPORT(lv_style_set_prop),
    ESP_ELFSYM_EXPORT(lv_obj_transform_point_array),
    ESP_ELFSYM_EXPORT(lv_label_set_text_selection_end),
    ESP_ELFSYM_EXPORT(icmp_input),
    ESP_ELFSYM_EXPORT(strcat),
    ESP_ELFSYM_EXPORT(ieee80211_set_phy_mode),
    ESP_ELFSYM_EXPORT(set_bb_wdg),
    ESP_ELFSYM_EXPORT(rcGetSched),
    ESP_ELFSYM_EXPORT(wifi_is_scanning),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_transfer_received),
    ESP_ELFSYM_EXPORT(g_adiList_stack_envP),
    ESP_ELFSYM_EXPORT(r_ble_lll_get_rxed_buffer),
    ESP_ELFSYM_EXPORT(lv_ll_is_empty),
    ESP_ELFSYM_EXPORT(igmp_leavegroup_netif),
    ESP_ELFSYM_EXPORT(g_log_level),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_finish_padded),
    ESP_ELFSYM_EXPORT(esp_sleep_enable_gpio_switch),
    ESP_ELFSYM_EXPORT(ieee80211_send_probereq),
    ESP_ELFSYM_EXPORT(psa_generate_random),
    ESP_ELFSYM_EXPORT(bluetooth_serial_stop),
    ESP_ELFSYM_EXPORT(coex_core_timer_idx_get),
    ESP_ELFSYM_EXPORT(lv_display_is_invalidation_enabled),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_recycle_sch_item),
    ESP_ELFSYM_EXPORT(lv_fs_close),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sync_pdu_ext_hdr_make),
    ESP_ELFSYM_EXPORT(esp32_sdspi_fs_free),
    ESP_ELFSYM_EXPORT(esp_wifi_start),
    ESP_ELFSYM_EXPORT(lv_color_darken),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_reset_nvs_pmk_internal),
    ESP_ELFSYM_EXPORT(lv_area_get_height),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_pad_right),
    ESP_ELFSYM_EXPORT(phy_force_txrx_off_new),
    ESP_ELFSYM_EXPORT(httpd_query_key_value),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_get_anchor),
    ESP_ELFSYM_EXPORT(http_parser_settings_init),
    ESP_ELFSYM_EXPORT(i2c_reset_tx_fifo),
    ESP_ELFSYM_EXPORT(esp_intr_alloc_info),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_init),
    ESP_ELFSYM_EXPORT(rtc_gpio_iomux_input),
    ESP_ELFSYM_EXPORT(phy_fe_reg_init),
    ESP_ELFSYM_EXPORT(hal_mac_tx_config_edca),
    ESP_ELFSYM_EXPORT(esp_sleep_config_gpio_isolate),
    ESP_ELFSYM_EXPORT(npl_freertos_sem_deinit),
    ESP_ELFSYM_EXPORT(feof),
    ESP_ELFSYM_EXPORT(esp_test_rx_ctrl),
    ESP_ELFSYM_EXPORT(hci_stack_disable),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_check_zero_ct),
    ESP_ELFSYM_EXPORT(ppSetBarRate),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_KEY_PURPOSE_5),
    ESP_ELFSYM_EXPORT(sys_thread_tcpip),
    ESP_ELFSYM_EXPORT(_ZSt7getlineIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EE),
    ESP_ELFSYM_EXPORT(rgb_led_pwm_driver),
    ESP_ELFSYM_EXPORT(lv_tabview_set_tab_bar_size),
    ESP_ELFSYM_EXPORT(hmac_hal_next_block_padding),
    ESP_ELFSYM_EXPORT(phy_get_tcode),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service7rtctime14RtcTimeServiceE),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_set_new_adv_data),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_parse),
    ESP_ELFSYM_EXPORT(memory_calloc_with_policy),
    ESP_ELFSYM_EXPORT(pwr_hal_clear_mac_modem_rx_beacon_miss_counter),
    ESP_ELFSYM_EXPORT(lv_spangroup_get_max_line_height),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_rxbuf_cnt_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_msys_update),
    ESP_ELFSYM_EXPORT(lmacProcessCollisions_task),
    ESP_ELFSYM_EXPORT(esp_vfs_unregister_fs_with_id),
    ESP_ELFSYM_EXPORT(esp_http_client_set_url),
    ESP_ELFSYM_EXPORT(ble_hs_unlock_nested),
    ESP_ELFSYM_EXPORT(phy_gen_rx_gain_table),
    ESP_ELFSYM_EXPORT(__bufio_buffer_allocate_locked),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_chk_resume),
    ESP_ELFSYM_EXPORT(rgb_led_gpio_driver),
    ESP_ELFSYM_EXPORT(i2c_hal_master_set_scl_timeout_val),
    ESP_ELFSYM_EXPORT(udp_connect),
    ESP_ELFSYM_EXPORT(key_mgr_wait_for_state),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_connect_peri),
    ESP_ELFSYM_EXPORT(sta_michael_mic_failure),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_point_write_binary),
    ESP_ELFSYM_EXPORT(device_set_driver),
    ESP_ELFSYM_EXPORT(esp_coex_adapter_register),
    ESP_ELFSYM_EXPORT(vTimerSetReloadMode),
    ESP_ELFSYM_EXPORT(rcLoRaSchedTbl),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_phymode),
    ESP_ELFSYM_EXPORT(esp_timer_impl_set_alarm_id),
    ESP_ELFSYM_EXPORT(lv_timer_ready),
    ESP_ELFSYM_EXPORT(spi_flash_init_lock),
    ESP_ELFSYM_EXPORT(pmksa_cache_auth_create_entry),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_hci_scheduling_len_set),
    ESP_ELFSYM_EXPORT(ic_get_default_sched),
    ESP_ELFSYM_EXPORT(esp_wifi_get_config),
    ESP_ELFSYM_EXPORT(hal_enable_rx_statistics),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_get_list),
    ESP_ELFSYM_EXPORT(r_ble_vendor_hci_duplicate_control),
    ESP_ELFSYM_EXPORT(xQueueGenericCreate),
    ESP_ELFSYM_EXPORT(esp_netif_get_hostname),
    ESP_ELFSYM_EXPORT(module_remove),
    ESP_ELFSYM_EXPORT(file_mutex_register),
    ESP_ELFSYM_EXPORT(vfwprintf),
    ESP_ELFSYM_EXPORT(aes_unwrap),
    ESP_ELFSYM_EXPORT(_ZZNKSt7__cxx117collateIwE12do_transformEPKwS3_EN4_BufD2Ev),
    ESP_ELFSYM_EXPORT(pm_start_twt),
    ESP_ELFSYM_EXPORT(esp_aes_setkey),
    ESP_ELFSYM_EXPORT(lv_cache_entry_get_ref),
    ESP_ELFSYM_EXPORT(spi_bus_get_attr),
    ESP_ELFSYM_EXPORT(ic_get_key),
    ESP_ELFSYM_EXPORT(pthread_key_create),
    ESP_ELFSYM_EXPORT(__retarget_lock_release),
    ESP_ELFSYM_EXPORT(lv_point_set),
    ESP_ELFSYM_EXPORT(pp_printf),
    ESP_ELFSYM_EXPORT(_Z29tusbStartMassStorageWithSdmmcb),
    ESP_ELFSYM_EXPORT(bt_bb_rx_set),
    ESP_ELFSYM_EXPORT(esp_fp_generated_step),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx118numpunctIwEE),
    ESP_ELFSYM_EXPORT(r_osi_coex_funcs_p),
    ESP_ELFSYM_EXPORT(phy_enable_low_rate),
    ESP_ELFSYM_EXPORT(remove),
    ESP_ELFSYM_EXPORT(vApplicationGetIdleTaskMemory),
    ESP_ELFSYM_EXPORT(lv_bar_class),
    ESP_ELFSYM_EXPORT(hmac_hal_write_block_512),
    ESP_ELFSYM_EXPORT(scan_hci_sendAdvLostReportVsEvent),
    ESP_ELFSYM_EXPORT(pm_set_next_tbtt),
    ESP_ELFSYM_EXPORT(gdma_hal_stop),
    ESP_ELFSYM_EXPORT(esp_crypto_mpi_lock_release),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY1),
    ESP_ELFSYM_EXPORT(rmdir),
    ESP_ELFSYM_EXPORT(file_system_for_each),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt10moneypunctIwLb0EEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(lv_style_set_shadow_opa),
    ESP_ELFSYM_EXPORT(phy_track_pll_init),
    ESP_ELFSYM_EXPORT(wifi_add_event_callback),
    ESP_ELFSYM_EXPORT(ble_hs_conn_foreach),
    ESP_ELFSYM_EXPORT(ieee80211_send_setup),
    ESP_ELFSYM_EXPORT(spi_bus_lock_bg_exit),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_unregister_path),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_piscan_wifi_connecting),
    ESP_ELFSYM_EXPORT(gpio_isr_handler_add),
    ESP_ELFSYM_EXPORT(panic_print_backtrace),
    ESP_ELFSYM_EXPORT(ppTxPkt),
    ESP_ELFSYM_EXPORT(extAdv_stack_enable),
    ESP_ELFSYM_EXPORT(net80211_data_ptr_init),
    ESP_ELFSYM_EXPORT(tcp_free_ooseq),
    ESP_ELFSYM_EXPORT(lv_menu_back_button_is_root),
    ESP_ELFSYM_EXPORT(hal_clear_rx_beacon_pti),
    ESP_ELFSYM_EXPORT(r_ble_log_internal_x1),
    ESP_ELFSYM_EXPORT(lv_style_init),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_disconnect_all),
    ESP_ELFSYM_EXPORT(lv_style_set_pad_column),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_key_usage),
    ESP_ELFSYM_EXPORT(sdspi_host_start_command),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_module_init),
    ESP_ELFSYM_EXPORT(pm_rx_data_process),
    ESP_ELFSYM_EXPORT(s_keys),
    ESP_ELFSYM_EXPORT(check_comeback_token),
    ESP_ELFSYM_EXPORT(usb_hid_device_send_gamepad),
    ESP_ELFSYM_EXPORT(lv_indev_get_next),
    ESP_ELFSYM_EXPORT(audio_codec_write),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_client_cert_data_der),
    ESP_ELFSYM_EXPORT(hostap_init),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_line_width),
    ESP_ELFSYM_EXPORT(ic_add_extra_softap_rx_ba),
    ESP_ELFSYM_EXPORT(esp_aes_crypt_ecb),
    ESP_ELFSYM_EXPORT(chm_get_current_band),
    ESP_ELFSYM_EXPORT(psa_export_public_key),
    ESP_ELFSYM_EXPORT(tcp_rexmit_rto_commit),
    ESP_ELFSYM_EXPORT(psa_generate_key),
    ESP_ELFSYM_EXPORT(lv_display_send_event),
    ESP_ELFSYM_EXPORT(r_get_be32),
    ESP_ELFSYM_EXPORT(pm_send_probe_start),
    ESP_ELFSYM_EXPORT(wpa_sm_init),
    ESP_ELFSYM_EXPORT(lv_draw_buf_reshape),
    ESP_ELFSYM_EXPORT(__wtime_am_pm),
    ESP_ELFSYM_EXPORT(lv_display_get_next),
    ESP_ELFSYM_EXPORT(lv_style_set_transform_height),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_head_check),
    ESP_ELFSYM_EXPORT(g_mmgmt_max_block_cnt),
    ESP_ELFSYM_EXPORT(usb_hid_device_is_connected),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_parse_pubkey),
    ESP_ELFSYM_EXPORT(expm1f),
    ESP_ELFSYM_EXPORT(_Z25app_ledger_free_argumentsiPPc),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_exec_write),
    ESP_ELFSYM_EXPORT(i2c_master_start),
    ESP_ELFSYM_EXPORT(pwrsave_flushq),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_indicate),
    ESP_ELFSYM_EXPORT(huk_hal_configure),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_phy_tx_transition_get),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_update_ad),
    ESP_ELFSYM_EXPORT(_ZTSSt12out_of_range),
    ESP_ELFSYM_EXPORT(tlsf_fit_size),
    ESP_ELFSYM_EXPORT(file_system_remove),
    ESP_ELFSYM_EXPORT(g_ble_ll_conn_evt_dura),
    ESP_ELFSYM_EXPORT(__fixdfdi),
    ESP_ELFSYM_EXPORT(tcp_send_empty_ack),
    ESP_ELFSYM_EXPORT(lv_arc_set_end_angle),
    ESP_ELFSYM_EXPORT(r_scan_duplicate_cache_refresh_timer_start),
    ESP_ELFSYM_EXPORT(chm_check_channel_is_valid),
    ESP_ELFSYM_EXPORT(regi2c_exit_critical),
    ESP_ELFSYM_EXPORT(lv_textarea_set_accepted_chars),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_image_to_argb8888),
    ESP_ELFSYM_EXPORT(putc_unlocked),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(minitar_checksum_header),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_check_periodic_sync),
    ESP_ELFSYM_EXPORT(wcscmp),
    ESP_ELFSYM_EXPORT(ieee80211_he_updatecaps),
    ESP_ELFSYM_EXPORT(_Z22app_metadata_get_valueRKSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_St4lessIS5_ESaISt4pairIKS5_S5_EEERS9_RS5_),
    ESP_ELFSYM_EXPORT(ieee80211_psq_drop_one_pkt),
    ESP_ELFSYM_EXPORT(ble_ll_hci_env_p),
    ESP_ELFSYM_EXPORT(esp_vfs_unregister_with_id),
    ESP_ELFSYM_EXPORT(_ZSt7__writeIwESt19ostreambuf_iteratorIT_St11char_traitsIS1_EES4_PKS1_i),
    ESP_ELFSYM_EXPORT(xTaskCheckForTimeOut),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY5),
    ESP_ELFSYM_EXPORT(adc_oneshot_hal_init),
    ESP_ELFSYM_EXPORT(ieee802_11_parse_elems),
    ESP_ELFSYM_EXPORT(xRingbufferCreate),
    ESP_ELFSYM_EXPORT(mbedtls_md_hmac_starts),
    ESP_ELFSYM_EXPORT(uECC_vli_set),
    ESP_ELFSYM_EXPORT(r_os_mbuf_leadingspace),
    ESP_ELFSYM_EXPORT(bluetooth_get_paired_peers),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_fill_random),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt8numpunctIcEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ffdh_generate_key),
    ESP_ELFSYM_EXPORT(lv_obj_get_screen),
    ESP_ELFSYM_EXPORT(crypto_bignum_rand),
    ESP_ELFSYM_EXPORT(pm_register_connectionless_wake_window),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_read_type_complete),
    ESP_ELFSYM_EXPORT(wifi_bt_common_module_enable),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_delconn),
    ESP_ELFSYM_EXPORT(lv_color32_eq),
    ESP_ELFSYM_EXPORT(heap_caps_aligned_calloc),
    ESP_ELFSYM_EXPORT(hmac_hal_clean),
    ESP_ELFSYM_EXPORT(r_ble_controller_enable),
    ESP_ELFSYM_EXPORT(lv_indev_set_read_cb),
    ESP_ELFSYM_EXPORT(zb_rx_v0_0_set),
    ESP_ELFSYM_EXPORT(lv_display_remove_event_cb_with_user_data),
    ESP_ELFSYM_EXPORT(get_suspend_sp_number),
    ESP_ELFSYM_EXPORT(mbedtls_sha256),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_deleted_irk_store),
    ESP_ELFSYM_EXPORT(ble_hs_hci_add_avail_pkts),
    ESP_ELFSYM_EXPORT(phy_txdc_cal_init),
    ESP_ELFSYM_EXPORT(esp_aes_acquire_hardware),
    ESP_ELFSYM_EXPORT(_he_twt_information_resume),
    ESP_ELFSYM_EXPORT(ble_gap_slave_adv_reattempt),
    ESP_ELFSYM_EXPORT(crypto_ec_point_to_bin),
    ESP_ELFSYM_EXPORT(ppSelectTxFormat),
    ESP_ELFSYM_EXPORT(phy_pbus_xpd_rx_off),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_init),
    ESP_ELFSYM_EXPORT(_Z14get_native_pinP14GpioDescriptor),
    ESP_ELFSYM_EXPORT(setup_timer_param),
    ESP_ELFSYM_EXPORT(_unlink_r),
    ESP_ELFSYM_EXPORT(npl_freertos_eventq_put_to_front),
    ESP_ELFSYM_EXPORT(r_ble_lll_ext_scan_coex_dpc_process),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_pop_overlapped_sch),
    ESP_ELFSYM_EXPORT(prefix_list),
    ESP_ELFSYM_EXPORT(adc_oneshot_hal_convert),
    ESP_ELFSYM_EXPORT(port_uxInterruptNesting),
    ESP_ELFSYM_EXPORT(g_wdev_dbg_rx),
    ESP_ELFSYM_EXPORT(memory_print_stats),
    ESP_ELFSYM_EXPORT(lv_display_get_theme),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_point_cmp),
    ESP_ELFSYM_EXPORT(esp_crypto_aes_enable_periph_clk),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_spiflash_mount_rw_wl),
    ESP_ELFSYM_EXPORT(xEventGroupWaitBits),
    ESP_ELFSYM_EXPORT(wifi_mesh_event_post),
    ESP_ELFSYM_EXPORT(device_construct_add),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_letter_space),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_a2dp_wifi_scan),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_switch_delay_rx_aod_1us),
    ESP_ELFSYM_EXPORT(_putc2),
    ESP_ELFSYM_EXPORT(ieee80211_get_tx_info_from_eb),
    ESP_ELFSYM_EXPORT(device_try_lock),
    ESP_ELFSYM_EXPORT(modem_clock_configure_wifi_status),
    ESP_ELFSYM_EXPORT(wl_clear_ap_no_lr),
    ESP_ELFSYM_EXPORT(esp_bt_controller_enable),
    ESP_ELFSYM_EXPORT(xStreamBufferSpacesAvailable),
    ESP_ELFSYM_EXPORT(wpabuf_free),
    ESP_ELFSYM_EXPORT(empty_func_p_pvoid_r_void),
    ESP_ELFSYM_EXPORT(lv_obj_set_scroll_dir),
    ESP_ELFSYM_EXPORT(spi_flash_hal_disable_auto_suspend_mode),
    ESP_ELFSYM_EXPORT(ble_hs_lock_nested),
    ESP_ELFSYM_EXPORT(pwm_backlight_driver),
    ESP_ELFSYM_EXPORT(rx11AXRate2AMPDULimit),
    ESP_ELFSYM_EXPORT(__fpclassifyf),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_apply),
    ESP_ELFSYM_EXPORT(lv_strcmp),
    ESP_ELFSYM_EXPORT(hid_report_map_keyboard_consumer_mouse),
    ESP_ELFSYM_EXPORT(os_timer_setfn),
    ESP_ELFSYM_EXPORT(_ZTVSt12out_of_range),
    ESP_ELFSYM_EXPORT(_ZSt15get_new_handlerv),
    ESP_ELFSYM_EXPORT(getcwd),
    ESP_ELFSYM_EXPORT(ieee80211_ampdu_start_age_timer),
    ESP_ELFSYM_EXPORT(coex_disable),
    ESP_ELFSYM_EXPORT(pointer_exit_sleep),
    ESP_ELFSYM_EXPORT(xTaskGetCoreID),
    ESP_ELFSYM_EXPORT(cnx_sta_color_change_fn),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_unlock_safely),
    ESP_ELFSYM_EXPORT(scan_set_pas_duration),
    ESP_ELFSYM_EXPORT(empty_func_p_void_r_void),
    ESP_ELFSYM_EXPORT(hal_mac_is_dma_enable),
    ESP_ELFSYM_EXPORT(pm_beacon_monitor_tbtt_stop),
    ESP_ELFSYM_EXPORT(i2c_master_stop),
    ESP_ELFSYM_EXPORT(panic_soc_fill_info),
    ESP_ELFSYM_EXPORT(gdma_del_channel),
    ESP_ELFSYM_EXPORT(device_for_each_of_type),
    ESP_ELFSYM_EXPORT(sdmmc_mmc_get_erase_timeout_ms),
    ESP_ELFSYM_EXPORT(rcGetAmpduSched),
    ESP_ELFSYM_EXPORT(__ledf2),
    ESP_ELFSYM_EXPORT(esp_cache_err_int_init),
    ESP_ELFSYM_EXPORT(phy_mac_enable_bb),
    ESP_ELFSYM_EXPORT(lv_font_get_glyph_width),
    ESP_ELFSYM_EXPORT(lv_strndup),
    ESP_ELFSYM_EXPORT(mbedtls_psa_rsa_import_key),
    ESP_ELFSYM_EXPORT(lv_display_flush_is_last),
    ESP_ELFSYM_EXPORT(btdm_broker_env_init),
    ESP_ELFSYM_EXPORT(esp_netif_free_rx_buffer),
    ESP_ELFSYM_EXPORT(lv_menu_main_header_cont_class),
    ESP_ELFSYM_EXPORT(lv_list_create),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_conn_wifi_conn),
    ESP_ELFSYM_EXPORT(r_ble_log_internal_hex),
    ESP_ELFSYM_EXPORT(lv_anim_set_reverse_delay),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_get_mempool_size),
    ESP_ELFSYM_EXPORT(task_wdt_timeout_abort),
    ESP_ELFSYM_EXPORT(wpa_auth_gen_wpa_ie),
    ESP_ELFSYM_EXPORT(esp32_i2c_master_driver),
    ESP_ELFSYM_EXPORT(ic_del_ptk_and_gtk_alg),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_samp_limit_samp),
    ESP_ELFSYM_EXPORT(hal_vhtdlmu_set_membership_status),
    ESP_ELFSYM_EXPORT(pmksa_cache_flush),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_modem_wakeup_enable),
    ESP_ELFSYM_EXPORT(esp_event_loop_run),
    ESP_ELFSYM_EXPORT(adiList_stack_listAdd),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_coex_dpc_update),
    ESP_ELFSYM_EXPORT(uart_controller_read_byte),
    ESP_ELFSYM_EXPORT(npl_freertos_sem_get_count),
    ESP_ELFSYM_EXPORT(wifi_create_softap),
    ESP_ELFSYM_EXPORT(ble_ll_conn_subrate_req_hci),
    ESP_ELFSYM_EXPORT(USB_HOST_HID_TYPE),
    ESP_ELFSYM_EXPORT(gpio_reset_pin),
    ESP_ELFSYM_EXPORT(ieee80211_setup_phy_mode),
    ESP_ELFSYM_EXPORT(__wrap_gpio_set_direction),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_mgmt_group_cipher),
    ESP_ELFSYM_EXPORT(setenv),
    ESP_ELFSYM_EXPORT(ic_ebuf_recycle_tx),
    ESP_ELFSYM_EXPORT(ble_store_full_event),
    ESP_ELFSYM_EXPORT(rgb_led_set_color),
    ESP_ELFSYM_EXPORT(hal_crypto_disable),
    ESP_ELFSYM_EXPORT(lmacDisableTransmit),
    ESP_ELFSYM_EXPORT(esp_hmac_setup_opaque),
    ESP_ELFSYM_EXPORT(psa_allocate_buffer_to_slot),
    ESP_ELFSYM_EXPORT(USB_HID_DEVICE_TYPE),
    ESP_ELFSYM_EXPORT(bundle_opt_int32),
    ESP_ELFSYM_EXPORT(r_ble_ll_customize_peer_sca_set),
    ESP_ELFSYM_EXPORT(ble_store_read),
    ESP_ELFSYM_EXPORT(wpa_crypto_funcs),
    ESP_ELFSYM_EXPORT(strcasecmp),
    ESP_ELFSYM_EXPORT(sync_stack_disable),
    ESP_ELFSYM_EXPORT(nan_call_ndp_confirm_cb),
    ESP_ELFSYM_EXPORT(ampdu_dispatch),
    ESP_ELFSYM_EXPORT(extAdv_stack_disable),
    ESP_ELFSYM_EXPORT(load_kern),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_rpa_timeout),
    ESP_ELFSYM_EXPORT(_kill_r),
    ESP_ELFSYM_EXPORT(netconn_write_vectors_partly),
    ESP_ELFSYM_EXPORT(esp_transport_connect_async),
    ESP_ELFSYM_EXPORT(_ZSt20__throw_length_errorPKc),
    ESP_ELFSYM_EXPORT(cJSON_IsNumber),
    ESP_ELFSYM_EXPORT(lv_color_hex),
    ESP_ELFSYM_EXPORT(esp_sha_hash_clone),
    ESP_ELFSYM_EXPORT(phy_rfpll_set_freq),
    ESP_ELFSYM_EXPORT(lv_arc_set_start_angle),
    ESP_ELFSYM_EXPORT(lv_textarea_set_password_show_time),
    ESP_ELFSYM_EXPORT(esp_cache_suspend_ext_mem_cache),
    ESP_ELFSYM_EXPORT(lv_lodepng_init),
    ESP_ELFSYM_EXPORT(__assert_func),
    ESP_ELFSYM_EXPORT(lv_obj_get_content_width),
    ESP_ELFSYM_EXPORT(dh_groups_get),
    ESP_ELFSYM_EXPORT(__wrap_lv_switch_create),
    ESP_ELFSYM_EXPORT(r_ble_ll_callout_env_init),
    ESP_ELFSYM_EXPORT(r_ble_hw_periodiclist_rmv),
    ESP_ELFSYM_EXPORT(ble_sm_enc_initiate),
    ESP_ELFSYM_EXPORT(_ZZNSt8__format14__formatter_fpIcE5parseERSt26basic_format_parse_contextIcEENKUlvE0_clEv),
    ESP_ELFSYM_EXPORT(os_memdup),
    ESP_ELFSYM_EXPORT(mbedtls_mutex_unlock),
    ESP_ELFSYM_EXPORT(esp_netif_action_connected),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx118numpunctIwE2idE),
    ESP_ELFSYM_EXPORT(lv_obj_create),
    ESP_ELFSYM_EXPORT(make_hw_cmd),
    ESP_ELFSYM_EXPORT(device_has_active_by_type),
    ESP_ELFSYM_EXPORT(psa_parse_key_data_from_storage),
    ESP_ELFSYM_EXPORT(ble_gatts_tx_notifications),
    ESP_ELFSYM_EXPORT(lv_obj_get_content_height),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_switch_delay_rx_aoa_2us),
    ESP_ELFSYM_EXPORT(hostap_query_mac_in_list),
    ESP_ELFSYM_EXPORT(mbedtls_psa_rsa_load_representation),
    ESP_ELFSYM_EXPORT(esp_md5_hash_finish),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_alloc_rx_buffer_and_hdr),
    ESP_ELFSYM_EXPORT(nvs_open),
    ESP_ELFSYM_EXPORT(chm_get_channel_index),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_sca_update),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_bg_image_opa),
    ESP_ELFSYM_EXPORT(sntp_sync_time),
    ESP_ELFSYM_EXPORT(lv_label_set_text_static),
    ESP_ELFSYM_EXPORT(test_tx_fail_statistics),
    ESP_ELFSYM_EXPORT(lv_indev_read_timer_cb),
    ESP_ELFSYM_EXPORT(npl_freertos_event_get_arg),
    ESP_ELFSYM_EXPORT(r_os_msys_get_pkthdr),
    ESP_ELFSYM_EXPORT(i2c_master_read),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_enable_cmd),
    ESP_ELFSYM_EXPORT(netif_add),
    ESP_ELFSYM_EXPORT(lv_display_get_original_horizontal_resolution),
    ESP_ELFSYM_EXPORT(esp_crosscore_int_send_yield),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_send_per_adv_rpt),
    ESP_ELFSYM_EXPORT(npl_freertos_get_time_forever),
    ESP_ELFSYM_EXPORT(ADC_CONTROLLER_TYPE),
    ESP_ELFSYM_EXPORT(hal_set_sta_tbtt_interval),
    ESP_ELFSYM_EXPORT(lv_style_set_outline_color),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_alg_null),
    ESP_ELFSYM_EXPORT(heap_caps_malloc_base),
    ESP_ELFSYM_EXPORT(_ZTVSt15_Sp_counted_ptrIPN2tt7service11displayidle18DisplayIdleServiceELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(ieee80211_recycle_cache_eb),
    ESP_ELFSYM_EXPORT(ieee80211_alloc_challenge),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_beacon_miss_intr_enable),
    ESP_ELFSYM_EXPORT(xTaskGenericNotifyWait),
    ESP_ELFSYM_EXPORT(panicHandler),
    ESP_ELFSYM_EXPORT(phy_freq_reg_init_new),
    ESP_ELFSYM_EXPORT(phy_bt_get_tx_gain_new),
    ESP_ELFSYM_EXPORT(regdma_link_stats),
    ESP_ELFSYM_EXPORT(wifi_softap_start),
    ESP_ELFSYM_EXPORT(psa_hash_setup),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_cfg_hs),
    ESP_ELFSYM_EXPORT(lv_cache_release),
    ESP_ELFSYM_EXPORT(psa_sign_hash),
    ESP_ELFSYM_EXPORT(gdma_get_group_channel_id),
    ESP_ELFSYM_EXPORT(__cxa_guard_release),
    ESP_ELFSYM_EXPORT(ieee80211_add_vhtcap),
    ESP_ELFSYM_EXPORT(scan_parse_ht2040_coex),
    ESP_ELFSYM_EXPORT(cipher_type_map_public_to_supp),
    ESP_ELFSYM_EXPORT(r_priv_sdk_config_options_init),
    ESP_ELFSYM_EXPORT(nvs_set_u16),
    ESP_ELFSYM_EXPORT(wpa_ap_get_peer_spp_msg),
    ESP_ELFSYM_EXPORT(hmac_sha1),
    ESP_ELFSYM_EXPORT(spi_flash_hal_supports_direct_write),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handshake_server_step),
    ESP_ELFSYM_EXPORT(lv_draw_border_dsc_init),
    ESP_ELFSYM_EXPORT(key_mgr_hal_set_xts_aes_key_len),
    ESP_ELFSYM_EXPORT(strtoul),
    ESP_ELFSYM_EXPORT(lv_span_set_text),
    ESP_ELFSYM_EXPORT(esp_transport_tcp_set_keep_alive),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_update),
    ESP_ELFSYM_EXPORT(gdma_hal_deinit),
    ESP_ELFSYM_EXPORT(gdma_hal_clear_intr),
    ESP_ELFSYM_EXPORT(lwip_getsockname),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_read_mult_var),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_periodic_set_info_transfer),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_env_deinit),
    ESP_ELFSYM_EXPORT(mac_tx_set_htsig),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_read_reg),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_line_opa),
    ESP_ELFSYM_EXPORT(pmu_hp_system_analog_param_default),
    ESP_ELFSYM_EXPORT(_lseek_r),
    ESP_ELFSYM_EXPORT(mbedtls_psa_mac_sign_finish),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handshake_free),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_find_configured),
    ESP_ELFSYM_EXPORT(esp_timer_impl_init),
    ESP_ELFSYM_EXPORT(r_bt_rf_coex_coded_txrx_time_upper_lim),
    ESP_ELFSYM_EXPORT(pm_noise_check),
    ESP_ELFSYM_EXPORT(psa_key_derivation_set_capacity),
    ESP_ELFSYM_EXPORT(r_ble_ll_init_get_conn_comp_ev),
    ESP_ELFSYM_EXPORT(__log10Pow2),
    ESP_ELFSYM_EXPORT(tcp_listen_with_backlog_and_err),
    ESP_ELFSYM_EXPORT(esp_mbedtls_cleanup),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_go_idle_state),
    ESP_ELFSYM_EXPORT(ip4addr_ntoa_r),
    ESP_ELFSYM_EXPORT(pm_get_sleep_type),
    ESP_ELFSYM_EXPORT(lvgl_module),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_hci_enable),
    ESP_ELFSYM_EXPORT(lv_text_encoded_get_byte_id),
    ESP_ELFSYM_EXPORT(pm_incr_twt_active_cnt),
    ESP_ELFSYM_EXPORT(lv_obj_set_ext_click_area),
    ESP_ELFSYM_EXPORT(pcl_stack_enableSetRssiThreshVsCmd),
    ESP_ELFSYM_EXPORT(lv_keyboard_set_textarea),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_get_write_protect),
    ESP_ELFSYM_EXPORT(coex_core_pti_get),
    ESP_ELFSYM_EXPORT(tcp_shutdown),
    ESP_ELFSYM_EXPORT(ble_gap_adv_active),
    ESP_ELFSYM_EXPORT(pmu_lp_system_init),
    ESP_ELFSYM_EXPORT(esp_time_impl_get_time_since_boot),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_swap),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_remove_entry),
    ESP_ELFSYM_EXPORT(ieee80211_align_eb),
    ESP_ELFSYM_EXPORT(uxTaskGetStackHighWaterMark),
    ESP_ELFSYM_EXPORT(mkdir),
    ESP_ELFSYM_EXPORT(spi_flash_init_chip_state),
    ESP_ELFSYM_EXPORT(test_rx_parse_mumimo),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_get_sampled_iq_data),
    ESP_ELFSYM_EXPORT(npl_freertos_mutex_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_env_deinit_for_sm),
    ESP_ELFSYM_EXPORT(nvs_get_blob),
    ESP_ELFSYM_EXPORT(psa_save_persistent_key),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_databuf_overflow),
    ESP_ELFSYM_EXPORT(ble_gap_conn_broken),
    ESP_ELFSYM_EXPORT(atol),
    ESP_ELFSYM_EXPORT(esp_wifi_clear_default_wifi_driver_and_handlers),
    ESP_ELFSYM_EXPORT(r_ble_lll_init_pre_process),
    ESP_ELFSYM_EXPORT(lv_spangroup_refresh),
    ESP_ELFSYM_EXPORT(console_access),
    ESP_ELFSYM_EXPORT(esp_crypto_sha_aes_lock_release),
    ESP_ELFSYM_EXPORT(phy_set_mac_data),
    ESP_ELFSYM_EXPORT(adv_stack_initEnv),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_set_new_scan_rsp_data),
    ESP_ELFSYM_EXPORT(lv_cache_drop_all),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_info_params_cmd_proc),
    ESP_ELFSYM_EXPORT(ble_hci_trans_env_p),
    ESP_ELFSYM_EXPORT(r_hal_timer_started),
    ESP_ELFSYM_EXPORT(usb_midi_device_set_name),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolve_set_priv_mode),
    ESP_ELFSYM_EXPORT(__udivdi3),
    ESP_ELFSYM_EXPORT(pmksa_cache_auth_init),
    ESP_ELFSYM_EXPORT(lv_draw_add_task),
    ESP_ELFSYM_EXPORT(etharp_query),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_profile_suiteb),
    ESP_ELFSYM_EXPORT(bundle_has_string),
    ESP_ELFSYM_EXPORT(r_ble_lll_df_ring_buffer_init),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_erase_sector),
    ESP_ELFSYM_EXPORT(ble_hs_id_reset),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_mode_disable),
    ESP_ELFSYM_EXPORT(lv_spinbox_set_digit_format),
    ESP_ELFSYM_EXPORT(sae_ecc_prime_len_2_hash_len),
    ESP_ELFSYM_EXPORT(cnx_sta_associated),
    ESP_ELFSYM_EXPORT(RTC_TYPE),
    ESP_ELFSYM_EXPORT(lv_spinbox_set_step),
    ESP_ELFSYM_EXPORT(gpio_iomux_output),
    ESP_ELFSYM_EXPORT(esp_timer_create),
    ESP_ELFSYM_EXPORT(wDev_record_ftm_data),
    ESP_ELFSYM_EXPORT(ble_gap_preempt_done),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_addr_check_in_table),
    ESP_ELFSYM_EXPORT(mt_init),
    ESP_ELFSYM_EXPORT(__retarget_lock_close_recursive),
    ESP_ELFSYM_EXPORT(preferences_put_bool),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_hci_set_random_addr),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_reset),
    ESP_ELFSYM_EXPORT(mbedtls_ecc_group_from_psa),
    ESP_ELFSYM_EXPORT(_ZTVN3nvs15NVSHandleSimpleE),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_io_tx_param),
    ESP_ELFSYM_EXPORT(aes_128_cbc_encrypt),
    ESP_ELFSYM_EXPORT(sae_parse_commit),
    ESP_ELFSYM_EXPORT(hmac_sha256),
    ESP_ELFSYM_EXPORT(camera_open),
    ESP_ELFSYM_EXPORT(_ZdlPv),
    ESP_ELFSYM_EXPORT(audio_codec_get_input_gain_multiplier),
    ESP_ELFSYM_EXPORT(ble_hs_stop),
    ESP_ELFSYM_EXPORT(lv_display_get_driver_data),
    ESP_ELFSYM_EXPORT(phy_chip_i2c_readReg_org),
    ESP_ELFSYM_EXPORT(ieee80211_add_ie_vendor_esp_manufacturer),
    ESP_ELFSYM_EXPORT(http_parser_url_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_coex_dpc_pti_get),
    ESP_ELFSYM_EXPORT(_strerror_r),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_buffer_owner_set),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_channel_class_reporting_make),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_ciphersuite_uses_ec),
    ESP_ELFSYM_EXPORT(npl_freertos_time_ticks_to_ms),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_alloc_and_copy_txbuf),
    ESP_ELFSYM_EXPORT(coex_schm_bt_page_wifi_connecting),
    ESP_ELFSYM_EXPORT(esp_panic_handler_enable_rtc_wdt),
    ESP_ELFSYM_EXPORT(g_in_blacklist_flag),
    ESP_ELFSYM_EXPORT(httpd_req_recv),
    ESP_ELFSYM_EXPORT(__d_vfscanf),
    ESP_ELFSYM_EXPORT(cipher_type_map_supp_to_public),
    ESP_ELFSYM_EXPORT(f_rename),
    ESP_ELFSYM_EXPORT(esp_log_level_get_timeout),
    ESP_ELFSYM_EXPORT(rc_enable_trc),
    ESP_ELFSYM_EXPORT(vPortClearInterruptMaskFromISR),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_parse_certificate),
    ESP_ELFSYM_EXPORT(lv_obj_get_y2),
    ESP_ELFSYM_EXPORT(lv_obj_event_base),
    ESP_ELFSYM_EXPORT(app_manager_install_path_scan),
    ESP_ELFSYM_EXPORT(ble_ll_conn_hci_subrate_req),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_cert_data),
    ESP_ELFSYM_EXPORT(lv_textarea_set_password_mode),
    ESP_ELFSYM_EXPORT(device_get),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_buf_alloc),
    ESP_ELFSYM_EXPORT(esp_log_writev),
    ESP_ELFSYM_EXPORT(i2c_periph_signal),
    ESP_ELFSYM_EXPORT(ieee80211_gpsq_init),
    ESP_ELFSYM_EXPORT(ieee80211_better_rsn_pairwise_cipher),
    ESP_ELFSYM_EXPORT(esp_wifi_set_ap_key_internal),
    ESP_ELFSYM_EXPORT(_ZThn8_NSdD1Ev),
    ESP_ELFSYM_EXPORT(esp_timer_stop),
    ESP_ELFSYM_EXPORT(i2s_controller_write),
    ESP_ELFSYM_EXPORT(lv_obj_style_apply_recolor),
    ESP_ELFSYM_EXPORT(ble_ll_conn_hci_set_default_subrate),
    ESP_ELFSYM_EXPORT(lvgl_port_init),
    ESP_ELFSYM_EXPORT(thread_get_stack_space),
    ESP_ELFSYM_EXPORT(ble_spp_set_conn_handle),
    ESP_ELFSYM_EXPORT(ble_store_config_init),
    ESP_ELFSYM_EXPORT(_ZTVSt15_Sp_counted_ptrIPN2tt7service9webserver16WebServerServiceELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(esp_netif_is_netif_up),
    ESP_ELFSYM_EXPORT(_fstat_r),
    ESP_ELFSYM_EXPORT(module_stop),
    ESP_ELFSYM_EXPORT(r_ble_phy_chan_to_freq),
    ESP_ELFSYM_EXPORT(ap_sa_query_timeout_process),
    ESP_ELFSYM_EXPORT(rtc_clk_slow_freq_get_hz),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_init),
    ESP_ELFSYM_EXPORT(sae_clear_temp_data),
    ESP_ELFSYM_EXPORT(ble_svc_gatt_init),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt11__timepunctIcEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(_Z21ble_start_advertisingP6DevicePK13ble_uuid128_t),
    ESP_ELFSYM_EXPORT(esp_netif_attach_wifi_ap),
    ESP_ELFSYM_EXPORT(esp_wifi_get_if_mac),
    ESP_ELFSYM_EXPORT(lv_spinbox_set_range),
    ESP_ELFSYM_EXPORT(gpio_backlight_driver),
    ESP_ELFSYM_EXPORT(wpa_gen_rsnxe),
    ESP_ELFSYM_EXPORT(lmacProcessCollision),
    ESP_ELFSYM_EXPORT(rtc_set_time),
    ESP_ELFSYM_EXPORT(ppRxProtoProc),
    ESP_ELFSYM_EXPORT(he_twt_setup_event_post),
    ESP_ELFSYM_EXPORT(wl_size),
    ESP_ELFSYM_EXPORT(spi_device_acquire_bus),
    ESP_ELFSYM_EXPORT(lv_layer_sys),
    ESP_ELFSYM_EXPORT(lv_draw_dispatch),
    ESP_ELFSYM_EXPORT(mac_tx_set_len),
    ESP_ELFSYM_EXPORT(esp_clk_slowclk_cal_get),
    ESP_ELFSYM_EXPORT(lv_label_get_text),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_env_deinit),
    ESP_ELFSYM_EXPORT(__letf2),
    ESP_ELFSYM_EXPORT(lv_tileview_class),
    ESP_ELFSYM_EXPORT(mbedtls_pk_ecc_set_key),
    ESP_ELFSYM_EXPORT(lv_mem_init),
    ESP_ELFSYM_EXPORT(ble_hs_conn_deinit),
    ESP_ELFSYM_EXPORT(wifi_osi_funcs_register),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_update_aux_data),
    ESP_ELFSYM_EXPORT(esp_vfs_write),
    ESP_ELFSYM_EXPORT(create_new_bss_for_sa_query_failed_sta),
    ESP_ELFSYM_EXPORT(lv_timer_reset),
    ESP_ELFSYM_EXPORT(esp_efuse_rtc_calib_get_ver),
    ESP_ELFSYM_EXPORT(coex_schm_bt_inq_wifi_conn),
    ESP_ELFSYM_EXPORT(lv_refr_init),
    ESP_ELFSYM_EXPORT(lv_display_set_draw_buffers),
    ESP_ELFSYM_EXPORT(r_ble_ll_max_byte_get),
    ESP_ELFSYM_EXPORT(coex_schm_init),
    ESP_ELFSYM_EXPORT(MODEM_CLOCK_instance),
    ESP_ELFSYM_EXPORT(vPortTCBPreDeleteHook),
    ESP_ELFSYM_EXPORT(efuse_hal_flash_encryption_enabled),
    ESP_ELFSYM_EXPORT(mbedtls_psa_key_agreement_ecdh),
    ESP_ELFSYM_EXPORT(dispatcher_dispatch_timed),
    ESP_ELFSYM_EXPORT(g_coex_adapter_funcs),
    ESP_ELFSYM_EXPORT(bt_bb_v2_init_cmplx),
    ESP_ELFSYM_EXPORT(esp_io_expander_gpio_wrapper_set_level),
    ESP_ELFSYM_EXPORT(_ZTVSo),
    ESP_ELFSYM_EXPORT(thread_set_priority),
    ESP_ELFSYM_EXPORT(lv_style_set_pad_left),
    ESP_ELFSYM_EXPORT(rcReachRetryLimit),
    ESP_ELFSYM_EXPORT(esp_flash_init_main),
    ESP_ELFSYM_EXPORT(httpd_sess_free_ctx),
    ESP_ELFSYM_EXPORT(lvgl_trackball_add),
    ESP_ELFSYM_EXPORT(pm_on_channel),
    ESP_ELFSYM_EXPORT(lv_obj_get_event_dsc),
    ESP_ELFSYM_EXPORT(pm_check_state),
    ESP_ELFSYM_EXPORT(wpabuf_zeropad),
    ESP_ELFSYM_EXPORT(wpa_sm_notify_assoc),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_setkey),
    ESP_ELFSYM_EXPORT(lv_group_get_focused),
    ESP_ELFSYM_EXPORT(lv_cubic_bezier),
    ESP_ELFSYM_EXPORT(regdma_link_destroy),
    ESP_ELFSYM_EXPORT(adc_set_hw_calibration_code),
    ESP_ELFSYM_EXPORT(ieee80211_crypto_decap),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_send_conn_comp_ev),
    ESP_ELFSYM_EXPORT(sae_prepare_commit_pt),
    ESP_ELFSYM_EXPORT(g_cnxMgr),
    ESP_ELFSYM_EXPORT(pm_disable_disconnected_sleep_delay_timer),
    ESP_ELFSYM_EXPORT(httpd_uri),
    ESP_ELFSYM_EXPORT(sae_prepare_commit),
    ESP_ELFSYM_EXPORT(bluetooth_serial_is_connected),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_create),
    ESP_ELFSYM_EXPORT(esp_psram_impl_get_physical_size),
    ESP_ELFSYM_EXPORT(esp_bt_controller_deinit),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sync_calculate),
    ESP_ELFSYM_EXPORT(ieee80211_add_ie_vendor_esp_head),
    ESP_ELFSYM_EXPORT(sync_stack_initEnv),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_read_type),
    ESP_ELFSYM_EXPORT(hal_now),
    ESP_ELFSYM_EXPORT(psa_driver_wrapper_get_key_buffer_size),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_ptr_check),
    ESP_ELFSYM_EXPORT(g_ble_ll_resolve_hdr),
    ESP_ELFSYM_EXPORT(wDev_IndicateAmpdu),
    ESP_ELFSYM_EXPORT(pm_twt_keep_alive_timeout),
    ESP_ELFSYM_EXPORT(esp_http_client_write),
    ESP_ELFSYM_EXPORT(r_ble_scan_advertise_filter_env_deinit),
    ESP_ELFSYM_EXPORT(ble_random_enable_isr),
    ESP_ELFSYM_EXPORT(tc_cmac_final),
    ESP_ELFSYM_EXPORT(r_hal_timer_stop),
    ESP_ELFSYM_EXPORT(esp_wifi_get_mode),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_set_default_irk),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_state_set),
    ESP_ELFSYM_EXPORT(lv_image_decoder_post_process),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_en),
    ESP_ELFSYM_EXPORT(lv_event_mark_deleted),
    ESP_ELFSYM_EXPORT(ppCalSubFrameLength),
    ESP_ELFSYM_EXPORT(register_chipv7_phy_init_param),
    ESP_ELFSYM_EXPORT(sta_twt_null_data_max_retry_times_reached),
    ESP_ELFSYM_EXPORT(sdmmc_io_reset),
    ESP_ELFSYM_EXPORT(_ZSt7__writeIcESt19ostreambuf_iteratorIT_St11char_traitsIS1_EES4_PKS1_i),
    ESP_ELFSYM_EXPORT(device_is_compatible),
    ESP_ELFSYM_EXPORT(lv_event_get_current_target),
    ESP_ELFSYM_EXPORT(bt_rf_coex_pti_dft_rom),
    ESP_ELFSYM_EXPORT(vTaskGetInfo),
    ESP_ELFSYM_EXPORT(cnx_sta_pm),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_point_read_binary),
    ESP_ELFSYM_EXPORT(strncmp),
    ESP_ELFSYM_EXPORT(vPortYieldFromISR),
    ESP_ELFSYM_EXPORT(ieee80211_parse_rsn),
    ESP_ELFSYM_EXPORT(ff_uni2oem),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_est_event_failed),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_abort_length),
    ESP_ELFSYM_EXPORT(__unordtf2),
    ESP_ELFSYM_EXPORT(lv_display_create),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_read),
    ESP_ELFSYM_EXPORT(mbedtls_md_init),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_update_out_pointers),
    ESP_ELFSYM_EXPORT(r_ble_ll_set_default_privacy_mode),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_init),
    ESP_ELFSYM_EXPORT(lv_obj_style_apply_color_filter),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_pre_process),
    ESP_ELFSYM_EXPORT(ledc_timer_config),
    ESP_ELFSYM_EXPORT(cJSON_GetArraySize),
    ESP_ELFSYM_EXPORT(lv_display_set_rotation),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_alloc_rxbuf),
    ESP_ELFSYM_EXPORT(ble_uuid_u16),
    ESP_ELFSYM_EXPORT(ieee80211_set_tim),
    ESP_ELFSYM_EXPORT(hal_set_beamf_pti),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_size),
    ESP_ELFSYM_EXPORT(ieee80211_mt_key_is_mask),
    ESP_ELFSYM_EXPORT(ff_mutex_create),
    ESP_ELFSYM_EXPORT(f_lseek),
    ESP_ELFSYM_EXPORT(phy_rx_pkdet_num_set),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_mode_config),
    ESP_ELFSYM_EXPORT(lv_style_set_height),
    ESP_ELFSYM_EXPORT(pm_send_probe_stop),
    ESP_ELFSYM_EXPORT(multi_heap_register_impl),
    ESP_ELFSYM_EXPORT(xTimerPendFunctionCall),
    ESP_ELFSYM_EXPORT(utoa),
    ESP_ELFSYM_EXPORT(capture_tcp_transport_error),
    ESP_ELFSYM_EXPORT(_ZSt10__pop_heapIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SI_SI_SL_),
    ESP_ELFSYM_EXPORT(lv_atan2),
    ESP_ELFSYM_EXPORT(power_supply_periph_signal),
    ESP_ELFSYM_EXPORT(xPortcheckValidStackMem),
    ESP_ELFSYM_EXPORT(empty_func_p_u8_u8_float_float_pparams_r_void),
    ESP_ELFSYM_EXPORT(esp_http_client_is_complete_data_received),
    ESP_ELFSYM_EXPORT(lvgl_keyboard_disable),
    ESP_ELFSYM_EXPORT(npl_freertos_mutex_init),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_broadcast_wakeup_disable),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_transform_scale_x),
    ESP_ELFSYM_EXPORT(g_cnx_probe_rc_list_cb),
    ESP_ELFSYM_EXPORT(r_filter_aux_duplicate_hash_check_in_table),
    ESP_ELFSYM_EXPORT(ieee80211_he_node_cleanup),
    ESP_ELFSYM_EXPORT(ic_clear_interrupt_handler),
    ESP_ELFSYM_EXPORT(_ZTVSt10moneypunctIcLb0EE),
    ESP_ELFSYM_EXPORT(eloop_init),
    ESP_ELFSYM_EXPORT(nan_send_sync_bcn_process),
    ESP_ELFSYM_EXPORT(lora_get_parameter),
    ESP_ELFSYM_EXPORT(i2c_master_write_to_device),
    ESP_ELFSYM_EXPORT(rcAmpduLowerRate),
    ESP_ELFSYM_EXPORT(phy_i2c_exit_critical),
    ESP_ELFSYM_EXPORT(phy_rfrx_rxdc_cal_new),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_deinit),
    ESP_ELFSYM_EXPORT(os_memblock_put_from_cb),
    ESP_ELFSYM_EXPORT(wpa_supplicant_stop_countermeasures),
    ESP_ELFSYM_EXPORT(ble_stack_enableVsEvents),
    ESP_ELFSYM_EXPORT(systimer_hal_init),
    ESP_ELFSYM_EXPORT(sta_support_auto_mode_connect_passive_channel_hidden_ap),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_aux_pdu_make),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_direct_allocate_set),
    ESP_ELFSYM_EXPORT(lv_color_over32),
    ESP_ELFSYM_EXPORT(wpa_parse_wpa_ie),
    ESP_ELFSYM_EXPORT(vRingbufferDeleteWithCaps),
    ESP_ELFSYM_EXPORT(r_DB_addr_list_reset),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_unicast_wakeup_enable),
    ESP_ELFSYM_EXPORT(lv_group_deinit),
    ESP_ELFSYM_EXPORT(psa_cipher_finish),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_mtu),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_trans_ctrl_pdu),
    ESP_ELFSYM_EXPORT(lv_group_init),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_image_to_rgb888),
    ESP_ELFSYM_EXPORT(lv_group_focus_next),
    ESP_ELFSYM_EXPORT(lv_delay_ms),
    ESP_ELFSYM_EXPORT(modem_clock_module_disable),
    ESP_ELFSYM_EXPORT(lv_label_cut_text),
    ESP_ELFSYM_EXPORT(wpa_remove_ptk),
    ESP_ELFSYM_EXPORT(tcp_process_refused_data),
    ESP_ELFSYM_EXPORT(spi_flash_chip_gd_set_io_mode),
    ESP_ELFSYM_EXPORT(ble_gatts_clt_cfg_access),
    ESP_ELFSYM_EXPORT(pm_dream),
    ESP_ELFSYM_EXPORT(r_os_mbuf_prepend),
    ESP_ELFSYM_EXPORT(mbedtls_oid_get_md_hmac),
    ESP_ELFSYM_EXPORT(lwip_close),
    ESP_ELFSYM_EXPORT(pmu_lp_system_power_param_default),
    ESP_ELFSYM_EXPORT(__negdf2),
    ESP_ELFSYM_EXPORT(tc_cmac_setup),
    ESP_ELFSYM_EXPORT(udp_init),
    ESP_ELFSYM_EXPORT(xRingbufferReceiveUpToFromISR),
    ESP_ELFSYM_EXPORT(_ZZN2tt7service9statusbar16StatusbarService13updateUsbIconEvENUlP10FileSystemPvE_4_FUNES4_S5_),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_get_first_item),
    ESP_ELFSYM_EXPORT(g_ble_single_env),
    ESP_ELFSYM_EXPORT(is_lmac_idle),
    ESP_ELFSYM_EXPORT(lv_strncpy),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_AVE_INITCODE_ATTEN1),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_pref_postion_get),
    ESP_ELFSYM_EXPORT(phy_set_rx_gain_cal_iq),
    ESP_ELFSYM_EXPORT(rcUpdateTxDone),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY_PURPOSE_5),
    ESP_ELFSYM_EXPORT(r_filter_exception_check_is_mesh_packet),
    ESP_ELFSYM_EXPORT(phy_ant_wifirx_cfg),
    ESP_ELFSYM_EXPORT(phy_en_hw_set_freq),
    ESP_ELFSYM_EXPORT(ff_diskio_get_pdrv_card),
    ESP_ELFSYM_EXPORT(phy_tx_cap_init),
    ESP_ELFSYM_EXPORT(ic_get_espnow_rate),
    ESP_ELFSYM_EXPORT(esp_http_client_get_and_clear_last_tls_error),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_height),
    ESP_ELFSYM_EXPORT(i2s_channel_write),
    ESP_ELFSYM_EXPORT(crypto_bignum_mod),
    ESP_ELFSYM_EXPORT(__month_lengths),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_finish),
    ESP_ELFSYM_EXPORT(ble_gap_adv_rsp_set_data),
    ESP_ELFSYM_EXPORT(lmacProcessModemStateRxBeacon),
    ESP_ELFSYM_EXPORT(test_rx_parse_nonmumimo),
    ESP_ELFSYM_EXPORT(sae_ffc_prime_len_2_hash_len),
    ESP_ELFSYM_EXPORT(cnx_bss_alloc),
    ESP_ELFSYM_EXPORT(esp_cache_err_get_panic_info),
    ESP_ELFSYM_EXPORT(_ZTVSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE),
    ESP_ELFSYM_EXPORT(_ZTVSt23__codecvt_abstract_baseIDic10_mbstate_tE),
    ESP_ELFSYM_EXPORT(rssi_saved),
    ESP_ELFSYM_EXPORT(spi_flash_common_write_status_8b_wrsr2),
    ESP_ELFSYM_EXPORT(esprv_int_set_vectored),
    ESP_ELFSYM_EXPORT(dbg_hal_check_set_mplen_bitmap),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_remaining_pri_after),
    ESP_ELFSYM_EXPORT(tcp_netif_ip_addr_changed),
    ESP_ELFSYM_EXPORT(hmac_sha1_vector),
    ESP_ELFSYM_EXPORT(esp_int_wdt_cpu_init),
    ESP_ELFSYM_EXPORT(r_ble_phy_ramup_time_set),
    ESP_ELFSYM_EXPORT(httpd_unregister_all_uri_handlers),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx118messagesIwEE),
    ESP_ELFSYM_EXPORT(bandmode2str),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_reschedule_event),
    ESP_ELFSYM_EXPORT(mbedtls_aes_init),
    ESP_ELFSYM_EXPORT(rgb_led_disable),
    ESP_ELFSYM_EXPORT(esp_crypto_mpi_enable_periph_clk),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_remove_and_restore),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_parse_cn_inet_pton),
    ESP_ELFSYM_EXPORT(esp_create_mbedtls_handle),
    ESP_ELFSYM_EXPORT(lv_display_set_render_mode),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_add_scan_rsp_adv),
    ESP_ELFSYM_EXPORT(memspi_host_program_page),
    ESP_ELFSYM_EXPORT(cnx_check_bssid_in_blacklist),
    ESP_ELFSYM_EXPORT(_ZTVSt7codecvtIDsc10_mbstate_tE),
    ESP_ELFSYM_EXPORT(sta_is_wpa3_enabled),
    ESP_ELFSYM_EXPORT(lv_color_luminance),
    ESP_ELFSYM_EXPORT(_ZTVSt8numpunctIwE),
    ESP_ELFSYM_EXPORT(__lesf2),
    ESP_ELFSYM_EXPORT(spi_flash_hal_erase_block),
    ESP_ELFSYM_EXPORT(ieee80211_amsdu_adjust_head),
    ESP_ELFSYM_EXPORT(gpio_output_enable),
    ESP_ELFSYM_EXPORT(esp_netif_set_old_ip_info),
    ESP_ELFSYM_EXPORT(lv_rb_drop_node),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_transform_rotation),
    ESP_ELFSYM_EXPORT(httpd_default_recv),
    ESP_ELFSYM_EXPORT(g_mt_version),
    ESP_ELFSYM_EXPORT(ieee80211_add_probe_resp_app_ies),
    ESP_ELFSYM_EXPORT(_ZSt11make_uniqueIA_cENSt8__detail9_MakeUniqIT_E7__arrayEj),
    ESP_ELFSYM_EXPORT(pmksa_cache_init),
    ESP_ELFSYM_EXPORT(r_exception_list_need_report_for_mesh),
    ESP_ELFSYM_EXPORT(bootloader_read_flash_id),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_user_block_check),
    ESP_ELFSYM_EXPORT(adc_oneshot_del_unit),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ecp_import_key),
    ESP_ELFSYM_EXPORT(esp_nimble_deinit),
    ESP_ELFSYM_EXPORT(ble_store_read_peer_sec),
    ESP_ELFSYM_EXPORT(os_memblock_from),
    ESP_ELFSYM_EXPORT(iswlower),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_soc_wakeup_disable),
    ESP_ELFSYM_EXPORT(get_wpa_sm),
    ESP_ELFSYM_EXPORT(module_resolve_symbol),
    ESP_ELFSYM_EXPORT(ble_hs_enqueue_hci_event),
    ESP_ELFSYM_EXPORT(netconn_err),
    ESP_ELFSYM_EXPORT(g_allowed_groups),
    ESP_ELFSYM_EXPORT(tt_symbol_resolver),
    ESP_ELFSYM_EXPORT(esp_cpu_configure_region_protection),
    ESP_ELFSYM_EXPORT(ble_sm_ioact_state),
    ESP_ELFSYM_EXPORT(phy_dpd_rxdc_cal),
    ESP_ELFSYM_EXPORT(lv_image_header_cache_is_enabled),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_send_csd),
    ESP_ELFSYM_EXPORT(_read_r),
    ESP_ELFSYM_EXPORT(ble_hs_conn_chan_find_by_scid),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_stop_limit_reached),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_conn),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_can_do),
    ESP_ELFSYM_EXPORT(mt_get_version),
    ESP_ELFSYM_EXPORT(hmac_hal_start),
    ESP_ELFSYM_EXPORT(BACKLIGHT_TYPE),
    ESP_ELFSYM_EXPORT(ble_gap_disc_active),
    ESP_ELFSYM_EXPORT(abs),
    ESP_ELFSYM_EXPORT(lv_canvas_set_px),
    ESP_ELFSYM_EXPORT(pm_beacon_offset_add_loss_counter),
    ESP_ELFSYM_EXPORT(ppTxqEmpty),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_recycling_is_out_of_order),
    ESP_ELFSYM_EXPORT(uart_controller_open),
    ESP_ELFSYM_EXPORT(xQueueGenericCreateStatic),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_cth_flow_free_credit),
    ESP_ELFSYM_EXPORT(spi_flash_hal_device_config),
    ESP_ELFSYM_EXPORT(mspi_timing_enter_high_speed_early),
    ESP_ELFSYM_EXPORT(battery_sense_driver),
    ESP_ELFSYM_EXPORT(__unordsf2),
    ESP_ELFSYM_EXPORT(r_ble_ll_update_max_tx_octets_phy_mode),
    ESP_ELFSYM_EXPORT(lmacProcessLongFrameSuccess),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_update_ap_info_internal),
    ESP_ELFSYM_EXPORT(ieee80211_setup_robust_mgmtframe),
    ESP_ELFSYM_EXPORT(i2c_master_probe),
    ESP_ELFSYM_EXPORT(esp_libc_include_strcmp_impl),
    ESP_ELFSYM_EXPORT(KERNEL_SYMBOLS),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_ecp_group_id_from_tls_id),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_write),
    ESP_ELFSYM_EXPORT(_ZSt16__throw_bad_castv),
    ESP_ELFSYM_EXPORT(display_mirror),
    ESP_ELFSYM_EXPORT(hal_agreement_add_rx_ba),
    ESP_ELFSYM_EXPORT(spi_flash_encryption_hal_destroy),
    ESP_ELFSYM_EXPORT(inet_chksum_pseudo),
    ESP_ELFSYM_EXPORT(_ZTVSt7codecvtIDsDu10_mbstate_tE),
    ESP_ELFSYM_EXPORT(esp_app_get_elf_sha256),
    ESP_ELFSYM_EXPORT(xQueueGiveFromISR),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sm_init),
    ESP_ELFSYM_EXPORT(lv_display_is_double_buffered),
    ESP_ELFSYM_EXPORT(dhcps_start),
    ESP_ELFSYM_EXPORT(xQueueCreateCountingSemaphore),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_aux_scannable_pdu_payload_len),
    ESP_ELFSYM_EXPORT(esp_ptr_external_ram),
    ESP_ELFSYM_EXPORT(r_ble_hw_driver_env_deinit),
    ESP_ELFSYM_EXPORT(lv_style_set_rotary_sensitivity),
    ESP_ELFSYM_EXPORT(tan),
    ESP_ELFSYM_EXPORT(lora_add_state_callback),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ctlr_bb_cmd_proc),
    ESP_ELFSYM_EXPORT(crypto_bignum_inverse),
    ESP_ELFSYM_EXPORT(esp_rom_cvt),
    ESP_ELFSYM_EXPORT(ble_gap_disc),
    ESP_ELFSYM_EXPORT(ieee80211_getbcnframe),
    ESP_ELFSYM_EXPORT(wifi_get_firmware_ops),
    ESP_ELFSYM_EXPORT(mbedtls_ms_time),
    ESP_ELFSYM_EXPORT(heap_caps_realloc),
    ESP_ELFSYM_EXPORT(lv_image_decoder_open),
    ESP_ELFSYM_EXPORT(uxTaskGetTaskNumber),
    ESP_ELFSYM_EXPORT(rgb_led_enable),
    ESP_ELFSYM_EXPORT(phytype2str),
    ESP_ELFSYM_EXPORT(esp_hw_stack_guard_get_fired_cpu),
    ESP_ELFSYM_EXPORT(power_supply_is_allowed_to_charge),
    ESP_ELFSYM_EXPORT(mbedtls_ecc_group_to_psa),
    ESP_ELFSYM_EXPORT(r_ble_lll_set_rxbuf_default_value),
    ESP_ELFSYM_EXPORT(apply_z),
    ESP_ELFSYM_EXPORT(__pow5bits),
    ESP_ELFSYM_EXPORT(_ZTTNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(esp_sha256_driver_update),
    ESP_ELFSYM_EXPORT(f_mkdir),
    ESP_ELFSYM_EXPORT(lv_calendar_get_showed_date),
    ESP_ELFSYM_EXPORT(phy_get_data_sat),
    ESP_ELFSYM_EXPORT(spi_bus_remove_device),
    ESP_ELFSYM_EXPORT(sdmmc_wait_for_idle),
    ESP_ELFSYM_EXPORT(coex_schm_bt_page_wifi_conn),
    ESP_ELFSYM_EXPORT(phy_bb_cbw_chan_cfg),
    ESP_ELFSYM_EXPORT(httpd_sess_process),
    ESP_ELFSYM_EXPORT(_ZTVSt21__ctype_abstract_baseIwE),
    ESP_ELFSYM_EXPORT(mbedtls_hmac_drbg_random),
    ESP_ELFSYM_EXPORT(lvgl_get_shared_icon_font_height),
    ESP_ELFSYM_EXPORT(mac_tx_set_vhtsig),
    ESP_ELFSYM_EXPORT(__crash),
    ESP_ELFSYM_EXPORT(hal_disable_softap_tsf),
    ESP_ELFSYM_EXPORT(phy_get_tm7_new),
    ESP_ELFSYM_EXPORT(_ZTVSt16_Sp_counted_baseILN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_init),
    ESP_ELFSYM_EXPORT(gpio_descriptor_add_callback),
    ESP_ELFSYM_EXPORT(ble_hs_conn_alloc),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_rsassa_pkcs1_v15_sign),
    ESP_ELFSYM_EXPORT(phy_band_change),
    ESP_ELFSYM_EXPORT(sdmmc_init_host_frequency),
    ESP_ELFSYM_EXPORT(lv_obj_get_self_width),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_chk_csm_flags),
    ESP_ELFSYM_EXPORT(ble_svc_gap_device_name),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_conf_ca_chain),
    ESP_ELFSYM_EXPORT(esp_crypto_ds_enable_periph_clk),
    ESP_ELFSYM_EXPORT(mbedtls_psa_asymmetric_encrypt),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_switch_delay_rx_aoa_2us_2m),
    ESP_ELFSYM_EXPORT(pp_register_config_cb),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_montmul_init),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt8numpunctIcEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_all_send_cid),
    ESP_ELFSYM_EXPORT(wmemmove),
    ESP_ELFSYM_EXPORT(phy_rfcal_rxiq),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_start_enc_send),
    ESP_ELFSYM_EXPORT(esp_flash_read_chip_id),
    ESP_ELFSYM_EXPORT(spi_bus_deinit_lock),
    ESP_ELFSYM_EXPORT(lv_obj_get_display),
    ESP_ELFSYM_EXPORT(hostap_ht_operation_update),
    ESP_ELFSYM_EXPORT(phy_get_chan_target_power),
    ESP_ELFSYM_EXPORT(ble_gap_disc_cancel),
    ESP_ELFSYM_EXPORT(ble_store_config_persist_our_secs),
    ESP_ELFSYM_EXPORT(wl_erase_range),
    ESP_ELFSYM_EXPORT(sta_rx_csa),
    ESP_ELFSYM_EXPORT(ledc_channel_config),
    ESP_ELFSYM_EXPORT(vQueueDeleteWithCaps),
    ESP_ELFSYM_EXPORT(ble_restore_peer_sec_nvs),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_read_antenna_info),
    ESP_ELFSYM_EXPORT(esp_crosscore_int_init),
    ESP_ELFSYM_EXPORT(_ZZNSt18__moneypunct_cacheIcLb1EE8_M_cacheERKSt6localeEN11_Scoped_strC1ERKSs),
    ESP_ELFSYM_EXPORT(xTaskGetSchedulerState),
    ESP_ELFSYM_EXPORT(esp_flash_get_physical_size),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_align_pld_cnt_with_rxbuffer),
    ESP_ELFSYM_EXPORT(spi_hal_usr_is_done),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_free),
    ESP_ELFSYM_EXPORT(phy_restart_cal),
    ESP_ELFSYM_EXPORT(lv_indev_set_user_data),
    ESP_ELFSYM_EXPORT(registered_heaps),
    ESP_ELFSYM_EXPORT(ets_timer_disarm),
    ESP_ELFSYM_EXPORT(ble_hs_mbuf_acl_pkt),
    ESP_ELFSYM_EXPORT(mmu_hal_check_valid_ext_vaddr_region),
    ESP_ELFSYM_EXPORT(tbtt_adaptive_setup),
    ESP_ELFSYM_EXPORT(trackball_get_button_pressed),
    ESP_ELFSYM_EXPORT(ieee80211_sta_tx_disconnect_cb),
    ESP_ELFSYM_EXPORT(mbedtls_ct_memcmp),
    ESP_ELFSYM_EXPORT(lv_obj_align_to),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_accepted),
    ESP_ELFSYM_EXPORT(lv_rb_find),
    ESP_ELFSYM_EXPORT(trc_calc_duration),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_mul_int),
    ESP_ELFSYM_EXPORT(gdma_hal_start_with_desc),
    ESP_ELFSYM_EXPORT(wcstoul),
    ESP_ELFSYM_EXPORT(rx11ACRate2AMPDULimit),
    ESP_ELFSYM_EXPORT(ble_gap_preempt_no_lock),
    ESP_ELFSYM_EXPORT(ieee80211_process_bar_info),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_mtu),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_callout_env_deinit),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_read_group_type_complete),
    ESP_ELFSYM_EXPORT(cnx_obss_scan),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_tls_id_from_ecp_group_id),
    ESP_ELFSYM_EXPORT(sdmmc_init_rca),
    ESP_ELFSYM_EXPORT(ble_gatts_notify),
    ESP_ELFSYM_EXPORT(powf),
    ESP_ELFSYM_EXPORT(_getpid_r),
    ESP_ELFSYM_EXPORT(npl_freertos_funcs_deinit),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_send_scr),
    ESP_ELFSYM_EXPORT(pp_hdrsize),
    ESP_ELFSYM_EXPORT(gpio_descriptor_get_native_pin_number),
    ESP_ELFSYM_EXPORT(_Z25app_metadata_copy_boundedPcjRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(dbg_lmac_ps_statis_reset),
    ESP_ELFSYM_EXPORT(lv_indev_get_type),
    ESP_ELFSYM_EXPORT(ble_hs_conn_chan_insert),
    ESP_ELFSYM_EXPORT(scan_remove_bssid),
    ESP_ELFSYM_EXPORT(mld6_tmr),
    ESP_ELFSYM_EXPORT(ic_get_trc),
    ESP_ELFSYM_EXPORT(phy_write_chan_freq),
    ESP_ELFSYM_EXPORT(psa_to_ssl_errors),
    ESP_ELFSYM_EXPORT(lv_color_make),
    ESP_ELFSYM_EXPORT(raw_new_ip_type),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_connless_cte_rx_enable),
    ESP_ELFSYM_EXPORT(audio_stream_set_volume),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_recv),
    ESP_ELFSYM_EXPORT(phy_set_bb_wdg),
    ESP_ELFSYM_EXPORT(ble_att_set_preferred_mtu),
    ESP_ELFSYM_EXPORT(trc_tid_isTxAmpduOperational),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_search),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_event_send),
    ESP_ELFSYM_EXPORT(esp_sha_release_hardware),
    ESP_ELFSYM_EXPORT(ledc_hal_set_duty_int_part),
    ESP_ELFSYM_EXPORT(mac_last_rxbuf_init),
    ESP_ELFSYM_EXPORT(wep_encap),
    ESP_ELFSYM_EXPORT(lv_keyboard_def_event_cb),
    ESP_ELFSYM_EXPORT(__wrap_lv_textarea_create),
    ESP_ELFSYM_EXPORT(config_is_cache_tx_buf_enabled),
    ESP_ELFSYM_EXPORT(xRingbufferReceiveUpTo),
    ESP_ELFSYM_EXPORT(gps_generic_module),
    ESP_ELFSYM_EXPORT(rsn_cipher_put_suites),
    ESP_ELFSYM_EXPORT(phy_rx_pkdet_dc_cal),
    ESP_ELFSYM_EXPORT(lv_arc_get_knob_offset),
    ESP_ELFSYM_EXPORT(ble_gap_terminate_with_conn),
    ESP_ELFSYM_EXPORT(__gttf2),
    ESP_ELFSYM_EXPORT(ble_gap_mtu_event),
    ESP_ELFSYM_EXPORT(esp_transport_list_destroy),
    ESP_ELFSYM_EXPORT(uECC_vli_bytesToNative),
    ESP_ELFSYM_EXPORT(lv_arc_set_rotation),
    ESP_ELFSYM_EXPORT(esp_tls_get_and_clear_error_type),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_d),
    ESP_ELFSYM_EXPORT(r_ble_ll_chk_txrx_time),
    ESP_ELFSYM_EXPORT(ieee80211_send_action_vendor_spec),
    ESP_ELFSYM_EXPORT(hal_tsf_timer_set_target),
    ESP_ELFSYM_EXPORT(rc11AXSchedTbl),
    ESP_ELFSYM_EXPORT(tcp_input),
    ESP_ELFSYM_EXPORT(coex_wifi_request),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_line_space),
    ESP_ELFSYM_EXPORT(lv_display_get_event_dsc),
    ESP_ELFSYM_EXPORT(pm_connectionless_wake_interval_timeout_process),
    ESP_ELFSYM_EXPORT(lv_grid_init),
    ESP_ELFSYM_EXPORT(esp_psram_check_ptr_addr),
    ESP_ELFSYM_EXPORT(esp_fp_get_callers),
    ESP_ELFSYM_EXPORT(ieee80211_add_htcap),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_a2dp_paused_wifi_scan),
    ESP_ELFSYM_EXPORT(tlsf_alloc_overhead),
    ESP_ELFSYM_EXPORT(memspi_host_erase_block),
    ESP_ELFSYM_EXPORT(lv_text_encoded_get_char_id),
    ESP_ELFSYM_EXPORT(pp_coex_tx_request),
    ESP_ELFSYM_EXPORT(lmacSetTxFrame),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_init),
    ESP_ELFSYM_EXPORT(sdmmc_init_select_card),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_rd_rem_used_feat),
    ESP_ELFSYM_EXPORT(ble_gap_reset_state),
    ESP_ELFSYM_EXPORT(udp_new),
    ESP_ELFSYM_EXPORT(app_install),
    ESP_ELFSYM_EXPORT(pointer_set_mirror_x),
    ESP_ELFSYM_EXPORT(ieee80211_ccmp_encrypt),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_grid_cell_y_align),
    ESP_ELFSYM_EXPORT(lv_spangroup_set_align),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_env_init),
    ESP_ELFSYM_EXPORT(pm_funcs_deinit),
    ESP_ELFSYM_EXPORT(ppCheckTxIdle),
    ESP_ELFSYM_EXPORT(nvs_flash_erase_partition),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceIN2tt7service14ServiceContextESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_initiator_start),
    ESP_ELFSYM_EXPORT(mbedtls_pk_rsa_set_pubkey),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt5ctypeIwEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(lwip_select),
    ESP_ELFSYM_EXPORT(ieee80211_pwrsave),
    ESP_ELFSYM_EXPORT(esp_crypto_shared_gdma_start_axi_ahb),
    ESP_ELFSYM_EXPORT(coex_schm_bt_piscan_wifi_connecting),
    ESP_ELFSYM_EXPORT(lv_anim_set_completed_cb),
    ESP_ELFSYM_EXPORT(iswgraph),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_free),
    ESP_ELFSYM_EXPORT(_ZGVNSt10moneypunctIwLb0EE2idE),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_alloc_sch_items),
    ESP_ELFSYM_EXPORT(coex_schm_bt_default_wifi_scan),
    ESP_ELFSYM_EXPORT(esp_test_tx_tb_complete),
    ESP_ELFSYM_EXPORT(huk_hal_get_risk_level),
    ESP_ELFSYM_EXPORT(ble_ll_rand_env_p),
    ESP_ELFSYM_EXPORT(adc_apb_periph_free),
    ESP_ELFSYM_EXPORT(phy_get_index_pwr),
    ESP_ELFSYM_EXPORT(ble_gap_rx_phy_update_complete),
    ESP_ELFSYM_EXPORT(he_twt_information_suspend),
    ESP_ELFSYM_EXPORT(lv_textarea_set_placeholder_text),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_timeout),
    ESP_ELFSYM_EXPORT(ecdsa_hal_set_efuse_key),
    ESP_ELFSYM_EXPORT(regdma_link_new_wait),
    ESP_ELFSYM_EXPORT(lv_dropdown_get_option_index),
    ESP_ELFSYM_EXPORT(esp_log_buffer_hex_internal),
    ESP_ELFSYM_EXPORT(arr_hci_enable),
    ESP_ELFSYM_EXPORT(mbedtls_oid_get_pk_alg),
    ESP_ELFSYM_EXPORT(pthread_mutex_lock),
    ESP_ELFSYM_EXPORT(ble_hs_lock),
    ESP_ELFSYM_EXPORT(hal_he_reset_muedca_timer),
    ESP_ELFSYM_EXPORT(mbedtls_oid_get_cipher_alg),
    ESP_ELFSYM_EXPORT(esp_http_client_cleanup),
    ESP_ELFSYM_EXPORT(wpa_neg_complete),
    ESP_ELFSYM_EXPORT(bluetooth_serial_write),
    ESP_ELFSYM_EXPORT(lv_timer_handler),
    ESP_ELFSYM_EXPORT(esp_system_include_startup_funcs),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_next_pri_event),
    ESP_ELFSYM_EXPORT(lwip_standard_chksum),
    ESP_ELFSYM_EXPORT(r_ble_lll_env_init),
    ESP_ELFSYM_EXPORT(ble_uuid_cmp),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_flags_clear),
    ESP_ELFSYM_EXPORT(_lock_init),
    ESP_ELFSYM_EXPORT(hostap_handle_timer),
    ESP_ELFSYM_EXPORT(set_gauss_coeff_1m),
    ESP_ELFSYM_EXPORT(pm_sleep),
    ESP_ELFSYM_EXPORT(memp_TCP_PCB),
    ESP_ELFSYM_EXPORT(bootloader_flash_reset_chip),
    ESP_ELFSYM_EXPORT(wDev_DiscardFrame),
    ESP_ELFSYM_EXPORT(i2s_channel_disable),
    ESP_ELFSYM_EXPORT(__getf2),
    ESP_ELFSYM_EXPORT(wpa_sm_set_state),
    ESP_ELFSYM_EXPORT(lv_draw_image_tiled_helper),
    ESP_ELFSYM_EXPORT(r_ble_phy_get_actual_tx_time),
    ESP_ELFSYM_EXPORT(is_esp_mesh_assoc),
    ESP_ELFSYM_EXPORT(opendir),
    ESP_ELFSYM_EXPORT(phy_bt_get_tx_tab_new),
    ESP_ELFSYM_EXPORT(mld6_leavegroup_netif),
    ESP_ELFSYM_EXPORT(trc_set_per_pkt_rate),
    ESP_ELFSYM_EXPORT(pm_process_tim),
    ESP_ELFSYM_EXPORT(memspi_host_erase_chip),
    ESP_ELFSYM_EXPORT(esp_wifi_deauthenticate_internal),
    ESP_ELFSYM_EXPORT(heap_caps_match),
    ESP_ELFSYM_EXPORT(_ZTVSt18__moneypunct_cacheIwLb1EE),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_enabled),
    ESP_ELFSYM_EXPORT(_ZTTSt14basic_ofstreamIcSt11char_traitsIcEE),
    ESP_ELFSYM_EXPORT(vTimerSetTimerNumber),
    ESP_ELFSYM_EXPORT(ble_gatts_register_svcs),
    ESP_ELFSYM_EXPORT(mbedtls_psa_get_random),
    ESP_ELFSYM_EXPORT(ieee80211_alloc_action_vendor_spec),
    ESP_ELFSYM_EXPORT(wl_is_ap_no_lr),
    ESP_ELFSYM_EXPORT(ieee80211_decap),
    ESP_ELFSYM_EXPORT(wifi_scan_start_process),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_read_group_type),
    ESP_ELFSYM_EXPORT(mbedtls_pk_verify_ext),
    ESP_ELFSYM_EXPORT(ppCalFrameTimes),
    ESP_ELFSYM_EXPORT(mbedtls_pk_info_from_type),
    ESP_ELFSYM_EXPORT(TmpSTAAPCloseAP),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_send_key),
    ESP_ELFSYM_EXPORT(lvgl_ppa_is_supported),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_border_opa),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_getaddr),
    ESP_ELFSYM_EXPORT(uxListRemove),
    ESP_ELFSYM_EXPORT(coex_schm_bt_conn_wifi_conn),
    ESP_ELFSYM_EXPORT(__ltsf2),
    ESP_ELFSYM_EXPORT(_Z16esp_err_to_errori),
    ESP_ELFSYM_EXPORT(wifi_scan),
    ESP_ELFSYM_EXPORT(phy_pwdet_reg_init),
    ESP_ELFSYM_EXPORT(sta_con_timer),
    ESP_ELFSYM_EXPORT(ble_freertos_mutex_pool),
    ESP_ELFSYM_EXPORT(gpio_sleep_set_pull_mode),
    ESP_ELFSYM_EXPORT(cJSON_GetStringValue),
    ESP_ELFSYM_EXPORT(ht_action_output),
    ESP_ELFSYM_EXPORT(bundle_put_bool),
    ESP_ELFSYM_EXPORT(nvs_commit),
    ESP_ELFSYM_EXPORT(nd6_input),
    ESP_ELFSYM_EXPORT(spi_bus_add_device),
    ESP_ELFSYM_EXPORT(pbuf_header_force),
    ESP_ELFSYM_EXPORT(flash_mmap_lock_init),
    ESP_ELFSYM_EXPORT(pmu_hp_system_clock_param_default),
    ESP_ELFSYM_EXPORT(_Z29ble_resolve_next_unnamed_peerP6Devicej),
    ESP_ELFSYM_EXPORT(ble_hs_flow_deinit),
    ESP_ELFSYM_EXPORT(lv_text_get_size),
    ESP_ELFSYM_EXPORT(spi_flash_hal_gpspi_read),
    ESP_ELFSYM_EXPORT(r_ble_ll_misc_options_set),
    ESP_ELFSYM_EXPORT(_ZTVSt23__codecvt_abstract_baseIcc10_mbstate_tE),
    ESP_ELFSYM_EXPORT(_ZSt14__add_groupingIcEPT_S1_S0_PKcjPKS0_S5_),
    ESP_ELFSYM_EXPORT(scannum),
    ESP_ELFSYM_EXPORT(lv_obj_add_state),
    ESP_ELFSYM_EXPORT(esp_coex_common_malloc_internal_wrapper),
    ESP_ELFSYM_EXPORT(crypto_bignum_legendre),
    ESP_ELFSYM_EXPORT(hal_mac_rx_set_abort_frames_from_transbss),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_get_timeline_num),
    ESP_ELFSYM_EXPORT(device_add),
    ESP_ELFSYM_EXPORT(_ZTVSt15_Sp_counted_ptrIPN2tt7service9statusbar16StatusbarServiceELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(xPortInterruptedFromISRContext),
    ESP_ELFSYM_EXPORT(spi_flash_encryption_hal_prepare),
    ESP_ELFSYM_EXPORT(acosf),
    ESP_ELFSYM_EXPORT(adc_channel_io_map),
    ESP_ELFSYM_EXPORT(esp_psram_io_get_cs_io),
    ESP_ELFSYM_EXPORT(app_manager_add),
    ESP_ELFSYM_EXPORT(pm_mac_try_enable_modem_state),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_retrieve),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_rx_process),
    ESP_ELFSYM_EXPORT(window_manager_configure),
    ESP_ELFSYM_EXPORT(os_mempool_ext_init),
    ESP_ELFSYM_EXPORT(lv_anim_start),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_send_gamepad),
    ESP_ELFSYM_EXPORT(esp_time_impl_get_time),
    ESP_ELFSYM_EXPORT(ecc_hal_enable_constant_time_point_mul),
    ESP_ELFSYM_EXPORT(stat),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_a2dp_wifi_conn),
    ESP_ELFSYM_EXPORT(tt_timezone_is_format_24_hour),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_aux_data_unref),
    ESP_ELFSYM_EXPORT(tcp_seg_free),
    ESP_ELFSYM_EXPORT(mbedtls_psa_cipher_finish),
    ESP_ELFSYM_EXPORT(lv_list_add_text),
    ESP_ELFSYM_EXPORT(lv_iter_create),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_mla),
    ESP_ELFSYM_EXPORT(bundle_has_int64),
    ESP_ELFSYM_EXPORT(__cxa_atexit),
    ESP_ELFSYM_EXPORT(loader_service_manifest),
    ESP_ELFSYM_EXPORT(lv_image_decoder_add_to_cache),
    ESP_ELFSYM_EXPORT(ieee80211_recv_bar),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_exp_mod_unsafe),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_write_key),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_add_image_button_action),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_del),
    ESP_ELFSYM_EXPORT(spi_device_polling_end),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_datalen_chg),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx118messagesIwE2idE),
    ESP_ELFSYM_EXPORT(__gtsf2),
    ESP_ELFSYM_EXPORT(bt_bb_v2_rx_set),
    ESP_ELFSYM_EXPORT(gpio_controller_deinit_descriptors),
    ESP_ELFSYM_EXPORT(pthread_once),
    ESP_ELFSYM_EXPORT(esp_transport_poll_write),
    ESP_ELFSYM_EXPORT(esp_tls_conn_write),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_grid_cell_x_align),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_group_load),
    ESP_ELFSYM_EXPORT(esp32_adc_oneshot_driver),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_update_buffer_hdr),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx118numpunctIcE2idE),
    ESP_ELFSYM_EXPORT(phy_get_cal_chan),
    ESP_ELFSYM_EXPORT(lv_array_resize),
    ESP_ELFSYM_EXPORT(hostap_deliver_data),
    ESP_ELFSYM_EXPORT(g_heap_usage),
    ESP_ELFSYM_EXPORT(ap_get_sta_internal),
    ESP_ELFSYM_EXPORT(esp_test_clr_rx_error_occurs),
    ESP_ELFSYM_EXPORT(r_ble_ll_init),
    ESP_ELFSYM_EXPORT(mbedtls_sha256_starts),
    ESP_ELFSYM_EXPORT(sqrtf),
    ESP_ELFSYM_EXPORT(vTaskPlaceOnUnorderedEventList),
    ESP_ELFSYM_EXPORT(wdev_process_tsf_timer),
    ESP_ELFSYM_EXPORT(__divsf3),
    ESP_ELFSYM_EXPORT(eap_client_get_eap_state),
    ESP_ELFSYM_EXPORT(dns_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_mbuf_get),
    ESP_ELFSYM_EXPORT(display_get_mirror_y),
    ESP_ELFSYM_EXPORT(empty_func_p_void_r_float),
    ESP_ELFSYM_EXPORT(lv_obj_get_index),
    ESP_ELFSYM_EXPORT(coex_pre_init),
    ESP_ELFSYM_EXPORT(ble_gap_security_initiate),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_reg_netstack_buf_cb_local),
    ESP_ELFSYM_EXPORT(cnx_start_obss_scan),
    ESP_ELFSYM_EXPORT(I2S_CONTROLLER_TYPE),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx118time_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(lv_canvas_fill_bg),
    ESP_ELFSYM_EXPORT(ble_gatts_free_svcs),
    ESP_ELFSYM_EXPORT(tcp_arg),
    ESP_ELFSYM_EXPORT(lv_image_set_pivot),
    ESP_ELFSYM_EXPORT(phy_open_fe_bb_clk),
    ESP_ELFSYM_EXPORT(esp_wifi_register_eapol_txdonecb_internal),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_set_iv),
    ESP_ELFSYM_EXPORT(fcntl),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_ext_set_enable),
    ESP_ELFSYM_EXPORT(spicommon_bus_initialize_io),
    ESP_ELFSYM_EXPORT(sys_arch_mbox_fetch),
    ESP_ELFSYM_EXPORT(httpd_start),
    ESP_ELFSYM_EXPORT(wpa_sta_in_4way_handshake),
    ESP_ELFSYM_EXPORT(lv_group_get_default),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_spiflash_mount),
    ESP_ELFSYM_EXPORT(lv_dropdown_add_option),
    ESP_ELFSYM_EXPORT(nan_transmit_action),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_set_global_chanmap),
    ESP_ELFSYM_EXPORT(bundle_has_bool),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_pad_v15_encode),
    ESP_ELFSYM_EXPORT(ledc_hal_get_duty),
    ESP_ELFSYM_EXPORT(backlight_set_brightness),
    ESP_ELFSYM_EXPORT(camera_release_frame),
    ESP_ELFSYM_EXPORT(vTaskRemoveFromUnorderedEventList),
    ESP_ELFSYM_EXPORT(scan_stack_disable),
    ESP_ELFSYM_EXPORT(g_mmgmt_mem_max_block_size),
    ESP_ELFSYM_EXPORT(ble_store_key_from_value_local_irk),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_phy_mode_to_hci),
    ESP_ELFSYM_EXPORT(lv_draw_buf_flush_cache),
    ESP_ELFSYM_EXPORT(_ZSt18uncaught_exceptionv),
    ESP_ELFSYM_EXPORT(esp_ble_unregister_bb_funcs),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_check_extended_key_usage),
    ESP_ELFSYM_EXPORT(lv_draw_task_get_draw_dsc),
    ESP_ELFSYM_EXPORT(esp_sha1_driver_finish),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand_deinit),
    ESP_ELFSYM_EXPORT(cnx_start_handoff_cb),
    ESP_ELFSYM_EXPORT(spi_flash_common_set_io_mode),
    ESP_ELFSYM_EXPORT(eapol_txcb),
    ESP_ELFSYM_EXPORT(lseek),
    ESP_ELFSYM_EXPORT(esp_netif_set_driver_config),
    ESP_ELFSYM_EXPORT(esp_now_register_recv_cb),
    ESP_ELFSYM_EXPORT(phy_i2c_paral_write_num),
    ESP_ELFSYM_EXPORT(lv_obj_set_size),
    ESP_ELFSYM_EXPORT(vht_max_apep_length),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service13memorychecker20MemoryCheckerServiceE),
    ESP_ELFSYM_EXPORT(__file_str_get),
    ESP_ELFSYM_EXPORT(f_close),
    ESP_ELFSYM_EXPORT(g_rts_threshold_bytes),
    ESP_ELFSYM_EXPORT(wpa_install_key),
    ESP_ELFSYM_EXPORT(wDev_remove_KeyEntry_all_cnx),
    ESP_ELFSYM_EXPORT(lv_malloc_core),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_set_wakeup_timer),
    ESP_ELFSYM_EXPORT(pm_set_beacon_duration),
    ESP_ELFSYM_EXPORT(efuse_hal_read),
    ESP_ELFSYM_EXPORT(ble_hs_hci_set_buf_sz),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_opaque_sign_hash_start),
    ESP_ELFSYM_EXPORT(esp_tls_conn_new_async),
    ESP_ELFSYM_EXPORT(lodepng_chunk_next),
    ESP_ELFSYM_EXPORT(udp_sendto),
    ESP_ELFSYM_EXPORT(isalnum),
    ESP_ELFSYM_EXPORT(mbedtls_net_init),
    ESP_ELFSYM_EXPORT(phy_set_channel_rfpll_freq_new),
    ESP_ELFSYM_EXPORT(cnx_softap_csa),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_our_irk),
    ESP_ELFSYM_EXPORT(phy_pbus_xpd_tx_on),
    ESP_ELFSYM_EXPORT(lv_tick_inc),
    ESP_ELFSYM_EXPORT(lv_obj_style_state_compare),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_psa_curve_info_from_tls_id),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_legacy_pri_chan_pdu_make),
    ESP_ELFSYM_EXPORT(fminf),
    ESP_ELFSYM_EXPORT(phy_rfcal_pwrctrl),
    ESP_ELFSYM_EXPORT(hid_consumer_input_handle),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_pad_bottom),
    ESP_ELFSYM_EXPORT(esp_elf_map_sym),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service9statusbar16StatusbarServiceE),
    ESP_ELFSYM_EXPORT(phy_get_xtal_code),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_receive_enable),
    ESP_ELFSYM_EXPORT(lodepng_state_init),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_RD_DIS_BLOCK_KEY3),
    ESP_ELFSYM_EXPORT(_Z22ble_scan_clear_resultsP6Device),
    ESP_ELFSYM_EXPORT(phy_wifi_11g_rate_chg),
    ESP_ELFSYM_EXPORT(i2s_channel_init_std_mode),
    ESP_ELFSYM_EXPORT(coex_schm_bt_sniff_sco_wifi_conn),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_pkcs1_decrypt),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_enabled),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_peer_cert),
    ESP_ELFSYM_EXPORT(ble_att_set_peer_mtu),
    ESP_ELFSYM_EXPORT(ble_store_util_count),
    ESP_ELFSYM_EXPORT(phy_txpwr_cal_track_new),
    ESP_ELFSYM_EXPORT(esp_supplicant_init),
    ESP_ELFSYM_EXPORT(mac_tx_set_plcp2),
    ESP_ELFSYM_EXPORT(efuse_hal_blk_version),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_delay_according_to_priority),
    ESP_ELFSYM_EXPORT(lv_draw_buf_create_ex),
    ESP_ELFSYM_EXPORT(pm_start),
    ESP_ELFSYM_EXPORT(lv_style_set_arc_color),
    ESP_ELFSYM_EXPORT(nan_faw_end_process),
    ESP_ELFSYM_EXPORT(esp_ecdsa_transparent_verify_hash),
    ESP_ELFSYM_EXPORT(__subsf3),
    ESP_ELFSYM_EXPORT(flash_mmap_lock_unfreeze),
    ESP_ELFSYM_EXPORT(ble_uuid_deinit),
    ESP_ELFSYM_EXPORT(lwip_shutdown),
    ESP_ELFSYM_EXPORT(dispatcher_free),
    ESP_ELFSYM_EXPORT(preferences_has_string),
    ESP_ELFSYM_EXPORT(esp_efuse_mac_get_default),
    ESP_ELFSYM_EXPORT(active_hid_rpt_map_len),
    ESP_ELFSYM_EXPORT(wifi_nvs_compare_cfg_diff),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_register_cfg),
    ESP_ELFSYM_EXPORT(lv_indev_active),
    ESP_ELFSYM_EXPORT(xTaskResumeAll),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_pkcs1_verify),
    ESP_ELFSYM_EXPORT(snprintf),
    ESP_ELFSYM_EXPORT(vPortSetupTimer),
    ESP_ELFSYM_EXPORT(nan_tx_action_frame),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_check_remaining_entries),
    ESP_ELFSYM_EXPORT(wpa_cipher_to_alg),
    ESP_ELFSYM_EXPORT(phy_rf_cal_data_recovery),
    ESP_ELFSYM_EXPORT(thread_alloc_full),
    ESP_ELFSYM_EXPORT(bluetooth_fire_event),
    ESP_ELFSYM_EXPORT(ble_sm_sc_oob_confirm),
    ESP_ELFSYM_EXPORT(ppTxqUpdateBitmap),
    ESP_ELFSYM_EXPORT(ieee80211_vnd_lora_ie_set),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_beacon_miss_limit),
    ESP_ELFSYM_EXPORT(_Z23ble_midi_start_internalP6Device),
    ESP_ELFSYM_EXPORT(psa_cipher_set_iv),
    ESP_ELFSYM_EXPORT(ble_gap_rx_rd_all_remote_feat),
    ESP_ELFSYM_EXPORT(pm_tx_null_data_done_quick_wake_process),
    ESP_ELFSYM_EXPORT(device_for_each_child),
    ESP_ELFSYM_EXPORT(s_fragment),
    ESP_ELFSYM_EXPORT(pm_parse_mbssid_element),
    ESP_ELFSYM_EXPORT(mbedtls_mutex_unlock_ptr),
    ESP_ELFSYM_EXPORT(ble_hs_is_parent_task),
    ESP_ELFSYM_EXPORT(_Z23acquire_pin_or_set_nullRK11GpioPinSpectPP14GpioDescriptor),
    ESP_ELFSYM_EXPORT(app_manager_get_topmost_instance_id),
    ESP_ELFSYM_EXPORT(lv_checkbox_create),
    ESP_ELFSYM_EXPORT(mbedtls_net_send),
    ESP_ELFSYM_EXPORT(ieee80211_phy_mode_show),
    ESP_ELFSYM_EXPORT(bt_bb_set_rx_sense),
    ESP_ELFSYM_EXPORT(huk_hal_recharge_huk_memory),
    ESP_ELFSYM_EXPORT(current_hid_profile),
    ESP_ELFSYM_EXPORT(_ZTSSt9bad_alloc),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_write_int),
    ESP_ELFSYM_EXPORT(spi_controller_unlock),
    ESP_ELFSYM_EXPORT(__cxa_pure_virtual),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_sub_int),
    ESP_ELFSYM_EXPORT(vQueueWaitForMessageRestricted),
    ESP_ELFSYM_EXPORT(sntp_init),
    ESP_ELFSYM_EXPORT(_ZTVN9__gnu_cxx26__concurrence_unlock_errorE),
    ESP_ELFSYM_EXPORT(r_bt_rf_coex_cfg_get_default),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_read_mult_rsp),
    ESP_ELFSYM_EXPORT(ble_hs_adv_parse_fields),
    ESP_ELFSYM_EXPORT(esp_cmac_finish),
    ESP_ELFSYM_EXPORT(r_ble_ll_acl_data_in),
    ESP_ELFSYM_EXPORT(wifi_nvs_cfg_init),
    ESP_ELFSYM_EXPORT(uECC_set_rng),
    ESP_ELFSYM_EXPORT(ic_set_ac_muedca_param),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_cal_path_loss_zone),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_get_tail_of),
    ESP_ELFSYM_EXPORT(panic_print_dec),
    ESP_ELFSYM_EXPORT(phy_read_hw_noisefloor),
    ESP_ELFSYM_EXPORT(_g_esp_netif_inherent_ap_config),
    ESP_ELFSYM_EXPORT(trc_ctl),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_alloc_cte_chain_mem),
    ESP_ELFSYM_EXPORT(pp_timer_do_process),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_set_zero),
    ESP_ELFSYM_EXPORT(hal_he_enable_dump_complete_hesigb),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_read_size),
    ESP_ELFSYM_EXPORT(ble_gatts_indicate_custom),
    ESP_ELFSYM_EXPORT(esp_efuse_check_errors),
    ESP_ELFSYM_EXPORT(rssi_index),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt7codecvtIcc10_mbstate_tEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(ble_ll_adv_env_p),
    ESP_ELFSYM_EXPORT(gpio_descriptor_set_flags),
    ESP_ELFSYM_EXPORT(_ZTVSt7codecvtIcc10_mbstate_tE),
    ESP_ELFSYM_EXPORT(lv_obj_init_draw_rect_dsc),
    ESP_ELFSYM_EXPORT(he_btwt_teardown_post_event),
    ESP_ELFSYM_EXPORT(ieee80211_set_tx_desc),
    ESP_ELFSYM_EXPORT(hal_mac_rate_autoack_init),
    ESP_ELFSYM_EXPORT(wifi_set_promiscuous_callback),
    ESP_ELFSYM_EXPORT(material_symbols_launcher_36),
    ESP_ELFSYM_EXPORT(sha_hal_write_digest),
    ESP_ELFSYM_EXPORT(ic_reset_extra_softap_rx_ba),
    ESP_ELFSYM_EXPORT(minitar_read_header),
    ESP_ELFSYM_EXPORT(r_ble_hw_rng_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_reset),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_reject_ind_send),
    ESP_ELFSYM_EXPORT(phy_wr_freq_mem),
    ESP_ELFSYM_EXPORT(gdma_reset),
    ESP_ELFSYM_EXPORT(phy_freq_num_get_data),
    ESP_ELFSYM_EXPORT(wpa_use_akm_defined),
    ESP_ELFSYM_EXPORT(lv_style_set_bg_opa),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_CH2_ATTEN0_INITCODE_DIFF),
    ESP_ELFSYM_EXPORT(r_ble_hw_whitelist_search),
    ESP_ELFSYM_EXPORT(cnx_update_bss_more),
    ESP_ELFSYM_EXPORT(hal_set_tx_min_pwr),
    ESP_ELFSYM_EXPORT(i2c_cmd_link_delete),
    ESP_ELFSYM_EXPORT(r_ble_ll_event_comp_pkts),
    ESP_ELFSYM_EXPORT(r_os_mbuf_trailingspace),
    ESP_ELFSYM_EXPORT(_ZTV9Partition),
    ESP_ELFSYM_EXPORT(uart_param_config),
    ESP_ELFSYM_EXPORT(__wrap__Unwind_Resume),
    ESP_ELFSYM_EXPORT(uxTaskResetEventItemValue),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_clear_button_ctrl_all),
    ESP_ELFSYM_EXPORT(r_os_mbuf_ctrl_mbuf_cnt_decrease),
    ESP_ELFSYM_EXPORT(lv_textarea_cursor_left),
    ESP_ELFSYM_EXPORT(_ZZNKSt7__cxx117collateIcE12do_transformEPKcS3_EN4_BufD2Ev),
    ESP_ELFSYM_EXPORT(__cxa_get_globals_fast),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_init),
    ESP_ELFSYM_EXPORT(ble_sm_cmd_get),
    ESP_ELFSYM_EXPORT(bluetooth_pair),
    ESP_ELFSYM_EXPORT(phy_en_pwdet),
    ESP_ELFSYM_EXPORT(esp_security_init_include_impl),
    ESP_ELFSYM_EXPORT(he_send_action_event_bsscolor),
    ESP_ELFSYM_EXPORT(midi_chars),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_update_modem_sleep_default_params),
    ESP_ELFSYM_EXPORT(settimeofday),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_validate_ciphersuite),
    ESP_ELFSYM_EXPORT(ble_gattc_write_flat),
    ESP_ELFSYM_EXPORT(lv_calendar_set_today_date),
    ESP_ELFSYM_EXPORT(sha1_prf),
    ESP_ELFSYM_EXPORT(lv_clamp_width),
    ESP_ELFSYM_EXPORT(ppAssembleMicHdr),
    ESP_ELFSYM_EXPORT(lv_chart_set_update_mode),
    ESP_ELFSYM_EXPORT(esp_log_level_set),
    ESP_ELFSYM_EXPORT(_ZGVNSt8numpunctIcE2idE),
    ESP_ELFSYM_EXPORT(pm_on_coex_start),
    ESP_ELFSYM_EXPORT(window_manager_create_ext),
    ESP_ELFSYM_EXPORT(igmp_report_groups),
    ESP_ELFSYM_EXPORT(esp_wifi_get_pmf_config_internal),
    ESP_ELFSYM_EXPORT(coex_schm_process_restart),
    ESP_ELFSYM_EXPORT(sdmmc_write_sectors_dma),
    ESP_ELFSYM_EXPORT(ieee80211_ioctl_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_schedule_aux),
    ESP_ELFSYM_EXPORT(uECC_vli_sub),
    ESP_ELFSYM_EXPORT(i2s_channel_try_to_constitute_duplex),
    ESP_ELFSYM_EXPORT(ble_spp_set_active),
    ESP_ELFSYM_EXPORT(lmac_stop_hw_txq),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_get_ext_adv_report),
    ESP_ELFSYM_EXPORT(lv_fs_get_drv),
    ESP_ELFSYM_EXPORT(modem_clock_module_mac_reset),
    ESP_ELFSYM_EXPORT(lv_list_button_class),
    ESP_ELFSYM_EXPORT(pmksa_cache_deinit),
    ESP_ELFSYM_EXPORT(aes_encrypt_init),
    ESP_ELFSYM_EXPORT(esp_test_enable_rx_statistics),
    ESP_ELFSYM_EXPORT(neighbor_cache),
    ESP_ELFSYM_EXPORT(wifi_register_mgmt_frame),
    ESP_ELFSYM_EXPORT(len_dh_ie),
    ESP_ELFSYM_EXPORT(service_instance_construct),
    ESP_ELFSYM_EXPORT(phy_get_fm_sar_dout),
    ESP_ELFSYM_EXPORT(wpa_eapol_key_mic),
    ESP_ELFSYM_EXPORT(sntp_get_sync_interval),
    ESP_ELFSYM_EXPORT(esp_psram_mspi_mb),
    ESP_ELFSYM_EXPORT(phy_wifi_set_tx_gain_new),
    ESP_ELFSYM_EXPORT(esp_http_client_add_auth),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_block_size_get),
    ESP_ELFSYM_EXPORT(g_ble_phy_mode_pkt_start_off_ro),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_bigendian_to_host),
    ESP_ELFSYM_EXPORT(__wrap___deregister_frame),
    ESP_ELFSYM_EXPORT(ledc_hal_set_range_number),
    ESP_ELFSYM_EXPORT(camera_capture_jpeg),
    ESP_ELFSYM_EXPORT(ppProcessTxQ),
    ESP_ELFSYM_EXPORT(qr_version_binary_sizes),
    ESP_ELFSYM_EXPORT(ieee80211_disassoc_construct),
    ESP_ELFSYM_EXPORT(ic_set_bssid),
    ESP_ELFSYM_EXPORT(gdma_connect),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_default_wifi_connecting),
    ESP_ELFSYM_EXPORT(ieee80211_add_power_cap),
    ESP_ELFSYM_EXPORT(uart_controller_flush_input),
    ESP_ELFSYM_EXPORT(preferences_opt_int64),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_rsaes_pkcs1_v15_decrypt),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_addr_table_add),
    ESP_ELFSYM_EXPORT(pm_get_connectionless_status),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_owe_config_internal),
    ESP_ELFSYM_EXPORT(multi_heap_minimum_free_size_impl),
    ESP_ELFSYM_EXPORT(driver_construct),
    ESP_ELFSYM_EXPORT(lvgl_software_keyboard_destruct),
    ESP_ELFSYM_EXPORT(lv_obj_set_grid_dsc_array),
    ESP_ELFSYM_EXPORT(esp_wifi_deinit_internal),
    ESP_ELFSYM_EXPORT(ieee80211_vnd_ie_set),
    ESP_ELFSYM_EXPORT(empty_func_p_u8_u8_pu16_pfloat_r_void),
    ESP_ELFSYM_EXPORT(pbuf_alloc),
    ESP_ELFSYM_EXPORT(psram_exec_cmd),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_color_to_rgb565),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_set_sched),
    ESP_ELFSYM_EXPORT(ble_hs_hci_set_le_supported_feat),
    ESP_ELFSYM_EXPORT(bootloader_init_mem),
    ESP_ELFSYM_EXPORT(pm_is_sending_keep_alive),
    ESP_ELFSYM_EXPORT(phy_set_pbus_reg),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_bitlen),
    ESP_ELFSYM_EXPORT(_regi2c_impl_write),
    ESP_ELFSYM_EXPORT(read),
    ESP_ELFSYM_EXPORT(esp_ds_finish_sign),
    ESP_ELFSYM_EXPORT(mbedtls_hmac_drbg_init),
    ESP_ELFSYM_EXPORT(f_closedir),
    ESP_ELFSYM_EXPORT(r_os_msys_unregister_block_num_get_cb),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_slave_new),
    ESP_ELFSYM_EXPORT(ieee80211_vht_updatecaps),
    ESP_ELFSYM_EXPORT(xQueueIsQueueFullFromISR),
    ESP_ELFSYM_EXPORT(raw_sendto_if_src),
    ESP_ELFSYM_EXPORT(lv_fs_stdio_init),
    ESP_ELFSYM_EXPORT(_ZTVSt16invalid_argument),
    ESP_ELFSYM_EXPORT(app_manager_start),
    ESP_ELFSYM_EXPORT(_ZZN2tt7service9statusbar16StatusbarService16updateSdCardIconEvENUlP10FileSystemPvE_4_FUNES4_S5_),
    ESP_ELFSYM_EXPORT(mbedtls_mutex_init),
    ESP_ELFSYM_EXPORT(phy_iq_swap_set),
    ESP_ELFSYM_EXPORT(g_wifi_osi_funcs),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_write_tag),
    ESP_ELFSYM_EXPORT(heap_caps_free),
    ESP_ELFSYM_EXPORT(esp_ecc_point_verify),
    ESP_ELFSYM_EXPORT(lv_menu_page_class),
    ESP_ELFSYM_EXPORT(lv_async_call),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_init),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt10moneypunctIwLb0EEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(btwt_setup_dwell_timeout_fn),
    ESP_ELFSYM_EXPORT(lv_font_get_default),
    ESP_ELFSYM_EXPORT(ble_hs_timer_resched),
    ESP_ELFSYM_EXPORT(tcp_recv),
    ESP_ELFSYM_EXPORT(mbedtls_psa_cipher_decrypt),
    ESP_ELFSYM_EXPORT(esp_http_client_set_authtype),
    ESP_ELFSYM_EXPORT(ble_random_mode_init_exec_process_cb_get),
    ESP_ELFSYM_EXPORT(psa_driver_wrapper_export_public_key),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_flex_grow),
    ESP_ELFSYM_EXPORT(esp_transport_utils_ms_to_timeval),
    ESP_ELFSYM_EXPORT(lodepng_encoder_settings_init),
    ESP_ELFSYM_EXPORT(_ZTISt12length_error),
    ESP_ELFSYM_EXPORT(hci_stack_initEnv),
    ESP_ELFSYM_EXPORT(nimble_hid_device_api),
    ESP_ELFSYM_EXPORT(phy_i2c_init1),
    ESP_ELFSYM_EXPORT(gpio_set_level),
    ESP_ELFSYM_EXPORT(psa_format_key_data_for_storage),
    ESP_ELFSYM_EXPORT(lvgl_trackball_remove),
    ESP_ELFSYM_EXPORT(i2s_controller_set_rx_tdm_config),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_exp_mod),
    ESP_ELFSYM_EXPORT(esp_wifi_register_if_rxcb),
    ESP_ELFSYM_EXPORT(wifi_is_started),
    ESP_ELFSYM_EXPORT(_ZZNKSt7collateIcE12do_transformEPKcS2_EN4_BufD1Ev),
    ESP_ELFSYM_EXPORT(mbedtls_pk_get_type),
    ESP_ELFSYM_EXPORT(mld6_leavegroup),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_add_hs_hdr_to_checksum),
    ESP_ELFSYM_EXPORT(phy_bb_bss_cbw40),
    ESP_ELFSYM_EXPORT(esp_event_isr_post_to),
    ESP_ELFSYM_EXPORT(__bufio_flush_locked),
    ESP_ELFSYM_EXPORT(pm_mac_force_disable_modem_state),
    ESP_ELFSYM_EXPORT(hostap_del_mac_info_from_list),
    ESP_ELFSYM_EXPORT(ble_hs_conn_addrs),
    ESP_ELFSYM_EXPORT(lv_menu_clear_history),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_get_selected_button),
    ESP_ELFSYM_EXPORT(bundle_free),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_add_int),
    ESP_ELFSYM_EXPORT(cnx_remove_rc_except),
    ESP_ELFSYM_EXPORT(spi_bus_init_lock),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_pref_itvl_get),
    ESP_ELFSYM_EXPORT(keyboard_is_present),
    ESP_ELFSYM_EXPORT(aes_128_cbc_decrypt),
    ESP_ELFSYM_EXPORT(ble_gatts_notify_custom),
    ESP_ELFSYM_EXPORT(wpa_auth_uses_mfp),
    ESP_ELFSYM_EXPORT(uart_hal_write_txfifo),
    ESP_ELFSYM_EXPORT(esp32_uart_driver),
    ESP_ELFSYM_EXPORT(regi2c_enter_critical),
    ESP_ELFSYM_EXPORT(_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_),
    ESP_ELFSYM_EXPORT(uart_hal_tx_break),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ffdh_import_key),
    ESP_ELFSYM_EXPORT(event_group_get),
    ESP_ELFSYM_EXPORT(hal_agreement_add_extra_softap_rx_ba),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_get_safe_instant_delta),
    ESP_ELFSYM_EXPORT(g_ble_phy_chan_freq_ro),
    ESP_ELFSYM_EXPORT(httpd_resp_send_chunk),
    ESP_ELFSYM_EXPORT(phy_tx_magtrk_init),
    ESP_ELFSYM_EXPORT(console_write),
    ESP_ELFSYM_EXPORT(phy_set_iqcal_ckgen_code),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_single_pkt_mode_get),
    ESP_ELFSYM_EXPORT(islower),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_scheduled),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_reg_netstack_buf_cb),
    ESP_ELFSYM_EXPORT(hal_get_dump_ctrl_frame_cfg),
    ESP_ELFSYM_EXPORT(lv_obj_init_draw_image_dsc),
    ESP_ELFSYM_EXPORT(lv_draw_layer_go_to_xy),
    ESP_ELFSYM_EXPORT(esp_event_loop_create_default),
    ESP_ELFSYM_EXPORT(esp_phy_load_cal_and_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_get_sm),
    ESP_ELFSYM_EXPORT(esp_netif_stop),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_sta_sae_identifier),
    ESP_ELFSYM_EXPORT(ieee80211_ampdu_request),
    ESP_ELFSYM_EXPORT(esp_http_client_close),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_lset),
    ESP_ELFSYM_EXPORT(_ZTVSt9basic_iosIcSt11char_traitsIcEE),
    ESP_ELFSYM_EXPORT(ble_gattc_read),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_flags_set),
    ESP_ELFSYM_EXPORT(ble_att_svr_reset),
    ESP_ELFSYM_EXPORT(esp_mesh_map_deauth),
    ESP_ELFSYM_EXPORT(spi_flash_chip_winbond_suspend_cmd_conf),
    ESP_ELFSYM_EXPORT(psa_mac_abort),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_align),
    ESP_ELFSYM_EXPORT(bsscolor_event_dump),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_find_info_complete),
    ESP_ELFSYM_EXPORT(ip4_route_src_hook),
    ESP_ELFSYM_EXPORT(lv_spinbox_create),
    ESP_ELFSYM_EXPORT(esp32_spi_get_cs_pin),
    ESP_ELFSYM_EXPORT(sys_sem_signal_isr),
    ESP_ELFSYM_EXPORT(isgraph),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_send_alert_message),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_ap_max_chan),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_reset),
    ESP_ELFSYM_EXPORT(igmp_input),
    ESP_ELFSYM_EXPORT(backlight_get_min_brightness),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_ad_type_config),
    ESP_ELFSYM_EXPORT(_Z11init_uc6580P6Device),
    ESP_ELFSYM_EXPORT(USB_DEVICE_CONTROLLER_TYPE),
    ESP_ELFSYM_EXPORT(lv_canvas_set_draw_buf),
    ESP_ELFSYM_EXPORT(r_get_le16),
    ESP_ELFSYM_EXPORT(tcp_output),
    ESP_ELFSYM_EXPORT(r_ble_get_npl_element_info),
    ESP_ELFSYM_EXPORT(qrcode_initText),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_client_key_password),
    ESP_ELFSYM_EXPORT(hal_set_sta_light_sleep_wake_ahead_time),
    ESP_ELFSYM_EXPORT(i2s_controller_disable_direction),
    ESP_ELFSYM_EXPORT(esp_now_init),
    ESP_ELFSYM_EXPORT(_ZSt19piecewise_construct),
    ESP_ELFSYM_EXPORT(ble_store_key_from_value_csfc),
    ESP_ELFSYM_EXPORT(lv_textarea_delete_char),
    ESP_ELFSYM_EXPORT(lv_dropdown_set_dir),
    ESP_ELFSYM_EXPORT(wDev_Rxbuf_Init),
    ESP_ELFSYM_EXPORT(phy_start_tx_tone_step),
    ESP_ELFSYM_EXPORT(esp_http_client_get_content_length),
    ESP_ELFSYM_EXPORT(psa_wipe_all_key_slots),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_bg_image_recolor_opa),
    ESP_ELFSYM_EXPORT(pm_on_data_tx_done),
    ESP_ELFSYM_EXPORT(ble_sm_sc_dhkey_check_exec),
    ESP_ELFSYM_EXPORT(ip6_input),
    ESP_ELFSYM_EXPORT(pbuf_try_get_at),
    ESP_ELFSYM_EXPORT(lv_draw_sw_arc),
    ESP_ELFSYM_EXPORT(lv_theme_get_color_primary),
    ESP_ELFSYM_EXPORT(ampdu_process_multicast_address_qos_frame),
    ESP_ELFSYM_EXPORT(wpa_sm_pmksa_cache_flush),
    ESP_ELFSYM_EXPORT(wdev_mac_sleep),
    ESP_ELFSYM_EXPORT(lv_bin_decoder_open),
    ESP_ELFSYM_EXPORT(systimer_us_to_ticks),
    ESP_ELFSYM_EXPORT(_powf),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_ext_master_init),
    ESP_ELFSYM_EXPORT(ic_set_trc),
    ESP_ELFSYM_EXPORT(bt_agc_gain_max),
    ESP_ELFSYM_EXPORT(psa_export_public_key_internal),
    ESP_ELFSYM_EXPORT(i2c_master_bus_add_device),
    ESP_ELFSYM_EXPORT(hal_mac_set_txq_invalid),
    ESP_ELFSYM_EXPORT(i2s_channel_enable),
    ESP_ELFSYM_EXPORT(hal_set_sta_tbtt_ahead_time),
    ESP_ELFSYM_EXPORT(sdmmc_init_io_bus_width),
    ESP_ELFSYM_EXPORT(esp_crypto_ecdsa_enable_periph_clk),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_config_host_io_mode),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_alloc),
    ESP_ELFSYM_EXPORT(lv_font_get_glyph_dsc),
    ESP_ELFSYM_EXPORT(esp_mmu_map_get_max_consecutive_free_block_size),
    ESP_ELFSYM_EXPORT(pm_on_mac_modem_beacon_miss),
    ESP_ELFSYM_EXPORT(lv_font_glyph_release_draw_data),
    ESP_ELFSYM_EXPORT(ppGetTaskHdl),
    ESP_ELFSYM_EXPORT(lv_image_decoder_create),
    ESP_ELFSYM_EXPORT(scan_check_hidden),
    ESP_ELFSYM_EXPORT(lora_get_radio_state),
    ESP_ELFSYM_EXPORT(wifi_copy_ap_record),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_ap_chan),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_insert_forcibly),
    ESP_ELFSYM_EXPORT(uart_set_select_notif_callback),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_first_chan),
    ESP_ELFSYM_EXPORT(ble_svc_gap_deinit),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_cte_select_iq_sample_data_auto),
    ESP_ELFSYM_EXPORT(atexit),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_ap_ssid),
    ESP_ELFSYM_EXPORT(rtc_gpio_pullup_en),
    ESP_ELFSYM_EXPORT(lvgl_trackball_set_settings),
    ESP_ELFSYM_EXPORT(lv_label_create),
    ESP_ELFSYM_EXPORT(esp_libc_include_strncmp_impl),
    ESP_ELFSYM_EXPORT(fmin),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_BLOCK_KEY0),
    ESP_ELFSYM_EXPORT(lv_draw_create_unit),
    ESP_ELFSYM_EXPORT(g_bss_color_collision_detection_enabled),
    ESP_ELFSYM_EXPORT(esp_key_mgr_deactivate_key),
    ESP_ELFSYM_EXPORT(mpi_hal_read_result_hw_op),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_rf_ctrl_enable),
    ESP_ELFSYM_EXPORT(lmacProcessTxTimeout),
    ESP_ELFSYM_EXPORT(lv_timer_get_user_data),
    ESP_ELFSYM_EXPORT(ic_set_mac),
    ESP_ELFSYM_EXPORT(localtime_r),
    ESP_ELFSYM_EXPORT(_ZTVSt11__timepunctIcE),
    ESP_ELFSYM_EXPORT(spi_bus_get_dma_ctx),
    ESP_ELFSYM_EXPORT(esp_log_linked_list_clean),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_connless_cte_tx_set_param),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_set_scan_params),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_get_cur_phy_txpwr),
    ESP_ELFSYM_EXPORT(ble_gatts_init),
    ESP_ELFSYM_EXPORT(adv_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_opaque_info),
    ESP_ELFSYM_EXPORT(npl_freertos_eventq_put),
    ESP_ELFSYM_EXPORT(display_get_gap_y),
    ESP_ELFSYM_EXPORT(pp_register_tx_cb),
    ESP_ELFSYM_EXPORT(esp_mmu_unmap),
    ESP_ELFSYM_EXPORT(hal_tsf_get_tbttstart),
    ESP_ELFSYM_EXPORT(esp_test_enable_rx_mu_statistics),
    ESP_ELFSYM_EXPORT(file_system_is_mounted),
    ESP_ELFSYM_EXPORT(ieee80211_classify),
    ESP_ELFSYM_EXPORT(lvgl_hardware_keyboard_remove_custom),
    ESP_ELFSYM_EXPORT(r_ble_ll_deinit),
    ESP_ELFSYM_EXPORT(lv_style_set_line_color),
    ESP_ELFSYM_EXPORT(esp_libc_init_funcs),
    ESP_ELFSYM_EXPORT(wifi_ipc_process),
    ESP_ELFSYM_EXPORT(pm_force_soc_wakeup),
    ESP_ELFSYM_EXPORT(psa_reset_key_attributes),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_skip_common_name_check),
    ESP_ELFSYM_EXPORT(lv_ll_clear),
    ESP_ELFSYM_EXPORT(ieee80211_protocol_attach),
    ESP_ELFSYM_EXPORT(http_header_set),
    ESP_ELFSYM_EXPORT(regularize_k),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_tls_version),
    ESP_ELFSYM_EXPORT(_ZGVNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(adv_stack_enableScanReqRxdVsEvent),
    ESP_ELFSYM_EXPORT(phy_i2c_bbpll_set),
    ESP_ELFSYM_EXPORT(psa_generic_status_to_mbedtls),
    ESP_ELFSYM_EXPORT(r_ble_phy_hw_state_check),
    ESP_ELFSYM_EXPORT(dhcp_fine_tmr),
    ESP_ELFSYM_EXPORT(bluetooth_disconnect),
    ESP_ELFSYM_EXPORT(scan_bitmap_to_channel_list),
    ESP_ELFSYM_EXPORT(adiList_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(psa_its_remove),
    ESP_ELFSYM_EXPORT(psa_raw_key_agreement),
    ESP_ELFSYM_EXPORT(rand_r),
    ESP_ELFSYM_EXPORT(select),
    ESP_ELFSYM_EXPORT(lv_group_set_default),
    ESP_ELFSYM_EXPORT(lv_draw_dispatch_request),
    ESP_ELFSYM_EXPORT(nimble_port_freertos_deinit),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_prep_write),
    ESP_ELFSYM_EXPORT(bt_bb_tx_cca_period),
    ESP_ELFSYM_EXPORT(esp_aes_cipher_update),
    ESP_ELFSYM_EXPORT(i2s_channel_read),
    ESP_ELFSYM_EXPORT(grove_set_mode),
    ESP_ELFSYM_EXPORT(esp_wifi_set_promiscuous),
    ESP_ELFSYM_EXPORT(lv_indev_search_obj),
    ESP_ELFSYM_EXPORT(mbedtls_md_free),
    ESP_ELFSYM_EXPORT(uart_pattern_queue_reset),
    ESP_ELFSYM_EXPORT(bt_rf_coex_pti_dft_p),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_outline_stroke_width),
    ESP_ELFSYM_EXPORT(periph_rcc_acquire_enter),
    ESP_ELFSYM_EXPORT(ieee80211_setup_pmf),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_aux_data_flag_set),
    ESP_ELFSYM_EXPORT(nonhe_clr_snd_ch_cfg),
    ESP_ELFSYM_EXPORT(lv_style_set_max_height),
    ESP_ELFSYM_EXPORT(esp_vfs_unregister_fs),
    ESP_ELFSYM_EXPORT(wdev_funcs_init),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_write_binary_le),
    ESP_ELFSYM_EXPORT(power_supply_supports_power_off),
    ESP_ELFSYM_EXPORT(ieee80211_set_gtk),
    ESP_ELFSYM_EXPORT(properties_file_has),
    ESP_ELFSYM_EXPORT(esp_vfs_rename),
    ESP_ELFSYM_EXPORT(sdmmc_write_sectors),
    ESP_ELFSYM_EXPORT(ieee80211_ifdetach),
    ESP_ELFSYM_EXPORT(coex_register_start_cb),
    ESP_ELFSYM_EXPORT(ic_mac_init),
    ESP_ELFSYM_EXPORT(ip4addr_aton),
    ESP_ELFSYM_EXPORT(ble_store_delete_csfc),
    ESP_ELFSYM_EXPORT(__wrap_gpio_set_level),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_sequence_free),
    ESP_ELFSYM_EXPORT(task_event_group_destruct),
    ESP_ELFSYM_EXPORT(ic_register_tx_cb),
    ESP_ELFSYM_EXPORT(r_ble_hw_whitelist_rmv),
    ESP_ELFSYM_EXPORT(omac1_aes_128_vector),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_prof_authmode_internal),
    ESP_ELFSYM_EXPORT(sync_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(ic_set_bar_rate),
    ESP_ELFSYM_EXPORT(mld6_joingroup_netif),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_get_country),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_starts),
    ESP_ELFSYM_EXPORT(lv_draw_dispatch_layer),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_sign_det_restartable),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_ciphersuite_sig_pk_psa_alg),
    ESP_ELFSYM_EXPORT(__xpg_strerror_r),
    ESP_ELFSYM_EXPORT(lv_image_set_src),
    ESP_ELFSYM_EXPORT(ieee80211_set_max_rate),
    ESP_ELFSYM_EXPORT(r_ble_ll_flush_pkt_queue),
    ESP_ELFSYM_EXPORT(esp_event_handler_unregister),
    ESP_ELFSYM_EXPORT(ethernet_output),
    ESP_ELFSYM_EXPORT(ble_osi_coex_funcs_register),
    ESP_ELFSYM_EXPORT(pm_twt_wake_up),
    ESP_ELFSYM_EXPORT(_Z26ble_gap_disc_event_handlerP13ble_gap_eventPv),
    ESP_ELFSYM_EXPORT(vPortFree),
    ESP_ELFSYM_EXPORT(esp_brownout_init),
    ESP_ELFSYM_EXPORT(spi_flash_hal_gpspi_device_config),
    ESP_ELFSYM_EXPORT(coexist_printf),
    ESP_ELFSYM_EXPORT(_lock_try_acquire_recursive),
    ESP_ELFSYM_EXPORT(ble_gap_notify_rx_event),
    ESP_ELFSYM_EXPORT(lv_cache_create),
    ESP_ELFSYM_EXPORT(_Z13init_atgm336hP6Device),
    ESP_ELFSYM_EXPORT(r_ble_ll_msys_mem_copy),
    ESP_ELFSYM_EXPORT(_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE),
    ESP_ELFSYM_EXPORT(spi_flash_cache2phys),
    ESP_ELFSYM_EXPORT(spi_flash_cache_enabled),
    ESP_ELFSYM_EXPORT(minitar_open),
    ESP_ELFSYM_EXPORT(wifi_get_promiscuous),
    ESP_ELFSYM_EXPORT(xpt2046_softspi_module),
    ESP_ELFSYM_EXPORT(ppRxPkt),
    ESP_ELFSYM_EXPORT(asin),
    ESP_ELFSYM_EXPORT(_exit),
    ESP_ELFSYM_EXPORT(ieee80211_ap_sa_query_timeout),
    ESP_ELFSYM_EXPORT(ic_register_timer_post_cb),
    ESP_ELFSYM_EXPORT(spi_hal_cal_timing),
    ESP_ELFSYM_EXPORT(sdspi_host_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_created),
    ESP_ELFSYM_EXPORT(phy_track_pll_deinit),
    ESP_ELFSYM_EXPORT(ble_gap_authorize_event),
    ESP_ELFSYM_EXPORT(wifi_sta_restore_default_mac),
    ESP_ELFSYM_EXPORT(_ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE),
    ESP_ELFSYM_EXPORT(pm_hmac_rx_data_process),
    ESP_ELFSYM_EXPORT(device_get_driver),
    ESP_ELFSYM_EXPORT(esp_http_client_perform),
    ESP_ELFSYM_EXPORT(g_cte_ant_delay),
    ESP_ELFSYM_EXPORT(hal_mac_is_txq_valid),
    ESP_ELFSYM_EXPORT(httpd_queue_work),
    ESP_ELFSYM_EXPORT(vEventGroupClearBitsCallback),
    ESP_ELFSYM_EXPORT(esp_wifi_set_promiscuous_rx_cb),
    ESP_ELFSYM_EXPORT(strncasecmp),
    ESP_ELFSYM_EXPORT(sdmmc_decode_scr),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_version_ind),
    ESP_ELFSYM_EXPORT(ble_controller_get_compile_version),
    ESP_ELFSYM_EXPORT(esp_sha1_finish),
    ESP_ELFSYM_EXPORT(_ZTVSt15_Sp_counted_ptrIPN2tt7service6espnow13EspNowServiceELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(periph_rcc_release_exit),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_pb_adv_link_id_table_remove),
    ESP_ELFSYM_EXPORT(r_ble_hw_rng_stop),
    ESP_ELFSYM_EXPORT(phy_change_channel),
    ESP_ELFSYM_EXPORT(ble_hs_misc_conn_chan_find_reqd),
    ESP_ELFSYM_EXPORT(esp_timer_get_time),
    ESP_ELFSYM_EXPORT(camera_get_height),
    ESP_ELFSYM_EXPORT(rtc_time_get),
    ESP_ELFSYM_EXPORT(advFilter_hci_configDuplicateExceptionList),
    ESP_ELFSYM_EXPORT(__libc_fini_array),
    ESP_ELFSYM_EXPORT(wdt_hal_init),
    ESP_ELFSYM_EXPORT(lv_obj_set_height),
    ESP_ELFSYM_EXPORT(periph_rcc_release_enter),
    ESP_ELFSYM_EXPORT(gpio_descriptor_get_level),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_ext),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_shift_l),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_conn_update),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_x),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_rpa_timer_cb),
    ESP_ELFSYM_EXPORT(ieee80211_sta_scan),
    ESP_ELFSYM_EXPORT(lv_event_get_layer),
    ESP_ELFSYM_EXPORT(phy_rfpll_set_adc_rate),
    ESP_ELFSYM_EXPORT(__wrap_lv_list_create),
    ESP_ELFSYM_EXPORT(ic_ampdu_op),
    ESP_ELFSYM_EXPORT(pm_on_sample_beacon),
    ESP_ELFSYM_EXPORT(pm_enable_dream_timer),
    ESP_ELFSYM_EXPORT(pm_mac_sleep),
    ESP_ELFSYM_EXPORT(esp_register_shutdown_handler),
    ESP_ELFSYM_EXPORT(esp_test_enable_tx_statistics),
    ESP_ELFSYM_EXPORT(esp_log_linked_list_get_level),
    ESP_ELFSYM_EXPORT(lv_display_get_color_format),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_env_deinit),
    ESP_ELFSYM_EXPORT(wpa_ap_remove),
    ESP_ELFSYM_EXPORT(lv_spinbox_set_digit_step_direction),
    ESP_ELFSYM_EXPORT(uart_controller_set_config),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_exec_write),
    ESP_ELFSYM_EXPORT(esp_efuse_get_field_size),
    ESP_ELFSYM_EXPORT(wdev_mac_wakeup),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_enable_etm_task),
    ESP_ELFSYM_EXPORT(get_estimated_batime),
    ESP_ELFSYM_EXPORT(crypto_ec_point_cmp),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_gcd_modinv_odd),
    ESP_ELFSYM_EXPORT(lv_tabview_get_tab_bar),
    ESP_ELFSYM_EXPORT(_ZZNKSt8__format14__formatter_fpIcE6formatIfNS_10_Sink_iterIcEEEENSt20basic_format_contextIT0_cE8iteratorET_RS7_ENKUlPcSB_E_clESB_SB_),
    ESP_ELFSYM_EXPORT(NUS_SVC_UUID),
    ESP_ELFSYM_EXPORT(esp_wifi_bt_power_domain_off),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_cmac_update),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_margin_top),
    ESP_ELFSYM_EXPORT(mbedtls_pk_ecc_set_group),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(memp_TCPIP_MSG_API),
    ESP_ELFSYM_EXPORT(esp_panic_handler),
    ESP_ELFSYM_EXPORT(ecc_hal_read_verify_result),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_update_sync_info_in_buffer),
    ESP_ELFSYM_EXPORT(tcp_update_rcv_ann_wnd),
    ESP_ELFSYM_EXPORT(mbedtls_psa_aead_abort),
    ESP_ELFSYM_EXPORT(properties_file_get),
    ESP_ELFSYM_EXPORT(ieee80211_copy_eb_header),
    ESP_ELFSYM_EXPORT(esp_test_get_rx_error_occurs),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_insert),
    ESP_ELFSYM_EXPORT(wifi_create_queue),
    ESP_ELFSYM_EXPORT(spi_flash_encryption_hal_enable),
    ESP_ELFSYM_EXPORT(ieee80211_recv_action_register),
    ESP_ELFSYM_EXPORT(ble_sm_num_procs),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_flush_output),
    ESP_ELFSYM_EXPORT(lv_dropdown_close),
    ESP_ELFSYM_EXPORT(lv_text_get_next_line),
    ESP_ELFSYM_EXPORT(psa_free_key_slot),
    ESP_ELFSYM_EXPORT(__ceil_log2pow5),
    ESP_ELFSYM_EXPORT(ieee80211_crypto_encap),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_list_find),
    ESP_ELFSYM_EXPORT(tcpip_try_callback),
    ESP_ELFSYM_EXPORT(wpabuf_alloc_copy),
    ESP_ELFSYM_EXPORT(sta_recv_ctl),
    ESP_ELFSYM_EXPORT(isupper),
    ESP_ELFSYM_EXPORT(nvs_flash_init_partition),
    ESP_ELFSYM_EXPORT(putle32),
    ESP_ELFSYM_EXPORT(phy_adc_rate_set),
    ESP_ELFSYM_EXPORT(ppDequeueTxDone_Locked),
    ESP_ELFSYM_EXPORT(udp_recv),
    ESP_ELFSYM_EXPORT(tcp_input_pcb),
    ESP_ELFSYM_EXPORT(_ZdlPvSt11align_val_t),
    ESP_ELFSYM_EXPORT(i2s_platform_release_occupation),
    ESP_ELFSYM_EXPORT(sdmmc_decode_ssr),
    ESP_ELFSYM_EXPORT(esp_system_reset_modules_on_exit),
    ESP_ELFSYM_EXPORT(lv_roller_class),
    ESP_ELFSYM_EXPORT(r_ble_ll_ext_scan_parse_aux_ptr),
    ESP_ELFSYM_EXPORT(lv_refr_deinit),
    ESP_ELFSYM_EXPORT(lodepng_chunk_check_crc),
    ESP_ELFSYM_EXPORT(phy_rate_to_index),
    ESP_ELFSYM_EXPORT(ic_interface_enabled),
    ESP_ELFSYM_EXPORT(app_manager_finish),
    ESP_ELFSYM_EXPORT(nd6_tmr),
    ESP_ELFSYM_EXPORT(pm_twt_set_target_tsf),
    ESP_ELFSYM_EXPORT(bluetooth_connect),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_info),
    ESP_ELFSYM_EXPORT(lv_arc_set_knob_offset),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_check_itvl_data_len_is_match),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_buf_free),
    ESP_ELFSYM_EXPORT(pm_stop),
    ESP_ELFSYM_EXPORT(esp_flash_init_default_chip),
    ESP_ELFSYM_EXPORT(g_msys_ctrl_noconn_mbuf_cnt),
    ESP_ELFSYM_EXPORT(esp_clk_tree_enable_src),
    ESP_ELFSYM_EXPORT(rcUpdateAMPDUParam),
    ESP_ELFSYM_EXPORT(lv_keyboard_class),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_read_unique_id),
    ESP_ELFSYM_EXPORT(sdmmc_select_current_limit),
    ESP_ELFSYM_EXPORT(ff_memfree),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx119money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(tc_cmac_update),
    ESP_ELFSYM_EXPORT(os_msys_deinit),
    ESP_ELFSYM_EXPORT(ic_stop_hw_txq),
    ESP_ELFSYM_EXPORT(ap_rxcb),
    ESP_ELFSYM_EXPORT(vTaskMissedYield),
    ESP_ELFSYM_EXPORT(app_paths_get_user_data_directory),
    ESP_ELFSYM_EXPORT(ap_sta_hash_add),
    ESP_ELFSYM_EXPORT(lv_obj_get_user_data),
    ESP_ELFSYM_EXPORT(esp_vfs_access),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx119money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEEE),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_read_type),
    ESP_ELFSYM_EXPORT(psa_its_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_stop_rsp_timer),
    ESP_ELFSYM_EXPORT(esp_wifi_is_if_ready_when_started),
    ESP_ELFSYM_EXPORT(hci_transport_deinit),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_scan),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_channel_class_enable_make),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_filter_addr_add),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_coex_dpc_process_sec),
    ESP_ELFSYM_EXPORT(mbedtls_platform_zeroize),
    ESP_ELFSYM_EXPORT(esp_reset_reason),
    ESP_ELFSYM_EXPORT(lv_checkbox_class),
    ESP_ELFSYM_EXPORT(expf),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_add_dropdown_action),
    ESP_ELFSYM_EXPORT(__lock___malloc_recursive_mutex),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_MAC),
    ESP_ELFSYM_EXPORT(adc_reset_lock_release),
    ESP_ELFSYM_EXPORT(tcp_keepalive),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_BLOCK_KEY4),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_env_init),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_bg_color),
    ESP_ELFSYM_EXPORT(pm_on_probe_resp_rx),
    ESP_ELFSYM_EXPORT(st7796_module),
    ESP_ELFSYM_EXPORT(esp_vfs_stat),
    ESP_ELFSYM_EXPORT(config_get_wifi_task_stack_size),
    ESP_ELFSYM_EXPORT(usb_hid_device_send_mouse),
    ESP_ELFSYM_EXPORT(lv_indev_reset),
    ESP_ELFSYM_EXPORT(mem_free),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_cmac_starts),
    ESP_ELFSYM_EXPORT(lv_spinbox_get_rollover),
    ESP_ELFSYM_EXPORT(_ZTVSt8time_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE),
    ESP_ELFSYM_EXPORT(lv_tick_elaps),
    ESP_ELFSYM_EXPORT(clk_hal_lp_slow_get_freq_hz),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_default_wifi_conn),
    ESP_ELFSYM_EXPORT(wpa_sm_free_eapol),
    ESP_ELFSYM_EXPORT(phy_get_max_pwr),
    ESP_ELFSYM_EXPORT(esp_sha_read_digest_state),
    ESP_ELFSYM_EXPORT(r_ble_phy_get_txdbm_by_level),
    ESP_ELFSYM_EXPORT(dhcp_renew),
    ESP_ELFSYM_EXPORT(esp32_ble_hid_device_driver),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_auth_pyld_timer_cb),
    ESP_ELFSYM_EXPORT(lv_ll_clear_custom),
    ESP_ELFSYM_EXPORT(ieee80211_get_cipher_mic_len),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_send_consumer),
    ESP_ELFSYM_EXPORT(coex_core_disable),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_suspend_cmd_conf),
    ESP_ELFSYM_EXPORT(_ZSt16__convert_from_vRKPiPciPKcz),
    ESP_ELFSYM_EXPORT(wifi_init_completed),
    ESP_ELFSYM_EXPORT(wifi_auto_scan_set_paused_function),
    ESP_ELFSYM_EXPORT(esp_netif_action_disconnected),
    ESP_ELFSYM_EXPORT(ble_sm_alg_g2),
    ESP_ELFSYM_EXPORT(lv_obj_get_style_recolor_recursive),
    ESP_ELFSYM_EXPORT(lvgl_trackball_settings_get_default),
    ESP_ELFSYM_EXPORT(minmea_parse_gga),
    ESP_ELFSYM_EXPORT(phy_iq_est_enable_new),
    ESP_ELFSYM_EXPORT(access),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_validate_crt),
    ESP_ELFSYM_EXPORT(r_ble_lll_df_ring_buffer_deinit),
    ESP_ELFSYM_EXPORT(wifi_hw_start),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_deduce_crt),
    ESP_ELFSYM_EXPORT(lv_spinbox_set_rollover),
    ESP_ELFSYM_EXPORT(ampdu_free_rx_ba_index),
    ESP_ELFSYM_EXPORT(ff_diskio_register_wl_partition),
    ESP_ELFSYM_EXPORT(pm_beacon_timestamp_statistic),
    ESP_ELFSYM_EXPORT(ble_hs_hci_acl_tx),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_start_with_desc),
    ESP_ELFSYM_EXPORT(lv_spangroup_add_span),
    ESP_ELFSYM_EXPORT(mt_is_peer_exist),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_periodic_stop),
    ESP_ELFSYM_EXPORT(mbedtls_sha256_finish),
    ESP_ELFSYM_EXPORT(wifi_station_save_ap_channel),
    ESP_ELFSYM_EXPORT(netconn_accept),
    ESP_ELFSYM_EXPORT(phy_txdc_cal_pwdet),
    ESP_ELFSYM_EXPORT(fdopen),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_ext_initiator_start),
    ESP_ELFSYM_EXPORT(s_tbttstart),
    ESP_ELFSYM_EXPORT(midi_io_handle),
    ESP_ELFSYM_EXPORT(hal_random),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_connecting),
    ESP_ELFSYM_EXPORT(esp_register_npl_funcs),
    ESP_ELFSYM_EXPORT(heap_caps_calloc_base),
    ESP_ELFSYM_EXPORT(_ZSt20__throw_out_of_rangePKc),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_env_init),
    ESP_ELFSYM_EXPORT(httpd_recv),
    ESP_ELFSYM_EXPORT(phy_dpd_rxdc_cal_init),
    ESP_ELFSYM_EXPORT(wifi_delete_queue),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_sequence_of),
    ESP_ELFSYM_EXPORT(vTaskSetThreadLocalStoragePointer),
    ESP_ELFSYM_EXPORT(esp_netif_start),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt10moneypunctIcLb0EEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(lv_style_set_pad_row),
    ESP_ELFSYM_EXPORT(phy_chan_to_freq),
    ESP_ELFSYM_EXPORT(mbedtls_mutex_init_ptr),
    ESP_ELFSYM_EXPORT(r_ble_enable_bqb_test),
    ESP_ELFSYM_EXPORT(lwip_inet_pton),
    ESP_ELFSYM_EXPORT(pm_tx_done_itwt_process),
    ESP_ELFSYM_EXPORT(ieee80211_vnd_lora_ie_size),
    ESP_ELFSYM_EXPORT(lv_color_mix),
    ESP_ELFSYM_EXPORT(hal_sniffer_enable),
    ESP_ELFSYM_EXPORT(g_os_mempool_list),
    ESP_ELFSYM_EXPORT(backlight_get_brightness),
    ESP_ELFSYM_EXPORT(phy_xpd_rf),
    ESP_ELFSYM_EXPORT(wifi_on_coex_schm_phase_process),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_base_lookup_table),
    ESP_ELFSYM_EXPORT(empty_func_p_ptsf_u32_r_void),
    ESP_ELFSYM_EXPORT(btwt_setup_timeout_fn_process),
    ESP_ELFSYM_EXPORT(bluetooth_set_hid_host_active),
    ESP_ELFSYM_EXPORT(spi_flash_hal_poll_cmd_done),
    ESP_ELFSYM_EXPORT(phy_txcap_setting),
    ESP_ELFSYM_EXPORT(lv_textarea_set_max_length),
    ESP_ELFSYM_EXPORT(s_phy_get_max_pwr_new),
    ESP_ELFSYM_EXPORT(uxTaskGetNumberOfTasks),
    ESP_ELFSYM_EXPORT(frexpf),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_is_dfs_channel),
    ESP_ELFSYM_EXPORT(pm_beacon_offset_funcs_init),
    ESP_ELFSYM_EXPORT(ieee80211_add_bss_max_idle),
    ESP_ELFSYM_EXPORT(sta_retry_assoc),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_controller_sleep_en),
    ESP_ELFSYM_EXPORT(ble_sm_slave_initiate),
    ESP_ELFSYM_EXPORT(phy_get_power_db),
    ESP_ELFSYM_EXPORT(send_deauth),
    ESP_ELFSYM_EXPORT(file_system_set_owner),
    ESP_ELFSYM_EXPORT(pm_noise_check_disable),
    ESP_ELFSYM_EXPORT(comeback_token_hash),
    ESP_ELFSYM_EXPORT(pm_off_channel),
    ESP_ELFSYM_EXPORT(efuse_hal_chip_revision),
    ESP_ELFSYM_EXPORT(display_get_resolution_y),
    ESP_ELFSYM_EXPORT(tt_timezone_set),
    ESP_ELFSYM_EXPORT(esp_aes_crypt_cbc),
    ESP_ELFSYM_EXPORT(esp_crypto_common_clk_enable),
    ESP_ELFSYM_EXPORT(pm_beacon_add_loss_counter),
    ESP_ELFSYM_EXPORT(aes_hal_mode_init),
    ESP_ELFSYM_EXPORT(lv_draw_rect),
    ESP_ELFSYM_EXPORT(r_swap_buf),
    ESP_ELFSYM_EXPORT(ble_gap_identity_event),
    ESP_ELFSYM_EXPORT(lv_cache_entry_release_data),
    ESP_ELFSYM_EXPORT(_ZTVSt7collateIcE),
    ESP_ELFSYM_EXPORT(_ZStlsIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_St8_SetfillIS3_E),
    ESP_ELFSYM_EXPORT(uart_disable_intr_mask),
    ESP_ELFSYM_EXPORT(ppResortTxAMPDU),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_set_selected_button),
    ESP_ELFSYM_EXPORT(tsf_hal_set_modem_timer_wakeup_early_time),
    ESP_ELFSYM_EXPORT(regdma_link_new_write),
    ESP_ELFSYM_EXPORT(r_ble_ll_set_sync_transfer_params),
    ESP_ELFSYM_EXPORT(lv_strlen),
    ESP_ELFSYM_EXPORT(npl_freertos_sem_pend),
    ESP_ELFSYM_EXPORT(gpio_hal_intr_disable),
    ESP_ELFSYM_EXPORT(phy_bbtx_outfilter),
    ESP_ELFSYM_EXPORT(lv_tabview_create),
    ESP_ELFSYM_EXPORT(esp_coex_common_task_yield_from_isr_wrapper),
    ESP_ELFSYM_EXPORT(r_ble_hw_set_connected_dev),
    ESP_ELFSYM_EXPORT(ble_gap_adv_start),
    ESP_ELFSYM_EXPORT(panic_print_registers),
    ESP_ELFSYM_EXPORT(ppCheckTxRTS),
    ESP_ELFSYM_EXPORT(BAROFDMSched),
    ESP_ELFSYM_EXPORT(esp_mpi_mul_mpi_hw_op),
    ESP_ELFSYM_EXPORT(g_ble_lll_sleep_env_p),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_reset),
    ESP_ELFSYM_EXPORT(pmksa_cache_add),
    ESP_ELFSYM_EXPORT(asinhf),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_read),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_angle_init),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_wifi_conn),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_data_base_reset),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_tx_process),
    ESP_ELFSYM_EXPORT(lvgl_software_keyboard_construct),
    ESP_ELFSYM_EXPORT(ispunct),
    ESP_ELFSYM_EXPORT(wDevCheckBlockError),
    ESP_ELFSYM_EXPORT(esp_flash_app_enable_os_functions),
    ESP_ELFSYM_EXPORT(stdout),
    ESP_ELFSYM_EXPORT(gdma_hal_disconnect_all),
    ESP_ELFSYM_EXPORT(esp_md5_hash_compute),
    ESP_ELFSYM_EXPORT(esf_buf_alloc_dynamic),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_cmp_abs),
    ESP_ELFSYM_EXPORT(hal_init_tx_pwr),
    ESP_ELFSYM_EXPORT(_ZGVNSt9money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(lv_strcat),
    ESP_ELFSYM_EXPORT(esp_hmac_opaque_size_function),
    ESP_ELFSYM_EXPORT(phy_wr_rx_gain_mem),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_allocated_flag_set),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_actual_word_size_get),
    ESP_ELFSYM_EXPORT(trc_init),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_private),
    ESP_ELFSYM_EXPORT(lv_obj_remove_flag),
    ESP_ELFSYM_EXPORT(freopen),
    ESP_ELFSYM_EXPORT(ieee80211_set_bss_max_idle_period),
    ESP_ELFSYM_EXPORT(display_get_color_format),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt11__timepunctIcEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(lv_obj_has_flag_any),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx1110moneypunctIcLb1EE2idE),
    ESP_ELFSYM_EXPORT(hal_he_set_power_save),
    ESP_ELFSYM_EXPORT(rssi_margin),
    ESP_ELFSYM_EXPORT(g_eb_list_desc),
    ESP_ELFSYM_EXPORT(ieee80211_ht_deattach),
    ESP_ELFSYM_EXPORT(s_itwt_id),
    ESP_ELFSYM_EXPORT(ble_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(_g_esp_netif_soft_ap_ip),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_periodic_start),
    ESP_ELFSYM_EXPORT(lodepng_add_text),
    ESP_ELFSYM_EXPORT(phy_bb_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_filter_enabled),
    ESP_ELFSYM_EXPORT(pthread_getspecific),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_conn_cte_req_enable),
    ESP_ELFSYM_EXPORT(lvgl_ppa_get_or_create),
    ESP_ELFSYM_EXPORT(lv_obj_enable_style_refresh),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_image_recolor),
    ESP_ELFSYM_EXPORT(lwip_strnicmp),
    ESP_ELFSYM_EXPORT(r_DB_check_addr_exist),
    ESP_ELFSYM_EXPORT(phy_ckgen_5g_cal),
    ESP_ELFSYM_EXPORT(lv_indev_set_group),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_set_arg),
    ESP_ELFSYM_EXPORT(wifi_init_bss_color),
    ESP_ELFSYM_EXPORT(cnx_beacon_timeout_process),
    ESP_ELFSYM_EXPORT(esp_event_post_to),
    ESP_ELFSYM_EXPORT(lv_obj_style_init),
    ESP_ELFSYM_EXPORT(acoshf),
    ESP_ELFSYM_EXPORT(regi2c_saradc_enable),
    ESP_ELFSYM_EXPORT(wpa_res_to_status_code),
    ESP_ELFSYM_EXPORT(_Z11init_ag33xxP6Device),
    ESP_ELFSYM_EXPORT(console_fsync),
    ESP_ELFSYM_EXPORT(vPortEnterCritical),
    ESP_ELFSYM_EXPORT(spi_hal_hw_prepare_rx),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_base_dir),
    ESP_ELFSYM_EXPORT(_ZTSSt12system_error),
    ESP_ELFSYM_EXPORT(crypto_mbedtls_get_grp_id),
    ESP_ELFSYM_EXPORT(lv_theme_get_color_secondary),
    ESP_ELFSYM_EXPORT(coex_env),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_is_dev_connected),
    ESP_ELFSYM_EXPORT(xEventGroupCreateStatic),
    ESP_ELFSYM_EXPORT(mt_register_recv_cb),
    ESP_ELFSYM_EXPORT(wpa_supplicant_pairwise_gtk),
    ESP_ELFSYM_EXPORT(esp_netif_sntp_deinit),
    ESP_ELFSYM_EXPORT(module_is_started),
    ESP_ELFSYM_EXPORT(lv_anim_set_user_data),
    ESP_ELFSYM_EXPORT(esp_tls_internal_event_tracker_destroy),
    ESP_ELFSYM_EXPORT(ieee80211_recv_action),
    ESP_ELFSYM_EXPORT(sta_is_11r_enabled),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_crypt),
    ESP_ELFSYM_EXPORT(tlsf_create),
    ESP_ELFSYM_EXPORT(__bswapdi2),
    ESP_ELFSYM_EXPORT(ble_gatts_peer_cl_sup_feat_get),
    ESP_ELFSYM_EXPORT(aes_siv_decrypt),
    ESP_ELFSYM_EXPORT(audio_stream_set_enabled),
    ESP_ELFSYM_EXPORT(esp_cache_unfreeze_caches_enable_interrupts),
    ESP_ELFSYM_EXPORT(usb_device_controller_get_active_class),
    ESP_ELFSYM_EXPORT(xEventGroupCreateWithCaps),
    ESP_ELFSYM_EXPORT(tcp_abort),
    ESP_ELFSYM_EXPORT(vht_get_min_subframe_len),
    ESP_ELFSYM_EXPORT(lv_timer_set_period),
    ESP_ELFSYM_EXPORT(bluetooth_add_event_callback),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_reset),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_init),
    ESP_ELFSYM_EXPORT(bt_osi_mem_free_internal),
    ESP_ELFSYM_EXPORT(esp_timer_init_include_func),
    ESP_ELFSYM_EXPORT(lv_spangroup_class),
    ESP_ELFSYM_EXPORT(hal_disable_sta_tsf),
    ESP_ELFSYM_EXPORT(lv_arc_get_max_value),
    ESP_ELFSYM_EXPORT(thread_join),
    ESP_ELFSYM_EXPORT(esp_wifi_unset_appie_internal),
    ESP_ELFSYM_EXPORT(phy_set_rx_gain_cal_dc),
    ESP_ELFSYM_EXPORT(esp_clk_tree_initialize),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_conn_wifi_connecting),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_set_padding),
    ESP_ELFSYM_EXPORT(heap_caps_add_region_with_caps),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_enable),
    ESP_ELFSYM_EXPORT(crypto_ec_order_len),
    ESP_ELFSYM_EXPORT(cnx_sta_color_change_fn_process),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_CH1_ATTEN0_INITCODE_DIFF),
    ESP_ELFSYM_EXPORT(pthread_mutex_destroy),
    ESP_ELFSYM_EXPORT(esp_mesh_quick_funcs),
    ESP_ELFSYM_EXPORT(hal_crypto_init),
    ESP_ELFSYM_EXPORT(ble_att_svr_read_handle),
    ESP_ELFSYM_EXPORT(_ZSt11__make_heapIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SI_SL_),
    ESP_ELFSYM_EXPORT(fabsf),
    ESP_ELFSYM_EXPORT(ic_get_80211_tx_rate),
    ESP_ELFSYM_EXPORT(ppAMPDU2Normal),
    ESP_ELFSYM_EXPORT(xRingbufferSend),
    ESP_ELFSYM_EXPORT(hal_wdev_timer_target_enable),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_timing_tuning),
    ESP_ELFSYM_EXPORT(esp_efuse_utility_fill_buff),
    ESP_ELFSYM_EXPORT(rand_init),
    ESP_ELFSYM_EXPORT(esp_crypto_ecdsa_lock_release),
    ESP_ELFSYM_EXPORT(_ZTVSt9money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE),
    ESP_ELFSYM_EXPORT(dhcp_release_and_stop),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_period_timer_cb),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_alloc_buffer_hdr),
    ESP_ELFSYM_EXPORT(ieee80211_is_he_cipher),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_proc_with_instant_initiated),
    ESP_ELFSYM_EXPORT(lv_theme_default_deinit),
    ESP_ELFSYM_EXPORT(esp_clk_utils_mspi_speed_mode_sync_after_cpu_freq_switching),
    ESP_ELFSYM_EXPORT(lv_obj_has_state),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx1110moneypunctIcLb0EEE),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_adjust_pyld_len),
    ESP_ELFSYM_EXPORT(ic_trc_set_per_pkt_rate),
    ESP_ELFSYM_EXPORT(lora_set_enabled),
    ESP_ELFSYM_EXPORT(lv_chart_set_all_values),
    ESP_ELFSYM_EXPORT(netconn_gethostbyname_addrtype_n),
    ESP_ELFSYM_EXPORT(power_supply_supports_charge_control),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_common_name),
    ESP_ELFSYM_EXPORT(lv_event_remove_all),
    ESP_ELFSYM_EXPORT(tcp_send_fin),
    ESP_ELFSYM_EXPORT(ecp_mul_restartable_internal_soft),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_gen_priv_addr),
    ESP_ELFSYM_EXPORT(pm_on_tsf_timer),
    ESP_ELFSYM_EXPORT(sar_periph_ctrl_adc_oneshot_power_acquire),
    ESP_ELFSYM_EXPORT(ieee80211_setup_vht_rates),
    ESP_ELFSYM_EXPORT(ble_gap_event_connect_call),
    ESP_ELFSYM_EXPORT(netif_get_by_index),
    ESP_ELFSYM_EXPORT(lv_indev_get_gesture_dir),
    ESP_ELFSYM_EXPORT(psa_initialize_key_slots),
    ESP_ELFSYM_EXPORT(udp_disconnect),
    ESP_ELFSYM_EXPORT(multi_heap_aligned_alloc_impl_offs),
    ESP_ELFSYM_EXPORT(phy_pbus_xpd_iq_path),
    ESP_ELFSYM_EXPORT(esp_sha512_driver_finish),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_schedule_chain),
    ESP_ELFSYM_EXPORT(spi_bus_initialize),
    ESP_ELFSYM_EXPORT(memspi_host_read_status_hs),
    ESP_ELFSYM_EXPORT(lv_calendar_header_dropdown_class),
    ESP_ELFSYM_EXPORT(uart_controller_is_open),
    ESP_ELFSYM_EXPORT(trc_isTxAmpduOperational),
    ESP_ELFSYM_EXPORT(os_ext_mempool_register_cb),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(ic_set_beacon_int),
    ESP_ELFSYM_EXPORT(lmacProcessCtsTimeout),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_image_recolor_opa),
    ESP_ELFSYM_EXPORT(coex_rom_data_init),
    ESP_ELFSYM_EXPORT(__subdf3),
    ESP_ELFSYM_EXPORT(lv_menu_section_class),
    ESP_ELFSYM_EXPORT(lv_memmove),
    ESP_ELFSYM_EXPORT(r_sdkconfig_set_opts),
    ESP_ELFSYM_EXPORT(netconn_write_partly),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_send_op_cond),
    ESP_ELFSYM_EXPORT(regdma_link_new_branch_write),
    ESP_ELFSYM_EXPORT(he_send_action_twt_setup),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_sta_aid_local),
    ESP_ELFSYM_EXPORT(offchan_recv_action),
    ESP_ELFSYM_EXPORT(cnx_update_bss),
    ESP_ELFSYM_EXPORT(coex_enable),
    ESP_ELFSYM_EXPORT(lv_event_get_key),
    ESP_ELFSYM_EXPORT(lv_span_get_style),
    ESP_ELFSYM_EXPORT(igmp_tmr),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_send_txpwr_report),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_vs_csa_set),
    ESP_ELFSYM_EXPORT(_ZTVSt17__timepunct_cacheIcE),
    ESP_ELFSYM_EXPORT(ble_svc_gap_device_name_set),
    ESP_ELFSYM_EXPORT(hal_he_set_mmss_and_aid),
    ESP_ELFSYM_EXPORT(scan_set_desChan),
    ESP_ELFSYM_EXPORT(g_dynamic_cs),
    ESP_ELFSYM_EXPORT(lv_text_set_text_vfmt),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_send_cid),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_state_sleep_limit_exceeded_wakeup_disable),
    ESP_ELFSYM_EXPORT(lv_draw_buf_align),
    ESP_ELFSYM_EXPORT(esp_sync_timekeeping_timers),
    ESP_ELFSYM_EXPORT(pp_delete_task),
    ESP_ELFSYM_EXPORT(lv_obj_update_layout),
    ESP_ELFSYM_EXPORT(_Z36app_metadata_is_valid_format_versionRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_info_from_type),
    ESP_ELFSYM_EXPORT(r_priv_sdk_config_options_deinit),
    ESP_ELFSYM_EXPORT(i2s_channel_tune_rate),
    ESP_ELFSYM_EXPORT(netif_set_link_up),
    ESP_ELFSYM_EXPORT(pthread_mutex_timedlock),
    ESP_ELFSYM_EXPORT(periph_rcc_exit),
    ESP_ELFSYM_EXPORT(r_bt_rf_coex_hook_deinit),
    ESP_ELFSYM_EXPORT(lmacTxDone),
    ESP_ELFSYM_EXPORT(pthread_internal_local_storage_destructor_callback),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_filter_addr_remove),
    ESP_ELFSYM_EXPORT(esp_tls_internal_event_tracker_capture),
    ESP_ELFSYM_EXPORT(ble_gatts_connection_broken),
    ESP_ELFSYM_EXPORT(__powidf2),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_skip_rxbuf_alloc_set),
    ESP_ELFSYM_EXPORT(i2c_master_bus_rm_device),
    ESP_ELFSYM_EXPORT(_ZSt27__unguarded_partition_pivotIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEESI_SI_SI_SK_),
    ESP_ELFSYM_EXPORT(esp_unregister_ext_funcs),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_outline_stroke_opa),
    ESP_ELFSYM_EXPORT(lv_area_join),
    ESP_ELFSYM_EXPORT(_ZnwjRKSt9nothrow_t),
    ESP_ELFSYM_EXPORT(lv_roller_set_selected),
    ESP_ELFSYM_EXPORT(sys_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_recycle_in_task),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_profile_default),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_transition_disable_internal),
    ESP_ELFSYM_EXPORT(nan_start),
    ESP_ELFSYM_EXPORT(phy_reset_ckgen),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_rx_pkt_in_on_aux),
    ESP_ELFSYM_EXPORT(ds_hal_set_key_source),
    ESP_ELFSYM_EXPORT(mktime),
    ESP_ELFSYM_EXPORT(xRingbufferCreateWithCaps),
    ESP_ELFSYM_EXPORT(controller_is_inited),
    ESP_ELFSYM_EXPORT(ble_store_read_local_irk),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_samp_limit_switch),
    ESP_ELFSYM_EXPORT(cnx_auth_timeout),
    ESP_ELFSYM_EXPORT(ieee80211_event_report_bsscolor_collision),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_notify),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_conn_cte_req_failed),
    ESP_ELFSYM_EXPORT(mbedtls_aes_crypt_xts),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_ltk_req),
    ESP_ELFSYM_EXPORT(rtc_clk_8m_enable),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_coex_dpc_process_pri),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ext_scan_set_enable),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_optimize_checksum),
    ESP_ELFSYM_EXPORT(hal_mac_tx_set_ppdu),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_pk_sig_alg_from_sig),
    ESP_ELFSYM_EXPORT(spi_bus_lock_acquire_start),
    ESP_ELFSYM_EXPORT(spi_flash_timing_is_tuned),
    ESP_ELFSYM_EXPORT(esp_coex_common_int_restore_wrapper),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_priority_reset),
    ESP_ELFSYM_EXPORT(r_os_mbuf_get_pkthdr),
    ESP_ELFSYM_EXPORT(phy_dac_rate_set),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_disable_owe_trans_internal),
    ESP_ELFSYM_EXPORT(esp_cmac_abort),
    ESP_ELFSYM_EXPORT(ble_gap_terminate),
    ESP_ELFSYM_EXPORT(__bswapsi2),
    ESP_ELFSYM_EXPORT(send_wake_null_timeout_process),
    ESP_ELFSYM_EXPORT(device_set_parent),
    ESP_ELFSYM_EXPORT(_ZStssIcSt11char_traitsIcESaIcEEDTcl21__char_traits_cmp_catIT0_ELi0EEERKNSt7__cxx1112basic_stringIT_S3_T1_EESB_),
    ESP_ELFSYM_EXPORT(wifi_set_promiscuous),
    ESP_ELFSYM_EXPORT(gps_meshtastic_module),
    ESP_ELFSYM_EXPORT(r_ble_lll_convert_timeline_num_to_sync_handle),
    ESP_ELFSYM_EXPORT(pm_beacon_monitor_tbtt_allowed),
    ESP_ELFSYM_EXPORT(uxQueueMessagesWaiting),
    ESP_ELFSYM_EXPORT(lv_tileview_tile_class),
    ESP_ELFSYM_EXPORT(ff_memalloc),
    ESP_ELFSYM_EXPORT(ampdu_alloc_extra_softap_rx_ba_index),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_clear_button_ctrl),
    ESP_ELFSYM_EXPORT(lvgl_sliderbox_get_value),
    ESP_ELFSYM_EXPORT(addba_timeout_process),
    ESP_ELFSYM_EXPORT(lv_label_get_text_selection_end),
    ESP_ELFSYM_EXPORT(vTaskDeleteWithCaps),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_tbtt_auto_period_enable),
    ESP_ELFSYM_EXPORT(heap_caps_realloc_base),
    ESP_ELFSYM_EXPORT(key_mgr_hal_start),
    ESP_ELFSYM_EXPORT(__floatsisf),
    ESP_ELFSYM_EXPORT(esp_vfs_rmdir),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_update_adv_scan_rsp_data),
    ESP_ELFSYM_EXPORT(rtc_gpio_set_direction),
    ESP_ELFSYM_EXPORT(r_os_mbuf_concat),
    ESP_ELFSYM_EXPORT(mbedtls_pem_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_tx_buffer),
    ESP_ELFSYM_EXPORT(memmove),
    ESP_ELFSYM_EXPORT(audio_stream_write),
    ESP_ELFSYM_EXPORT(esp_wifi_set_appie_internal),
    ESP_ELFSYM_EXPORT(ble_store_config_delete),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_swap_xy),
    ESP_ELFSYM_EXPORT(r_put_le16),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_send),
    ESP_ELFSYM_EXPORT(audio_codec_open),
    ESP_ELFSYM_EXPORT(phy_bt_filter_reg),
    ESP_ELFSYM_EXPORT(wDev_ProcessBeaconMemoryRxSuc),
    ESP_ELFSYM_EXPORT(esp_coex_common_semphr_delete_wrapper),
    ESP_ELFSYM_EXPORT(regi2c_ctrl_write_reg),
    ESP_ELFSYM_EXPORT(esp_wifi_ipc_internal),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_probe),
    ESP_ELFSYM_EXPORT(esp_intr_get_cpu),
    ESP_ELFSYM_EXPORT(mmu_hal_unmap_region),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_le_set_phy),
    ESP_ELFSYM_EXPORT(xpt2046_softspi_read_valid_touch),
    ESP_ELFSYM_EXPORT(lv_event_remove),
    ESP_ELFSYM_EXPORT(spi_flash_chip_boya_probe),
    ESP_ELFSYM_EXPORT(ble_att_svr_stop),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_sdspi_mount),
    ESP_ELFSYM_EXPORT(ble_store_key_from_value_cccd),
    ESP_ELFSYM_EXPORT(npl_freertos_event_run),
    ESP_ELFSYM_EXPORT(ble_l2cap_is_mtu_req_sent),
    ESP_ELFSYM_EXPORT(tlsf_malloc),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_link_state),
    ESP_ELFSYM_EXPORT(cnx_do_handoff_internal),
    ESP_ELFSYM_EXPORT(get_iav_key),
    ESP_ELFSYM_EXPORT(rtc_gpio_deinit),
    ESP_ELFSYM_EXPORT(esp_wifi_register_wpa3_ap_cb),
    ESP_ELFSYM_EXPORT(sdspi_host_io_int_wait),
    ESP_ELFSYM_EXPORT(pm_dream_timeout_process),
    ESP_ELFSYM_EXPORT(mem_malloc),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_deletion_end),
    ESP_ELFSYM_EXPORT(esp_tls_conn_destroy),
    ESP_ELFSYM_EXPORT(_ZThn8_NSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED0Ev),
    ESP_ELFSYM_EXPORT(hal_mac_set_bssid),
    ESP_ELFSYM_EXPORT(esp_md5_hash_setup),
    ESP_ELFSYM_EXPORT(wifi_get_scan_results),
    ESP_ELFSYM_EXPORT(r_ble_ll_usecs_to_ticks_round_up),
    ESP_ELFSYM_EXPORT(esp_clk_tree_src_get_freq_hz),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_coex_dpc_update_on_adv_start),
    ESP_ELFSYM_EXPORT(fclose),
    ESP_ELFSYM_EXPORT(ap_free_sta),
    ESP_ELFSYM_EXPORT(__wrap__Unwind_DeleteException),
    ESP_ELFSYM_EXPORT(phy_get_mac_addr),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_alloc_aux_memory),
    ESP_ELFSYM_EXPORT(ble_store_config_deinit),
    ESP_ELFSYM_EXPORT(psa_key_derivation_setup),
    ESP_ELFSYM_EXPORT(esp_coex_common_timer_disarm_wrapper),
    ESP_ELFSYM_EXPORT(lv_draw_buf_has_flag),
    ESP_ELFSYM_EXPORT(lv_obj_get_scrollbar_mode),
    ESP_ELFSYM_EXPORT(esp_netif_is_valid_static_ip),
    ESP_ELFSYM_EXPORT(ble_att_is_response_op),
    ESP_ELFSYM_EXPORT(esp_vfs_unregister),
    ESP_ELFSYM_EXPORT(key_mgr_hal_is_huk_valid),
    ESP_ELFSYM_EXPORT(_ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS_),
    ESP_ELFSYM_EXPORT(lv_draw_task_get_label_dsc),
    ESP_ELFSYM_EXPORT(lora_find_first_registered_device),
    ESP_ELFSYM_EXPORT(lv_array_at),
    ESP_ELFSYM_EXPORT(tlsf_get_pool),
    ESP_ELFSYM_EXPORT(lv_style_set_outline_width),
    ESP_ELFSYM_EXPORT(esp_task_wdt_init),
    ESP_ELFSYM_EXPORT(lv_indev_get_vect),
    ESP_ELFSYM_EXPORT(sntp_set_sync_mode),
    ESP_ELFSYM_EXPORT(ble_hs_tx_data),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_len_proc),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_snap_y),
    ESP_ELFSYM_EXPORT(lv_arc_get_angle_end),
    ESP_ELFSYM_EXPORT(vTaskSetTaskNumber),
    ESP_ELFSYM_EXPORT(ets_timer_arm_us),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_serial),
    ESP_ELFSYM_EXPORT(psa_aead_decrypt),
    ESP_ELFSYM_EXPORT(__ashrdi3),
    ESP_ELFSYM_EXPORT(timer_reset_with_interval),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_insertion_begin),
    ESP_ELFSYM_EXPORT(thread_alloc),
    ESP_ELFSYM_EXPORT(lv_style_prop_lookup_flags),
    ESP_ELFSYM_EXPORT(lv_textarea_set_text_selection),
    ESP_ELFSYM_EXPORT(phy_set_pbus_mem_5g),
    ESP_ELFSYM_EXPORT(esp_netif_attach),
    ESP_ELFSYM_EXPORT(_ZGVNSt8time_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_is_inited_check),
    ESP_ELFSYM_EXPORT(offchan_send_action_tx_status),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_reset),
    ESP_ELFSYM_EXPORT(__find_locale),
    ESP_ELFSYM_EXPORT(lv_group_get_editing),
    ESP_ELFSYM_EXPORT(__locale_mb_cur_max),
    ESP_ELFSYM_EXPORT(ieee80211_alloc_proberesp),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_get_addr_from_ext_adv),
    ESP_ELFSYM_EXPORT(i2c_controller_write_register),
    ESP_ELFSYM_EXPORT(phy_freq_i2c_data_write),
    ESP_ELFSYM_EXPORT(esp_set_scan_ie),
    ESP_ELFSYM_EXPORT(g_scan_chan_list),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_init),
    ESP_ELFSYM_EXPORT(wDev_disable_low_rate),
    ESP_ELFSYM_EXPORT(_ZTVSt11logic_error),
    ESP_ELFSYM_EXPORT(lvgl_get_ui_density),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_prof_password_internal),
    ESP_ELFSYM_EXPORT(lv_obj_destruct),
    ESP_ELFSYM_EXPORT(coex_init),
    ESP_ELFSYM_EXPORT(lv_list_add_button),
    ESP_ELFSYM_EXPORT(fread),
    ESP_ELFSYM_EXPORT(esp_vfs_uart_get_vfs),
    ESP_ELFSYM_EXPORT(esp_sha_hash_compute),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_initiate_tx_pwr_ctrl_req),
    ESP_ELFSYM_EXPORT(tcpip_send_msg_wait_sem),
    ESP_ELFSYM_EXPORT(lodepng_convert),
    ESP_ELFSYM_EXPORT(esp_partition_write_raw),
    ESP_ELFSYM_EXPORT(lv_ll_ins_head),
    ESP_ELFSYM_EXPORT(temperature_sensor_attributes),
    ESP_ELFSYM_EXPORT(r_ble_lll_deinit),
    ESP_ELFSYM_EXPORT(lv_obj_get_coords),
    ESP_ELFSYM_EXPORT(lv_line_set_points),
    ESP_ELFSYM_EXPORT(esp_log_cache_set_level),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_coex_dpc_update_on_event_end),
    ESP_ELFSYM_EXPORT(lv_chart_class),
    ESP_ELFSYM_EXPORT(pm_c5_flash_freq_limit_init_include_func),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_prof_authmode_internal),
    ESP_ELFSYM_EXPORT(wifi_ant_to_ant_type),
    ESP_ELFSYM_EXPORT(esp_log_util_set_cache_enabled_cb),
    ESP_ELFSYM_EXPORT(xQueueGetMutexHolder),
    ESP_ELFSYM_EXPORT(lv_display_get_layer_sys),
    ESP_ELFSYM_EXPORT(wifi_mesh_ps_duty_cycle_get_process),
    ESP_ELFSYM_EXPORT(lodepng_info_init),
    ESP_ELFSYM_EXPORT(wpa_deauthenticate),
    ESP_ELFSYM_EXPORT(pm_coex_customized_wifi_time_slice),
    ESP_ELFSYM_EXPORT(multi_heap_free_size_impl),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_switch_delay_rx_aod_2us),
    ESP_ELFSYM_EXPORT(netif_set_down),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_next_chan),
    ESP_ELFSYM_EXPORT(nvs_set_u32),
    ESP_ELFSYM_EXPORT(DISPLAY_TYPE),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_listen),
    ESP_ELFSYM_EXPORT(adc_apb_periph_claim),
    ESP_ELFSYM_EXPORT(hostap_new_assoc_sta),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_state_sleep_limit),
    ESP_ELFSYM_EXPORT(xPortSysTickHandler),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_get_ticks),
    ESP_ELFSYM_EXPORT(power_supply_set_quick_charge_enabled),
    ESP_ELFSYM_EXPORT(uxTaskPriorityGet),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_sched_priority_increase),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_set_resv_count),
    ESP_ELFSYM_EXPORT(ppInitTxq),
    ESP_ELFSYM_EXPORT(__addsf3),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_recycle_sch_item),
    ESP_ELFSYM_EXPORT(lmacDiscardAgedMSDU),
    ESP_ELFSYM_EXPORT(dhcp_release),
    ESP_ELFSYM_EXPORT(heap_caps_init),
    ESP_ELFSYM_EXPORT(ic_update_modem_sleep_default_params),
    ESP_ELFSYM_EXPORT(__ashldi3),
    ESP_ELFSYM_EXPORT(pm_on_isr_sleep),
    ESP_ELFSYM_EXPORT(__libc_init_array),
    ESP_ELFSYM_EXPORT(haptic_start_playback),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_default_wifi_scan),
    ESP_ELFSYM_EXPORT(r_ble_ll_get_npl_element_info),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_conn_req_rxd),
    ESP_ELFSYM_EXPORT(wifi_hmac_init),
    ESP_ELFSYM_EXPORT(esp_vfs_read),
    ESP_ELFSYM_EXPORT(ff_diskio_get_drive),
    ESP_ELFSYM_EXPORT(_ZTVSt20bad_array_new_length),
    ESP_ELFSYM_EXPORT(ledc_hal_clear_left_off_fade_param),
    ESP_ELFSYM_EXPORT(multi_heap_aligned_alloc_offs),
    ESP_ELFSYM_EXPORT(r_ble_ll_mtrack_initialized_set),
    ESP_ELFSYM_EXPORT(sae_pk_base32_decode),
    ESP_ELFSYM_EXPORT(ble_sm_timer),
    ESP_ELFSYM_EXPORT(lvgl_get_statusbar_icon_font_height),
    ESP_ELFSYM_EXPORT(_Z16init_mtk_pa1616sP6Device),
    ESP_ELFSYM_EXPORT(__fixdfsi),
    ESP_ELFSYM_EXPORT(scan_fill_wps_scan_ie),
    ESP_ELFSYM_EXPORT(sdspi_host_get_real_freq),
    ESP_ELFSYM_EXPORT(ftm_is_responder_supported),
    ESP_ELFSYM_EXPORT(r_hal_timer_env_init),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_prep_write_rsp),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_tag),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_alloc_memory),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_connecting),
    ESP_ELFSYM_EXPORT(psa_hash_finish),
    ESP_ELFSYM_EXPORT(r_ble_bb_cte_sample_limit_get),
    ESP_ELFSYM_EXPORT(mbedtls_pk_can_do_psa),
    ESP_ELFSYM_EXPORT(crypto_bignum_exptmod),
    ESP_ELFSYM_EXPORT(esp_mmu_paddr_find_caps),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_init),
    ESP_ELFSYM_EXPORT(r_ble_log_internal_x0),
    ESP_ELFSYM_EXPORT(hostapd_send_eapol),
    ESP_ELFSYM_EXPORT(ant_dft_cfg),
    ESP_ELFSYM_EXPORT(i2s_periph_signal),
    ESP_ELFSYM_EXPORT(wifi_deinit_in_caller_task),
    ESP_ELFSYM_EXPORT(ble_gap_notify_tx_event),
    ESP_ELFSYM_EXPORT(thread_get_current),
    ESP_ELFSYM_EXPORT(lv_image_header_cache_init),
    ESP_ELFSYM_EXPORT(pp_deattach),
    ESP_ELFSYM_EXPORT(ble_gatts_send_next_indicate),
    ESP_ELFSYM_EXPORT(xTaskGenericNotify),
    ESP_ELFSYM_EXPORT(__atomic_load_8),
    ESP_ELFSYM_EXPORT(phy_i2c_readReg),
    ESP_ELFSYM_EXPORT(dhcps_delete),
    ESP_ELFSYM_EXPORT(ble_svc_sps_reset),
    ESP_ELFSYM_EXPORT(_Z15is_pin_invertedP14GpioDescriptor),
    ESP_ELFSYM_EXPORT(phy_module_has_clock_bits),
    ESP_ELFSYM_EXPORT(lv_draw_sw_letter),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_env_init),
    ESP_ELFSYM_EXPORT(rcGet11NHighestRateIdx),
    ESP_ELFSYM_EXPORT(esp_transport_read),
    ESP_ELFSYM_EXPORT(esp_panic_handler_disable_timg_wdts),
    ESP_ELFSYM_EXPORT(ble_hs_misc_peer_addr_type_to_id),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_parse_sig_alg_ext),
    ESP_ELFSYM_EXPORT(i2c_controller_read_register),
    ESP_ELFSYM_EXPORT(ieee80211_decap_amsdu),
    ESP_ELFSYM_EXPORT(sdmmc_init_card_hs_mode),
    ESP_ELFSYM_EXPORT(lv_style_set_border_post),
    ESP_ELFSYM_EXPORT(ble_att_chan_mtu),
    ESP_ELFSYM_EXPORT(spi_device_get_trans_result),
    ESP_ELFSYM_EXPORT(itwt_probe_rc_tx_cb),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_env_init),
    ESP_ELFSYM_EXPORT(mbedtls_psa_cipher_encrypt_setup),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_mount_initialized),
    ESP_ELFSYM_EXPORT(wDev_remove_KeyEntry),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx119money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(rc4_skip),
    ESP_ELFSYM_EXPORT(r_advertise_filter_init),
    ESP_ELFSYM_EXPORT(i2c_param_config),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_get_local_rpa),
    ESP_ELFSYM_EXPORT(r_ble_scan_advertise_filter_init),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_color_to_rgb888),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_set_aux_data_at),
    ESP_ELFSYM_EXPORT(wpa_get_ntp_timestamp),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_default_wifi_conn),
    ESP_ELFSYM_EXPORT(set_client_config),
    ESP_ELFSYM_EXPORT(noise_check_loop),
    ESP_ELFSYM_EXPORT(tcpip_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_utils_csa2_prng),
    ESP_ELFSYM_EXPORT(prvTaskCreateDynamicPinnedToCoreWithCaps),
    ESP_ELFSYM_EXPORT(device_listener_notify),
    ESP_ELFSYM_EXPORT(fopen),
    ESP_ELFSYM_EXPORT(vsprintf),
    ESP_ELFSYM_EXPORT(ip_addr_any_type),
    ESP_ELFSYM_EXPORT(regdma_link_new_continuous),
    ESP_ELFSYM_EXPORT(esp_supplicant_unset_all_appie),
    ESP_ELFSYM_EXPORT(ieee80211_mt_key_clear_mask),
    ESP_ELFSYM_EXPORT(cache_hal_freeze),
    ESP_ELFSYM_EXPORT(pbuf_copy_partial_pbuf),
    ESP_ELFSYM_EXPORT(cs_send_to_ctrl_sock),
    ESP_ELFSYM_EXPORT(crypto_ec_get_b),
    ESP_ELFSYM_EXPORT(tcp_accept),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_rxbuffer_recycle_empty_hdr),
    ESP_ELFSYM_EXPORT(esp_sha1_driver_clone),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_ciphersuite_from_id),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_move),
    ESP_ELFSYM_EXPORT(wDev_isNANPktInValidSlot),
    ESP_ELFSYM_EXPORT(coex_schm_bt_a2dp_wifi_connecting),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY4),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_min_chan),
    ESP_ELFSYM_EXPORT(lv_obj_set_local_style_prop),
    ESP_ELFSYM_EXPORT(modem_clock_hal_enable_ble_rtc_timer_clock),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_aux_conn_rsp_pdu_make),
    ESP_ELFSYM_EXPORT(lv_rb_init),
    ESP_ELFSYM_EXPORT(scan_start),
    ESP_ELFSYM_EXPORT(ble_store_key_from_value_rpa_rec),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_abort_enable),
    ESP_ELFSYM_EXPORT(cnx_rc_search),
    ESP_ELFSYM_EXPORT(bundle_put_int64),
    ESP_ELFSYM_EXPORT(ieee80211_set_tx_pti),
    ESP_ELFSYM_EXPORT(ieee80211_hostapd_beacon_txcb),
    ESP_ELFSYM_EXPORT(_ZTVSt9bad_alloc),
    ESP_ELFSYM_EXPORT(raw_connect),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_conf_renegotiation),
    ESP_ELFSYM_EXPORT(lv_anim_set_exec_cb),
    ESP_ELFSYM_EXPORT(esp_http_client_set_header),
    ESP_ELFSYM_EXPORT(esp_transport_get_errno),
    ESP_ELFSYM_EXPORT(lv_textarea_get_text),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_module_reset),
    ESP_ELFSYM_EXPORT(__d_vfprintf),
    ESP_ELFSYM_EXPORT(r_ble_ll_env_deinit),
    ESP_ELFSYM_EXPORT(esp_partition_find_first),
    ESP_ELFSYM_EXPORT(POWER_SUPPLY_TYPE),
    ESP_ELFSYM_EXPORT(ble_hs_conn_find_assert),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_list_details_dump),
    ESP_ELFSYM_EXPORT(lv_theme_get_from_obj),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_gen_privkey),
    ESP_ELFSYM_EXPORT(lv_screen_load_anim),
    ESP_ELFSYM_EXPORT(chm_cancel_op),
    ESP_ELFSYM_EXPORT(phy_ant_need_update),
    ESP_ELFSYM_EXPORT(wdev_set_promis),
    ESP_ELFSYM_EXPORT(file_system_get_owner),
    ESP_ELFSYM_EXPORT(spi_hal_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_set_adv_data),
    ESP_ELFSYM_EXPORT(tt_timezone_set_format_24_hour),
    ESP_ELFSYM_EXPORT(xQueueCreateCountingSemaphoreStatic),
    ESP_ELFSYM_EXPORT(_ZTVSt7codecvtIDic10_mbstate_tE),
    ESP_ELFSYM_EXPORT(rcGetHighestRateIdx),
    ESP_ELFSYM_EXPORT(log),
    ESP_ELFSYM_EXPORT(ble_hs_hci_cmd_send_buf),
    ESP_ELFSYM_EXPORT(phy_wifi_enable_set),
    ESP_ELFSYM_EXPORT(nvs_flash_erase),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_err),
    ESP_ELFSYM_EXPORT(ppRegisterRxCallback),
    ESP_ELFSYM_EXPORT(ip6_route),
    ESP_ELFSYM_EXPORT(TRC_AMPDU_PER_DOWN_THRESHOLD),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_periodic_ind),
    ESP_ELFSYM_EXPORT(pm_set_sleep_type),
    ESP_ELFSYM_EXPORT(hid_gamepad_input_handle),
    ESP_ELFSYM_EXPORT(ieee80211_node_pwrsave),
    ESP_ELFSYM_EXPORT(tcp_recv_null),
    ESP_ELFSYM_EXPORT(bluetooth_scan_stop),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_memblock_put_cb),
    ESP_ELFSYM_EXPORT(lv_area_is_point_on),
    ESP_ELFSYM_EXPORT(lv_draw_sw_layer),
    ESP_ELFSYM_EXPORT(hal_set_pri20_chan_offset),
    ESP_ELFSYM_EXPORT(crypto_ec_point_mul),
    ESP_ELFSYM_EXPORT(_ZSt19__throw_ios_failurePKc),
    ESP_ELFSYM_EXPORT(scan_validate_owe_scenarios),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_finish_handshake_msg),
    ESP_ELFSYM_EXPORT(spi_flash_hal_init),
    ESP_ELFSYM_EXPORT(ecc_hal_write_verify_param),
    ESP_ELFSYM_EXPORT(ledc_set_fade_step_and_start),
    ESP_ELFSYM_EXPORT(lv_obj_area_is_visible),
    ESP_ELFSYM_EXPORT(mbedtls_oid_get_oid_by_md),
    ESP_ELFSYM_EXPORT(lv_timer_enable),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_disp_on_off),
    ESP_ELFSYM_EXPORT(lv_color32_make),
    ESP_ELFSYM_EXPORT(lv_rand_set_seed),
    ESP_ELFSYM_EXPORT(_ZSt3hexRSt8ios_base),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetINSt7__cxx118numpunctIcEEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(wifi_softap_max_support_num),
    ESP_ELFSYM_EXPORT(hal_he_disable_obss_narrow_bw_ru),
    ESP_ELFSYM_EXPORT(get_vfs_for_path),
    ESP_ELFSYM_EXPORT(rtc_dig_clk8m_enable),
    ESP_ELFSYM_EXPORT(wifi_get_bss_color),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_origin_update),
    ESP_ELFSYM_EXPORT(_ZTV8WL_Flash),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_deinit),
    ESP_ELFSYM_EXPORT(esp_aes_cipher_decrypt),
    ESP_ELFSYM_EXPORT(os_random),
    ESP_ELFSYM_EXPORT(wDevMacSleep),
    ESP_ELFSYM_EXPORT(uECC_generate_random_int),
    ESP_ELFSYM_EXPORT(rcGet11GHighestRateIdx),
    ESP_ELFSYM_EXPORT(lv_image_decoder_set_close_cb),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_parse_ext_hdr),
    ESP_ELFSYM_EXPORT(uECC_vli_modSub),
    ESP_ELFSYM_EXPORT(esp_tls_get_platform_time),
    ESP_ELFSYM_EXPORT(r_get_le64),
    ESP_ELFSYM_EXPORT(lv_binfont_create),
    ESP_ELFSYM_EXPORT(lv_timer_handler_set_resume_cb),
    ESP_ELFSYM_EXPORT(lv_led_class),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_set_gap),
    ESP_ELFSYM_EXPORT(_uart_set_pin6),
    ESP_ELFSYM_EXPORT(xTimerGetExpiryTime),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_set_max_buffer_number),
    ESP_ELFSYM_EXPORT(wl_sector_size),
    ESP_ELFSYM_EXPORT(rc_get_G6M_sched),
    ESP_ELFSYM_EXPORT(gpio_descriptor_enable_interrupt),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_phy_update_ind),
    ESP_ELFSYM_EXPORT(esp_transport_get_error_handle),
    ESP_ELFSYM_EXPORT(_ZSt17__verify_groupingPKcjRKSs),
    ESP_ELFSYM_EXPORT(esp_send_assoc_resp),
    ESP_ELFSYM_EXPORT(acd_netif_ip_addr_changed),
    ESP_ELFSYM_EXPORT(_ZTISt12system_error),
    ESP_ELFSYM_EXPORT(uart_controller_get_available),
    ESP_ELFSYM_EXPORT(lwip_freeaddrinfo),
    ESP_ELFSYM_EXPORT(chmod),
    ESP_ELFSYM_EXPORT(_ZTVSt15_Sp_counted_ptrIPN2tt7service5audio12AudioServiceELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(pm_keep_alive_timeout_process),
    ESP_ELFSYM_EXPORT(ble_hs_mbuf_att_pkt),
    ESP_ELFSYM_EXPORT(lv_arc_set_angles),
    ESP_ELFSYM_EXPORT(pm_coex_update_rx_beacon_pti),
    ESP_ELFSYM_EXPORT(esp_vfs_select_triggered),
    ESP_ELFSYM_EXPORT(cache_hal_is_cache_enabled),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_local_addr_rd),
    ESP_ELFSYM_EXPORT(lv_color_16_16_mix),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_rxbuf_direct_alloc),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_env_init),
    ESP_ELFSYM_EXPORT(modem_clock_deselect_all_module_lp_clock_source),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_cancel_complete_event),
    ESP_ELFSYM_EXPORT(__bufio_close),
    ESP_ELFSYM_EXPORT(lora_add_rx_callback),
    ESP_ELFSYM_EXPORT(esp_vApplicationTickHook),
    ESP_ELFSYM_EXPORT(r_ble_scan_advertise_filter_destroy),
    ESP_ELFSYM_EXPORT(ieee80211_parse_hecap),
    ESP_ELFSYM_EXPORT(ble_att_svr_ticks_until_tmo),
    ESP_ELFSYM_EXPORT(r_ble_hw_whitelist_clear),
    ESP_ELFSYM_EXPORT(ieee80211_parse_htc),
    ESP_ELFSYM_EXPORT(esp_elf_free),
    ESP_ELFSYM_EXPORT(esp_rom_vprintf),
    ESP_ELFSYM_EXPORT(ble_sm_alg_gen_key_pair),
    ESP_ELFSYM_EXPORT(gps_settings_remove_configuration_at),
    ESP_ELFSYM_EXPORT(lvgl_port_lock),
    ESP_ELFSYM_EXPORT(ic_set_ac_param),
    ESP_ELFSYM_EXPORT(esp_crt_bundle_attach),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_clear_all),
    ESP_ELFSYM_EXPORT(_do_wifi_start),
    ESP_ELFSYM_EXPORT(lv_list_text_class),
    ESP_ELFSYM_EXPORT(netconn_listen_with_backlog),
    ESP_ELFSYM_EXPORT(test_nonmimo_update_user_info),
    ESP_ELFSYM_EXPORT(cs_create_ctrl_sock),
    ESP_ELFSYM_EXPORT(bluetooth_start_advertising),
    ESP_ELFSYM_EXPORT(ble_store_config_read),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_force_sync_enable),
    ESP_ELFSYM_EXPORT(lv_obj_is_valid),
    ESP_ELFSYM_EXPORT(preferences_opt_string),
    ESP_ELFSYM_EXPORT(lv_obj_set_scrollbar_mode),
    ESP_ELFSYM_EXPORT(__kernel_tan),
    ESP_ELFSYM_EXPORT(_ZTVSt7codecvtIwc10_mbstate_tE),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_update_new_phy),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_KEY_PURPOSE_0),
    ESP_ELFSYM_EXPORT(hal_mac_get_txq_complete),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand_prand_get),
    ESP_ELFSYM_EXPORT(mpi_hal_write_m_prime),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_send_status),
    ESP_ELFSYM_EXPORT(i2c_controller_read),
    ESP_ELFSYM_EXPORT(hal_set_sta_beacon_filter),
    ESP_ELFSYM_EXPORT(sha_hal_read_digest),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_phy_update),
    ESP_ELFSYM_EXPORT(pm_deattach),
    ESP_ELFSYM_EXPORT(lv_calendar_class),
    ESP_ELFSYM_EXPORT(netif_invoke_ext_callback),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_recycle_sch_item),
    ESP_ELFSYM_EXPORT(phy_set_rx_pbus_freq),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_rx_process),
    ESP_ELFSYM_EXPORT(wifi_he_get_hetb_tid_bitmap),
    ESP_ELFSYM_EXPORT(hal_he_set_bcast_ru),
    ESP_ELFSYM_EXPORT(uart_hal_set_rx_timeout),
    ESP_ELFSYM_EXPORT(esp_netif_get_flags),
    ESP_ELFSYM_EXPORT(lv_group_remove_obj),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_pwr_ctrl_resp),
    ESP_ELFSYM_EXPORT(ble_hs_conn_find_by_addr),
    ESP_ELFSYM_EXPORT(psa_verify_hash_builtin),
    ESP_ELFSYM_EXPORT(cnx_node_join),
    ESP_ELFSYM_EXPORT(uECC_vli_cmp),
    ESP_ELFSYM_EXPORT(vTaskSetThreadLocalStoragePointerAndDelCallback),
    ESP_ELFSYM_EXPORT(ble_gap_rx_param_req),
    ESP_ELFSYM_EXPORT(psa_unregister_read),
    ESP_ELFSYM_EXPORT(mbedtls_base64_decode),
    ESP_ELFSYM_EXPORT(r_ble_hw_get_static_addr),
    ESP_ELFSYM_EXPORT(ceilf),
    ESP_ELFSYM_EXPORT(pmu_hp_system_init),
    ESP_ELFSYM_EXPORT(_ZGVNSt9money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(r_ble_phy_sleep_related_etm_check),
    ESP_ELFSYM_EXPORT(ets_isr_mask),
    ESP_ELFSYM_EXPORT(spi_flash_hal_gpspi_supports_direct_read),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_create),
    ESP_ELFSYM_EXPORT(file_system_unmount),
    ESP_ELFSYM_EXPORT(scan_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(esp_vfs_unlink),
    ESP_ELFSYM_EXPORT(hal_crypto_enable),
    ESP_ELFSYM_EXPORT(pm_update_next_tbtt),
    ESP_ELFSYM_EXPORT(map_wifi_config_sae_pwe_to_supp),
    ESP_ELFSYM_EXPORT(ppCheckTxHEAMPDUlength),
    ESP_ELFSYM_EXPORT(g_wifi_menuconfig),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_KEY_PURPOSE_4),
    ESP_ELFSYM_EXPORT(_ZSt16__introsort_loopIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEEiNS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SI_SK_T1_),
    ESP_ELFSYM_EXPORT(_ZSt7nothrow),
    ESP_ELFSYM_EXPORT(resend_eapol_handle),
    ESP_ELFSYM_EXPORT(r_ble_lll_linkstate_cte_config_init),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_sta_listen_interval),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_verify),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_max_chan),
    ESP_ELFSYM_EXPORT(_Z25ble_spp_init_gatt_handlesP6Device),
    ESP_ELFSYM_EXPORT(esp_wifi_set_storage),
    ESP_ELFSYM_EXPORT(g_offchan_packet_lifetime),
    ESP_ELFSYM_EXPORT(adv_stack_disable),
    ESP_ELFSYM_EXPORT(pm_is_sleeping),
    ESP_ELFSYM_EXPORT(s_vht_cap_bmfmee_nsts),
    ESP_ELFSYM_EXPORT(rc5G11AXSchedTbl),
    ESP_ELFSYM_EXPORT(esp_startup_start_app),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_class),
    ESP_ELFSYM_EXPORT(esp_wifi_get_event_mask),
    ESP_ELFSYM_EXPORT(esp_http_client_set_redirection),
    ESP_ELFSYM_EXPORT(esp_crypto_ecc_enable_periph_clk),
    ESP_ELFSYM_EXPORT(mbedtls_md_hmac_update),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_version),
    ESP_ELFSYM_EXPORT(ff_diskio_register),
    ESP_ELFSYM_EXPORT(spi_bus_lock_bg_entry),
    ESP_ELFSYM_EXPORT(GPIO_CONTROLLER_TYPE),
    ESP_ELFSYM_EXPORT(cnx_connect_next_ap_timeout),
    ESP_ELFSYM_EXPORT(vTaskSuspendAll),
    ESP_ELFSYM_EXPORT(sleep_retention_find_link_by_id),
    ESP_ELFSYM_EXPORT(wDev_Reset_TBTT),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_encrypt_chg),
    ESP_ELFSYM_EXPORT(bt_rf_coex_cfg_cb),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_CH0_ATTEN0_INITCODE_DIFF),
    ESP_ELFSYM_EXPORT(gdma_hal_is_tx_link_switch_event_supported),
    ESP_ELFSYM_EXPORT(wifi_module_disable),
    ESP_ELFSYM_EXPORT(driver_destruct),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_get_sec_pdu_len),
    ESP_ELFSYM_EXPORT(trc_get_80211_tx_rate_config),
    ESP_ELFSYM_EXPORT(itwt_setup_dwell_timeout_fn),
    ESP_ELFSYM_EXPORT(wifi_wpa2_is_started),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_mode_from_transform),
    ESP_ELFSYM_EXPORT(wdev_csi_rx_process),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_disable_wpa2_authmode_internal),
    ESP_ELFSYM_EXPORT(ble_hs_deinit),
    ESP_ELFSYM_EXPORT(_ZGVNSt7num_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(pwm_set_inverted),
    ESP_ELFSYM_EXPORT(r_os_mbuf_copydata),
    ESP_ELFSYM_EXPORT(npl_freertos_get_current_task_id),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_exec_write),
    ESP_ELFSYM_EXPORT(esp_tls_get_conn_sockfd),
    ESP_ELFSYM_EXPORT(lv_textarea_cursor_up),
    ESP_ELFSYM_EXPORT(pm_get_tx_blocks_retention_mask),
    ESP_ELFSYM_EXPORT(ble_store_util_delete_all),
    ESP_ELFSYM_EXPORT(r_os_mbuf_appendfrom),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_exec_write_rsp),
    ESP_ELFSYM_EXPORT(netconn_free),
    ESP_ELFSYM_EXPORT(xEventGroupCreate),
    ESP_ELFSYM_EXPORT(ic_register_michael_mic_failure_cb),
    ESP_ELFSYM_EXPORT(ieee80211_assoc_req_construct),
    ESP_ELFSYM_EXPORT(esp_test_set_rx_error_occurs),
    ESP_ELFSYM_EXPORT(sys_now),
    ESP_ELFSYM_EXPORT(mac_tx_get_rts_rate),
    ESP_ELFSYM_EXPORT(mbrtowc),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_periodic_sch_cnt_required),
    ESP_ELFSYM_EXPORT(esp_wifi_80211_tx),
    ESP_ELFSYM_EXPORT(usb_hid_device_stop),
    ESP_ELFSYM_EXPORT(towlower),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_npl_deinit),
    ESP_ELFSYM_EXPORT(lv_obj_is_group_def),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_driver_strength),
    ESP_ELFSYM_EXPORT(os_mempool_clear),
    ESP_ELFSYM_EXPORT(lv_point_swap),
    ESP_ELFSYM_EXPORT(ieee80211_rfid_locp_recv),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSt14basic_ofstreamIcSt11char_traitsIcEED0Ev),
    ESP_ELFSYM_EXPORT(sys_mbox_free),
    ESP_ELFSYM_EXPORT(raw_bind_netif),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_copy_into_mbuf),
    ESP_ELFSYM_EXPORT(rcUpdateAckSnr),
    ESP_ELFSYM_EXPORT(adiList_stack_listClear),
    ESP_ELFSYM_EXPORT(ieee80211_get_nvs_he_dcm_max_constellation_tx),
    ESP_ELFSYM_EXPORT(__wrap_lv_obj_set_flex_flow),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_read_rsp),
    ESP_ELFSYM_EXPORT(vTaskPlaceOnEventList),
    ESP_ELFSYM_EXPORT(preferences_has_bool),
    ESP_ELFSYM_EXPORT(_ZSt13__int_to_charIwyEiPT_T0_PKS0_St13_Ios_Fmtflagsb),
    ESP_ELFSYM_EXPORT(pp_register_timer_cb),
    ESP_ELFSYM_EXPORT(crypto_bignum_rshift),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_pad_v15_unpad),
    ESP_ELFSYM_EXPORT(bt_rf_coex_hooks_p),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_io_tx_color),
    ESP_ELFSYM_EXPORT(lv_obj_get_group),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_info_from_values),
    ESP_ELFSYM_EXPORT(pm_is_in_wifi_slice_threshold),
    ESP_ELFSYM_EXPORT(lv_pct),
    ESP_ELFSYM_EXPORT(esp_test_get_tx_tb_statistics),
    ESP_ELFSYM_EXPORT(coex_core_release),
    ESP_ELFSYM_EXPORT(wpa_crypto_funcs_init),
    ESP_ELFSYM_EXPORT(esp_cpu_set_breakpoint),
    ESP_ELFSYM_EXPORT(_ZTTSo),
    ESP_ELFSYM_EXPORT(spi_flash_common_write_status_16b_wrsr),
    ESP_ELFSYM_EXPORT(esp_libc_include_strcpy_impl),
    ESP_ELFSYM_EXPORT(rc11GSchedTbl),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_event_delete_and_reschedule),
    ESP_ELFSYM_EXPORT(hci_transport_host_acl_tx),
    ESP_ELFSYM_EXPORT(wpa_receive),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_init),
    ESP_ELFSYM_EXPORT(lv_refr_get_disp_refreshing),
    ESP_ELFSYM_EXPORT(ble_ll_conn_env_p),
    ESP_ELFSYM_EXPORT(lv_pct_to_px),
    ESP_ELFSYM_EXPORT(sae_process_commit),
    ESP_ELFSYM_EXPORT(lv_obj_set_grid_cell),
    ESP_ELFSYM_EXPORT(ppClearRxFragment),
    ESP_ELFSYM_EXPORT(rtc_clk_freq_cal),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_module_reset),
    ESP_ELFSYM_EXPORT(bt_bb_tx_cca_fifo_empty),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_stop),
    ESP_ELFSYM_EXPORT(esp32_gpio_is_mspi_pin),
    ESP_ELFSYM_EXPORT(xQueueGenericSend),
    ESP_ELFSYM_EXPORT(spi_flash_hal_erase_sector),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_ciphersuite_sig_alg),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_established),
    ESP_ELFSYM_EXPORT(r_ble_ll_rxpdu_alloc),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_is_active),
    ESP_ELFSYM_EXPORT(bt_bb_coex_config),
    ESP_ELFSYM_EXPORT(esp_event_handler_unregister_with_internal),
    ESP_ELFSYM_EXPORT(sys_arch_protect),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_deinit),
    ESP_ELFSYM_EXPORT(r_npl_funcs),
    ESP_ELFSYM_EXPORT(ble_store_write_peer_sec),
    ESP_ELFSYM_EXPORT(uart_hal_init),
    ESP_ELFSYM_EXPORT(g_ble_ll_supp_cmds_ro),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_clear),
    ESP_ELFSYM_EXPORT(cnx_add_to_blacklist),
    ESP_ELFSYM_EXPORT(__log10Pow5),
    ESP_ELFSYM_EXPORT(esp_coex_common_env_is_chip_wrapper),
    ESP_ELFSYM_EXPORT(ble_gatts_stop),
    ESP_ELFSYM_EXPORT(nan_is_in_dw),
    ESP_ELFSYM_EXPORT(esp_http_client_read_response),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_reset),
    ESP_ELFSYM_EXPORT(esp_fast_psk),
    ESP_ELFSYM_EXPORT(pm_coex_separate_connectionless_window),
    ESP_ELFSYM_EXPORT(hal_he_set_bss_color),
    ESP_ELFSYM_EXPORT(ip6_addr_any),
    ESP_ELFSYM_EXPORT(ic_txq_empty),
    ESP_ELFSYM_EXPORT(vht_set_snd_ch_cfg),
    ESP_ELFSYM_EXPORT(mbedtls_psa_mac_update),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_lt_ct),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_y),
    ESP_ELFSYM_EXPORT(dragonfly_generate_scalar),
    ESP_ELFSYM_EXPORT(phy_i2c_writeReg_Mask),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_get_intr_status_reg),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_soc_wakeup_enable),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_set_bit),
    ESP_ELFSYM_EXPORT(hal_get_tsf_time),
    ESP_ELFSYM_EXPORT(pwr_hal_clear_mac_modem_beacon_miss_intr_filter),
    ESP_ELFSYM_EXPORT(_ZGVNSt7num_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(time),
    ESP_ELFSYM_EXPORT(camera_close),
    ESP_ELFSYM_EXPORT(_ZTVSt23__codecvt_abstract_baseIDsc10_mbstate_tE),
    ESP_ELFSYM_EXPORT(__flockfile_init),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_outline_stroke_color),
    ESP_ELFSYM_EXPORT(hal_mac_tx_set_cca),
    ESP_ELFSYM_EXPORT(ieee80211_encap_esfbuf),
    ESP_ELFSYM_EXPORT(__wrap__Unwind_RaiseException),
    ESP_ELFSYM_EXPORT(_ZTVSt10moneypunctIwLb1EE),
    ESP_ELFSYM_EXPORT(phy_get_freq_init),
    ESP_ELFSYM_EXPORT(lv_keyboard_create),
    ESP_ELFSYM_EXPORT(rc11NSchedTbl),
    ESP_ELFSYM_EXPORT(timer_start),
    ESP_ELFSYM_EXPORT(nan_get_clust_id),
    ESP_ELFSYM_EXPORT(_ZTVSt15_Sp_counted_ptrIPN2tt7service7rtctime14RtcTimeServiceELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_config_sae_pk_internal),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_set_weight),
    ESP_ELFSYM_EXPORT(audio_codec_close),
    ESP_ELFSYM_EXPORT(ble_sm_enc_key_refresh_rx),
    ESP_ELFSYM_EXPORT(r_ble_phy_get_min_txpwr_dbm),
    ESP_ELFSYM_EXPORT(ble_att_is_request_op),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY0),
    ESP_ELFSYM_EXPORT(r_ble_lll_get_txed_buffer),
    ESP_ELFSYM_EXPORT(ble_uuid_to_mbuf),
    ESP_ELFSYM_EXPORT(he_twt_teardown_post_event),
    ESP_ELFSYM_EXPORT(xTaskPriorityDisinherit),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_read_ocr),
    ESP_ELFSYM_EXPORT(ble_sm_enc_change_rx),
    ESP_ELFSYM_EXPORT(lwip_fcntl),
    ESP_ELFSYM_EXPORT(gpio_get_level),
    ESP_ELFSYM_EXPORT(_ZZNSt19_Sp_make_shared_tag5_S_tiEvE5__tag),
    ESP_ELFSYM_EXPORT(owe_process_assoc_resp),
    ESP_ELFSYM_EXPORT(phy_init_data),
    ESP_ELFSYM_EXPORT(lv_obj_get_child_count_by_type),
    ESP_ELFSYM_EXPORT(esp_mbedtls_write),
    ESP_ELFSYM_EXPORT(wifi_nan_set_config_local),
    ESP_ELFSYM_EXPORT(__wrap_lv_list_add_button),
    ESP_ELFSYM_EXPORT(lv_obj_scroll_by_raw),
    ESP_ELFSYM_EXPORT(coex_core_status_get),
    ESP_ELFSYM_EXPORT(trc_SetTxAmpduState),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_txbuf_adi_field_update),
    ESP_ELFSYM_EXPORT(pwm_enable),
    ESP_ELFSYM_EXPORT(in_rssi_adjust),
    ESP_ELFSYM_EXPORT(ieee80211_add_countryie),
    ESP_ELFSYM_EXPORT(fgetc),
    ESP_ELFSYM_EXPORT(iswxdigit),
    ESP_ELFSYM_EXPORT(phy_i2cmst_reg_init),
    ESP_ELFSYM_EXPORT(phy_agc_reg_init_new),
    ESP_ELFSYM_EXPORT(sae_write_commit),
    ESP_ELFSYM_EXPORT(pmksa_cache_get_opportunistic),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_cmp_int),
    ESP_ELFSYM_EXPORT(display_get_mirror_x),
    ESP_ELFSYM_EXPORT(uart_flush),
    ESP_ELFSYM_EXPORT(raw_new),
    ESP_ELFSYM_EXPORT(stderr),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_connecting),
    ESP_ELFSYM_EXPORT(cache_hal_suspend),
    ESP_ELFSYM_EXPORT(tcp_active_pcbs),
    ESP_ELFSYM_EXPORT(s_fix_rate_mask),
    ESP_ELFSYM_EXPORT(ppCheckIsConnTraffic),
    ESP_ELFSYM_EXPORT(I8080_CONTROLLER_TYPE),
    ESP_ELFSYM_EXPORT(wDev_Rxbuf_Deinit),
    ESP_ELFSYM_EXPORT(minitar_read_contents_to_file),
    ESP_ELFSYM_EXPORT(pwr_hal_get_mac_modem_state_sleep_limit_exceeded_status),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_pmf_enabled),
    ESP_ELFSYM_EXPORT(g_wdev_csi_rx),
    ESP_ELFSYM_EXPORT(i2s_init_dma_intr),
    ESP_ELFSYM_EXPORT(r_ble_ll_is_addr_empty),
    ESP_ELFSYM_EXPORT(netif_init),
    ESP_ELFSYM_EXPORT(esp32_grove_driver),
    ESP_ELFSYM_EXPORT(_ZSt9uppercaseRSt8ios_base),
    ESP_ELFSYM_EXPORT(dns_gethostbyname),
    ESP_ELFSYM_EXPORT(r_ble_phy_freq_to_chan),
    ESP_ELFSYM_EXPORT(hal_sniffer_disable),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_resume_aux_scan),
    ESP_ELFSYM_EXPORT(he_data_bits_per_sym),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_init),
    ESP_ELFSYM_EXPORT(ic_deinit),
    ESP_ELFSYM_EXPORT(crypto_ec_prime_len),
    ESP_ELFSYM_EXPORT(esp_sha_hash_finish),
    ESP_ELFSYM_EXPORT(wpa_parse_kde_ies),
    ESP_ELFSYM_EXPORT(etharp_output),
    ESP_ELFSYM_EXPORT(aes_hal_transform_dma_start),
    ESP_ELFSYM_EXPORT(ieee80211_mlme_connect_bss),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx119money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(ble_l2cap_prepend_hdr),
    ESP_ELFSYM_EXPORT(event_group_clear),
    ESP_ELFSYM_EXPORT(uart_enable_tx_intr),
    ESP_ELFSYM_EXPORT(hal_vht_enable_bwsignaling_rts),
    ESP_ELFSYM_EXPORT(cnx_rc_update_state_metric),
    ESP_ELFSYM_EXPORT(_g_esp_netif_netstack_default_wifi_sta),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_wake_up_ahead_check),
    ESP_ELFSYM_EXPORT(ccmp_encap),
    ESP_ELFSYM_EXPORT(wifi_station_start),
    ESP_ELFSYM_EXPORT(ble_hs_log_flat_buf),
    ESP_ELFSYM_EXPORT(esp_http_client_prepare),
    ESP_ELFSYM_EXPORT(ieee80211_psq_send_one_pkt),
    ESP_ELFSYM_EXPORT(_vector_table),
    ESP_ELFSYM_EXPORT(phy_pa_dc_code_set),
    ESP_ELFSYM_EXPORT(crypto_bignum_sqrmod),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_calculate_aux_duration),
    ESP_ELFSYM_EXPORT(sdmmc_enable_hs_mode_and_check),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_phy_req),
    ESP_ELFSYM_EXPORT(lv_obj_is_editable),
    ESP_ELFSYM_EXPORT(netconn_bind),
    ESP_ELFSYM_EXPORT(lv_display_set_buffers),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_outline_pad),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_parse_ext_hdr),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_slave_start),
    ESP_ELFSYM_EXPORT(lv_event_push),
    ESP_ELFSYM_EXPORT(wdev_funcs_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_recycle_sch_item),
    ESP_ELFSYM_EXPORT(mbedtls_symbols),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_a2dp_wifi_conn),
    ESP_ELFSYM_EXPORT(lv_dropdown_class),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_set_scan_rsp_data),
    ESP_ELFSYM_EXPORT(BasicOFDMSched),
    ESP_ELFSYM_EXPORT(ppRegressAmpdu),
    ESP_ELFSYM_EXPORT(esp_sha_hash_abort),
    ESP_ELFSYM_EXPORT(pm_send_sleep_null_cb),
    ESP_ELFSYM_EXPORT(esp_efuse_utility_read_reg),
    ESP_ELFSYM_EXPORT(s_is_6m),
    ESP_ELFSYM_EXPORT(lmacProcessAllTxTimeout),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_prof_is_wpa2_internal),
    ESP_ELFSYM_EXPORT(s_hci_stack_vsEventsMask),
    ESP_ELFSYM_EXPORT(esp_netif_sntp_renew_servers),
    ESP_ELFSYM_EXPORT(__bufio_setdir_locked),
    ESP_ELFSYM_EXPORT(_ZZNSt18__moneypunct_cacheIwLb0EE8_M_cacheERKSt6localeEN11_Scoped_strC2ERKSbIwSt11char_traitsIwESaIwEE),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_rtc_to_ticks),
    ESP_ELFSYM_EXPORT(wl_read),
    ESP_ELFSYM_EXPORT(etharp_input),
    ESP_ELFSYM_EXPORT(lodepng_get_raw_size),
    ESP_ELFSYM_EXPORT(mspi_timing_psram_tuning),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_fetch_new),
    ESP_ELFSYM_EXPORT(sha256_prf),
    ESP_ELFSYM_EXPORT(lvgl_module_symbols),
    ESP_ELFSYM_EXPORT(esp_sha_block),
    ESP_ELFSYM_EXPORT(phy_get_pkdet_data),
    ESP_ELFSYM_EXPORT(lv_image_src_get_type),
    ESP_ELFSYM_EXPORT(__eqsf2),
    ESP_ELFSYM_EXPORT(xTimerCreate),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_mod_mpi),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service11development18DevelopmentServiceE),
    ESP_ELFSYM_EXPORT(wifi_calloc),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_gen_rpa),
    ESP_ELFSYM_EXPORT(lmacGetTxFrame),
    ESP_ELFSYM_EXPORT(key_mgr_hal_continue),
    ESP_ELFSYM_EXPORT(pm_is_force_return_home_chan),
    ESP_ELFSYM_EXPORT(r_ble_rtc_wake_up_cpu_set),
    ESP_ELFSYM_EXPORT(modem_clock_hal_select_wifi_lpclk_source),
    ESP_ELFSYM_EXPORT(ble_hs_id_set_pub),
    ESP_ELFSYM_EXPORT(btowc),
    ESP_ELFSYM_EXPORT(gcc_soft_float_symbols),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_proc_rsp_timer_cb),
    ESP_ELFSYM_EXPORT(lv_display_get_layer_top),
    ESP_ELFSYM_EXPORT(fileno),
    ESP_ELFSYM_EXPORT(ble_gap_connect),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_crt_bundle_attach),
    ESP_ELFSYM_EXPORT(dup_binstr),
    ESP_ELFSYM_EXPORT(arr_hci_setSchedLen),
    ESP_ELFSYM_EXPORT(ble_gattc_read_by_uuid),
    ESP_ELFSYM_EXPORT(vTaskResume),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_send_keyboard),
    ESP_ELFSYM_EXPORT(ieee80211_update_bandwidth),
    ESP_ELFSYM_EXPORT(sdspi_host_set_card_clk),
    ESP_ELFSYM_EXPORT(r_ble_lll_df_env_init),
    ESP_ELFSYM_EXPORT(esp_test_disable_rx_statistics),
    ESP_ELFSYM_EXPORT(ieee80211_pm_tx_null_process),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_set_ctrl_map),
    ESP_ELFSYM_EXPORT(pm_on_coex_schm_process_restart),
    ESP_ELFSYM_EXPORT(register_chipv7_phy),
    ESP_ELFSYM_EXPORT(regdma_link_init),
    ESP_ELFSYM_EXPORT(_ZTVNSt8ios_base7failureB5cxx11E),
    ESP_ELFSYM_EXPORT(phy_freq_mem_data),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_samp_limit_ref),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_bool),
    ESP_ELFSYM_EXPORT(uECC_valid_public_key),
    ESP_ELFSYM_EXPORT(lv_obj_scroll_to_x),
    ESP_ELFSYM_EXPORT(temp_sensor_get_raw_value),
    ESP_ELFSYM_EXPORT(__bufio_get),
    ESP_ELFSYM_EXPORT(ppSearchTxQueue),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_reset),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_crc_on_off),
    ESP_ELFSYM_EXPORT(__cxa_guard_abort),
    ESP_ELFSYM_EXPORT(module_ensure_started),
    ESP_ELFSYM_EXPORT(esp_wifi_set_config),
    ESP_ELFSYM_EXPORT(memory_free),
    ESP_ELFSYM_EXPORT(gpio_descriptor_get_owner_type),
    ESP_ELFSYM_EXPORT(coex_core_pre_init),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_prof_ssid_internal),
    ESP_ELFSYM_EXPORT(__lock___libc_recursive_mutex),
    ESP_ELFSYM_EXPORT(__popcountsi2),
    ESP_ELFSYM_EXPORT(sdmmc_io_rw_direct),
    ESP_ELFSYM_EXPORT(heap_caps_get_allocated_size),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_fill_random),
    ESP_ELFSYM_EXPORT(rtc_clk_slow_src_set),
    ESP_ELFSYM_EXPORT(multi_heap_aligned_free),
    ESP_ELFSYM_EXPORT(coex_schm_interval_set),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_conn_broken),
    ESP_ELFSYM_EXPORT(uECC_secp256r1),
    ESP_ELFSYM_EXPORT(r_ble_log_reset_buf_index_flag),
    ESP_ELFSYM_EXPORT(__time_weekday),
    ESP_ELFSYM_EXPORT(vprintf),
    ESP_ELFSYM_EXPORT(cnx_bss_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_is_enabled),
    ESP_ELFSYM_EXPORT(write),
    ESP_ELFSYM_EXPORT(lv_slider_get_mode),
    ESP_ELFSYM_EXPORT(hal_agreement_del_extra_softap_rx_ba),
    ESP_ELFSYM_EXPORT(memp_RAW_PCB),
    ESP_ELFSYM_EXPORT(g_wdev_record_t2t3_cb),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_alloc_sch),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx1110moneypunctIcLb0EE2idE),
    ESP_ELFSYM_EXPORT(esp_intr_enable),
    ESP_ELFSYM_EXPORT(__cxx_eh_arena_size_get),
    ESP_ELFSYM_EXPORT(esp_reset_reason_set_hint),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hcc_params_set_fallback),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_shift_r),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_a2dp_wifi_connecting),
    ESP_ELFSYM_EXPORT(lv_color_lighten),
    ESP_ELFSYM_EXPORT(i2s_output_gpio_revoke),
    ESP_ELFSYM_EXPORT(_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EEPKS5_OS8_),
    ESP_ELFSYM_EXPORT(ecc_hal_read_mul_result),
    ESP_ELFSYM_EXPORT(_regi2c_impl_write_mask),
    ESP_ELFSYM_EXPORT(wpa_auth_for_each_sta),
    ESP_ELFSYM_EXPORT(__cxx_init_dummy),
    ESP_ELFSYM_EXPORT(ble_gap_conn_find),
    ESP_ELFSYM_EXPORT(ic_del_key),
    ESP_ELFSYM_EXPORT(_findenv),
    ESP_ELFSYM_EXPORT(lv_spinbox_set_value),
    ESP_ELFSYM_EXPORT(wifi_rf_phy_enable),
    ESP_ELFSYM_EXPORT(r_ble_ll_chk_txrx_octets),
    ESP_ELFSYM_EXPORT(esp_md5_hash_clone),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_ds_data),
    ESP_ELFSYM_EXPORT(uECC_vli_modAdd),
    ESP_ELFSYM_EXPORT(r_ble_ll_set_random_addr),
    ESP_ELFSYM_EXPORT(wpa_sm_set_ap_rsn_ie),
    ESP_ELFSYM_EXPORT(lv_realloc_core),
    ESP_ELFSYM_EXPORT(pm_twt_wake_done),
    ESP_ELFSYM_EXPORT(mbedtls_sha256_update),
    ESP_ELFSYM_EXPORT(_cosf),
    ESP_ELFSYM_EXPORT(task_event_group_claim_bit),
    ESP_ELFSYM_EXPORT(lv_indev_find_scroll_obj),
    ESP_ELFSYM_EXPORT(wifi_get_rand_macaddr),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_reschedule_event),
    ESP_ELFSYM_EXPORT(lv_obj_class_create_obj),
    ESP_ELFSYM_EXPORT(coex_schm_curr_phase_get),
    ESP_ELFSYM_EXPORT(base_stack_enable),
    ESP_ELFSYM_EXPORT(mmu_hal_ctx_init),
    ESP_ELFSYM_EXPORT(spi_flash_guard_get),
    ESP_ELFSYM_EXPORT(crypto_bignum_is_odd),
    ESP_ELFSYM_EXPORT(crypto_bignum_sub),
    ESP_ELFSYM_EXPORT(_ZSt8_DestroyIPSt6vectorIhSaIhEEEvT_S4_),
    ESP_ELFSYM_EXPORT(intr_handler_get),
    ESP_ELFSYM_EXPORT(uart_hal_set_rxfifo_full_thr),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_state_sleep_limit_exceeded_wakeup_enable),
    ESP_ELFSYM_EXPORT(r_hal_timer_convert_to_u32),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_adj_data_len_param),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_point_free),
    ESP_ELFSYM_EXPORT(tcp_zero_window_probe),
    ESP_ELFSYM_EXPORT(wifi_mode_set),
    ESP_ELFSYM_EXPORT(bluetooth_unpair),
    ESP_ELFSYM_EXPORT(hid_report_map_gamepad),
    ESP_ELFSYM_EXPORT(cnx_node_is_existing),
    ESP_ELFSYM_EXPORT(wdev_mac_special_reg_store),
    ESP_ELFSYM_EXPORT(esp_vfs_lseek),
    ESP_ELFSYM_EXPORT(setvbuf),
    ESP_ELFSYM_EXPORT(ethip6_output),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_set_peer_rpa),
    ESP_ELFSYM_EXPORT(lv_arc_rotate_obj_to_angle),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_conn_param_reply),
    ESP_ELFSYM_EXPORT(r_ble_hw_get_cte_packet_rx_rate_from_buffer),
    ESP_ELFSYM_EXPORT(gdma_set_weight),
    ESP_ELFSYM_EXPORT(sdmmc_fix_host_flags),
    ESP_ELFSYM_EXPORT(esp_efuse_rtc_calib_get_chan_compens),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_ap_password),
    ESP_ELFSYM_EXPORT(esp_owe_build_assoc_resp_dhie),
    ESP_ELFSYM_EXPORT(ble_gattc_disc_all_svcs),
    ESP_ELFSYM_EXPORT(__isnanf),
    ESP_ELFSYM_EXPORT(nimble_serial_api),
    ESP_ELFSYM_EXPORT(wifi_txq_empty),
    ESP_ELFSYM_EXPORT(_ZTVSt17_Sp_counted_arrayISaIhELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(nan_get_tsf),
    ESP_ELFSYM_EXPORT(ppCertSetRate),
    ESP_ELFSYM_EXPORT(lv_fs_seek),
    ESP_ELFSYM_EXPORT(memp_ND6_QUEUE),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_coex_dpc_update),
    ESP_ELFSYM_EXPORT(lv_display_delete_event),
    ESP_ELFSYM_EXPORT(asprintf),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_update_handshake_status),
    ESP_ELFSYM_EXPORT(rc11ASchedTbl),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_set_sample_ring_buffer_ptr),
    ESP_ELFSYM_EXPORT(i2s_get_source_clk_freq),
    ESP_ELFSYM_EXPORT(owe_deinit),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_ap_min_chan),
    ESP_ELFSYM_EXPORT(dns_mquery_v6group),
    ESP_ELFSYM_EXPORT(hal_init_bf),
    ESP_ELFSYM_EXPORT(esp_event_handler_register),
    ESP_ELFSYM_EXPORT(trc_set_bf_report_rate),
    ESP_ELFSYM_EXPORT(wpabuf_put),
    ESP_ELFSYM_EXPORT(rtc_clk_cpu_freq_set_config_fast),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_USER_DATA_MAC_CUSTOM),
    ESP_ELFSYM_EXPORT(psa_destroy_persistent_key),
    ESP_ELFSYM_EXPORT(wifi_scan_stop_process),
    ESP_ELFSYM_EXPORT(esp_newlib_locks_init),
    ESP_ELFSYM_EXPORT(uart_disable_tx_intr),
    ESP_ELFSYM_EXPORT(regi2c_ctrl_write_reg_mask),
    ESP_ELFSYM_EXPORT(lv_async_call_cancel),
    ESP_ELFSYM_EXPORT(ieee80211_free_beacon_eb),
    ESP_ELFSYM_EXPORT(_ZnajRKSt9nothrow_t),
    ESP_ELFSYM_EXPORT(iscntrl),
    ESP_ELFSYM_EXPORT(esp_newlib_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_env_init),
    ESP_ELFSYM_EXPORT(ieee80211_set_key),
    ESP_ELFSYM_EXPORT(r_ble_ll_mtrack_initialized_get),
    ESP_ELFSYM_EXPORT(lv_anim_set_deleted_cb),
    ESP_ELFSYM_EXPORT(phy_i2c_pkdet_set),
    ESP_ELFSYM_EXPORT(lv_display_refr_timer),
    ESP_ELFSYM_EXPORT(mpi_hal_clear_interrupt),
    ESP_ELFSYM_EXPORT(lv_anim_init),
    ESP_ELFSYM_EXPORT(esp_event_loop_delete),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_int),
    ESP_ELFSYM_EXPORT(ble_gap_conn_cancel),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_init),
    ESP_ELFSYM_EXPORT(isatty),
    ESP_ELFSYM_EXPORT(phy_rx_sig_pwr_sub),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_chk_wthr_adj_max_evt_time),
    ESP_ELFSYM_EXPORT(destination_cache),
    ESP_ELFSYM_EXPORT(lmacDiscardMSDU),
    ESP_ELFSYM_EXPORT(btwt_setup_timer),
    ESP_ELFSYM_EXPORT(wifi_nvs_init),
    ESP_ELFSYM_EXPORT(hid_report_map_keyboard_consumer_len),
    ESP_ELFSYM_EXPORT(ieee80211_btwt_teardown),
    ESP_ELFSYM_EXPORT(wpa_auth_sta_init),
    ESP_ELFSYM_EXPORT(esp_wifi_destroy_if_driver),
    ESP_ELFSYM_EXPORT(ble_l2cap_rx),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_periodic),
    ESP_ELFSYM_EXPORT(minmea_sentence_id),
    ESP_ELFSYM_EXPORT(wl_write),
    ESP_ELFSYM_EXPORT(esp_vfs_mkdir),
    ESP_ELFSYM_EXPORT(pthread_include_pthread_impl),
    ESP_ELFSYM_EXPORT(spi_flash_chip_winbond_get_caps),
    ESP_ELFSYM_EXPORT(ble_hid_get_active),
    ESP_ELFSYM_EXPORT(ble_store_config_write),
    ESP_ELFSYM_EXPORT(ic_disable_rx),
    ESP_ELFSYM_EXPORT(ccmp_encrypt),
    ESP_ELFSYM_EXPORT(multi_heap_malloc),
    ESP_ELFSYM_EXPORT(bluetooth_midi_send),
    ESP_ELFSYM_EXPORT(r_advertise_filter_env_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_sched_conflict_handler),
    ESP_ELFSYM_EXPORT(ieee80211_setup_ratetable),
    ESP_ELFSYM_EXPORT(lv_color_premultiply),
    ESP_ELFSYM_EXPORT(r_ble_hw_driver_env_init),
    ESP_ELFSYM_EXPORT(ff_disk_ioctl),
    ESP_ELFSYM_EXPORT(g_panic_abort_details),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_pend_fatal_alert),
    ESP_ELFSYM_EXPORT(closedir),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_env_deinit),
    ESP_ELFSYM_EXPORT(spi_flash_hal_gpspi_common_command),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_enc_allowed_pdu),
    ESP_ELFSYM_EXPORT(esp_timer_get_expiry_time),
    ESP_ELFSYM_EXPORT(ble_hs_mbuf_l2cap_pkt),
    ESP_ELFSYM_EXPORT(esp_newlib_time_init),
    ESP_ELFSYM_EXPORT(ppRecordBarRRC),
    ESP_ELFSYM_EXPORT(pwr_hal_get_mac_modem_beacon_miss_limit_exceeded_status),
    ESP_ELFSYM_EXPORT(esp_libc_init_global_stdio),
    ESP_ELFSYM_EXPORT(lv_obj_align),
    ESP_ELFSYM_EXPORT(sdmmc_read_sectors),
    ESP_ELFSYM_EXPORT(nvs_flash_init),
    ESP_ELFSYM_EXPORT(esp_crypto_hmac_lock_acquire),
    ESP_ELFSYM_EXPORT(g_startup_time),
    ESP_ELFSYM_EXPORT(lv_obj_style_create_transition),
    ESP_ELFSYM_EXPORT(esp_sha512_driver_compute),
    ESP_ELFSYM_EXPORT(preferences_put_int64),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_gen_prime),
    ESP_ELFSYM_EXPORT(r_ble_lll_df_env_deinit),
    ESP_ELFSYM_EXPORT(esp_test_tx_count_retry),
    ESP_ELFSYM_EXPORT(hal_mac_init),
    ESP_ELFSYM_EXPORT(pm_noise_check_process),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_hs_acl_tx),
    ESP_ELFSYM_EXPORT(lv_obj_remove_event),
    ESP_ELFSYM_EXPORT(audio_stream_open_input),
    ESP_ELFSYM_EXPORT(coex_hw_timer_enable),
    ESP_ELFSYM_EXPORT(esp_libc_include_memcmp_impl),
    ESP_ELFSYM_EXPORT(lv_style_set_border_opa),
    ESP_ELFSYM_EXPORT(lv_display_trigger_activity),
    ESP_ELFSYM_EXPORT(get_channel_max_bandwidth),
    ESP_ELFSYM_EXPORT(lv_chart_get_series_next),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_store_received_rssi),
    ESP_ELFSYM_EXPORT(ieee80211_update_channel),
    ESP_ELFSYM_EXPORT(r_ble_lll_timer_current_tick_get),
    ESP_ELFSYM_EXPORT(ledc_hal_get_clk_cfg),
    ESP_ELFSYM_EXPORT(system_event_callback_add),
    ESP_ELFSYM_EXPORT(lv_group_get_obj_count),
    ESP_ELFSYM_EXPORT(mbedtls_psa_rsa_verify_hash),
    ESP_ELFSYM_EXPORT(service_manager_find_instance),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_is_revoked),
    ESP_ELFSYM_EXPORT(hal_he_bsr_init),
    ESP_ELFSYM_EXPORT(gdma_hal_enable_burst),
    ESP_ELFSYM_EXPORT(wifi_set_protocol_process),
    ESP_ELFSYM_EXPORT(esf_buf_alloc_dynamic_default_handler),
    ESP_ELFSYM_EXPORT(sha256_vector),
    ESP_ELFSYM_EXPORT(display_get_frame_buffer_count),
    ESP_ELFSYM_EXPORT(lv_arc_get_rotation),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_config_free),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_next_event),
    ESP_ELFSYM_EXPORT(wifi_bt_common_module_disable),
    ESP_ELFSYM_EXPORT(esp_event_handler_instance_register),
    ESP_ELFSYM_EXPORT(i2c_master_read_byte),
    ESP_ELFSYM_EXPORT(device_set_driver_data),
    ESP_ELFSYM_EXPORT(r_ble_lll_current_rx_info_get),
    ESP_ELFSYM_EXPORT(sdmmc_mmc_decode_csd),
    ESP_ELFSYM_EXPORT(timer_set_pending_callback),
    ESP_ELFSYM_EXPORT(_ZSt17__throw_bad_allocv),
    ESP_ELFSYM_EXPORT(__lttf2),
    ESP_ELFSYM_EXPORT(ieee80211_recv_action_vendor_spec),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_alloc),
    ESP_ELFSYM_EXPORT(netconn_getaddr),
    ESP_ELFSYM_EXPORT(bluetooth_scan_start),
    ESP_ELFSYM_EXPORT(f_stat),
    ESP_ELFSYM_EXPORT(aes_gcm_ae),
    ESP_ELFSYM_EXPORT(ble_random_pa_env_p),
    ESP_ELFSYM_EXPORT(r_ble_rtc_wake_up_cpu_clr),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_conn_wifi_connecting),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_search_and_delete),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_legacy_pdu_make),
    ESP_ELFSYM_EXPORT(spicommon_dma_desc_alloc),
    ESP_ELFSYM_EXPORT(usb_device_controller_release),
    ESP_ELFSYM_EXPORT(phy_dc_iq_est_new),
    ESP_ELFSYM_EXPORT(spi_flash_chip_winbond_erase_sector),
    ESP_ELFSYM_EXPORT(sdspi_crc7),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_scan),
    ESP_ELFSYM_EXPORT(phy_11p_set),
    ESP_ELFSYM_EXPORT(rate2str),
    ESP_ELFSYM_EXPORT(device_start),
    ESP_ELFSYM_EXPORT(lvgl_pointer_get_calibration),
    ESP_ELFSYM_EXPORT(fmaxf),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_no_rxbuf_left),
    ESP_ELFSYM_EXPORT(psa_mac_verify_finish),
    ESP_ELFSYM_EXPORT(esp_transport_destroy),
    ESP_ELFSYM_EXPORT(esp_cpu_compare_and_set),
    ESP_ELFSYM_EXPORT(esp_log_impl_lock_timeout),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_dup_new),
    ESP_ELFSYM_EXPORT(esp_event_handler_register_with),
    ESP_ELFSYM_EXPORT(iswprint),
    ESP_ELFSYM_EXPORT(hostapd_setup_wpa_psk),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_exception_handle),
    ESP_ELFSYM_EXPORT(npl_freertos_event_init),
    ESP_ELFSYM_EXPORT(lora_add_tx_callback),
    ESP_ELFSYM_EXPORT(sar_periph_ctrl_adc_reset),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_create_chan),
    ESP_ELFSYM_EXPORT(esp_deep_sleep_register_phy_hook),
    ESP_ELFSYM_EXPORT(wpa_sm_mlme_setprotection),
    ESP_ELFSYM_EXPORT(lv_event_get_scroll_anim),
    ESP_ELFSYM_EXPORT(service_manager_get_state),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY_PURPOSE_2),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_msys_alloc_check),
    ESP_ELFSYM_EXPORT(__issignaling),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_make_done),
    ESP_ELFSYM_EXPORT(chm_get_band_from_chan),
    ESP_ELFSYM_EXPORT(rcLoRaRate2SchedIdx),
    ESP_ELFSYM_EXPORT(coex_schm_get_phase_by_idx),
    ESP_ELFSYM_EXPORT(esp_wifi_init),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_update_in_pointers),
    ESP_ELFSYM_EXPORT(tc_aes_encrypt),
    ESP_ELFSYM_EXPORT(lv_event_add),
    ESP_ELFSYM_EXPORT(sys_mbox_post),
    ESP_ELFSYM_EXPORT(lv_area_get_width),
    ESP_ELFSYM_EXPORT(esp_http_client_get_errno),
    ESP_ELFSYM_EXPORT(esp_test_disable_rx_mu_statistics),
    ESP_ELFSYM_EXPORT(sdmmc_allocate_aligned_buf),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_rf_ctrl_wait_cycles),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_send_sync_ind),
    ESP_ELFSYM_EXPORT(keyboard_read_key),
    ESP_ELFSYM_EXPORT(cache_hal_get_cache_line_size),
    ESP_ELFSYM_EXPORT(acd_remove),
    ESP_ELFSYM_EXPORT(sdspi_host_init_device),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sm_reset),
    ESP_ELFSYM_EXPORT(nan_send_action_process),
    ESP_ELFSYM_EXPORT(wpa_sm_rsn_overriding_supported),
    ESP_ELFSYM_EXPORT(mbedtls_ct_memcpy_if),
    ESP_ELFSYM_EXPORT(npl_freertos_eventq_deinit),
    ESP_ELFSYM_EXPORT(mt_send),
    ESP_ELFSYM_EXPORT(lv_timer_delete),
    ESP_ELFSYM_EXPORT(lv_indev_get_scroll_obj),
    ESP_ELFSYM_EXPORT(_Z8tusbStopv),
    ESP_ELFSYM_EXPORT(_esp_error_check_failed),
    ESP_ELFSYM_EXPORT(esp_get_free_internal_heap_size),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_info_event),
    ESP_ELFSYM_EXPORT(phy_iq_est_disable),
    ESP_ELFSYM_EXPORT(phy_i2c_clk_sel),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_delete_by_sm_num_and_type),
    ESP_ELFSYM_EXPORT(r_ble_ll_is_rpa),
    ESP_ELFSYM_EXPORT(lv_spinbox_step_next),
    ESP_ELFSYM_EXPORT(mbedtls_pk_setup),
    ESP_ELFSYM_EXPORT(esp_flash_registered_chips),
    ESP_ELFSYM_EXPORT(i2c_controller_register8_get),
    ESP_ELFSYM_EXPORT(cache_hal_init),
    ESP_ELFSYM_EXPORT(esp_wifi_opr_bss_color),
    ESP_ELFSYM_EXPORT(ppCalDeliNum),
    ESP_ELFSYM_EXPORT(pwm_is_enabled),
    ESP_ELFSYM_EXPORT(esp_vApplicationIdleHook),
    ESP_ELFSYM_EXPORT(httpd_req_handle_err),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_alloc_txbuf),
    ESP_ELFSYM_EXPORT(coex_core_enable),
    ESP_ELFSYM_EXPORT(ic_create_wifi_task),
    ESP_ELFSYM_EXPORT(esp_wifi_stop),
    ESP_ELFSYM_EXPORT(i2c_controller_register16le_set),
    ESP_ELFSYM_EXPORT(ble_gap_timer),
    ESP_ELFSYM_EXPORT(priv_config_opts_ro),
    ESP_ELFSYM_EXPORT(sae_deinit_pt),
    ESP_ELFSYM_EXPORT(lmacDiscardFrameExchangeSequence),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_cert_data_der),
    ESP_ELFSYM_EXPORT(pvTimerGetTimerID),
    ESP_ELFSYM_EXPORT(phy_freq_i2c_write_set),
    ESP_ELFSYM_EXPORT(coex_schm_bt_conn_wifi_connecting),
    ESP_ELFSYM_EXPORT(sta_update_mbssid),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_pb_adv_link_id_table_is_empty),
    ESP_ELFSYM_EXPORT(lv_layout_init),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_subject_alt_name),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_msys_insert),
    ESP_ELFSYM_EXPORT(__nesf2),
    ESP_ELFSYM_EXPORT(coex_core_init),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_opaque_sign_hash_abort),
    ESP_ELFSYM_EXPORT(bundle_opt_string),
    ESP_ELFSYM_EXPORT(gdma_default_rx_isr),
    ESP_ELFSYM_EXPORT(wpa_config_profile),
    ESP_ELFSYM_EXPORT(lv_image_class),
    ESP_ELFSYM_EXPORT(ic_register_timer_cb),
    ESP_ELFSYM_EXPORT(localtime),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_add_spinner_action),
    ESP_ELFSYM_EXPORT(acd_start),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_reserve),
    ESP_ELFSYM_EXPORT(esp_http_client_set_username),
    ESP_ELFSYM_EXPORT(lv_image_decoder_close),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_retrieve),
    ESP_ELFSYM_EXPORT(pp_stop_sw_txq),
    ESP_ELFSYM_EXPORT(lv_spinbox_get_step),
    ESP_ELFSYM_EXPORT(hci_stack_vsEventIsEnabled),
    ESP_ELFSYM_EXPORT(trc_update_def_rate),
    ESP_ELFSYM_EXPORT(ble_bb_cte_get_cte_samp_limit_switch),
    ESP_ELFSYM_EXPORT(_ZSt21__throw_runtime_errorPKc),
    ESP_ELFSYM_EXPORT(audio_stream_set_change_callback),
    ESP_ELFSYM_EXPORT(__floatdidf),
    ESP_ELFSYM_EXPORT(wpa_config_assoc_ie),
    ESP_ELFSYM_EXPORT(wifi_set_default_ssid),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_get_local_rpa),
    ESP_ELFSYM_EXPORT(lv_obj_set_content_height),
    ESP_ELFSYM_EXPORT(_ZGVNSt11__timepunctIcE2idE),
    ESP_ELFSYM_EXPORT(misc_nvs_deinit),
    ESP_ELFSYM_EXPORT(lv_slider_create),
    ESP_ELFSYM_EXPORT(nan_faw_start_process),
    ESP_ELFSYM_EXPORT(coex_core_request),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_free),
    ESP_ELFSYM_EXPORT(__wrap___cxa_call_unexpected),
    ESP_ELFSYM_EXPORT(coex_pti_get),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_CH5_ATTEN0_INITCODE_DIFF),
    ESP_ELFSYM_EXPORT(esp_netif_action_start),
    ESP_ELFSYM_EXPORT(_lock_close_recursive),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_check_pubkey),
    ESP_ELFSYM_EXPORT(lvgl_ppa_supports_color_format),
    ESP_ELFSYM_EXPORT(esp_btbb_enable),
    ESP_ELFSYM_EXPORT(tcp_poll),
    ESP_ELFSYM_EXPORT(bundle_get_int32),
    ESP_ELFSYM_EXPORT(esp32_sdspi_fs_alloc),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_a2dp_paused_wifi_conn),
    ESP_ELFSYM_EXPORT(lv_strnlen),
    ESP_ELFSYM_EXPORT(phy_set_rx_comp_new),
    ESP_ELFSYM_EXPORT(scan_get_type),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_border_color),
    ESP_ELFSYM_EXPORT(cnx_add_rc),
    ESP_ELFSYM_EXPORT(xTaskCreateStaticPinnedToCore),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_cth_flow_error_fn),
    ESP_ELFSYM_EXPORT(esp_hw_stack_guard_get_pc),
    ESP_ELFSYM_EXPORT(esp_netif_up),
    ESP_ELFSYM_EXPORT(lv_timer_pause),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_acl_rx),
    ESP_ELFSYM_EXPORT(ieee80211_mt_key_set_mask),
    ESP_ELFSYM_EXPORT(xTaskIncrementTick),
    ESP_ELFSYM_EXPORT(lv_event_get_dsc),
    ESP_ELFSYM_EXPORT(ble_hs_conn_find),
    ESP_ELFSYM_EXPORT(phy_write_pbus_mem),
    ESP_ELFSYM_EXPORT(crypto_ec_point_from_bin),
    ESP_ELFSYM_EXPORT(lvgl_devices_detach),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_default_wifi_connecting),
    ESP_ELFSYM_EXPORT(lv_calendar_set_month_shown),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_msys_get_block_num),
    ESP_ELFSYM_EXPORT(ppProcTxDone),
    ESP_ELFSYM_EXPORT(spi_bus_lock_register_dev),
    ESP_ELFSYM_EXPORT(esp_netif_dhcps_stop),
    ESP_ELFSYM_EXPORT(sdmmc_send_app_cmd),
    ESP_ELFSYM_EXPORT(r_put_le32),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_transfer),
    ESP_ELFSYM_EXPORT(http_header_clean),
    ESP_ELFSYM_EXPORT(esp_sha512_driver_update),
    ESP_ELFSYM_EXPORT(pm_handle_tbtt_interval),
    ESP_ELFSYM_EXPORT(adc_calc_hw_calibration_code),
    ESP_ELFSYM_EXPORT(audio_codec_get_volume),
    ESP_ELFSYM_EXPORT(lv_dropdown_get_selected),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx1110moneypunctIcLb1EEE),
    ESP_ELFSYM_EXPORT(_ZZNKSt7collateIwE12do_transformEPKwS2_EN4_BufD2Ev),
    ESP_ELFSYM_EXPORT(xTimerPendFunctionCallFromISR),
    ESP_ELFSYM_EXPORT(npl_freertos_time_delay),
    ESP_ELFSYM_EXPORT(panic_print_str),
    ESP_ELFSYM_EXPORT(TRACKBALL_TYPE),
    ESP_ELFSYM_EXPORT(tcp_pcb_lists),
    ESP_ELFSYM_EXPORT(GROVE_TYPE),
    ESP_ELFSYM_EXPORT(heap_caps_realloc_default),
    ESP_ELFSYM_EXPORT(pwm_disable),
    ESP_ELFSYM_EXPORT(ieee80211_setup_rates),
    ESP_ELFSYM_EXPORT(ble_sm_sc_public_key_rx),
    ESP_ELFSYM_EXPORT(ieee80211_hostap_attach),
    ESP_ELFSYM_EXPORT(mbedtls_md_get_size),
    ESP_ELFSYM_EXPORT(ble_ll_env_p),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_enable_intr),
    ESP_ELFSYM_EXPORT(ieee80211_crypto_aes_128_cmac_decrypt),
    ESP_ELFSYM_EXPORT(app_scheduler_current_app_id),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_MAC_EXT),
    ESP_ELFSYM_EXPORT(lv_draw_task_get_fill_dsc),
    ESP_ELFSYM_EXPORT(lvgl_sliderbox_set_value),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_read),
    ESP_ELFSYM_EXPORT(modem_clock_module_bits_get),
    ESP_ELFSYM_EXPORT(lv_mem_deinit),
    ESP_ELFSYM_EXPORT(pp_coex_tx_release),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_aux_pdu_ext_hdr_make),
    ESP_ELFSYM_EXPORT(power_supply_supports_property),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_fSt12chars_formati),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_find_info),
    ESP_ELFSYM_EXPORT(lodepng_decoder_settings_init),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_info_from_psa),
    ESP_ELFSYM_EXPORT(spi_periph_signal),
    ESP_ELFSYM_EXPORT(round),
    ESP_ELFSYM_EXPORT(lv_obj_get_parent),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_assign_user_block),
    ESP_ELFSYM_EXPORT(lwip_hook_ip6_input),
    ESP_ELFSYM_EXPORT(lv_dropdown_set_selected_highlight),
    ESP_ELFSYM_EXPORT(MEMORY_POLICY_DEFAULT),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_disconnect),
    ESP_ELFSYM_EXPORT(g_adv_stack_envP),
    ESP_ELFSYM_EXPORT(esp_io_expander_gpio_wrapper_reset_pin),
    ESP_ELFSYM_EXPORT(lv_textarea_get_max_length),
    ESP_ELFSYM_EXPORT(panic_abort),
    ESP_ELFSYM_EXPORT(mbedtls_psa_mac_verify_finish),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sync_get_pdu_len),
    ESP_ELFSYM_EXPORT(tzset),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_read_group_type),
    ESP_ELFSYM_EXPORT(npl_freertos_mempool_deinit),
    ESP_ELFSYM_EXPORT(xQueueTakeMutexRecursive),
    ESP_ELFSYM_EXPORT(lv_draw_finalize_task_creation),
    ESP_ELFSYM_EXPORT(lv_color_white),
    ESP_ELFSYM_EXPORT(g_lmac_cnt),
    ESP_ELFSYM_EXPORT(ip4_output_if),
    ESP_ELFSYM_EXPORT(app_esp32_module),
    ESP_ELFSYM_EXPORT(pm_wake_done),
    ESP_ELFSYM_EXPORT(esp_http_client_get_transport_type),
    ESP_ELFSYM_EXPORT(ieee80211_set_sta_gtk_index),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_set_scan_rsp_data),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_deinit),
    ESP_ELFSYM_EXPORT(coex_bt_release),
    ESP_ELFSYM_EXPORT(conn_hci_sendChanMapUpdCompVsEvent),
    ESP_ELFSYM_EXPORT(chm_init),
    ESP_ELFSYM_EXPORT(esp_now_add_peer),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_end),
    ESP_ELFSYM_EXPORT(ble_hid_get_conn_handle),
    ESP_ELFSYM_EXPORT(scan_get_apnum),
    ESP_ELFSYM_EXPORT(ledc_set_fade_with_time),
    ESP_ELFSYM_EXPORT(esp_clk_slowclk_cal_set),
    ESP_ELFSYM_EXPORT(_ZSteqIhLj6EEbRKSt5arrayIT_XT0_EES4_),
    ESP_ELFSYM_EXPORT(lv_style_set_arc_rounded),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_conf_authmode),
    ESP_ELFSYM_EXPORT(sta_sa_query_process_timeout),
    ESP_ELFSYM_EXPORT(crypto_ec_key_parse_pub),
    ESP_ELFSYM_EXPORT(twtsetupcmd_str),
    ESP_ELFSYM_EXPORT(ble_sm_sc_dhkey_check_rx),
    ESP_ELFSYM_EXPORT(lv_bar_get_value),
    ESP_ELFSYM_EXPORT(esp_efuse_utility_check_errors),
    ESP_ELFSYM_EXPORT(ledc_stop),
    ESP_ELFSYM_EXPORT(pm_offchan_ready_or_defer),
    ESP_ELFSYM_EXPORT(tcp_enqueue_flags),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_env_init_for_sm),
    ESP_ELFSYM_EXPORT(sys_thread_new),
    ESP_ELFSYM_EXPORT(hal_mac_set_addr),
    ESP_ELFSYM_EXPORT(wpa_sta_cur_pmksa_matches_akm),
    ESP_ELFSYM_EXPORT(ble_sm_sc_oob_data_check),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_prepare_handshake_record),
    ESP_ELFSYM_EXPORT(xIsrStack),
    ESP_ELFSYM_EXPORT(r_ble_ll_get_tx_pwr_compensation),
    ESP_ELFSYM_EXPORT(pointer_set_mirror_y),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_page_program),
    ESP_ELFSYM_EXPORT(lv_dropdown_set_selected),
    ESP_ELFSYM_EXPORT(hal_he_set_ersu),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_close),
    ESP_ELFSYM_EXPORT(usb_midi_is_connected),
    ESP_ELFSYM_EXPORT(device_is_ready),
    ESP_ELFSYM_EXPORT(netif_ip6_addr_set),
    ESP_ELFSYM_EXPORT(lvgl_spinner_create),
    ESP_ELFSYM_EXPORT(ledc_timer_pause),
    ESP_ELFSYM_EXPORT(httpd_req_delete),
    ESP_ELFSYM_EXPORT(ble_hs_hci_evt_acl_process),
    ESP_ELFSYM_EXPORT(ieee80211_parse_wpa),
    ESP_ELFSYM_EXPORT(coex_schm_bt_inq_wifi_scan),
    ESP_ELFSYM_EXPORT(ieee80211_ampdu_enable),
    ESP_ELFSYM_EXPORT(thread_get_state),
    ESP_ELFSYM_EXPORT(lv_display_get_vertical_resolution),
    ESP_ELFSYM_EXPORT(free_bss_info),
    ESP_ELFSYM_EXPORT(esp_wifi_wpa_ptk_init_done_internal),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_rsaes_oaep_decrypt),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_rd_rssi),
    ESP_ELFSYM_EXPORT(ppHEAMPDU2Normal),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_set_path_loss_en),
    ESP_ELFSYM_EXPORT(r_os_mbuf_free_chain),
    ESP_ELFSYM_EXPORT(hostap_deinit),
    ESP_ELFSYM_EXPORT(ic_set_sleep_min_active_time),
    ESP_ELFSYM_EXPORT(lv_display_enable_invalidation),
    ESP_ELFSYM_EXPORT(esp_sha1_starts),
    ESP_ELFSYM_EXPORT(npl_freertos_mutex_release),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_add_switch_action),
    ESP_ELFSYM_EXPORT(wdev_process_tbtt),
    ESP_ELFSYM_EXPORT(lv_event_set_ext_draw_size),
    ESP_ELFSYM_EXPORT(ble_bb_cte_get_cte_samp_limit_ref),
    ESP_ELFSYM_EXPORT(i2c_master_write),
    ESP_ELFSYM_EXPORT(kernel_init),
    ESP_ELFSYM_EXPORT(npl_freertos_time_ticks_to_ms32),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_e),
    ESP_ELFSYM_EXPORT(tcp_backlog_accepted),
    ESP_ELFSYM_EXPORT(wpa_cipher_valid_pairwise),
    ESP_ELFSYM_EXPORT(ic_set_sta),
    ESP_ELFSYM_EXPORT(he_twt_information_event_post),
    ESP_ELFSYM_EXPORT(lv_bar_set_mode),
    ESP_ELFSYM_EXPORT(mbedtls_md_update),
    ESP_ELFSYM_EXPORT(_set),
    ESP_ELFSYM_EXPORT(ble_hs_cfg),
    ESP_ELFSYM_EXPORT(ble_store_delete),
    ESP_ELFSYM_EXPORT(log1pf),
    ESP_ELFSYM_EXPORT(ieee80211_close_all_twt_sessions),
    ESP_ELFSYM_EXPORT(uart_is_driver_installed),
    ESP_ELFSYM_EXPORT(xEventGroupSetBitsFromISR),
    ESP_ELFSYM_EXPORT(inet_chksum),
    ESP_ELFSYM_EXPORT(pbuf_cat),
    ESP_ELFSYM_EXPORT(sdmmc_mmc_decode_cid),
    ESP_ELFSYM_EXPORT(ptr_check_val),
    ESP_ELFSYM_EXPORT(__adddf3),
    ESP_ELFSYM_EXPORT(get_vfs_for_index),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_update_safe_data_len),
    ESP_ELFSYM_EXPORT(memspi_host_flush_cache),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_pdu_make),
    ESP_ELFSYM_EXPORT(lv_tabview_get_tab_active),
    ESP_ELFSYM_EXPORT(he_twt_information_txcb),
    ESP_ELFSYM_EXPORT(ledc_get_freq),
    ESP_ELFSYM_EXPORT(ledc_set_pin),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_can_chg_whitelist),
    ESP_ELFSYM_EXPORT(r_ble_lll_get_npl_element_info),
    ESP_ELFSYM_EXPORT(g_wpa3_hostap_auth_api_lock),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_le_encrypt),
    ESP_ELFSYM_EXPORT(esp_flash_chip_driver_initialized),
    ESP_ELFSYM_EXPORT(coex_dbg_output),
    ESP_ELFSYM_EXPORT(pxPortInitialiseStack),
    ESP_ELFSYM_EXPORT(mbedtls_strerror),
    ESP_ELFSYM_EXPORT(ic_set_rx_policy_ubssid_check),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_set_hostname),
    ESP_ELFSYM_EXPORT(ff_diskio_get_pdrv_cnt_card),
    ESP_ELFSYM_EXPORT(ff_disk_read),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_delete_by_type),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_bg_image_recolor),
    ESP_ELFSYM_EXPORT(phy_rx_filter_mode),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_color),
    ESP_ELFSYM_EXPORT(he_preamble_su),
    ESP_ELFSYM_EXPORT(r_hal_timer_read_tick),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_stop),
    ESP_ELFSYM_EXPORT(_ZSt16__do_uninit_copyIN9__gnu_cxx17__normal_iteratorIPKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESt6vectorIS7_SaIS7_EEEESD_PS7_ET1_T_T0_SF_),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_le_pwr_change_ind_make),
    ESP_ELFSYM_EXPORT(wlanif_init_sta),
    ESP_ELFSYM_EXPORT(coex_schm_bt_idle_wifi_connecting),
    ESP_ELFSYM_EXPORT(call_start_cpu0),
    ESP_ELFSYM_EXPORT(ble_random_mode_pa_exec_process_cb_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_put_syncinfo),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_AVE_INITCODE_ATTEN0),
    ESP_ELFSYM_EXPORT(lvgl_port_unlock),
    ESP_ELFSYM_EXPORT(service_instance_get_state),
    ESP_ELFSYM_EXPORT(esp_netif_create_default_wifi_sta),
    ESP_ELFSYM_EXPORT(nan_dp_delete_peer),
    ESP_ELFSYM_EXPORT(lv_malloc_zeroed),
    ESP_ELFSYM_EXPORT(esp_ecdsa_transparent_verify_hash_abort),
    ESP_ELFSYM_EXPORT(console_fstat),
    ESP_ELFSYM_EXPORT(test_rx_parse_nonmumimo_complete_sigb),
    ESP_ELFSYM_EXPORT(s_btwt_id_bitmap),
    ESP_ELFSYM_EXPORT(__retarget_lock_init_recursive),
    ESP_ELFSYM_EXPORT(lv_obj_remove_event_cb),
    ESP_ELFSYM_EXPORT(extAdv_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(r_scan_duplicate_cache_refresh_cb),
    ESP_ELFSYM_EXPORT(lv_menu_sidebar_header_cont_class),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_set_mode),
    ESP_ELFSYM_EXPORT(intr_handler_set),
    ESP_ELFSYM_EXPORT(apm_hal_enable_ctrl_filter_all),
    ESP_ELFSYM_EXPORT(acd_arp_reply),
    ESP_ELFSYM_EXPORT(heap_caps_malloc_extmem_enable),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_scan),
    ESP_ELFSYM_EXPORT(ieee80211_amsdu_length_check),
    ESP_ELFSYM_EXPORT(sntp_get_sync_status),
    ESP_ELFSYM_EXPORT(mbedtls_aes_crypt_cbc),
    ESP_ELFSYM_EXPORT(hal_disable_sta_beacon_filter),
    ESP_ELFSYM_EXPORT(ble_hs_gap_svc_ctx),
    ESP_ELFSYM_EXPORT(psa_custom_key_parameters_are_default),
    ESP_ELFSYM_EXPORT(crypto_bignum_init_uint),
    ESP_ELFSYM_EXPORT(phy_open_i2c_xpd),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_set_bio),
    ESP_ELFSYM_EXPORT(lv_draw_buf_align_ex),
    ESP_ELFSYM_EXPORT(pm_enable_twt_keep_alive_timer),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_validate_params),
    ESP_ELFSYM_EXPORT(ble_l2cap_chan_free),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_alloc_memory),
    ESP_ELFSYM_EXPORT(netconn_shutdown),
    ESP_ELFSYM_EXPORT(ieee80211_find_ext_ie),
    ESP_ELFSYM_EXPORT(console_close),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_check_need_report),
    ESP_ELFSYM_EXPORT(__atof_engine),
    ESP_ELFSYM_EXPORT(lv_dropdown_set_options),
    ESP_ELFSYM_EXPORT(hid_report_map_keyboard_consumer),
    ESP_ELFSYM_EXPORT(ampdu_alloc_rx_ba_index),
    ESP_ELFSYM_EXPORT(ethzero),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_set_ring_list_max_num),
    ESP_ELFSYM_EXPORT(ieee80211_getcapinfo),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_set_periodic_acad_chM),
    ESP_ELFSYM_EXPORT(rtc_clk_cpu_freq_get_config),
    ESP_ELFSYM_EXPORT(lv_image_create),
    ESP_ELFSYM_EXPORT(phy_wifi_get_tx_tab_new),
    ESP_ELFSYM_EXPORT(ieee80211_beacon_alloc),
    ESP_ELFSYM_EXPORT(wifi_hw_stop),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_init),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_exp_mod_soft),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_write_pubkey),
    ESP_ELFSYM_EXPORT(pwr_hal_clear_mac_modem_rx_beacon_sleep_counter),
    ESP_ELFSYM_EXPORT(ble_att_svr_tx_error_rsp),
    ESP_ELFSYM_EXPORT(esp_wpa3_free_sae_data),
    ESP_ELFSYM_EXPORT(lv_point_array_transform),
    ESP_ELFSYM_EXPORT(psa_mac_sign_finish),
    ESP_ELFSYM_EXPORT(g_mesh_topology),
    ESP_ELFSYM_EXPORT(lv_dropdown_set_symbol),
    ESP_ELFSYM_EXPORT(vRingbufferReturnItem),
    ESP_ELFSYM_EXPORT(ic_update_trc_params),
    ESP_ELFSYM_EXPORT(wifi_nvs_get_sta_listen_interval),
    ESP_ELFSYM_EXPORT(chm_start_op),
    ESP_ELFSYM_EXPORT(lv_area_move),
    ESP_ELFSYM_EXPORT(lv_draw_dispatch_wait_for_request),
    ESP_ELFSYM_EXPORT(esp_send_sae_auth_reply),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ecdsa_sign_hash),
    ESP_ELFSYM_EXPORT(pbkdf2_sha1),
    ESP_ELFSYM_EXPORT(PAU_instance),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_radius_init),
    ESP_ELFSYM_EXPORT(_write_r),
    ESP_ELFSYM_EXPORT(_ZTISt17bad_function_call),
    ESP_ELFSYM_EXPORT(esp_time_impl_set_boot_time),
    ESP_ELFSYM_EXPORT(pm_is_twt_awake),
    ESP_ELFSYM_EXPORT(minitar_align_up_to_block_size),
    ESP_ELFSYM_EXPORT(ble_gatts_indicate),
    ESP_ELFSYM_EXPORT(r_ble_ll_ch_map_update_timer_cb),
    ESP_ELFSYM_EXPORT(lv_font_get_glyph_bitmap),
    ESP_ELFSYM_EXPORT(_Z11release_pinPP14GpioDescriptor),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_abort_aux_sched),
    ESP_ELFSYM_EXPORT(window_manager_await_state_change),
    ESP_ELFSYM_EXPORT(xQueueGenericGetStaticBuffers),
    ESP_ELFSYM_EXPORT(adiList_stack_disable),
    ESP_ELFSYM_EXPORT(__lock___atexit_recursive_mutex),
    ESP_ELFSYM_EXPORT(xTimerCreateStatic),
    ESP_ELFSYM_EXPORT(esp32_sdspi_driver),
    ESP_ELFSYM_EXPORT(ppCalTxHESMPDULength),
    ESP_ELFSYM_EXPORT(esp_wifi_get_sta_key_internal),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_RD_DIS_BLOCK_KEY0),
    ESP_ELFSYM_EXPORT(rv_core_critical_regs_frame),
    ESP_ELFSYM_EXPORT(ds_hal_start_sign),
    ESP_ELFSYM_EXPORT(lv_rb_remove_node),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_stop),
    ESP_ELFSYM_EXPORT(wpa_validate_wpa_ie),
    ESP_ELFSYM_EXPORT(xTimerCreateTimerTask),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_module_init),
    ESP_ELFSYM_EXPORT(app_manager_find_manifest),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sec_chan_sch_cnt_get),
    ESP_ELFSYM_EXPORT(crypto_ec_point_init),
    ESP_ELFSYM_EXPORT(reachable_time),
    ESP_ELFSYM_EXPORT(nvs_get_u16),
    ESP_ELFSYM_EXPORT(lv_obj_scroll_to),
    ESP_ELFSYM_EXPORT(__gettzinfo),
    ESP_ELFSYM_EXPORT(mt_set_pmk),
    ESP_ELFSYM_EXPORT(lv_tick_get),
    ESP_ELFSYM_EXPORT(ic_set_interrupt_handler),
    ESP_ELFSYM_EXPORT(abort_expect_void),
    ESP_ELFSYM_EXPORT(udp_sendto_if),
    ESP_ELFSYM_EXPORT(sys_timeout),
    ESP_ELFSYM_EXPORT(esp_intr_free),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_parse_server_name_ext),
    ESP_ELFSYM_EXPORT(ble_gap_unpair_oldest_except),
    ESP_ELFSYM_EXPORT(__time_month),
    ESP_ELFSYM_EXPORT(paths_get_user_data_path),
    ESP_ELFSYM_EXPORT(_panic_handler),
    ESP_ELFSYM_EXPORT(fgets),
    ESP_ELFSYM_EXPORT(timer_alloc),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_write_mpi),
    ESP_ELFSYM_EXPORT(phy_bb_bss_cbw40_dig),
    ESP_ELFSYM_EXPORT(ppCalTxVHTSMPDULength),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_ext_estimate_data_itvl),
    ESP_ELFSYM_EXPORT(lv_draw_get_next_available_task),
    ESP_ELFSYM_EXPORT(he_recv_action_event_bsscolor),
    ESP_ELFSYM_EXPORT(i2s_check_set_mclk),
    ESP_ELFSYM_EXPORT(properties_file_set),
    ESP_ELFSYM_EXPORT(ble_hs_id_gen_rnd),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceIN2tt6PubSubINS0_7service9webserver14WebServerEventEEESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(sdmmc_io_send_op_cond),
    ESP_ELFSYM_EXPORT(rv_utils_dbgr_is_attached),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_read_binary),
    ESP_ELFSYM_EXPORT(lv_draw_buf_set_flag),
    ESP_ELFSYM_EXPORT(lv_theme_simple_deinit),
    ESP_ELFSYM_EXPORT(hal_he_set_bf_report_rate),
    ESP_ELFSYM_EXPORT(itwt_information_timer),
    ESP_ELFSYM_EXPORT(service_paths_get_user_data_path),
    ESP_ELFSYM_EXPORT(extension_advfilter_module_env_p),
    ESP_ELFSYM_EXPORT(atan),
    ESP_ELFSYM_EXPORT(lv_timer_periodic_handler),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_create),
    ESP_ELFSYM_EXPORT(esp_wifi_connect_internal),
    ESP_ELFSYM_EXPORT(ieee80211_add_probe_req_ies),
    ESP_ELFSYM_EXPORT(wpa_mic_len),
    ESP_ELFSYM_EXPORT(g_beacon_eb),
    ESP_ELFSYM_EXPORT(pm_local_tsf_process),
    ESP_ELFSYM_EXPORT(mbedtls_psa_rsa_export_public_key),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_wifi_scan),
    ESP_ELFSYM_EXPORT(ff_disk_initialize),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_deleted_irk_rmv),
    ESP_ELFSYM_EXPORT(ieee80211_softap_add_uora_parameter),
    ESP_ELFSYM_EXPORT(cfree),
    ESP_ELFSYM_EXPORT(mpi_hal_set_search_position),
    ESP_ELFSYM_EXPORT(panic_arch_fill_info),
    ESP_ELFSYM_EXPORT(lv_chart_set_point_count),
    ESP_ELFSYM_EXPORT(esp_ble_controller_info_capture),
    ESP_ELFSYM_EXPORT(sha256_prf_bits),
    ESP_ELFSYM_EXPORT(ble_hs_mbuf_pullup_base),
    ESP_ELFSYM_EXPORT(esp_sha_dma),
    ESP_ELFSYM_EXPORT(crypto_ec_init),
    ESP_ELFSYM_EXPORT(device_get_by_name),
    ESP_ELFSYM_EXPORT(ferror),
    ESP_ELFSYM_EXPORT(lv_dropdown_get_dir),
    ESP_ELFSYM_EXPORT(netconn_join_leave_group),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_verify_certificate),
    ESP_ELFSYM_EXPORT(lv_arc_set_bg_end_angle),
    ESP_ELFSYM_EXPORT(ble_spp_get_conn_handle),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_lost_event),
    ESP_ELFSYM_EXPORT(_ZSt11make_sharedIA_hESt10shared_ptrIT_Ej),
    ESP_ELFSYM_EXPORT(nvs_erase_key),
    ESP_ELFSYM_EXPORT(minitar_validate_header),
    ESP_ELFSYM_EXPORT(lv_image_decoder_set_info_cb),
    ESP_ELFSYM_EXPORT(lv_obj_allocate_spec_attr),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_get_earliest_start_time),
    ESP_ELFSYM_EXPORT(hal_mac_tsf_reset),
    ESP_ELFSYM_EXPORT(esp_netif_down),
    ESP_ELFSYM_EXPORT(r_hal_rtc_irq_handler),
    ESP_ELFSYM_EXPORT(driver_construct_add),
    ESP_ELFSYM_EXPORT(lv_spinbox_decrement),
    ESP_ELFSYM_EXPORT(pm_tx_data_done_process),
    ESP_ELFSYM_EXPORT(pmksa_cache_free_entry),
    ESP_ELFSYM_EXPORT(esp_netif_init),
    ESP_ELFSYM_EXPORT(mbedtls_mutex_lock_ptr),
    ESP_ELFSYM_EXPORT(phy_rfcal_txiq),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_get_button_text),
    ESP_ELFSYM_EXPORT(coex_schm_bt_piscan_wifi_conn),
    ESP_ELFSYM_EXPORT(lv_arc_get_min_value),
    ESP_ELFSYM_EXPORT(rcGet11AXHighestRateIdx),
    ESP_ELFSYM_EXPORT(lwip_write),
    ESP_ELFSYM_EXPORT(heap_caps_get_free_size),
    ESP_ELFSYM_EXPORT(isnan),
    ESP_ELFSYM_EXPORT(esp_pbuf_allocate),
    ESP_ELFSYM_EXPORT(phy_chan_freq_hw_init),
    ESP_ELFSYM_EXPORT(iswupper),
    ESP_ELFSYM_EXPORT(asinf),
    ESP_ELFSYM_EXPORT(minmea_check),
    ESP_ELFSYM_EXPORT(memspi_host_write_data_slicer),
    ESP_ELFSYM_EXPORT(__kernel_sin),
    ESP_ELFSYM_EXPORT(lv_text_encoded_size),
    ESP_ELFSYM_EXPORT(adc_channel_read_raw),
    ESP_ELFSYM_EXPORT(httpd_sess_set_descriptors),
    ESP_ELFSYM_EXPORT(dhcp_start),
    ESP_ELFSYM_EXPORT(phy_xtal_duty_cal),
    ESP_ELFSYM_EXPORT(cnx_obss_scan_done_cb),
    ESP_ELFSYM_EXPORT(uart_vfs_include_dev_init),
    ESP_ELFSYM_EXPORT(esp_crypto_hmac_lock_release),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_RD_DIS_BLOCK_KEY4),
    ESP_ELFSYM_EXPORT(esp_transport_list_add),
    ESP_ELFSYM_EXPORT(__clzsi2),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_env_deinit),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_le_csa),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_npl_store),
    ESP_ELFSYM_EXPORT(wDev_IndicateFrame),
    ESP_ELFSYM_EXPORT(lora_can_transmit),
    ESP_ELFSYM_EXPORT(ieee80211_ht_attach),
    ESP_ELFSYM_EXPORT(__retarget_lock_try_acquire_recursive),
    ESP_ELFSYM_EXPORT(vTaskSuspend),
    ESP_ELFSYM_EXPORT(uECC_valid_point),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_muladd_restartable),
    ESP_ELFSYM_EXPORT(device_destruct),
    ESP_ELFSYM_EXPORT(udp_remove),
    ESP_ELFSYM_EXPORT(phy_set_loopback_gain),
    ESP_ELFSYM_EXPORT(sscanf),
    ESP_ELFSYM_EXPORT(r_ble_ll_utils_verify_aa),
    ESP_ELFSYM_EXPORT(unregister_ieee80211_action_vendor_get_key_cb),
    ESP_ELFSYM_EXPORT(ble_sm_inject_io),
    ESP_ELFSYM_EXPORT(sys_sem_free),
    ESP_ELFSYM_EXPORT(mbedtls_free),
    ESP_ELFSYM_EXPORT(esp_transport_set_errors),
    ESP_ELFSYM_EXPORT(lv_ll_get_len),
    ESP_ELFSYM_EXPORT(_Z15gps_ledger_syncv),
    ESP_ELFSYM_EXPORT(ieee80211_parse_htcap),
    ESP_ELFSYM_EXPORT(phy_chip_set_chan_ana),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_est_event_success),
    ESP_ELFSYM_EXPORT(lv_group_create),
    ESP_ELFSYM_EXPORT(r_ble_ll_customize_peer_sca_get),
    ESP_ELFSYM_EXPORT(pm_mac_disable_tsf_tbtt_modem_wakeup),
    ESP_ELFSYM_EXPORT(esp_wifi_create_if_driver),
    ESP_ELFSYM_EXPORT(esp_libc_include_memmove_impl),
    ESP_ELFSYM_EXPORT(ic_trc_update_def_rate),
    ESP_ELFSYM_EXPORT(ff_mutex_give),
    ESP_ELFSYM_EXPORT(ble_sm_alg_f6),
    ESP_ELFSYM_EXPORT(event_group_set),
    ESP_ELFSYM_EXPORT(esp_clk_tree_xtal32k_get_freq_hz),
    ESP_ELFSYM_EXPORT(ble_sm_alg_encrypt),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handshake_wrapup_free_hs_transform),
    ESP_ELFSYM_EXPORT(hal_sniffer_set_promis_misc_pkt),
    ESP_ELFSYM_EXPORT(vPortAssertIfInISR),
    ESP_ELFSYM_EXPORT(psa_get_key_attributes),
    ESP_ELFSYM_EXPORT(lv_textarea_get_label),
    ESP_ELFSYM_EXPORT(_regi2c_impl_read_mask),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_send_truncated_per_adv_rpt),
    ESP_ELFSYM_EXPORT(wDev_Set_Beacon_Int),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_tx_pkt_in),
    ESP_ELFSYM_EXPORT(lodepng_decompress_settings_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_put_aux_ptr),
    ESP_ELFSYM_EXPORT(esp_event_loop_delete_default),
    ESP_ELFSYM_EXPORT(tcp_bound_pcbs),
    ESP_ELFSYM_EXPORT(lv_cache_entry_init),
    ESP_ELFSYM_EXPORT(hal_crypto_get_key_entry),
    ESP_ELFSYM_EXPORT(aes_ctr_encrypt),
    ESP_ELFSYM_EXPORT(sleep_retention_module_init),
    ESP_ELFSYM_EXPORT(pp_unregister_tx_cb),
    ESP_ELFSYM_EXPORT(i2s_hal_set_tx_clock),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_adv_decode_addr),
    ESP_ELFSYM_EXPORT(r_advertise_filter_env_deinit),
    ESP_ELFSYM_EXPORT(clk_hal_xtal_get_freq_mhz),
    ESP_ELFSYM_EXPORT(minitar_parse_metadata_from_tar_header),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_reset_periodic_link_state),
    ESP_ELFSYM_EXPORT(cal_ap_bandwith),
    ESP_ELFSYM_EXPORT(ble_sm_sc_init),
    ESP_ELFSYM_EXPORT(lv_bin_decoder_get_area),
    ESP_ELFSYM_EXPORT(lv_obj_move_to_index),
    ESP_ELFSYM_EXPORT(preferences_open),
    ESP_ELFSYM_EXPORT(wifi_event_post),
    ESP_ELFSYM_EXPORT(pm_coex_schm_overall_period_get),
    ESP_ELFSYM_EXPORT(memp_TCPIP_MSG_INPKT),
    ESP_ELFSYM_EXPORT(i2s_channel_reconfig_std_clock),
    ESP_ELFSYM_EXPORT(esp_flash_write_encrypted),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_addr_ring_list_add),
    ESP_ELFSYM_EXPORT(stl_symbols),
    ESP_ELFSYM_EXPORT(scan_stack_initEnv),
    ESP_ELFSYM_EXPORT(esp_gpio_is_reserved),
    ESP_ELFSYM_EXPORT(lv_arc_get_bg_angle_end),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_buffer_owner_clear),
    ESP_ELFSYM_EXPORT(esp_crypto_sha_enable_periph_clk),
    ESP_ELFSYM_EXPORT(esp_crypto_aes_gcm_decrypt),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_rsassa_pss_params),
    ESP_ELFSYM_EXPORT(ble_att_tx_with_conn),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handle_pending_alert),
    ESP_ELFSYM_EXPORT(ieee80211_send_proberesp),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_write_be),
    ESP_ELFSYM_EXPORT(string_symbols),
    ESP_ELFSYM_EXPORT(band2str),
    ESP_ELFSYM_EXPORT(crypto_ecdh_deinit),
    ESP_ELFSYM_EXPORT(XYcZ_add),
    ESP_ELFSYM_EXPORT(httpd_sess_clear_ctx),
    ESP_ELFSYM_EXPORT(esp_log),
    ESP_ELFSYM_EXPORT(regi2c_saradc_disable),
    ESP_ELFSYM_EXPORT(rtc_isr_noniram_disable),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_dir),
    ESP_ELFSYM_EXPORT(lmacIsIdle),
    ESP_ELFSYM_EXPORT(esp_mpi_enable_hardware_hw_op),
    ESP_ELFSYM_EXPORT(esp_cpu_wait_for_intr),
    ESP_ELFSYM_EXPORT(I2C_CONTROLLER_TYPE),
    ESP_ELFSYM_EXPORT(phy_byte_to_word),
    ESP_ELFSYM_EXPORT(wifi_menuconfig_init),
    ESP_ELFSYM_EXPORT(lv_obj_is_layout_positioned),
    ESP_ELFSYM_EXPORT(lv_point_from_precise),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_deinit),
    ESP_ELFSYM_EXPORT(__clzdi2),
    ESP_ELFSYM_EXPORT(usb_msc_eject),
    ESP_ELFSYM_EXPORT(esp_timer_impl_early_init),
    ESP_ELFSYM_EXPORT(phy_txiq_cal_init),
    ESP_ELFSYM_EXPORT(_ZTVSt23__codecvt_abstract_baseIDsDu10_mbstate_tE),
    ESP_ELFSYM_EXPORT(esp_vfs_readdir),
    ESP_ELFSYM_EXPORT(lv_tabview_get_tab_count),
    ESP_ELFSYM_EXPORT(lv_timer_set_user_data),
    ESP_ELFSYM_EXPORT(httpd_sess_close_lru),
    ESP_ELFSYM_EXPORT(port_xSchedulerRunning),
    ESP_ELFSYM_EXPORT(__unorddf2),
    ESP_ELFSYM_EXPORT(os_get_random),
    ESP_ELFSYM_EXPORT(hal_attenna_init),
    ESP_ELFSYM_EXPORT(_lock_init_recursive),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_prep_write),
    ESP_ELFSYM_EXPORT(strncpy),
    ESP_ELFSYM_EXPORT(lmacProcessCollisions),
    ESP_ELFSYM_EXPORT(ds_hal_start),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_sniff_sco_wifi_scan),
    ESP_ELFSYM_EXPORT(_ZTVSt12length_error),
    ESP_ELFSYM_EXPORT(sys_arch_unprotect),
    ESP_ELFSYM_EXPORT(spi_flash_set_erasing_flag),
    ESP_ELFSYM_EXPORT(__wrap___cxa_allocate_exception),
    ESP_ELFSYM_EXPORT(__mulPow5InvDivPow2),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_intr_disable),
    ESP_ELFSYM_EXPORT(conn_stack_enableChanMapUpdCompVsEvent),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_yield),
    ESP_ELFSYM_EXPORT(lmacAdjustTimestamp),
    ESP_ELFSYM_EXPORT(lv_anim_set_delay),
    ESP_ELFSYM_EXPORT(nan_ndc_start_process),
    ESP_ELFSYM_EXPORT(trc_deinit),
    ESP_ELFSYM_EXPORT(audio_stream_read),
    ESP_ELFSYM_EXPORT(ble_sm_sc_public_key_exec),
    ESP_ELFSYM_EXPORT(adv_hci_clearLegacyAdv),
    ESP_ELFSYM_EXPORT(__cxa_guard_dummy),
    ESP_ELFSYM_EXPORT(bundle_clone),
    ESP_ELFSYM_EXPORT(npl_funcs),
    ESP_ELFSYM_EXPORT(strndup),
    ESP_ELFSYM_EXPORT(r_ble_phy_max_data_pdu_pyld),
    ESP_ELFSYM_EXPORT(crypto_bignum_mulmod),
    ESP_ELFSYM_EXPORT(display_init),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_wifi_conn),
    ESP_ELFSYM_EXPORT(esp_log_cache_get_level),
    ESP_ELFSYM_EXPORT(service_manager_remove),
    ESP_ELFSYM_EXPORT(audio_stream_set_mute),
    ESP_ELFSYM_EXPORT(ble_hs_hci_set_hci_supported_cmd),
    ESP_ELFSYM_EXPORT(i2c_select_periph_clock),
    ESP_ELFSYM_EXPORT(esp_crypto_aes_gcm_decrypt_setup),
    ESP_ELFSYM_EXPORT(pm_disconnected_sleep_delay_timeout_process),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_set_nav_action),
    ESP_ELFSYM_EXPORT(ic_set_he_rts_threshold_bytes_tab),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_modem_wakeup_disable),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_mul_mpi),
    ESP_ELFSYM_EXPORT(_Z26app_metadata_is_valid_nameRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(mbedtls_aes_xts_setkey_dec),
    ESP_ELFSYM_EXPORT(ant_rx_cfg),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_opaque_info),
    ESP_ELFSYM_EXPORT(tcp_connect),
    ESP_ELFSYM_EXPORT(cJSON_IsArray),
    ESP_ELFSYM_EXPORT(esp_coex_common_clk_slowclk_cal_get_wrapper),
    ESP_ELFSYM_EXPORT(wpa_sm_set_assoc_rsnxe),
    ESP_ELFSYM_EXPORT(eloop_register_timeout),
    ESP_ELFSYM_EXPORT(usb_host_hid_is_connected),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_sca_req_rsp_make),
    ESP_ELFSYM_EXPORT(spicommon_cs_initialize),
    ESP_ELFSYM_EXPORT(ble_store_key_from_value_sec),
    ESP_ELFSYM_EXPORT(get_max_conts_miss_beacon_sp_count),
    ESP_ELFSYM_EXPORT(phy_tx_pwctrl_init_new),
    ESP_ELFSYM_EXPORT(ethernet_input),
    ESP_ELFSYM_EXPORT(lvgl_ppa_delete),
    ESP_ELFSYM_EXPORT(coex_schm_bt_sniff_sco_wifi_connecting),
    ESP_ELFSYM_EXPORT(lv_switch_class),
    ESP_ELFSYM_EXPORT(mbedtls_psa_cipher_set_iv),
    ESP_ELFSYM_EXPORT(esp_ble_controller_lib_check),
    ESP_ELFSYM_EXPORT(lv_scale_class),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_find_info_idata),
    ESP_ELFSYM_EXPORT(lv_ll_move_before),
    ESP_ELFSYM_EXPORT(lv_anim_path_ease_out),
    ESP_ELFSYM_EXPORT(assoc_ie_buf),
    ESP_ELFSYM_EXPORT(dtm_hci_connlessIqEvtReport),
    ESP_ELFSYM_EXPORT(hal_he_bssid_deinit),
    ESP_ELFSYM_EXPORT(service_paths_get_user_data_directory),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_sub_abs),
    ESP_ELFSYM_EXPORT(_Z34app_metadata_is_valid_version_codeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx1110moneypunctIwLb0EEE),
    ESP_ELFSYM_EXPORT(hal_get_time_to_sta_next_tbtt),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_notify_node_sae_auth_done),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_hci_set_adv_report_flow_ctrl),
    ESP_ELFSYM_EXPORT(coex_schm_unlock),
    ESP_ELFSYM_EXPORT(r_ble_hw_encrypt_block),
    ESP_ELFSYM_EXPORT(wDev_BeaconMemory_Init),
    ESP_ELFSYM_EXPORT(mbedtls_ct_memcpy_offset),
    ESP_ELFSYM_EXPORT(tcp_init),
    ESP_ELFSYM_EXPORT(retrans_timer),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_find_type_value),
    ESP_ELFSYM_EXPORT(lv_label_is_char_under_pos),
    ESP_ELFSYM_EXPORT(r_ble_ll_channel_assess),
    ESP_ELFSYM_EXPORT(phy_is_low_rate_enabled),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_dup_check_legacy),
    ESP_ELFSYM_EXPORT(os_mempool_flags_set),
    ESP_ELFSYM_EXPORT(g_exc_frames),
    ESP_ELFSYM_EXPORT(ble_random_adv_env_p),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_data_base_deinit),
    ESP_ELFSYM_EXPORT(hal_enable_tsf_timer),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_color_to_al88),
    ESP_ELFSYM_EXPORT(xRingbufferGetCurFreeSize),
    ESP_ELFSYM_EXPORT(ieee80211_parse_mbssid),
    ESP_ELFSYM_EXPORT(_i2c_hal_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_env_init),
    ESP_ELFSYM_EXPORT(lv_style_transition_dsc_init),
    ESP_ELFSYM_EXPORT(lvgl_is_running),
    ESP_ELFSYM_EXPORT(psa_validate_unstructured_key_bit_size),
    ESP_ELFSYM_EXPORT(bluetooth_get_device_name),
    ESP_ELFSYM_EXPORT(r_ble_ll_qa_enable),
    ESP_ELFSYM_EXPORT(r_ble_controller_deinit),
    ESP_ELFSYM_EXPORT(lv_textarea_get_password_bullet),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_set_rpa_tmo),
    ESP_ELFSYM_EXPORT(esp_heap_adjust_alignment_to_hw),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_random),
    ESP_ELFSYM_EXPORT(lv_dropdownlist_class),
    ESP_ELFSYM_EXPORT(wifi_scan_restore_random_mac),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_delete_and_resch),
    ESP_ELFSYM_EXPORT(uart_controller_read_until),
    ESP_ELFSYM_EXPORT(esp_clk_cpu_freq),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_deinit),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_list_search),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service11displayidle18MystifyScreensaverE),
    ESP_ELFSYM_EXPORT(ble_mqueue_deinit),
    ESP_ELFSYM_EXPORT(lv_indev_get_scroll_dir),
    ESP_ELFSYM_EXPORT(esp_coex_common_semphr_create_wrapper),
    ESP_ELFSYM_EXPORT(coex_schm_external_coex_wifi_default_rxonly),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_priority_increase),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_refresh_nrpa),
    ESP_ELFSYM_EXPORT(esp_psram_mspi_mb_init),
    ESP_ELFSYM_EXPORT(tc_cmac_erase),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx118messagesIcE2idE),
    ESP_ELFSYM_EXPORT(gpio_controller_get_controller_context),
    ESP_ELFSYM_EXPORT(lv_obj_get_click_area),
    ESP_ELFSYM_EXPORT(lv_group_set_editing),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_BLOCK_KEY1),
    ESP_ELFSYM_EXPORT(hal_mac_ftm_get_t3),
    ESP_ELFSYM_EXPORT(esp_phy_update_country_info),
    ESP_ELFSYM_EXPORT(ip4_route_src),
    ESP_ELFSYM_EXPORT(phy_get_rx_sig_pwr),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx119money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEEE),
    ESP_ELFSYM_EXPORT(lv_obj_get_self_height),
    ESP_ELFSYM_EXPORT(lv_bar_get_mode),
    ESP_ELFSYM_EXPORT(pmu_hp_system_digital_param_default),
    ESP_ELFSYM_EXPORT(esp_event_handler_instance_unregister),
    ESP_ELFSYM_EXPORT(i2c_master_transmit_receive),
    ESP_ELFSYM_EXPORT(rcAttach),
    ESP_ELFSYM_EXPORT(_Z9gps_probeP6Device),
    ESP_ELFSYM_EXPORT(__kernel_tanf),
    ESP_ELFSYM_EXPORT(strxfrm),
    ESP_ELFSYM_EXPORT(service_manager_start),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_set_calc_verify_md),
    ESP_ELFSYM_EXPORT(ble_sm_connection_broken),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_data_cache),
    ESP_ELFSYM_EXPORT(esp_now_get_version),
    ESP_ELFSYM_EXPORT(pm_parse_beacon),
    ESP_ELFSYM_EXPORT(ble_random_init_env_p),
    ESP_ELFSYM_EXPORT(ble_gattc_disc_all_dscs),
    ESP_ELFSYM_EXPORT(phy_set_chan_misc_new),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_set_random_addr),
    ESP_ELFSYM_EXPORT(vRingbufferReturnItemFromISR),
    ESP_ELFSYM_EXPORT(spi_hal_user_start),
    ESP_ELFSYM_EXPORT(r_hal_timer_set_exp_tick),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_check_privkey),
    ESP_ELFSYM_EXPORT(__fixunsdfdi),
    ESP_ELFSYM_EXPORT(bundle_opt_bool),
    ESP_ELFSYM_EXPORT(__lock___arc4random_mutex),
    ESP_ELFSYM_EXPORT(mbedtls_psa_aead_encrypt),
    ESP_ELFSYM_EXPORT(lv_obj_get_transformed_area),
    ESP_ELFSYM_EXPORT(expm1),
    ESP_ELFSYM_EXPORT(__wtime_mon),
    ESP_ELFSYM_EXPORT(esp_coex_common_int_disable_wrapper),
    ESP_ELFSYM_EXPORT(esp_sha256_starts),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetINSt7__cxx118numpunctIcEEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(esp_wifi_connect),
    ESP_ELFSYM_EXPORT(lv_style_set_outline_pad),
    ESP_ELFSYM_EXPORT(pvalloc),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_connect_internal),
    ESP_ELFSYM_EXPORT(btdm_broker_deinit),
    ESP_ELFSYM_EXPORT(lv_obj_get_x),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_sub),
    ESP_ELFSYM_EXPORT(memp_malloc),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_pre_process),
    ESP_ELFSYM_EXPORT(hal_he_set_mac_delay),
    ESP_ELFSYM_EXPORT(spi_hal_fetch_result),
    ESP_ELFSYM_EXPORT(phy_rxevm_reset_mem),
    ESP_ELFSYM_EXPORT(dtm_hal_module_env_p),
    ESP_ELFSYM_EXPORT(file_system_mount),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_image_to_i1),
    ESP_ELFSYM_EXPORT(ieee80211_get_key),
    ESP_ELFSYM_EXPORT(adc_load_hw_calibration_chan_compens),
    ESP_ELFSYM_EXPORT(ftm_is_initiator_supported),
    ESP_ELFSYM_EXPORT(tcp_err),
    ESP_ELFSYM_EXPORT(r_hal_timer_init),
    ESP_ELFSYM_EXPORT(g_rmac_scan_saved_mac),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_wait_idle),
    ESP_ELFSYM_EXPORT(etharp_remove_static_entry),
    ESP_ELFSYM_EXPORT(ic_ebuf_alloc),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_max_height),
    ESP_ELFSYM_EXPORT(__retarget_lock_close),
    ESP_ELFSYM_EXPORT(scalbnf),
    ESP_ELFSYM_EXPORT(i2s_channel_reconfig_std_slot),
    ESP_ELFSYM_EXPORT(lodepng_crc32),
    ESP_ELFSYM_EXPORT(httpd_default_send),
    ESP_ELFSYM_EXPORT(uart_hal_set_parity),
    ESP_ELFSYM_EXPORT(spicommon_dma_chan_free),
    ESP_ELFSYM_EXPORT(ieee80211_add_dh_param),
    ESP_ELFSYM_EXPORT(_ZTVSt7collateIwE),
    ESP_ELFSYM_EXPORT(esp_log_util_cvt_hex),
    ESP_ELFSYM_EXPORT(f_mkfs),
    ESP_ELFSYM_EXPORT(spi_hal_deinit),
    ESP_ELFSYM_EXPORT(g_offchan_ctx),
    ESP_ELFSYM_EXPORT(ble_l2cap_deinit),
    ESP_ELFSYM_EXPORT(ic_set_sleep_wait_broadcast_data_time),
    ESP_ELFSYM_EXPORT(__cxa_end_catch),
    ESP_ELFSYM_EXPORT(sae_pk_set_password),
    ESP_ELFSYM_EXPORT(_ZSt13__int_to_charIcyEiPT_T0_PKS0_St13_Ios_Fmtflagsb),
    ESP_ELFSYM_EXPORT(platform_esp32_module),
    ESP_ELFSYM_EXPORT(unregister_ieee80211_action_vendor_spec_cb),
    ESP_ELFSYM_EXPORT(esp_partition_write),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_le_read_local_features),
    ESP_ELFSYM_EXPORT(mpi_hal_enable_search),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_process_all_recv_pkt),
    ESP_ELFSYM_EXPORT(esp_crypto_sha_aes_lock_acquire),
    ESP_ELFSYM_EXPORT(lv_tabview_set_tab_bar_position),
    ESP_ELFSYM_EXPORT(esp_mesh_get_running_active_duty_cycle),
    ESP_ELFSYM_EXPORT(ble_gap_preempt),
    ESP_ELFSYM_EXPORT(itwt_stop_process),
    ESP_ELFSYM_EXPORT(sinhf),
    ESP_ELFSYM_EXPORT(sdspi_host_do_transaction),
    ESP_ELFSYM_EXPORT(cnx_rc_update_rssi),
    ESP_ELFSYM_EXPORT(hal_mac_dump_rx_evm),
    ESP_ELFSYM_EXPORT(vTaskGenericNotifyGiveFromISR),
    ESP_ELFSYM_EXPORT(hal_mac_deinit),
    ESP_ELFSYM_EXPORT(lv_style_set_shadow_color),
    ESP_ELFSYM_EXPORT(adc_oneshot_hal_setup),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_mul),
    ESP_ELFSYM_EXPORT(app_paths_get_assets_path),
    ESP_ELFSYM_EXPORT(lv_obj_set_user_data),
    ESP_ELFSYM_EXPORT(modem_clock_hal_enable_modem_common_fe_clock),
    ESP_ELFSYM_EXPORT(ble_store_read_csfc),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_md_alg_from_hash),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_session_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_event_tx_pkt),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_npl_reset),
    ESP_ELFSYM_EXPORT(esp_transport_init_foundation_transport),
    ESP_ELFSYM_EXPORT(memp_FRAG_PBUF),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_update_new_chan_map),
    ESP_ELFSYM_EXPORT(ppCalTxopDur),
    ESP_ELFSYM_EXPORT(pm_tbtt_process),
    ESP_ELFSYM_EXPORT(app_module),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_connless_iq_report),
    ESP_ELFSYM_EXPORT(lv_dropdown_set_text),
    ESP_ELFSYM_EXPORT(pcTaskGetName),
    ESP_ELFSYM_EXPORT(wpa_config_done),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceISt6atomicIiESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_io_del),
    ESP_ELFSYM_EXPORT(hid_appearance),
    ESP_ELFSYM_EXPORT(wpa_sm_set_key),
    ESP_ELFSYM_EXPORT(mld6_stop),
    ESP_ELFSYM_EXPORT(ppProcessWaitingQueue),
    ESP_ELFSYM_EXPORT(uart_read_bytes),
    ESP_ELFSYM_EXPORT(display_disp_on_off),
    ESP_ELFSYM_EXPORT(hci_transport_host_callback_register),
    ESP_ELFSYM_EXPORT(lv_spinbox_step_prev),
    ESP_ELFSYM_EXPORT(rtc_gpio_pullup_dis),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_msys_deinit),
    ESP_ELFSYM_EXPORT(hal_wdev_timer_set_target),
    ESP_ELFSYM_EXPORT(atof),
    ESP_ELFSYM_EXPORT(__cxa_guard_acquire),
    ESP_ELFSYM_EXPORT(ecc_hal_write_mul_param),
    ESP_ELFSYM_EXPORT(ble_mqueue_get),
    ESP_ELFSYM_EXPORT(ble_gattc_timer),
    ESP_ELFSYM_EXPORT(wifi_get_ap_list_process),
    ESP_ELFSYM_EXPORT(xQueueCreateWithCaps),
    ESP_ELFSYM_EXPORT(fseeko),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_coex_dpc_calc_pti_update_itvl),
    ESP_ELFSYM_EXPORT(__isinff),
    ESP_ELFSYM_EXPORT(phy_wifi_track_tx_power_new),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_configure),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_set_tx_power_enable),
    ESP_ELFSYM_EXPORT(__double_computeInvPow5),
    ESP_ELFSYM_EXPORT(djb2_data),
    ESP_ELFSYM_EXPORT(g_rmac_scan_saved),
    ESP_ELFSYM_EXPORT(pwr_hal_set_modem_state_rxend_exit_code),
    ESP_ELFSYM_EXPORT(esp_clk_init),
    ESP_ELFSYM_EXPORT(crypt_prewarm),
    ESP_ELFSYM_EXPORT(lv_label_set_text),
    ESP_ELFSYM_EXPORT(esp_bt_controller_disable),
    ESP_ELFSYM_EXPORT(ble_sm_sc_confirm_exec),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_rx_pkt_in),
    ESP_ELFSYM_EXPORT(lvgl_software_keyboard_get_last),
    ESP_ELFSYM_EXPORT(pm_beacon_monitor_tbtt_timeout_process),
    ESP_ELFSYM_EXPORT(pau_regdma_set_entry_link_addr),
    ESP_ELFSYM_EXPORT(lv_tabview_class),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_env_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_df_recycle_cte_buffer_with_check),
    ESP_ELFSYM_EXPORT(phy_rf_cal_data_backup),
    ESP_ELFSYM_EXPORT(sae_pk_buf_shift_left_19),
    ESP_ELFSYM_EXPORT(lvgl_port_deinit),
    ESP_ELFSYM_EXPORT(npl_freertos_time_ms_to_ticks32),
    ESP_ELFSYM_EXPORT(gdma_get_alignment_constraints),
    ESP_ELFSYM_EXPORT(lodepng_state_cleanup),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_finished),
    ESP_ELFSYM_EXPORT(pcl_hci_setRssiThresh),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_hash_check_in_table),
    ESP_ELFSYM_EXPORT(usb_device_controller_allocate_interfaces),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_init),
    ESP_ELFSYM_EXPORT(tcp_recved),
    ESP_ELFSYM_EXPORT(dhcps_set_option_info),
    ESP_ELFSYM_EXPORT(lmacEndRetryAMPDUFail),
    ESP_ELFSYM_EXPORT(crypt_generate_iv),
    ESP_ELFSYM_EXPORT(aes_hal_setkey),
    ESP_ELFSYM_EXPORT(hal_agreement_del_rx_ba),
    ESP_ELFSYM_EXPORT(esp_aes_init),
    ESP_ELFSYM_EXPORT(xTimerGenericCommand),
    ESP_ELFSYM_EXPORT(wpa3_hostap_post_evt),
    ESP_ELFSYM_EXPORT(g_twdt_isr),
    ESP_ELFSYM_EXPORT(memp_pools),
    ESP_ELFSYM_EXPORT(net80211_printf),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_dup_update_legacy),
    ESP_ELFSYM_EXPORT(sdmmc_mmc_switch),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_a2dp_wifi_connecting),
    ESP_ELFSYM_EXPORT(hid_kb_input_handle),
    ESP_ELFSYM_EXPORT(phy_pbus_force_mode),
    ESP_ELFSYM_EXPORT(ppRemoveHTC),
    ESP_ELFSYM_EXPORT(wifi_get_init_state),
    ESP_ELFSYM_EXPORT(ic_ebuf_recycle_rx),
    ESP_ELFSYM_EXPORT(chm_set_home_channel),
    ESP_ELFSYM_EXPORT(adiList_stack_enable),
    ESP_ELFSYM_EXPORT(pm_stop_twt),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_list_reset),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_check_failed),
    ESP_ELFSYM_EXPORT(uECC_make_key),
    ESP_ELFSYM_EXPORT(phy_pbus_reg_store),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_y),
    ESP_ELFSYM_EXPORT(module_construct_add_start),
    ESP_ELFSYM_EXPORT(ip_chksum_pseudo),
    ESP_ELFSYM_EXPORT(spi_hal_setup_device),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_callout_env_init_for_sm),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_deinit),
    ESP_ELFSYM_EXPORT(i2s_platform_acquire_occupation),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_scan_set_enable),
    ESP_ELFSYM_EXPORT(inet_chksum_pbuf),
    ESP_ELFSYM_EXPORT(lvgl_get_launcher_icon_font),
    ESP_ELFSYM_EXPORT(lv_cache_entry_delete),
    ESP_ELFSYM_EXPORT(lv_obj_readjust_scroll),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_sig_from_pk_alg),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_reset_in_pointers),
    ESP_ELFSYM_EXPORT(lv_palette_lighten),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_common_init),
    ESP_ELFSYM_EXPORT(regdma_link_new_branch_wait),
    ESP_ELFSYM_EXPORT(ieee80211_user_ie_init),
    ESP_ELFSYM_EXPORT(cJSON_Delete),
    ESP_ELFSYM_EXPORT(lv_area_set),
    ESP_ELFSYM_EXPORT(r_ble_ll_init_rx_pkt_in),
    ESP_ELFSYM_EXPORT(esp_netif_get_io_driver),
    ESP_ELFSYM_EXPORT(_Z19getUrlFromCrashDataB5cxx11v),
    ESP_ELFSYM_EXPORT(crypt_module),
    ESP_ELFSYM_EXPORT(_ZThn8_NSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED1Ev),
    ESP_ELFSYM_EXPORT(r_DB_data_node_create),
    ESP_ELFSYM_EXPORT(sdspi_host_io_int_enable),
    ESP_ELFSYM_EXPORT(r_ble_ll_is_valid_own_addr_type),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_mtu),
    ESP_ELFSYM_EXPORT(r_put_le64),
    ESP_ELFSYM_EXPORT(ble_gattc_disc_all_chrs),
    ESP_ELFSYM_EXPORT(cache_hal_unfreeze),
    ESP_ELFSYM_EXPORT(gmtime_r),
    ESP_ELFSYM_EXPORT(lv_draw_line_dsc_init),
    ESP_ELFSYM_EXPORT(pm_coex_recalculate_wifi_time_slice),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_set_adv_data),
    ESP_ELFSYM_EXPORT(sae_pk_get_be19),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_outline_width),
    ESP_ELFSYM_EXPORT(r_DB_data_list_destroy),
    ESP_ELFSYM_EXPORT(httpd_recv_with_opt),
    ESP_ELFSYM_EXPORT(_fcntl_r),
    ESP_ELFSYM_EXPORT(phy_wifi_get_tx_gain),
    ESP_ELFSYM_EXPORT(dragonfly_min_pwe_loop_iter),
    ESP_ELFSYM_EXPORT(wpa_config_bss),
    ESP_ELFSYM_EXPORT(esp_log_impl_unlock),
    ESP_ELFSYM_EXPORT(sdmmc_init_mmc_read_ext_csd),
    ESP_ELFSYM_EXPORT(r_ble_phy_init_registers),
    ESP_ELFSYM_EXPORT(rtcio_hal_set_direction),
    ESP_ELFSYM_EXPORT(lv_display_set_driver_data),
    ESP_ELFSYM_EXPORT(sae_set_group),
    ESP_ELFSYM_EXPORT(spi_flash_chip_boya_get_caps),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_max_out_record_payload),
    ESP_ELFSYM_EXPORT(ble_hs_conn_delete_chan),
    ESP_ELFSYM_EXPORT(esp_vfs_fstat),
    ESP_ELFSYM_EXPORT(lv_textarea_add_text),
    ESP_ELFSYM_EXPORT(esp_bt_rtc_slow_clk_select),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_handle_get_by_sch),
    ESP_ELFSYM_EXPORT(esp_hmac_verify_finish_opaque),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_link_ctrl_cmd_proc),
    ESP_ELFSYM_EXPORT(pm_on_data_tx),
    ESP_ELFSYM_EXPORT(psa_key_derivation_input_key),
    ESP_ELFSYM_EXPORT(lv_ll_init),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_conn_wifi_scan),
    ESP_ELFSYM_EXPORT(wpa_auth_sta_get_pmksa),
    ESP_ELFSYM_EXPORT(pm_twt_process),
    ESP_ELFSYM_EXPORT(lv_dropdown_is_open),
    ESP_ELFSYM_EXPORT(esp_aes_dma_start),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_read_txpwr),
    ESP_ELFSYM_EXPORT(esp_http_client_get_username),
    ESP_ELFSYM_EXPORT(hci_stack_enableSetVsEvtMaskVsCmd),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_bitstring),
    ESP_ELFSYM_EXPORT(f_write),
    ESP_ELFSYM_EXPORT(_ZTVSt13basic_filebufIcSt11char_traitsIcEE),
    ESP_ELFSYM_EXPORT(wifi_wps_is_started),
    ESP_ELFSYM_EXPORT(udp_sendto_if_src),
    ESP_ELFSYM_EXPORT(esp_partition_next),
    ESP_ELFSYM_EXPORT(ieee80211_mt_key_is_mask_zero),
    ESP_ELFSYM_EXPORT(_Z15getRtcCrashDatav),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_a2dp_wifi_scan),
    ESP_ELFSYM_EXPORT(rcSetBarRate),
    ESP_ELFSYM_EXPORT(wifi_send_mgmt_frame),
    ESP_ELFSYM_EXPORT(esp_cache_err_get_cpuid),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_hci_update_adv_report_flow_ctrl),
    ESP_ELFSYM_EXPORT(_Z22ble_spp_start_internalP6Device),
    ESP_ELFSYM_EXPORT(httpd_sess_new),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_periodic_enable),
    ESP_ELFSYM_EXPORT(r_ble_cte_bb_update_config),
    ESP_ELFSYM_EXPORT(pwr_hal_select_wifimac_regdma_link),
    ESP_ELFSYM_EXPORT(pmksa_cache_get),
    ESP_ELFSYM_EXPORT(ieee80211_crypto_bip_encrypt),
    ESP_ELFSYM_EXPORT(lodepng_chunk_append),
    ESP_ELFSYM_EXPORT(esp_unregister_shutdown_handler),
    ESP_ELFSYM_EXPORT(ets_isr_unmask),
    ESP_ELFSYM_EXPORT(ff_diskio_register_raw_partition),
    ESP_ELFSYM_EXPORT(mbedtls_pk_sign_ext),
    ESP_ELFSYM_EXPORT(lv_line_create),
    ESP_ELFSYM_EXPORT(esp_transport_tcp_init),
    ESP_ELFSYM_EXPORT(_ZTVSt15basic_streambufIcSt11char_traitsIcEE),
    ESP_ELFSYM_EXPORT(ble_sm_incr_peer_sign_counter),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_BLOCK_KEY5),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_append),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ecdsa_verify_hash),
    ESP_ELFSYM_EXPORT(file_mutex_unlock),
    ESP_ELFSYM_EXPORT(BLUETOOTH_TYPE),
    ESP_ELFSYM_EXPORT(getle32),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_rx_pkt_in),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_gen_key),
    ESP_ELFSYM_EXPORT(ble_lll_df_env_p),
    ESP_ELFSYM_EXPORT(scan_reset_default_app_params),
    ESP_ELFSYM_EXPORT(ieee80211_vht_node_init),
    ESP_ELFSYM_EXPORT(pm_attach),
    ESP_ELFSYM_EXPORT(cnx_assoc_timeout),
    ESP_ELFSYM_EXPORT(twt_update_inactive_time_secs),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_opa),
    ESP_ELFSYM_EXPORT(start_internal),
    ESP_ELFSYM_EXPORT(ledc_set_duty),
    ESP_ELFSYM_EXPORT(r_os_mbuf_cmpm),
    ESP_ELFSYM_EXPORT(r_ble_phy_ccm_encrypt_block),
    ESP_ELFSYM_EXPORT(esp32_ble_midi_driver),
    ESP_ELFSYM_EXPORT(phy_get_rc_dout),
    ESP_ELFSYM_EXPORT(i2c_controller_write_read),
    ESP_ELFSYM_EXPORT(SysTickIsrHandler),
    ESP_ELFSYM_EXPORT(bt_agc_gain_offset),
    ESP_ELFSYM_EXPORT(ble_uuid_to_any),
    ESP_ELFSYM_EXPORT(tcp_ticks),
    ESP_ELFSYM_EXPORT(wpa_set_passphrase),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_calc_itvl_ticks),
    ESP_ELFSYM_EXPORT(ieee80211_amsdu_encap_check),
    ESP_ELFSYM_EXPORT(_ZTVSt9money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE),
    ESP_ELFSYM_EXPORT(bt_bb_tx_cca_set),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_data_base_clear),
    ESP_ELFSYM_EXPORT(psa_key_derivation_abort),
    ESP_ELFSYM_EXPORT(s_wifi_nvs),
    ESP_ELFSYM_EXPORT(vApplicationGetTimerTaskMemory),
    ESP_ELFSYM_EXPORT(adc_oneshot_new_unit),
    ESP_ELFSYM_EXPORT(sys_mbox_trypost),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_grid_row_dsc_array),
    ESP_ELFSYM_EXPORT(regdma_find_next_module_link_head),
    ESP_ELFSYM_EXPORT(r_get_le32),
    ESP_ELFSYM_EXPORT(phy_rx_dco_cal_1step_new),
    ESP_ELFSYM_EXPORT(_ZTVSt5ctypeIwE),
    ESP_ELFSYM_EXPORT(pm_enable_twt_keep_alive),
    ESP_ELFSYM_EXPORT(wDev_ProcessRxData_NAN_Interface_Hook),
    ESP_ELFSYM_EXPORT(hal_set_tb_pti),
    ESP_ELFSYM_EXPORT(esp_ptr_dma_ext_capable),
    ESP_ELFSYM_EXPORT(httpd_resp_set_type),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_buffer_software_own),
    ESP_ELFSYM_EXPORT(lvgl_arch_start),
    ESP_ELFSYM_EXPORT(__kernel_cosf),
    ESP_ELFSYM_EXPORT(ledc_set_fade_time_and_start),
    ESP_ELFSYM_EXPORT(spi_device_release_bus),
    ESP_ELFSYM_EXPORT(test_tx_succ_statistics),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_indicate),
    ESP_ELFSYM_EXPORT(bt_agc_gain_set),
    ESP_ELFSYM_EXPORT(r_os_mbuf_pullup),
    ESP_ELFSYM_EXPORT(tcp_alloc),
    ESP_ELFSYM_EXPORT(ic_add_rx_ba),
    ESP_ELFSYM_EXPORT(mbedtls_x509_oid_get_md_alg),
    ESP_ELFSYM_EXPORT(r_ble_lll_current_tx_info_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_path_loss_mon),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_ciphersuite_sig_pk_psa_usage),
    ESP_ELFSYM_EXPORT(_ZTVSt8messagesIwE),
    ESP_ELFSYM_EXPORT(rx11NRate2AMPDULimit),
    ESP_ELFSYM_EXPORT(lv_timer_core_deinit),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_wifi_conn),
    ESP_ELFSYM_EXPORT(ble_hs_hci_util_rand),
    ESP_ELFSYM_EXPORT(gpio_pullup_en),
    ESP_ELFSYM_EXPORT(multi_heap_free_size),
    ESP_ELFSYM_EXPORT(ble_hs_adv_parse_free),
    ESP_ELFSYM_EXPORT(esp_mpi_disable_hardware_hw_op),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_time_sync_set),
    ESP_ELFSYM_EXPORT(qrcode_getModule),
    ESP_ELFSYM_EXPORT(lv_obj_delete),
    ESP_ELFSYM_EXPORT(dispatcher_alloc),
    ESP_ELFSYM_EXPORT(esp_chip_info),
    ESP_ELFSYM_EXPORT(wcscoll),
    ESP_ELFSYM_EXPORT(gdma_register_tx_event_callbacks),
    ESP_ELFSYM_EXPORT(hal_mac_color_get_bitmap),
    ESP_ELFSYM_EXPORT(ppUnregisterTxCallback),
    ESP_ELFSYM_EXPORT(esp_sha512_starts),
    ESP_ELFSYM_EXPORT(modem_clock_module_enable),
    ESP_ELFSYM_EXPORT(lv_image_header_cache_drop),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_handshake_msg_ext),
    ESP_ELFSYM_EXPORT(thread_set_state_callback),
    ESP_ELFSYM_EXPORT(timer_reset),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_update_anchor_check_by_adva),
    ESP_ELFSYM_EXPORT(wifi_sta_reg_rxcb),
    ESP_ELFSYM_EXPORT(esp_wifi_get_macaddr_internal),
    ESP_ELFSYM_EXPORT(coex_schm_status_bit_set),
    ESP_ELFSYM_EXPORT(phy_freq_mem_change_5g_),
    ESP_ELFSYM_EXPORT(_ZTVSt12system_error),
    ESP_ELFSYM_EXPORT(spi_flash_hal_common_command),
    ESP_ELFSYM_EXPORT(mspi_timing_change_speed_mode_cache_safe),
    ESP_ELFSYM_EXPORT(multi_heap_minimum_free_size),
    ESP_ELFSYM_EXPORT(ip_addr_broadcast),
    ESP_ELFSYM_EXPORT(hal_get_tx_pwr),
    ESP_ELFSYM_EXPORT(_ZGVNSt10moneypunctIcLb0EE2idE),
    ESP_ELFSYM_EXPORT(ieee80211_set_phy_bw),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceIN2tt6PubSubINS0_7service5audio10AudioEventEEESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(sae_check_confirm_pk),
    ESP_ELFSYM_EXPORT(netif_poll),
    ESP_ELFSYM_EXPORT(phy_temp_to_power_new),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_disable),
    ESP_ELFSYM_EXPORT(hal_utils_calc_clk_div_frac_accurate),
    ESP_ELFSYM_EXPORT(gcmp),
    ESP_ELFSYM_EXPORT(f_fdisk),
    ESP_ELFSYM_EXPORT(get_act_lp_dbias),
    ESP_ELFSYM_EXPORT(ieee80211_ht_node_init),
    ESP_ELFSYM_EXPORT(pm_beacon_misc_process),
    ESP_ELFSYM_EXPORT(timer_stop),
    ESP_ELFSYM_EXPORT(lv_array_deinit),
    ESP_ELFSYM_EXPORT(get_vfs_upper_bound),
    ESP_ELFSYM_EXPORT(lv_cache_entry_dec_ref),
    ESP_ELFSYM_EXPORT(lv_cache_add),
    ESP_ELFSYM_EXPORT(hal_set_rx_ack_pti),
    ESP_ELFSYM_EXPORT(mbedtls_pk_ecc_set_pubkey),
    ESP_ELFSYM_EXPORT(wpa_ap_get_wpa_ie),
    ESP_ELFSYM_EXPORT(unsetenv),
    ESP_ELFSYM_EXPORT(psa_asymmetric_encrypt),
    ESP_ELFSYM_EXPORT(phy_get_dreg_1p1),
    ESP_ELFSYM_EXPORT(r_ble_lll_mgmmt_rxbuf_adjust_check),
    ESP_ELFSYM_EXPORT(buf_shift_right),
    ESP_ELFSYM_EXPORT(httpd_req_get_url_query_str),
    ESP_ELFSYM_EXPORT(hal_he_clr_tid_bitmap),
    ESP_ELFSYM_EXPORT(nimble_midi_api),
    ESP_ELFSYM_EXPORT(ic_del_key_all),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_select_card),
    ESP_ELFSYM_EXPORT(tlsf_block_size),
    ESP_ELFSYM_EXPORT(ets_timer_arm),
    ESP_ELFSYM_EXPORT(tcp_write),
    ESP_ELFSYM_EXPORT(psa_unregister_read_under_mutex),
    ESP_ELFSYM_EXPORT(lv_memcmp),
    ESP_ELFSYM_EXPORT(memspi_host_erase_sector),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_insert_txbuf_ahead),
    ESP_ELFSYM_EXPORT(test_mumimo_get_heltf_num),
    ESP_ELFSYM_EXPORT(cache_hal_vaddr_to_cache_level_id),
    ESP_ELFSYM_EXPORT(ble_hs_conn_free),
    ESP_ELFSYM_EXPORT(mbedtls_aes_free),
    ESP_ELFSYM_EXPORT(tcp_listen_pcbs),
    ESP_ELFSYM_EXPORT(esp_wifi_is_wpa3_compatible_mode_enabled),
    ESP_ELFSYM_EXPORT(hal_get_sta_tsf),
    ESP_ELFSYM_EXPORT(pm_enable_active_timer),
    ESP_ELFSYM_EXPORT(pm_is_open),
    ESP_ELFSYM_EXPORT(esp_partition_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_alloc),
    ESP_ELFSYM_EXPORT(coex_schm_external_coex_wifi_scan),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_generic_data_init),
    ESP_ELFSYM_EXPORT(uxQueueGetQueueNumber),
    ESP_ELFSYM_EXPORT(unregister_fd),
    ESP_ELFSYM_EXPORT(wpa_auth_add_sae_pmkid),
    ESP_ELFSYM_EXPORT(esp_transport_tcp_set_interface_name),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_set_button_ctrl),
    ESP_ELFSYM_EXPORT(WIFI_TYPE),
    ESP_ELFSYM_EXPORT(regdma_find_link_by_id),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_conn_wifi_conn),
    ESP_ELFSYM_EXPORT(wpa_parse_wpa_ie_wpa),
    ESP_ELFSYM_EXPORT(display_reset),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_wifi_connecting),
    ESP_ELFSYM_EXPORT(lora_get_modulation),
    ESP_ELFSYM_EXPORT(lv_draw_arc),
    ESP_ELFSYM_EXPORT(esp_netif_set_mac),
    ESP_ELFSYM_EXPORT(esp_netif_add_to_list_unsafe),
    ESP_ELFSYM_EXPORT(psa_destroy_key),
    ESP_ELFSYM_EXPORT(wpa_supplicant_transition_disable),
    ESP_ELFSYM_EXPORT(pm_on_twt_force_tx),
    ESP_ELFSYM_EXPORT(pm_force_scan_unlock),
    ESP_ELFSYM_EXPORT(spi_flash_chip_list_check),
    ESP_ELFSYM_EXPORT(spi_flash_hal_suspend),
    ESP_ELFSYM_EXPORT(coex_schm_bt_inq_wifi_connecting),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx118numpunctIcEE),
    ESP_ELFSYM_EXPORT(cnx_csa_fn),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_alloc_link_state),
    ESP_ELFSYM_EXPORT(spi_flash_disable_cache),
    ESP_ELFSYM_EXPORT(hal_mac_enable_low_rate),
    ESP_ELFSYM_EXPORT(pm_disconnected_start),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_superversion_timer_cb),
    ESP_ELFSYM_EXPORT(spi_flash_enable_cache),
    ESP_ELFSYM_EXPORT(esp_wifi_cert_vht_mcs8),
    ESP_ELFSYM_EXPORT(iswcntrl),
    ESP_ELFSYM_EXPORT(ppAssembleAMPDU),
    ESP_ELFSYM_EXPORT(ble_gap_unpair),
    ESP_ELFSYM_EXPORT(esp_cmac_compute),
    ESP_ELFSYM_EXPORT(esp_wifi_init_internal),
    ESP_ELFSYM_EXPORT(GPIO_HOLD_MASK),
    ESP_ELFSYM_EXPORT(npl_freertos_sem_release),
    ESP_ELFSYM_EXPORT(lv_text_unicode_to_encoded),
    ESP_ELFSYM_EXPORT(ble_att_svr_prev_handle),
    ESP_ELFSYM_EXPORT(pm_sleep_delay_timeout_process),
    ESP_ELFSYM_EXPORT(lmacProcessTxComplete),
    ESP_ELFSYM_EXPORT(lvgl_get_text_font),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_stop),
    ESP_ELFSYM_EXPORT(__bufio_seek),
    ESP_ELFSYM_EXPORT(__bufio_setvbuf),
    ESP_ELFSYM_EXPORT(spi_flash_chip_winbond_read),
    ESP_ELFSYM_EXPORT(pm_tx_data_process),
    ESP_ELFSYM_EXPORT(s_ni),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_phy_update_proc_complete),
    ESP_ELFSYM_EXPORT(hal_init_imrsp_power),
    ESP_ELFSYM_EXPORT(phy_txdc_cal),
    ESP_ELFSYM_EXPORT(bt_agc_restart_set),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_ext_check_data_itvl),
    ESP_ELFSYM_EXPORT(i2s_controller_set_config),
    ESP_ELFSYM_EXPORT(mbedtls_md_hmac_finish),
    ESP_ELFSYM_EXPORT(xTaskAbortDelay),
    ESP_ELFSYM_EXPORT(esp_unregister_npl_funcs),
    ESP_ELFSYM_EXPORT(netif_loop_output),
    ESP_ELFSYM_EXPORT(btwt_setup_timeout_fn),
    ESP_ELFSYM_EXPORT(esp_vfs_lwip_sockets_register),
    ESP_ELFSYM_EXPORT(spi_flash_chip_gd_get_caps),
    ESP_ELFSYM_EXPORT(_ZTVSt15_Sp_counted_ptrIPN2tt7service11development18DevelopmentServiceELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(esp_pthread_set_cfg),
    ESP_ELFSYM_EXPORT(lv_array_push_back),
    ESP_ELFSYM_EXPORT(phy_get_i2c_mst0_mask),
    ESP_ELFSYM_EXPORT(sae_derive_pwe_from_pt_ecc),
    ESP_ELFSYM_EXPORT(memp_init),
    ESP_ELFSYM_EXPORT(esp_log_set_vprintf),
    ESP_ELFSYM_EXPORT(regdma_link_update_next),
    ESP_ELFSYM_EXPORT(ieee80211_get_robustmgtframe),
    ESP_ELFSYM_EXPORT(esp_mmu_map_reserve_block_with_caps),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_set_ext_con_params),
    ESP_ELFSYM_EXPORT(mbedtls_aes_crypt_ctr),
    ESP_ELFSYM_EXPORT(wifi_ap_reg_rxcb),
    ESP_ELFSYM_EXPORT(ampdu_dispatch_movement),
    ESP_ELFSYM_EXPORT(unlink),
    ESP_ELFSYM_EXPORT(__retarget_lock_try_acquire),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_draw_bitmap),
    ESP_ELFSYM_EXPORT(chm_set_snd_chan),
    ESP_ELFSYM_EXPORT(lv_timer_handler_run_in_period),
    ESP_ELFSYM_EXPORT(esp_mspi_pin_reserve),
    ESP_ELFSYM_EXPORT(ic_get_G6M_sched),
    ESP_ELFSYM_EXPORT(_on_exit),
    ESP_ELFSYM_EXPORT(lmacSetAcParam),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_top),
    ESP_ELFSYM_EXPORT(coex_schm_bt_conn_wifi_scan),
    ESP_ELFSYM_EXPORT(efuse_hal_get_disable_wafer_version_major),
    ESP_ELFSYM_EXPORT(xQueueReceiveFromISR),
    ESP_ELFSYM_EXPORT(r_hal_timer_set_cb),
    ESP_ELFSYM_EXPORT(lv_style_set_recolor),
    ESP_ELFSYM_EXPORT(esp_hmac_verify_finish_transparent),
    ESP_ELFSYM_EXPORT(_ZSt25__unguarded_linear_insertIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops14_Val_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SK_),
    ESP_ELFSYM_EXPORT(lv_obj_class_init_obj),
    ESP_ELFSYM_EXPORT(app_scheduler_stop),
    ESP_ELFSYM_EXPORT(esp_netif_tcpip_exec),
    ESP_ELFSYM_EXPORT(esp_timer_start_periodic),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_update_configuration),
    ESP_ELFSYM_EXPORT(fsetpos),
    ESP_ELFSYM_EXPORT(logf),
    ESP_ELFSYM_EXPORT(regdomain_table),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_pwr_ctrl_req),
    ESP_ELFSYM_EXPORT(r_ble_ll_read_supp_states),
    ESP_ELFSYM_EXPORT(wifi_create_sta),
    ESP_ELFSYM_EXPORT(btdm_broker_attach),
    ESP_ELFSYM_EXPORT(i2s_new_channel),
    ESP_ELFSYM_EXPORT(ble_store_util_status_rr),
    ESP_ELFSYM_EXPORT(ieee80211_pwrsave_txcb),
    ESP_ELFSYM_EXPORT(spi_flash_chip_gd_get_io_mode),
    ESP_ELFSYM_EXPORT(lodepng_info_cleanup),
    ESP_ELFSYM_EXPORT(ble_hs_conn_init),
    ESP_ELFSYM_EXPORT(omac1_aes_128),
    ESP_ELFSYM_EXPORT(netconn_alloc),
    ESP_ELFSYM_EXPORT(phy_get_rate_fcc_index),
    ESP_ELFSYM_EXPORT(esp_fill_random),
    ESP_ELFSYM_EXPORT(chm_acquire_lock),
    ESP_ELFSYM_EXPORT(btdm_broker_publish),
    ESP_ELFSYM_EXPORT(lv_theme_default_init),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_list_ciphersuites),
    ESP_ELFSYM_EXPORT(ble_random_mode_adv_exec_process_cb_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_remove),
    ESP_ELFSYM_EXPORT(pm_coex_schm_process),
    ESP_ELFSYM_EXPORT(tcp_rexmit_fast),
    ESP_ELFSYM_EXPORT(esp_hmac_import_key_opaque),
    ESP_ELFSYM_EXPORT(spi_device_polling_transmit),
    ESP_ELFSYM_EXPORT(phy_freq_set_reg),
    ESP_ELFSYM_EXPORT(modem_clock_hal_select_coex_lpclk_source),
    ESP_ELFSYM_EXPORT(wifi_get_access_point_state),
    ESP_ELFSYM_EXPORT(_ZSt8_DestroyIPN2tt3app4chat13StoredMessageEEvT_S5_),
    ESP_ELFSYM_EXPORT(ble_hs_startup_go),
    ESP_ELFSYM_EXPORT(esp_http_client_delete_all_headers),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_channel_reporting_ind),
    ESP_ELFSYM_EXPORT(sinh),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_reset),
    ESP_ELFSYM_EXPORT(__atod_engine),
    ESP_ELFSYM_EXPORT(r_ble_ll_ext_conn_create),
    ESP_ELFSYM_EXPORT(lv_event_get_count),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_transform_free),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service6espnow13EspNowServiceE),
    ESP_ELFSYM_EXPORT(raw_send),
    ESP_ELFSYM_EXPORT(ppDirectRecycleAmpdu),
    ESP_ELFSYM_EXPORT(phy_wakeup_init),
    ESP_ELFSYM_EXPORT(esp_partition_find_first_err),
    ESP_ELFSYM_EXPORT(esp_transport_poll_read),
    ESP_ELFSYM_EXPORT(wifi_set_channel),
    ESP_ELFSYM_EXPORT(igmp_start),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_piscan_wifi_connecting),
    ESP_ELFSYM_EXPORT(ieee80211_he_deattach),
    ESP_ELFSYM_EXPORT(coex_schm_bt_a2dp_wifi_conn),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_wait_data_ready),
    ESP_ELFSYM_EXPORT(_ZTVSt14basic_ofstreamIcSt11char_traitsIcEE),
    ESP_ELFSYM_EXPORT(_ZZNKSt7__cxx117collateIwE12do_transformEPKwS3_EN4_BufD1Ev),
    ESP_ELFSYM_EXPORT(misc_nvs_init),
    ESP_ELFSYM_EXPORT(lv_obj_scroll_by_bounded),
    ESP_ELFSYM_EXPORT(clear_ic_key),
    ESP_ELFSYM_EXPORT(pp_register_michael_mic_failure_cb),
    ESP_ELFSYM_EXPORT(uECC_vli_cmp_unsafe),
    ESP_ELFSYM_EXPORT(ble_transport_free),
    ESP_ELFSYM_EXPORT(xTaskGetTickCount),
    ESP_ELFSYM_EXPORT(lv_indev_set_cursor),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_le_ltk_reply),
    ESP_ELFSYM_EXPORT(__wtime_weekday),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_sniff_sco_wifi_conn),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_auth_decrypt),
    ESP_ELFSYM_EXPORT(lv_indev_get_key),
    ESP_ELFSYM_EXPORT(mac_tx_set_mplen),
    ESP_ELFSYM_EXPORT(ble_hs_mbuf_from_flat),
    ESP_ELFSYM_EXPORT(lv_obj_get_layer_type),
    ESP_ELFSYM_EXPORT(hal_mac_deinit_twt_tx),
    ESP_ELFSYM_EXPORT(ble_l2cap_tx),
    ESP_ELFSYM_EXPORT(__fpclassifyd),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_values_from_psa),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_periodic_txbuf_make),
    ESP_ELFSYM_EXPORT(ble_hs_flow_startup),
    ESP_ELFSYM_EXPORT(lv_text_get_width_with_flags),
    ESP_ELFSYM_EXPORT(__cxa_get_globals),
    ESP_ELFSYM_EXPORT(esp_coex_common_timer_setfn_wrapper),
    ESP_ELFSYM_EXPORT(wifi_set_channel_process),
    ESP_ELFSYM_EXPORT(i2c_master_cmd_begin),
    ESP_ELFSYM_EXPORT(power_supply_set_allowed_to_charge),
    ESP_ELFSYM_EXPORT(mbedtls_pk_verify_restartable),
    ESP_ELFSYM_EXPORT(esp_reset_reason_get_hint),
    ESP_ELFSYM_EXPORT(esp_crypto_ds_lock_acquire),
    ESP_ELFSYM_EXPORT(i2s_channel_init_pdm_rx_mode),
    ESP_ELFSYM_EXPORT(wDev_ftm_record_t2t3),
    ESP_ELFSYM_EXPORT(pm_allow_tx),
    ESP_ELFSYM_EXPORT(ic_set_keep_alive_time),
    ESP_ELFSYM_EXPORT(sdmmc_card_init),
    ESP_ELFSYM_EXPORT(dragonfly_sqrt),
    ESP_ELFSYM_EXPORT(s_cache_hal_init_ctx),
    ESP_ELFSYM_EXPORT(psa_hash_update),
    ESP_ELFSYM_EXPORT(_ZTVN10__cxxabiv117__class_type_infoE),
    ESP_ELFSYM_EXPORT(sdmmc_init_mmc_bus_width),
    ESP_ELFSYM_EXPORT(lv_cache_entry_set_invalid),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_set_burst_size),
    ESP_ELFSYM_EXPORT(nd6_cleanup_netif),
    ESP_ELFSYM_EXPORT(esp_intr_ptr_in_isr_region),
    ESP_ELFSYM_EXPORT(app_event_unsubscribe),
    ESP_ELFSYM_EXPORT(ble_sm_lgcy_random_rx),
    ESP_ELFSYM_EXPORT(advFilter_hci_setDuplicateParam),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_power_ctrl_param_init),
    ESP_ELFSYM_EXPORT(heap_caps_malloc),
    ESP_ELFSYM_EXPORT(nus_chars_with_handle),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_update_in_connect),
    ESP_ELFSYM_EXPORT(_cnx_start_connect_without_scan),
    ESP_ELFSYM_EXPORT(dhcps_new),
    ESP_ELFSYM_EXPORT(esp_wifi_get_mac),
    ESP_ELFSYM_EXPORT(pm_save_tbtt_info),
    ESP_ELFSYM_EXPORT(phy_txdc_cal_pwdet_init),
    ESP_ELFSYM_EXPORT(app_manager_install_path_add),
    ESP_ELFSYM_EXPORT(coex_wifi_release),
    ESP_ELFSYM_EXPORT(lv_style_set_bg_grad_color),
    ESP_ELFSYM_EXPORT(wifi_reset_mac),
    ESP_ELFSYM_EXPORT(ppAdd2AMPDUTail),
    ESP_ELFSYM_EXPORT(wDev_ftm_set_t1t4),
    ESP_ELFSYM_EXPORT(fflush),
    ESP_ELFSYM_EXPORT(esp_log_cache_clean),
    ESP_ELFSYM_EXPORT(pm_tbtt_adaptive_update_target),
    ESP_ELFSYM_EXPORT(ieee80211_add_vhtopr),
    ESP_ELFSYM_EXPORT(ble_gatts_bonding_established),
    ESP_ELFSYM_EXPORT(spi_flash_encryption_hal_done),
    ESP_ELFSYM_EXPORT(lv_obj_remove_event_cb_with_user_data),
    ESP_ELFSYM_EXPORT(test_rx_parse_mumimo_complete_sigb),
    ESP_ELFSYM_EXPORT(lv_anim_resolve_speed),
    ESP_ELFSYM_EXPORT(r_ble_phy_set_rx_pwr_compensation),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_ext_set_adv_data),
    ESP_ELFSYM_EXPORT(esp_sleep_sub_mode_config),
    ESP_ELFSYM_EXPORT(phy_rfcal_txcap),
    ESP_ELFSYM_EXPORT(esp_efuse_utility_process),
    ESP_ELFSYM_EXPORT(memp_UDP_PCB),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_peer_addr_rd),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand_start),
    ESP_ELFSYM_EXPORT(pm_tx_null_data_done_process),
    ESP_ELFSYM_EXPORT(supplicant_sta_disconn_handler),
    ESP_ELFSYM_EXPORT(_ZZNSt8__format14__write_paddedINS_10_Sink_iterIcEEcEET_S3_St17basic_string_viewIT0_St11char_traitsIS5_EENS_6_AlignEjDiENKUljRS2_E_clEjSA_),
    ESP_ELFSYM_EXPORT(modem_clock_hal_get_clock_domain_icg_bitmap),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_chk_resume_in_task),
    ESP_ELFSYM_EXPORT(scan_cancel),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_reset_link_state),
    ESP_ELFSYM_EXPORT(lv_area_intersect),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_list_empty),
    ESP_ELFSYM_EXPORT(r_ble_lll_recycle_sch_entry),
    ESP_ELFSYM_EXPORT(esp_netif_dhcps_start),
    ESP_ELFSYM_EXPORT(esp_io_expander_gpio_wrapper_set_pull_mode),
    ESP_ELFSYM_EXPORT(esp_mspi_register_isr),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_wr_auth_pyld_tmo),
    ESP_ELFSYM_EXPORT(gpio_descriptor_get_flags),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_send_adv_set_terminated),
    ESP_ELFSYM_EXPORT(_ZTVSt16__numpunct_cacheIcE),
    ESP_ELFSYM_EXPORT(app_paths_get_user_data_path),
    ESP_ELFSYM_EXPORT(phy_get_tone_sar_dout),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_write),
    ESP_ELFSYM_EXPORT(r_ble_lll_inform_task_to_recycle_sch),
    ESP_ELFSYM_EXPORT(__shiftright128),
    ESP_ELFSYM_EXPORT(wmemset),
    ESP_ELFSYM_EXPORT(esp_cmac_setup),
    ESP_ELFSYM_EXPORT(hal_he_set_htc),
    ESP_ELFSYM_EXPORT(iswpunct),
    ESP_ELFSYM_EXPORT(lv_event_get_target_obj),
    ESP_ELFSYM_EXPORT(psa_key_derivation_output_bytes),
    ESP_ELFSYM_EXPORT(esp_flash_chip_winbond),
    ESP_ELFSYM_EXPORT(pm_is_twt_active_cnt_zero),
    ESP_ELFSYM_EXPORT(multi_heap_free),
    ESP_ELFSYM_EXPORT(phy_get_power_atten),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_periodic_sched_priority_increase),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_set_adv_params),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_rx_buffer_hdr_chain),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_keep_alive),
    ESP_ELFSYM_EXPORT(wdev_mac_reg_store),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_rsaes_oaep_encrypt),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_ext_estimate_data_itvl),
    ESP_ELFSYM_EXPORT(phy_cfg_tx_magtrk),
    ESP_ELFSYM_EXPORT(mbedtls_pem_free),
    ESP_ELFSYM_EXPORT(forced_memzero),
    ESP_ELFSYM_EXPORT(hal_he_get_rx_trigger_cnt),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_mul_restartable),
    ESP_ELFSYM_EXPORT(_ZZNSt18__moneypunct_cacheIwLb1EE8_M_cacheERKSt6localeEN11_Scoped_strC1ERKSbIwSt11char_traitsIwESaIwEE),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_verify_result),
    ESP_ELFSYM_EXPORT(lv_event_get_rotary_diff),
    ESP_ELFSYM_EXPORT(hid_report_map_mouse_len),
    ESP_ELFSYM_EXPORT(ble_hs_atomic_conn_flags),
    ESP_ELFSYM_EXPORT(_system_r),
    ESP_ELFSYM_EXPORT(window_manager_start),
    ESP_ELFSYM_EXPORT(esp_psram_is_initialized),
    ESP_ELFSYM_EXPORT(pm_coex_go_to_sleep),
    ESP_ELFSYM_EXPORT(mbedtls_aes_xts_setkey_enc),
    ESP_ELFSYM_EXPORT(lv_obj_invalidate),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_phy_update_cancel),
    ESP_ELFSYM_EXPORT(gpio_descriptor_get_pin_number),
    ESP_ELFSYM_EXPORT(ble_sm_sc_random_rx),
    ESP_ELFSYM_EXPORT(ble_store_config_persist_csfcs),
    ESP_ELFSYM_EXPORT(lv_cache_entry_is_invalid),
    ESP_ELFSYM_EXPORT(usb_msc_device_get),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_release_user_block),
    ESP_ELFSYM_EXPORT(spicommon_bus_free),
    ESP_ELFSYM_EXPORT(g_mesh_init_ps_type),
    ESP_ELFSYM_EXPORT(floorf),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_find_type_value),
    ESP_ELFSYM_EXPORT(lv_draw_buf_adjust_stride),
    ESP_ELFSYM_EXPORT(xQueueGetMutexHolderFromISR),
    ESP_ELFSYM_EXPORT(g_phy_cap_rx_stbc),
    ESP_ELFSYM_EXPORT(ble_store_util_bonded_peers),
    ESP_ELFSYM_EXPORT(esp_aes_cipher_encrypt),
    ESP_ELFSYM_EXPORT(aes_decrypt_deinit),
    ESP_ELFSYM_EXPORT(wDev_AppendRxBlocks),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_init),
    ESP_ELFSYM_EXPORT(spi_hal_set_data_pin_idle_level),
    ESP_ELFSYM_EXPORT(vRingbufferDelete),
    ESP_ELFSYM_EXPORT(vTaskPriorityDisinheritAfterTimeout),
    ESP_ELFSYM_EXPORT(scan_flush_all_tx_buf),
    ESP_ELFSYM_EXPORT(pm_extend_tbtt_adaptive_instance),
    ESP_ELFSYM_EXPORT(phy_fe_txrx_reset),
    ESP_ELFSYM_EXPORT(ieee80211_encap_amsdu),
    ESP_ELFSYM_EXPORT(coex_schm_all_default),
    ESP_ELFSYM_EXPORT(xTaskPriorityInherit),
    ESP_ELFSYM_EXPORT(hal_coex_enable_default_pti),
    ESP_ELFSYM_EXPORT(ff_mutex_delete),
    ESP_ELFSYM_EXPORT(lv_point_to_precise),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_outline_opa),
    ESP_ELFSYM_EXPORT(preferences_opt_bool),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_enable_now),
    ESP_ELFSYM_EXPORT(esp_test_tx_addba_request),
    ESP_ELFSYM_EXPORT(EccPoint_compute_public_key),
    ESP_ELFSYM_EXPORT(ble_bb_cte_get_cte_samp_limit_samp),
    ESP_ELFSYM_EXPORT(i2c_reset_rx_fifo),
    ESP_ELFSYM_EXPORT(display_invert_color),
    ESP_ELFSYM_EXPORT(lv_canvas_create),
    ESP_ELFSYM_EXPORT(lv_obj_set_pos),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_free),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_rx_pkt_in),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_session_free),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_callout_env_deinit_for_sm),
    ESP_ELFSYM_EXPORT(chanSel_stack_enableSetCsaVsCmd),
    ESP_ELFSYM_EXPORT(rcSetTxAmpduLimit),
    ESP_ELFSYM_EXPORT(pm_extend_tbtt_adaptive_servo),
    ESP_ELFSYM_EXPORT(lv_font_get_bitmap_fmt_txt),
    ESP_ELFSYM_EXPORT(crypto_ecdh_set_peerkey),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_block_cnt_check),
    ESP_ELFSYM_EXPORT(get_sublen_offset),
    ESP_ELFSYM_EXPORT(r_ble_log_raw_export),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_coex_dpc_update),
    ESP_ELFSYM_EXPORT(psa_cipher_encrypt),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service11displayidle20StackChanScreensaverE),
    ESP_ELFSYM_EXPORT(ieee80211_rfid_locp_recv_reset),
    ESP_ELFSYM_EXPORT(_ZSt13__int_to_charIwmEiPT_T0_PKS0_St13_Ios_Fmtflagsb),
    ESP_ELFSYM_EXPORT(wpa_sm_rx_eapol),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_color_to_argb8888),
    ESP_ELFSYM_EXPORT(coex_schm_lock),
    ESP_ELFSYM_EXPORT(get_vfs_for_fd),
    ESP_ELFSYM_EXPORT(hal_he_set_uora_parameter),
    ESP_ELFSYM_EXPORT(register_fd),
    ESP_ELFSYM_EXPORT(zb_rx_auto_sel),
    ESP_ELFSYM_EXPORT(_ZStneIwSt11char_traitsIwEEbRKSt19istreambuf_iteratorIT_T0_ES7_),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_ll_evt_tx),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_rd_rem_ver_cmd),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_rollback_last_unmapped_chan),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_restart),
    ESP_ELFSYM_EXPORT(usb_device_controller_get),
    ESP_ELFSYM_EXPORT(pmu_hal_hp_set_modem_active_backup_disable),
    ESP_ELFSYM_EXPORT(_ZTSN9__gnu_cxx24__concurrence_lock_errorE),
    ESP_ELFSYM_EXPORT(get_total_scan_time),
    ESP_ELFSYM_EXPORT(hmac_hal_read_result_256),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_cmac_finish),
    ESP_ELFSYM_EXPORT(esp_log_write),
    ESP_ELFSYM_EXPORT(esp_test_tx_enab_statistics),
    ESP_ELFSYM_EXPORT(uxTimerGetTimerNumber),
    ESP_ELFSYM_EXPORT(lv_span_stack_deinit),
    ESP_ELFSYM_EXPORT(he_send_action_twt_teardown),
    ESP_ELFSYM_EXPORT(esp_elf_relocate),
    ESP_ELFSYM_EXPORT(nimble_port_get_dflt_eventq),
    ESP_ELFSYM_EXPORT(f_utime),
    ESP_ELFSYM_EXPORT(rtc_gpio_iomux_output),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_mirror),
    ESP_ELFSYM_EXPORT(nan_warmup_timeout_process),
    ESP_ELFSYM_EXPORT(modf),
    ESP_ELFSYM_EXPORT(cosf),
    ESP_ELFSYM_EXPORT(ieee80211_assoc_resp_construct),
    ESP_ELFSYM_EXPORT(_ZSteqIcSt11char_traitsIcESaIcEEbRKNSt7__cxx1112basic_stringIT_T0_T1_EESA_),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_npl_init),
    ESP_ELFSYM_EXPORT(phy_filter_dcap_set),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_mpi),
    ESP_ELFSYM_EXPORT(cnx_node_alloc),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_grid_cell_column_pos),
    ESP_ELFSYM_EXPORT(os_reltime_initialized),
    ESP_ELFSYM_EXPORT(lv_draw_sw_get_blend_handler),
    ESP_ELFSYM_EXPORT(ppDequeueRxq_Locked),
    ESP_ELFSYM_EXPORT(_ZGVNSt8messagesIcE2idE),
    ESP_ELFSYM_EXPORT(esp_elf_init),
    ESP_ELFSYM_EXPORT(esp_partition_mmap),
    ESP_ELFSYM_EXPORT(mspi_timing_enter_low_speed_early),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_opaque_signature_sign_hash),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_ensure_started),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_finish),
    ESP_ELFSYM_EXPORT(esp_int_wdt_init),
    ESP_ELFSYM_EXPORT(environ),
    ESP_ELFSYM_EXPORT(lv_timer_get_next),
    ESP_ELFSYM_EXPORT(sdspi_crc16),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_delay_tx_aod_2us),
    ESP_ELFSYM_EXPORT(phy_enable_agc),
    ESP_ELFSYM_EXPORT(lv_layer_init),
    ESP_ELFSYM_EXPORT(qsort),
    ESP_ELFSYM_EXPORT(hal_he_set_hw_qos_null_ra_to_trans),
    ESP_ELFSYM_EXPORT(lv_obj_get_y_aligned),
    ESP_ELFSYM_EXPORT(hal_cca_set_snd20_cfg),
    ESP_ELFSYM_EXPORT(lv_style_set_width),
    ESP_ELFSYM_EXPORT(app_get_install_path),
    ESP_ELFSYM_EXPORT(ble_freertos_ev_pool),
    ESP_ELFSYM_EXPORT(ff_wtoupper),
    ESP_ELFSYM_EXPORT(raw_remove),
    ESP_ELFSYM_EXPORT(gps_module),
    ESP_ELFSYM_EXPORT(task_event_group_wait),
    ESP_ELFSYM_EXPORT(tcp_free),
    ESP_ELFSYM_EXPORT(pthread_include_pthread_local_storage_impl),
    ESP_ELFSYM_EXPORT(_Z19ble_get_scan_activeP6Device),
    ESP_ELFSYM_EXPORT(phy_txcap_comp),
    ESP_ELFSYM_EXPORT(http_header_delete),
    ESP_ELFSYM_EXPORT(wep),
    ESP_ELFSYM_EXPORT(ble_hs_conn_timer),
    ESP_ELFSYM_EXPORT(wifi_nvs_cfg_item_init),
    ESP_ELFSYM_EXPORT(pbuf_copy),
    ESP_ELFSYM_EXPORT(mbedtls_pk_can_do),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_whether_reset_rpa_timer),
    ESP_ELFSYM_EXPORT(r_ble_phy_env_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_pri_chan_txbuf_alloc_and_make),
    ESP_ELFSYM_EXPORT(_ZSt6formatIJNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEES5_St19basic_format_stringIcJDpNSt13type_identityIT_E4typeEEEDpOS8_),
    ESP_ELFSYM_EXPORT(pm_mac_wakeup),
    ESP_ELFSYM_EXPORT(ppClearTxq),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_deduce_private_exponent),
    ESP_ELFSYM_EXPORT(_Z11ble_get_ctxP6Device),
    ESP_ELFSYM_EXPORT(device_put),
    ESP_ELFSYM_EXPORT(_ZTVN2tt10ScopedLockE),
    ESP_ELFSYM_EXPORT(pmu_sleep_enable_regdma_backup),
    ESP_ELFSYM_EXPORT(wifi_he_get_hetb_tid_bawin),
    ESP_ELFSYM_EXPORT(lv_style_set_text_line_space),
    ESP_ELFSYM_EXPORT(_ZTINSt8ios_base7failureB5cxx11E),
    ESP_ELFSYM_EXPORT(soc_memory_regions),
    ESP_ELFSYM_EXPORT(wDevCtrl),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_find_active_conn),
    ESP_ELFSYM_EXPORT(rtc_clk_xtal_freq_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_reject_ind),
    ESP_ELFSYM_EXPORT(rcP2P11GSchedTbl),
    ESP_ELFSYM_EXPORT(i2c_hal_master_handle_rx_event),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_send_adv_report),
    ESP_ELFSYM_EXPORT(sys_mutex_unlock),
    ESP_ELFSYM_EXPORT(ieee80211_ftm_detach),
    ESP_ELFSYM_EXPORT(ieee80211_timer_process),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_le_power_req_make),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_default_wifi_scan),
    ESP_ELFSYM_EXPORT(wpabuf_resize),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service11displayidle21MatrixRainScreensaverE),
    ESP_ELFSYM_EXPORT(lv_dropdown_get_option_count),
    ESP_ELFSYM_EXPORT(ieee80211_add_assoc_req_ies),
    ESP_ELFSYM_EXPORT(lv_tabview_get_content),
    ESP_ELFSYM_EXPORT(mbedtls_pk_get_psa_attributes),
    ESP_ELFSYM_EXPORT(lv_style_set_border_width),
    ESP_ELFSYM_EXPORT(hal_coex_pti_init),
    ESP_ELFSYM_EXPORT(r_ext_funcs_p),
    ESP_ELFSYM_EXPORT(g_cte_ant_switch_delay_2m),
    ESP_ELFSYM_EXPORT(esp_timer_impl_get_time),
    ESP_ELFSYM_EXPORT(hal_agreement_clr_extra_softap_rx_ba),
    ESP_ELFSYM_EXPORT(phy_pbus_workmode),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tsf_time_deviation),
    ESP_ELFSYM_EXPORT(aes_gmac),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_periodic_send_sync_ind),
    ESP_ELFSYM_EXPORT(pointer_read_data),
    ESP_ELFSYM_EXPORT(scan_enter_oper_channel_handler),
    ESP_ELFSYM_EXPORT(ff_mutex_take),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_set_rxbuf_cnt_in_need),
    ESP_ELFSYM_EXPORT(hci_stack_enable),
    ESP_ELFSYM_EXPORT(sta_is_wifi_ent_enabled),
    ESP_ELFSYM_EXPORT(ieee80211_merge_mbssid_profile),
    ESP_ELFSYM_EXPORT(pm_noise_check_enable),
    ESP_ELFSYM_EXPORT(tkip_decap),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_piscan_wifi_conn),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_update_data_if_needed),
    ESP_ELFSYM_EXPORT(lv_theme_get_font_large),
    ESP_ELFSYM_EXPORT(lv_display_flush_ready),
    ESP_ELFSYM_EXPORT(igmp_joingroup_netif),
    ESP_ELFSYM_EXPORT(lwip_netconn_is_err_msg),
    ESP_ELFSYM_EXPORT(esp_phy_modem_init),
    ESP_ELFSYM_EXPORT(httpd_sess_get_free),
    ESP_ELFSYM_EXPORT(_Z16gps_ledger_clearv),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_set_one_checked),
    ESP_ELFSYM_EXPORT(lvgl_arch_stop),
    ESP_ELFSYM_EXPORT(qrcode_initBytes),
    ESP_ELFSYM_EXPORT(ieee80211_is_bufferable_mmpdu),
    ESP_ELFSYM_EXPORT(nvs_get_i32),
    ESP_ELFSYM_EXPORT(raw_netif_ip_addr_changed),
    ESP_ELFSYM_EXPORT(etharp_add_static_entry),
    ESP_ELFSYM_EXPORT(lodepng_color_mode_init),
    ESP_ELFSYM_EXPORT(sdmmc_io_enable_hs_mode),
    ESP_ELFSYM_EXPORT(lv_image_decoder_get_info),
    ESP_ELFSYM_EXPORT(vPortExitCritical),
    ESP_ELFSYM_EXPORT(lv_color_format_get_bpp),
    ESP_ELFSYM_EXPORT(psa_aead_encrypt),
    ESP_ELFSYM_EXPORT(ble_att_init),
    ESP_ELFSYM_EXPORT(sae_derive_pwe_from_pt_ffc),
    ESP_ELFSYM_EXPORT(spi_flash_hal_supports_direct_read),
    ESP_ELFSYM_EXPORT(lv_label_get_letter_pos),
    ESP_ELFSYM_EXPORT(ll_task_h),
    ESP_ELFSYM_EXPORT(phy_fe_adc_on),
    ESP_ELFSYM_EXPORT(lv_style_set_text_align),
    ESP_ELFSYM_EXPORT(ecc_hal_set_mode),
    ESP_ELFSYM_EXPORT(esp_clk_utils_mspi_speed_mode_sync_before_cpu_freq_switching),
    ESP_ELFSYM_EXPORT(lv_ll_ins_tail),
    ESP_ELFSYM_EXPORT(g_phyFuns),
    ESP_ELFSYM_EXPORT(r_ble_lll_convert_timeline_num_to_conn_handle),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_set_path_loss_report_param),
    ESP_ELFSYM_EXPORT(mbedtls_md_starts),
    ESP_ELFSYM_EXPORT(owe_process_assoc_req),
    ESP_ELFSYM_EXPORT(start_cpu0),
    ESP_ELFSYM_EXPORT(ieee80211_add_htinfo),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_env_deinit),
    ESP_ELFSYM_EXPORT(esp_hmac_finish_transparent),
    ESP_ELFSYM_EXPORT(lv_anim_set_repeat_count),
    ESP_ELFSYM_EXPORT(wpa_write_rsnxe),
    ESP_ELFSYM_EXPORT(ble_hs_atomic_conn_delete),
    ESP_ELFSYM_EXPORT(wcrtomb),
    ESP_ELFSYM_EXPORT(ldexp),
    ESP_ELFSYM_EXPORT(lv_style_set_anim_duration),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx1110moneypunctIwLb0EE2idE),
    ESP_ELFSYM_EXPORT(crypto_ec_key_deinit),
    ESP_ELFSYM_EXPORT(sync_stack_enable),
    ESP_ELFSYM_EXPORT(crypto_ec_get_a),
    ESP_ELFSYM_EXPORT(lvgl_pointer_remove),
    ESP_ELFSYM_EXPORT(rtc_io_num_map),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt11__timepunctIwEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(__wrap___gxx_personality_v0),
    ESP_ELFSYM_EXPORT(preferences_has_int32),
    ESP_ELFSYM_EXPORT(r_get_max_skip),
    ESP_ELFSYM_EXPORT(netif_set_link_down),
    ESP_ELFSYM_EXPORT(test_parse_rx_mu_nonmimo),
    ESP_ELFSYM_EXPORT(esf_buf_setup),
    ESP_ELFSYM_EXPORT(npl_freertos_sem_init),
    ESP_ELFSYM_EXPORT(r_swap_in_place),
    ESP_ELFSYM_EXPORT(esp_wifi_set_sleep_min_active_time),
    ESP_ELFSYM_EXPORT(os_memblock_put),
    ESP_ELFSYM_EXPORT(lv_chart_add_series),
    ESP_ELFSYM_EXPORT(lv_layout_apply),
    ESP_ELFSYM_EXPORT(esp_ptr_executable),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_rxbuf_empty_check),
    ESP_ELFSYM_EXPORT(lv_memcpy),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_check_pubkey_soft),
    ESP_ELFSYM_EXPORT(bluetooth_find_first_ready_device),
    ESP_ELFSYM_EXPORT(wifi_get_station_state),
    ESP_ELFSYM_EXPORT(ccmp_decap),
    ESP_ELFSYM_EXPORT(wpa_write_rsn_ie),
    ESP_ELFSYM_EXPORT(g_wifi_nvs),
    ESP_ELFSYM_EXPORT(lv_map),
    ESP_ELFSYM_EXPORT(dhcps_pbuf_alloc),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_encrypt_and_tag),
    ESP_ELFSYM_EXPORT(haptic_select_library),
    ESP_ELFSYM_EXPORT(_ZSt11make_sharedIN2tt6PubSubINS0_7service5audio10AudioEventEEEJEESt10shared_ptrIT_EDpOT0_),
    ESP_ELFSYM_EXPORT(timer_is_running),
    ESP_ELFSYM_EXPORT(netconn_recv_tcp_pbuf_flags),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_timer_exp),
    ESP_ELFSYM_EXPORT(_ZSt4swapI6direntENSt9enable_ifIXsrSt6__and_IJSt6__not_ISt15__is_tuple_likeIT_EESt21is_move_constructibleIS5_ESt18is_move_assignableIS5_EEE5valueEvE4typeERS5_SF_),
    ESP_ELFSYM_EXPORT(esf_buf_alloc_default_handler),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_cth_flow_alloc_credit),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_piscan_wifi_conn),
    ESP_ELFSYM_EXPORT(usb_msc_device_is_connected),
    ESP_ELFSYM_EXPORT(esp_netif_sntp_init),
    ESP_ELFSYM_EXPORT(lv_theme_default_is_inited),
    ESP_ELFSYM_EXPORT(aes_decrypt_init),
    ESP_ELFSYM_EXPORT(lv_chart_set_axis_range),
    ESP_ELFSYM_EXPORT(sdmmc_check_host_function_ptr_integrity),
    ESP_ELFSYM_EXPORT(igmp_init),
    ESP_ELFSYM_EXPORT(esp_hmac_update_opaque),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_calc_dci),
    ESP_ELFSYM_EXPORT(window_manager_create),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_enabled),
    ESP_ELFSYM_EXPORT(ampdu_dispatch_as_many_as_possible),
    ESP_ELFSYM_EXPORT(if_ctrl),
    ESP_ELFSYM_EXPORT(ble_midi_ensure_keepalive),
    ESP_ELFSYM_EXPORT(BLUETOOTH_SERIAL_TYPE),
    ESP_ELFSYM_EXPORT(lv_textarea_get_one_line),
    ESP_ELFSYM_EXPORT(hostapd_get_psk),
    ESP_ELFSYM_EXPORT(usb_midi_device_start),
    ESP_ELFSYM_EXPORT(f_truncate),
    ESP_ELFSYM_EXPORT(ic_delete_wifi_task),
    ESP_ELFSYM_EXPORT(xQueueCreateMutexStatic),
    ESP_ELFSYM_EXPORT(mac_tx_set_pti),
    ESP_ELFSYM_EXPORT(r_hal_timer_deinit),
    ESP_ELFSYM_EXPORT(g_wdev_is_nan_pkt_in_valid_slot_cb),
    ESP_ELFSYM_EXPORT(__wrap_gpio_get_level),
    ESP_ELFSYM_EXPORT(charge_pump_init),
    ESP_ELFSYM_EXPORT(r_ble_log_reset),
    ESP_ELFSYM_EXPORT(g_ble_ll_ctrl_pkt_lengths_ro),
    ESP_ELFSYM_EXPORT(lv_style_set_border_color),
    ESP_ELFSYM_EXPORT(_ZSt24__throw_invalid_argumentPKc),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_first_pri_event),
    ESP_ELFSYM_EXPORT(pm_update_by_connectionless_status),
    ESP_ELFSYM_EXPORT(display_has_capability),
    ESP_ELFSYM_EXPORT(hal_set_rx_beacon_pti),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_sub_int),
    ESP_ELFSYM_EXPORT(lv_textarea_delete_char_forward),
    ESP_ELFSYM_EXPORT(ppClearAlldataTxq_ifx),
    ESP_ELFSYM_EXPORT(multi_heap_get_allocated_size),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_clear_all_wl_bit),
    ESP_ELFSYM_EXPORT(lv_obj_get_state),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_sm_stop),
    ESP_ELFSYM_EXPORT(nan_dw_end_process),
    ESP_ELFSYM_EXPORT(lv_display_get_horizontal_resolution),
    ESP_ELFSYM_EXPORT(chm_get_home_channel),
    ESP_ELFSYM_EXPORT(base_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(rcUpdateTxDoneAmpdu2),
    ESP_ELFSYM_EXPORT(ieee80211_set_appie),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_set_title),
    ESP_ELFSYM_EXPORT(lvgl_pointer_set_calibration),
    ESP_ELFSYM_EXPORT(mbedtls_pem_read_buffer),
    ESP_ELFSYM_EXPORT(phy_set_pbus_mem),
    ESP_ELFSYM_EXPORT(lv_draw_buf_dup_ex),
    ESP_ELFSYM_EXPORT(psa_import_key_into_slot),
    ESP_ELFSYM_EXPORT(crypto_ec_point_is_on_curve),
    ESP_ELFSYM_EXPORT(lv_timer_get_idle),
    ESP_ELFSYM_EXPORT(port_uxCoreStartupDone),
    ESP_ELFSYM_EXPORT(r_ble_ll_phy_to_phy_mode),
    ESP_ELFSYM_EXPORT(sta_bip_check),
    ESP_ELFSYM_EXPORT(r_ble_hw_periodiclist_clear),
    ESP_ELFSYM_EXPORT(iswdigit),
    ESP_ELFSYM_EXPORT(_ZStneIcSt11char_traitsIcEEbRKSt19istreambuf_iteratorIT_T0_ES7_),
    ESP_ELFSYM_EXPORT(bt_agc_recorrect_set),
    ESP_ELFSYM_EXPORT(phy_txbbgain_to_index),
    ESP_ELFSYM_EXPORT(os_mempool_init_internal),
    ESP_ELFSYM_EXPORT(rom_flash_chip_dummy_hpm),
    ESP_ELFSYM_EXPORT(spi_flash_chip_gd_detect_size),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_policy),
    ESP_ELFSYM_EXPORT(ieee80211_encap_keep_alive_data),
    ESP_ELFSYM_EXPORT(tc_cmac_init),
    ESP_ELFSYM_EXPORT(gpio_input_enable),
    ESP_ELFSYM_EXPORT(lv_text_encoded_letter_next_2),
    ESP_ELFSYM_EXPORT(_ZZNSt18__moneypunct_cacheIwLb0EE8_M_cacheERKSt6localeEN11_Scoped_strC1ERKSbIwSt11char_traitsIwESaIwEE),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_cth_flow_is_enabled),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_uint_le_mpi),
    ESP_ELFSYM_EXPORT(lv_chart_set_type),
    ESP_ELFSYM_EXPORT(esp_netif_next_unsafe),
    ESP_ELFSYM_EXPORT(r_ble_lll_df_ring_buffer_handler_done),
    ESP_ELFSYM_EXPORT(wpa_attach),
    ESP_ELFSYM_EXPORT(HAPTIC_TYPE),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_is_ap_notify_completed_rsne_internal),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_check_channel),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_phy_update_ind_make),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_get_mont_r2_unsafe),
    ESP_ELFSYM_EXPORT(tcp_timer_needed),
    ESP_ELFSYM_EXPORT(lv_image_decoder_deinit),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_active_chanset_clear),
    ESP_ELFSYM_EXPORT(wdev_process_mac_modem_beacon_miss),
    ESP_ELFSYM_EXPORT(lv_draw_arc_get_area),
    ESP_ELFSYM_EXPORT(app_paths_get_assets_directory),
    ESP_ELFSYM_EXPORT(lv_text_get_encoded_length),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY3),
    ESP_ELFSYM_EXPORT(wifi_log),
    ESP_ELFSYM_EXPORT(sdmmc_check_scr),
    ESP_ELFSYM_EXPORT(usb_hid_device_send_keyboard),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_aux),
    ESP_ELFSYM_EXPORT(r_esp_ble_msys_deinit),
    ESP_ELFSYM_EXPORT(regdma_find_prev_module_link_tail),
    ESP_ELFSYM_EXPORT(fmax),
    ESP_ELFSYM_EXPORT(ieee80211_ifattach),
    ESP_ELFSYM_EXPORT(tbtt_adaptive_data_alloc),
    ESP_ELFSYM_EXPORT(esp_gpio_revoke),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_classify_filter_init),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_prof_is_rsn_internal),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_check_timer),
    ESP_ELFSYM_EXPORT(_gettimeofday_r),
    ESP_ELFSYM_EXPORT(npl_freertos_set_controller_npl_info),
    ESP_ELFSYM_EXPORT(sdspi_host_remove_device),
    ESP_ELFSYM_EXPORT(nan_dp_get_own_ndi),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_bottom),
    ESP_ELFSYM_EXPORT(tcp_backlog_delayed),
    ESP_ELFSYM_EXPORT(spi_flash_mmap),
    ESP_ELFSYM_EXPORT(lv_rb_destroy),
    ESP_ELFSYM_EXPORT(http_utils_assign_string),
    ESP_ELFSYM_EXPORT(coex_hw_timer_freq_set),
    ESP_ELFSYM_EXPORT(extAdv_stack_initEnv),
    ESP_ELFSYM_EXPORT(bt_bb_rx_filter_sel),
    ESP_ELFSYM_EXPORT(vApplicationStackOverflowHook),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_env_deinit),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handle_message_type),
    ESP_ELFSYM_EXPORT(lvgl_lock),
    ESP_ELFSYM_EXPORT(f_read),
    ESP_ELFSYM_EXPORT(mbedtls_pk_import_into_psa),
    ESP_ELFSYM_EXPORT(phy_noise_floor_auto_set),
    ESP_ELFSYM_EXPORT(phy_bt_track_tx_power_new),
    ESP_ELFSYM_EXPORT(send_wake_null_timeout),
    ESP_ELFSYM_EXPORT(ieee80211_vht_deattach),
    ESP_ELFSYM_EXPORT(heap_caps_aligned_alloc_base),
    ESP_ELFSYM_EXPORT(lora_remove_state_callback),
    ESP_ELFSYM_EXPORT(r_ble_hw_resolv_list_set),
    ESP_ELFSYM_EXPORT(etharp_tmr),
    ESP_ELFSYM_EXPORT(scan_set_current_scan_times),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_piscan_wifi_scan),
    ESP_ELFSYM_EXPORT(ieee80211_update_phy_country),
    ESP_ELFSYM_EXPORT(pp_sig_cnt),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_start),
    ESP_ELFSYM_EXPORT(bzero),
    ESP_ELFSYM_EXPORT(wDev_SetCurChannel),
    ESP_ELFSYM_EXPORT(xRingbufferSendFromISR),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_a2dp_paused_wifi_connecting),
    ESP_ELFSYM_EXPORT(igmp_stop),
    ESP_ELFSYM_EXPORT(g_ble_sca_ppm_tbl_ro),
    ESP_ELFSYM_EXPORT(r_ble_lll_aux_scan_drop),
    ESP_ELFSYM_EXPORT(io_mux_force_disable_lp_io_clock),
    ESP_ELFSYM_EXPORT(r_ble_ll_callout_env_deinit),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_border_post),
    ESP_ELFSYM_EXPORT(phy_dac_scale_set),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_get_eof_desc_addr),
    ESP_ELFSYM_EXPORT(wdev_bank_store),
    ESP_ELFSYM_EXPORT(hal_set_rx_beacon_time),
    ESP_ELFSYM_EXPORT(lv_style_remove_prop),
    ESP_ELFSYM_EXPORT(r_ble_ll_support_features_init),
    ESP_ELFSYM_EXPORT(lvgl_software_keyboard_activate),
    ESP_ELFSYM_EXPORT(r_ble_ll_hw_err_timer_cb),
    ESP_ELFSYM_EXPORT(translate_path),
    ESP_ELFSYM_EXPORT(_ZTVSt8bad_cast),
    ESP_ELFSYM_EXPORT(lv_obj_set_scroll_snap_x),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_copy),
    ESP_ELFSYM_EXPORT(_ZTVSi),
    ESP_ELFSYM_EXPORT(lv_anim_speed_clamped),
    ESP_ELFSYM_EXPORT(crypto_bignum_to_bin),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_piscan_wifi_scan),
    ESP_ELFSYM_EXPORT(sys_mutex_new),
    ESP_ELFSYM_EXPORT(crypto_ec_key_group),
    ESP_ELFSYM_EXPORT(sdmmc_mmc_send_ext_csd_data),
    ESP_ELFSYM_EXPORT(hid_mouse_input_handle),
    ESP_ELFSYM_EXPORT(hal_vht_sta_set_aid),
    ESP_ELFSYM_EXPORT(esp_intr_disable_source),
    ESP_ELFSYM_EXPORT(esp_transport_close),
    ESP_ELFSYM_EXPORT(ieee80211_send_action_register),
    ESP_ELFSYM_EXPORT(spicommon_dma_rx_mb),
    ESP_ELFSYM_EXPORT(modem_clock_hal_deselect_all_wifi_lpclk_source),
    ESP_ELFSYM_EXPORT(audio_stream_get_volume),
    ESP_ELFSYM_EXPORT(esp_log_is_tag_loggable),
    ESP_ELFSYM_EXPORT(lvgl_software_keyboard_show),
    ESP_ELFSYM_EXPORT(gps_get_model_name),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_find_info),
    ESP_ELFSYM_EXPORT(lv_ll_get_next),
    ESP_ELFSYM_EXPORT(esp_wifi_deinit),
    ESP_ELFSYM_EXPORT(phy_pbus_rx_dco_cal),
    ESP_ELFSYM_EXPORT(ppCheckTxQIdle),
    ESP_ELFSYM_EXPORT(he_preamble_ersu),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_set_peer_rpa),
    ESP_ELFSYM_EXPORT(wDev_ProcessRxSucData),
    ESP_ELFSYM_EXPORT(dhcp_cleanup),
    ESP_ELFSYM_EXPORT(ble_gap_adv_set_fields),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_free),
    ESP_ELFSYM_EXPORT(r_ble_lll_df_get_max_cte_sample_times_num),
    ESP_ELFSYM_EXPORT(ieee80211_recv_action_vendor_esp_now),
    ESP_ELFSYM_EXPORT(esp_ds_start_sign),
    ESP_ELFSYM_EXPORT(fmod),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_classify_filter_aux_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_update_interval_tick),
    ESP_ELFSYM_EXPORT(_g_esp_netif_inherent_sta_config),
    ESP_ELFSYM_EXPORT(pm_enable_sleep_delay_timer),
    ESP_ELFSYM_EXPORT(periph_rcc_enter),
    ESP_ELFSYM_EXPORT(GetAccess),
    ESP_ELFSYM_EXPORT(wifi_destroy_softap),
    ESP_ELFSYM_EXPORT(mbedtls_eckeydh_info),
    ESP_ELFSYM_EXPORT(ble_att_clt_tx_write_req),
    ESP_ELFSYM_EXPORT(esp_log_set_default_level),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_image_to_rgb565),
    ESP_ELFSYM_EXPORT(itwt_probe_timer),
    ESP_ELFSYM_EXPORT(lv_timer_core_init),
    ESP_ELFSYM_EXPORT(wDev_Crypto_Disable),
    ESP_ELFSYM_EXPORT(pm_register_pm_tx_null_cb),
    ESP_ELFSYM_EXPORT(system_event_callback_remove),
    ESP_ELFSYM_EXPORT(r_os_msys_get),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt10moneypunctIcLb1EEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(iswalnum),
    ESP_ELFSYM_EXPORT(lv_arc_get_value),
    ESP_ELFSYM_EXPORT(lv_textarea_set_text),
    ESP_ELFSYM_EXPORT(hal_he_set_packet_padding_us),
    ESP_ELFSYM_EXPORT(lv_area_is_out),
    ESP_ELFSYM_EXPORT(mbedtls_pkcs5_pbkdf2_hmac_ext),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_CH3_ATTEN0_INITCODE_DIFF),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_default_wifi_scan),
    ESP_ELFSYM_EXPORT(hmac_hal_write_one_block_512),
    ESP_ELFSYM_EXPORT(BLUETOOTH_HID_DEVICE_TYPE),
    ESP_ELFSYM_EXPORT(r_ble_phy_mode_to_rate),
    ESP_ELFSYM_EXPORT(camera_get_frame),
    ESP_ELFSYM_EXPORT(lvgl_get_launcher_icon_font_height),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_duration_timer_cb),
    ESP_ELFSYM_EXPORT(adc_lock_try_acquire),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_get_data_len),
    ESP_ELFSYM_EXPORT(ieee80211_process_btwt_setup_action),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceIN2tt3app13fileselection5StateESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(ds_hal_write_message),
    ESP_ELFSYM_EXPORT(hal_mac_set_rxbuf_reload_use_hw_beacon_enable),
    ESP_ELFSYM_EXPORT(pointer_enter_sleep),
    ESP_ELFSYM_EXPORT(_Z34app_metadata_is_valid_version_nameRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(esp_transport_init),
    ESP_ELFSYM_EXPORT(cplusplus_symbols),
    ESP_ELFSYM_EXPORT(pm_beacon_monitor_tbtt_start),
    ESP_ELFSYM_EXPORT(raw_bind),
    ESP_ELFSYM_EXPORT(_ZTSSt17bad_function_call),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_join_leave_group),
    ESP_ELFSYM_EXPORT(httpd_sess_trigger_close_),
    ESP_ELFSYM_EXPORT(i2s_hal_std_set_tx_slot),
    ESP_ELFSYM_EXPORT(ic_stop_sw_txq),
    ESP_ELFSYM_EXPORT(esp_nimble_disable),
    ESP_ELFSYM_EXPORT(netif_add_ext_callback),
    ESP_ELFSYM_EXPORT(thread_set_name),
    ESP_ELFSYM_EXPORT(wifi_sta_reg_eapol_txdone_cb),
    ESP_ELFSYM_EXPORT(vli_mmod_fast_secp256r1),
    ESP_ELFSYM_EXPORT(gpio_sleep_sel_dis),
    ESP_ELFSYM_EXPORT(esp_sha_acquire_hardware),
    ESP_ELFSYM_EXPORT(esp_coex_init_include_func),
    ESP_ELFSYM_EXPORT(ble_sm_ltk_req_rx),
    ESP_ELFSYM_EXPORT(__lock___sinit_recursive_mutex),
    ESP_ELFSYM_EXPORT(phy_bbpll_cal),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ffdh_export_public_key),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_info),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSdD1Ev),
    ESP_ELFSYM_EXPORT(ble_hs_conn_exists),
    ESP_ELFSYM_EXPORT(g_sync_stack_envP),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_scan_rsp_legacy_pri_chan_pdu_make),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_setup),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_reset_sm),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_KEY_PURPOSE_3),
    ESP_ELFSYM_EXPORT(esp_nimble_init),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_setkey),
    ESP_ELFSYM_EXPORT(s_wifi_api_lock),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handshake_client_step),
    ESP_ELFSYM_EXPORT(gpio_isr_register),
    ESP_ELFSYM_EXPORT(ledc_set_freq),
    ESP_ELFSYM_EXPORT(wpabuf_alloc),
    ESP_ELFSYM_EXPORT(lv_obj_set_state),
    ESP_ELFSYM_EXPORT(ieee80211_espnow_get_init_flag),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_error),
    ESP_ELFSYM_EXPORT(xTaskGenericNotifyFromISR),
    ESP_ELFSYM_EXPORT(device_is_added),
    ESP_ELFSYM_EXPORT(mbedtls_pk_parse_subpubkey),
    ESP_ELFSYM_EXPORT(ieee80211_set_shortslottime),
    ESP_ELFSYM_EXPORT(rcClearCurAMPDUSched),
    ESP_ELFSYM_EXPORT(ieee80211_timer_do_process),
    ESP_ELFSYM_EXPORT(sdmmc_do_timing_tuning),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_le_cmd_proc),
    ESP_ELFSYM_EXPORT(ht_set_snd_ch_cfg),
    ESP_ELFSYM_EXPORT(r_ext_bb_funcs_p),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_param_nrr),
    ESP_ELFSYM_EXPORT(xQueueCreateSet),
    ESP_ELFSYM_EXPORT(esp_flash_default_chip),
    ESP_ELFSYM_EXPORT(ampduTidState2str),
    ESP_ELFSYM_EXPORT(adc_oneshot_config_channel),
    ESP_ELFSYM_EXPORT(wifi_remove_event_callback),
    ESP_ELFSYM_EXPORT(log_stack_enableLogsRelatedVsCmd),
    ESP_ELFSYM_EXPORT(pp_default_event_handler),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_idle_wifi_conn),
    ESP_ELFSYM_EXPORT(spi_bus_free),
    ESP_ELFSYM_EXPORT(r_ble_controller_disable),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_phy_from_phy_mask),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_alloc_tx_buffer),
    ESP_ELFSYM_EXPORT(pm_disable_sleep_delay_timer),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_list_clear),
    ESP_ELFSYM_EXPORT(esp_netif_destroy),
    ESP_ELFSYM_EXPORT(__time_am_pm),
    ESP_ELFSYM_EXPORT(wDev_SnifferRxData),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_periodic_sync_ind),
    ESP_ELFSYM_EXPORT(ble_hid_init_gatt),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_callout_env_init),
    ESP_ELFSYM_EXPORT(audio_stream_get_mute),
    ESP_ELFSYM_EXPORT(npl_freertos_hw_is_in_critical),
    ESP_ELFSYM_EXPORT(key_mgr_hal_is_key_deployment_valid),
    ESP_ELFSYM_EXPORT(r_DB_data_list_add),
    ESP_ELFSYM_EXPORT(ftm_initiator_cleanup),
    ESP_ELFSYM_EXPORT(crypto_ec_key_verify_signature),
    ESP_ELFSYM_EXPORT(psa_mac_verify_setup),
    ESP_ELFSYM_EXPORT(regi2c_ctrl_read_reg_mask),
    ESP_ELFSYM_EXPORT(esp_wifi_unregister_wpa_cb_internal),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_read_be),
    ESP_ELFSYM_EXPORT(_global_interrupt_handler),
    ESP_ELFSYM_EXPORT(aes_hal_transform_dma_finish),
    ESP_ELFSYM_EXPORT(sae_pk_valid_password),
    ESP_ELFSYM_EXPORT(esp_timer_impl_get_min_period_us),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_check_curve_tls_id),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_set_sta_ip),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_ext_set_param),
    ESP_ELFSYM_EXPORT(g_mmgmt_mem_cnt_in_need),
    ESP_ELFSYM_EXPORT(fputwc),
    ESP_ELFSYM_EXPORT(__floatdisf),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_pkcs1_sign),
    ESP_ELFSYM_EXPORT(cnx_validate_owe_bss),
    ESP_ELFSYM_EXPORT(ble_mqueue_init),
    ESP_ELFSYM_EXPORT(wdt_hal_deinit),
    ESP_ELFSYM_EXPORT(os_memblock_get),
    ESP_ELFSYM_EXPORT(uart_controller_write_byte),
    ESP_ELFSYM_EXPORT(phy_printf),
    ESP_ELFSYM_EXPORT(i2c_new_master_bus),
    ESP_ELFSYM_EXPORT(wdev_mac_reg_load),
    ESP_ELFSYM_EXPORT(ble_midi_get_use_indicate),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_timeout),
    ESP_ELFSYM_EXPORT(crypto_ec_point_invert),
    ESP_ELFSYM_EXPORT(pm_mac_modem_params_rt_update),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_update),
    ESP_ELFSYM_EXPORT(sdmmc_can_discard),
    ESP_ELFSYM_EXPORT(heap_caps_aligned_alloc),
    ESP_ELFSYM_EXPORT(lv_display_get_event_count),
    ESP_ELFSYM_EXPORT(lv_dropdown_get_selected_highlight),
    ESP_ELFSYM_EXPORT(dbg_hal_check_clr_mplen_bitmap),
    ESP_ELFSYM_EXPORT(driver_remove_destruct),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_copy),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_get_alg),
    ESP_ELFSYM_EXPORT(r_ble_lll_ext_scan_coex_dpc_update),
    ESP_ELFSYM_EXPORT(npl_freertos_eventq_is_empty),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_mod_int),
    ESP_ELFSYM_EXPORT(mbedtls_oid_get_ec_grp),
    ESP_ELFSYM_EXPORT(r_ble_phy_isr),
    ESP_ELFSYM_EXPORT(xTaskGetCurrentTaskHandleForCore),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_get),
    ESP_ELFSYM_EXPORT(lv_indev_scroll_handler),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_sae_identifier_internal),
    ESP_ELFSYM_EXPORT(hw_critical_state_status),
    ESP_ELFSYM_EXPORT(strtof),
    ESP_ELFSYM_EXPORT(ble_gattc_write),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_beacon_miss_limit_exceeded_wakeup_enable),
    ESP_ELFSYM_EXPORT(pm_twt_set_target_wdev_time),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_env_deinit),
    ESP_ELFSYM_EXPORT(phy_i2c_txrate_init_new),
    ESP_ELFSYM_EXPORT(ecdsa_hal_verify_signature),
    ESP_ELFSYM_EXPORT(wifi_find_first_registered_device),
    ESP_ELFSYM_EXPORT(g_wdev_set_t1t4_cb),
    ESP_ELFSYM_EXPORT(lv_obj_get_child_by_type),
    ESP_ELFSYM_EXPORT(esp_mac_addr_len_get),
    ESP_ELFSYM_EXPORT(cJSON_GetObjectItemCaseSensitive),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_conn_wifi_conn),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_change_cipher_spec),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_data_ring_list_init),
    ESP_ELFSYM_EXPORT(lwip_getsockopt_impl_ext),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_gcd),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_msys_init),
    ESP_ELFSYM_EXPORT(crypto_bignum_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_init),
    ESP_ELFSYM_EXPORT(ppCheckTxConnTrafficIdle),
    ESP_ELFSYM_EXPORT(ble_transport_alloc_cmd),
    ESP_ELFSYM_EXPORT(psa_sign_hash_builtin),
    ESP_ELFSYM_EXPORT(mspi_timing_config_set_flash_clock),
    ESP_ELFSYM_EXPORT(ble_sm_proc_find),
    ESP_ELFSYM_EXPORT(esp_libc_time_init),
    ESP_ELFSYM_EXPORT(ble_hs_flow_connection_broken),
    ESP_ELFSYM_EXPORT(gdma_hal_connect_mem),
    ESP_ELFSYM_EXPORT(RC_GetBlockAckTime),
    ESP_ELFSYM_EXPORT(chm_release_lock),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_set_bus_width),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_join_leave_group_netif),
    ESP_ELFSYM_EXPORT(ieee80211_ampdu_reorder),
    ESP_ELFSYM_EXPORT(r_ble_log_set_buf_index_flag),
    ESP_ELFSYM_EXPORT(lv_anim_core_init),
    ESP_ELFSYM_EXPORT(ic_get_next_bcnoffset),
    ESP_ELFSYM_EXPORT(_ZTISt13runtime_error),
    ESP_ELFSYM_EXPORT(npl_freertos_event_set_arg),
    ESP_ELFSYM_EXPORT(phy_set_rf_freq_offset),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_rxbuffer_alloc_precheck),
    ESP_ELFSYM_EXPORT(ble_gap_rx_conn_complete),
    ESP_ELFSYM_EXPORT(lv_global),
    ESP_ELFSYM_EXPORT(ledc_update_duty),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_deinit),
    ESP_ELFSYM_EXPORT(node_remove_from_list),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_ADC1_AVE_INITCODE_ATTEN3),
    ESP_ELFSYM_EXPORT(esp_efuse_read_field_blob),
    ESP_ELFSYM_EXPORT(__kernel_rem_pio2f),
    ESP_ELFSYM_EXPORT(esp_ptr_byte_accessible),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_pkcs1_encrypt),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_reset_out_pointers),
    ESP_ELFSYM_EXPORT(multi_heap_free_impl),
    ESP_ELFSYM_EXPORT(ble_gap_enc_event),
    ESP_ELFSYM_EXPORT(_ZTVSt19_Sp_counted_deleterIPN2tt7service12ServicePathsESt14default_deleteIS2_ESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(abort),
    ESP_ELFSYM_EXPORT(cipher_type_map_public_to_internal),
    ESP_ELFSYM_EXPORT(__fixunssfdi),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_arr_pool_init),
    ESP_ELFSYM_EXPORT(lv_obj_remove_style),
    ESP_ELFSYM_EXPORT(regdma_link_new_branch_addr_map_default),
    ESP_ELFSYM_EXPORT(g_mt),
    ESP_ELFSYM_EXPORT(esp32_sdspi_fs_api),
    ESP_ELFSYM_EXPORT(ieee80211w_get_active_igtk_key_id),
    ESP_ELFSYM_EXPORT(phy_set_freq_i2c_new),
    ESP_ELFSYM_EXPORT(ppCalTkipMic),
    ESP_ELFSYM_EXPORT(esp_mmu_map_init),
    ESP_ELFSYM_EXPORT(ble_store_write_cccd),
    ESP_ELFSYM_EXPORT(r_ble_lll_convert_timeline_num_to_dtm_handle),
    ESP_ELFSYM_EXPORT(rc_get_trc_by_index),
    ESP_ELFSYM_EXPORT(gpio_descriptor_set_level),
    ESP_ELFSYM_EXPORT(wpa_ap_rx_eapol),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt8numpunctIwEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(esp_crypto_hmac_enable_periph_clk),
    ESP_ELFSYM_EXPORT(s_pm_beacon_offset_config),
    ESP_ELFSYM_EXPORT(root_module),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_check_ap_channel),
    ESP_ELFSYM_EXPORT(clear_bss_queue),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_ptr_check_set),
    ESP_ELFSYM_EXPORT(memcmp),
    ESP_ELFSYM_EXPORT(vsnprintf),
    ESP_ELFSYM_EXPORT(r_os_mbuf_adj),
    ESP_ELFSYM_EXPORT(audio_codec_read),
    ESP_ELFSYM_EXPORT(pxCurrentTCBs),
    ESP_ELFSYM_EXPORT(lv_os_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_num_comp_pkts_event_send),
    ESP_ELFSYM_EXPORT(esp_tls_init),
    ESP_ELFSYM_EXPORT(esp_tls_get_error_handle),
    ESP_ELFSYM_EXPORT(_ZSt13__adjust_heapIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEEiS2_NS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SK_SK_T1_T2_),
    ESP_ELFSYM_EXPORT(esp_system_abort),
    ESP_ELFSYM_EXPORT(_ZTSSt12length_error),
    ESP_ELFSYM_EXPORT(ds_hal_read_result),
    ESP_ELFSYM_EXPORT(i2c_controller_register8_reset_bits),
    ESP_ELFSYM_EXPORT(wifi_init_key),
    ESP_ELFSYM_EXPORT(phy_rc_cal),
    ESP_ELFSYM_EXPORT(spi_flash_common_read_status_16b_rdsr_rdsr2),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_reset),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt10moneypunctIwLb1EEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(lv_display_delete),
    ESP_ELFSYM_EXPORT(_ZSt14__convert_to_vIdEvPKcRT_RSt12_Ios_IostateRKPi),
    ESP_ELFSYM_EXPORT(pm_coex_tbtt_process),
    ESP_ELFSYM_EXPORT(ieee80211_vht_attach),
    ESP_ELFSYM_EXPORT(adc_lock_release),
    ESP_ELFSYM_EXPORT(ieee80211_pwrsave_node_cleanup),
    ESP_ELFSYM_EXPORT(s_eapol_txdone_cb),
    ESP_ELFSYM_EXPORT(lv_event_stop_bubbling),
    ESP_ELFSYM_EXPORT(ieee80211_encap_null_data),
    ESP_ELFSYM_EXPORT(ecc_hal_is_calc_finished),
    ESP_ELFSYM_EXPORT(lv_draw_buf_destroy),
    ESP_ELFSYM_EXPORT(r_ble_lll_priority_value_get),
    ESP_ELFSYM_EXPORT(scan_stack_enableAdvFlowCtrlVsCmd),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_le_cmd_send_cmd_status),
    ESP_ELFSYM_EXPORT(ieee80211_parse_obss_scan_param),
    ESP_ELFSYM_EXPORT(__fixsfsi),
    ESP_ELFSYM_EXPORT(r_ble_log_internal_x3),
    ESP_ELFSYM_EXPORT(_ZTVNSt8__format19_Formatting_scannerINS_10_Sink_iterIcEEcEE),
    ESP_ELFSYM_EXPORT(esp_clk_apb_freq),
    ESP_ELFSYM_EXPORT(phy_set_rfpll_freq),
    ESP_ELFSYM_EXPORT(base_stack_initEnv),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_list_add),
    ESP_ELFSYM_EXPORT(r_ble_hw_list_details_dump),
    ESP_ELFSYM_EXPORT(lv_dropdown_get_options),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_reset),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_init),
    ESP_ELFSYM_EXPORT(app_event_emit_close),
    ESP_ELFSYM_EXPORT(lvgl_pointer_get_default),
    ESP_ELFSYM_EXPORT(_ZSt19__throw_logic_errorPKc),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_cb_customize_peer_sca),
    ESP_ELFSYM_EXPORT(udp_send),
    ESP_ELFSYM_EXPORT(r_hal_rtc_intr_init),
    ESP_ELFSYM_EXPORT(ppResumeTxAMPDU),
    ESP_ELFSYM_EXPORT(ap_rx_cb),
    ESP_ELFSYM_EXPORT(rc_get_default_sched),
    ESP_ELFSYM_EXPORT(isprint),
    ESP_ELFSYM_EXPORT(mbedtls_pk_get_key_type),
    ESP_ELFSYM_EXPORT(esp_netif_action_got_ip),
    ESP_ELFSYM_EXPORT(lv_cache_is_enabled),
    ESP_ELFSYM_EXPORT(lv_menu_class),
    ESP_ELFSYM_EXPORT(lvgl_sliderbox_add_value_changed_cb),
    ESP_ELFSYM_EXPORT(phy_get_freq_mem_param),
    ESP_ELFSYM_EXPORT(udp_bind_netif),
    ESP_ELFSYM_EXPORT(lv_display_get_physical_vertical_resolution),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_point_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_set_data_len),
    ESP_ELFSYM_EXPORT(lv_spangroup_create),
    ESP_ELFSYM_EXPORT(ppFillAMPDUBar),
    ESP_ELFSYM_EXPORT(lv_ll_get_head),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_init_phy),
    ESP_ELFSYM_EXPORT(mpi_hal_write_at_offset),
    ESP_ELFSYM_EXPORT(crypto_ec_get_mbedtls_to_nist_group_id),
    ESP_ELFSYM_EXPORT(tlsf_realloc),
    ESP_ELFSYM_EXPORT(tt_init_tactility_c),
    ESP_ELFSYM_EXPORT(g_mmgmt_mempool_size),
    ESP_ELFSYM_EXPORT(sta_rx_cb),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_set_padding_mode),
    ESP_ELFSYM_EXPORT(esp_flash_is_quad_mode),
    ESP_ELFSYM_EXPORT(lv_color16_luminance),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_create_cancel),
    ESP_ELFSYM_EXPORT(clk_hal_soc_root_get_freq_mhz),
    ESP_ELFSYM_EXPORT(sntp_setservername),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_check_mesh_special_in_table),
    ESP_ELFSYM_EXPORT(aes_ccm_ad),
    ESP_ELFSYM_EXPORT(lmacProcessTxSuccess),
    ESP_ELFSYM_EXPORT(r_ble_ll_auth_pyld_tmo_event_send),
    ESP_ELFSYM_EXPORT(tsf_hal_set_rx_beacon_abort_tsf_time_deviation_sync_enable),
    ESP_ELFSYM_EXPORT(phy_freq_chan_en_sw),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_width),
    ESP_ELFSYM_EXPORT(ppCalTxHEAMPDULength),
    ESP_ELFSYM_EXPORT(r_ble_ll_set_host_feat),
    ESP_ELFSYM_EXPORT(wpa_auth_uses_sae),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_erase_block),
    ESP_ELFSYM_EXPORT(ic_register_config_cb),
    ESP_ELFSYM_EXPORT(g_mesh_is_root),
    ESP_ELFSYM_EXPORT(hostap_eapol_resend_process),
    ESP_ELFSYM_EXPORT(sprintf),
    ESP_ELFSYM_EXPORT(sta_auth_ft),
    ESP_ELFSYM_EXPORT(ble_sm_lgcy_confirm_exec),
    ESP_ELFSYM_EXPORT(itwt_probe_timeout_fn_process),
    ESP_ELFSYM_EXPORT(ble_store_write_csfc),
    ESP_ELFSYM_EXPORT(phy_set_tsens_pwr),
    ESP_ELFSYM_EXPORT(lmacInitAc),
    ESP_ELFSYM_EXPORT(lv_event_get_current_target_obj),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand_sample),
    ESP_ELFSYM_EXPORT(ble_lll_sched_env_p),
    ESP_ELFSYM_EXPORT(phy_rx_loop_cap_set),
    ESP_ELFSYM_EXPORT(lv_tabview_add_tab),
    ESP_ELFSYM_EXPORT(ieee80211_set_hmac_stop),
    ESP_ELFSYM_EXPORT(ieee80211_mgmt_output),
    ESP_ELFSYM_EXPORT(WIFI_EVENT),
    ESP_ELFSYM_EXPORT(lv_arc_get_mode),
    ESP_ELFSYM_EXPORT(_scan_build_chan_list),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_ap_num),
    ESP_ELFSYM_EXPORT(mpi_hal_interrupt_enable),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_register),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_tx_buffer_hdr),
    ESP_ELFSYM_EXPORT(lv_textarea_cursor_right),
    ESP_ELFSYM_EXPORT(ble_hs_stop_deinit),
    ESP_ELFSYM_EXPORT(pm_set_keep_alive_time),
    ESP_ELFSYM_EXPORT(ble_gap_reattempt_count),
    ESP_ELFSYM_EXPORT(acos),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_rsaes_pkcs1_v15_encrypt),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_pad_row),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_get_country_ie),
    ESP_ELFSYM_EXPORT(_Z8gps_initP6Device8GpsModel),
    ESP_ELFSYM_EXPORT(scan_set_act_duration),
    ESP_ELFSYM_EXPORT(cs_free_ctrl_sock),
    ESP_ELFSYM_EXPORT(esp_wifi_register_owe_cb),
    ESP_ELFSYM_EXPORT(lwip_getaddrinfo),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_module_deinit),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_tx_pwr_report),
    ESP_ELFSYM_EXPORT(wifi_sta_connect_internal_process),
    ESP_ELFSYM_EXPORT(ppSetInterface),
    ESP_ELFSYM_EXPORT(_ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base),
    ESP_ELFSYM_EXPORT(adc_oneshot_read),
    ESP_ELFSYM_EXPORT(ble_gatts_start),
    ESP_ELFSYM_EXPORT(r_DB_hash_list_remove),
    ESP_ELFSYM_EXPORT(esp_vfs_close),
    ESP_ELFSYM_EXPORT(_ZSt25__throw_bad_function_callv),
    ESP_ELFSYM_EXPORT(ble_att_mtu_by_cid),
    ESP_ELFSYM_EXPORT(rtos_int_exit),
    ESP_ELFSYM_EXPORT(lv_ll_remove),
    ESP_ELFSYM_EXPORT(_ZGVNSt8time_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(esp_test_rx_process_complete),
    ESP_ELFSYM_EXPORT(s_itwt_flow_id_bitmap),
    ESP_ELFSYM_EXPORT(tcp_rexmit_rto),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_rpa),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_post_rd_p256_pubkey_cmp_evt),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_restart_link),
    ESP_ELFSYM_EXPORT(wifi_stop_process),
    ESP_ELFSYM_EXPORT(ff_wl_handles),
    ESP_ELFSYM_EXPORT(ppRecycleAmpdu),
    ESP_ELFSYM_EXPORT(_ZSt17iostream_categoryv),
    ESP_ELFSYM_EXPORT(_ZTVSt14basic_ifstreamIcSt11char_traitsIcEE),
    ESP_ELFSYM_EXPORT(pm_beacon_offset_is_enabled),
    ESP_ELFSYM_EXPORT(range_read_addr_blocks),
    ESP_ELFSYM_EXPORT(wpa_ether_send),
    ESP_ELFSYM_EXPORT(wifi_softap_process_bsscolor_collision),
    ESP_ELFSYM_EXPORT(lv_draw_label),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSt14basic_ifstreamIcSt11char_traitsIcEED0Ev),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_timer_tick_get),
    ESP_ELFSYM_EXPORT(xTaskGetIdleTaskHandleForCore),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_chan_in_range),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx1110moneypunctIwLb1EE2idE),
    ESP_ELFSYM_EXPORT(lv_obj_remove_style_all),
    ESP_ELFSYM_EXPORT(i2c_controller_register16be_set),
    ESP_ELFSYM_EXPORT(r_os_msys_unregister),
    ESP_ELFSYM_EXPORT(multi_heap_internal_lock),
    ESP_ELFSYM_EXPORT(hal_he_get_mplen_addr_start),
    ESP_ELFSYM_EXPORT(ble_hs_id_ensure_ctx),
    ESP_ELFSYM_EXPORT(nan_stop),
    ESP_ELFSYM_EXPORT(lmacStopTransmit),
    ESP_ELFSYM_EXPORT(r_ble_ll_calc_offset_ticks_us_for_rampup),
    ESP_ELFSYM_EXPORT(phy_chan14_mic_cfg_new),
    ESP_ELFSYM_EXPORT(esp_io_expander_gpio_wrapper_configure_pin),
    ESP_ELFSYM_EXPORT(gWpaSm),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_sig),
    ESP_ELFSYM_EXPORT(phy_dis_hw_set_freq_new),
    ESP_ELFSYM_EXPORT(hci_driver_vhci_ops),
    ESP_ELFSYM_EXPORT(esp_wifi_set_igtk_internal),
    ESP_ELFSYM_EXPORT(gdma_stop),
    ESP_ELFSYM_EXPORT(esp_clk_tree_lp_slow_get_freq_hz),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_crypt_and_tag),
    ESP_ELFSYM_EXPORT(spi_flash_needs_reset_check),
    ESP_ELFSYM_EXPORT(pmu_hal_hp_set_sleep_modem_backup_enable),
    ESP_ELFSYM_EXPORT(__nedf2),
    ESP_ELFSYM_EXPORT(lv_strcpy),
    ESP_ELFSYM_EXPORT(module_add),
    ESP_ELFSYM_EXPORT(mmu_hal_vaddr_to_paddr),
    ESP_ELFSYM_EXPORT(_Z33app_fs_list_direct_subdirectoriesRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERSt6vectorIS4_SaIS4_EE),
    ESP_ELFSYM_EXPORT(lv_calendar_header_arrow_class),
    ESP_ELFSYM_EXPORT(mbedtls_oid_get_ec_grp_algid),
    ESP_ELFSYM_EXPORT(ble_gap_rx_disconn_complete),
    ESP_ELFSYM_EXPORT(esp_test_tx_count_timeout),
    ESP_ELFSYM_EXPORT(esp_wifi_power_domain_off),
    ESP_ELFSYM_EXPORT(pbuf_take),
    ESP_ELFSYM_EXPORT(esp_mmu_vaddr_to_paddr),
    ESP_ELFSYM_EXPORT(sys_arch_mbox_tryfetch),
    ESP_ELFSYM_EXPORT(trcAmpduSetState),
    ESP_ELFSYM_EXPORT(http_parser_parse_url),
    ESP_ELFSYM_EXPORT(mbedtls_psa_rsa_export_key),
    ESP_ELFSYM_EXPORT(ble_store_iterate),
    ESP_ELFSYM_EXPORT(netif_remove_ext_callback),
    ESP_ELFSYM_EXPORT(preferences_put_int32),
    ESP_ELFSYM_EXPORT(ppRecycleRxPkt),
    ESP_ELFSYM_EXPORT(r_ble_phy_disable),
    ESP_ELFSYM_EXPORT(esp_phy_load_cal_data_from_nvs),
    ESP_ELFSYM_EXPORT(_interrupt_handler),
    ESP_ELFSYM_EXPORT(phy_xpd_tsens),
    ESP_ELFSYM_EXPORT(ip_addr_any),
    ESP_ELFSYM_EXPORT(wifi_create_nan),
    ESP_ELFSYM_EXPORT(ble_hs_conn_find_by_idx),
    ESP_ELFSYM_EXPORT(ic_mac_deinit),
    ESP_ELFSYM_EXPORT(clearerr),
    ESP_ELFSYM_EXPORT(lv_arc_get_angle_start),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_configure_wake_window),
    ESP_ELFSYM_EXPORT(phymode2str),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_update),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_check_acad),
    ESP_ELFSYM_EXPORT(xRingbufferReceiveFromISR),
    ESP_ELFSYM_EXPORT(f_unlink),
    ESP_ELFSYM_EXPORT(esp_flash_spi_init_include_func),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_read_type),
    ESP_ELFSYM_EXPORT(trackball_read_delta),
    ESP_ELFSYM_EXPORT(lv_obj_calculate_ext_draw_size),
    ESP_ELFSYM_EXPORT(lv_timer_resume),
    ESP_ELFSYM_EXPORT(_ZTVSt5ctypeIcE),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_master_new),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_sign_restartable),
    ESP_ELFSYM_EXPORT(copy_arguments),
    ESP_ELFSYM_EXPORT(_lock_release),
    ESP_ELFSYM_EXPORT(i2s_hal_std_set_rx_slot),
    ESP_ELFSYM_EXPORT(coshf),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_rf_ctrl_disable),
    ESP_ELFSYM_EXPORT(pm_mac_enable_tsf_tbtt_modem_wakeup),
    ESP_ELFSYM_EXPORT(lv_textarea_get_accepted_chars),
    ESP_ELFSYM_EXPORT(lv_fs_deinit),
    ESP_ELFSYM_EXPORT(sdmmc_can_trim),
    ESP_ELFSYM_EXPORT(scan_inter_channel_timeout_process),
    ESP_ELFSYM_EXPORT(ble_random_adjust_co_count),
    ESP_ELFSYM_EXPORT(phy_rfrx_gain_cal),
    ESP_ELFSYM_EXPORT(ieee80211_hostapd_ps_txcb),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handshake_step),
    ESP_ELFSYM_EXPORT(gdma_hal_enable_intr),
    ESP_ELFSYM_EXPORT(ble_random_conn_env_p),
    ESP_ELFSYM_EXPORT(hal_disable_sta_tbtt),
    ESP_ELFSYM_EXPORT(g_sw_cte_buffer_ptr),
    ESP_ELFSYM_EXPORT(pmu_hp_system_power_param_default),
    ESP_ELFSYM_EXPORT(esp_psram_impl_enable),
    ESP_ELFSYM_EXPORT(esf_buf_recycle),
    ESP_ELFSYM_EXPORT(xEventGroupClearBitsFromISR),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_verify_restartable),
    ESP_ELFSYM_EXPORT(npl_freertos_time_get),
    ESP_ELFSYM_EXPORT(ble_store_read_our_sec),
    ESP_ELFSYM_EXPORT(audio_stream_open_output),
    ESP_ELFSYM_EXPORT(tcp_bind_netif),
    ESP_ELFSYM_EXPORT(spi_flash_hal_configure_host_io_mode),
    ESP_ELFSYM_EXPORT(ip6_options_add_hbh_ra),
    ESP_ELFSYM_EXPORT(__truncdfsf2),
    ESP_ELFSYM_EXPORT(lv_arc_class),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_setkey),
    ESP_ELFSYM_EXPORT(g_panic_abort),
    ESP_ELFSYM_EXPORT(__lock___dd_hash_mutex),
    ESP_ELFSYM_EXPORT(pmu_lp_system_analog_param_default),
    ESP_ELFSYM_EXPORT(bt_bb_tx_cca_fifo_count),
    ESP_ELFSYM_EXPORT(net80211_funcs_deinit),
    ESP_ELFSYM_EXPORT(lv_indev_get_display),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service5audio12AudioServiceE),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand_data_get),
    ESP_ELFSYM_EXPORT(__lock___tz_mutex),
    ESP_ELFSYM_EXPORT(display_get_swap_xy),
    ESP_ELFSYM_EXPORT(hal_mac_color_get_collide_bitmap),
    ESP_ELFSYM_EXPORT(r_ble_controller_init),
    ESP_ELFSYM_EXPORT(sdmmc_erase_sectors),
    ESP_ELFSYM_EXPORT(modem_clock_hal_enable_wifipwr_clock),
    ESP_ELFSYM_EXPORT(scan_add_probe_ssid),
    ESP_ELFSYM_EXPORT(wpa_alloc_eapol),
    ESP_ELFSYM_EXPORT(r_esp_ble_set_cca_enable),
    ESP_ELFSYM_EXPORT(mbedtls_gcm_starts),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_rd_auth_pyld_tmo),
    ESP_ELFSYM_EXPORT(bt_set_rx_comp),
    ESP_ELFSYM_EXPORT(int_array_len),
    ESP_ELFSYM_EXPORT(phy_bt_txdc_cal),
    ESP_ELFSYM_EXPORT(phy_freq_module_resetn),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_remove_overtime_entries),
    ESP_ELFSYM_EXPORT(lv_draw_sw_grad_color_calculate),
    ESP_ELFSYM_EXPORT(_ZSt7vformatB5cxx11St17basic_string_viewIcSt11char_traitsIcEESt17basic_format_argsISt20basic_format_contextINSt8__format10_Sink_iterIcEEcEE),
    ESP_ELFSYM_EXPORT(mbedtls_x509_parse_subject_alt_name),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_event_is_destroyed),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_a2dp_wifi_scan),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_switch_func),
    ESP_ELFSYM_EXPORT(audio_stream_close),
    ESP_ELFSYM_EXPORT(_ZZNSt18__moneypunct_cacheIcLb0EE8_M_cacheERKSt6localeEN11_Scoped_strC1ERKSs),
    ESP_ELFSYM_EXPORT(ble_gap_passkey_event),
    ESP_ELFSYM_EXPORT(ble_hs_flow_init),
    ESP_ELFSYM_EXPORT(ieee80211_get_spp),
    ESP_ELFSYM_EXPORT(vListInsertEnd),
    ESP_ELFSYM_EXPORT(get_vendor_ie),
    ESP_ELFSYM_EXPORT(esp_crypto_aes_gcm_encrypt),
    ESP_ELFSYM_EXPORT(strcasestr),
    ESP_ELFSYM_EXPORT(phy_linear_to_db),
    ESP_ELFSYM_EXPORT(power_supply_is_quick_charge_enabled),
    ESP_ELFSYM_EXPORT(ets_timer_setfn),
    ESP_ELFSYM_EXPORT(ble_att_tx),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_addr_ring_list_init),
    ESP_ELFSYM_EXPORT(rtc_clk_32k_enable),
    ESP_ELFSYM_EXPORT(ic_get_pp_hdl),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY_PURPOSE_3),
    ESP_ELFSYM_EXPORT(file_mutex_try_lock),
    ESP_ELFSYM_EXPORT(regdma_link_set_write_wait_content),
    ESP_ELFSYM_EXPORT(lv_bar_set_range),
    ESP_ELFSYM_EXPORT(esp_aes_crypt_ctr),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_dup_move_to_head),
    ESP_ELFSYM_EXPORT(key_mgr_hal_get_state),
    ESP_ELFSYM_EXPORT(device_get_first_by_type),
    ESP_ELFSYM_EXPORT(mbedtls_pkcs5_pbes2_ext),
    ESP_ELFSYM_EXPORT(ble_att_svr_find_by_handle),
    ESP_ELFSYM_EXPORT(f_opendir),
    ESP_ELFSYM_EXPORT(_ZSt28__throw_bad_array_new_lengthv),
    ESP_ELFSYM_EXPORT(ieee80211_amsdu_send_check),
    ESP_ELFSYM_EXPORT(scan_remove_probe_ssid),
    ESP_ELFSYM_EXPORT(g_ble_phy_t_rxaddrdelay_ro),
    ESP_ELFSYM_EXPORT(g_config_softap_on_dfs_channel),
    ESP_ELFSYM_EXPORT(tt_app_fileselection_start_for_existing_or_new_file),
    ESP_ELFSYM_EXPORT(strnlen),
    ESP_ELFSYM_EXPORT(esp_http_client_get_header),
    ESP_ELFSYM_EXPORT(raw_recv),
    ESP_ELFSYM_EXPORT(lwip_setsockopt_impl_ext),
    ESP_ELFSYM_EXPORT(hal_mac_is_low_rate_enabled),
    ESP_ELFSYM_EXPORT(esp_ecc_point_multiply),
    ESP_ELFSYM_EXPORT(wifi_set_appie_process),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_rx_process),
    ESP_ELFSYM_EXPORT(libcoexist_reversion_git),
    ESP_ELFSYM_EXPORT(psa_validate_key_persistence),
    ESP_ELFSYM_EXPORT(he_recv_action_muedca_reset),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_rx_process),
    ESP_ELFSYM_EXPORT(console_open),
    ESP_ELFSYM_EXPORT(lvgl_get_statusbar_icon_font),
    ESP_ELFSYM_EXPORT(lv_style_get_prop),
    ESP_ELFSYM_EXPORT(USB_HOST_MSC_TYPE),
    ESP_ELFSYM_EXPORT(_open_r),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_switch_delay_rx_aoa_1us_2m),
    ESP_ELFSYM_EXPORT(lv_palette_darken),
    ESP_ELFSYM_EXPORT(phy_bt_tx_gain_init_new),
    ESP_ELFSYM_EXPORT(wpa_auth_sta_deinit),
    ESP_ELFSYM_EXPORT(stdin),
    ESP_ELFSYM_EXPORT(g_hci_stack_envP),
    ESP_ELFSYM_EXPORT(esp_http_client_open),
    ESP_ELFSYM_EXPORT(lv_draw_fill),
    ESP_ELFSYM_EXPORT(esp_setup_newlib_syscalls),
    ESP_ELFSYM_EXPORT(bluetooth_set_device_name),
    ESP_ELFSYM_EXPORT(sntp_stop),
    ESP_ELFSYM_EXPORT(ppDisableQueue),
    ESP_ELFSYM_EXPORT(lv_obj_set_flex_grow),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_unacked_ctrl_pdu_handle),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_ssr),
    ESP_ELFSYM_EXPORT(esp_flash_freq_limit_cb),
    ESP_ELFSYM_EXPORT(wdev_csi_len_align),
    ESP_ELFSYM_EXPORT(__global_locale),
    ESP_ELFSYM_EXPORT(aes_hal_set_iv),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_enqueue_pkt),
    ESP_ELFSYM_EXPORT(lv_cache_entry_inc_ref),
    ESP_ELFSYM_EXPORT(uECC_shared_secret),
    ESP_ELFSYM_EXPORT(fseek),
    ESP_ELFSYM_EXPORT(lv_color_to_u16),
    ESP_ELFSYM_EXPORT(pm_beacon_monitor_timeout_process),
    ESP_ELFSYM_EXPORT(pm_on_data_rx),
    ESP_ELFSYM_EXPORT(r_os_msys_register_block_num_get_cb),
    ESP_ELFSYM_EXPORT(esp_sha256_driver_finish),
    ESP_ELFSYM_EXPORT(phy_rx_gain_force),
    ESP_ELFSYM_EXPORT(esp_wifi_register_wpa3_cb),
    ESP_ELFSYM_EXPORT(wpa_parse_wpa_ie_wrapper),
    ESP_ELFSYM_EXPORT(ppDequeueTxQ),
    ESP_ELFSYM_EXPORT(psa_crypto_init),
    ESP_ELFSYM_EXPORT(pm_offchan_is_waiting),
    ESP_ELFSYM_EXPORT(vTaskSetTimeOutState),
    ESP_ELFSYM_EXPORT(register_ieee80211_action_vendor_spec_cb),
    ESP_ELFSYM_EXPORT(lv_anim_path_ease_in_out),
    ESP_ELFSYM_EXPORT(esp_phy_disable),
    ESP_ELFSYM_EXPORT(BLUETOOTH_MIDI_TYPE),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_update_aux_offset_in_ext),
    ESP_ELFSYM_EXPORT(ic_register_pm_tx_null_cb),
    ESP_ELFSYM_EXPORT(s_pm_beacon_offset),
    ESP_ELFSYM_EXPORT(lv_cache_set_name),
    ESP_ELFSYM_EXPORT(lv_canvas_class),
    ESP_ELFSYM_EXPORT(driver_find_compatible),
    ESP_ELFSYM_EXPORT(preferences_close),
    ESP_ELFSYM_EXPORT(ieee80211_setup_rateset),
    ESP_ELFSYM_EXPORT(sdmmc_mmc_enable_hs_mode),
    ESP_ELFSYM_EXPORT(lv_style_set_bg_image_src),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_remote_read_transmit_power_level),
    ESP_ELFSYM_EXPORT(exp),
    ESP_ELFSYM_EXPORT(ledc_hal_set_fade_param),
    ESP_ELFSYM_EXPORT(pm_set_beacon_filter),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_flex_cross_place),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_align),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_env_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_fetch_new),
    ESP_ELFSYM_EXPORT(root_is_model),
    ESP_ELFSYM_EXPORT(lv_menu_set_page),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_global_block_malloc),
    ESP_ELFSYM_EXPORT(ble_sm_deinit),
    ESP_ELFSYM_EXPORT(wps_get_wps_sm_cb),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_eSt12chars_formati),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_find_type_value_hinfo),
    ESP_ELFSYM_EXPORT(esp_netif_get_nr_of_ifs),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_cmp_mpi),
    ESP_ELFSYM_EXPORT(SPI_CONTROLLER_TYPE),
    ESP_ELFSYM_EXPORT(rtc_clk_slow_src_get),
    ESP_ELFSYM_EXPORT(display_get_frame_buffer),
    ESP_ELFSYM_EXPORT(memp_ARP_QUEUE),
    ESP_ELFSYM_EXPORT(tcp_fasttmr),
    ESP_ELFSYM_EXPORT(ieee80211_ht_updatehtcap),
    ESP_ELFSYM_EXPORT(lv_style_set_length),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_channel_status_report_timer_cb),
    ESP_ELFSYM_EXPORT(sin),
    ESP_ELFSYM_EXPORT(spicommon_irqsource_for_host),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_dSt12chars_format),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_partition_drive),
    ESP_ELFSYM_EXPORT(phy_txcal_debuge_mode_new),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_free_rx_buffer),
    ESP_ELFSYM_EXPORT(lv_arc_create),
    ESP_ELFSYM_EXPORT(phy_iq_corr_enable),
    ESP_ELFSYM_EXPORT(phy_agc_max_gain_set),
    ESP_ELFSYM_EXPORT(tanf),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_beacon_miss_limit_exceeded_wakeup_disable),
    ESP_ELFSYM_EXPORT(dts_modules),
    ESP_ELFSYM_EXPORT(vfs_include_syscalls_impl),
    ESP_ELFSYM_EXPORT(__bufio_put),
    ESP_ELFSYM_EXPORT(udp_input),
    ESP_ELFSYM_EXPORT(ieee80211_update_twt),
    ESP_ELFSYM_EXPORT(bt_agc_detect_set),
    ESP_ELFSYM_EXPORT(panic_restart),
    ESP_ELFSYM_EXPORT(esp32_wifi_driver),
    ESP_ELFSYM_EXPORT(nimble_port_deinit),
    ESP_ELFSYM_EXPORT(uECC_vli_clear),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_check_pubkey),
    ESP_ELFSYM_EXPORT(r_hal_timer_process),
    ESP_ELFSYM_EXPORT(heap_caps_get_total_size),
    ESP_ELFSYM_EXPORT(tcp_close_ext),
    ESP_ELFSYM_EXPORT(pm_tx_null_data_done_twt_process),
    ESP_ELFSYM_EXPORT(vhtcaps_maxRxMPDULen),
    ESP_ELFSYM_EXPORT(lv_obj_get_height),
    ESP_ELFSYM_EXPORT(phy_wait_freq_hw_hop_done),
    ESP_ELFSYM_EXPORT(wep_decap),
    ESP_ELFSYM_EXPORT(check_bss_queue),
    ESP_ELFSYM_EXPORT(g_msys_pool_list),
    ESP_ELFSYM_EXPORT(bootloader_flash_update_id),
    ESP_ELFSYM_EXPORT(ieee80211_decap1),
    ESP_ELFSYM_EXPORT(wpa_auth_pmksa_add2),
    ESP_ELFSYM_EXPORT(lvgl_module_config),
    ESP_ELFSYM_EXPORT(lv_textarea_set_one_line),
    ESP_ELFSYM_EXPORT(phy_read_sar_dout),
    ESP_ELFSYM_EXPORT(ble_gap_adv_rsp_set_fields),
    ESP_ELFSYM_EXPORT(i2c_common_deinit_pins),
    ESP_ELFSYM_EXPORT(__netf2),
    ESP_ELFSYM_EXPORT(lv_obj_set_layout),
    ESP_ELFSYM_EXPORT(ieee80211_scan_deattach),
    ESP_ELFSYM_EXPORT(he_get_min_subframe_len),
    ESP_ELFSYM_EXPORT(crypto_bignum_init_set),
    ESP_ELFSYM_EXPORT(ieee80211_ioctl),
    ESP_ELFSYM_EXPORT(_ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_current_limit),
    ESP_ELFSYM_EXPORT(_Z24ble_schedule_adv_restartP6Devicey),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_get_next_rxbuf_reuse_sch_item),
    ESP_ELFSYM_EXPORT(pmksa_cache_get_current),
    ESP_ELFSYM_EXPORT(periph_rcc_acquire_exit),
    ESP_ELFSYM_EXPORT(coex_core_register_start_cb),
    ESP_ELFSYM_EXPORT(mbedtls_mutex_lock),
    ESP_ELFSYM_EXPORT(lv_obj_class),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_param_rr),
    ESP_ELFSYM_EXPORT(esp_sha512_driver_clone),
    ESP_ELFSYM_EXPORT(g_extAdv_stack_envP),
    ESP_ELFSYM_EXPORT(h_errno),
    ESP_ELFSYM_EXPORT(ppTxProtoProc),
    ESP_ELFSYM_EXPORT(cache_hal_invalidate_addr),
    ESP_ELFSYM_EXPORT(ampdu_free_extra_softap_rx_ba_index),
    ESP_ELFSYM_EXPORT(multi_heap_internal_unlock),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_set_cur_phy_txpwr),
    ESP_ELFSYM_EXPORT(scan_build_chan_list),
    ESP_ELFSYM_EXPORT(lv_theme_default_get),
    ESP_ELFSYM_EXPORT(lwip_gethostbyname),
    ESP_ELFSYM_EXPORT(esp_psram_impl_get_cs_io),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_client_cert_data),
    ESP_ELFSYM_EXPORT(mac_txrx_init),
    ESP_ELFSYM_EXPORT(rand),
    ESP_ELFSYM_EXPORT(sys_untimeout),
    ESP_ELFSYM_EXPORT(i2s_channel_reconfig_std_gpio),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_eSt12chars_format),
    ESP_ELFSYM_EXPORT(sntp_set_system_time),
    ESP_ELFSYM_EXPORT(zb_rx_core_set),
    ESP_ELFSYM_EXPORT(dhcp_network_changed_link_up),
    ESP_ELFSYM_EXPORT(g_ic),
    ESP_ELFSYM_EXPORT(he_recv_action_twt_information),
    ESP_ELFSYM_EXPORT(_Z19app_fs_is_directoryRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(memchr),
    ESP_ELFSYM_EXPORT(r_scan_duplicate_cache_refresh_timer_stop),
    ESP_ELFSYM_EXPORT(spi_hal_push_tx_buffer),
    ESP_ELFSYM_EXPORT(esp_efuse_startup_include_func),
    ESP_ELFSYM_EXPORT(vTaskDelete),
    ESP_ELFSYM_EXPORT(ble_gap_master_in_progress),
    ESP_ELFSYM_EXPORT(lv_spinbox_get_value),
    ESP_ELFSYM_EXPORT(tsf_hal_get_counter_value),
    ESP_ELFSYM_EXPORT(lv_bin_decoder_init),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_interface_name),
    ESP_ELFSYM_EXPORT(hostapd_setup_sae_pt),
    ESP_ELFSYM_EXPORT(ieee80211_add_operating_mode_notification),
    ESP_ELFSYM_EXPORT(device_construct),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_module_deinit),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_connect),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_detect_size),
    ESP_ELFSYM_EXPORT(he_recv_action_bsscolor_collision),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSoD1Ev),
    ESP_ELFSYM_EXPORT(pm_offchan_execute_ready_cb),
    ESP_ELFSYM_EXPORT(netconn_send),
    ESP_ELFSYM_EXPORT(cache_error_msg),
    ESP_ELFSYM_EXPORT(esp_flash_write),
    ESP_ELFSYM_EXPORT(driver_bind),
    ESP_ELFSYM_EXPORT(esp_perip_clk_init),
    ESP_ELFSYM_EXPORT(esp_netif_dhcpc_get_status),
    ESP_ELFSYM_EXPORT(psa_cipher_update),
    ESP_ELFSYM_EXPORT(esp_rtc_get_time_us),
    ESP_ELFSYM_EXPORT(lvgl_pointer_get_slot_index),
    ESP_ELFSYM_EXPORT(mpi_hal_enable_constant_time),
    ESP_ELFSYM_EXPORT(esp_vfs_null_register),
    ESP_ELFSYM_EXPORT(ble_hs_atomic_first_conn_handle),
    ESP_ELFSYM_EXPORT(phy_rfcal_data_check),
    ESP_ELFSYM_EXPORT(uart_hal_set_data_bit_num),
    ESP_ELFSYM_EXPORT(task_event_group_construct),
    ESP_ELFSYM_EXPORT(mbedtls_psa_aead_decrypt),
    ESP_ELFSYM_EXPORT(_ZTSN9__gnu_cxx26__concurrence_unlock_errorE),
    ESP_ELFSYM_EXPORT(gdma_periph_signals),
    ESP_ELFSYM_EXPORT(ble_gap_rx_update_complete),
    ESP_ELFSYM_EXPORT(VolToPart),
    ESP_ELFSYM_EXPORT(esp_key_mgr_activate_key),
    ESP_ELFSYM_EXPORT(lv_realloc),
    ESP_ELFSYM_EXPORT(strlcat),
    ESP_ELFSYM_EXPORT(r_ble_log_hci_vs_cmd_proc),
    ESP_ELFSYM_EXPORT(ble_gatts_rx_indicate_ack),
    ESP_ELFSYM_EXPORT(spi_flash_common_read_status_8b_rdsr2),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_rem_conn_parm_req),
    ESP_ELFSYM_EXPORT(lmacProcessShortFrameSuccess),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_pre_process),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_uhs1),
    ESP_ELFSYM_EXPORT(add_mic_ie_bip),
    ESP_ELFSYM_EXPORT(ble_sm_lgcy_io_action),
    ESP_ELFSYM_EXPORT(pm_unregister_connectionless_wake_window),
    ESP_ELFSYM_EXPORT(lmacRetryTxFrame),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_update_mem_last_ptr),
    ESP_ELFSYM_EXPORT(atan2f),
    ESP_ELFSYM_EXPORT(r_ble_ll_event_dbuf_overflow),
    ESP_ELFSYM_EXPORT(wifi_deinit),
    ESP_ELFSYM_EXPORT(i2c_hal_slave_init),
    ESP_ELFSYM_EXPORT(mbedtls_ct_memmove_left),
    ESP_ELFSYM_EXPORT(ble_hs_util_ensure_addr),
    ESP_ELFSYM_EXPORT(wpa3_hostap_auth_deinit),
    ESP_ELFSYM_EXPORT(_close_r),
    ESP_ELFSYM_EXPORT(power_supply_power_off),
    ESP_ELFSYM_EXPORT(crypto_ecdh_init),
    ESP_ELFSYM_EXPORT(lv_textarea_get_text_selection),
    ESP_ELFSYM_EXPORT(lv_binfont_destroy),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_instant_wakeup_check),
    ESP_ELFSYM_EXPORT(lv_image_cache_drop),
    ESP_ELFSYM_EXPORT(xpt2046_softspi_power_supply_driver),
    ESP_ELFSYM_EXPORT(modem_clock_get_module_deps),
    ESP_ELFSYM_EXPORT(xStreamBufferBytesAvailable),
    ESP_ELFSYM_EXPORT(ble_store_write_our_sec),
    ESP_ELFSYM_EXPORT(task_event_group_signal),
    ESP_ELFSYM_EXPORT(gps_get_state),
    ESP_ELFSYM_EXPORT(r_ble_ll_pdu_tx_time_get),
    ESP_ELFSYM_EXPORT(chm_set_current_band),
    ESP_ELFSYM_EXPORT(wifi_api_unlock),
    ESP_ELFSYM_EXPORT(lv_refr_now),
    ESP_ELFSYM_EXPORT(esp_vfs_register),
    ESP_ELFSYM_EXPORT(_ZTSSt11logic_error),
    ESP_ELFSYM_EXPORT(xTaskCreatePinnedToCoreWithCaps),
    ESP_ELFSYM_EXPORT(ble_sm_pair_initiate),
    ESP_ELFSYM_EXPORT(_ZZNKSt8__format14__formatter_fpIcE6formatIeNS_10_Sink_iterIcEEEENSt20basic_format_contextIT0_cE8iteratorET_RS7_ENKUlPcSB_E_clESB_SB_),
    ESP_ELFSYM_EXPORT(pm_is_waked),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_country),
    ESP_ELFSYM_EXPORT(wpa_compare_rsn_ie),
    ESP_ELFSYM_EXPORT(chm_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_event_is_destroyed),
    ESP_ELFSYM_EXPORT(phy_freq_band_reg_set),
    ESP_ELFSYM_EXPORT(uxQueueSpacesAvailable),
    ESP_ELFSYM_EXPORT(phy_bb_reg_init_new),
    ESP_ELFSYM_EXPORT(sta_rx_eapol),
    ESP_ELFSYM_EXPORT(_ctype_b),
    ESP_ELFSYM_EXPORT(cnx_node_search),
    ESP_ELFSYM_EXPORT(gScanStruct),
    ESP_ELFSYM_EXPORT(lv_obj_get_scroll_right),
    ESP_ELFSYM_EXPORT(isinf),
    ESP_ELFSYM_EXPORT(ble_gatts_rx_indicate_rsp),
    ESP_ELFSYM_EXPORT(s_wifi_task_hdl),
    ESP_ELFSYM_EXPORT(esp_netif_transmit_wrap),
    ESP_ELFSYM_EXPORT(__divdi3),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_calc_closest_event_cntr),
    ESP_ELFSYM_EXPORT(ble_uuid_init_from_att_mbuf),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_set_button_width),
    ESP_ELFSYM_EXPORT(phy_chan_vs_index),
    ESP_ELFSYM_EXPORT(phy_freq_to_index),
    ESP_ELFSYM_EXPORT(lv_theme_get_font_small),
    ESP_ELFSYM_EXPORT(bt_bb_tx_cca_fifo_read),
    ESP_ELFSYM_EXPORT(i2c_hal_master_handle_tx_event),
    ESP_ELFSYM_EXPORT(thread_get_task_handle),
    ESP_ELFSYM_EXPORT(lv_draw_image),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_frame_crc_state),
    ESP_ELFSYM_EXPORT(phy_close_fe_bb_clk),
    ESP_ELFSYM_EXPORT(printf_decode),
    ESP_ELFSYM_EXPORT(lv_fs_open),
    ESP_ELFSYM_EXPORT(lvgl_sliderbox_create),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_move_window_to),
    ESP_ELFSYM_EXPORT(ble_att_svr_prep_clear),
    ESP_ELFSYM_EXPORT(esp_bt_controller_get_status),
    ESP_ELFSYM_EXPORT(thread_set_main_function),
    ESP_ELFSYM_EXPORT(xStreamBufferSend),
    ESP_ELFSYM_EXPORT(pbuf_free_header),
    ESP_ELFSYM_EXPORT(mbedtls_psa_rsa_generate_key),
    ESP_ELFSYM_EXPORT(ble_store_write_local_irk),
    ESP_ELFSYM_EXPORT(gpio_od_disable),
    ESP_ELFSYM_EXPORT(i2s_gpio_check_and_set),
    ESP_ELFSYM_EXPORT(lv_display_add_event_cb),
    ESP_ELFSYM_EXPORT(hal_mac_txq_disable),
    ESP_ELFSYM_EXPORT(gpio_get_io_config),
    ESP_ELFSYM_EXPORT(_ZSt13__heap_selectIN9__gnu_cxx17__normal_iteratorIPtSt6vectorItSaItEEEENS0_5__ops15_Iter_less_iterEEvT_S9_S9_T0_),
    ESP_ELFSYM_EXPORT(wpa_auth_pmksa_add_sae),
    ESP_ELFSYM_EXPORT(esp_hw_stack_guard_get_bounds),
    ESP_ELFSYM_EXPORT(i2c_cmd_link_create),
    ESP_ELFSYM_EXPORT(pm_is_twt_start),
    ESP_ELFSYM_EXPORT(mbedtls_pk_rsa_set_key),
    ESP_ELFSYM_EXPORT(is_off_channel),
    ESP_ELFSYM_EXPORT(esp_netif_is_netif_listed),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_a2dp_wifi_connecting),
    ESP_ELFSYM_EXPORT(aes_hal_transform_block),
    ESP_ELFSYM_EXPORT(ieee80211_htinfo_update_chw),
    ESP_ELFSYM_EXPORT(chip_disable),
    ESP_ELFSYM_EXPORT(esp_aes_cipher_encrypt_setup),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_terminate_start),
    ESP_ELFSYM_EXPORT(ble_hid_set_active),
    ESP_ELFSYM_EXPORT(esf_buf_recycle_default_handler),
    ESP_ELFSYM_EXPORT(uart_periph_signal),
    ESP_ELFSYM_EXPORT(lv_text_cut),
    ESP_ELFSYM_EXPORT(adiList_stack_rmSidFromList),
    ESP_ELFSYM_EXPORT(gpio_intr_disable),
    ESP_ELFSYM_EXPORT(ieee80211_parse_extcap),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_stop),
    ESP_ELFSYM_EXPORT(pbuf_add_header),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_group_init),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_sniff_sco_wifi_connecting),
    ESP_ELFSYM_EXPORT(pm_twt_set_state),
    ESP_ELFSYM_EXPORT(hal_vhtdlmu_set_group_id),
    ESP_ELFSYM_EXPORT(ble_gap_tx_test_evt),
    ESP_ELFSYM_EXPORT(cJSON_GetArrayItem),
    ESP_ELFSYM_EXPORT(r_DB_check_hash_exist),
    ESP_ELFSYM_EXPORT(phy_set_txcap_reg),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_blocklen),
    ESP_ELFSYM_EXPORT(fmodf),
    ESP_ELFSYM_EXPORT(wdevProcessModemStateRxBeacon),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceIN2tt6PubSubI9WifiEventEESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(haptic_set_waveform),
    ESP_ELFSYM_EXPORT(memp_NETDB),
    ESP_ELFSYM_EXPORT(cnx_obss_scan_timeout),
    ESP_ELFSYM_EXPORT(lv_draw_buf_init_handlers),
    ESP_ELFSYM_EXPORT(coex_schm_ble_default_bt_idle_wifi_connecting),
    ESP_ELFSYM_EXPORT(nan_sm_handle_event),
    ESP_ELFSYM_EXPORT(hal_mac_rx_set_policy),
    ESP_ELFSYM_EXPORT(module_destruct),
    ESP_ELFSYM_EXPORT(r_ble_ll_read_supp_features),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_buffer_hdr),
    ESP_ELFSYM_EXPORT(pau_hal_set_regdma_wait_timeout),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_subject_alt_name_ext),
    ESP_ELFSYM_EXPORT(tsf_hal_set_tbtt_intr_enable),
    ESP_ELFSYM_EXPORT(hci_transport_init),
    ESP_ELFSYM_EXPORT(ble_sm_process_result),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_set_map),
    ESP_ELFSYM_EXPORT(netif_set_addr),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_gethostbyname),
    ESP_ELFSYM_EXPORT(lv_obj_move_children_by),
    ESP_ELFSYM_EXPORT(hal_tsf_clear_soc_wakeup_request),
    ESP_ELFSYM_EXPORT(spi_bus_register_destroy_func),
    ESP_ELFSYM_EXPORT(coex_schm_bt_a2dp_paused_wifi_conn),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_stop),
    ESP_ELFSYM_EXPORT(mesh_sta_auth_expire_time),
    ESP_ELFSYM_EXPORT(dispatcher_consume_timed),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_rx_pkt_in_restore_addr_data),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_f),
    ESP_ELFSYM_EXPORT(he_send_action_bsscolor_collision),
    ESP_ELFSYM_EXPORT(hal_vhtdlmu_clr_group_id),
    ESP_ELFSYM_EXPORT(app_manager_get_topmost_app_id),
    ESP_ELFSYM_EXPORT(wifi_process_bsscolor_collision),
    ESP_ELFSYM_EXPORT(esp_vfs_select_triggered_isr),
    ESP_ELFSYM_EXPORT(esp_lcd_panel_disp_sleep),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_filter_enable),
    ESP_ELFSYM_EXPORT(wifi_nvs_set),
    ESP_ELFSYM_EXPORT(lv_label_get_recolor),
    ESP_ELFSYM_EXPORT(lv_display_get_screen_active),
    ESP_ELFSYM_EXPORT(phy_chip_set_chan_offset),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_find_type_value_complete),
    ESP_ELFSYM_EXPORT(ppProcTxSecFrame),
    ESP_ELFSYM_EXPORT(wifi_softap_get_config),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_piscan_wifi_conn),
    ESP_ELFSYM_EXPORT(phy_set_tsens_power),
    ESP_ELFSYM_EXPORT(_ZGVNSt8time_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_alloc_memory),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_reset_sm),
    ESP_ELFSYM_EXPORT(bundle_alloc),
    ESP_ELFSYM_EXPORT(g_ble_ll_pdu_header_tx_time_ro),
    ESP_ELFSYM_EXPORT(ble_transport_to_hs_acl_impl),
    ESP_ELFSYM_EXPORT(isalpha),
    ESP_ELFSYM_EXPORT(wifi_station_get_rssi),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_npl_init),
    ESP_ELFSYM_EXPORT(phy_rf_init),
    ESP_ELFSYM_EXPORT(wpa_free_eapol),
    ESP_ELFSYM_EXPORT(lv_obj_update_layer_type),
    ESP_ELFSYM_EXPORT(lv_obj_refresh_style),
    ESP_ELFSYM_EXPORT(phy_xtal_duty_cal_init),
    ESP_ELFSYM_EXPORT(dts_devices),
    ESP_ELFSYM_EXPORT(ble_gap_event_listener_unregister),
    ESP_ELFSYM_EXPORT(haptic_stop_playback),
    ESP_ELFSYM_EXPORT(lv_obj_add_event_cb),
    ESP_ELFSYM_EXPORT(lv_draw_sw_init),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_fSt12chars_format),
    ESP_ELFSYM_EXPORT(GPS_TYPE),
    ESP_ELFSYM_EXPORT(memp_PBUF_POOL),
    ESP_ELFSYM_EXPORT(cJSON_ParseWithLengthOpts),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_alloc_rx_buffer),
    ESP_ELFSYM_EXPORT(key_mgr_hal_set_key_purpose),
    ESP_ELFSYM_EXPORT(wpa_sm_drop_sa),
    ESP_ELFSYM_EXPORT(regdma_link_new_addr_map_default),
    ESP_ELFSYM_EXPORT(cnx_handshake_timeout),
    ESP_ELFSYM_EXPORT(sdmmc_io_init_check_card_cap),
    ESP_ELFSYM_EXPORT(sdmmc_io_init_read_card_cap),
    ESP_ELFSYM_EXPORT(ble_store_delete_peer_sec),
    ESP_ELFSYM_EXPORT(netbuf_delete),
    ESP_ELFSYM_EXPORT(phy_freq_to_mbgain),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_parse_der_nocopy),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_scr),
    ESP_ELFSYM_EXPORT(ip6_frag),
    ESP_ELFSYM_EXPORT(dhcp_append_extra_opts),
    ESP_ELFSYM_EXPORT(ppCheckTxAMPDUlength),
    ESP_ELFSYM_EXPORT(lwip_getsockopt),
    ESP_ELFSYM_EXPORT(ble_random_mode_conn_exec_process_cb_get),
    ESP_ELFSYM_EXPORT(phy_tx_pwctrl_init_cal_new),
    ESP_ELFSYM_EXPORT(ppPrepareBarFrame),
    ESP_ELFSYM_EXPORT(esp_register_freertos_idle_hook_for_cpu),
    ESP_ELFSYM_EXPORT(esp_http_client_read),
    ESP_ELFSYM_EXPORT(usb_midi_device_send),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_info),
    ESP_ELFSYM_EXPORT(esp_sha1_driver_compute),
    ESP_ELFSYM_EXPORT(ble_uuid_to_str),
    ESP_ELFSYM_EXPORT(scan_enter_oper_channel_process),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_free),
    ESP_ELFSYM_EXPORT(esp_supplicant_common_deinit),
    ESP_ELFSYM_EXPORT(_ZTVN2tt7service9webserver16WebServerServiceE),
    ESP_ELFSYM_EXPORT(mbedtls_pk_psa_rsa_sign_ext),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_conf_alpn_protocols),
    ESP_ELFSYM_EXPORT(r_ble_hw_whitelist_add),
    ESP_ELFSYM_EXPORT(esp_log_cache_add),
    ESP_ELFSYM_EXPORT(s_nan_cb),
    ESP_ELFSYM_EXPORT(phy_bb_dcmem_clr),
    ESP_ELFSYM_EXPORT(lv_display_get_dpi),
    ESP_ELFSYM_EXPORT(r_os_mbuf_copyinto),
    ESP_ELFSYM_EXPORT(sdmmc_io_enable_int),
    ESP_ELFSYM_EXPORT(ampdu_dispatch_upto),
    ESP_ELFSYM_EXPORT(r_ble_rtc_wake_up_state_clr),
    ESP_ELFSYM_EXPORT(ble_gap_pairing_complete_event),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_stop),
    ESP_ELFSYM_EXPORT(ble_att_conn_chan_find),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_on_list),
    ESP_ELFSYM_EXPORT(rcGetDCMMaxRate),
    ESP_ELFSYM_EXPORT(app_metadata_parse),
    ESP_ELFSYM_EXPORT(pm_coex_schm_process_restart),
    ESP_ELFSYM_EXPORT(nan_dp_node_exists),
    ESP_ELFSYM_EXPORT(dbg_cnt_lmac_drop),
    ESP_ELFSYM_EXPORT(aes_decrypt),
    ESP_ELFSYM_EXPORT(_ZTSSt13runtime_error),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_cfg_ll),
    ESP_ELFSYM_EXPORT(lv_obj_set_align),
    ESP_ELFSYM_EXPORT(lv_style_prop_get_default),
    ESP_ELFSYM_EXPORT(sha_hal_hash_dma),
    ESP_ELFSYM_EXPORT(bluetooth_hid_device_send_mouse),
    ESP_ELFSYM_EXPORT(ieee80211_add_action_vendor_spec_esp),
    ESP_ELFSYM_EXPORT(_ZTVSt9money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE),
    ESP_ELFSYM_EXPORT(_ZGVNSt9money_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(trc_update_ifx_phy_mode),
    ESP_ELFSYM_EXPORT(esp_restart),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_rmv),
    ESP_ELFSYM_EXPORT(ble_gattc_init),
    ESP_ELFSYM_EXPORT(_ZTVSt8ios_base),
    ESP_ELFSYM_EXPORT(phy_clr_modem_flag),
    ESP_ELFSYM_EXPORT(spi_bus_lock_get_dev_id),
    ESP_ELFSYM_EXPORT(lv_anim_set_repeat_delay),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_alloc_memory),
    ESP_ELFSYM_EXPORT(lvgl_module_configure),
    ESP_ELFSYM_EXPORT(lv_rb_insert),
    ESP_ELFSYM_EXPORT(wifi_assert),
    ESP_ELFSYM_EXPORT(_ZZNKSt7collateIcE12do_transformEPKcS2_EN4_BufD2Ev),
    ESP_ELFSYM_EXPORT(audio_codec_set_volume),
    ESP_ELFSYM_EXPORT(httpd_resp_send),
    ESP_ELFSYM_EXPORT(lv_obj_stop_scroll_anim),
    ESP_ELFSYM_EXPORT(__ffsdi2),
    ESP_ELFSYM_EXPORT(color_change_timer),
    ESP_ELFSYM_EXPORT(mbedtls_ct_zeroize_if),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_env_deinit),
    ESP_ELFSYM_EXPORT(ble_gap_adv_stop),
    ESP_ELFSYM_EXPORT(sys_mbox_new),
    ESP_ELFSYM_EXPORT(lv_chart_set_next_value),
    ESP_ELFSYM_EXPORT(gdma_register_rx_event_callbacks),
    ESP_ELFSYM_EXPORT(phy_freq_correct),
    ESP_ELFSYM_EXPORT(phy_set_chanfreq),
    ESP_ELFSYM_EXPORT(rename),
    ESP_ELFSYM_EXPORT(ic_set_current_channel),
    ESP_ELFSYM_EXPORT(meshtastic_gps_driver),
    ESP_ELFSYM_EXPORT(pm_coex_set_reconnect_policy),
    ESP_ELFSYM_EXPORT(regdma_link_new_continuous_default),
    ESP_ELFSYM_EXPORT(lv_color_mix32),
    ESP_ELFSYM_EXPORT(spi_flash_hal_check_status),
    ESP_ELFSYM_EXPORT(ble_ll_hci_ev_subrate_change),
    ESP_ELFSYM_EXPORT(esp_tls_get_bytes_avail),
    ESP_ELFSYM_EXPORT(psa_export_key),
    ESP_ELFSYM_EXPORT(i2s_controller_reset),
    ESP_ELFSYM_EXPORT(esp_wifi_auth_done_internal),
    ESP_ELFSYM_EXPORT(spi_flash_encryption_hal_enable_pseudo_rounds),
    ESP_ELFSYM_EXPORT(ap_no_lr),
    ESP_ELFSYM_EXPORT(material_symbols_shared_16),
    ESP_ELFSYM_EXPORT(spi_bus_lock_set_bg_control),
    ESP_ELFSYM_EXPORT(sdmmc_init_host_bus_width),
    ESP_ELFSYM_EXPORT(rc_get_trc),
    ESP_ELFSYM_EXPORT(conn_stack_getBroker),
    ESP_ELFSYM_EXPORT(sdmmc_init_ocr),
    ESP_ELFSYM_EXPORT(getc_unlocked),
    ESP_ELFSYM_EXPORT(tlsf_free),
    ESP_ELFSYM_EXPORT(s_itwt_suspend_flow_id_bitmap),
    ESP_ELFSYM_EXPORT(lv_style_set_transform_width),
    ESP_ELFSYM_EXPORT(xTimerGetPeriod),
    ESP_ELFSYM_EXPORT(esp_cache_sync_ops_exit_critical_section),
    ESP_ELFSYM_EXPORT(ble_hs_start),
    ESP_ELFSYM_EXPORT(dns_tmr),
    ESP_ELFSYM_EXPORT(he_time_per_sym),
    ESP_ELFSYM_EXPORT(coex_schm_bt_sniff_sco_wifi_scan),
    ESP_ELFSYM_EXPORT(pp_timer_register_post_cb),
    ESP_ELFSYM_EXPORT(pbuf_free_ooseq_pending),
    ESP_ELFSYM_EXPORT(pm_is_twt_send_null),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_read_mult_var),
    ESP_ELFSYM_EXPORT(esp_wifi_scan_stop),
    ESP_ELFSYM_EXPORT(lv_color_to_int),
    ESP_ELFSYM_EXPORT(esp_task_wdt_add),
    ESP_ELFSYM_EXPORT(__lshrdi3),
    ESP_ELFSYM_EXPORT(pm_send_nullfunc),
    ESP_ELFSYM_EXPORT(pmu_hal_hp_set_sleep_active_backup_disable),
    ESP_ELFSYM_EXPORT(__ascii_wctomb),
    ESP_ELFSYM_EXPORT(mpi_hal_wait_op_complete),
    ESP_ELFSYM_EXPORT(lv_anim_set_reverse_duration),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_safe_cond_swap),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_beacon_miss_intr_disable),
    ESP_ELFSYM_EXPORT(ieee80211_setup_lr_rates),
    ESP_ELFSYM_EXPORT(ble_sm_sc_ensure_ctx),
    ESP_ELFSYM_EXPORT(esp_aes_cipher_abort),
    ESP_ELFSYM_EXPORT(lv_text_get_width),
    ESP_ELFSYM_EXPORT(netconn_tcp_recvd),
    ESP_ELFSYM_EXPORT(ble_stack_enableVsCmds),
    ESP_ELFSYM_EXPORT(lvgl_window_manager_module),
    ESP_ELFSYM_EXPORT(multi_heap_set_lock),
    ESP_ELFSYM_EXPORT(misc_nvs_load),
    ESP_ELFSYM_EXPORT(r_ble_hw_get_public_addr),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_set_timer),
    ESP_ELFSYM_EXPORT(pointer_get_mirror_y),
    ESP_ELFSYM_EXPORT(r_DB_addr_list_add),
    ESP_ELFSYM_EXPORT(esp_intr_disable),
    ESP_ELFSYM_EXPORT(wifi_stop_sw_txq),
    ESP_ELFSYM_EXPORT(phy_module_disable),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_is_zero),
    ESP_ELFSYM_EXPORT(esp_flash_erase_region),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_add_if),
    ESP_ELFSYM_EXPORT(g_deauth_mac_list),
    ESP_ELFSYM_EXPORT(ble_hs_misc_restore_irks),
    ESP_ELFSYM_EXPORT(lv_display_get_inactive_time),
    ESP_ELFSYM_EXPORT(ieee80211w_get_igtk_from_keyidx),
    ESP_ELFSYM_EXPORT(r_ble_ll_is_valid_random_addr),
    ESP_ELFSYM_EXPORT(ieee80211_add_rates),
    ESP_ELFSYM_EXPORT(sdmmc_decode_csd),
    ESP_ELFSYM_EXPORT(dragonfly_is_quadratic_residue_blind),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand_env_init),
    ESP_ELFSYM_EXPORT(RC_GetCtsTime),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_periodic_set_param),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_cmd_get),
    ESP_ELFSYM_EXPORT(lmacRxDone),
    ESP_ELFSYM_EXPORT(ble_hs_conn_insert),
    ESP_ELFSYM_EXPORT(bin_clear_free),
    ESP_ELFSYM_EXPORT(lv_event_get_code),
    ESP_ELFSYM_EXPORT(ieee80211_construct_action_vendor_spec),
    ESP_ELFSYM_EXPORT(spi_flash_hal_read),
    ESP_ELFSYM_EXPORT(gpio_hal_hysteresis_soft_enable),
    ESP_ELFSYM_EXPORT(spicommon_cs_free_io),
    ESP_ELFSYM_EXPORT(spicommon_periph_free),
    ESP_ELFSYM_EXPORT(_pow),
    ESP_ELFSYM_EXPORT(hal_timer_update_by_rtc),
    ESP_ELFSYM_EXPORT(lv_fs_read),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_ns_cert_type),
    ESP_ELFSYM_EXPORT(update_cs_thres),
    ESP_ELFSYM_EXPORT(esp_transport_connect),
    ESP_ELFSYM_EXPORT(igmp_joingroup),
    ESP_ELFSYM_EXPORT(ble_freertos_evq_pool),
    ESP_ELFSYM_EXPORT(phy_get_tm7),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_RD_DIS_BLOCK_KEY1),
    ESP_ELFSYM_EXPORT(__cxa_throw_bad_array_new_length),
    ESP_ELFSYM_EXPORT(memory_alloc_with_policy),
    ESP_ELFSYM_EXPORT(esp_hmac_setup_transparent),
    ESP_ELFSYM_EXPORT(wDev_enable_low_rate),
    ESP_ELFSYM_EXPORT(wcsxfrm),
    ESP_ELFSYM_EXPORT(lv_ll_ins_prev),
    ESP_ELFSYM_EXPORT(_ZZNKSt7collateIwE12do_transformEPKwS2_EN4_BufD1Ev),
    ESP_ELFSYM_EXPORT(_ZSt11_Hash_bytesPKvjj),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_update),
    ESP_ELFSYM_EXPORT(ieee80211_parse_vhtcap),
    ESP_ELFSYM_EXPORT(wifi_lmac_init),
    ESP_ELFSYM_EXPORT(xt_unhandled_exception),
    ESP_ELFSYM_EXPORT(mbedtls_mutex_free),
    ESP_ELFSYM_EXPORT(wpa_sm_set_pmk_from_pmksa),
    ESP_ELFSYM_EXPORT(send_wake_null_timer),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_text_font),
    ESP_ELFSYM_EXPORT(pm_disconnected_sleep),
    ESP_ELFSYM_EXPORT(icmp6_input),
    ESP_ELFSYM_EXPORT(pm_on_isr_twt_wake),
    ESP_ELFSYM_EXPORT(xTimerGetTimerDaemonTaskHandle),
    ESP_ELFSYM_EXPORT(_ZZNSt18__moneypunct_cacheIcLb1EE8_M_cacheERKSt6localeEN11_Scoped_strC2ERKSs),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_is_le_event_enabled),
    ESP_ELFSYM_EXPORT(device_is_constructed),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_gtk_rekeying_config_internal),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_margin_right),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_write_le),
    ESP_ELFSYM_EXPORT(xTaskDelayUntil),
    ESP_ELFSYM_EXPORT(ieee80211_empty_txq),
    ESP_ELFSYM_EXPORT(r_os_cputime_usecs_to_ticks),
    ESP_ELFSYM_EXPORT(lv_obj_refresh_self_size),
    ESP_ELFSYM_EXPORT(pthread_mutex_unlock),
    ESP_ELFSYM_EXPORT(lv_tabview_set_active),
    ESP_ELFSYM_EXPORT(wpa_cipher_key_len),
    ESP_ELFSYM_EXPORT(auth_build_token_req),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_grid_cell_column_span),
    ESP_ELFSYM_EXPORT(mac_tx_set_plcp0),
    ESP_ELFSYM_EXPORT(wpa_gen_wpa_ie),
    ESP_ELFSYM_EXPORT(crypto_ec_get_prime),
    ESP_ELFSYM_EXPORT(ieee80211_ioctl_init),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_create),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_read),
    ESP_ELFSYM_EXPORT(hal_set_itwt_pti),
    ESP_ELFSYM_EXPORT(mbedtls_psa_cipher_abort),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_alloc),
    ESP_ELFSYM_EXPORT(lv_timer_set_cb),
    ESP_ELFSYM_EXPORT(mbedtls_psa_mac_verify_setup),
    ESP_ELFSYM_EXPORT(rx_evm_data_rate_mcs_coefficient),
    ESP_ELFSYM_EXPORT(coex_schm_bt_idle_wifi_conn),
    ESP_ELFSYM_EXPORT(rgb_led_get_color),
    ESP_ELFSYM_EXPORT(ieee80211_ioctl_process),
    ESP_ELFSYM_EXPORT(__atomic_fetch_or_8),
    ESP_ELFSYM_EXPORT(advFilter_stack_enableDupExcListVsCmd),
    ESP_ELFSYM_EXPORT(bt_bb_tx_pwr_table_get),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_update_ad),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_get),
    ESP_ELFSYM_EXPORT(etharp_cleanup_netif),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_connect_mem),
    ESP_ELFSYM_EXPORT(pm_send_wake_null_cb),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_delay_rx_aoa_2us),
    ESP_ELFSYM_EXPORT(netconn_new_with_proto_and_callback),
    ESP_ELFSYM_EXPORT(ble_stack_initEnv),
    ESP_ELFSYM_EXPORT(g_timer_info),
    ESP_ELFSYM_EXPORT(lv_dropdown_get_list),
    ESP_ELFSYM_EXPORT(esp_netif_remove_from_list_unsafe),
    ESP_ELFSYM_EXPORT(usb_midi_device_get),
    ESP_ELFSYM_EXPORT(mbedtls_hmac_drbg_update),
    ESP_ELFSYM_EXPORT(localeconv),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_find_type_value),
    ESP_ELFSYM_EXPORT(ds_hal_configure_iv),
    ESP_ELFSYM_EXPORT(lvgl_display_add),
    ESP_ELFSYM_EXPORT(esp_partition_erase_range),
    ESP_ELFSYM_EXPORT(__mulsf3),
    ESP_ELFSYM_EXPORT(lv_obj_tree_walk),
    ESP_ELFSYM_EXPORT(esp_cache_freeze_caches_disable_interrupts),
    ESP_ELFSYM_EXPORT(esp_mbedtls_verify_certificate),
    ESP_ELFSYM_EXPORT(lv_obj_send_event),
    ESP_ELFSYM_EXPORT(hal_mac_color_clr_bitmap),
    ESP_ELFSYM_EXPORT(cnx_auth_done),
    ESP_ELFSYM_EXPORT(r_ble_lll_env_deinit),
    ESP_ELFSYM_EXPORT(nimble_port_run),
    ESP_ELFSYM_EXPORT(usb_midi_set_callback),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_send_scan_req_recv),
    ESP_ELFSYM_EXPORT(_ZTTNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(lv_draw_line),
    ESP_ELFSYM_EXPORT(rcUpSched),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_free),
    ESP_ELFSYM_EXPORT(wpa_pmk_to_ptk),
    ESP_ELFSYM_EXPORT(tcp_active_pcbs_changed),
    ESP_ELFSYM_EXPORT(esp_mspi_32bit_address_flash_feature_check),
    ESP_ELFSYM_EXPORT(ble_hs_hci_util_handle_pb_bc_join),
    ESP_ELFSYM_EXPORT(lv_style_set_text_font),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_cth_flow_set_buffers),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_RD_DIS_BLOCK_KEY5),
    ESP_ELFSYM_EXPORT(os_timer_done),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_transform_pivot_x),
    ESP_ELFSYM_EXPORT(wifi_ap_pmf_enabled),
    ESP_ELFSYM_EXPORT(phy_get_rom_ver),
    ESP_ELFSYM_EXPORT(esp_timer_init),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_rawflash_mount),
    ESP_ELFSYM_EXPORT(esp_vfs_register_fs),
    ESP_ELFSYM_EXPORT(pm_connectionless_wake_window_timeout_process),
    ESP_ELFSYM_EXPORT(lv_screen_active),
    ESP_ELFSYM_EXPORT(mbedtls_threading_psa_rngdata_mutex),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_certificate),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt11__timepunctIwEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(hal_he_set_co_hosted_bss),
    ESP_ELFSYM_EXPORT(r_ble_vendor_hci_duplicate_exception_list_config),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_opaque_import_key),
    ESP_ELFSYM_EXPORT(i2s_controller_set_rx_pdm_config),
    ESP_ELFSYM_EXPORT(mbedtls_hmac_drbg_random_with_add),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_fetch_input),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_proc_init),
    ESP_ELFSYM_EXPORT(memp_NETBUF),
    ESP_ELFSYM_EXPORT(wctob),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_default_wifi_connecting),
    ESP_ELFSYM_EXPORT(_sin),
    ESP_ELFSYM_EXPORT(uart_controller_write_bytes),
    ESP_ELFSYM_EXPORT(http_header_generate_string),
    ESP_ELFSYM_EXPORT(ip4_route),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_set_enable),
    ESP_ELFSYM_EXPORT(he_recv_action_twt_teardown),
    ESP_ELFSYM_EXPORT(ble_log_env),
    ESP_ELFSYM_EXPORT(pm_sleep_for),
    ESP_ELFSYM_EXPORT(_ZGVNSt10moneypunctIcLb1EE2idE),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_info_offset_get),
    ESP_ELFSYM_EXPORT(test_rx_process_complete_retry),
    ESP_ELFSYM_EXPORT(ble_hs_hci_cmd_tx),
    ESP_ELFSYM_EXPORT(camera_get_width),
    ESP_ELFSYM_EXPORT(fgetpos),
    ESP_ELFSYM_EXPORT(ble_gap_unpair_oldest_peer),
    ESP_ELFSYM_EXPORT(rtc_clk_cpu_freq_set_xtal),
    ESP_ELFSYM_EXPORT(_ZZNKSt8__format15__formatter_strIcE6formatINS_10_Sink_iterIcEEEET_St17basic_string_viewIcSt11char_traitsIcEERSt20basic_format_contextIS5_cEENKUlvE_clEv),
    ESP_ELFSYM_EXPORT(phy_tsens_read_init),
    ESP_ELFSYM_EXPORT(r_ll_assert),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_broadcast_wakeup_enable),
    ESP_ELFSYM_EXPORT(spi_device_queue_trans),
    ESP_ELFSYM_EXPORT(ff_oem2uni),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_add_mpi),
    ESP_ELFSYM_EXPORT(i2s_controller_read),
    ESP_ELFSYM_EXPORT(lv_bin_decoder_info),
    ESP_ELFSYM_EXPORT(i2c_master_transmit),
    ESP_ELFSYM_EXPORT(igmp_lookfor_group),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx117collateIcE2idE),
    ESP_ELFSYM_EXPORT(esp_stdio_register),
    ESP_ELFSYM_EXPORT(BcnInterval),
    ESP_ELFSYM_EXPORT(esp_wifi_use_supp_pmk_cache),
    ESP_ELFSYM_EXPORT(pow),
    ESP_ELFSYM_EXPORT(ble_sm_alg_f5),
    ESP_ELFSYM_EXPORT(key_mgr_hal_write_assist_info),
    ESP_ELFSYM_EXPORT(lv_anim_path_linear),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand),
    ESP_ELFSYM_EXPORT(hid_report_map_gamepad_len),
    ESP_ELFSYM_EXPORT(g_rssi_threshold_failure),
    ESP_ELFSYM_EXPORT(g_i2s),
    ESP_ELFSYM_EXPORT(esp_wifi_set_sleep_wait_broadcast_data_time),
    ESP_ELFSYM_EXPORT(ble_mqueue_put),
    ESP_ELFSYM_EXPORT(httpd_sess_delete_invalid),
    ESP_ELFSYM_EXPORT(pwm_set_period),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_sdcard_unmount),
    ESP_ELFSYM_EXPORT(esp_elf_malloc),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_reset),
    ESP_ELFSYM_EXPORT(wifi_realloc),
    ESP_ELFSYM_EXPORT(trc_onAmpduOp),
    ESP_ELFSYM_EXPORT(pmksa_cache_auth_add),
    ESP_ELFSYM_EXPORT(usb_msc_device_start),
    ESP_ELFSYM_EXPORT(dbg_lmac_init),
    ESP_ELFSYM_EXPORT(npl_freertos_event_is_queued),
    ESP_ELFSYM_EXPORT(coex_schm_env),
    ESP_ELFSYM_EXPORT(pmksa_cache_auth_add_entry),
    ESP_ELFSYM_EXPORT(esp_mbedtls_mem_free),
    ESP_ELFSYM_EXPORT(ieee80211_get_ptk),
    ESP_ELFSYM_EXPORT(phy_freq_get_i2c_data),
    ESP_ELFSYM_EXPORT(lv_style_set_shadow_width),
    ESP_ELFSYM_EXPORT(r_ble_hw_get_cte_packet_rssi_from_buffer),
    ESP_ELFSYM_EXPORT(psa_to_pk_ecdsa_errors),
    ESP_ELFSYM_EXPORT(npl_freertos_eventq_remove),
    ESP_ELFSYM_EXPORT(_ZSteqIcSt11char_traitsIcESaIcEEbRKNSt7__cxx1112basic_stringIT_T0_T1_EEPKS5_),
    ESP_ELFSYM_EXPORT(r_ble_phy_module_deinit),
    ESP_ELFSYM_EXPORT(esp_sta_reset_rmac_process),
    ESP_ELFSYM_EXPORT(esp_mbedtls_mem_calloc),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_alloc_periodic_memory),
    ESP_ELFSYM_EXPORT(_Z17ble_publish_eventP6Device7BtEvent),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_pairwise_cipher_internal),
    ESP_ELFSYM_EXPORT(__popcountdi2),
    ESP_ELFSYM_EXPORT(ble_hs_flow_track_data_mbuf),
    ESP_ELFSYM_EXPORT(modem_clock_hal_deselect_all_ble_rtc_timer_lpclk_source),
    ESP_ELFSYM_EXPORT(gpio_hal_intr_enable_on_core),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_le_ltk_neg_reply),
    ESP_ELFSYM_EXPORT(lv_event_stop_processing),
    ESP_ELFSYM_EXPORT(device_remove),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_group_free),
    ESP_ELFSYM_EXPORT(lv_anim_set_values),
    ESP_ELFSYM_EXPORT(httpd_req_new),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_lsb),
    ESP_ELFSYM_EXPORT(esp_netif_netstack_buf_free),
    ESP_ELFSYM_EXPORT(mbedtls_psa_mac_compute),
    ESP_ELFSYM_EXPORT(uECC_vli_modInv),
    ESP_ELFSYM_EXPORT(r_ble_lll_per_adv_coex_dpc_calc_pti_update_itvl),
    ESP_ELFSYM_EXPORT(vSystimerSetup),
    ESP_ELFSYM_EXPORT(esp_wifi_get_protocol),
    ESP_ELFSYM_EXPORT(ieee802154_txon_delay_set),
    ESP_ELFSYM_EXPORT(ble_hs_adv_parse),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write),
    ESP_ELFSYM_EXPORT(esp_log_vprint_func),
    ESP_ELFSYM_EXPORT(strstr),
    ESP_ELFSYM_EXPORT(phy_get_i2c_hostid_),
    ESP_ELFSYM_EXPORT(g_wpa_supp),
    ESP_ELFSYM_EXPORT(uart_controller_read_bytes),
    ESP_ELFSYM_EXPORT(lv_obj_set_y),
    ESP_ELFSYM_EXPORT(phy_ant_update),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_calc_dci_csa1),
    ESP_ELFSYM_EXPORT(_Z15tusbIsSupportedv),
    ESP_ELFSYM_EXPORT(igmp_leavegroup),
    ESP_ELFSYM_EXPORT(fstat),
    ESP_ELFSYM_EXPORT(scan_get_owe_channel_info),
    ESP_ELFSYM_EXPORT(bt_bb_set_corr_thresh_le),
    ESP_ELFSYM_EXPORT(lv_group_add_obj),
    ESP_ELFSYM_EXPORT(lmacRequestTxopQueue),
    ESP_ELFSYM_EXPORT(app_event_emit),
    ESP_ELFSYM_EXPORT(he_get_min_subframe_len_dcm),
    ESP_ELFSYM_EXPORT(httpd_sess_delete),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_process_conn_params),
    ESP_ELFSYM_EXPORT(phy_bt_tx_pwctrl_init),
    ESP_ELFSYM_EXPORT(heap_caps_enable_nonos_stack_heaps),
    ESP_ELFSYM_EXPORT(ble_sm_alg_gen_dhkey),
    ESP_ELFSYM_EXPORT(lv_event_pop),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_txbuf_insert_after),
    ESP_ELFSYM_EXPORT(g_conn_stack_envP),
    ESP_ELFSYM_EXPORT(xPortStartScheduler),
    ESP_ELFSYM_EXPORT(module_construct),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_conn_cte_rx_set_param),
    ESP_ELFSYM_EXPORT(spicommon_dma_setup_priv_buffer),
    ESP_ELFSYM_EXPORT(g_mmu_mem_regions),
    ESP_ELFSYM_EXPORT(esp_intr_alloc),
    ESP_ELFSYM_EXPORT(_ZSt14__convert_to_vIeEvPKcRT_RSt12_Ios_IostateRKPi),
    ESP_ELFSYM_EXPORT(atan2),
    ESP_ELFSYM_EXPORT(mpi_hal_calc_hardware_words),
    ESP_ELFSYM_EXPORT(_ZSt14__convert_to_vIfEvPKcRT_RSt12_Ios_IostateRKPi),
    ESP_ELFSYM_EXPORT(lv_menu_separator_class),
    ESP_ELFSYM_EXPORT(psa_export_key_internal),
    ESP_ELFSYM_EXPORT(lv_anim_set_duration),
    ESP_ELFSYM_EXPORT(g_debug_le_private_key_ro),
    ESP_ELFSYM_EXPORT(lv_color16_premultiply),
    ESP_ELFSYM_EXPORT(cnx_sta_leave),
    ESP_ELFSYM_EXPORT(mbedtls_md),
    ESP_ELFSYM_EXPORT(r_ble_phy_module_init),
    ESP_ELFSYM_EXPORT(_ZSt9use_facetISt10moneypunctIwLb1EEERKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(include_esp_phy_override),
    ESP_ELFSYM_EXPORT(cnx_can_do_obss_scan),
    ESP_ELFSYM_EXPORT(coex_hw_timer_tick_get),
    ESP_ELFSYM_EXPORT(s_table),
    ESP_ELFSYM_EXPORT(r_ble_ll_get_our_devaddr),
    ESP_ELFSYM_EXPORT(wDev_ProcessFiq),
    ESP_ELFSYM_EXPORT(memspi_host_read_data_slicer),
    ESP_ELFSYM_EXPORT(conn_ll_getSuppMaxTxOctets),
    ESP_ELFSYM_EXPORT(esp_http_client_set_auth_data),
    ESP_ELFSYM_EXPORT(bt_bb_v2_version),
    ESP_ELFSYM_EXPORT(ieee80211_add_he_mu_edca_parameter_set),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_enable_global_ca_store),
    ESP_ELFSYM_EXPORT(_Z24app_metadata_is_valid_idRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(lv_theme_apply),
    ESP_ELFSYM_EXPORT(aes_hal_wait_done),
    ESP_ELFSYM_EXPORT(xRingbufferGetStaticBuffer),
    ESP_ELFSYM_EXPORT(lmacProcessTxRtsError),
    ESP_ELFSYM_EXPORT(ble_sm_ia_ra),
    ESP_ELFSYM_EXPORT(lv_draw_sw_fill),
    ESP_ELFSYM_EXPORT(esp_coex_common_spin_lock_create_wrapper),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_client_key_config),
    ESP_ELFSYM_EXPORT(i2s_channel_init_tdm_mode),
    ESP_ELFSYM_EXPORT(g_txop_queue_status),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_grid_column_dsc_array),
    ESP_ELFSYM_EXPORT(task_event_group_release_bit),
    ESP_ELFSYM_EXPORT(npl_freertos_eventq_get),
    ESP_ELFSYM_EXPORT(mbedtls_hmac_drbg_reseed),
    ESP_ELFSYM_EXPORT(lv_obj_refr_size),
    ESP_ELFSYM_EXPORT(uart_hal_set_tx_idle_num),
    ESP_ELFSYM_EXPORT(phy_i2c_paral_write),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_rpa_update),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_alloc_rxbuf),
    ESP_ELFSYM_EXPORT(pbuf_put_at),
    ESP_ELFSYM_EXPORT(coex_schm_register_callback),
    ESP_ELFSYM_EXPORT(esp_phy_modem_deinit),
    ESP_ELFSYM_EXPORT(esp_hmac_abort_transparent),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEED0Ev),
    ESP_ELFSYM_EXPORT(_ZTVSt10moneypunctIcLb1EE),
    ESP_ELFSYM_EXPORT(phy_rxiq_cover_mg_mp),
    ESP_ELFSYM_EXPORT(sta_recv_mgmt),
    ESP_ELFSYM_EXPORT(ble_gap_subscribe_event),
    ESP_ELFSYM_EXPORT(mspi_timing_flash_tuning),
    ESP_ELFSYM_EXPORT(http_header_set_format),
    ESP_ELFSYM_EXPORT(r_ble_lll_alternative_tx_buffer_set),
    ESP_ELFSYM_EXPORT(tcp_rst_netif),
    ESP_ELFSYM_EXPORT(lv_area_get_size),
    ESP_ELFSYM_EXPORT(lmacIsLongFrame),
    ESP_ELFSYM_EXPORT(hex2byte),
    ESP_ELFSYM_EXPORT(http_parser_execute),
    ESP_ELFSYM_EXPORT(__find_charset),
    ESP_ELFSYM_EXPORT(mbedtls_pk_set_pubkey_from_prv),
    ESP_ELFSYM_EXPORT(mbedtls_psa_cipher_encrypt),
    ESP_ELFSYM_EXPORT(lv_image_decoder_get_area),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_deleted_irk_list_clear),
    ESP_ELFSYM_EXPORT(scan_update_scan_history),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_default_wifi_conn),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_report_bsscolor_collision),
    ESP_ELFSYM_EXPORT(device_get_child_count),
    ESP_ELFSYM_EXPORT(lv_array_init),
    ESP_ELFSYM_EXPORT(wDev_ftm_record_t1t4),
    ESP_ELFSYM_EXPORT(lv_draw_task_get_border_dsc),
    ESP_ELFSYM_EXPORT(ieee80211_setup_basic_htrates),
    ESP_ELFSYM_EXPORT(ieee80211_add_htinfo_body),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_copy),
    ESP_ELFSYM_EXPORT(r_ble_phy_get_max_txpwr_dbm),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_default_wifi_conn),
    ESP_ELFSYM_EXPORT(rsn_set_snonce_cookie),
    ESP_ELFSYM_EXPORT(netif_ip6_addr_set_parts),
    ESP_ELFSYM_EXPORT(window_manager_get_state),
    ESP_ELFSYM_EXPORT(ble_single_clear_single_step),
    ESP_ELFSYM_EXPORT(wpa_supplicant_key_neg_complete),
    ESP_ELFSYM_EXPORT(g_esp_wifi_cmw500_stbc_rx_test_enabled),
    ESP_ELFSYM_EXPORT(esp_http_client_set_timeout_ms),
    ESP_ELFSYM_EXPORT(sdmmc_init_csd),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_reset_link_state),
    ESP_ELFSYM_EXPORT(phy_pbus_force_test),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_pad_top),
    ESP_ELFSYM_EXPORT(hci_stack_setVsEventMask),
    ESP_ELFSYM_EXPORT(esp_wifi_register_wpa_cb_internal),
    ESP_ELFSYM_EXPORT(__log2pow5),
    ESP_ELFSYM_EXPORT(phy_get_temp_init),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_handshake),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_init),
    ESP_ELFSYM_EXPORT(lv_checkbox_set_text),
    ESP_ELFSYM_EXPORT(lv_refr_set_disp_refreshing),
    ESP_ELFSYM_EXPORT(gdma_new_ahb_channel),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt10moneypunctIcLb1EEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(mbedtls_hmac_drbg_free),
    ESP_ELFSYM_EXPORT(r_mem_init_mbuf_pool),
    ESP_ELFSYM_EXPORT(mbedtls_md_hmac_reset),
    ESP_ELFSYM_EXPORT(esp_flash_app_init),
    ESP_ELFSYM_EXPORT(ble_hs_id_set_rnd),
    ESP_ELFSYM_EXPORT(esp_flash_get_size),
    ESP_ELFSYM_EXPORT(lv_cache_entry_acquire_data),
    ESP_ELFSYM_EXPORT(lv_obj_check_type),
    ESP_ELFSYM_EXPORT(tanhf),
    ESP_ELFSYM_EXPORT(wdev_data_init),
    ESP_ELFSYM_EXPORT(mbedtls_sha256_init),
    ESP_ELFSYM_EXPORT(NAN_NETWORK_ID),
    ESP_ELFSYM_EXPORT(sleep_system_peripheral_dummy),
    ESP_ELFSYM_EXPORT(mumimo_spatial_cfg_get_nsts_tot),
    ESP_ELFSYM_EXPORT(lv_draw_image_dsc_init),
    ESP_ELFSYM_EXPORT(phy_bb_txpwr_init),
    ESP_ELFSYM_EXPORT(EccPoint_mult),
    ESP_ELFSYM_EXPORT(ieee80211_add_htinfo_vendor),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_grp_id_list),
    ESP_ELFSYM_EXPORT(tc_aes128_set_encrypt_key),
    ESP_ELFSYM_EXPORT(lv_display_set_flush_cb),
    ESP_ELFSYM_EXPORT(adc2_wifi_acquire),
    ESP_ELFSYM_EXPORT(esp_netif_attach_wifi_station),
    ESP_ELFSYM_EXPORT(phy_tx_paon_set),
    ESP_ELFSYM_EXPORT(rtc_clk_fast_src_set),
    ESP_ELFSYM_EXPORT(esp32_wifi_pinned_driver),
    ESP_ELFSYM_EXPORT(lv_vsnprintf),
    ESP_ELFSYM_EXPORT(__ascii_mbtowc),
    ESP_ELFSYM_EXPORT(_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_i),
    ESP_ELFSYM_EXPORT(psa_wipe_key_slot),
    ESP_ELFSYM_EXPORT(uart_hal_get_symb_len),
    ESP_ELFSYM_EXPORT(io_mux_enable_lp_io_clock),
    ESP_ELFSYM_EXPORT(lvgl_ppa_rotate),
    ESP_ELFSYM_EXPORT(lv_draw_sw_deinit),
    ESP_ELFSYM_EXPORT(ble_capture_info_user_handler),
    ESP_ELFSYM_EXPORT(ble_gattc_rx_read_type_adata),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_vendor_unregister),
    ESP_ELFSYM_EXPORT(uart_controller_get_config),
    ESP_ELFSYM_EXPORT(i2s_output_gpio_reserve),
    ESP_ELFSYM_EXPORT(__rem_pio2),
    ESP_ELFSYM_EXPORT(spi_controller_try_lock),
    ESP_ELFSYM_EXPORT(r_os_mbuf_extend),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_create),
    ESP_ELFSYM_EXPORT(esp_intr_alloc_intrstatus),
    ESP_ELFSYM_EXPORT(lv_display_get_screen_prev),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_outline_color),
    ESP_ELFSYM_EXPORT(esp_hmac_compute_transparent),
    ESP_ELFSYM_EXPORT(mbedtls_psa_cipher_update),
    ESP_ELFSYM_EXPORT(pau_hal_set_regdma_entry_link_addr),
    ESP_ELFSYM_EXPORT(phy_nrx_freq_set),
    ESP_ELFSYM_EXPORT(g_wpa_config_changed),
    ESP_ELFSYM_EXPORT(usb_device_controller_is_cdc_enabled),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_rd_rem_ver),
    ESP_ELFSYM_EXPORT(mbedtls_asn1_write_len),
    ESP_ELFSYM_EXPORT(lv_anim_set_early_apply),
    ESP_ELFSYM_EXPORT(mbedtls_mutex_free_ptr),
    ESP_ELFSYM_EXPORT(crypto_ec_point_compute_y_sqr),
    ESP_ELFSYM_EXPORT(rtc_clk_apb_freq_get),
    ESP_ELFSYM_EXPORT(pwm_set_duty),
    ESP_ELFSYM_EXPORT(rcGetRate),
    ESP_ELFSYM_EXPORT(ble_sm_sc_random_exec),
    ESP_ELFSYM_EXPORT(nd6_find_route),
    ESP_ELFSYM_EXPORT(ieee80211_get_regdomain),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_hw_err),
    ESP_ELFSYM_EXPORT(mbedtls_zeroize_and_free),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_conf_ciphersuites),
    ESP_ELFSYM_EXPORT(esp_mpi_exp_mpi_mod_hw_op),
    ESP_ELFSYM_EXPORT(_ZStmiRKSt15_Deque_iteratorISt6vectorIhSaIhEERS2_PS2_ES7_),
    ESP_ELFSYM_EXPORT(i2c_controller_write),
    ESP_ELFSYM_EXPORT(lodepng_palette_clear),
    ESP_ELFSYM_EXPORT(nan_get_config_local),
    ESP_ELFSYM_EXPORT(service_manager_find_manifest),
    ESP_ELFSYM_EXPORT(httpd_sess_enum),
    ESP_ELFSYM_EXPORT(esp_hmac_calculate),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_update_features),
    ESP_ELFSYM_EXPORT(lv_line_class),
    ESP_ELFSYM_EXPORT(_ZGVNSt8time_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(esp_partition_is_flash_region_writable),
    ESP_ELFSYM_EXPORT(adv_stack_getBroker),
    ESP_ELFSYM_EXPORT(wifi_station_connect),
    ESP_ELFSYM_EXPORT(usb_hid_device_get),
    ESP_ELFSYM_EXPORT(coex_schm_status_bit_clear),
    ESP_ELFSYM_EXPORT(g_mac_sleep_en),
    ESP_ELFSYM_EXPORT(ip4_output_if_opt_src),
    ESP_ELFSYM_EXPORT(ieee80211_output_raw_process),
    ESP_ELFSYM_EXPORT(r_ble_ll_arr_common_factor_get),
    ESP_ELFSYM_EXPORT(__eqtf2),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_chk_phy_upd_start),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_reset_checksum),
    ESP_ELFSYM_EXPORT(phy_txcal_work_mode),
    ESP_ELFSYM_EXPORT(mbedtls_pk_free),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_init),
    ESP_ELFSYM_EXPORT(memory_realloc_with_policy),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSiD0Ev),
    ESP_ELFSYM_EXPORT(tlsf_size),
    ESP_ELFSYM_EXPORT(httpd_stop),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_on_coex_schm_phase),
    ESP_ELFSYM_EXPORT(_ZSt13get_terminatev),
    ESP_ELFSYM_EXPORT(esp_test_tx_count_collision),
    ESP_ELFSYM_EXPORT(pm_clear_wakeup_signal),
    ESP_ELFSYM_EXPORT(coex_schm_status_bitmap_get),
    ESP_ELFSYM_EXPORT(lv_spangroup_get_expand_height),
    ESP_ELFSYM_EXPORT(wifi_nvs_commit),
    ESP_ELFSYM_EXPORT(ble_l2cap_chan_alloc),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_remove),
    ESP_ELFSYM_EXPORT(global_hapd),
    ESP_ELFSYM_EXPORT(bt_bb_tx_cca_fifo_reset),
    ESP_ELFSYM_EXPORT(nd6_restart_netif),
    ESP_ELFSYM_EXPORT(ble_gatts_conn_can_alloc),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_opaque_size_function),
    ESP_ELFSYM_EXPORT(pvt_auto_dbias_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_conn_iq_report),
    ESP_ELFSYM_EXPORT(ieee80211_vht_get_vhtflags),
    ESP_ELFSYM_EXPORT(gdma_hal_set_weight),
    ESP_ELFSYM_EXPORT(xTaskGetTickCountFromISR),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_line_color),
    ESP_ELFSYM_EXPORT(lv_display_dpx),
    ESP_ELFSYM_EXPORT(ieee80211_parse_spatial_reuse),
    ESP_ELFSYM_EXPORT(esp_vfs_include_console_register),
    ESP_ELFSYM_EXPORT(ieee80211_output_pending_eb),
    ESP_ELFSYM_EXPORT(r__os_mbuf_copypkthdr),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_safe_cond_assign),
    ESP_ELFSYM_EXPORT(wifi_station_disconnect),
    ESP_ELFSYM_EXPORT(vPortYield),
    ESP_ELFSYM_EXPORT(xEventGroupSetBits),
    ESP_ELFSYM_EXPORT(r_os_mbuf_prepend_pullup),
    ESP_ELFSYM_EXPORT(lv_obj_get_child),
    ESP_ELFSYM_EXPORT(lv_text_encoded_next),
    ESP_ELFSYM_EXPORT(lv_obj_scroll_by),
    ESP_ELFSYM_EXPORT(wpa_get_key),
    ESP_ELFSYM_EXPORT(esp_phy_enable),
    ESP_ELFSYM_EXPORT(pm_twt_disallow_tx),
    ESP_ELFSYM_EXPORT(gpio_controller_get_pin_count),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx117collateIwE2idE),
    ESP_ELFSYM_EXPORT(sae_group_allowed),
    ESP_ELFSYM_EXPORT(ieee80211_parse_beacon),
    ESP_ELFSYM_EXPORT(chm_end_op),
    ESP_ELFSYM_EXPORT(ip6_select_source_address),
    ESP_ELFSYM_EXPORT(lmacReachShortLimit),
    ESP_ELFSYM_EXPORT(esp_crypto_ecc_lock_release),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_conf_own_cert),
    ESP_ELFSYM_EXPORT(esp_crypto_mpi_lock_acquire),
    ESP_ELFSYM_EXPORT(bt_agc_rssi_thresh),
    ESP_ELFSYM_EXPORT(esp_test_disable_tx_statistics),
    ESP_ELFSYM_EXPORT(app_scan_params),
    ESP_ELFSYM_EXPORT(he_recv_action_twt_setup),
    ESP_ELFSYM_EXPORT(r_os_mbuf_pool_init),
    ESP_ELFSYM_EXPORT(psa_copy_key_material_into_slot),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_recycle_buffer),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_prof_ap_ssid_internal),
    ESP_ELFSYM_EXPORT(lv_color_eq),
    ESP_ELFSYM_EXPORT(_ZThn8_NSdD0Ev),
    ESP_ELFSYM_EXPORT(sar_periph_ctrl_adc_oneshot_power_release),
    ESP_ELFSYM_EXPORT(ieee80211_sta_is_connected),
    ESP_ELFSYM_EXPORT(ieee80211_freedom_deinit),
    ESP_ELFSYM_EXPORT(r_os_mbuf_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_power_change_ind),
    ESP_ELFSYM_EXPORT(wmemcpy),
    ESP_ELFSYM_EXPORT(wlanif_init),
    ESP_ELFSYM_EXPORT(wifi_set_config_process),
    ESP_ELFSYM_EXPORT(esp_ecdsa_transparent_verify_hash_complete),
    ESP_ELFSYM_EXPORT(preferences_has_int64),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_coex_dpc_process),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_ll_acl_tx),
    ESP_ELFSYM_EXPORT(wpa_set_profile),
    ESP_ELFSYM_EXPORT(lv_strlcpy),
    ESP_ELFSYM_EXPORT(ftm_resp_ctx),
    ESP_ELFSYM_EXPORT(wpa_sm_notify_disassoc),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_transfer_get),
    ESP_ELFSYM_EXPORT(phy_chip_i2c_writeReg),
    ESP_ELFSYM_EXPORT(cnx_sta_scan_cmd),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_addr_table_is_empty),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_border_width),
    ESP_ELFSYM_EXPORT(power_supply_get_property),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_margin_bottom),
    ESP_ELFSYM_EXPORT(conn_stack_disable),
    ESP_ELFSYM_EXPORT(_ZSt8to_charsPcS_dSt12chars_formati),
    ESP_ELFSYM_EXPORT(wifi_nvs_validate_lr_info),
    ESP_ELFSYM_EXPORT(esp_http_client_get_password),
    ESP_ELFSYM_EXPORT(ble_svc_gap_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_aux_pdu_make),
    ESP_ELFSYM_EXPORT(sta_set_rmac_restart),
    ESP_ELFSYM_EXPORT(coex_wifi_channel_set),
    ESP_ELFSYM_EXPORT(esp_transport_write),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_set_scan_link_state),
    ESP_ELFSYM_EXPORT(wifi_sta_get_prof_password),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_reg_rxcb),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_starts),
    ESP_ELFSYM_EXPORT(PWM_TYPE),
    ESP_ELFSYM_EXPORT(spi_flash_hal_gpspi_configure_host_io_mode),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_flex_track_place),
    ESP_ELFSYM_EXPORT(psa_key_derivation_input_bytes),
    ESP_ELFSYM_EXPORT(esp_http_client_set_password),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_whitelist_enabled),
    ESP_ELFSYM_EXPORT(hal_disable_tsf_timer_wakeup),
    ESP_ELFSYM_EXPORT(ble_hid_set_conn_handle),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_deinit),
    ESP_ELFSYM_EXPORT(_lock_try_acquire),
    ESP_ELFSYM_EXPORT(wifi_is_stop_in_progress),
    ESP_ELFSYM_EXPORT(i2c_driver_delete),
    ESP_ELFSYM_EXPORT(ap_get_sta),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_end_adv_evt),
    ESP_ELFSYM_EXPORT(omac1_aes_vector),
    ESP_ELFSYM_EXPORT(__wtime_wday),
    ESP_ELFSYM_EXPORT(ppMapTxQueue),
    ESP_ELFSYM_EXPORT(usb_hid_device_set_name),
    ESP_ELFSYM_EXPORT(ble_sm_alg_aes_cmac),
    ESP_ELFSYM_EXPORT(phy_get_chan_cap),
    ESP_ELFSYM_EXPORT(hostap_recv_mgmt),
    ESP_ELFSYM_EXPORT(uart_driver_delete),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_reset),
    ESP_ELFSYM_EXPORT(tcpip_callback),
    ESP_ELFSYM_EXPORT(pm_set_bss_max_idle),
    ESP_ELFSYM_EXPORT(uECC_vli_testBit),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_opaque_set_session_timeout),
    ESP_ELFSYM_EXPORT(phy_set_rx_sense),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_tbtt_auto_period_interval),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_feature_rsp),
    ESP_ELFSYM_EXPORT(esp_http_client_get_status_code),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_clear_all_adi_list_bit),
    ESP_ELFSYM_EXPORT(ledc_timer_rst),
    ESP_ELFSYM_EXPORT(lv_init),
    ESP_ELFSYM_EXPORT(phy_ant_wifitx_cfg),
    ESP_ELFSYM_EXPORT(gpio_hal_iomux_in),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_exp_mod_working_limbs),
    ESP_ELFSYM_EXPORT(lv_arc_set_change_rate),
    ESP_ELFSYM_EXPORT(spi_bus_lock_unregister_dev),
    ESP_ELFSYM_EXPORT(r_ble_hw_get_cte_packet_freq_from_buffer),
    ESP_ELFSYM_EXPORT(esp_io_expander_gpio_wrapper_get_level),
    ESP_ELFSYM_EXPORT(wifi_sta_change_mac_internal),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_BLOCK_KEY2),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_signed_write),
    ESP_ELFSYM_EXPORT(ds_hal_write_private_key_params),
    ESP_ELFSYM_EXPORT(regdma_link_get_owner_bitmap),
    ESP_ELFSYM_EXPORT(r_ble_lll_alternative_tx_buffer_get),
    ESP_ELFSYM_EXPORT(ble_gattc_connection_broken),
    ESP_ELFSYM_EXPORT(gpio_pullup_dis),
    ESP_ELFSYM_EXPORT(wpa_use_aes_key_wrap),
    ESP_ELFSYM_EXPORT(spi_flash_hal_resume),
    ESP_ELFSYM_EXPORT(ap_try_sa_query_process),
    ESP_ELFSYM_EXPORT(ble_store_read_cccd),
    ESP_ELFSYM_EXPORT(ble_transport_ll_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_process_tx_power_releated),
    ESP_ELFSYM_EXPORT(dhcp_stop),
    ESP_ELFSYM_EXPORT(esp_netif_set_mac_api),
    ESP_ELFSYM_EXPORT(nimble_mem_calloc),
    ESP_ELFSYM_EXPORT(f_open),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_env_deinit),
    ESP_ELFSYM_EXPORT(pm_set_sleep_wait_broadcast_data_time),
    ESP_ELFSYM_EXPORT(nan_send_disc_bcn_process),
    ESP_ELFSYM_EXPORT(nd6_get_next_hop_addr_or_queue),
    ESP_ELFSYM_EXPORT(lv_spangroup_get_expand_width),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_pcl_rssi_threshold),
    ESP_ELFSYM_EXPORT(_i2c_hal_set_bus_timing),
    ESP_ELFSYM_EXPORT(s_itwt_resume_flow_id_bitmap),
    ESP_ELFSYM_EXPORT(esp_netif_get_old_ip_info),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_set_blocklen),
    ESP_ELFSYM_EXPORT(pmu_hal_hp_set_modem_active_backup_enable),
    ESP_ELFSYM_EXPORT(cnx_csa_fn_process),
    ESP_ELFSYM_EXPORT(lv_free_core),
    ESP_ELFSYM_EXPORT(ant_tx_cfg),
    ESP_ELFSYM_EXPORT(s_head),
    ESP_ELFSYM_EXPORT(esp_aes_free),
    ESP_ELFSYM_EXPORT(console_fcntl),
    ESP_ELFSYM_EXPORT(lv_event_get_draw_task),
    ESP_ELFSYM_EXPORT(lv_color24_luminance),
    ESP_ELFSYM_EXPORT(device_get_driver_data),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_list_remove),
    ESP_ELFSYM_EXPORT(_ZSt14__add_groupingIwEPT_S1_S0_PKcjPKS0_S5_),
    ESP_ELFSYM_EXPORT(esp_wifi_build_rsnxe),
    ESP_ELFSYM_EXPORT(ble_sm_alg_c1),
    ESP_ELFSYM_EXPORT(mbedtls_ccm_update),
    ESP_ELFSYM_EXPORT(vfscanf),
    ESP_ELFSYM_EXPORT(pmu_hal_hp_set_sleep_active_backup_enable),
    ESP_ELFSYM_EXPORT(esp_intr_alloc_intrstatus_bind),
    ESP_ELFSYM_EXPORT(audio_stream_is_supported),
    ESP_ELFSYM_EXPORT(key_mgr_hal_set_key_generator_mode),
    ESP_ELFSYM_EXPORT(gpio_hog_driver),
    ESP_ELFSYM_EXPORT(cnx_node_remove),
    ESP_ELFSYM_EXPORT(esp_mpi_mult_mpi_failover_mod_mult_hw_op),
    ESP_ELFSYM_EXPORT(ieee80211_rsn_cipher_priority),
    ESP_ELFSYM_EXPORT(phy_band_reg),
    ESP_ELFSYM_EXPORT(empty_func_p_void_r_bool),
    ESP_ELFSYM_EXPORT(ble_hs_hci_get_hci_supported_cmd),
    ESP_ELFSYM_EXPORT(lv_style_set_pad_bottom),
    ESP_ELFSYM_EXPORT(ble_gap_init),
    ESP_ELFSYM_EXPORT(i2s_channel_preload_data),
    ESP_ELFSYM_EXPORT(rutype2str),
    ESP_ELFSYM_EXPORT(cnx_clear_blacklist),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_sch_chain),
    ESP_ELFSYM_EXPORT(mac_list_lock),
    ESP_ELFSYM_EXPORT(crypt_decrypt),
    ESP_ELFSYM_EXPORT(display_swap_xy),
    ESP_ELFSYM_EXPORT(ble_att_svr_register),
    ESP_ELFSYM_EXPORT(ieee80211_post_hmac_tx),
    ESP_ELFSYM_EXPORT(sntp_set_time_sync_notification_cb),
    ESP_ELFSYM_EXPORT(ble_gatts_add_svcs),
    ESP_ELFSYM_EXPORT(_Z25ble_start_advertising_hidP6Devicet),
    ESP_ELFSYM_EXPORT(ungetc),
    ESP_ELFSYM_EXPORT(esp_netif_get_event_id),
    ESP_ELFSYM_EXPORT(cte_sample_select_index),
    ESP_ELFSYM_EXPORT(hal_he_disable_rts_threshold),
    ESP_ELFSYM_EXPORT(netconn_join_leave_group_netif),
    ESP_ELFSYM_EXPORT(regdma_link_new_branch_continuous_default),
    ESP_ELFSYM_EXPORT(tlsf_add_pool),
    ESP_ELFSYM_EXPORT(spi_flash_chip_gd_probe),
    ESP_ELFSYM_EXPORT(temperature_sensor_power_release),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_conn),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_list_clr),
    ESP_ELFSYM_EXPORT(ftell),
    ESP_ELFSYM_EXPORT(coex_schm_curr_period_get),
    ESP_ELFSYM_EXPORT(spacing2str),
    ESP_ELFSYM_EXPORT(aes_encrypt_deinit),
    ESP_ELFSYM_EXPORT(app_event_poll),
    ESP_ELFSYM_EXPORT(is_use_muedca),
    ESP_ELFSYM_EXPORT(app_main),
    ESP_ELFSYM_EXPORT(http_header_init),
    ESP_ELFSYM_EXPORT(hal_set_beamf_mt_pti),
    ESP_ELFSYM_EXPORT(lvgl_get_shared_icon_font),
    ESP_ELFSYM_EXPORT(phy_get_xtal_freq),
    ESP_ELFSYM_EXPORT(acd_network_changed_link_down),
    ESP_ELFSYM_EXPORT(ble_hs_init),
    ESP_ELFSYM_EXPORT(ble_hs_hci_util_data_hdr_strip),
    ESP_ELFSYM_EXPORT(multi_heap_malloc_impl),
    ESP_ELFSYM_EXPORT(ieee80211_is_40mhz_valid_bw),
    ESP_ELFSYM_EXPORT(lmacPostTxComplete),
    ESP_ELFSYM_EXPORT(esp_vfs_fcntl_r),
    ESP_ELFSYM_EXPORT(_ZTVNSt8__format14_Fixedbuf_sinkIcEE),
    ESP_ELFSYM_EXPORT(read_hw_noisefloor),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_ciphersuite_sig_pk_alg),
    ESP_ELFSYM_EXPORT(lv_label_set_text_fmt),
    ESP_ELFSYM_EXPORT(_ZTVSt23_Sp_counted_ptr_inplaceIN2tt3app5files5StateESaIvELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(aes_encrypt),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_req_peer_sca),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_conn_wifi_connecting),
    ESP_ELFSYM_EXPORT(adv_stack_enable),
    ESP_ELFSYM_EXPORT(esp_register_ext_funcs),
    ESP_ELFSYM_EXPORT(wifi_ap_sta_sae_auth_done_process),
    ESP_ELFSYM_EXPORT(mbedtls_pk_parse_public_key),
    ESP_ELFSYM_EXPORT(gdma_disconnect),
    ESP_ELFSYM_EXPORT(coex_bt_request),
    ESP_ELFSYM_EXPORT(lv_menu_sidebar_cont_class),
    ESP_ELFSYM_EXPORT(r_ble_hw_resolv_list_search),
    ESP_ELFSYM_EXPORT(resolve_symbol),
    ESP_ELFSYM_EXPORT(lv_indev_set_type),
    ESP_ELFSYM_EXPORT(ble_att_svr_deinit),
    ESP_ELFSYM_EXPORT(phy_rfcal_data_sub),
    ESP_ELFSYM_EXPORT(wifi_get_radio_state),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_sig_alg_ext),
    ESP_ELFSYM_EXPORT(mbedtls_net_recv),
    ESP_ELFSYM_EXPORT(esp_mbedtls_handshake),
    ESP_ELFSYM_EXPORT(lv_utils_bsearch),
    ESP_ELFSYM_EXPORT(lv_arc_set_value),
    ESP_ELFSYM_EXPORT(rtc_time_us_to_slowclk),
    ESP_ELFSYM_EXPORT(chm_get_current_channel),
    ESP_ELFSYM_EXPORT(esp_log_impl_lock),
    ESP_ELFSYM_EXPORT(scan_start_handler),
    ESP_ELFSYM_EXPORT(esp_sha_dma_start),
    ESP_ELFSYM_EXPORT(ieee80211_setup_htrates),
    ESP_ELFSYM_EXPORT(multi_heap_get_allocated_size_impl),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_switch_delay_rx_aod_1us_2m),
    ESP_ELFSYM_EXPORT(chip_enable),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_if_cond),
    ESP_ELFSYM_EXPORT(file_system_get_path),
    ESP_ELFSYM_EXPORT(gpio_sleep_sel_en),
    ESP_ELFSYM_EXPORT(r_ble_lll_sync_event_end),
    ESP_ELFSYM_EXPORT(atanf),
    ESP_ELFSYM_EXPORT(atoi),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_public),
    ESP_ELFSYM_EXPORT(sha_hal_sha512_init_hash),
    ESP_ELFSYM_EXPORT(get_fattime),
    ESP_ELFSYM_EXPORT(ieee80211_sta_parse_he_color_change_announcent),
    ESP_ELFSYM_EXPORT(esp_tls_get_and_clear_last_error),
    ESP_ELFSYM_EXPORT(mbedtls_x509_crt_check_key_usage),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_parse_alpn_ext),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_update_did),
    ESP_ELFSYM_EXPORT(ble_hw_env_p),
    ESP_ELFSYM_EXPORT(__file_str_put_alloc),
    ESP_ELFSYM_EXPORT(lv_image_decoder_init),
    ESP_ELFSYM_EXPORT(ieee80211_ampdu_age_all),
    ESP_ELFSYM_EXPORT(lv_area_set_height),
    ESP_ELFSYM_EXPORT(psa_to_pk_rsa_errors),
    ESP_ELFSYM_EXPORT(ble_hs_id_copy_addr),
    ESP_ELFSYM_EXPORT(g_wdev_csi_rx_ctx),
    ESP_ELFSYM_EXPORT(dns_mquery_v4group),
    ESP_ELFSYM_EXPORT(strpbrk),
    ESP_ELFSYM_EXPORT(g_authmode_incompatible),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSt14basic_ofstreamIcSt11char_traitsIcEED1Ev),
    ESP_ELFSYM_EXPORT(__rem_pio2f),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_enc_req_make),
    ESP_ELFSYM_EXPORT(service_manager_stop),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_inv_mod_odd),
    ESP_ELFSYM_EXPORT(tcp_rst),
    ESP_ELFSYM_EXPORT(chm_get_chan_info),
    ESP_ELFSYM_EXPORT(device_get_type),
    ESP_ELFSYM_EXPORT(readdir),
    ESP_ELFSYM_EXPORT(_ZZNSt8__format19_Formatting_scannerINS_10_Sink_iterIcEEcE13_M_format_argEjENKUlRT_E_clIcEEDaS5_),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_resume_from_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_decrypt_cipher_data),
    ESP_ELFSYM_EXPORT(r_ble_hw_driver_deinit),
    ESP_ELFSYM_EXPORT(xQueueGenericSendFromISR),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_invalid_hci_memory_free),
    ESP_ELFSYM_EXPORT(esp_ble_hw_get_static_addr),
    ESP_ELFSYM_EXPORT(esp_http_client_is_chunked_response),
    ESP_ELFSYM_EXPORT(esp_timer_start_once),
    ESP_ELFSYM_EXPORT(dhcps_set_new_lease_cb),
    ESP_ELFSYM_EXPORT(lv_dropdown_clear_options),
    ESP_ELFSYM_EXPORT(ieee80211_phy_deinit),
    ESP_ELFSYM_EXPORT(_ZGVNSt10moneypunctIwLb1EE2idE),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_proc_stop),
    ESP_ELFSYM_EXPORT(esp_wifi_get_config_sae_pwe_h2e_internal),
    ESP_ELFSYM_EXPORT(lv_menu_main_cont_class),
    ESP_ELFSYM_EXPORT(hal_init_tb_power),
    ESP_ELFSYM_EXPORT(set_gauss_coeff_2m),
    ESP_ELFSYM_EXPORT(ble_att_svr_init),
    ESP_ELFSYM_EXPORT(adc_hal_set_controller),
    ESP_ELFSYM_EXPORT(esp_btbb_disable),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_get_len),
    ESP_ELFSYM_EXPORT(pthread_symbols),
    ESP_ELFSYM_EXPORT(npl_freertos_eventq_init),
    ESP_ELFSYM_EXPORT(mac_tx_set_txop_q),
    ESP_ELFSYM_EXPORT(lv_color_to_32),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_sdspi_sdcard_init),
    ESP_ELFSYM_EXPORT(TRC_PER_IS_GOOD),
    ESP_ELFSYM_EXPORT(lv_label_get_long_mode),
    ESP_ELFSYM_EXPORT(psa_purge_key),
    ESP_ELFSYM_EXPORT(esp_http_client_get_url),
    ESP_ELFSYM_EXPORT(phy_bb_fsm_rst),
    ESP_ELFSYM_EXPORT(itwt_probe_timeout_fn),
    ESP_ELFSYM_EXPORT(hal_enable_sta_tsf),
    ESP_ELFSYM_EXPORT(lv_slider_set_range),
    ESP_ELFSYM_EXPORT(r_ble_lll_df_cte_set_pattern_config),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_rx_pkt_state),
    ESP_ELFSYM_EXPORT(lv_image_decoder_set_get_area_cb),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend),
    ESP_ELFSYM_EXPORT(lv_indev_get_state),
    ESP_ELFSYM_EXPORT(rtc_clk_cpu_set_to_default_config),
    ESP_ELFSYM_EXPORT(bt_bb_rx_dpo_set),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_ring_buffer_index_clear),
    ESP_ELFSYM_EXPORT(r_ble_ll_utils_calc_dci_csa2),
    ESP_ELFSYM_EXPORT(pm_wake_up),
    ESP_ELFSYM_EXPORT(phy_freq_i2c_mem_write),
    ESP_ELFSYM_EXPORT(lv_group_delete),
    ESP_ELFSYM_EXPORT(ble_store_write),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_rx_conn_param_rsp),
    ESP_ELFSYM_EXPORT(_lock_release_recursive),
    ESP_ELFSYM_EXPORT(rom_flash_chip_dummy),
    ESP_ELFSYM_EXPORT(ff_diskio_get_sector_size),
    ESP_ELFSYM_EXPORT(err_to_errno),
    ESP_ELFSYM_EXPORT(ledc_hal_set_duty_start),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_start_handshake_msg),
    ESP_ELFSYM_EXPORT(wifi_rf_phy_disable),
    ESP_ELFSYM_EXPORT(lv_arc_get_bg_angle_start),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_cth_flow_process_cmd),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_write_no_rsp),
    ESP_ELFSYM_EXPORT(_Z26ble_midi_init_gatt_handlesP6Device),
    ESP_ELFSYM_EXPORT(lv_indev_delete),
    ESP_ELFSYM_EXPORT(xTaskRemoveFromEventList),
    ESP_ELFSYM_EXPORT(coex_schm_bt_a2dp_paused_wifi_connecting),
    ESP_ELFSYM_EXPORT(npl_freertos_mempool_init),
    ESP_ELFSYM_EXPORT(temperature_sensor_hal_init),
    ESP_ELFSYM_EXPORT(i2c_controller_write_register_array),
    ESP_ELFSYM_EXPORT(lv_image_cache_init),
    ESP_ELFSYM_EXPORT(net80211_funcs_init),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_setup),
    ESP_ELFSYM_EXPORT(modem_clock_domain_icg_config),
    ESP_ELFSYM_EXPORT(usb_midi_device_stop),
    ESP_ELFSYM_EXPORT(phy_pwdet_sar2_init),
    ESP_ELFSYM_EXPORT(wifi_destroy_sta),
    ESP_ELFSYM_EXPORT(esp_elf_deinit),
    ESP_ELFSYM_EXPORT(lv_display_get_layer_bottom),
    ESP_ELFSYM_EXPORT(esp32_ble_serial_driver),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_version_ind_make),
    ESP_ELFSYM_EXPORT(phy_rxevm_init_cfg),
    ESP_ELFSYM_EXPORT(hostap_handle_timer_process),
    ESP_ELFSYM_EXPORT(power_supply_supports_quick_charge),
    ESP_ELFSYM_EXPORT(ip6_output_if),
    ESP_ELFSYM_EXPORT(ble_att_truncate_to_mtu),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free_sch),
    ESP_ELFSYM_EXPORT(ble_hs_hci_acl_tx_now),
    ESP_ELFSYM_EXPORT(uECC_get_rng),
    ESP_ELFSYM_EXPORT(esp_panic_handler_increment_entry_count),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_last_txbuf_memory_size_calculate),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_deinit),
    ESP_ELFSYM_EXPORT(conn_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(uxTaskPriorityGetFromISR),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_deinit),
    ESP_ELFSYM_EXPORT(ic_get_he_rts_threshold_bytes),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_master_init),
    ESP_ELFSYM_EXPORT(esp_http_client_get_post_field),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_sched_large_aux_offset),
    ESP_ELFSYM_EXPORT(ble_att_inc_tx_stat),
    ESP_ELFSYM_EXPORT(esp_md5_hash_update),
    ESP_ELFSYM_EXPORT(rtcio_hal_iomux_input),
    ESP_ELFSYM_EXPORT(_ZTSNSt8ios_base7failureB5cxx11E),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_update_sched_offset),
    ESP_ELFSYM_EXPORT(lv_layout_deinit),
    ESP_ELFSYM_EXPORT(esp32_spi_deselect_all_cs),
    ESP_ELFSYM_EXPORT(wifi_ap_remove_sta_node_process),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_calc_first_anchor_point),
    ESP_ELFSYM_EXPORT(ieee80211_add_dsparams),
    ESP_ELFSYM_EXPORT(lv_obj_add_flag),
    ESP_ELFSYM_EXPORT(lv_obj_get_local_style_prop),
    ESP_ELFSYM_EXPORT(gdma_config_transfer),
    ESP_ELFSYM_EXPORT(lv_roller_set_options),
    ESP_ELFSYM_EXPORT(sdmmc_get_status),
    ESP_ELFSYM_EXPORT(device_get_parent),
    ESP_ELFSYM_EXPORT(esp32_i2c_driver),
    ESP_ELFSYM_EXPORT(spi_flash_enable_interrupts_caches_and_other_cpu),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_read_key),
    ESP_ELFSYM_EXPORT(wpa_cb),
    ESP_ELFSYM_EXPORT(wpa_cipher_to_suite),
    ESP_ELFSYM_EXPORT(memp_IGMP_GROUP),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_put_syncinfo),
    ESP_ELFSYM_EXPORT(r_ble_phy_stats_reset),
    ESP_ELFSYM_EXPORT(phy_init_clk_set),
    ESP_ELFSYM_EXPORT(ip_input),
    ESP_ELFSYM_EXPORT(ble_gap_master_connect_reattempt),
    ESP_ELFSYM_EXPORT(frexpl),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx118time_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE2idE),
    ESP_ELFSYM_EXPORT(wpa_deattach),
    ESP_ELFSYM_EXPORT(hal_mac_workaround_vht_rx_no_ack),
    ESP_ELFSYM_EXPORT(pbuf_clen),
    ESP_ELFSYM_EXPORT(ieee80211_crypto_attach),
    ESP_ELFSYM_EXPORT(ip6addr_aton),
    ESP_ELFSYM_EXPORT(hal_enable_sta_btwt_tsf),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_rd_chan_map),
    ESP_ELFSYM_EXPORT(lv_draw_buf_width_to_stride_ex),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_conn_param_pdu_proc),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_time),
    ESP_ELFSYM_EXPORT(crypto_bignum_div),
    ESP_ELFSYM_EXPORT(pm_extend_tbtt_adaptive_start),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_env_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_pop_executed_sch),
    ESP_ELFSYM_EXPORT(soc_get_available_memory_regions),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_sig_from_pk),
    ESP_ELFSYM_EXPORT(sys_sem_signal),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_send_adv_report),
    ESP_ELFSYM_EXPORT(strtoll),
    ESP_ELFSYM_EXPORT(sae_check_confirm),
    ESP_ELFSYM_EXPORT(r_ble_hw_cte_get_sample_ring_buffer_ptr),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_shadow_width),
    ESP_ELFSYM_EXPORT(phy_vco_init_cal),
    ESP_ELFSYM_EXPORT(audio_codec_get_native_sample_rate),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_chk_conn_params),
    ESP_ELFSYM_EXPORT(lv_palette_main),
    ESP_ELFSYM_EXPORT(esp_hmac_abort_opaque),
    ESP_ELFSYM_EXPORT(lv_draw_sw_image),
    ESP_ELFSYM_EXPORT(tkip_encap),
    ESP_ELFSYM_EXPORT(f_sync),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_get_map),
    ESP_ELFSYM_EXPORT(psa_reserve_free_key_slot),
    ESP_ELFSYM_EXPORT(r_ble_lll_sleep_reset),
    ESP_ELFSYM_EXPORT(spi_hal_setup_trans),
    ESP_ELFSYM_EXPORT(lv_textarea_clear_selection),
    ESP_ELFSYM_EXPORT(ieee80211_psq_find_max_bss),
    ESP_ELFSYM_EXPORT(getc),
    ESP_ELFSYM_EXPORT(ble_store_config_vars),
    ESP_ELFSYM_EXPORT(r__os_msys_find_pool),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sched_priority_adjust),
    ESP_ELFSYM_EXPORT(npl_freertos_hw_exit_critical),
    ESP_ELFSYM_EXPORT(device_lock),
    ESP_ELFSYM_EXPORT(sta_is_bss_max_idle_enabled),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_sm_stop_timeout),
    ESP_ELFSYM_EXPORT(ieee80211_freedom_inside_cb),
    ESP_ELFSYM_EXPORT(lv_obj_get_y),
    ESP_ELFSYM_EXPORT(pbuf_take_at),
    ESP_ELFSYM_EXPORT(xEventGroupSync),
    ESP_ELFSYM_EXPORT(os_reltime_expired),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_alpn_protocol),
    ESP_ELFSYM_EXPORT(ppTxFragmentProc),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_vendor_hci_legacy_adv_clear),
    ESP_ELFSYM_EXPORT(esp_libc_include_getentropy_impl),
    ESP_ELFSYM_EXPORT(phy_i2c_correct),
    ESP_ELFSYM_EXPORT(lv_obj_set_flex_flow),
    ESP_ELFSYM_EXPORT(esp_event_handler_instance_register_with),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_clear_all_pl_bit),
    ESP_ELFSYM_EXPORT(phy_set_most_tpw_new),
    ESP_ELFSYM_EXPORT(driver_remove),
    ESP_ELFSYM_EXPORT(r_os_mbuf_append),
    ESP_ELFSYM_EXPORT(__negsf2),
    ESP_ELFSYM_EXPORT(spicommon_dma_desc_setup_link),
    ESP_ELFSYM_EXPORT(ble_att_clt_rx_read),
    ESP_ELFSYM_EXPORT(nvs_get_u32),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ecp_export_public_key),
    ESP_ELFSYM_EXPORT(lmacReleaseTxopQueue),
    ESP_ELFSYM_EXPORT(cnx_handshake_timeout_process),
    ESP_ELFSYM_EXPORT(lv_draw_buf_premultiply),
    ESP_ELFSYM_EXPORT(esp_now_set_pmk),
    ESP_ELFSYM_EXPORT(os_timer_arm_us),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_set_io_mode),
    ESP_ELFSYM_EXPORT(coex_schm_bt_piscan_wifi_scan),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY_PURPOSE_0),
    ESP_ELFSYM_EXPORT(ieee80211_wme_updateparams),
    ESP_ELFSYM_EXPORT(lv_checkbox_set_text_static),
    ESP_ELFSYM_EXPORT(esp_mspi_pin_init),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_issue_disconnect),
    ESP_ELFSYM_EXPORT(ble_hs_log_mbuf),
    ESP_ELFSYM_EXPORT(wcscpy),
    ESP_ELFSYM_EXPORT(wDev_Get_Next_TBTT),
    ESP_ELFSYM_EXPORT(wifi_station_get_target_ssid),
    ESP_ELFSYM_EXPORT(ble_hs_id_infer_auto),
    ESP_ELFSYM_EXPORT(ip4_frag),
    ESP_ELFSYM_EXPORT(phy_wait_rfpll_cal_end_new),
    ESP_ELFSYM_EXPORT(heap_caps_malloc_prefer),
    ESP_ELFSYM_EXPORT(grove_get_mode),
    ESP_ELFSYM_EXPORT(esp_mspi_get_io),
    ESP_ELFSYM_EXPORT(ppSearchTxframe),
    ESP_ELFSYM_EXPORT(esp_restart_noos),
    ESP_ELFSYM_EXPORT(cos),
    ESP_ELFSYM_EXPORT(mbedtls_psa_mac_abort),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_get_addr_data_from_legacy),
    ESP_ELFSYM_EXPORT(r_ble_ll_mem_generic_data_deinit),
    ESP_ELFSYM_EXPORT(xEventGroupClearBits),
    ESP_ELFSYM_EXPORT(xpt2046_softspi_driver),
    ESP_ELFSYM_EXPORT(sta_input),
    ESP_ELFSYM_EXPORT(lv_style_set_radius),
    ESP_ELFSYM_EXPORT(sdmmc_init_mmc_decode_cid),
    ESP_ELFSYM_EXPORT(root_driver),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_cmd_proc),
    ESP_ELFSYM_EXPORT(phy_get_tx_gain_value),
    ESP_ELFSYM_EXPORT(esp_set_assoc_ie),
    ESP_ELFSYM_EXPORT(esp_mmu_map),
    ESP_ELFSYM_EXPORT(__powisf2),
    ESP_ELFSYM_EXPORT(r_ble_lll_recycle_sch_in_task),
    ESP_ELFSYM_EXPORT(rcGet11ACHighestRateIdx),
    ESP_ELFSYM_EXPORT(uECC_vli_nativeToBytes),
    ESP_ELFSYM_EXPORT(phy_bt_set_tx_gain_new),
    ESP_ELFSYM_EXPORT(lv_point_transform),
    ESP_ELFSYM_EXPORT(ieee80211_parse_action),
    ESP_ELFSYM_EXPORT(esp_cache_err_clear_active_err),
    ESP_ELFSYM_EXPORT(_ZTSSt9exception),
    ESP_ELFSYM_EXPORT(tcp_eff_send_mss_netif),
    ESP_ELFSYM_EXPORT(esp_transport_set_async_connect_func),
    ESP_ELFSYM_EXPORT(lv_dropdown_create),
    ESP_ELFSYM_EXPORT(pm_disable_beacon_monitor_timer),
    ESP_ELFSYM_EXPORT(spi_device_get_actual_freq),
    ESP_ELFSYM_EXPORT(lv_anim_set_path_cb),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_set_write_protect),
    ESP_ELFSYM_EXPORT(hostap_add_in_mac_list),
    ESP_ELFSYM_EXPORT(ble_uuid_flat),
    ESP_ELFSYM_EXPORT(r_ble_lll_per_adv_coex_dpc_update),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_prep_write),
    ESP_ELFSYM_EXPORT(lv_text_is_cmd),
    ESP_ELFSYM_EXPORT(esp_supplicant_common_init),
    ESP_ELFSYM_EXPORT(ip6_chksum_pseudo),
    ESP_ELFSYM_EXPORT(wDev_SnifferRxAmpdu),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_check_cert_usage),
    ESP_ELFSYM_EXPORT(dhcps_stop),
    ESP_ELFSYM_EXPORT(phy_ant_btrx_cfg),
    ESP_ELFSYM_EXPORT(pm_coex_update_wifi_time_slice_state),
    ESP_ELFSYM_EXPORT(wifi_deinit_process),
    ESP_ELFSYM_EXPORT(ledc_hal_init),
    ESP_ELFSYM_EXPORT(device_get_first_by_compatible),
    ESP_ELFSYM_EXPORT(r_ble_ll_set_default_sync_transfer_params),
    ESP_ELFSYM_EXPORT(advFilter_hci_controlDuplicate),
    ESP_ELFSYM_EXPORT(_ZTVN9__gnu_cxx24__concurrence_lock_errorE),
    ESP_ELFSYM_EXPORT(httpd_unrecv),
    ESP_ELFSYM_EXPORT(esp_transport_set_func),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_update_random_addr),
    ESP_ELFSYM_EXPORT(hal_mac_set_csi_cbw),
    ESP_ELFSYM_EXPORT(sdmmc_init_sd_bus_width),
    ESP_ELFSYM_EXPORT(esp_system_get_time),
    ESP_ELFSYM_EXPORT(lv_draw_label_dsc_init),
    ESP_ELFSYM_EXPORT(phy_i2c_rc_cal_set),
    ESP_ELFSYM_EXPORT(lv_bar_set_value),
    ESP_ELFSYM_EXPORT(esp_event_handler_register_with_internal),
    ESP_ELFSYM_EXPORT(esp_wifi_get_wps_status_internal),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_from_mont_rep),
    ESP_ELFSYM_EXPORT(ble_uuid_init_from_att_buf),
    ESP_ELFSYM_EXPORT(phy_disable_agc),
    ESP_ELFSYM_EXPORT(r_ble_hw_periodiclist_add),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_cth_flow_enable),
    ESP_ELFSYM_EXPORT(esp_wifi_register_mgmt_frame_internal),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_hash_ring_list_add),
    ESP_ELFSYM_EXPORT(r_os_msys_register),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_get_one_checked),
    ESP_ELFSYM_EXPORT(gpio_func_sel),
    ESP_ELFSYM_EXPORT(pm_allow_to_enable_bcn_filter),
    ESP_ELFSYM_EXPORT(eb_txdesc_space),
    ESP_ELFSYM_EXPORT(tcpip_api_call),
    ESP_ELFSYM_EXPORT(display_disp_sleep),
    ESP_ELFSYM_EXPORT(display_draw_bitmap),
    ESP_ELFSYM_EXPORT(uECC_vli_isZero),
    ESP_ELFSYM_EXPORT(sdmmc_init_spi_crc),
    ESP_ELFSYM_EXPORT(_ZTV11WL_Ext_Perf),
    ESP_ELFSYM_EXPORT(r_esp_ble_ll_set_public_addr),
    ESP_ELFSYM_EXPORT(ble_hs_hci_get_hci_version),
    ESP_ELFSYM_EXPORT(ndp_rxcb),
    ESP_ELFSYM_EXPORT(isspace),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_set_resolve_enabled),
    ESP_ELFSYM_EXPORT(sdmmc_sd_get_erase_timeout_ms),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_flex_flow),
    ESP_ELFSYM_EXPORT(lv_draw_buf_init),
    ESP_ELFSYM_EXPORT(xswap),
    ESP_ELFSYM_EXPORT(netbuf_alloc),
    ESP_ELFSYM_EXPORT(i2s_hal_calc_mclk_precise_division),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_recycle_periodic_event_end),
    ESP_ELFSYM_EXPORT(ble_midi_get_conn_handle),
    ESP_ELFSYM_EXPORT(mbedtls_aes_setkey_dec),
    ESP_ELFSYM_EXPORT(esp_libc_timekeeping_adjtime_get_remaining_us),
    ESP_ELFSYM_EXPORT(phy_tsens_temp_read),
    ESP_ELFSYM_EXPORT(wpa_akm_to_suite),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_le_rd_phy),
    ESP_ELFSYM_EXPORT(ieee80211_psq_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_delete_by_sm_num),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_rxbuffer_reuse_unapply_set),
    ESP_ELFSYM_EXPORT(lv_obj_get_style_opa_recursive),
    ESP_ELFSYM_EXPORT(multi_heap_realloc),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_pad_column),
    ESP_ELFSYM_EXPORT(task_event_group_wait_any),
    ESP_ELFSYM_EXPORT(ble_hs_wakeup_tx),
    ESP_ELFSYM_EXPORT(lv_cache_entry_get_data),
    ESP_ELFSYM_EXPORT(esp_flash_chip_gd),
    ESP_ELFSYM_EXPORT(ic_get_random),
    ESP_ELFSYM_EXPORT(lv_event_get_target),
    ESP_ELFSYM_EXPORT(pbuf_chain),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_config_init),
    ESP_ELFSYM_EXPORT(i2c_master_receive),
    ESP_ELFSYM_EXPORT(pbuf_free),
    ESP_ELFSYM_EXPORT(frexp),
    ESP_ELFSYM_EXPORT(lv_draw_sw_grad_get),
    ESP_ELFSYM_EXPORT(_ZTTSi),
    ESP_ELFSYM_EXPORT(r_os_mbuf_cmpf),
    ESP_ELFSYM_EXPORT(hal_init_tb_ru_power),
    ESP_ELFSYM_EXPORT(psa_free_persistent_key_data),
    ESP_ELFSYM_EXPORT(ic_disable_crypto),
    ESP_ELFSYM_EXPORT(_ZdlPvj),
    ESP_ELFSYM_EXPORT(srand),
    ESP_ELFSYM_EXPORT(adv_hci_sendScanReqRxdVsEvent),
    ESP_ELFSYM_EXPORT(current_task_is_wifi_task),
    ESP_ELFSYM_EXPORT(lv_obj_calculate_style_text_align),
    ESP_ELFSYM_EXPORT(wifi_crypto_init),
    ESP_ELFSYM_EXPORT(r_ble_hw_whitelist_update),
    ESP_ELFSYM_EXPORT(psa_cipher_abort),
    ESP_ELFSYM_EXPORT(rc_get_trc_default),
    ESP_ELFSYM_EXPORT(lv_obj_scroll_to_view_recursive),
    ESP_ELFSYM_EXPORT(hal_set_rx_active_pti),
    ESP_ELFSYM_EXPORT(ledc_hal_ls_channel_update),
    ESP_ELFSYM_EXPORT(regdma_link_new_addr_map),
    ESP_ELFSYM_EXPORT(crypto_bignum_is_one),
    ESP_ELFSYM_EXPORT(ppRemoveHEAMPDUflags),
    ESP_ELFSYM_EXPORT(wpa_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_set_chan_class),
    ESP_ELFSYM_EXPORT(pwr_hal_set_mac_modem_state_wakeup_protect_disable),
    ESP_ELFSYM_EXPORT(esp_mbedtls_get_bytes_avail),
    ESP_ELFSYM_EXPORT(spicommon_bus_free_io_cfg),
    ESP_ELFSYM_EXPORT(r_ble_ll_tx_pkt_in),
    ESP_ELFSYM_EXPORT(g_sa_query_mac_list),
    ESP_ELFSYM_EXPORT(__ctzdi2),
    ESP_ELFSYM_EXPORT(vTaskDelay),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_prof_is_wpa_internal),
    ESP_ELFSYM_EXPORT(esp_transport_destroy_foundation_transport),
    ESP_ELFSYM_EXPORT(_ZSt4sortIN9__gnu_cxx17__normal_iteratorIPtSt6vectorItSaItEEEEEvT_S7_),
    ESP_ELFSYM_EXPORT(memspi_host_init_pointers),
    ESP_ELFSYM_EXPORT(ble_sm_proc_can_advance),
    ESP_ELFSYM_EXPORT(lv_anim_core_deinit),
    ESP_ELFSYM_EXPORT(lv_bar_is_symmetrical),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_enabled),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_data_base_init),
    ESP_ELFSYM_EXPORT(pm_beacon_offset_funcs_empty_init),
    ESP_ELFSYM_EXPORT(lv_image_set_rotation),
    ESP_ELFSYM_EXPORT(http_should_keep_alive),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_default_wifi_scan),
    ESP_ELFSYM_EXPORT(heap_caps_malloc_default),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_num_of_written_blocks),
    ESP_ELFSYM_EXPORT(r_ble_scan_advertise_filter_env_init),
    ESP_ELFSYM_EXPORT(ble_hs_pvcy_add_entry),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_chk_proc_start),
    ESP_ELFSYM_EXPORT(lv_buttonmatrix_has_button_ctrl),
    ESP_ELFSYM_EXPORT(__cxa_rethrow),
    ESP_ELFSYM_EXPORT(lv_ll_get_prev),
    ESP_ELFSYM_EXPORT(lv_memset),
    ESP_ELFSYM_EXPORT(bluetooth_stop_advertising),
    ESP_ELFSYM_EXPORT(esp_log_default_level),
    ESP_ELFSYM_EXPORT(ieee80211_psq_is_buff_pkt),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_reset),
    ESP_ELFSYM_EXPORT(crypt_get_iv),
    ESP_ELFSYM_EXPORT(coex_event_duration_get),
    ESP_ELFSYM_EXPORT(__lock___sfp_recursive_mutex),
    ESP_ELFSYM_EXPORT(sleep_retention_entries_create),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_read_le),
    ESP_ELFSYM_EXPORT(wpa_cipher_valid_mgmt_group),
    ESP_ELFSYM_EXPORT(esp_netif_new),
    ESP_ELFSYM_EXPORT(lv_fs_drv_register),
    ESP_ELFSYM_EXPORT(g_authmode_threshold_failure),
    ESP_ELFSYM_EXPORT(lv_area_align),
    ESP_ELFSYM_EXPORT(gpio_controller_init_descriptors),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_periodic_set_new_data),
    ESP_ELFSYM_EXPORT(ieee80211_ap_try_sa_query),
    ESP_ELFSYM_EXPORT(mbedtls_ecp_write_key_ext),
    ESP_ELFSYM_EXPORT(ble_hs_conn_remove),
    ESP_ELFSYM_EXPORT(ble_stack_enable),
    ESP_ELFSYM_EXPORT(lv_style_is_empty),
    ESP_ELFSYM_EXPORT(__wrap_gpio_reset_pin),
    ESP_ELFSYM_EXPORT(esp32_i2s_driver),
    ESP_ELFSYM_EXPORT(_ZSt15__ostream_writeIcSt11char_traitsIcEEvRSt13basic_ostreamIT_T0_EPKS3_i),
    ESP_ELFSYM_EXPORT(app_manager_start_for_result),
    ESP_ELFSYM_EXPORT(pm_coex_slice_timeout_process),
    ESP_ELFSYM_EXPORT(esp_wifi_disconnect_internal),
    ESP_ELFSYM_EXPORT(i2c_driver_install),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_check_overlap),
    ESP_ELFSYM_EXPORT(app_manager_for_each_manifest),
    ESP_ELFSYM_EXPORT(i2c_master_multi_buffer_transmit),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ffdh_key_agreement),
    ESP_ELFSYM_EXPORT(lv_textarea_set_align),
    ESP_ELFSYM_EXPORT(phy_txiq_cover),
    ESP_ELFSYM_EXPORT(CAMERA_TYPE),
    ESP_ELFSYM_EXPORT(ds_hal_finish),
    ESP_ELFSYM_EXPORT(xTimerGetReloadMode),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_rssi),
    ESP_ELFSYM_EXPORT(hal_he_bssid_init),
    ESP_ELFSYM_EXPORT(hid_report_map_keyboard_consumer_mouse_len),
    ESP_ELFSYM_EXPORT(lv_timer_set_auto_delete),
    ESP_ELFSYM_EXPORT(wifi_set_promis_process),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_key_exchange_md_tls1_2),
    ESP_ELFSYM_EXPORT(mbedtls_psa_cipher_decrypt_setup),
    ESP_ELFSYM_EXPORT(chm_set_current_channel),
    ESP_ELFSYM_EXPORT(pmksa_cache_set_current),
    ESP_ELFSYM_EXPORT(lv_obj_refresh_ext_draw_size),
    ESP_ELFSYM_EXPORT(r_sdkconfig_get_opts),
    ESP_ELFSYM_EXPORT(sta_update_power_off_reset_flag),
    ESP_ELFSYM_EXPORT(esp_wifi_scan_start),
    ESP_ELFSYM_EXPORT(tt_app_selectiondialog_start),
    ESP_ELFSYM_EXPORT(wDev_Insert_KeyEntry),
    ESP_ELFSYM_EXPORT(hal_enable_tx_statistics),
    ESP_ELFSYM_EXPORT(_ZGVNSt7collateIcE2idE),
    ESP_ELFSYM_EXPORT(PMU_instance),
    ESP_ELFSYM_EXPORT(esp_vfs_register_fd_range),
    ESP_ELFSYM_EXPORT(test_get_nonmumimo_common),
    ESP_ELFSYM_EXPORT(_ZZNKSt7__cxx117collateIcE12do_transformEPKcS3_EN4_BufD1Ev),
    ESP_ELFSYM_EXPORT(vTaskPlaceOnEventListRestricted),
    ESP_ELFSYM_EXPORT(lvgl_keyboard_remove),
    ESP_ELFSYM_EXPORT(npl_freertos_event_reset),
    ESP_ELFSYM_EXPORT(ipaddr_aton),
    ESP_ELFSYM_EXPORT(_rename_r),
    ESP_ELFSYM_EXPORT(netconn_delete),
    ESP_ELFSYM_EXPORT(phy_bb_wdg_cfg),
    ESP_ELFSYM_EXPORT(hal_crypto_is_key_valid),
    ESP_ELFSYM_EXPORT(r_os_cputime_ticks_to_usecs),
    ESP_ELFSYM_EXPORT(port_uxCriticalNesting),
    ESP_ELFSYM_EXPORT(ieee80211_add_beacon_app_ies),
    ESP_ELFSYM_EXPORT(tlsf_walk_pool),
    ESP_ELFSYM_EXPORT(lv_obj_hit_test),
    ESP_ELFSYM_EXPORT(_Znwj),
    ESP_ELFSYM_EXPORT(bootloader_execute_flash_command),
    ESP_ELFSYM_EXPORT(phy_i2c_master_reset),
    ESP_ELFSYM_EXPORT(lv_layer_top),
    ESP_ELFSYM_EXPORT(bt_bb_rx_correlator_set),
    ESP_ELFSYM_EXPORT(adiList_stack_addAdiList),
    ESP_ELFSYM_EXPORT(minmea_parse_rmc),
    ESP_ELFSYM_EXPORT(NAN_STANDARD_BSSID),
    ESP_ELFSYM_EXPORT(ieee80211_muedca_updateparams),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_enable_burst),
    ESP_ELFSYM_EXPORT(lv_anim_path_ease_in),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_post_gen_dhkey_cmp_evt),
    ESP_ELFSYM_EXPORT(ble_gap_deinit),
    ESP_ELFSYM_EXPORT(lv_indev_scroll_throw_predict),
    ESP_ELFSYM_EXPORT(tcp_new_ip_type),
    ESP_ELFSYM_EXPORT(cnx_assoc_timeout_process),
    ESP_ELFSYM_EXPORT(nvs_open_from_partition),
    ESP_ELFSYM_EXPORT(esp_cache_sync_ops_enter_critical_section),
    ESP_ELFSYM_EXPORT(esf_buf_setdown),
    ESP_ELFSYM_EXPORT(ic_trc_update_ifx_phy_mode),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_parse_change_cipher_spec),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_rx_data_pdu),
    ESP_ELFSYM_EXPORT(hexstr2bin),
    ESP_ELFSYM_EXPORT(port_uxOldInterruptState),
    ESP_ELFSYM_EXPORT(r_ble_ll_addr_is_id),
    ESP_ELFSYM_EXPORT(hci_stack_deinitEnv),
    ESP_ELFSYM_EXPORT(itwt_information_timeout_fn),
    ESP_ELFSYM_EXPORT(pp_attach),
    ESP_ELFSYM_EXPORT(lv_style_reset),
    ESP_ELFSYM_EXPORT(npl_freertos_os_started),
    ESP_ELFSYM_EXPORT(ppEnqueueRxq),
    ESP_ELFSYM_EXPORT(lodepng_get_bpp),
    ESP_ELFSYM_EXPORT(ieee80211_he_updateparams),
    ESP_ELFSYM_EXPORT(lv_area_diff),
    ESP_ELFSYM_EXPORT(hmac_hal_configure),
    ESP_ELFSYM_EXPORT(ic_get_ptk_alg),
    ESP_ELFSYM_EXPORT(nvs_set_i8),
    ESP_ELFSYM_EXPORT(hal_mac_rx_disable),
    ESP_ELFSYM_EXPORT(npl_freertos_time_ms_to_ticks),
    ESP_ELFSYM_EXPORT(coex_schm_bt_idle_wifi_scan),
    ESP_ELFSYM_EXPORT(strdup),
    ESP_ELFSYM_EXPORT(trc_onPPTxDone),
    ESP_ELFSYM_EXPORT(_cos),
    ESP_ELFSYM_EXPORT(r_ble_phy_init),
    ESP_ELFSYM_EXPORT(lv_chart_refresh),
    ESP_ELFSYM_EXPORT(__time_wday),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_aux_scannable_pdu_make),
    ESP_ELFSYM_EXPORT(esp_event_loop_create),
    ESP_ELFSYM_EXPORT(hal_mac_tx_clr_mplen),
    ESP_ELFSYM_EXPORT(_ZZ10app_ledgervE6ledger),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_sm_new),
    ESP_ELFSYM_EXPORT(wmemchr),
    ESP_ELFSYM_EXPORT(http_auth_basic),
    ESP_ELFSYM_EXPORT(ip6_output_if_src),
    ESP_ELFSYM_EXPORT(phy_pbus_print),
    ESP_ELFSYM_EXPORT(r_ble_ll_mtrack_update),
    ESP_ELFSYM_EXPORT(vListInitialiseItem),
    ESP_ELFSYM_EXPORT(lmacConfMib),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_npl_init),
    ESP_ELFSYM_EXPORT(ble_sm_lgcy_random_exec),
    ESP_ELFSYM_EXPORT(spi_hal_get_intr_mask),
    ESP_ELFSYM_EXPORT(lv_draw_buf_width_to_stride),
    ESP_ELFSYM_EXPORT(tsf_hal_get_time),
    ESP_ELFSYM_EXPORT(ble_transport_hs_init),
    ESP_ELFSYM_EXPORT(setlocale),
    ESP_ELFSYM_EXPORT(gps_settings_add_configuration),
    ESP_ELFSYM_EXPORT(modem_clock_hal_select_ble_rtc_timer_lpclk_source),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_update_link_state),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_free_param),
    ESP_ELFSYM_EXPORT(esp_get_free_heap_size),
    ESP_ELFSYM_EXPORT(nvs_close),
    ESP_ELFSYM_EXPORT(pbuf_alloced_custom),
    ESP_ELFSYM_EXPORT(phy_pbus_clear_reg),
    ESP_ELFSYM_EXPORT(ppGetTxframe),
    ESP_ELFSYM_EXPORT(pm_beacon_miss_exceeded_wakeup_disabled),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_WR_DIS_KEY_PURPOSE_2),
    ESP_ELFSYM_EXPORT(inc_byte_array),
    ESP_ELFSYM_EXPORT(_ZTISt20bad_array_new_length),
    ESP_ELFSYM_EXPORT(lora_set_modulation),
    ESP_ELFSYM_EXPORT(esp_coex_common_timer_arm_us_wrapper),
    ESP_ELFSYM_EXPORT(wpa_sta_clear_curr_pmksa),
    ESP_ELFSYM_EXPORT(rtc_clk_32k_disable_external),
    ESP_ELFSYM_EXPORT(lv_anim_path_step),
    ESP_ELFSYM_EXPORT(lmacRecycleMPDU),
    ESP_ELFSYM_EXPORT(esp_partition_main_flash_region_safe),
    ESP_ELFSYM_EXPORT(aes_wrap),
    ESP_ELFSYM_EXPORT(mspi_timing_config_set_psram_clock),
    ESP_ELFSYM_EXPORT(_ZTIN9__gnu_cxx24__concurrence_lock_errorE),
    ESP_ELFSYM_EXPORT(_ZTISt16invalid_argument),
    ESP_ELFSYM_EXPORT(_tzset_unlocked),
    ESP_ELFSYM_EXPORT(ble_gap_conn_active),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_active_chanset_is_sec),
    ESP_ELFSYM_EXPORT(ieee80211_gettid),
    ESP_ELFSYM_EXPORT(g_wifi_event_mask),
    ESP_ELFSYM_EXPORT(pointer_get_touched_points),
    ESP_ELFSYM_EXPORT(_ZGVNSt9money_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(phy_get_rf_cal_version),
    ESP_ELFSYM_EXPORT(pthread_include_pthread_semaphore_impl),
    ESP_ELFSYM_EXPORT(mac_rxbuf_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_get_chan_num),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_txbuf_pdu_info_get),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_get_handle),
    ESP_ELFSYM_EXPORT(lv_event_get_param),
    ESP_ELFSYM_EXPORT(device_construct_add_start),
    ESP_ELFSYM_EXPORT(lv_slider_get_value),
    ESP_ELFSYM_EXPORT(regdma_find_module_link_tail),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd_send_if_cond),
    ESP_ELFSYM_EXPORT(audio_stream_get_enabled),
    ESP_ELFSYM_EXPORT(offchan_in_progress),
    ESP_ELFSYM_EXPORT(ppSelectNextQueue),
    ESP_ELFSYM_EXPORT(ieee80211_is_lr_only),
    ESP_ELFSYM_EXPORT(gpio_uninstall_isr_service),
    ESP_ELFSYM_EXPORT(ieee80211_find_elem),
    ESP_ELFSYM_EXPORT(tcp_segs_free),
    ESP_ELFSYM_EXPORT(ppTask),
    ESP_ELFSYM_EXPORT(lv_table_class),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_set_scan_rsp_data),
    ESP_ELFSYM_EXPORT(AUDIO_STREAM_TYPE),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_update_aux_offset_in_buffer),
    ESP_ELFSYM_EXPORT(lv_draw_sw_rotate),
    ESP_ELFSYM_EXPORT(ble_gap_adv_set_data),
    ESP_ELFSYM_EXPORT(lv_free),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_write_record),
    ESP_ELFSYM_EXPORT(_ZTVSt7codecvtIDiDu10_mbstate_tE),
    ESP_ELFSYM_EXPORT(r_ble_log_internal_x2),
    ESP_ELFSYM_EXPORT(hci_transport_host_cmd_tx),
    ESP_ELFSYM_EXPORT(hal_mac_txq_enable),
    ESP_ELFSYM_EXPORT(ieee80211_parse_wapi),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_can_chg_whitelist),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_reset_link_state),
    ESP_ELFSYM_EXPORT(lv_draw_sw_blend_image_to_l8),
    ESP_ELFSYM_EXPORT(psa_hash_abort),
    ESP_ELFSYM_EXPORT(psa_mac_sign_setup),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_cleanup),
    ESP_ELFSYM_EXPORT(ic_get_addr),
    ESP_ELFSYM_EXPORT(esp_http_client_request_send),
    ESP_ELFSYM_EXPORT(ieee80211_scan_attach),
    ESP_ELFSYM_EXPORT(r_hal_timer_task_start),
    ESP_ELFSYM_EXPORT(swprintf),
    ESP_ELFSYM_EXPORT(adc_oneshot_hal_channel_config),
    ESP_ELFSYM_EXPORT(_copy),
    ESP_ELFSYM_EXPORT(md5_vector),
    ESP_ELFSYM_EXPORT(coex_schm_bt_default_wifi_conn),
    ESP_ELFSYM_EXPORT(lv_label_class),
    ESP_ELFSYM_EXPORT(hostapd_cleanup),
    ESP_ELFSYM_EXPORT(btdm_broker_env_deinit),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_init),
    ESP_ELFSYM_EXPORT(device_exists_of_type),
    ESP_ELFSYM_EXPORT(_sinf),
    ESP_ELFSYM_EXPORT(esp_vfs_open),
    ESP_ELFSYM_EXPORT(wifi_start_process),
    ESP_ELFSYM_EXPORT(generic_esp32c5_module),
    ESP_ELFSYM_EXPORT(tcp_seg_copy),
    ESP_ELFSYM_EXPORT(fscanf),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_invoke_ll_process),
    ESP_ELFSYM_EXPORT(i2c_controller_register16be_get),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_get_empty_packet_rssi),
    ESP_ELFSYM_EXPORT(lv_obj_set_content_width),
    ESP_ELFSYM_EXPORT(memp_TCP_SEG),
    ESP_ELFSYM_EXPORT(conn_stack_initEnv),
    ESP_ELFSYM_EXPORT(rcGetDefaultHigestRateIdx),
    ESP_ELFSYM_EXPORT(xSemaphoreCreateGenericWithCaps),
    ESP_ELFSYM_EXPORT(tcp_pcb_remove),
    ESP_ELFSYM_EXPORT(explicit_bzero),
    ESP_ELFSYM_EXPORT(lv_cache_drop),
    ESP_ELFSYM_EXPORT(lodepng_decode),
    ESP_ELFSYM_EXPORT(tbtt_adaptive_servo),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_tls12_write_client_hello_exts),
    ESP_ELFSYM_EXPORT(hal_crypto_mgmt_rx_enabled),
    ESP_ELFSYM_EXPORT(ble_hs_hw_error),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_random),
    ESP_ELFSYM_EXPORT(lv_flex_init),
    ESP_ELFSYM_EXPORT(esf_buf_free_static),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_sniff_sco_wifi_scan),
    ESP_ELFSYM_EXPORT(ble_sm_alg_s1),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_is_sta_sae_reauth_node),
    ESP_ELFSYM_EXPORT(lv_font_has_static_bitmap),
    ESP_ELFSYM_EXPORT(_ZTVSt18__moneypunct_cacheIcLb1EE),
    ESP_ELFSYM_EXPORT(ble_uuid_init_from_buf),
    ESP_ELFSYM_EXPORT(esp_wifi_ap_get_sta_aid),
    ESP_ELFSYM_EXPORT(r_ble_phy_rate_to_mode),
    ESP_ELFSYM_EXPORT(mbedtls_pk_init),
    ESP_ELFSYM_EXPORT(bundle_get_int64),
    ESP_ELFSYM_EXPORT(lv_indev_send_event),
    ESP_ELFSYM_EXPORT(ble_store_config_persist_peer_secs),
    ESP_ELFSYM_EXPORT(ht_get_min_subframe_len),
    ESP_ELFSYM_EXPORT(lv_label_get_text_selection_start),
    ESP_ELFSYM_EXPORT(spi_flash_guard_set),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx118time_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEEE),
    ESP_ELFSYM_EXPORT(pm_set_wifimac_regdma_link_selection),
    ESP_ELFSYM_EXPORT(mt_fetch_peer),
    ESP_ELFSYM_EXPORT(spi_controller_lock),
    ESP_ELFSYM_EXPORT(xIsrStackTop),
    ESP_ELFSYM_EXPORT(window_manager_remove),
    ESP_ELFSYM_EXPORT(spi_flash_restore_cache),
    ESP_ELFSYM_EXPORT(ble_lll_mmgmt_env_p),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand_env_deinit),
    ESP_ELFSYM_EXPORT(dns_setserver),
    ESP_ELFSYM_EXPORT(esp_app_desc),
    ESP_ELFSYM_EXPORT(ble_att_create_chan),
    ESP_ELFSYM_EXPORT(lv_display_set_default),
    ESP_ELFSYM_EXPORT(_ZSt20__throw_system_errori),
    ESP_ELFSYM_EXPORT(http_parser_init),
    ESP_ELFSYM_EXPORT(fabs),
    ESP_ELFSYM_EXPORT(phy_get_tsens_value),
    ESP_ELFSYM_EXPORT(esp_flash_read),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_alloc_tx_buffer_and_hdr),
    ESP_ELFSYM_EXPORT(phy_index_to_txbbgain),
    ESP_ELFSYM_EXPORT(r_ble_lll_ext_scan_coex_dpc_update_on_start),
    ESP_ELFSYM_EXPORT(_ZTVN2tt14RecursiveMutexE),
    ESP_ELFSYM_EXPORT(wpa_michael_mic_failure),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_active_chanset_set_sec),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_recycle_buffer),
    ESP_ELFSYM_EXPORT(ieee80211_ht_node_cleanup),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_shift_r),
    ESP_ELFSYM_EXPORT(pm_beacon_offset_is_sampling),
    ESP_ELFSYM_EXPORT(__extendsfdf2),
    ESP_ELFSYM_EXPORT(lv_bin_decoder_close),
    ESP_ELFSYM_EXPORT(vEventGroupSetBitsCallback),
    ESP_ELFSYM_EXPORT(rcUpdateRate),
    ESP_ELFSYM_EXPORT(lv_obj_set_parent),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ecp_load_representation),
    ESP_ELFSYM_EXPORT(itwt_cal_conts_miss_beacon_sp_count),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_start_rsp_timer),
    ESP_ELFSYM_EXPORT(spi_flash_chip_winbond_erase_block),
    ESP_ELFSYM_EXPORT(esp_intr_enable_source),
    ESP_ELFSYM_EXPORT(ble_transport_ll_deinit),
    ESP_ELFSYM_EXPORT(pwm_get_duty),
    ESP_ELFSYM_EXPORT(lv_obj_set_scroll_snap_y),
    ESP_ELFSYM_EXPORT(is_fid_setup),
    ESP_ELFSYM_EXPORT(coex_hw_timer_disable),
    ESP_ELFSYM_EXPORT(esp_wifi_set_keep_alive_time),
    ESP_ELFSYM_EXPORT(rtos_int_enter),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_next_event),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_core_bitlen),
    ESP_ELFSYM_EXPORT(esp_clk_tree_lp_fast_get_freq_hz),
    ESP_ELFSYM_EXPORT(__bufio_fill_locked),
    ESP_ELFSYM_EXPORT(lv_chart_set_div_line_count),
    ESP_ELFSYM_EXPORT(gdma_hal_set_priority),
    ESP_ELFSYM_EXPORT(cnx_connect_next_ap_timeout_process),
    ESP_ELFSYM_EXPORT(psa_cipher_decrypt_setup),
    ESP_ELFSYM_EXPORT(tcp_next_iss),
    ESP_ELFSYM_EXPORT(rcUpdateRxDone),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_wifi_scan),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY2),
    ESP_ELFSYM_EXPORT(lv_group_focus_obj),
    ESP_ELFSYM_EXPORT(netif_set_up),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx1110moneypunctIwLb1EEE),
    ESP_ELFSYM_EXPORT(esp_event_post),
    ESP_ELFSYM_EXPORT(lv_deinit),
    ESP_ELFSYM_EXPORT(g_spi_lock_main_flash_dev),
    ESP_ELFSYM_EXPORT(phy_get_sar_sig_ref),
    ESP_ELFSYM_EXPORT(apm_hal_set_master_sec_mode),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_have_rxd_scan_rsp),
    ESP_ELFSYM_EXPORT(lodepng_decode32),
    ESP_ELFSYM_EXPORT(crypto_ec_deinit),
    ESP_ELFSYM_EXPORT(scan_prefer_chan),
    ESP_ELFSYM_EXPORT(tcpip_input),
    ESP_ELFSYM_EXPORT(httpd_resp_send_err),
    ESP_ELFSYM_EXPORT(esp_efuse_get_key_purpose),
    ESP_ELFSYM_EXPORT(lv_draw_init),
    ESP_ELFSYM_EXPORT(__kernel_sinf),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_output_max_frag_len),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_grid_cell_row_span),
    ESP_ELFSYM_EXPORT(chm_end_op_timeout_process),
    ESP_ELFSYM_EXPORT(lv_draw_sw_line),
    ESP_ELFSYM_EXPORT(bt_rf_coex_cfg_p),
    ESP_ELFSYM_EXPORT(pm_disconnected_wake),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_config_bt_conn_wifi_scan),
    ESP_ELFSYM_EXPORT(ledc_bind_channel_timer),
    ESP_ELFSYM_EXPORT(esp_libc_include_memcpy_impl),
    ESP_ELFSYM_EXPORT(brownout_hal_config),
    ESP_ELFSYM_EXPORT(spi_flash_disable_interrupts_caches_and_other_cpu),
    ESP_ELFSYM_EXPORT(mem_init),
    ESP_ELFSYM_EXPORT(ic_set_he_min_subframe_len_tab),
    ESP_ELFSYM_EXPORT(send_ap_probe),
    ESP_ELFSYM_EXPORT(__floatunsisf),
    ESP_ELFSYM_EXPORT(ieee80211_vht_node_cleanup),
    ESP_ELFSYM_EXPORT(__eqdf2),
    ESP_ELFSYM_EXPORT(pm_twt_keep_alive_timeout_process),
    ESP_ELFSYM_EXPORT(ble_bb_cte_set_cte_ant_delay_rx_aoa_1us),
    ESP_ELFSYM_EXPORT(spi_bus_lock_get_by_id),
    ESP_ELFSYM_EXPORT(esp_mmu_map_virt),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_write_encrypted),
    ESP_ELFSYM_EXPORT(regdma_find_module_link_head),
    ESP_ELFSYM_EXPORT(esp_rtc_init),
    ESP_ELFSYM_EXPORT(phy_ant_clr_update_flag),
    ESP_ELFSYM_EXPORT(cnx_sta_connect_led_timer_cb),
    ESP_ELFSYM_EXPORT(ic_is_mgmt_hwdecr_enabled),
    ESP_ELFSYM_EXPORT(pthread_setspecific),
    ESP_ELFSYM_EXPORT(service_instance_destruct),
    ESP_ELFSYM_EXPORT(ble_hs_state_ctx),
    ESP_ELFSYM_EXPORT(lv_image_buf_get_transformed_area),
    ESP_ELFSYM_EXPORT(xTaskGenericNotifyStateClear),
    ESP_ELFSYM_EXPORT(r_DB_hash_list_reset),
    ESP_ELFSYM_EXPORT(uart_hal_set_stop_bits),
    ESP_ELFSYM_EXPORT(wifi_connect_process),
    ESP_ELFSYM_EXPORT(lmacInit),
    ESP_ELFSYM_EXPORT(hostapd_config_clear_wpa_psk),
    ESP_ELFSYM_EXPORT(esp_aes_release_hardware),
    ESP_ELFSYM_EXPORT(lv_draw_sw_box_shadow),
    ESP_ELFSYM_EXPORT(battery_sense_power_supply_driver),
    ESP_ELFSYM_EXPORT(esp_partition_read_raw),
    ESP_ELFSYM_EXPORT(usb_device_controller_claim),
    ESP_ELFSYM_EXPORT(sta_auth_sae),
    ESP_ELFSYM_EXPORT(wDev_Get_Next_BcnOffset),
    ESP_ELFSYM_EXPORT(r_filter_duplicate_mode_enable),
    ESP_ELFSYM_EXPORT(esp_aes_gcm_init),
    ESP_ELFSYM_EXPORT(ldexpf),
    ESP_ELFSYM_EXPORT(ieee80211_action_vendor_spec_attach),
    ESP_ELFSYM_EXPORT(tcp_sent),
    ESP_ELFSYM_EXPORT(lv_anim_delete),
    ESP_ELFSYM_EXPORT(pm_extend_tbtt_adaptive_attach),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_sec_chan_pdu_update),
    ESP_ELFSYM_EXPORT(ledc_hal_set_hpoint),
    ESP_ELFSYM_EXPORT(wifi_init_in_caller_task),
    ESP_ELFSYM_EXPORT(ble_midi_set_active),
    ESP_ELFSYM_EXPORT(_ZTVSt15_Sp_counted_ptrIPN2tt7service13memorychecker20MemoryCheckerServiceELN9__gnu_cxx12_Lock_policyE1EE),
    ESP_ELFSYM_EXPORT(r_ble_lll_rxpdu_copy),
    ESP_ELFSYM_EXPORT(pthread_include_pthread_cond_var_impl),
    ESP_ELFSYM_EXPORT(hal_he_get_aid),
    ESP_ELFSYM_EXPORT(coex_schm_interval_get),
    ESP_ELFSYM_EXPORT(rsn_is_snonce_cookie),
    ESP_ELFSYM_EXPORT(__lock___env_recursive_mutex),
    ESP_ELFSYM_EXPORT(ieee80211_reg_netstack_buf_cb),
    ESP_ELFSYM_EXPORT(mbedtls_rsa_free),
    ESP_ELFSYM_EXPORT(memp_NETCONN),
    ESP_ELFSYM_EXPORT(mspi_timing_get_psram_low_speed_freq_mhz),
    ESP_ELFSYM_EXPORT(hal_he_set_bbrxhung_time),
    ESP_ELFSYM_EXPORT(_Z14app_fs_is_fileRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(esp_system_get_time_resolution),
    ESP_ELFSYM_EXPORT(ieee80211_beacon_construct),
    ESP_ELFSYM_EXPORT(psa_get_and_lock_key_slot),
    ESP_ELFSYM_EXPORT(material_symbols_statusbar_16),
    ESP_ELFSYM_EXPORT(ble_transport_to_ll_cmd_impl),
    ESP_ELFSYM_EXPORT(pm_disable_dream_timer),
    ESP_ELFSYM_EXPORT(roundf),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_start),
    ESP_ELFSYM_EXPORT(_ZSt17__verify_groupingPKcjRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE),
    ESP_ELFSYM_EXPORT(rtc_get_time),
    ESP_ELFSYM_EXPORT(lmac_update_tx_statistic),
    ESP_ELFSYM_EXPORT(lv_list_set_button_text),
    ESP_ELFSYM_EXPORT(ble_gap_update_params),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_run),
    ESP_ELFSYM_EXPORT(esp_vfs_fat_format_drive),
    ESP_ELFSYM_EXPORT(esp_libc_locks_init),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_is_prime_ext),
    ESP_ELFSYM_EXPORT(ble_hs_hci_util_set_data_len),
    ESP_ELFSYM_EXPORT(r_ble_ll_mtrack_length_set),
    ESP_ELFSYM_EXPORT(gpio_hal_matrix_out),
    ESP_ELFSYM_EXPORT(bluetooth_midi_stop),
    ESP_ELFSYM_EXPORT(tlsf_create_with_pool),
    ESP_ELFSYM_EXPORT(hal_mac_get_txq_pmd),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_adv_set_enable),
    ESP_ELFSYM_EXPORT(icmp6_param_problem),
    ESP_ELFSYM_EXPORT(ieee80211_ht_updateparams),
    ESP_ELFSYM_EXPORT(ip4_output_if_opt),
    ESP_ELFSYM_EXPORT(lvgl_try_lock),
    ESP_ELFSYM_EXPORT(ble_gap_rx_rd_rem_ver_info_complete),
    ESP_ELFSYM_EXPORT(hal_set_sta_tbtt),
    ESP_ELFSYM_EXPORT(__kernel_rem_pio2),
    ESP_ELFSYM_EXPORT(spi_hal_cal_clock_conf),
    ESP_ELFSYM_EXPORT(rtc_gpio_pulldown_dis),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_update_priv_addr),
    ESP_ELFSYM_EXPORT(ieee80211_send_deauth_no_bss),
    ESP_ELFSYM_EXPORT(memspi_host_set_write_protect),
    ESP_ELFSYM_EXPORT(__wtime_month),
    ESP_ELFSYM_EXPORT(_do_wifi_stop),
    ESP_ELFSYM_EXPORT(lv_obj_get_style_prop),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_txbuf_memory_alloc_and_reset),
    ESP_ELFSYM_EXPORT(gf_double),
    ESP_ELFSYM_EXPORT(isxdigit),
    ESP_ELFSYM_EXPORT(esp_http_client_fetch_headers),
    ESP_ELFSYM_EXPORT(lv_indev_enable),
    ESP_ELFSYM_EXPORT(lmacMSDUAged),
    ESP_ELFSYM_EXPORT(spi_flash_chip_generic_erase_chip),
    ESP_ELFSYM_EXPORT(r_ble_ll_get_addr_type),
    ESP_ELFSYM_EXPORT(panic_print_hex),
    ESP_ELFSYM_EXPORT(phy_txpwr_correct_new),
    ESP_ELFSYM_EXPORT(nd6_clear_destination_cache),
    ESP_ELFSYM_EXPORT(MPI_OPERATIONS_REG),
    ESP_ELFSYM_EXPORT(esp_task_wdt_print_triggered_tasks),
    ESP_ELFSYM_EXPORT(r_ble_lll_adv_set_adv_data),
    ESP_ELFSYM_EXPORT(sae_accept_sta),
    ESP_ELFSYM_EXPORT(r_ble_ll_resolv_rpa_timer_reset),
    ESP_ELFSYM_EXPORT(lv_draw_buf_init_with_default_handlers),
    ESP_ELFSYM_EXPORT(psa_generate_key_internal),
    ESP_ELFSYM_EXPORT(nl_langinfo),
    ESP_ELFSYM_EXPORT(preferences_put_string),
    ESP_ELFSYM_EXPORT(mld6_report_groups),
    ESP_ELFSYM_EXPORT(lv_area_is_in),
    ESP_ELFSYM_EXPORT(ble_ll_scan_env_p),
    ESP_ELFSYM_EXPORT(esp_wifi_get_promiscuous),
    ESP_ELFSYM_EXPORT(towupper),
    ESP_ELFSYM_EXPORT(wpa_sm_set_ap_rsnxe_override),
    ESP_ELFSYM_EXPORT(lv_textarea_add_char),
    ESP_ELFSYM_EXPORT(lv_style_set_bg_color),
    ESP_ELFSYM_EXPORT(esp_wifi_enable_sae_pk_only_mode_internal),
    ESP_ELFSYM_EXPORT(lv_draw_mask_rect),
    ESP_ELFSYM_EXPORT(lv_timer_create_basic),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_master_common_init),
    ESP_ELFSYM_EXPORT(lvgl_unlock),
    ESP_ELFSYM_EXPORT(r_ble_lll_recycle_buffer_in_task),
    ESP_ELFSYM_EXPORT(esp_wifi_set_mode),
    ESP_ELFSYM_EXPORT(esp_phy_store_cal_data_to_nvs),
    ESP_ELFSYM_EXPORT(gdma_hal_get_eof_desc_addr),
    ESP_ELFSYM_EXPORT(pbuf_add_header_force),
    ESP_ELFSYM_EXPORT(r_os_msys_mbuf_get_enhanced),
    ESP_ELFSYM_EXPORT(esp_ecdsa_transparent_verify_hash_start),
    ESP_ELFSYM_EXPORT(lmacProcessLongRetryFail),
    ESP_ELFSYM_EXPORT(udp_netif_ip_addr_changed),
    ESP_ELFSYM_EXPORT(log10),
    ESP_ELFSYM_EXPORT(_ZTVN10__cxxabiv120__si_class_type_infoE),
    ESP_ELFSYM_EXPORT(esp32_bluetooth_driver),
    ESP_ELFSYM_EXPORT(r_ble_ll_is_valid_adv_mode),
    ESP_ELFSYM_EXPORT(r_ble_ll_reset),
    ESP_ELFSYM_EXPORT(r_ble_ll_whitelist_init),
    ESP_ELFSYM_EXPORT(lv_style_set_transition),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_min_width),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_derive_keys),
    ESP_ELFSYM_EXPORT(esp_vfs_register_common),
    ESP_ELFSYM_EXPORT(esp_test_clr_tx_tb_statistics),
    ESP_ELFSYM_EXPORT(netif_remove),
    ESP_ELFSYM_EXPORT(uxTaskGetSystemState),
    ESP_ELFSYM_EXPORT(pm_beacon_add_total_counter),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_bind),
    ESP_ELFSYM_EXPORT(BcnSendTick),
    ESP_ELFSYM_EXPORT(one_bits),
    ESP_ELFSYM_EXPORT(lv_anim_set_var),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_initiate_dle),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt10moneypunctIcLb0EEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(esp_netif_dhcpc_start),
    ESP_ELFSYM_EXPORT(ble_gap_event_listener_register),
    ESP_ELFSYM_EXPORT(r_ble_ll_adv_scan_req_rxd),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_sm_npl_deinit),
    ESP_ELFSYM_EXPORT(ppProcessRxPktHdr),
    ESP_ELFSYM_EXPORT(ieee80211_add_assoc_resp_ies),
    ESP_ELFSYM_EXPORT(f_readdir),
    ESP_ELFSYM_EXPORT(valloc),
    ESP_ELFSYM_EXPORT(r_ble_hw_whitelist_sort),
    ESP_ELFSYM_EXPORT(gpio_matrix_input),
    ESP_ELFSYM_EXPORT(ble_sm_tx),
    ESP_ELFSYM_EXPORT(mspi_timing_enter_low_speed_mode),
    ESP_ELFSYM_EXPORT(esp_wifi_internal_set_log_level),
    ESP_ELFSYM_EXPORT(lv_dropdown_set_options_static),
    ESP_ELFSYM_EXPORT(gps_settings_for_each_configuration),
    ESP_ELFSYM_EXPORT(phy_i2c_master_cmd_mem_init),
    ESP_ELFSYM_EXPORT(phy_set_txclk_en),
    ESP_ELFSYM_EXPORT(mbedtls_x509_get_name),
    ESP_ELFSYM_EXPORT(phy_pkdadc_set),
    ESP_ELFSYM_EXPORT(sntp_setoperatingmode),
    ESP_ELFSYM_EXPORT(spi_bus_lock_get_acquiring_dev),
    ESP_ELFSYM_EXPORT(_ZZNSt9formatterIPKvcE5parseERSt26basic_format_parse_contextIcEENKUlvE0_clEv),
    ESP_ELFSYM_EXPORT(mbedtls_x509_free_subject_alt_name),
    ESP_ELFSYM_EXPORT(phy_param_track_tot),
    ESP_ELFSYM_EXPORT(tt_timezone_get_code),
    ESP_ELFSYM_EXPORT(uECC_vli_numBits),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_bg_opa),
    ESP_ELFSYM_EXPORT(heap_caps_get_info),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_der_to_raw),
    ESP_ELFSYM_EXPORT(mbedtls_aes_crypt_ofb),
    ESP_ELFSYM_EXPORT(pvt_func_enable),
    ESP_ELFSYM_EXPORT(usb_host_hid_unsubscribe),
    ESP_ELFSYM_EXPORT(_ZGVNSt7__cxx119money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE2idE),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_write_binary),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_check_curve),
    ESP_ELFSYM_EXPORT(lv_anim_get),
    ESP_ELFSYM_EXPORT(wifi_disconnect_process),
    ESP_ELFSYM_EXPORT(bluetooth_set_radio_enabled),
    ESP_ELFSYM_EXPORT(lv_draw_buf_clear),
    ESP_ELFSYM_EXPORT(_Z13init_mtk_l76bP6Device),
    ESP_ELFSYM_EXPORT(_ZSt9terminatev),
    ESP_ELFSYM_EXPORT(r_ble_lll_conn_event_abort_in_first_rx_tx_pair),
    ESP_ELFSYM_EXPORT(isinff),
    ESP_ELFSYM_EXPORT(tcp_bind),
    ESP_ELFSYM_EXPORT(mbedtls_psa_ecp_export_key),
    ESP_ELFSYM_EXPORT(error_to_string),
    ESP_ELFSYM_EXPORT(rcLowerSched),
    ESP_ELFSYM_EXPORT(f_mount),
    ESP_ELFSYM_EXPORT(efuse_hal_get_minor_chip_version),
    ESP_ELFSYM_EXPORT(esp_sha256_driver_compute),
    ESP_ELFSYM_EXPORT(netconn_recv_udp_raw_netbuf_flags),
    ESP_ELFSYM_EXPORT(ble_sm_our_pair_rand),
    ESP_ELFSYM_EXPORT(ble_att_cmd_prepare),
    ESP_ELFSYM_EXPORT(lv_style_set_outline_opa),
    ESP_ELFSYM_EXPORT(sys_thread_sem_get),
    ESP_ELFSYM_EXPORT(lvgl_toolbar_clear_actions),
    ESP_ELFSYM_EXPORT(wpa_sm_deauthenticate),
    ESP_ELFSYM_EXPORT(r_ble_ll_scan_get_peer_rpa),
    ESP_ELFSYM_EXPORT(s_microseconds_offset),
    ESP_ELFSYM_EXPORT(mbedtls_md_error_from_psa),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_border_side),
    ESP_ELFSYM_EXPORT(lv_style_set_line_width),
    ESP_ELFSYM_EXPORT(phy_get_modem_flag),
    ESP_ELFSYM_EXPORT(sdmmc_enter_higher_speed_mode),
    ESP_ELFSYM_EXPORT(lv_slider_set_mode),
    ESP_ELFSYM_EXPORT(wifi_set_rxcb_process),
    ESP_ELFSYM_EXPORT(chm_mhz2num),
    ESP_ELFSYM_EXPORT(_Z21app_metadata_parse_v2RKSt3mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES5_St4lessIS5_ESaISt4pairIKS5_S5_EEER11AppMetadata),
    ESP_ELFSYM_EXPORT(esp_task_wdt_reset),
    ESP_ELFSYM_EXPORT(g_in_blacklist_scanned_again),
    ESP_ELFSYM_EXPORT(mbedtls_threading_key_slot_mutex),
    ESP_ELFSYM_EXPORT(active_hid_rpt_map),
    ESP_ELFSYM_EXPORT(adv_stack_enableClearLegacyAdvVsCmd),
    ESP_ELFSYM_EXPORT(cnx_connect_next_ap),
    ESP_ELFSYM_EXPORT(__tzcalc_limits),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_div_mpi),
    ESP_ELFSYM_EXPORT(icmp6_dest_unreach),
    ESP_ELFSYM_EXPORT(_isatty_r),
    ESP_ELFSYM_EXPORT(adc_reset_lock_acquire),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_is_active_scan),
    ESP_ELFSYM_EXPORT(esp_netif_create_default_wifi_ap),
    ESP_ELFSYM_EXPORT(esp_task_wdt_impl_timer_feed),
    ESP_ELFSYM_EXPORT(__file_str_put),
    ESP_ELFSYM_EXPORT(i2c_hal_master_trans_start),
    ESP_ELFSYM_EXPORT(lv_display_get_rotation),
    ESP_ELFSYM_EXPORT(rc5G11ACSchedTbl),
    ESP_ELFSYM_EXPORT(_i2c_hal_deinit),
    ESP_ELFSYM_EXPORT(MIDI_SVC_UUID),
    ESP_ELFSYM_EXPORT(esp_vfs_set_readonly_flag),
    ESP_ELFSYM_EXPORT(ieee80211_alloc_deauth),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_sm_active_insert),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_update),
    ESP_ELFSYM_EXPORT(ieee80211_espnow_set_init_flag),
    ESP_ELFSYM_EXPORT(_ZTVNSt7__cxx119money_putIwSt19ostreambuf_iteratorIwSt11char_traitsIwEEEE),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_raw_to_der),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_get_bytes_avail),
    ESP_ELFSYM_EXPORT(pm_on_beacon_rx),
    ESP_ELFSYM_EXPORT(wifi_station_stop),
    ESP_ELFSYM_EXPORT(nvs_sec_provider_include_impl),
    ESP_ELFSYM_EXPORT(ieee80211_regdomain_chan_num),
    ESP_ELFSYM_EXPORT(panic_clear_active_interrupts),
    ESP_ELFSYM_EXPORT(ip4_input),
    ESP_ELFSYM_EXPORT(double_jacobian_default),
    ESP_ELFSYM_EXPORT(esp_partition_read),
    ESP_ELFSYM_EXPORT(ble_store_config_persist_rpa_recs),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_force_sync_limit),
    ESP_ELFSYM_EXPORT(xQueueReceive),
    ESP_ELFSYM_EXPORT(lwip_netconn_do_write),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_chanmap_req_make),
    ESP_ELFSYM_EXPORT(wpa_cipher_put_suites),
    ESP_ELFSYM_EXPORT(bt_bb_set_le_tx_on_delay),
    ESP_ELFSYM_EXPORT(phy_rxiq_get_mis),
    ESP_ELFSYM_EXPORT(pm_scale_listen_interval),
    ESP_ELFSYM_EXPORT(esp_cmac_verify_finish),
    ESP_ELFSYM_EXPORT(lv_chart_create),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_conf_verify),
    ESP_ELFSYM_EXPORT(spi_flash_hal_gpspi_check_status),
    ESP_ELFSYM_EXPORT(wcslen),
    ESP_ELFSYM_EXPORT(timestamp_id),
    ESP_ELFSYM_EXPORT(wDev_record_ftm_data_local),
    ESP_ELFSYM_EXPORT(lmacReachLongLimit),
    ESP_ELFSYM_EXPORT(bluetooth_midi_start),
    ESP_ELFSYM_EXPORT(__issignalingf),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_update),
    ESP_ELFSYM_EXPORT(psa_pk_status_to_mbedtls),
    ESP_ELFSYM_EXPORT(ieee80211_parse_wmeparams),
    ESP_ELFSYM_EXPORT(ic_enable_rx),
    ESP_ELFSYM_EXPORT(r_ble_hci_trans_hs_cmd_tx),
    ESP_ELFSYM_EXPORT(wdev_set_promis_ctrl_pkt),
    ESP_ELFSYM_EXPORT(lwip_hook_tcp_isn),
    ESP_ELFSYM_EXPORT(sdmmc_init_io),
    ESP_ELFSYM_EXPORT(sha_hal_set_mode),
    ESP_ELFSYM_EXPORT(r_ble_ll_sync_cancel),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSt14basic_ifstreamIcSt11char_traitsIcEED1Ev),
    ESP_ELFSYM_EXPORT(phy_wait_freq_set_busy_new),
    ESP_ELFSYM_EXPORT(pp_gettid),
    ESP_ELFSYM_EXPORT(sys_timeouts_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_update_cur_entry),
    ESP_ELFSYM_EXPORT(lv_textarea_cursor_down),
    ESP_ELFSYM_EXPORT(r_ble_ll_df_conn_cte_tx_set_param),
    ESP_ELFSYM_EXPORT(wpa_parse_wpa_ie_rsn),
    ESP_ELFSYM_EXPORT(esp_gpio_reserve),
    ESP_ELFSYM_EXPORT(__floatundisf),
    ESP_ELFSYM_EXPORT(wpa3_hostap_auth_init),
    ESP_ELFSYM_EXPORT(flash_mmap_lock_freeze),
    ESP_ELFSYM_EXPORT(esp_log_linked_list_set_level),
    ESP_ELFSYM_EXPORT(ble_att_cmd_get),
    ESP_ELFSYM_EXPORT(pm_active_timeout_process),
    ESP_ELFSYM_EXPORT(ieee80211_output_process),
    ESP_ELFSYM_EXPORT(camera_set_rotation),
    ESP_ELFSYM_EXPORT(scan_set_scan_id),
    ESP_ELFSYM_EXPORT(he_twt_setup_txcb),
    ESP_ELFSYM_EXPORT(mbedtls_x509_oid_get_x509_ext_type),
    ESP_ELFSYM_EXPORT(ieee80211_psq_take_head),
    ESP_ELFSYM_EXPORT(_ZTSN10__cxxabiv115__forced_unwindE),
    ESP_ELFSYM_EXPORT(__wpa_send_eapol),
    ESP_ELFSYM_EXPORT(phy_ant_init),
    ESP_ELFSYM_EXPORT(uart_intr_config),
    ESP_ELFSYM_EXPORT(lv_label_set_text_selection_start),
    ESP_ELFSYM_EXPORT(lv_text_ins),
    ESP_ELFSYM_EXPORT(lv_cache_acquire),
    ESP_ELFSYM_EXPORT(r_ble_ll_ctrl_conn_param_pdu_make),
    ESP_ELFSYM_EXPORT(esp_rsa_ds_opaque_sign_hash_complete),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_standby_bt_conn_wifi_scan),
    ESP_ELFSYM_EXPORT(gdma_ahb_hal_read_intr_status),
    ESP_ELFSYM_EXPORT(memp_PBUF),
    ESP_ELFSYM_EXPORT(reset_cs_thres),
    ESP_ELFSYM_EXPORT(get_fd_entry),
    ESP_ELFSYM_EXPORT(tt_timezone_get_name),
    ESP_ELFSYM_EXPORT(ble_store_config_compare_bond_count),
    ESP_ELFSYM_EXPORT(__time_mon),
    ESP_ELFSYM_EXPORT(ppCalVHTDeliNum),
    ESP_ELFSYM_EXPORT(service_manager_add),
    ESP_ELFSYM_EXPORT(coex_core_deinit),
    ESP_ELFSYM_EXPORT(lv_textarea_set_password_bullet),
    ESP_ELFSYM_EXPORT(ic_del_extra_softap_rx_ba),
    ESP_ELFSYM_EXPORT(get_act_hp_dbias),
    ESP_ELFSYM_EXPORT(rcClearCurSched),
    ESP_ELFSYM_EXPORT(psa_mac_update),
    ESP_ELFSYM_EXPORT(mbedtls_psa_external_get_random),
    ESP_ELFSYM_EXPORT(esp_aes_cipher_finish),
    ESP_ELFSYM_EXPORT(__wrap___bufio_setvbuf),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_config_defaults),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_radius),
    ESP_ELFSYM_EXPORT(ble_gattc_read_long),
    ESP_ELFSYM_EXPORT(ble_sm_create_chan),
    ESP_ELFSYM_EXPORT(_ZZNSt18__moneypunct_cacheIwLb1EE8_M_cacheERKSt6localeEN11_Scoped_strC2ERKSbIwSt11char_traitsIwESaIwEE),
    ESP_ELFSYM_EXPORT(_ZSt13__int_to_charIcmEiPT_T0_PKS0_St13_Ios_Fmtflagsb),
    ESP_ELFSYM_EXPORT(r_ble_ll_rand_init),
    ESP_ELFSYM_EXPORT(r_ble_ll_conn_hci_read_rem_features),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_start_renegotiation),
    ESP_ELFSYM_EXPORT(ieee80211_find_ext_elem),
    ESP_ELFSYM_EXPORT(esp_http_client_get_user_data),
    ESP_ELFSYM_EXPORT(esp_panic_handler_feed_wdts),
    ESP_ELFSYM_EXPORT(lv_display_get_original_vertical_resolution),
    ESP_ELFSYM_EXPORT(wifi_malloc),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_rxbuffer_free_all),
    ESP_ELFSYM_EXPORT(ic_set_key),
    ESP_ELFSYM_EXPORT(ESP_EFUSE_KEY_PURPOSE_4),
    ESP_ELFSYM_EXPORT(esp_ip4addr_ntoa),
    ESP_ELFSYM_EXPORT(ble_att_svr_rx_notify_multi),
    ESP_ELFSYM_EXPORT(esp_wifi_set_sta_key_internal),
    ESP_ELFSYM_EXPORT(_ZTVSt16__numpunct_cacheIwE),
    ESP_ELFSYM_EXPORT(gdma_start),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_process_aux_data),
    ESP_ELFSYM_EXPORT(ap_sta_add),
    ESP_ELFSYM_EXPORT(putc),
    ESP_ELFSYM_EXPORT(pmu_init),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_register_rxbuf_cb),
    ESP_ELFSYM_EXPORT(r_ble_lll_sched_delay_if_overlapped),
    ESP_ELFSYM_EXPORT(g_espnow_lock),
    ESP_ELFSYM_EXPORT(wifi_softap_cur_max_ap_bss_num),
    ESP_ELFSYM_EXPORT(wpa_supplicant_process_1_of_4),
    ESP_ELFSYM_EXPORT(mbedtls_pk_parse_key),
    ESP_ELFSYM_EXPORT(ieee80211_is_support_rate),
    ESP_ELFSYM_EXPORT(pbuf_ref),
    ESP_ELFSYM_EXPORT(phy_write_gain_mem),
    ESP_ELFSYM_EXPORT(rcGetSMPDURate),
    ESP_ELFSYM_EXPORT(ble_l2cap_get_conn_handle),
    ESP_ELFSYM_EXPORT(log10f),
    ESP_ELFSYM_EXPORT(_ZTVSt8messagesIcE),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_get_rxbuf_management_sch_item),
    ESP_ELFSYM_EXPORT(esp_rom_output_to_channels),
    ESP_ELFSYM_EXPORT(lv_fs_get_ext),
    ESP_ELFSYM_EXPORT(os_mempool_init),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_addr_family),
    ESP_ELFSYM_EXPORT(mpi_hal_enable_hardware_hw_op),
    ESP_ELFSYM_EXPORT(_ZTVSt7num_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE),
    ESP_ELFSYM_EXPORT(register_ieee80211_action_vendor_get_key_cb),
    ESP_ELFSYM_EXPORT(strcpy),
    ESP_ELFSYM_EXPORT(i2c_common_set_pins),
    ESP_ELFSYM_EXPORT(esp_transport_ssl_set_client_key_data_der),
    ESP_ELFSYM_EXPORT(lvgl_keyboard_on_start_lvgl),
    ESP_ELFSYM_EXPORT(esp_cache_msync),
    ESP_ELFSYM_EXPORT(sdmmc_send_cmd),
    ESP_ELFSYM_EXPORT(mbedtls_psa_crypto_init_include_impl),
    ESP_ELFSYM_EXPORT(hal_set_sta_tsf),
    ESP_ELFSYM_EXPORT(r_ble_ll_generate_dh_key_v1),
    ESP_ELFSYM_EXPORT(adjtime),
    ESP_ELFSYM_EXPORT(lv_obj_set_width),
    ESP_ELFSYM_EXPORT(_ZTv0_n12_NSiD1Ev),
    ESP_ELFSYM_EXPORT(lv_obj_set_flex_align),
    ESP_ELFSYM_EXPORT(lv_draw_rect_dsc_init),
    ESP_ELFSYM_EXPORT(ble_gatts_conn_init),
    ESP_ELFSYM_EXPORT(npl_freertos_callout_remaining_ticks),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_free_aux_memory),
    ESP_ELFSYM_EXPORT(tcp_abandon),
    ESP_ELFSYM_EXPORT(lv_draw_image_normal_helper),
    ESP_ELFSYM_EXPORT(_ZTVSt8time_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE),
    ESP_ELFSYM_EXPORT(_putc1),
    ESP_ELFSYM_EXPORT(sdmmc_get_erase_timeout_ms),
    ESP_ELFSYM_EXPORT(s_dp),
    ESP_ELFSYM_EXPORT(ble_sm_alg_f4),
    ESP_ELFSYM_EXPORT(vTaskSwitchContext),
    ESP_ELFSYM_EXPORT(esp_hmac_finish_opaque),
    ESP_ELFSYM_EXPORT(esp_efuse_is_flash_encryption_enabled),
    ESP_ELFSYM_EXPORT(esf_buf_setup_static),
    ESP_ELFSYM_EXPORT(app_uninstall),
    ESP_ELFSYM_EXPORT(lv_indev_get_user_data),
    ESP_ELFSYM_EXPORT(uart_get_selectlock),
    ESP_ELFSYM_EXPORT(pm_get_null_max_tx_time),
    ESP_ELFSYM_EXPORT(memspi_host_read_id_hs),
    ESP_ELFSYM_EXPORT(hal_mac_set_rxq_policy),
    ESP_ELFSYM_EXPORT(hci_stack_getBroker),
    ESP_ELFSYM_EXPORT(hal_sniffer_rx_set_promis),
    ESP_ELFSYM_EXPORT(_lock_acquire),
    ESP_ELFSYM_EXPORT(pp_post),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_ev_path_loss_threshold),
    ESP_ELFSYM_EXPORT(mbedtls_pk_get_bitlen),
    ESP_ELFSYM_EXPORT(__wrap_gpio_set_pull_mode),
    ESP_ELFSYM_EXPORT(ble_store_delete_cccd),
    ESP_ELFSYM_EXPORT(mbedtls_ecdsa_sign),
    ESP_ELFSYM_EXPORT(phy_set_pbus_mem_2g),
    ESP_ELFSYM_EXPORT(g_cte_ant_switch_delay_1m),
    ESP_ELFSYM_EXPORT(r_ble_ll_hci_is_event_enabled),
    ESP_ELFSYM_EXPORT(sta_get_beacon_timeout),
    ESP_ELFSYM_EXPORT(mbedtls_aes_crypt_ecb),
    ESP_ELFSYM_EXPORT(hal_mac_set_rx_beacon_memory_addr_map),
    ESP_ELFSYM_EXPORT(lvgl_hardware_keyboard_is_available),
    ESP_ELFSYM_EXPORT(service_instance_get_manifest),
    ESP_ELFSYM_EXPORT(lv_indev_create),
    ESP_ELFSYM_EXPORT(sha_hal_wait_idle),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_transform_scale_y),
    ESP_ELFSYM_EXPORT(ble_gap_rx_test_evt),
    ESP_ELFSYM_EXPORT(netconn_disconnect),
    ESP_ELFSYM_EXPORT(ff_disk_status),
    ESP_ELFSYM_EXPORT(panic_soc_check_pseudo_cause),
    ESP_ELFSYM_EXPORT(r_ble_lll_mmgmt_free),
    ESP_ELFSYM_EXPORT(itwt_information_timeout_fn_process),
    ESP_ELFSYM_EXPORT(xTaskGetStaticBuffers),
    ESP_ELFSYM_EXPORT(esp_psram_chip_init),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_set_reset_nvs_pmk_internal),
    ESP_ELFSYM_EXPORT(xStreamBufferReceive),
    ESP_ELFSYM_EXPORT(putwc),
    ESP_ELFSYM_EXPORT(lv_arc_set_bg_angles),
    ESP_ELFSYM_EXPORT(lv_draw_sw_mask_rect),
    ESP_ELFSYM_EXPORT(esp_netif_action_stop),
    ESP_ELFSYM_EXPORT(elf_find_sym_default),
    ESP_ELFSYM_EXPORT(esp_mbedtls_conn_delete),
    ESP_ELFSYM_EXPORT(ledc_timer_resume),
    ESP_ELFSYM_EXPORT(_ZTVSt8time_getIwSt19istreambuf_iteratorIwSt11char_traitsIwEEE),
    ESP_ELFSYM_EXPORT(esp_netif_netstack_buf_ref),
    ESP_ELFSYM_EXPORT(coex_schm_ble_mesh_traffic_bt_sniff_sco_wifi_conn),
    ESP_ELFSYM_EXPORT(mbedtls_ssl_read_record),
    ESP_ELFSYM_EXPORT(vListInitialise),
    ESP_ELFSYM_EXPORT(rc_disable_trc_by_interface),
    ESP_ELFSYM_EXPORT(_ZSt16__insertion_sortIN9__gnu_cxx17__normal_iteratorIP6direntSt6vectorIS2_SaIS2_EEEENS0_5__ops15_Iter_comp_iterIZNSt6ranges8__detail16__make_comp_projIPFbRKS2_SE_ESt8identityEEDaRT_RT0_EUlOSI_OSK_E_EEEvSI_SI_SK_),
    ESP_ELFSYM_EXPORT(__isnand),
    ESP_ELFSYM_EXPORT(npl_funcs_ro),
    ESP_ELFSYM_EXPORT(gdma_hal_set_burst_size),
    ESP_ELFSYM_EXPORT(ic_obtain_key),
    ESP_ELFSYM_EXPORT(aes_ccm_ae),
    ESP_ELFSYM_EXPORT(adc_hal_calibration_init),
    ESP_ELFSYM_EXPORT(spi_flash_hal_set_write_protect),
    ESP_ELFSYM_EXPORT(lv_style_set_border_side),
    ESP_ELFSYM_EXPORT(lmacProcessAckTimeout),
    ESP_ELFSYM_EXPORT(phy_version_str),
    ESP_ELFSYM_EXPORT(lv_event_get_indev),
    ESP_ELFSYM_EXPORT(esp_libc_timekeeping_adjtime_apply),
    ESP_ELFSYM_EXPORT(ppEmptyDelimiterLength),
    ESP_ELFSYM_EXPORT(esp_sleep_clock_config),
    ESP_ELFSYM_EXPORT(esp_wifi_sta_get_group_cipher_internal),
    ESP_ELFSYM_EXPORT(lv_obj_get_width),
    ESP_ELFSYM_EXPORT(_ZSt15__try_use_facetISt7num_getIcSt19istreambuf_iteratorIcSt11char_traitsIcEEEEPKT_RKSt6locale),
    ESP_ELFSYM_EXPORT(soc_memory_region_count),
    ESP_ELFSYM_EXPORT(_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EEPKS5_RKS8_),
    ESP_ELFSYM_EXPORT(crypto_bignum_cmp),
    ESP_ELFSYM_EXPORT(esp_partition_iterator_release),
    ESP_ELFSYM_EXPORT(r_ble_lll_scan_get_aux_data_at),
    ESP_ELFSYM_EXPORT(cJSON_GetNumberValue),
    ESP_ELFSYM_EXPORT(s_btdm_broker_env_p),
    ESP_ELFSYM_EXPORT(mpi_hal_write_rinv),
    ESP_ELFSYM_EXPORT(driver_is_compatible),
    ESP_ELFSYM_EXPORT(esp_transport_set_default_port),
    ESP_ELFSYM_EXPORT(esp_lcd_new_panel_st7796),
    ESP_ELFSYM_EXPORT(ble_l2cap_sig_timer),
    ESP_ELFSYM_EXPORT(_ZZNSt18__moneypunct_cacheIcLb0EE8_M_cacheERKSt6localeEN11_Scoped_strC2ERKSs),
    ESP_ELFSYM_EXPORT(eloop_destroy),
    ESP_ELFSYM_EXPORT(esp_rom_get_bootloader_offset),
    ESP_ELFSYM_EXPORT(scalbn),
    ESP_ELFSYM_EXPORT(mac_tx_set_tb),
    ESP_ELFSYM_EXPORT(spicommon_periph_claim),
    ESP_ELFSYM_EXPORT(ieee80211_parse_heopr),
    ESP_ELFSYM_EXPORT(mbedtls_mpi_shift_l),
    ESP_ELFSYM_EXPORT(netif_get_ip6_addr_match),
    ESP_ELFSYM_EXPORT(mspi_hw_info),
    ESP_ELFSYM_EXPORT(lv_obj_set_style_line_rounded),
    ESP_ELFSYM_EXPORT(wDev_IndicateCtrlFrame),
    ESP_ELFSYM_EXPORT(crypto_bignum_is_zero),
    ESP_ELFSYM_EXPORT(esp_cache_resume_ext_mem_cache),
    ESP_ELFSYM_EXPORT(ieee80211_action_vendor_reset_seq_state),
    ESP_ELFSYM_EXPORT(xPortSwitchFlag),
    ESP_ELFSYM_EXPORT(i2c_set_pin),
    ESP_ELFSYM_EXPORT(r_exception_list_ble_mesh_addr_table_remove),
    ESP_ELFSYM_EXPORT(wifi_send_raw_frame),
    ESP_ELFSYM_EXPORT(hal_mac_tx_get_blockack),
    ESP_ELFSYM_EXPORT(mbedtls_cipher_definitions),
    ESP_ELFSYM_EXPORT(_ZStmiRKSt15_Deque_iteratorI10QueuedItemRS0_PS0_ES5_),
    ESP_ELFSYM_EXPORT(wifi_softap_stop),
    ESP_ELFSYM_EXPORT(lv_button_create),
    ESP_ELFSYM_EXPORT(base_stack_disable),
    ESP_ELFSYM_EXPORT(lv_timer_create),
    ESP_ELFSYM_EXPORT(lv_font_montserrat_14),
    ESP_ELFSYM_EXPORT(psa_remove_key_data_from_memory),
    ESP_ELFSYM_EXPORT(pbuf_get_at),
    ESP_ELFSYM_EXPORT(psa_cipher_encrypt_setup),
    ESP_ELFSYM_EXPORT(httpd_req_get_url_query_len),
    ESP_ELFSYM_EXPORT(hal_mac_tsf_get_time),
    ESP_ELFSYM_EXPORT(pwr_hal_set_beacon_filter_force_dump_enable),
    ESP_ELFSYM_EXPORT(ble_att_svr_find_by_uuid),
    ESP_ELFSYM_EXPORT(pm_reset_twt_active_cnt),
    ESP_ELFSYM_EXPORT(heap_caps_get_largest_free_block),
    ESP_ELFSYM_END
};
