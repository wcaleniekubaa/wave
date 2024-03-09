#include <wave/wave.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <output.wav>\n", argv[0]);
		return 1;
	}

	wave_file_t wave;
	wave.num_channels = 2;
	wave.sample_rate = 44100;
	wave.bits_per_sample = 16;
	wave.data_size = 0;
	wave.data = NULL;

	wave_save_to_file(argv[1], &wave);
	if (wave_file_get_error()!= WAVE_ERROR_NONE)
		printf("Error: %s\n", wave_error_to_str(wave_file_get_error()));
	else
		printf("Saved to %s\n", argv[1]);

	return 0;
}
