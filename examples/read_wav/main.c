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