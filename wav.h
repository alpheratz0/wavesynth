/*
	Copyright (C) 2023 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License version 2 as published by the
	Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place, Suite 330, Boston, MA 02111-1307 USA

*/

#pragma once

#include <stddef.h>

struct wav_hdr {
	const char                  *group_id;
	unsigned int                file_size;
	const char                 *riff_type;
	const char                  *chunk_id;
	unsigned int               chunk_size;
	unsigned short             format_tag;
	unsigned short               channels;
	unsigned int          samples_per_sec;
	unsigned int        avg_bytes_per_sec;
	unsigned short            block_align;
	unsigned short        bits_per_sample;
};

struct wav_data {
	const char                  *chunk_id;
	unsigned int               chunk_size;
	unsigned int         samples_capacity;
	unsigned int              samples_len;
	unsigned short               *samples;
};

extern void
wav_hdr_init(struct wav_hdr *hdr);

extern size_t
wav_hdr_size(const struct wav_hdr *hdr);

extern void
wav_hdr_recalculate_file_size(struct wav_hdr *hdr,
		const struct wav_data *data);

extern size_t
wav_hdr_bytes(const struct wav_hdr *hdr, unsigned char **bytes);

extern void
wav_data_init(struct wav_data *data);

extern void
wav_data_add_sample(struct wav_data *data, unsigned short sample);

extern void
wav_data_add_samples(struct wav_data *data,
		unsigned short *samples, size_t n);

extern void
wav_data_reset(struct wav_data *data);

extern size_t
wav_data_size(const struct wav_data *data);

extern size_t
wav_data_bytes(const struct wav_data *data, unsigned char **bytes);

extern unsigned long
wav_duration(const struct wav_hdr *hdr,
		const struct wav_data *data);

extern int
wav_write(const struct wav_hdr *hdr, const struct wav_data *data,
		const char *path);
