/*
* @Author: adamov
* @Date:   2020-02-28 17:18:37
* @Last Modified by:   adamov1
* @Last Modified time: 2020-02-28 18:57:41
*/

#include "lozenge.h"
#include "make_image.h"
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	int N = stoi(argv[1]);

	if (N > 0)
	{
		vector<vector<int>> slices = generate_lozenge_paths(N);

		lozenge_paths_to_image(slices, "hex_tiling.svg", 1000, 0.05, true);
	}
}