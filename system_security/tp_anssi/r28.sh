# BOOT
if mount | grep '^/boot' | grep -q nosuid; then
  echo "OK: nosuid is in effect in /boot"
else
  echo "KO: nosuid is NOT in effect in /boot"
fi

if mount | grep '^/boot' | grep -q nodev; then
  echo "OK: nodev is in effect in /boot"
else
  echo "KO: nodev is NOT in effect in /boot"
fi

if mount | grep '^/boot' | grep -q noexec; then
  echo "OK: noexec is in effect in /boot"
else
  echo "KO: noexec is NOT in effect in /boot"
fi

# OPT
if mount | grep '^/opt' | grep -q nosuid; then
  echo "OK: nosuid is in effect in /opt"
else
  echo "KO: nosuid is NOT in effect in /opt"
fi

if mount | grep '^/opt' | grep -q nodev; then
  echo "OK: nodev is in effect in /opt"
else
  echo "KO: nodev is NOT in effect in /opt"
fi

# TMP
if mount | grep '^/tmp' | grep -q nosuid; then
  echo "OK: nosuid is in effect in /tmp"
else
  echo "KO: nosuid is NOT in effect in /tmp"
fi

if mount | grep '^/tmp' | grep -q nodev; then
  echo "OK: nodev is in effect in /tmp"
else
  echo "KO: nodev is NOT in effect in /tmp"
fi

if mount | grep '^/tmp' | grep -q noexec; then
  echo "OK: noexec is in effect in /tmp"
else
  echo "KO: noexec is NOT in effect in /tmp"
fi

# SRV
if mount | grep '^/srv' | grep -q nosuid; then
  echo "OK: nosuid is in effect in /srv"
else
  echo "KO: nosuid is NOT in effect in /srv"
fi

if mount | grep '^/srv' | grep -q nodev; then
  echo "OK: nodev is in effect in /srv"
else
  echo "KO: nodev is NOT in effect in /srv"
fi

# HOME
if mount | grep '^/home' | grep -q nosuid; then
  echo "OK: nosuid is in effect in /home"
else
  echo "KO: nosuid is NOT in effect in /home"
fi

if mount | grep '^/home' | grep -q nodev; then
  echo "OK: nodev is in effect in /home"
else
  echo "KO: nodev is NOT in effect in /home"
fi

if mount | grep '^/home' | grep -q noexec; then
  echo "OK: noexec is in effect in /home"
else
  echo "KO: noexec is NOT in effect in /home"
fi

# PROC
# hidepid=2?

# USR
if mount | grep '^/usr' | grep -q nodev; then
  echo "OK: nodev is in effect in /usr"
else
  echo "KO: nodev is NOT in effect in /usr"
fi

# VAR
if mount | grep '^/var' | grep -q nosuid; then
  echo "OK: nosuid is in effect in /var"
else
  echo "KO: nosuid is NOT in effect in /var"
fi

if mount | grep '^/var' | grep -q nodev; then
  echo "OK: nodev is in effect in /var"
else
  echo "KO: nodev is NOT in effect in /var"
fi

if mount | grep '^/var' | grep -q noexec; then
  echo "OK: noexec is in effect in /var"
else
  echo "KO: noexec is NOT in effect in /var"
fi

# VAR/LOG
if mount | grep '^/var/log' | grep -q nosuid; then
  echo "OK: nosuid is in effect in /var/log"
else
  echo "KO: nosuid is NOT in effect in /var/log"
fi

if mount | grep '^/var/log' | grep -q nodev; then
  echo "OK: nodev is in effect in /var/log"
else
  echo "KO: nodev is NOT in effect in /var/log"
fi

if mount | grep '^/var/log' | grep -q noexec; then
  echo "OK: noexec is in effect in /var/log"
else
  echo "KO: noexec is NOT in effect in /var/log"
fi

# VAR/TMP
if mount | grep '^/var/tmp' | grep -q nosuid; then
  echo "OK: nosuid is in effect in /var/tmp"
else
  echo "KO: nosuid is NOT in effect in /var/tmp"
fi

if mount | grep '^/var/tmp' | grep -q nodev; then
  echo "OK: nodev is in effect in /var/tmp"
else
  echo "KO: nodev is NOT in effect in /var/tmp"
fi

if mount | grep '^/var/tmp' | grep -q noexec; then
  echo "OK: noexec is in effect in /var/tmp"
else
  echo "KO: noexec is NOT in effect in /var/tmp"
fi