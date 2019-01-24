# network_programming__dnsc
simple DNS client that is able to send a DNS query with QTYPE A(a host address) to specified DNS server

# 0. purpose
- to learn about RFC 1035: "DOMAIN NAMES - IMPLEMENTATION AND SPECIFICATION" ( https://www.ietf.org/rfc/rfc1035.txt )
- to implement a simple DNS client tool that is able to send a DNS query with QTYPE A(a host address) to specified DNS server
- https://tools.ietf.org/html/rfc2151

**dnsc:**
```
        ---------------------------------------------------------------------------------------------
        > This tool is able to send a DNS query with QTYPE A(a host address) to specified DNS server
        > This tool is built based on RFC 1035: "DOMAIN NAMES - IMPLEMENTATION AND SPECIFICATION"
        ---------------------------------------------------------------------------------------------
        > An example on how to use:
                > dnsc --addr=8.8.8.8 --id=10 --rd=1 --qname="www.google.com"
                         Where:
                         --addr : IPv4 DNS Server Address
                         --id   : "identifier" in the DNS header
                         --rd   : "recursion desired" iun the DNS header
                         --qname: "domain name" in the Question Part of DNS message
        ---------------------------------------------------------------------------------------------
        > DNS Query:
                                  0  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1
                                  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |                      ID                       |
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |                    QDCOUNT                    |
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |                    ANCOUNT                    |
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |                    NSCOUNT                    |
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |                    ARCOUNT                    |
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |                                               |
                                /                     QNAME                     /
                                /                                               /
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |                     QTYPE                     |
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
                                |                     QCLASS                    |
                                +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        ---------------------------------------------------------------------------------------------
        Nguyen Dai Son
```

# 1. dnsc
**1.1 project structure:**
```
        network_programming__dnsc/
        ├── build_unix.sh
        ├── build_windows.bat
        ├── CMakeLists.txt
        ├── README.md
        └── src
                ├── dnsc.cpp
                ├── dnsc.hpp
                ├── main.cpp
                ├── mbuf.cpp
                └── mbuf.hpp
```
**1.2 this tool is able to run on Windows**

# 2. sample output
**2.1 sample output on Ubuntu**
```
$ ./dnsc --addr=8.8.8.8 --id=10 --rd=1 --qname="www.daisonit.com"
0x0000: 000a 0100 0001 0000 0000 0000 0377 7777 [.............www]
0x0010: 0864 6169 736f 6e69 7403 636f 6d00 0001 [.daisonit.com...]
0x0020: 0001 0000 0000 0000 1104 0000 0000 0000 [...]


# tcpdump -i eth0 port 53 and host 8.8.8.8 -XX -c 2
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth0, link-type EN10MB (Ethernet), capture size 262144 bytes
------------------deleted-------------------------------: 10+ A? www.daisonit.com. (34)
        0x0000:  xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  xxxxxxxxxxxxxxxx
        0x0010:  xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  xxxxxxxxxxxxxxxx
        0x0020:  xxxx xxxx xxxx xxxx xxxx 000a 0100 0001  xxxxxxxxxxxxxxxx
        0x0030:  0000 0000 0000 0377 7777 0864 6169 736f  .......www.daiso
        0x0040:  6e69 7403 636f 6d00 0001 0001            nit.com.....
------------------deleted-------------------------------: 10 1/0/0 A 68.66.207.35 (50)
        0x0000:  xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  xxxxxxxxxxxxxxxx
        0x0010:  xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx  xxxxxxxxxxxxxxxx
        0x0020:  xxxx xxxx xxxx xxxx xxxx 000a 8180 0001  xxxxxxxxxxxxxxxx
        0x0030:  0001 0000 0000 0377 7777 0864 6169 736f  .......www.daiso
        0x0040:  6e69 7403 636f 6d00 0001 0001 c00c 0001  nit.com.........
        0x0050:  0001 0000 0690 0004 4442 cf23            ........DB.#
2 packets captured
2 packets received by filter
0 packets dropped by kernel
56 packets dropped by interface
```

