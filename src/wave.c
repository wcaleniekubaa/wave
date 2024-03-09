/*
   Copyright 2024 wcaleniekubaa

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <wave/wave.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#if IS_BIG_ENDIAN
	#define IS_LITTLE_ENDIAN 0
#else
	#define IS_LITTLE_ENDIAN 1
#endif

inline static void swap_endianness(uint8_t* data, size_t len)
{
	uint8_t tmp;
	size_t i;
	for (i = 0; i < len / 2; ++i)
	{
		tmp = data[i];
		data[i] = data[len - i - 1];
		data[len - i - 1] = tmp;
	}
}

MAYBE_UNUSED inline

static void to_little_endian(uint8_t* data, size_t len)
{
#if !IS_LITTLE_ENDIAN
	swap_endianness(data, len);
#endif
	UNUSED(data);
	UNUSED(len);

}

MAYBE_UNUSED inline

static void from_little_endian(uint8_t* data, size_t len)
{
#if !IS_LITTLE_ENDIAN
	swap_endianness(data, len);
#endif
	UNUSED(data);
	UNUSED(len);
}

MAYBE_UNUSED inline

static void to_big_endian(uint8_t* data, size_t len)
{
#if IS_LITTLE_ENDIAN
	swap_endianness(data, len);
#endif
	UNUSED(data);
	UNUSED(len);
}

MAYBE_UNUSED inline

static void from_big_endian(uint8_t* data, size_t len)
{
#if IS_LITTLE_ENDIAN
	swap_endianness(data, len);
#endif
	UNUSED(data);
	UNUSED(len);
}

#define CHECK_CHUNK(x, x1) (x[0] == x1[0] && x[1] == x1[1] && x[2] == x1[2] && x[3] == x1[3])

typedef struct wave_error_list_node_t
{
	wave_error_t error;
	struct wave_error_list_node_t* next;
} wave_error_list_node_t;

static wave_error_list_node_t* wave_error_list = NULL;

static void wave_push_error(wave_error_t error)
{
	wave_error_list_node_t* node = malloc(sizeof(wave_error_list_node_t));
	node->error = error;
	node->next = wave_error_list;
	wave_error_list = node;
}

wave_file_t* wave_file_load(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (!f)
	{
		wave_push_error(WAVE_ERROR_FILE_NOT_FOUND);
		return NULL;
	}

	uint8_t* buf;
	size_t buf_size;
	fseek(f, 0, SEEK_END);
	buf_size = ftell(f);
	rewind(f);
	buf = (uint8_t*)malloc(buf_size);
	if (!buf)
	{
		wave_push_error(WAVE_ERROR_HEAP_FAIL);
		return NULL;
	}
	fread(buf, 1, buf_size, f);
	fclose(f);

	wave_file_t* wave = wave_load_from_memory(buf_size, buf);
	free(buf);
	return wave;
}

wave_error_t wave_file_get_error(void)
{
	wave_error_t error = WAVE_ERROR_NONE;
	if (wave_error_list)
	{
		error = wave_error_list->error;

		wave_error_list_node_t* node = wave_error_list;
		wave_error_list = wave_error_list->next;
		free(node);
	}
	return error;
}

wave_file_t* wave_load_from_memory(size_t len, const uint8_t* data)
{
	wave_file_t* wave = calloc(1, sizeof(wave_file_t));
	if (wave == NULL)
	{
		wave_push_error(WAVE_ERROR_HEAP_FAIL);
		free(wave);
		return NULL;
	}

	if (len < 44)
	{
		wave_push_error(WAVE_ERROR_INVALID_FORMAT);
		free(wave);
		return NULL;
	}

	size_t offset = 0;
	while (1)
	{
		int8_t chunk_id[4];
		memcpy(chunk_id, data + offset, 4);

		if (CHECK_CHUNK(chunk_id, "RIFF"))
		{
			uint32_t data_size;
			memcpy(&data_size, data + offset + 4, 4);
			from_little_endian((uint8_t * ) & data_size, 4);

			offset += 4;
		}
		else if (CHECK_CHUNK(chunk_id, "fmt "))
		{
			uint32_t chunk_size;
			memcpy(&chunk_size, data + offset + 4, 4);

			from_little_endian((uint8_t * ) & chunk_size, 4);
			if (chunk_size < 16)
			{
				wave_push_error(WAVE_ERROR_INVALID_FORMAT);
				free(wave);
				return NULL;
			}

			uint16_t format;

			memcpy(&format, data + offset + 8, 2);

			from_little_endian((uint8_t * ) & format, 2);
			if (format != 1)
			{
				wave_push_error(WAVE_ERROR_INVALID_FORMAT);

				free(wave);

				return NULL;

			}

			uint16_t num_channels;

			memcpy(&num_channels, data + offset + 10, 2);
			from_little_endian((uint8_t * ) & num_channels, 2);
			wave->num_channels = num_channels;

			uint32_t sample_rate;
			memcpy(&sample_rate, data + offset + 12, 4);
			from_little_endian((uint8_t * ) & sample_rate, 4);

			wave->sample_rate = sample_rate;

			uint32_t byte_rate;
			memcpy(&byte_rate, data + offset + 16, 4);
			from_little_endian((uint8_t * ) & byte_rate, 4);

			uint16_t block_align;
			memcpy(&block_align, data + offset + 20, 2);
			from_little_endian((uint8_t * ) & block_align, 2);

			uint16_t bits_per_sample;
			memcpy(&bits_per_sample, data + offset + 22, 2);
			from_little_endian((uint8_t * ) & bits_per_sample, 2);

			wave->bits_per_sample = bits_per_sample;

			offset += chunk_size;
		}
		else if (CHECK_CHUNK(chunk_id, "data"))
		{
			uint32_t chunk_size;
			memcpy(&chunk_size, data + offset + 4, 4);
			from_little_endian((uint8_t * ) & chunk_size, 4);
			wave->data_size = chunk_size;

			wave->data = malloc(chunk_size);
			memcpy(wave->data, data + offset + 8, chunk_size);
			break;
		}
		offset += 4;
	}
	return wave;
}
void wave_file_free(wave_file_t* wave)
{
	free(wave->data);
	free(wave);
}

const char* wave_error_to_str(wave_error_t error)
{
	switch (error)
	{
	case WAVE_ERROR_HEAP_FAIL:
		return "HEAP_FAIL";
	case WAVE_ERROR_FILE_NOT_FOUND:
		return "FILE_NOT_FOUND";
	case WAVE_ERROR_EOF:
		return "EOF";
	case WAVE_ERROR_INVALID_FORMAT:
		return "INVALID_FORMAT";
	case WAVE_ERROR_INVALID_DATA:
		return "INVALID_DATA";
	case WAVE_ERROR_NONE:
		return "NONE";
	default: // in case someone wanted to cast random integer to wave_error_t
		return "UNKNOWN";
	}
}

uint8_t wave_save_to_file(const char* filename, wave_file_t* wave)
{
	FILE* f = fopen(filename, "wb");
	if (!f)
	{
		wave_push_error(WAVE_ERROR_FILE_NOT_FOUND);
		return 0;
	}

	fwrite("RIFF", 1, 4, f);

	uint32_t data_size = 36 + wave->data_size;
	to_little_endian((uint8_t * ) & data_size, 4);
	fwrite(&data_size, 1, 4, f);
	fwrite("WAVE", 1, 4, f);
	fwrite("fmt ", 1, 4, f);

	uint32_t chunk_size = 16;
	to_little_endian((uint8_t * ) & chunk_size, 4);
	fwrite(&chunk_size, 1, 4, f);

	uint16_t format = 1;
	to_little_endian((uint8_t * ) & format, 2);
	fwrite(&format, 1, 2, f);

	uint16_t num_channels = wave->num_channels;
	to_little_endian((uint8_t * ) & num_channels, 2);
	fwrite(&num_channels, 1, 2, f);

	uint32_t sample_rate = wave->sample_rate;
	to_little_endian((uint8_t * ) & sample_rate, 4);
	fwrite(&sample_rate, 1, 4, f);

	uint32_t byte_rate = wave->sample_rate * wave->num_channels * (wave->bits_per_sample / 8);
	to_little_endian((uint8_t * ) & byte_rate, 4);
	fwrite(&byte_rate, 1, 4, f);

	uint16_t block_align = wave->num_channels * (wave->bits_per_sample / 8);
	to_little_endian((uint8_t * ) & block_align, 2);
	fwrite(&block_align, 1, 2, f);

	uint16_t bits_per_sample = wave->bits_per_sample;
	to_little_endian((uint8_t * ) & bits_per_sample, 2);
	fwrite(&bits_per_sample, 1, 2, f);
	fwrite("data", 1, 4, f);

	fwrite(&wave->data_size, 1, 4, f);

	if (!wave->data)
	{
		for (size_t i = 0; i < wave->data_size; i++)
			fwrite("\0", 1, 1, f);
	}
	else
	{
		fwrite(wave->data, 1, wave->data_size, f);
	}

	fclose(f);
	return 1;
}