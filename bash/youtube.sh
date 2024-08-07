sudo ./nfqws --qnum=200 --dpi-desync=disorder2 --dpi-desync-split-pos=1 --hostlist=youtube-domain.txt &
sudo iptables -I OUTPUT -o wlp0s20f3 -p tcp --dport 443 -m connbytes --connbytes-dir=original --connbytes-mode=packets --connbytes 1:6 -m mark ! --mark 0x40000000/0x40000000 -j NFQUEUE --queue-num 200 --queue-bypass
sudo ip6tables -I OUTPUT -o wlp0s20f3 -p tcp --dport 443 -m connbytes --connbytes-dir=original --connbytes-mode=packets --connbytes 1:6 -m mark ! --mark 0x40000000/0x40000000 -j NFQUEUE --queue-num 200 --queue-bypass

