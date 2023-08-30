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

#include <stddef.h>
#include <string.h>

#include "note.h"

/* src: https://pages.mtu.edu/~suits/notefreq432.html */
static const double notes[][12] = {
	/*        C       C#        D       D#        E        F       F#        G       G#        A       A#        B  */
	{     16.05,   17.01,   18.02,   19.09,   20.23,   21.43,   22.70,   24.05,   25.48,   27.00,   28.61,   30.31   },
	{     32.11,   34.02,   36.04,   38.18,   40.45,   42.86,   45.41,   48.11,   50.97,   54.00,   57.21,   60.61   },
	{     64.22,   68.04,   72.08,   76.37,   80.91,   85.72,   90.82,   96.22,  101.94,  108.00,  114.42,  121.23   },
	{    128.43,  136.07,  144.16,  152.74,  161.82,  171.44,  181.63,  192.43,  203.88,  216.00,  228.84,  242.45   },
	{    256.87,  272.14,  288.33,  305.47,  323.63,  342.88,  363.27,  384.87,  407.75,  432.00,  457.69,  484.90   },
	{    513.74,  544.29,  576.65,  610.94,  647.27,  685.76,  726.53,  769.74,  815.51,  864.00,  915.38,  969.81   },
	{   1027.47, 1088.57, 1153.30, 1221.88, 1294.54, 1371.51, 1453.07, 1539.47, 1631.01, 1728.00, 1830.75, 1939.61   },
	{   2054.95, 2177.14, 2306.60, 2443.76, 2589.07, 2743.03, 2906.14, 3078.95, 3262.03, 3456.00, 3661.50, 3879.23   },
	{   4109.90, 4354.29, 4613.21, 4887.52, 5178.15, 5486.06, 5812.28, 6157.89, 6524.06, 6912.00, 7323.01, 7758.46   },
};

extern void
note_make(struct note *out, int note, int octave)
{
	out->note = note % 12;
	out->octave = octave + note / 12;
	out->freq = notes[out->octave][out->note];
}

extern void
scale_make(struct scale *scale, const char *pattern)
{
	size_t i, pattern_len;

	pattern_len = strlen(pattern);
	scale->num_intervals = pattern_len;

	if (scale->num_intervals > MAX_SCALE_INTERVALS)
		return;

	for (i = 0; i < pattern_len; ++i) {
		switch (pattern[i]) {
		case 'h': case 'H': scale->intervals[i] = 1; break;
		case 'w': case 'W': scale->intervals[i] = 2; break;
		}
	}
}

extern void
scale_note(const struct scale *scale,
		const struct note *root, int n,
		struct note *out)
{
	size_t interval;
	int note, octave;

	note = root->note;
	octave = root->octave;

	for (interval = 0; n > 0; --n, ++interval) {
		interval %= scale->num_intervals;
		note += scale->intervals[interval];
	}

	note_make(out, note, octave);
}
