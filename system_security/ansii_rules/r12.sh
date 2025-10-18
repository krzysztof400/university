recommended_options=(
    "net.core.bpf_jit_harden"
    "net.ipv4.ip_forward"
    "net.ipv4.conf.all.accept_local"
    "net.ipv4.conf.all.accept_redirects"
    "net.ipv4.conf.default.accept_redirects"
    "net.ipv4.conf.all.secure_redirects"
    "net.ipv4.conf.default.secure_redirects"
    "net.ipv4.conf.all.shared_media"
    "net.ipv4.conf.default.shared_media"
    "net.ipv4.conf.all.accept_source_route"
    "net.ipv4.conf.default.accept_source_route"
    "net.ipv4.conf.all.arp_filter"
    "net.ipv4.conf.all.arp_ignore"
    "net.ipv4.conf.all.route_localnet"
    "net.ipv4.conf.all.drop_gratuitous_arp"
    "net.ipv4.conf.default.rp_filter"
    "net.ipv4.conf.all.rp_filter"
    "net.ipv4.conf.default.send_redirects"
    "net.ipv4.conf.all.send_redirects"
    "net.ipv4.icmp_ignore_bogus_error_responses"
    "net.ipv4.ip_local_port_range"
    "net.ipv4.tcp_rfc1337"
)

for option in "${recommended_options[@]}"; do
    sysctl $option
done