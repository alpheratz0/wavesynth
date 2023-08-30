/*
	Copyright (C) 2023 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License version 2 as published by
	the Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#pragma once

#include <stddef.h>

#define MAX_SCALE_INTERVALS 12

enum {
	NOTE_C,         NOTE_C_SHARP,   NOTE_D,
	NOTE_D_SHARP,   NOTE_E,         NOTE_F,
	NOTE_F_SHARP,   NOTE_G,         NOTE_G_SHARP,
	NOTE_A,         NOTE_A_SHARP,   NOTE_B
};

struct note {
	int octave;
	int note;
	double freq;
};

struct scale {
	size_t num_intervals;
	int intervals[MAX_SCALE_INTERVALS];
};

extern void
note_make(struct note *out, int note, int octave);

extern void
scale_make(struct scale *scale, const char *pattern);

extern void
scale_note(const struct scale *scale,
		const struct note *root, int n,
		struct note *out);
