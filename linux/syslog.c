/* See LICENSE file for copyright and license details. */
#include <sys/klog.h>
#include <unistd.h>
#include <stdio.h>

enum {
	SYSLOG_ACTION_READ_ALL = 3,
	SYSLOG_ACTION_SIZE_BUFFER = 10
};

int
syslog_size(void)
{
	return klogctl(SYSLOG_ACTION_SIZE_BUFFER, NULL, 0);
}

int
syslog_read(void *buf, size_t n)
{
	return klogctl(SYSLOG_ACTION_READ_ALL, buf, n);
}

int
syslog_show(int fd, const void *buf, size_t n)
{
	int last = '\n';
	const char *p = buf;
	size_t i;

	for (i = 0; i < n; ) {
		if (last == '\n' && p[i] == '<') {
			i += 2;
			if (i + 1 < n && p[i + 1] == '>')
				i++;
		} else {
			if (write(fd, &p[i], 1) != 1)
				return -1;
		}
		last = p[i++];
	}
	return 0;
}
