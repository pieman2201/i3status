#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "i3status.h"

/*
 * Just parses /proc/net/wireless looking for lines beginning with
 * wlan_interface, extracting the quality of the link and adding the
 * current IP address of wlan_interface.
 *
 */
const char *get_wireless_info() {
        char buf[1024];
        static char part[512];
        char *interfaces;
        memset(buf, 0, sizeof(buf));
        memset(part, 0, sizeof(part));

	slurp("/proc/net/wireless", buf, sizeof(buf));

        interfaces = skip_character(buf, '\n', 1) + 1;
        while ((interfaces = skip_character(interfaces, '\n', 1)+1) < buf+strlen(buf)) {
                while (isspace((int)*interfaces))
                        interfaces++;
                if (!BEGINS_WITH(interfaces, wlan_interface))
                        continue;
                int quality;
                if (sscanf(interfaces, "%*[^:]: 0000 %d", &quality) != 1)
                        continue;
                if ((quality == UCHAR_MAX) || (quality == 0)) {
                        if (use_colors)
                                (void)snprintf(part, sizeof(part), "%sW: down", color("#FF0000"));
                        else (void)snprintf(part, sizeof(part), "W: down");
                } else (void)snprintf(part, sizeof(part), "%sW: (%03d%%) %s",
                                color("#00FF00"), quality, get_ip_addr(wlan_interface));
                return part;
        }

        return part;
}

