/*
* @Author: adamov
* @Date:   2020-02-28 17:18:37
* @Last Modified by:   adamov1
* @Last Modified time: 2020-03-01 02:27:08
*/

#include "lozenge.h"
#include "make_image.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
	/*
	* Execute by running ./tile_background <N> <size> <line_width> <arctic_circle> where arctic_circle is either 0 or 1
	*
	*/
	int N = 8;
	int size = 1000;
	double line_width = 0.0;
	bool arctic_circle = false;

	if (argc >= 2) N = max(1, stoi(argv[1]));
	if (argc >= 3) size = max(1, stoi(argv[2]));
	if (argc >= 4) line_width = max(0.0, stod(argv[3]));
	if (argc >= 5) arctic_circle = ( argv[4][0] == '1' );

	cout << setprecision(4) << fixed;

	auto start1 = steady_clock::now();
	vector<vector<int>> slices = generate_lozenge_paths(N);
	auto end1 = steady_clock::now();
	duration<double> dur1 = end1-start1;
	cout << "Generating paths took:\t" << dur1.count() << " seconds." << endl;

	auto start2 = steady_clock::now();
	lozenge_paths_to_image(slices, "hex_tiling.svg", size, line_width, arctic_circle);
	auto end2 = steady_clock::now();
	duration<double> dur2 = end2-start2;
	cout << "Creating SVG file took:\t" << dur2.count() << " seconds." << endl;
}