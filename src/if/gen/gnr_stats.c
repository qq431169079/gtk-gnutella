/*
 * Generated on Sun May  6 22:15:51 2018 by enum-msg.pl -- DO NOT EDIT
 *
 * Command: ../../../scripts/enum-msg.pl stats.lst
 */

#include "common.h"

#include "gnr_stats.h"

#include "lib/str.h"
#include "lib/override.h"	/* Must be the last header included */

/*
 * Symbolic descriptions for gnr_stats_t.
 */
static const char *stats_symbols[] = {
	"routing_errors",
	"routing_table_chunks",
	"routing_table_capacity",
	"routing_table_count",
	"routing_transient_avoided",
	"dups_with_higher_ttl",
	"spam_sha1_hits",
	"spam_name_hits",
	"spam_fake_hits",
	"spam_dup_hits",
	"spam_caught_hostile_ip",
	"spam_caught_hostile_held",
	"spam_ip_held",
	"local_searches",
	"local_hits",
	"local_partial_hits",
	"local_whats_new_hits",
	"local_query_hits",
	"local_g2_searches",
	"local_g2_hits",
	"local_g2_partial_hits",
	"local_aliased_hits",
	"oob_proxied_query_hits",
	"oob_queries",
	"oob_queries_stripped",
	"oob_queries_ignored",
	"query_oob_proxied_dups",
	"oob_hits_for_proxied_queries",
	"oob_hits_with_alien_ip",
	"oob_hits_ignored_on_spammer_hit",
	"oob_hits_ignored_on_unsecure_hit",
	"notified_oob_hits",
	"reliably_notified_oob_hits",
	"unclaimed_oob_hits",
	"unclaimed_reliable_oob_hits",
	"partially_claimed_oob_hits",
	"partially_claimed_reliable_oob_hits",
	"expired_oob_hits",
	"expired_reliable_oob_hits",
	"spurious_oob_hit_claim",
	"unrequested_oob_hits",
	"query_hit_for_untracked_query",
	"query_tracked_muids",
	"query_compact_count",
	"query_compact_size",
	"query_utf8",
	"query_sha1",
	"query_whats_new",
	"query_g2_utf8",
	"query_g2_sha1",
	"query_aliased_words",
	"query_guess",
	"query_guess_02",
	"guess_link_cache",
	"guess_cached_query_keys_held",
	"guess_cached_02_hosts_held",
	"guess_cached_g2_hosts_held",
	"guess_local_queries",
	"guess_local_running",
	"guess_local_query_hits",
	"guess_ultra_queried",
	"guess_ultra_acknowledged",
	"guess_g2_queried",
	"guess_g2_acknowledged",
	"local_push_throttled",
	"remote_push_throttled",
	"broadcasted_pushes",
	"push_proxy_udp_relayed",
	"push_proxy_tcp_relayed",
	"push_proxy_tcp_fw2fw",
	"push_proxy_broadcasted",
	"push_proxy_route_not_proxied",
	"push_proxy_failed",
	"push_relayed_via_local_route",
	"push_relayed_via_table_route",
	"local_dyn_queries",
	"leaf_dyn_queries",
	"oob_proxied_queries",
	"dyn_queries_completed_full",
	"dyn_queries_completed_partial",
	"dyn_queries_completed_zero",
	"dyn_queries_linger_extra",
	"dyn_queries_linger_results",
	"dyn_queries_linger_completed",
	"gtkg_total_queries",
	"gtkg_requeries",
	"queries_with_ggep_h",
	"queries_with_sr_udp",
	"giv_callbacks",
	"giv_discarded",
	"queue_callbacks",
	"queue_discarded",
	"banned_fds_total",
	"udp_read_ahead_count_sum",
	"udp_read_ahead_bytes_sum",
	"udp_read_ahead_old_sum",
	"udp_read_ahead_count_max",
	"udp_read_ahead_bytes_max",
	"udp_read_ahead_delay_max",
	"udp_fw2fw_pushes",
	"udp_fw2fw_pushes_to_self",
	"udp_fw2fw_pushes_patched",
	"udp_uhc_pings",
	"udp_uhc_pongs",
	"udp_bogus_source_ip",
	"udp_shunned_source_ip",
	"udp_rx_truncated",
	"udp_alien_message",
	"udp_unprocessed_message",
	"udp_tx_compressed",
	"udp_rx_compressed",
	"udp_compression_attempts",
	"udp_larger_hence_not_compressed",
	"udp_ambiguous",
	"udp_ambiguous_deeper_inspection",
	"udp_ambiguous_as_semi_reliable",
	"udp_sr_tx_messages_given",
	"udp_sr_tx_messages_deflated",
	"udp_sr_tx_messages_unsent",
	"udp_sr_tx_messages_banned",
	"udp_sr_tx_messages_good",
	"udp_sr_tx_messages_clogging",
	"udp_sr_tx_reliable_messages_given",
	"udp_sr_tx_reliable_messages_sent",
	"udp_sr_tx_reliable_messages_unsent",
	"udp_sr_tx_fragments_sent",
	"udp_sr_tx_fragments_resent",
	"udp_sr_tx_fragments_sending_avoided",
	"udp_sr_tx_fragments_oversent",
	"udp_sr_tx_total_acks_received",
	"udp_sr_tx_cumulative_acks_received",
	"udp_sr_tx_extended_acks_received",
	"udp_sr_tx_spurious_acks_received",
	"udp_sr_tx_invalid_acks_received",
	"udp_sr_tx_acks_requeued",
	"udp_sr_tx_plain_acks_dropped",
	"udp_sr_tx_enhanced_acks_dropped",
	"udp_sr_tx_plain_acks_sent",
	"udp_sr_tx_cumulative_acks_sent",
	"udp_sr_tx_extended_acks_sent",
	"udp_sr_tx_ears_sent",
	"udp_sr_tx_ears_oversent",
	"udp_sr_tx_ear_nacks_received",
	"udp_sr_tx_ear_followed_by_acks",
	"udp_sr_rx_fragments_received",
	"udp_sr_rx_fragments_duplicate",
	"udp_sr_rx_fragments_unreliable",
	"udp_sr_rx_fragments_dropped",
	"udp_sr_rx_fragments_lingering",
	"udp_sr_rx_messages_expired",
	"udp_sr_rx_messages_received",
	"udp_sr_rx_messages_inflated",
	"udp_sr_rx_messages_inflation_error",
	"udp_sr_rx_messages_unreliable",
	"udp_sr_rx_messages_empty",
	"udp_sr_rx_total_acks_sent",
	"udp_sr_rx_cumulative_acks_sent",
	"udp_sr_rx_extended_acks_sent",
	"udp_sr_rx_avoided_acks",
	"udp_sr_rx_ears_received",
	"udp_sr_rx_ears_for_unknown_message",
	"udp_sr_rx_ears_for_lingering_message",
	"udp_sr_rx_from_hostile_ip",
	"udp_g2_hits_rerouted_to_hub",
	"udp_g2_hits_undelivered",
	"consolidated_servers",
	"dup_downloads_in_consolidation",
	"discovered_server_guid",
	"changed_server_guid",
	"guid_collisions",
	"own_guid_collisions",
	"banned_guid_held",
	"received_known_fw_node_info",
	"revitalized_push_routes",
	"collected_push_proxies",
	"attempted_resource_switching",
	"attempted_resource_switching_after_error",
	"successful_resource_switching",
	"successful_plain_resource_switching",
	"successful_resource_switching_after_error",
	"queued_after_switching",
	"sunk_data",
	"ignored_data",
	"ignoring_after_mismatch",
	"ignoring_to_preserve_connection",
	"ignoring_during_aggressive_swarming",
	"ignoring_refused",
	"client_resource_switching",
	"client_plain_resource_switching",
	"client_followup_after_error",
	"parq_slot_resource_switching",
	"parq_retry_after_violation",
	"parq_retry_after_kick_out",
	"parq_slot_limit_overrides",
	"parq_quick_slots_granted",
	"parq_queue_sending_attempts",
	"parq_queue_sent",
	"parq_queue_follow_ups",
	"sha1_verifications",
	"tth_verifications",
	"qhit_seeding_of_orphan",
	"upload_seeding_of_orphan",
	"rudp_tx_bytes",
	"rudp_rx_bytes",
	"dht_estimated_size",
	"dht_estimated_size_stderr",
	"dht_kball_theoretical",
	"dht_kball_furthest",
	"dht_kball_closest",
	"dht_routing_buckets",
	"dht_routing_leaves",
	"dht_routing_max_depth",
	"dht_routing_good_nodes",
	"dht_routing_stale_nodes",
	"dht_routing_pending_nodes",
	"dht_routing_evicted_nodes",
	"dht_routing_evicted_firewalled_nodes",
	"dht_routing_evicted_quota_nodes",
	"dht_routing_promoted_pending_nodes",
	"dht_routing_pinged_promoted_nodes",
	"dht_routing_rejected_node_bucket_quota",
	"dht_routing_rejected_node_global_quota",
	"dht_completed_bucket_refresh",
	"dht_forced_bucket_refresh",
	"dht_forced_bucket_merge",
	"dht_bucket_merge",
	"dht_bucket_split",
	"dht_denied_unsplitable_bucket_refresh",
	"dht_bucket_alive_check",
	"dht_alive_pings_to_good_nodes",
	"dht_alive_pings_to_stale_nodes",
	"dht_alive_pings_to_shutdowning_nodes",
	"dht_alive_pings_avoided",
	"dht_alive_pings_skipped",
	"dht_revitalized_stale_nodes",
	"dht_rejected_value_on_quota",
	"dht_rejected_value_on_creator",
	"dht_lookup_rejected_node_on_net_quota",
	"dht_lookup_rejected_node_on_proximity",
	"dht_lookup_rejected_node_on_divergence",
	"dht_lookup_fixed_node_contact",
	"dht_keys_held",
	"dht_cached_keys_held",
	"dht_values_held",
	"dht_cached_kuid_targets_held",
	"dht_cached_roots_held",
	"dht_cached_roots_exact_hits",
	"dht_cached_roots_approximate_hits",
	"dht_cached_roots_misses",
	"dht_cached_roots_kball_lookups",
	"dht_cached_roots_contact_refreshed",
	"dht_cached_tokens_held",
	"dht_cached_tokens_hits",
	"dht_stable_nodes_held",
	"dht_fetch_local_hits",
	"dht_fetch_local_cached_hits",
	"dht_returned_expanded_values",
	"dht_returned_secondary_keys",
	"dht_claimed_secondary_keys",
	"dht_returned_expanded_cached_values",
	"dht_returned_cached_secondary_keys",
	"dht_claimed_cached_secondary_keys",
	"dht_published",
	"dht_removed",
	"dht_stale_replication",
	"dht_replication",
	"dht_republish",
	"dht_secondary_key_fetch",
	"dht_dup_values",
	"dht_kuid_collisions",
	"dht_own_kuid_collisions",
	"dht_caching_attempts",
	"dht_caching_successful",
	"dht_caching_partially_successful",
	"dht_key_offloading_checks",
	"dht_keys_selected_for_offloading",
	"dht_key_offloading_attempts",
	"dht_key_offloading_successful",
	"dht_key_offloading_partially_successful",
	"dht_values_offloaded",
	"dht_msg_received",
	"dht_msg_matching_contact_address",
	"dht_msg_fixed_contact_address",
	"dht_msg_from_hostile_address",
	"dht_msg_from_hostile_contact_address",
	"dht_rpc_msg_prepared",
	"dht_rpc_msg_cancelled",
	"dht_rpc_timed_out",
	"dht_rpc_replies_received",
	"dht_rpc_replies_fixed_contact",
	"dht_rpc_late_replies_received",
	"dht_rpc_kuid_reply_mismatch",
	"dht_rpc_recent_nodes_held",
	"dht_node_verifications",
	"dht_publishing_attempts",
	"dht_publishing_successful",
	"dht_publishing_partially_successful",
	"dht_publishing_satisfactory",
	"dht_republished_late",
	"dht_publishing_to_self",
	"dht_publishing_bg_attempts",
	"dht_publishing_bg_improvements",
	"dht_publishing_bg_successful",
	"dht_sha1_data_type_collisions",
	"dht_passively_protected_lookup_path",
	"dht_actively_protected_lookup_path",
	"dht_alt_loc_lookups",
	"dht_push_proxy_lookups",
	"dht_successful_alt_loc_lookups",
	"dht_successful_push_proxy_lookups",
	"dht_successful_node_push_entry_lookups",
	"dht_seeding_of_orphan",
	"stats_digest",
	"stats_tcp_digest",
	"stats_udp_digest",
};

/**
 * @return the symbolic description of the enum value.
 */
const char *
gnet_stats_general_to_string(gnr_stats_t x)
{
	if G_UNLIKELY(UNSIGNED(x) >= G_N_ELEMENTS(stats_symbols)) {
		str_t *s = str_private(G_STRFUNC, 80);
		str_printf(s, "Invalid gnr_stats_t code: %d", (int) x);
		return str_2c(s);
	}

	return stats_symbols[x];
}

/*
 * English descriptions for gnr_stats_t.
 */
static const char *stats_text[] = {
	N_("Routing errors"),
	N_("Routing table chunks"),
	N_("Routing table message capacity"),
	N_("Routing table message count"),
	N_("Routing through transient node avoided"),
	N_("Duplicates with higher TTL"),
	N_("SPAM SHA1 database hits"),
	N_("SPAM filename and size hits"),
	N_("SPAM fake hits"),
	N_("SPAM duplicate hits"),
	N_("SPAM dynamically caught hostile IP addresses"),
	N_("SPAM dynamically caught hostile IP held"),
	N_("SPAM spotted spamming IP addresses held"),
	N_("Searches to local DB"),
	N_("Hits on local DB"),
	N_("Hits on local partial files"),
	N_("Hits on \"what's new?\" queries"),
	N_("Query hits received for local queries"),
	N_("G2 searches to local DB"),
	N_("G2 hits on local DB"),
	N_("G2 hits on local partial files"),
	N_("Hits on aliased queries"),
	N_("Query hits received for OOB-proxied queries"),
	N_("Queries requesting OOB hit delivery"),
	N_("Stripped OOB flag on queries"),
	N_("Ignored OOB queries due to unclaimed hits"),
	N_("Duplicate OOB-proxied queries"),
	N_("OOB hits received for OOB-proxied queries"),
	N_("OOB hits bearing alien IP address"),
	N_("OOB hits ignored due to identified spamming address"),
	N_("OOB hits ignored due to unsecure promise from known secure host"),
	N_("Notifications sent for local OOB hits"),
	N_("Notifications reliably sent for local OOB hits"),
	N_("Unclaimed locally-generated OOB hits"),
	N_("Unclaimed locally-generated reliable OOB hits"),
	N_("Partially claimed locally-generated OOB hits"),
	N_("Partially claimed locally-generated reliable OOB hits"),
	N_("Expired delivery for local OOB hits"),
	N_("Expired delivery for local reliable OOB hits"),
	N_("Spurious OOB hit claiming received"),
	N_("Unrequested OOB hits received"),
	N_("Received query hits for untracked queries"),
	N_("Tracked query MUIDs"),
	N_("Compacted queries"),
	N_("Bytes saved by compacting"),
	N_("UTF8 queries"),
	N_("SHA1 queries"),
	N_("\"What's New?\" queries"),
	N_("UTF8 G2 queries"),
	N_("SHA1 G2 queries"),
	N_("Queries with aliased words"),
	N_("GUESS queries"),
	N_("GUESS queries (0.2)"),
	N_("GUESS link cache size"),
	N_("GUESS cached query keys held"),
	N_("GUESS cached 0.2 hosts held"),
	N_("GUESS cached G2 hosts held"),
	N_("GUESS locally generated queries"),
	N_("GUESS currently running queries"),
	N_("GUESS hits received for locally generated queries"),
	N_("GUESS ultra nodes queried"),
	N_("GUESS ultra nodes sending back an acknowledgment"),
	N_("GUESS G2 nodes queried"),
	N_("GUESS G2 nodes sending back an acknowledgment"),
	N_("Throttled local push messages"),
	N_("Throttled received push messages"),
	N_("Broadcasted push messages"),
	N_("Push-proxy UDP relayed messages"),
	N_("Push-proxy TCP relayed messages"),
	N_("Push-proxy TCP for FW<->FW transfers"),
	N_("Push-proxy broadcasted messages"),
	N_("Push-proxy found un-proxied local route"),
	N_("Push-proxy lookup failures"),
	N_("Push relayed via local route"),
	N_("Push relayed via routing table"),
	N_("Locally generated dynamic queries"),
	N_("Leaf-generated dynamic queries"),
	N_("OOB-proxied leaf queries"),
	N_("Fully completed dynamic queries"),
	N_("Partially completed dynamic queries"),
	N_("Dynamic queries ended with no results"),
	N_("Fully completed dynamic queries getting late results"),
	N_("Dynamic queries with partial late results"),
	N_("Dynamic queries completed by late results"),
	N_("Queries seen from GTKG"),
	N_("Queries seen from GTKG that were re-queries"),
	N_("Queries advertising support of GGEP \"H\""),
	N_("Queries advertising support of semi-reliable UDP"),
	N_("GIV callbacks received"),
	N_("GIV discarded due to no suitable download"),
	N_("QUEUE callbacks received"),
	N_("QUEUE discarded due to no suitable download"),
	N_("File descriptors banned running count"),
	N_("UDP read-ahead datagram running count"),
	N_("UDP read-ahead datagram running bytes"),
	N_("UDP read-ahead datagram \"old\" processed"),
	N_("UDP read-ahead datagram max count"),
	N_("UDP read-ahead datagram max bytes"),
	N_("UDP read-ahead datagram max delay"),
	N_("UDP push messages received for FW<->FW connections"),
	N_("UDP push messages requesting FW<->FW connection with ourselves"),
	N_("UDP push messages patched for FW<->FW connections"),
	N_("UDP UHC pings received"),
	N_("UDP UHC pongs sent"),
	N_("UDP messages with bogus source IP"),
	N_("UDP messages from shunned IP (discarded)"),
	N_("UDP truncated incoming messages"),
	N_("Alien UDP messages (non-Gnutella)"),
	N_("Unprocessed UDP Gnutella messages"),
	N_("Compressed UDP messages enqueued"),
	N_("Compressed UDP messages received"),
	N_("Candidates for UDP message compression"),
	N_("Uncompressed UDP messages due to no gain"),
	N_("Ambiguous UDP messages received"),
	N_("Ambiguous UDP messages inspected more deeply"),
	N_("Ambiguous UDP messages handled as semi-reliable UDP"),
	N_("Semi-reliable UDP total messages given for transmission"),
	N_("Semi-reliable UDP total messages deflated"),
	N_("Semi-reliable UDP total messages unsent"),
	N_("Semi-reliable UDP total messages dropped due to temporary ban"),
	N_("Semi-reliable UDP total messages sent to known responsive hosts"),
	N_("Semi-reliable UDP total messages partially sent due to clogging"),
	N_("Semi-reliable UDP reliable messages given for transmission"),
	N_("Semi-reliable UDP reliable messages correctly transmited"),
	N_("Semi-reliable UDP reliable messages not fully acknowledged"),
	N_("Semi-reliable UDP fragments sent"),
	N_("Semi-reliable UDP fragments resent"),
	N_("Semi-reliable UDP fragment sendings avoided"),
	N_("Semi-reliable UDP fragments sent too many times"),
	N_("Semi-reliable UDP total acknowledgments received"),
	N_("Semi-reliable UDP cumulative acknowledgments received"),
	N_("Semi-reliable UDP extended acknowledgments received"),
	N_("Semi-reliable UDP spurious acknowledgments received"),
	N_("Semi-reliable UDP invalid acknowledgments received"),
	N_("Semi-reliable UDP acknowledgments re-queued for sending"),
	N_("Semi-reliable UDP plain acknowledgments dropped"),
	N_("Semi-reliable UDP enhanced acknowledgments dropped"),
	N_("Semi-reliable UDP plain acknowledgments transmitted"),
	N_("Semi-reliable UDP cumulative acknowledgments transmitted"),
	N_("Semi-reliable UDP extended acknowledgments transmitted"),
	N_("Semi-reliable UDP EARs sent"),
	N_("Semi-reliable UDP too many EARs sent"),
	N_("Semi-reliable UDP EAR negative acknowledgments received"),
	N_("Semi-reliable UDP acknowledgments received after sending EARs"),
	N_("Semi-reliable UDP fragments received"),
	N_("Semi-reliable UDP duplicate fragments received"),
	N_("Semi-reliable UDP unreliable fragments received"),
	N_("Semi-reliable UDP dropped received fragments"),
	N_("Semi-reliable UDP fragments received whilst lingering"),
	N_("Semi-reliable UDP messages expired before re-assembly"),
	N_("Semi-reliable UDP messages re-assembled completely"),
	N_("Semi-reliable UDP messages inflated successfully"),
	N_("Semi-reliable UDP messages inflated incorrectly"),
	N_("Semi-reliable UDP unreliable messages received"),
	N_("Semi-reliable UDP empty messages received"),
	N_("Semi-reliable UDP total acknowledgments sent"),
	N_("Semi-reliable UDP cumulative acknowledgments sent"),
	N_("Semi-reliable UDP extended acknowledgments sent"),
	N_("Semi-reliable UDP avoided acknowledgment sendings"),
	N_("Semi-reliable UDP EARs received"),
	N_("Semi-reliable UDP EARs received for unknown message"),
	N_("Semi-reliable UDP EARs received whilst lingering"),
	N_("Semi-reliable UDP fragments from hostile IP addresses"),
	N_("UDP G2 hits rerouted to hub for delivery"),
	N_("UDP G2 hits undelivered"),
	N_("Consolidated servers (after GUID and IP address linking)"),
	N_("Duplicate downloads found during server consolidation"),
	N_("Discovered server GUIDs"),
	N_("Changed server GUIDs"),
	N_("Detected GUID collisions"),
	N_("Detected collisions with our own GUID"),
	N_("GUID dynamically banned"),
	N_("Firewalled node info for known hosts received in upload requests"),
	N_("Revitalized PUSH routes"),
	N_("Collected new PUSH proxies from other query hits"),
	N_("Attempted download resource switching on completion"),
	N_("Attempted download resource switching after error"),
	N_("Successful download resource switching (all kind)"),
	N_("Successful download resource switching between plain files"),
	N_("Successful download resource switching after error"),
	N_("Actively queued after resource switching attempt"),
	N_("Sunk HTTP reply data on error codes"),
	N_("Ignored downloaded data"),
	N_("Ignoring requested after data mismatch"),
	N_("Ignoring requested to preserve connection"),
	N_("Ignoring requested due to aggressive swarming"),
	N_("Ignoring refused (data too large or server too slow)"),
	N_("Client resource switching (all detected)"),
	N_("Client resource switching between plain files"),
	N_("Client follow-up request after HTTP error was returned"),
	N_("PARQ client resource switching in slots (SHA-1 based)"),
	N_("PARQ client retry-after violation"),
	N_("PARQ client kicked out after too many retry-after violations"),
	N_("PARQ upload slot limit overrides"),
	N_("PARQ quick upload slots granted"),
	N_("PARQ QUEUE sending attempts"),
	N_("PARQ QUEUE messages sent"),
	N_("PARQ QUEUE follow-up requests received"),
	N_("Launched SHA-1 file verifications"),
	N_("Launched TTH file verifications"),
	N_("Re-seeding of orphan downloads through query hits"),
	N_("Re-seeding of orphan downloads through upload requests"),
	N_("RUDP sent bytes"),
	N_("RUDP received bytes"),
	N_("DHT estimated amount of nodes"),
	N_("DHT standard error of estimated amount of nodes"),
	N_("DHT k-ball theoretical frontier (bits)"),
	N_("DHT k-ball furthest frontier (bits)"),
	N_("DHT k-ball closeest frontier (bits)"),
	N_("DHT routing table buckets"),
	N_("DHT routing table leaves"),
	N_("DHT routing table maximum depth"),
	N_("DHT routing table good nodes"),
	N_("DHT routing table stale nodes"),
	N_("DHT routing table pending nodes"),
	N_("DHT routing table evicted nodes"),
	N_("DHT routing table evicted firewalled nodes"),
	N_("DHT routing table evicted nodes due to quota"),
	N_("DHT routing table promoted pending nodes"),
	N_("DHT routing table pinged promoted nodes"),
	N_("DHT routing table rejected node due to bucket network quota"),
	N_("DHT routing table rejected node due to global network quota"),
	N_("DHT completed bucket refreshes"),
	N_("DHT forced bucket refreshes"),
	N_("DHT forced bucket merges"),
	N_("DHT bucket merges"),
	N_("DHT bucket splits"),
	N_("DHT denied non-splitable bucket refresh"),
	N_("DHT initiated bucket alive checks"),
	N_("DHT alive pings sent to good nodes"),
	N_("DHT alive pings sent to stale nodes"),
	N_("DHT alive pings sent to shutdowning nodes"),
	N_("DHT alive pings avoided"),
	N_("DHT alive pings skipped"),
	N_("DHT revitalized stale nodes on RPC reply"),
	N_("DHT value store rejected on IP/network quota grounds"),
	N_("DHT value store rejected on creator validation grounds"),
	N_("DHT nodes rejected during lookup based on network quota"),
	N_("DHT nodes rejected during lookup based on suspicious proximity"),
	N_("DHT nodes rejected during lookup based on frequency divergence"),
	N_("DHT node contact IP addresses fixed during lookup"),
	N_("DHT keys held"),
	N_("DHT cached keys held"),
	N_("DHT values held"),
	N_("DHT cached KUID targets held"),
	N_("DHT cached closest root nodes"),
	N_("DHT cached roots exact hits"),
	N_("DHT cached roots approximate hits"),
	N_("DHT cached roots misses"),
	N_("DHT cached roots lookups within k-ball"),
	N_("DHT cached roots contact address refreshed"),
	N_("DHT cached security tokens held"),
	N_("DHT cached security tokens hits"),
	N_("DHT stable node information held"),
	N_("DHT local hits on value lookups"),
	N_("DHT local hits returning values from cached keys"),
	N_("DHT returned expanded values"),
	N_("DHT returned values as secondary keys"),
	N_("DHT claimed values via secondary keys"),
	N_("DHT returned cached expanded values"),
	N_("DHT returned cached values as secondary-keys"),
	N_("DHT claimed cached values via secondary keys"),
	N_("DHT successfully received value publications"),
	N_("DHT successfully received value removals"),
	N_("DHT replication of stale value avoided"),
	N_("DHT replication of held values"),
	N_("DHT republishing of held values"),
	N_("DHT secondary-key value fetch issued"),
	N_("DHT duplicate values returned in lookups"),
	N_("DHT detected KUID collisions"),
	N_("DHT detected collisions with our own KUID"),
	N_("DHT caching attempts"),
	N_("DHT caching ended successfully"),
	N_("DHT caching partially completed"),
	N_("DHT key-offloading checks after discovering new closest node"),
	N_("DHT keys selected for offloading"),
	N_("DHT key-offloading attempts"),
	N_("DHT key-offloading ended successfully"),
	N_("DHT key-offloading partially completed"),
	N_("DHT values successfully offloaded"),
	N_("DHT incoming messages"),
	N_("DHT incoming messages with UDP-matching contact address"),
	N_("DHT incoming messages with contact address fixed"),
	N_("DHT incoming messages from hostile addresses"),
	N_("DHT incoming messages with hostile contact address"),
	N_("DHT RPC messages prepared"),
	N_("DHT RPC messages cancelled"),
	N_("DHT RPC timed out"),
	N_("DHT RPC replies received"),
	N_("DHT RPC replies with contact address fixed"),
	N_("DHT RPC late replies received"),
	N_("DHT RPC detected KUID mismatches on reply"),
	N_("DHT RPC recent nodes held"),
	N_("DHT node verifications"),
	N_("DHT publishing attempts"),
	N_("DHT publishing ended successfully (all roots)"),
	N_("DHT publishing partially completed (root subset only)"),
	N_("DHT publishing ending with proper value presence"),
	N_("DHT value republishing occurring too late (after expiry)"),
	N_("DHT publishing to self"),
	N_("DHT background publishing completion attempts"),
	N_("DHT background publishing completion showing improvements"),
	N_("DHT background publishing completion successful (all roots)"),
	N_("DHT SHA1 data type collisions"),
	N_("DHT lookup path passively protected against attack"),
	N_("DHT lookup path actively protected against attack"),
	N_("DHT alt-loc lookups issued"),
	N_("DHT push-proxy lookups issued"),
	N_("DHT successful alt-loc lookups"),
	N_("DHT successful push-proxy lookups"),
	N_("DHT successful node push-entry lookups"),
	N_("DHT re-seeding of orphan downloads"),
	N_("Digests computed on general statistics"),
	N_("Digests computed on TCP statistics"),
	N_("Digests computed on UDP statistics"),
};

/**
 * @return the English description of the enum value.
 */
const char *
gnet_stats_general_description(gnr_stats_t x)
{
	if G_UNLIKELY(UNSIGNED(x) >= G_N_ELEMENTS(stats_text)) {
		str_t *s = str_private(G_STRFUNC, 80);
		str_printf(s, "Invalid gnr_stats_t code: %d", (int) x);
		return str_2c(s);
	}

	return stats_text[x];
}

/* vi: set ts=4 sw=4 cindent: */
