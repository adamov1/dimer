/*
* @Author: adamov
* @Date:   2020-02-28 17:18:37
* @Last Modified by:   adamov1
* @Last Modified time: 2020-05-14 22:20:52
*/

#include "lozenge.h"
#include "make_image.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
	/*
	* Execute by running ./tile <filename> <size> <line_width> <arctic_circle> <a> <b> <c>
	*
	*/
	const char* filename = "hex_tiling.svg";
	int a = 8;
	int b = 8;
	int c = 8;
	int size = 1000;
	double line_width = 0.0;
	bool arctic_circle = false;

	if (argc >= 2) filename = argv[1];
	if (argc >= 3) size = max(1, stoi(argv[2]));
	if (argc >= 4) line_width = max(0.0, stod(argv[3]));
	if (argc >= 5) arctic_circle = (strcmp(argv[4], "true")==0);
	if (argc >= 6) a = max(1, stoi(argv[5]));
	if (argc >= 7) b = max(1, stoi(argv[6]));
	else b = a;
	if (argc >= 8) c = max(1, stoi(argv[7]));
	else c = a;

	cout << setprecision(4) << fixed;

	auto start1 = steady_clock::now();
	vector<vector<int>> slices = generate_lozenge_paths(a, b, c);
	auto end1 = steady_clock::now();
	duration<double> dur1 = end1-start1;
	cout << "Generating paths took:\t" << dur1.count() << " seconds." << endl;

	auto start2 = steady_clock::now();
	lozenge_paths_to_image(slices, filename, size, line_width, arctic_circle);
	auto end2 = steady_clock::now();
	duration<double> dur2 = end2-start2;
	cout << "Creating SVG file took:\t" << dur2.count() << " seconds." << endl;
}
