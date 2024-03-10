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

#ifndef WAVE_HPP
#define WAVE_HPP

#include "wave.h"
#include <string>

namespace wave
{
	enum error_code
	{
		ERROR_HEAP_FAIL = WAVE_ERROR_HEAP_FAIL,
		ERROR_FILE_NOT_FOUND = WAVE_ERROR_FILE_NOT_FOUND,
		ERROR_EOF = WAVE_ERROR_EOF,
		ERROR_INVALID_FORMAT = WAVE_ERROR_INVALID_FORMAT,
		ERROR_INVALID_DATA = WAVE_ERROR_INVALID_DATA,
		ERROR_NONE = WAVE_ERROR_NONE,
	};

	struct error
	{
		error_code code;

		static inline error get()
		{
			return error{ static_cast<error_code>(wave_file_get_error()) };
		}

		inline const char* to_str() const
		{
			return wave_error_to_str(static_cast<wave_error_t>(code));
		}

		inline bool is_error() const
		{
			return this->code != ERROR_NONE;
		}

	};

	class file
	{
	public:
		inline file()
			: m_file(nullptr)
		{
		}
		inline ~file()
		{
			if (m_file) wave_file_free(m_file);
		}

		inline error load_from_file(const std::string& filename)
		{
			m_file = wave_file_load(filename.c_str());
			if (!m_file)
				return error::get();
			return error{ ERROR_NONE };
		}

		template<typename T>
		inline error load_from_file(T t)
		{
			return load_from_file(std::string(t));
		}
		template<typename T, size_t N>
		inline error load_from_memory(T t[N])
		{
			m_file = wave_file_load_from_memory(t, N * sizeof(T));
			if (!m_file)
				return error::get();
			return error{ ERROR_NONE };
		}

		inline error save_to_file(const std::string& filename)
		{
			if (!m_file) return error{ ERROR_INVALID_DATA };
			wave_save_to_file(filename.c_str(), m_file);
			return error::get();
		}

		template<typename T>
		inline error save_to_file(T t)
		{
			return save_to_file(std::string(t));
		}

		inline const uint16_t& num_channels() const
		{
			return m_file->num_channels;
		}
		inline uint16_t& num_channels()
		{
			return m_file->num_channels;
		}

		inline const uint32_t& sample_rate() const
		{
			return m_file->sample_rate;
		}

		inline uint32_t& sample_rate()
		{
			return m_file->sample_rate;
		}

		inline const uint16_t& bits_per_sample() const
		{
			return m_file->bits_per_sample;
		}

		inline uint16_t& bits_per_sample()
		{
			return m_file->bits_per_sample;
		}

		inline const uint32_t& data_size() const
		{
			return m_file->data_size;
		}

		inline uint32_t& data_size()
		{
			return m_file->data_size;
		}

		inline const uint8_t* data() const
		{
			return m_file->data;
		}

		inline uint8_t* data()
		{
			return m_file->data;
		}

		inline const wave_file_t* get() const
		{
			return m_file;
		}

		inline wave_file_t* get()
		{
			return m_file;
		}
	private:
		wave_file_t* m_file;
	};
}

#endif //WAVE_HPP
