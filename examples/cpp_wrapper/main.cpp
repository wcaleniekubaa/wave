
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

