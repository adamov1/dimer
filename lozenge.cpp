/*
* @Author: adamov
* @Date:   2020-02-26 18:58:52
* @Last Modified by:   adamov1
* @Last Modified time: 2020-03-01 05:35:33
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


void push_up(vector<vector<int>> &slices)
{
	/*
	* Corresponds to the S->S+1 operation in the paper
	*
	* slices is 2d vector where slices[t] is a vector containing the t'th positions of the N paths which make it up
	*
	*/
	int T = slices.size()-1;
	int N = slices[0].size();
	int S = slices[T][0];

	// slices[t+1] will be X[t+1] and slices[t] will be Y[t] (note that Y[0]=X[0] always)
	for (int t = 0; t < T; t++)
	{
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

				int k = slices[t+1][blockstart];
				int xi = D(k+T-t-S-1, k+1, i-blockstart)(generator);

				for (int j = blockstart+xi; j < i; j++) slices[t+1][j]++;
			}
		}
	}
}


void push_down(vector<vector<int>> &slices)
{
	/*
	* Corresponds to the S->S-1 operation in the paper
	* 
	* Original paper seems to have an error in the definition of xi, should be using falling factorials which we implement here by 
	* reusing D by switching arguments and subtracting from length to avoid dividing by 0
	*
	* slices is 2d vector where slices[t] is a vector containing the t'th positions of the N paths which make it up
	*
	*/
	int T = slices.size()-1;
	int N = slices[0].size();
	int S = slices[T][0];

	// slices[t+1] will be X[t+1] and slices[t] will be Y[t] (note that Y[0]=X[0] always)
	for (int t = 0; t < T; t++)
	{
		int blockstart;
		int i = 0;

		while (i < N)
		{
			blockstart = i;
			i++;

			if (slices[t+1][blockstart] == slices[t][blockstart]);  // we can't do anything
			else if (slices[t+1][blockstart] == slices[t][blockstart]+2) slices[t+1][blockstart]--;  // we must push down
			else  // these have slices[t+1][blockstart] == slices[t][blockstart]+1
			{
				while (i < N and slices[t+1][i] == slices[t][i]+1 and slices[t][i] == slices[t][i-1]+1) i++;

				int k = slices[t+1][blockstart];
				int length = i-blockstart;
				int xi = length-D(N+S-k-1-(length-1), N+t-k+1-(length-1), length)(generator);

				for (int j = blockstart; j < blockstart+xi; j++) slices[t+1][j]--;
			}
		}
	}
}


vector<vector<int>> generate_lozenge_paths(int N)
{
	/*
	* Generates uniformly random set of N nonintersecting paths from (0,0), (0, 1), ..., (0, N-1) to (2N, N), (2N, N+1), ... (2N, 2N-1)
	* respectively such that every step in a path is either (1,0) or (1,1)
	*
	*/
	int T = 2*N;
	vector<vector<int>> slices(T + 1, vector<int> (N));

	for (int t = 0; t <= T; t++) for (int i = 0; i < N; i++) slices[t][i] = i;

	for (int S = 0; S < N; S++) push_up(slices);

	return slices;
}


