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

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <wave file>\n", argv[0]);
		return 1;
	}

	wave_file_t* wave = wave_file_load(argv[1]);
	if (wave == NULL)
	{
		wave_error_t error = wave_file_get_error();
		printf("Error: %s\n", wave_error_to_str(error));
		return 1;
	}

	printf("file->num_channels = %d\n", wave->num_channels);
	printf("file->sample_rate = %d\n", wave->sample_rate);
	printf("file->bits_per_sample = %d\n", wave->bits_per_sample);
	printf("file->data_size = %d\n", wave->data_size);
	printf("file->data = %p\n", wave->data);

	wave_file_free(wave);
	return 0;
}