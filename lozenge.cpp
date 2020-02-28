/*
* @Author: adamov
* @Date:   2020-02-26 18:58:52
* @Last Modified by:   adamov
* @Last Modified time: 2020-02-28 17:31:47
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
	* Generates distribution D on {0, 1, ..., n} such that k has weight (a)^^k/(b)^^k where (-)^^k represents the rising factorial
	*
	*/
	vector<double> weights(n+1);
	weights[0] = 1;

	for (int k = 1; k<=n; k++)
	{
		weights[k]=weights[k-1]*((double)(a+k-1)/(double)(b+k-1));
	}

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
	int S = slices[T][N-1]-N+1;

	int newslice[N];

	//slices[t+1] will be X[t+1] and slices[t] will be Y[t] (note that Y[0]=X[0] always)
	for (int t = 0; t < T; t++)
	{
		for (int i = 0; i < N; i++)
		{
			if (slices[t+1][i]-slices[t][i] == 1) newslice[i] = slices[t+1][i];
			else if (slices[t+1][i]-slices[t][i] == -1) newslice[i] = slices[t][i];
			else newslice[i] = -1; //by construction must have slices[t+1][i] == slices[t][i] here, -1 is a placeholder
		}

		int blockstart;
		int length = 1;
		int i = 0;

		while (i < N)
		{
			if (newslice[i] != -1)
			{
				i++;
				continue;
			}

			blockstart = i;
			i++;

			while (i < N and newslice[i] == -1 and slices[t][i]-slices[t][i-1] == 1)
			{
				length++;
				i++;
			}

			discrete_distribution<int> dist = D(slices[t][blockstart]+T-t-S-1, slices[t][blockstart]+1, length);
			int xi = dist(generator);

			for (int j = 0; j < length; j++)
			{
				if (j < xi) newslice[blockstart+j] = slices[t][blockstart+j];
				else newslice[blockstart+j] = slices[t][blockstart+j]+1;
			}

			length = 1;
		}

		for (int i = 0; i < N; i++) slices[t+1][i] = newslice[i];
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
	vector<vector<int>> slices(T+1, vector<int> (N));

	for (int t = 0; t <= T; t++) for (int i = 0; i < N; i++) slices[t][i] = i;

	for (int S = 0; S < N; S++) push_up(slices);

	return slices;
}


