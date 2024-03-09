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

#ifndef WAVE_H
#define WAVE_H

#include <stdint.h>

#ifdef __cplusplus
	#define C_LINKAGE extern "C"
#else
	#define C_LINKAGE extern
#endif

#ifdef WAVE_SHARED
	#ifdef _WIN32
		#ifdef WAVE_EXPORT
			#define WAVE_API C_LINKAGE __declspec(dllexport)
		#else
			#define WAVE_API C_LINKAGE __declspec(dllimport)
		#endif
	#else
		#define WAVE_API C_LINKAGE __attribute__((visibility("default")))
	#endif
#else
	#define WAVE_API C_LINKAGE
#endif

#define UNUSED(x) (void)(x)
#define MAYBE_UNUSED __attribute__((unused))

typedef struct
{
	uint16_t num_channels;
	uint32_t sample_rate;
	uint16_t bits_per_sample;
	uint32_t data_size;
	uint8_t *data;
} wave_file_t;

typedef enum
{
	WAVE_ERROR_HEAP_FAIL,
	WAVE_ERROR_FILE_NOT_FOUND,
	WAVE_ERROR_EOF,
	WAVE_ERROR_INVALID_FORMAT,
	WAVE_ERROR_INVALID_DATA,
	WAVE_ERROR_NONE
} wave_error_t;

WAVE_API wave_file_t *wave_file_load(const char *filename);
WAVE_API wave_file_t *wave_load_from_memory(size_t len, const uint8_t *data);
WAVE_API void wave_file_free(wave_file_t *wave);
WAVE_API wave_error_t wave_file_get_error(void);
WAVE_API const char* wave_error_to_str(wave_error_t error);
WAVE_API uint8_t wave_save_to_file(const char *filename, wave_file_t *wave);

#endif //WAVE_H
