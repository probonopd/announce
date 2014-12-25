/* 
 * Based on abrasive/shairport/master/mdns_tinysvcmdns.c
 * Copyright (c) 2013 Paul Lietar
 * Copyright (c) 2015 Simon Peter
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include "tinysvcmdns.h"
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <net/if.h> 
#include <unistd.h>
#include <netinet/in.h>

int done = 0;

static struct mdnsd *svr = NULL;

void term(int signum)
{
    printf("\nReceived SIGTERM, exiting...\n");

    done = 1;
}


int main(int argc, char *argv[]) {

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL); // Catch kill, killall
    sigaction(SIGINT, &action, NULL);  // Catch Ctrl-C

    struct ifaddrs *ifalist;
    struct ifaddrs *ifa;

    svr = mdnsd_start();
    if (svr == NULL) {
        printf("tinysvcmdns: mdnsd_start() failed\n");
        return -1;
    }

    /* NOTE: There is a bug: If the hostname is 6 characters or less, then it does not work.
     */

    // room for name + .local + NULL
    char hostname[100 + 6];
    gethostname(hostname, 99);
    // according to POSIX, this may be truncated without a final NULL
    hostname[99] = 0;

    // will not work if the hostname doesn't end in .local
    char *hostend = hostname + strlen(hostname);
    if ((strlen(hostname) > 6) &&
        strcmp(hostend - 6, ".local"))
    {
        strcat(hostname, ".local");
    }


    if (getifaddrs(&ifalist) < 0)
    {
        printf("tinysvcmdns: getifaddrs() failed\n");
        return -1;
    }

    ifa = ifalist;

    // Look for an ipv4/ipv6 non-loopback interface to use as the main one.
    for (ifa = ifalist; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!(ifa->ifa_flags & IFF_LOOPBACK) && ifa->ifa_addr &&
            ifa->ifa_addr->sa_family == AF_INET)
        {
            uint32_t main_ip = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;

            mdnsd_set_hostname(svr, hostname, main_ip);
            break;
        }
        else if (!(ifa->ifa_flags & IFF_LOOPBACK) && ifa->ifa_addr &&
                 ifa->ifa_addr->sa_family == AF_INET6)
        {
            // ipv6 unimplemented
        }
    }

    if (ifa == NULL)
    {
        printf("tinysvcmdns: no non-loopback interface found\n");
        return -1;
    }


    // Skip the first one, it was already added by set_hostname
    for (ifa = ifa->ifa_next; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_flags & IFF_LOOPBACK) // Skip loop-back interfaces
            continue;

        switch (ifa->ifa_addr->sa_family)
        {
            case AF_INET: { // ipv4
                    uint32_t ip = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;
                    struct rr_entry *a_e = rr_create_a(create_nlabel(hostname), ip);
                    mdnsd_add_rr(svr, a_e);
                }
                break;
            case AF_INET6: { // ipv6
                    struct in6_addr *addr = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                    struct rr_entry *aaaa_e = rr_create_aaaa(create_nlabel(hostname), addr);
                    mdnsd_add_rr(svr, aaaa_e);
                }
                break;
        }
    }

    freeifaddrs(ifa);

    const char *txt0[] = { "path=/", NULL };
    struct mdns_service *svc0 = mdnsd_register_svc(svr,
                                hostname,
                                "_http._tcp.local",
                                80,
                                NULL,
                                txt0);
    mdns_service_destroy(svc0);

    const char *txt1[] = { "", NULL };
    struct mdns_service *svc1 = mdnsd_register_svc(svr,
                                hostname,
                                "_ssh._tcp.local",
                                22,
                                NULL,
                                txt1);
    mdns_service_destroy(svc1);

    printf("Announcing services. Press Ctrl-C to exit\n");

    while (!done) sleep(1);

    mdnsd_stop(svr);
    printf("Stopped announcing services.\n");
    return 0;
}
