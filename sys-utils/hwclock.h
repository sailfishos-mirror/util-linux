/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#ifndef HWCLOCK_CLOCK_H
#define HWCLOCK_CLOCK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "c.h"
#include "debug.h"
#include "nls.h"

#define HWCLOCK_DEBUG_INIT		(1 << 0)
#define HWCLOCK_DEBUG_RANDOM_SLEEP	(1 << 1)
#define HWCLOCK_DEBUG_DELTA_VS_TARGET	(1 << 2)
#define HWCLOCK_DEBUG_ALL		0xFFFF

UL_DEBUG_DECLARE_MASK(hwclock);
#define DBG(m, x)	__UL_DBG(hwclock, HWCLOCK_DEBUG_, m, x)
#define ON_DBG(m, x)	__UL_DBG_CALL(hwclock, HWCLOCK_DEBUG_, m, x)

struct hwclock_control {
	char *date_opt;
	char *adj_file_name;
	double rtc_delay;	/* --delay <seconds> */
#if defined(__linux__) && defined(__alpha__)
	char *epoch_option;
#endif
#if defined(__linux__) || defined(__GNU__)
	char *rtc_dev_name;
#endif
#ifdef __linux__
	uint32_t param_idx;	/* --param-index <n> */
#endif
	char *param_get_option;
	char *param_set_option;
	bool	hwaudit_on,
		adjust,
		show,
		hctosys,
		utc,
		systohc,
#if defined(__linux__) && defined(__alpha__)
		getepoch,
		setepoch,
#endif
		noadjfile,
		local_opt,
		directisa,
		testing,
		systz,
		predict,
		get,
		set,
		update,
		universal,	/* will store hw_clock_is_utc() return value */
		vl_read,
		vl_clear,
		verbose;
};

struct clock_ops {
	char *interface_name;
	int (*get_permissions) (void);
	int (*read_hardware_clock) (const struct hwclock_control *ctl, struct tm * tm);
	int (*set_hardware_clock) (const struct hwclock_control *ctl, const struct tm * tm);
	int (*synchronize_to_clock_tick) (const struct hwclock_control *ctl);
	const char *(*get_device_path) (void);
};

extern const struct clock_ops *probe_for_cmos_clock(void);
extern const struct clock_ops *probe_for_rtc_clock(const struct hwclock_control *ctl);

/* rtc.c */
#if defined(__linux__) && defined(__alpha__)
extern int get_epoch_rtc(const struct hwclock_control *ctl, unsigned long *epoch);
extern int set_epoch_rtc(const struct hwclock_control *ctl);
#endif

struct hwclock_param {
	int id;
	const char *name;
	const char *help;
};

extern const struct hwclock_param *get_hwclock_params(void);
extern int get_param_rtc(const struct hwclock_control *ctl,
			const char *name, uint64_t *id, uint64_t *value);
extern int set_param_rtc(const struct hwclock_control *ctl, const char *name);

extern int rtc_vl_read(const struct hwclock_control *ctl);
extern int rtc_vl_clear(const struct hwclock_control *ctl);

extern void __attribute__((__noreturn__))
hwclock_exit(const struct hwclock_control *ctl, int status);

extern int parse_date(struct timespec *, char const *, struct timespec const *);

#endif				/* HWCLOCK_CLOCK_H */
