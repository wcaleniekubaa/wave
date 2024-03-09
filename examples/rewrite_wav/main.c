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
	if (argc < 3)
	{
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		return 1;
	}

	wave_file_t* wave = wave_file_load(argv[1]);
	if (wave == NULL)
		return 1;

	for (int i = 0; i < wave->data_size; i++)
	{
		wave->data[i] = wave->data[i] / 2;
	}

	wave_save_to_file(argv[2], wave);

	wave_file_free(wave);

	return 0;
}