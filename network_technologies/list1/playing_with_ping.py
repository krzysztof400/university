import subprocess
import platform
import re
from tabulate import tabulate

def ping(host, packet_size):
    system = platform.system().lower()
    cmd = ["ping", "-c", "4", "-s", str(packet_size), host]
    
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=5)
        return result.stdout
    except subprocess.TimeoutExpired:
        return "Timeout"

def extract_avg_time(output):
    match = re.search(r"rtt min/avg/max/mdev = ([\d.]+)/([\d.]+)/([\d.]+)/([\d.]+)", output)
    return float(match.group(2)) if match else None

def main():
    hosts = ["pwr.edu.pl", "uwr.edu.pl", "esiee.fr", "183.81.128.116"]
    packet_sizes = [32, 64, 128, 256, 512, 1048568, 16777208]
    results = []
    
    for host in hosts:
        for size in packet_sizes:
            output = ping(host, size)
            avg_time = extract_avg_time(output)
            results.append([host, size, avg_time if avg_time is not None else "Error"])
    
    print(tabulate(results, headers=["Host", "Packet Size", "Avg Response Time (ms)"], tablefmt="grid"))

if __name__ == "__main__":
    main()
