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

#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "note.h"
#include "wav.h"

static void
synth(const struct wav_hdr *hdr,
		struct wav_data *data, double freq,
		unsigned int ms, int amp)
{
	int reps, i;
	unsigned short s;
	double t;

#define pi 3.14159265358979323846

	reps = (hdr->samples_per_sec * hdr->channels * ms) / 1000;
	t = (pi * 2 * freq) / (hdr->samples_per_sec * hdr->channels);

	for (i = 0; i < reps; ++i) {
		s = amp * (pow(sin(pi * t * i), 3) + sin(pi * (t * i + 2.0f / 3)));
		wav_data_add_sample(data, s);
	}
}

int
main(void)
{
	struct wav_hdr hdr;
	struct wav_data data;
	struct scale minor_scale;
	struct note c_note, toplay;

	wav_hdr_init(&hdr);
	wav_data_init(&data);

	srand(time(NULL));
	note_make(&c_note, NOTE_C, 3);
	scale_make(&minor_scale, "whwwhww");

	while (wav_duration(&hdr, &data) < 10000) {
		scale_note(&minor_scale, &c_note, rand() % 8, &toplay);
		synth(&hdr, &data, toplay.freq, 200, 32760);
	}

	wav_hdr_recalculate_file_size(&hdr, &data);
	wav_write(&hdr, &data, "synth.wav");
	wav_data_reset(&data);

	return 0;
}
