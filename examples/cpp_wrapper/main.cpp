
#include <wave/wave.hpp>
#include <iostream>

int main(int argc, char **argv)
{
	if (argc < 2)	{
		std::cout << "Usage: " << argv[0] << " <input file>\n";
		return 1;
	}

	wave::file f;
	wave::error err = f.load_from_file(argv[1]);
	if (err.is_error())
	{
		std::cout << "error: " << err.to_str() << "\n";
		return 1;
	}

	std::cout <<
		"f.num_channels() = " << f.num_channels() << "\n" <<
		"f.sample_rate() = " << f.sample_rate() << "\n" <<
		"f.bits_per_sample() = " << f.bits_per_sample() << "\n" <<
		"f.data_size() = " << f.data_size() << "\n";

	return 0;
}