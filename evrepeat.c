// SPDX-License-Identifier: GPL-2.0
// evrepeat: get or set input device repeat parameters
//
// Copyright (c) 2019 Matthias Reichl <hias@horus.com>

#define _POSIX_C_SOURCE 2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>

int main(int argc, char** argv)
{
	int ret = 0;
	int delay = -1;
	int period = -1;
	int quiet = 0;
	int f;
	int opt;
	unsigned int rep[2];

	while ((opt = getopt(argc, argv, "d:p:q")) != -1) {
		switch (opt) {
		case 'd':
			delay = atoi(optarg);
			break;
		case 'p':
			period = atoi(optarg);
			break;
		case 'q':
			quiet = 1;
			break;
		default:
			fprintf(stderr, "unknown option '%c'\n", opt);
			goto usage;
		}
	}

	if (optind + 1 != argc)
		goto usage;

	f = open(argv[optind], O_RDWR);
	if (f < 0) {
		fprintf(stderr, "cannot open %s\n", argv[optind]);
		ret = 1;
		goto exit_noclose;
	}

	if (ioctl(f, EVIOCGREP, rep) < 0) {
		perror("evdev ioctl");
		ret = 1;
		goto exit_close;
	}

	if (!quiet) {
		printf("Repeat delay = %u ms, repeat period = %u ms\n",
			rep[0], rep[1]);
	}

	if (delay < 0 && period < 0)
		goto exit_close;

	if (delay >= 0)
		rep[0] = delay;

	if (period >= 0)
		rep[1] = period;

	if (ioctl(f, EVIOCSREP, rep) < 0) {
		perror("evdev ioctl");
		ret = 1;
		goto exit_close;
	}

	if (!quiet) {
		printf("Changed repeat delay to %u ms and repeat period to %u ms\n",
			rep[0], rep[1]);
	}

exit_close:
	close(f);

exit_noclose:

	return ret;

usage:
	fprintf(stderr, "Usage: evrepeat [-q] [-d DELAY] [-p PERIOD] DEVICE\n");
	return 1;
}

