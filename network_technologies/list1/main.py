import subprocess
import re

def ping_host(host, ttl_value):
    try:
        result = subprocess.run(["ping", "-c", "1", "-t", str(ttl_value), host], capture_output=True, text=True, timeout=5)
        output = result.stdout
        
        ttl_match = re.search(r'ttl=(\d+)', output)
        time_match = re.search(r'time=(\d+\.?\d*) ms', output)
        
        if ttl_match and time_match:
            ttl = int(ttl_match.group(1))
            time = float(time_match.group(1))
            return ttl, time
        else:
            return None, None
    except subprocess.TimeoutExpired:
        return None, None

def main():
    hosts = {
        "PWR": "cs.pwr.edu.pl",
        "Wrocław": "www.wroclaw.pl",
        "Kraków": "www.krakow.pl",
        "Warszawa": "um.warszawa.pl",
        "Ukraina": "ukraine.ua",
        "Francja": "esiee.fr",
        "Rosja": "www.gov.ru",
        "Kirgistan": "www.gov.kg",
        "Nigeria": "www.nigeria.gov.ng",
        "Indie": "mcdonline.nic.in",
        "Chiny": "www.gov.cn",
        "Nowa Zelandia": "www.govt.nz",
        "Tongo": "www.gov.to",
        "Chile": "www.gob.cl",
        "RPA": "www.gov.za",
        "Wyspa św. Heleny": "www.sainthelena.gov.sh",
        "USA, Pentagon": "www.defense.gov",
        "USA Wschód": "www.nyc.gov",
        "USA Zachód": "www.lacity.gov",
        "Fiji" : "183.81.128.116"
    }
    
    for location, domain in hosts.items():
        ttl_value = 1
        success = False
        
        while ttl_value <= 50:
            ttl, time = ping_host(domain, ttl_value)
            if ttl and time:
                print(f"{location} - {domain}: ttl={ttl}, czas={time}ms, skoki={ttl_value - 1}")
                success = True
                break
            ttl_value += 1
        
        if not success:
            print(f"{location} - {domain}: Brak odpowiedzi")
            
if __name__ == "__main__":
    main()

