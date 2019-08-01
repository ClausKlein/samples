#ifdef _WIN32
#    undef UNICODE

// link with Ws2_32.lib
#    pragma comment(lib, "Ws2_32.lib")

#    include <winsock2.h>
#    include <ws2tcpip.h>
#else
#    include <arpa/inet.h> // inet_ntop, inet_pton
#    include <err.h>       // errx
#    include <netdb.h>     // getaddrinfo
#endif

#include <stdint.h> // uint8_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset

//
// see
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms738520(v=vs.85).aspx
//
// Minimum system required are
// Windows Server 2008 or Windows Vista
// http://msdn.microsoft.com/en-us/library/aa383745.aspx
// #define _WIN32_WINNT 0x0600


const char* hexdump(const uint8_t* binbuf, size_t binbuflen)
{
    /* FIXME: this isn't thead-safe! */
    static char hexbuf[INET6_ADDRSTRLEN * 2 + 1];
    // XXX char *hexbuf = new char[INET6_ADDRSTRLEN * 2 + 1];

    size_t i, j = 0;
    size_t len =
        ((binbuflen > INET6_ADDRSTRLEN) ? INET6_ADDRSTRLEN : binbuflen);
    const uint8_t* ibuf = binbuf;
    const char* hexchar = "0123456789abcdef";

    if (NULL == binbuf || 0 == binbuflen) {
        hexbuf[0] = '\0';
        return hexbuf;
    }

    for (i = 0; i < len; i++) {
        hexbuf[j++] = hexchar[(ibuf[i] & 0xf0) >> 4];
        hexbuf[j++] = hexchar[ibuf[i] & 0x0f];
    }
    hexbuf[j] = '\0';
    return hexbuf;
}


int main(int argc, char* argv[])
{

    struct addrinfo hints, *res, *p;
    int error;
    // Provide big enough buffer, ipv6 should be the biggest
    char ipstr[INET6_ADDRSTRLEN];
    uint8_t buf[sizeof(struct in6_addr)];

    if (argc != 2) {
        printf("Usage: %s <hostname>\n", argv[0]);
        printf("Example: %s www.yahoo.com\n", argv[0]);
        return 1;
    }

#ifdef _WIN32
    // Initialization
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        printf("WSAStartup() failed miserably! With error code %ld\n",
            WSAGetLastError());
        return 1;
    } else {
        printf("WSAStartup() looks fine!\n");
    }

#endif

    memset(&hints, 0, sizeof hints);
    hints.ai_family =
        AF_UNSPEC; // TODO: use AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    // TODO: TBD hints.ai_flags = AI_NUMERICHOST;

    if ((error = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
#ifndef _WIN32
        errx(1, "%s", gai_strerror(error));
        /*NOTREACHED*/
#else
        printf("getaddrinfo() failed lor! with error code %ld\n",
            WSAGetLastError());
        WSACleanup();
        return 1;
#endif
    }

    printf("The IP addresses for %s:\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next) {
        void* addr;
        size_t ss_len;
        const char* ipver = "IPv6";

        // Get the pointer to the address itself, different fields in IPv4 and
        // IPv6
        if (p->ai_family == AF_INET) {
            // IPv4
            // TODO:  error: static_cast from 'struct sockaddr *' to 'struct
            // sockaddr_in6 *' is not allowed
            struct sockaddr_in* ipv4 =
                static_cast<struct sockaddr_in*>(addr = p->ai_addr);
            addr   = &(ipv4->sin_addr);
            ss_len = sizeof(ipv4->sin_addr);
            ipver  = "IPv4";
        } else {
            // IPv6
            // TODO:  error: static_cast from 'struct sockaddr *' to 'struct
            // sockaddr_in6 *' is not allowed
            struct sockaddr_in6* ipv6 =
                static_cast<struct sockaddr_in6*>(addr = p->ai_addr);
            addr   = &(ipv6->sin6_addr);
            ss_len = sizeof(ipv6->sin6_addr);
        }

        // Convert the IP to a string and print it
        printf("String format: ");
        // InetNtop(address_family,
        // IP_address_in_network_byte_to_convert_to_a_string,
        //         buffer_to_store_the_IP_address_string,
        //         the_IP_string_length_in_character);
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s: %s\n", ipver, ipstr);

        printf(" Value format: ");
        // InetPton(address_family, string_to_be_converted,
        // buffer_to_store_the_converted_string);
        inet_pton(p->ai_family, ipstr, buf);
        printf("%s: %s\n", ipver, hexdump(buf, ss_len));
    }

    // Deallocate the resource...
    freeaddrinfo(res);

#ifdef _WIN32
    // Cleanup the WSA...
    WSACleanup();
#endif

    return 0;
}
