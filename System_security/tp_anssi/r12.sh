#!/bin/bash

# Define IPv4 configurations
r12_configurations=("core.bpf_jit_harden" "ipv4.ip_forward" "ipv4.conf.all.accept_local" "ipv4.conf.all.accept_redirects"
 "ipv4.conf.default.accept_redirects" "ipv4.conf.all.secure_redirects" "ipv4.conf.default.secure_redirects" "ipv4.conf.all.shared_media"
  "ipv4.conf.default.shared_media" "ipv4.conf.all.accept_source_route" "ipv4.conf.default.accept_source_route" "ipv4.conf.all.arp_filter"
  "ipv4.conf.all.arp_ignore" "ipv4.conf.all.route_localnet" "ipv4.conf.all.drop_gratuitous_arp" "ipv4.conf.default.rp_filter"
  "ipv4.conf.all.rp_filter" "ipv4.conf.default.send_redirects" "ipv4.conf.all.send_redirects" "ipv4.icmp_ignore_bogus_error_responses"
  "ipv4.ip_local_port_range" "ipv4.tcp_rfc1337" "ipv4.tcp_syncookies")
# Define prefered values
r12_expected_vals=(2 0 0 0 0 0 0 0 0 0 0 1 2 0 1 1 1 0 0 1 32768 1 1)
# used for if you only want to print a single OK or KO
r12_outputs=()
config_name="net"

# Check if the IPv4 configurations are set to the prefered values
for i in "${!r12_configurations[@]}"; do
  config="${r12_configurations[$i]}"
  actual_value=$(sysctl -n "$config_name.$config" 2>/dev/null)
  expected_value="${r12_expected_vals[$i]}"
  if [[ "$actual_value" == "$expected_value" ]]; then
    echo "OK: $config_name.$config is $actual_value"
    r12_outputs+=("$0")
  else
    # this has to be here because the value of pid_max is not fixed
    if [[ "$config" == "pid_max" ]]; then
      echo "OK: $config_name.$config is $actual_value (value has to be only specified)"
      r12_outputs+=("$0")
    else
      echo "KO: $config_name.$config is $actual_value (expected $expected_value)"
      r12_outputs+=("$1")
    fi
  fi
done