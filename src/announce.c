/*
 * Announce services using tinysvcmdns
 */

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <signal.h>
#include <stdio.h>
#include "mdns.h"
#include "mdnsd.h"
#include <netdb.h>

int done = 0;

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

	if (argc < 7) {
		printf("Usage: %s 'hello.local' 192.168.0.11 'My Website' '_http._tcp.local' 80 'path=/mywebsite'\n", argv[0]);
		exit(1);
	}

	struct mdnsd *svr = mdnsd_start();
	if (svr == NULL) {
		printf("mdnsd_start() error\n");
		return 1;
	}
	
	char *hostname = argv[1];
	mdnsd_set_hostname(svr, hostname, inet_addr(argv[2]));

	struct rr_entry *a2_e = NULL;
	a2_e = rr_create_a(create_nlabel(hostname), inet_addr(argv[2]));
	mdnsd_add_rr(svr, a2_e);

	const char *txt[] = { argv[6], NULL };
	struct mdns_service *svc = mdnsd_register_svc(svr, argv[3], argv[4], atoi(argv[5]), NULL, txt);
	mdns_service_destroy(svc);

	printf("Announcing service. Press Ctrl-C to exit\n");

	while (!done) sleep(1);

	mdnsd_stop(svr);
        printf("Stopped\n");
	return 0;
}
