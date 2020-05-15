/*
* @Author: adamov
* @Date:   2020-02-26 18:58:52
* @Last Modified by:   adamov1
* @Last Modified time: 2020-03-01 19:58:01
* 
* Based on algorithm described in arXiv:0804.3071
*/

#include "lozenge.h"
#include <random>

using namespace std;

default_random_engine generator(random_device{}());

discrete_distribution<int> D(int a, int b, int n)
{
	/*
	* Samples from distribution D on {0, 1, ..., n} such that k has weight (a)^^k/(b)^^k where (-)^^k represents the rising factorial
	*
	*/
	vector<double> weights(n+1);
	weights[0] = 1.0;

	for (int k = 0; k < n; k++) weights[k+1] = weights[k]*(a+k)/(b+k);

	return discrete_distribution<int> (weights.begin(), weights.end());
}


void push_up_slice(vector<vector<int>> &slices, int t)
{
	/*
	* Helper for push_up to push up slice t+1 (slices[t+1] is X(t+1) and slices[t] is Y(t))
	*
	*/
	int T = slices.size()-1;
	int N = slices[0].size();
	int S = slices[T][0];

	int blockstart;
	int i = 0;

	while (i < N)
	{
		blockstart = i;
		i++;

		if (slices[t+1][blockstart] == slices[t][blockstart]+1);  // we can't do anything
		else if (slices[t+1][blockstart] == slices[t][blockstart]-1) slices[t+1][blockstart]++;  // we must push up
		else  // these have slices[t+1][blockstart] == slices[t][blockstart]
		{
			while (i < N and slices[t+1][i] == slices[t][i] and slices[t][i] == slices[t][i-1]+1) i++;

			int xi = D(slices[t+1][blockstart]+T-t-S-1, slices[t+1][blockstart]+1, i-blockstart)(generator);

			for (int j = blockstart+xi; j < i; j++) slices[t+1][j]++;
		}
	}
}


void push_down_slice(vector<vector<int>> &slices, int t)
{
	/*
	* Helper for push_down to push down slice t+1 (slices[t+1] is X(t+1) and slices[t] is Y(t))
	* 
	* Original paper seems to have an error in the definition of xi, should be using falling factorials, which we implement here
	* by reusing D, switching arguments and running from top to bottom instead of bottom to top
	*
	*/
	int T = slices.size()-1;
	int N = slices[0].size();
	int S = slices[T][0];

	int blockstart;
	int i = N-1;

	while (i >= 0)
	{
		blockstart = i;
		i--;

		if (slices[t+1][blockstart] == slices[t][blockstart]);  // we can't do anything
		else if (slices[t+1][blockstart] == slices[t][blockstart]+2) slices[t+1][blockstart]--;  // we must push down
		else  // these have slices[t+1][blockstart] == slices[t][blockstart]+1
		{
			while (i >= 0 and slices[t+1][i] == slices[t][i]+1 and slices[t][i] == slices[t][i+1]-1) i--;

			int xi = D(N+S-slices[t+1][blockstart]-1, N+t-slices[t+1][blockstart]+1, blockstart-i)(generator);

			for (int j = blockstart-xi; j > i; j--) slices[t+1][j]--;
		}
	}
}


void push_up(vector<vector<int>> &slices)
{
	/*
	* Corresponds to the S->S+1 operation in the paper
	*
	*/
	for (int t = 0; t < slices.size()-1; t++) push_up_slice(slices, t);
}


void push_down(vector<vector<int>> &slices)
{
	/*
	* Corresponds to the S->S-1 operation in the paper
	*
	*/
	for (int t = 0; t < slices.size()-1; t++) push_down_slice(slices, t);
}


vector<vector<int>> generate_lozenge_paths(int N)
{
	/*
	* Generates uniformly random set of N nonintersecting paths from (0,0), (0, 1), ..., (0, N-1) to (2N, N), (2N, N+1), ... (2N, 2N-1)
	* respectively such that every step in a path is either (1,0) or (1,1)
	* 
	* slices is 2d vector where slices[t] is a vector containing the t'th positions of the N paths which make it up
	*
	*/
	int T = 2*N;
	vector<vector<int>> slices(T + 1, vector<int> (N));

	for (int t = 0; t <= T; t++) for (int i = 0; i < N; i++) slices[t][i] = i;

	for (int S = 0; S < N; S++) push_up(slices);

	return slices;
}


