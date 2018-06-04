#include <sys/file.h>
#include <stdio.h>
#include <string.h>
#include "plcm_ioctl.h"

#define MAX_CHARS 20
#define INTERVAL 250000
#define LINE2_INTERVAL 10

void copy(unsigned char *source, unsigned char *destination, int index)
{
	unsigned int i;
	for (i = 0; i < MAX_CHARS; i++)
	{
		destination[i] = source[(index + i) % strlen(source)];
	}
}

int main(int argc, char *argv[])
{
	int devfd;
	unsigned char messageLine1[]  = "TIS/Axsh booth 525G http://www.tis.co.jp/ http://axsh.jp/ ";
	unsigned char *messageLine2[] = {
		"11th,Jun LiquidMetal",
		" is now RELEASED !! ",
		"Visit booth 5G25 !!!"
	};
	unsigned char line1buf[MAX_CHARS + 1] = {0};
	unsigned char line2buf[MAX_CHARS + 1] = {0};
	unsigned long line2count = sizeof(messageLine2) / sizeof(messageLine2[0]);

	devfd = open("/dev/plcm_drv", O_RDWR);
	if (devfd == -1)
	{
		printf("Can't open /dev/plcm_drv\n");
		return -1;
	}

	ioctl(devfd, PLCM_IOCTL_CLEARDISPLAY, 0);
	ioctl(devfd, PLCM_IOCTL_DISPLAY_D, 1);
	ioctl(devfd, PLCM_IOCTL_SHIFT_SC, 0);
	ioctl(devfd, PLCM_IOCTL_SET_LINE, 1);
	ioctl(devfd, PLCM_IOCTL_DISPLAY_B, 0);
	ioctl(devfd, PLCM_IOCTL_DISPLAY_C, 0);
	ioctl(devfd, PLCM_IOCTL_SET_LINE, 2);
	ioctl(devfd, PLCM_IOCTL_DISPLAY_B, 0);
	ioctl(devfd, PLCM_IOCTL_DISPLAY_C, 0);
	ioctl(devfd, PLCM_IOCTL_BACKLIGHT, 1);

	unsigned long i = 0, j = 0;
	for (;; i ++, j ++) {
		int line2index = (j / LINE2_INTERVAL) % line2count;

		copy(messageLine1, line1buf, i);
		copy(messageLine2[line2index], line2buf, 0);

		ioctl(devfd, PLCM_IOCTL_SET_LINE, 1);
		write(devfd, line1buf, MAX_CHARS);
		ioctl(devfd, PLCM_IOCTL_SET_LINE, 2);
		write(devfd, line2buf, MAX_CHARS);

		usleep(INTERVAL);

		if (i == strlen(messageLine1)) {
			i = 0;
		}
		if (j == (LINE2_INTERVAL * line2count)) {
			j = 0;
		}
	}

	close(devfd);
	return 0;
}
