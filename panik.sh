if ! command -v insmod >/dev/null 2>&1; then
    echo "ERROR: insmod not found"
    exit 1
fi

if [ "$(id -u)" -ne 0 ]; then
    echo "ERROR: not root"
    exit 1
fi

if [ -z "$1" ]; then
    echo "ERROR: no panik message specified"
    exit 1
fi

printf "$@" > /tmp/panik

echo "$PANIK_B64"|base64 -d>/tmp/panik.ko
insmod /tmp/panik.ko

sleep 1

echo "ERROR: kernel did not panic"
dmesg
exit 1
