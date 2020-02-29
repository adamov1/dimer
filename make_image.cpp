/*
* @Author: adamov
* @Date:   2020-02-26 21:18:44
* @Last Modified by:   adamov1
* @Last Modified time: 2020-02-28 18:53:31
*/

#include "make_image.h"
#include <cairo-svg.h>
#include <math.h>

using namespace std;

void draw_quadrilateral(cairo_t *cr, 
	double x1, double y1, 
	double x2, double y2, 
	double x3, double y3, 
	double x4, double y4)
{
	/*
	* Adds quadrilateral specified by four points in order to context cr
	*
	*/
	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_line_to(cr, x3, y3);
	cairo_line_to(cr, x4, y4);
	cairo_close_path(cr);
}


void lozenge_paths_to_image(vector<vector<int>> slices, const char* filename, double image_height, double relative_line_width, bool arctic_circle)
{
	/*
	* Given slices, which contains N nonintersecting paths from from (0,0), (0, 1), ..., (0, N-1) to (2N, N), (2N, N+1), ... (2N, 2N-1)
	* respectively such that every step in a path is either (1,0) or (1,1), generates an image of the lozenge tiling corresponding to it
	* 
	* Output will be saved in filename
	* 
	* Size of the image will have aspect ratio T*sqrt(3) by 2*N+T (approximate due to slight transparent border), and height image_height px
	* 
	* Width of outlines of tiles determined by relative_line_width (value of 1 causes outlines to almost entirely cover tiles)
	* 
	* If relative_line_width == 0.0 then will not draw outlines of tiles (significantly decreases file size)
	*
	* If arctic_circle (default false) then will draw tiles on the outer edges in different color to highlight arctic circle phenomenon
	*
	*/
	double 
		outline_r = 0.0, 
		outline_g = 0.0, 
		outline_b = 0.0,

		left_r = 0.8, 
		left_g = 0.3, 
		left_b = 0.3,

		right_r = 0.5, 
		right_g = 0.3, 
		right_b = 0.3, 

		horizontal_r = 0.9, 
		horizontal_g = 0.9, 
		horizontal_b = 0.9,

		arctic_r = 0.8, 
		arctic_g = 0.95, 
		arctic_b = 0.95;

	int T = slices.size()-1;
	int N = slices[0].size();
	int height = slices[T][N-1];
	int S = height-N+1;

	double scale_factor = 0.96*image_height/(2*N+T);
	bool outline = (relative_line_width != 0);

	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_svg_surface_create(filename, 0.04*image_height+scale_factor*sqrt(3)*(T), 0.04*image_height+scale_factor*(2*N+T));
	cr = cairo_create(surface);

	cairo_matrix_t hex;  // this will be the transform which makes the image appear as equilateral triangular grid
	cairo_matrix_init(&hex, scale_factor*sqrt(3), scale_factor, 0.0, -2*scale_factor, 0.02*image_height , 0.02*image_height+scale_factor*(2*height-S+2));

	cairo_set_line_width(cr, scale_factor*relative_line_width);  // sets linewidth of outlines of tiles
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
	cairo_set_source_rgb(cr, outline_r, outline_g, outline_b);  // color of outlines
	cairo_save(cr);

	cairo_transform(cr, &hex);

	//
	// FILLING
	//

	// horizontal faces
	// draw large hexagon to be painted over instead of individual horizontal tiles
	cairo_move_to(cr, 0, 0);
	cairo_line_to(cr, T-S, 0);
	cairo_line_to(cr, T, S);
	cairo_line_to(cr, T, N+S);
	cairo_line_to(cr, S, N+S);
	cairo_line_to(cr, 0, N);
	cairo_close_path(cr);

	cairo_set_source_rgb(cr, horizontal_r, horizontal_g, horizontal_b);  // color of horizontal tiles
	cairo_fill(cr);

	if (arctic_circle)
	{
		for (int t = 1; t < T; t++)
		{
			for (int y = max(0, t-T+S); y < slices[t][0]; y++)  // faces with z coordinate 0 when viewed in 3d
			{
				draw_quadrilateral(cr, 
					t, y, 
					t+1, y+1, 
					t, y+1, 
					t-1, y);
				cairo_set_source_rgb(cr, arctic_r, arctic_g, arctic_b);
				cairo_fill(cr);
			}
			for (int y = slices[t][N-1]+1; y <= min(t+N-1, S+N-1); y++)  // faces with z coordinate N when viewed in 3d
			{
				draw_quadrilateral(cr, 
					t, y, 
					t+1, y+1, 
					t, y+1, 
					t-1, y);
				cairo_set_source_rgb(cr, arctic_r, arctic_g, arctic_b);
				cairo_fill(cr);
			}		
		}
	}

	// lateral faces
	for (int t = 0; t < T; t++)
	{
		int i = 0;
		
		while (i < N)
		{
			int j = i;

			while (j < N and slices[t][i]-slices[t+1][i]==slices[t][j]-slices[t+1][j] and slices[t][j]-slices[t][i]==j-i) j++;

			draw_quadrilateral(cr, 
				t, slices[t][i],
				t+1, slices[t+1][i], 
				t+1, slices[t+1][i]+j-i, 
				t, slices[t][i]+j-i);

			if (slices[t][i] == slices[t+1][i])  // left facing faces
			{
				if (arctic_circle and (slices[t][i] == i or slices[t][i] == S+i)) cairo_set_source_rgb(cr, arctic_r, arctic_g, arctic_b);  // far left or far right
				else cairo_set_source_rgb(cr, left_r, left_g, left_b);
			}
			else  // right facing faces
			{
				if (arctic_circle and (slices[t][i] == t+i or slices[t][i] == t-T+S+i)) cairo_set_source_rgb(cr, arctic_r, arctic_g, arctic_b);  // far left or far right
				else cairo_set_source_rgb(cr, right_r, right_g, right_b);
			}

			cairo_fill(cr);
			i = j;
		}
	}

	//
	// OUTLINES
	//

	if (outline)
	{
		// lateral faces
		for (int i = 0; i < N; i++)
		{
			for (int t = 0; t < T; t++)
			{
				draw_quadrilateral(cr, 
					t, slices[t][i], 
					t+1, slices[t+1][i], 
					t+1, slices[t+1][i]+1, 
					t, slices[t][i]+1);
				cairo_restore(cr);  // makes linewidths unaffected by transformation (transformation unequally scales x and y)
				cairo_stroke(cr);
				cairo_save(cr);
				cairo_transform(cr, &hex);
			}
		}

		// horizontal faces
		for (int t = 1; t < T; t++)
		{
			for (int y = max(0, t-T+S); y < slices[t][0]; y++)  // faces with z coordinate 0 when viewed in 3d
			{
				draw_quadrilateral(cr, 
					t, y, 
					t+1, y+1, 
					t, y+1, 
					t-1, y);
				cairo_restore(cr);
				cairo_stroke(cr);
				cairo_save(cr);
				cairo_transform(cr, &hex);
			}
			for (int i = 0; i < N-1; i++)
			{
				for (int y = slices[t][i]+1; y < slices[t][i+1]; y++)  // faces with z coordinate strictly between 0 and N in 3d
				{
					draw_quadrilateral(cr, 
						t, y, 
						t+1, y+1, 
						t, y+1, 
						t-1, y);
					cairo_restore(cr);
					cairo_stroke(cr);
					cairo_save(cr);
					cairo_transform(cr, &hex);
				}
			}
			for (int y = slices[t][N-1]+1; y <= min(t+N-1, S+N-1); y++)  // faces with z coordinate N when viewed in 3d
			{
				draw_quadrilateral(cr, 
					t, y, 
					t+1, y+1, 
					t, y+1, 
					t-1, y);
				cairo_restore(cr);
				cairo_stroke(cr);
				cairo_save(cr);
				cairo_transform(cr, &hex);
			}		
		}
	}

	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}


