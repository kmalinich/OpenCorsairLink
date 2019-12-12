/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017-2019  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "device.h"
#include "driver.h"
#include "logic/options.h"
#include "logic/scan.h"
#include "print.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int hydro_asetekpro_settings(struct corsair_device_scan scanned_device, struct option_flags flags, struct option_parse_return settings);

int main(int argc, char* argv[]) {
	int rr; // result from libusb functions

	int8_t device_number = 0;

	int scanlist_count = 0;

	struct option_flags flags;
	struct option_parse_return settings;

	options_parse(argc, argv, &flags, &device_number, &settings);

	msg_debug("[DBUG] [main.c] main() :: Start\n");

	libusb_context* context = NULL;

	rr = libusb_init(&context);
	if (rr < 0) {
		msg_info("[INFO] Init error %d\n", rr);
		return 1;
	}

	// rr = libusb_set_option( context, LIBUSB_OPTION_LOG_LEVEL, 2 );

	corsairlink_device_scanner(context, &scanlist_count);
	msg_debug("[DBUG] [main.c] main() :: Scan done, starting routines for device_number = %d\n", device_number);

	if (device_number < 0) return 0;

	if (device_number >= scanlist_count) {
		msg_info("[INFO] Detected %d device(s), submitted device %d is out of range\n", scanlist_count, device_number);
		return 0;
	}

	if (scanlist[device_number].device->driver == &corsairlink_driver_asetekpro) {
		hydro_asetekpro_settings(scanlist[device_number], flags, settings);
		return 0;
	}

	corsairlink_close(context);
	return 0;
}
