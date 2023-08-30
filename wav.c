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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "wav.h"


#define memcpy_adv(dest, src, n) do { \
	memcpy(dest, src, n); \
	dest += n; \
} while (0)

////////////////////////////// TODO(alpheratz):
////////////////////////////// stereo support
extern void
wav_hdr_init(struct wav_hdr *hdr)
{
	hdr->group_id = "RIFF";
	hdr->file_size = 0;
	hdr->riff_type = "WAVE";
	hdr->chunk_id = "fmt ";
	hdr->chunk_size = 16;
	hdr->format_tag = 1;
	hdr->channels = 1;
	hdr->samples_per_sec = 44100;
	hdr->bits_per_sample = 16;
	hdr->block_align = hdr->channels * (hdr->bits_per_sample / 8);
	hdr->avg_bytes_per_sec = hdr->samples_per_sec * hdr->block_align;
}

extern size_t
wav_hdr_size(const struct wav_hdr *hdr)
{
	return 0
		+ strlen(hdr->group_id)
		+ sizeof(hdr->file_size)
		+ strlen(hdr->riff_type)
		+ strlen(hdr->chunk_id)
		+ sizeof(hdr->chunk_size)
		+ sizeof(hdr->format_tag)
		+ sizeof(hdr->channels)
		+ sizeof(hdr->samples_per_sec)
		+ sizeof(hdr->avg_bytes_per_sec)
		+ sizeof(hdr->block_align)
		+ sizeof(hdr->bits_per_sample);
}

extern void
wav_hdr_recalculate_file_size(struct wav_hdr *hdr,
		const struct wav_data *data)
{
	hdr->file_size = 0;
	hdr->file_size += wav_hdr_size(hdr);
	hdr->file_size -= strlen(hdr->group_id);
	hdr->file_size -= sizeof(hdr->file_size);
	hdr->file_size += wav_data_size(data);
}

extern size_t
wav_hdr_bytes(const struct wav_hdr *hdr, unsigned char **bytes)
{
	size_t sz;
	unsigned char *tmp;

	sz = wav_hdr_size(hdr);
	tmp = *bytes = xmalloc(sz);

	memcpy_adv(tmp, hdr->group_id, strlen(hdr->group_id));
	memcpy_adv(tmp, &hdr->file_size, sizeof(hdr->file_size));
	memcpy_adv(tmp, hdr->riff_type, strlen(hdr->riff_type));
	memcpy_adv(tmp, hdr->chunk_id, strlen(hdr->chunk_id));
	memcpy_adv(tmp, &hdr->chunk_size, sizeof(hdr->chunk_size));
	memcpy_adv(tmp, &hdr->format_tag, sizeof(hdr->format_tag));
	memcpy_adv(tmp, &hdr->channels, sizeof(hdr->channels));
	memcpy_adv(tmp, &hdr->samples_per_sec, sizeof(hdr->samples_per_sec));
	memcpy_adv(tmp, &hdr->avg_bytes_per_sec, sizeof(hdr->avg_bytes_per_sec));
	memcpy_adv(tmp, &hdr->block_align, sizeof(hdr->block_align));
	memcpy_adv(tmp, &hdr->bits_per_sample, sizeof(hdr->bits_per_sample));

	return sz;
}

extern void
wav_data_init(struct wav_data *data)
{
	data->chunk_id = "data";
	data->chunk_size = 0;
	data->samples_capacity = 0;
	data->samples_len = 0;
	data->samples = NULL;
}

extern void
wav_data_add_sample(struct wav_data *data, unsigned short sample)
{
	if (data->samples_len >= data->samples_capacity) {
		data->samples_capacity += 128;
		data->samples = xrealloc(data->samples,
				data->samples_capacity * sizeof(data->samples[0]));
	}

	data->samples[data->samples_len++] = sample;
	data->chunk_size = data->samples_len * sizeof(data->samples[0]);
}

extern void
wav_data_add_samples(struct wav_data *data,
		unsigned short *samples, size_t n)
{
	while (n-- > 0)
		wav_data_add_sample(data, *samples++);
}

extern void
wav_data_reset(struct wav_data *data)
{
	free(data->samples);
	wav_data_init(data);
}

extern size_t
wav_data_size(const struct wav_data *data)
{
	return 0
		+ strlen(data->chunk_id)
		+ sizeof(data->chunk_size)
		+ sizeof(data->samples[0]) * data->samples_len;
}

extern size_t
wav_data_bytes(const struct wav_data *data, unsigned char **bytes)
{
	size_t i, sz;
	unsigned char *tmp;

	sz = wav_data_size(data);
	tmp = *bytes = xmalloc(sz);

	memcpy_adv(tmp, data->chunk_id, strlen(data->chunk_id));
	memcpy_adv(tmp, &data->chunk_size, sizeof(data->chunk_size));

	for (i = 0; i < data->samples_len; ++i) {
		memcpy_adv(tmp, &data->samples[i],
				sizeof(data->samples[0]));
	}

	return sz;
}

extern unsigned long
wav_duration(const struct wav_hdr *hdr,
		const struct wav_data *data)
{
	return ((unsigned long) data->samples_len * 1000) /
		(hdr->samples_per_sec * hdr->channels);
}

extern int
wav_write(const struct wav_hdr *hdr, const struct wav_data *data,
		const char *path)
{
	FILE *fp;
	size_t hdr_size, data_size;
	unsigned char *hdr_bytes, *data_bytes;

	if (NULL == (fp = fopen(path, "wb")))
		return -1;

	hdr_size = wav_hdr_bytes(hdr, &hdr_bytes);
	data_size = wav_data_bytes(data, &data_bytes);

	fwrite(hdr_bytes, hdr_size, 1, fp);
	fwrite(data_bytes, data_size, 1, fp);
	fclose(fp);

	free(hdr_bytes);
	free(data_bytes);

	return 0;
}
