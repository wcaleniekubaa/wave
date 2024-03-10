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
#include <stddef.h>

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

#ifdef _MSC_VER
	#define MAYBE_UNUSED
#else
	#define MAYBE_UNUSED __attribute__((unused))
#endif
/**
 * Simple representation of a WAV file.
 * @see wave_file_load
 * @see wave_file_free
 * @see wave_file_save
 */
typedef struct
{
	uint16_t num_channels;
	uint32_t sample_rate;
	uint16_t bits_per_sample;
	uint32_t data_size;
	uint8_t *data;
} wave_file_t;

/**
 * Error codes.
 * @see wave_get_error
 */
typedef enum
{
	WAVE_ERROR_HEAP_FAIL,
	WAVE_ERROR_FILE_NOT_FOUND,
	WAVE_ERROR_EOF,
	WAVE_ERROR_INVALID_FORMAT,
	WAVE_ERROR_INVALID_DATA,
	WAVE_ERROR_NONE
} wave_error_t;

/**
 * Loads a WAV file from disk.
 * @param filename The path to the WAV file.
 * @return The WAV file, or NULL if an error occurred.
 * @see wave_file_free
 */
WAVE_API wave_file_t *wave_file_load(const char *filename);

/**
 * Loads a WAV file from memory.
 * @param len The length of the data.
 * @param data Pointer to the data.
 * @return The WAV file, or NULL if an error occurred.
 * @see wave_file_load
 */
WAVE_API wave_file_t *wave_load_from_memory(size_t len, const uint8_t *data);
/**
 * Frees a WAV file.
 * @param wave The WAV file.
 * @see wave_file_load
 */
WAVE_API void wave_file_free(wave_file_t *wave);

/**
 * Returns the last error that occurred.
 * @return The last error that occurred.
 * @see wave_error_t
 */
WAVE_API wave_error_t wave_file_get_error(void);

/**
 * Returns a human-readable string for a given error.
 * @param error The error.
 * @return String describing the error.
 */
WAVE_API const char* wave_error_to_str(wave_error_t error);

/**
 * Saves a WAV file to disk.
 * @param filename The path to the WAV file to save to.
 * @param wave The WAV file to save.
 * @return 1 if the WAV file was saved successfully, 0 otherwise.
 */
WAVE_API uint8_t wave_save_to_file(const char *filename, wave_file_t *wave);

#endif //WAVE_H
