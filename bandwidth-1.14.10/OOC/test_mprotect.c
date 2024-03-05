
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#include "Double.h"

static void signal_handler(int sig, siginfo_t *si, void *unused)
{
	// printf("Got expected SIGSEGV at address: 0x%lx\n", (unsigned long) si->si_addr);

	deallocateClasses();
	exit (0);
}

int main()
{
	uint32_t value;
	Double *vec;

	// Test new.
	vec = new(Double);

	DoubleClass *class = vec->is_a;

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset (&sa.sa_mask);
	sa.sa_sigaction = signal_handler;
	if (sigaction(SIGSEGV, &sa, NULL) == -1)
        	return 2;

	class->destroy = NULL;
	class->classSize++;

	release(vec);

	// No SIGSEGV means the test failed.
	return 1;
}

