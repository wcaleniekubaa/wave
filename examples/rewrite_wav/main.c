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