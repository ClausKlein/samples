#include <stdlib.h> // exit
#include <string.h> // memcpy
#include <stdio.h>  // printf, puts
#include <stdint.h> // uint32_t, uint8_t
#include <arpa/inet.h>  // htonl, ntohl, inet_ntop, inet_pton

#include <assert.h>
//XXX #include <algorithm>    // std::swap

//
//
// FIXME: The problem of host byte ordering versus network byte ordering is confusing!
//
//see too
// http://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c
//
// most of this demo program is ansi c;
// only one template is used for test

inline int isLittleEndian() {
    const int i = 1;
    const char *p = (const char *) &i;
    if (p[0] == 1) {    // Lowest address contains the least significant byte
        return 1; // true;
    } else {
        return 0; // false;
    }
}


typedef union {
    uint32_t i;
    uint8_t c[4];
} u;


#ifndef __cplusplus
/**
 * same ase gcc __builtin_bswap32()
 **/
uint32_t bswap32(uint32_t i) {
    uint8_t *c = (uint8_t *) &i;
    return * (uint32_t *)(uint8_t[4]) {
        c[3], c[2], c[1], c[0]
    };
// FIXME byteorder.c:44: warning: ISO C90 forbids compound literals
// FIXME byteorder.cpp|44 col 5| error: taking address of temporary array (c++11)
}
#endif


#ifdef __cplusplus
/**
 * usage: swap_endian<uint32_t>(42).
 * generic version of gcc __builtin_bswap32()
 **/
template <typename T>
T swap_endian(T u) {
    union {
        T u;
        uint8_t u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++) {
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    }

    return dest.u;
}
#endif


const char *hexdump(const uint8_t *binbuf, size_t binbuflen) {
    /* FIXME: this isn't thead-safe! */
    static char hexbuf[INET6_ADDRSTRLEN * 2 + 1];
    size_t i, j = 0;
    size_t len = ((binbuflen >
                   INET6_ADDRSTRLEN) ? INET6_ADDRSTRLEN : binbuflen);
    const uint8_t *ibuf = binbuf;
    const char *hexchar = "0123456789abcdef";

    if (NULL == binbuf || 0 == binbuflen) {
        return "";
    }

    for (i = 0; i < len; i++) {
        hexbuf[j++] = hexchar[(ibuf[i] & 0xf0) >> 4];
        hexbuf[j++] = hexchar[ibuf[i] & 0x0f];
    }
    hexbuf[j] = '\0';
    return hexbuf;
}

// static void _dump(u temp, int swap = 0);
//FIXME error: C does not support default arguments

/// local helper
static void _dump(u temp, int swap) {
    char addrstr[INET_ADDRSTRLEN] = {0};
    uint32_t in_addr = 0;

    // =======================================
    // swap bytes to simulate other byte order
    // =======================================
    if (swap) {
#ifdef __cplusplus
        temp.i = swap_endian<uint32_t>(temp.i);
#else
        temp.i = bswap32(temp.i);
#endif
    }

    in_addr = temp.i;  //XXX should be htonl(temp.i); but not used for demo! ck
    //TODO convert to network byte order on LittleEndian machine
    if (!swap && isLittleEndian()) {
#ifdef __cplusplus
        in_addr = swap_endian<uint32_t>(in_addr);
#else
        in_addr = bswap32(in_addr);
#endif
    }

    // The function inet_ntop() converts an address *src from network format
    // (usually a struct in_addr or some other binary form, in network byte
    // order) to presentation format (suitable for external display purposes).
    inet_ntop(AF_INET, &in_addr, addrstr, sizeof(addrstr));

    printf("   as ipv4 addr: inet_ntop(%010u): '%s'\n", ntohl(in_addr), addrstr);
    printf("bytes in memory: 0x%02x 0x%02x 0x%02x 0x%02x\t", temp.c[0], temp.c[1], temp.c[2], temp.c[3]);
    printf("'%c%c%c%c'\n", temp.c[0], temp.c[1], temp.c[2], temp.c[3]); // interpreted as ASCIi

    if (inet_pton(AF_INET, addrstr, &in_addr) == 0) {
        assert(temp.i == in_addr);
    }
}

/// local wrapper to ntohl() to prevent casts
static inline uint32_t _ntohl(uint8_t *p) {
    u tmp;

    assert(p);
    memcpy(&tmp.c[0], p, sizeof(tmp.i));

    return ntohl(tmp.i);
}



int main(int argc, char **argv) {
    u temp;

    //XXX temp.i = 0x31323334;
    //FIXME temp.i = * (uint32_t *)(uint8_t[4]) { 'A', 'B', 'C', 'D' };
    //FIXME byteorder.c:44: warning: ISO C90 forbids compound literals

    uint8_t buf[sizeof(struct in6_addr)];
    char str[INET6_ADDRSTRLEN] = "fe80::1";
    char * ptr = argc > 1 ? argv[1] : str;  // use 1. arg if given
    int protocol = AF_INET;
    int i;
    char * last, *addr;

    // determine if ipv6 addess used:
    if (strchr(ptr, ':') != NULL) {
        protocol = AF_INET6;
    }

    // split prefix or scope from ipaddr string
    addr = strtok_r(ptr, "/%", &last);

    // The inet_pton() function converts a presentation format address (that
    // is, printable form as held in a character string) to network format
    // (usually a struct in_addr or some other internal binary representation,
    // in network byte order).
    if (!addr || inet_pton(protocol, addr, buf) != 1) {
        perror("inet_pton()");
        exit(1);    //FIXME must be handeld right!
    }

    {
        // show byteorder for port
        const uint16_t port = 162;
        uint16_t sin_port = htons(port);
        printf("sin_port(162)=0x%s\n", hexdump((uint8_t *)&sin_port, sizeof(sin_port)));
        assert(port == ntohs(sin_port));
#if 1
        buf[0] = (ntohs(sin_port) & 0xff00) >> 8; // high byte
        buf[1] = (ntohs(sin_port) & 0x00ff) >> 0; //  low byte
#else
        buf[0] = (htons(sin_port) & 0xff00) >> 8; // high byte
        buf[1] = (htons(sin_port) & 0x00ff) >> 0; //  low byte
#endif
        printf("sin_port(162)=0x%s\n", hexdump(buf, 2));
        uint16_t porttmp = uint16_t(buf[0] << 8) + uint16_t(buf[1]);    // ntohs()
        assert(port == porttmp);
    }

    // inet_pton('fe80:0000:0000:0000:0000:0000:0000:0000'): 0xfe800000000000000000000000000001
    printf("strlen: %lu inet_pton('%s'): 0x%s\n", strlen(addr), addr, hexdump(buf, (protocol == AF_INET ? 4 : 16)));

    //NOTE: The first 4 bytes has to be used as 1. long paramater of an IPv6 addr!
    //  =>  The byte with the most significant bit at network byte order!
    for (i = 0; i < 4; i++) {

        //NOTE: It is very important to convert it back to host byte order
        //      before print to preset or send via corba!
        temp.i =  _ntohl(&buf[4 * i]);
        printf("P%i: 0x%08x = %010u of (%s)\n", i + 1, temp.i, temp.i, addr);

        if (argc == 1) {
            temp.i =  0x31323334;   // use if only program name is given
        }

        if (isLittleEndian()) {
            puts("we are LittleEndian:");
            _dump(temp, 0);

            puts("       as BigEndian:");
            _dump(temp, 1);      //XXX swap to BigEndian
        } else {
            puts("we are BigEndian:");
            _dump(temp, 0);

            puts(" as LittleEndian:");
            _dump(temp, 1);      //XXX swap to LittleEndian

        }

#ifndef __cplusplus
        //XXX test only
        uint32_t swapped1 = bswap32(temp.i);
        uint32_t swapped2 = swap_endian<uint32_t>(temp.i);
        assert(swapped1 == swapped2);
#endif

        if (argc == 1 || protocol == AF_INET) {
            break;  // done
        }

        puts("");
    }   // end for each ipaddr part

    return 0;
}

/**

clausklein$ ./byteorder
P1: 0xfe800000 = 4269801472 (fe80::1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(0825373492): '49.50.51.52'
bytes in memory: 0x34 0x33 0x32 0x31    '4321'
       as BigEndian:
   as ipv4 addr: inet_ntop(0825373492): '49.50.51.52'
bytes in memory: 0x31 0x32 0x33 0x34    '1234'


clausklein$ ./byteorder ::ffff:127.0.0.1
P1: 0x00000000 = 0000000000 (::ffff:127.0.0.1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(0000000000): '0.0.0.0'
bytes in memory: 0 0 0 0    ''
       as BigEndian:
   as ipv4 addr: inet_ntop(0000000000): '0.0.0.0'
bytes in memory: 0 0 0 0    ''

P2: 0x00000000 = 0000000000 (::ffff:127.0.0.1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(0000000000): '0.0.0.0'
bytes in memory: 0 0 0 0    ''
       as BigEndian:
   as ipv4 addr: inet_ntop(0000000000): '0.0.0.0'
bytes in memory: 0 0 0 0    ''

P3: 0x0000ffff = 0000065535 (::ffff:127.0.0.1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(0000065535): '0.0.255.255'
bytes in memory: 0xff 0xff 0 0  '??'
       as BigEndian:
   as ipv4 addr: inet_ntop(0000065535): '0.0.255.255'
bytes in memory: 0 0 0xff 0xff  '??'

P4: 0x7f000001 = 2130706433 (::ffff:127.0.0.1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(2130706433): '127.0.0.1'
bytes in memory: 0x1 0 0 0x7f   ''
       as BigEndian:
   as ipv4 addr: inet_ntop(2130706433): '127.0.0.1'
bytes in memory: 0x7f 0 0 0x1   ''


clausklein$ ifconfig en1
en1: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    ether 00:23:6c:8c:45:b1
    inet6 fe80::223:6cff:fe8c:45b1%en1 prefixlen 64 scopeid 0x6
    inet 192.168.1.154 netmask 0xffffff00 broadcast 192.168.1.255
    inet6 fd00::223:6cff:fe8c:45b1 prefixlen 64 autoconf
    media: autoselect
    status: active


clausklein$ ./byteorder fd00::223:6cff:fe8c:45b1
P1: 0xfd000000 = 4244635648 (fd00::223:6cff:fe8c:45b1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(4244635648): '253.0.0.0'
bytes in memory: 0 0 0 0xfd '?'
       as BigEndian:
   as ipv4 addr: inet_ntop(4244635648): '253.0.0.0'
bytes in memory: 0xfd 0 0 0 '?'

P2: 0x00000000 = 0000000000 (fd00::223:6cff:fe8c:45b1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(0000000000): '0.0.0.0'
bytes in memory: 0 0 0 0    ''
       as BigEndian:
   as ipv4 addr: inet_ntop(0000000000): '0.0.0.0'
bytes in memory: 0 0 0 0    ''

P3: 0x02236cff = 0035876095 (fd00::223:6cff:fe8c:45b1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(0035876095): '2.35.108.255'
bytes in memory: 0xff 0x6c 0x23 0x2 '?l#'
       as BigEndian:
   as ipv4 addr: inet_ntop(0035876095): '2.35.108.255'
bytes in memory: 0x2 0x23 0x6c 0xff '#l?'

P4: 0xfe8c45b1 = 4270605745 (fd00::223:6cff:fe8c:45b1)
we are LittleEndian:
   as ipv4 addr: inet_ntop(4270605745): '254.140.69.177'
bytes in memory: 0xb1 0x45 0x8c 0xfe    '?E??'
       as BigEndian:
   as ipv4 addr: inet_ntop(4270605745): '254.140.69.177'
bytes in memory: 0xfe 0x8c 0x45 0xb1    '??E?'

 **/
