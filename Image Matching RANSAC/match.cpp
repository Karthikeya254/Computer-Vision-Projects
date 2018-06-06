// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.


//Link to the header file
#include "CImg.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <cmath>
#include <bits/stdc++.h>
#include <algorithm>

//Use the cimg namespace to access functions easily
using namespace cimg_library;
using namespace std;

struct d_pairs{
    SiftDescriptor d1;
    SiftDescriptor d2;
    float dist;
    float ssd_ratio;
};

void draw_descriptor_image(CImg<double> image, const vector<SiftDescriptor> descriptors, const char *filename)
{
  for(unsigned int i=0; i < descriptors.size(); i++)
    {
      int tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0;
      double color_point[] = {255.0, 255.0, 0};
      for(int x=-2; x<3; x++)
	for(int y=-2; y<3; y++)
	  if(x==0 || y==0)
	    for(int c=0; c<3; c++){
	      //Find if coordinates are in workspace to draw crosshair
	      tx1 = (descriptors[i].col + y - 1);
	      ty1 = (descriptors[i].row + x - 1);
	      if (tx1 >= 0 && tx1 < image.width() && ty1 >= 0 && ty1 < image.height())
		image( tx1, ty1, 0, c) = color_point[c];
	    }
    }
  image.get_normalize(0,255).save(filename);
}


//Combines two images horizontally
CImg<double> combineImages(CImg<double> image_1, CImg<double> image_2);

//Find and return SIFT matching descriptor pairs
vector<d_pairs> siftMatchingPairs(vector<SiftDescriptor> descriptors_1, vector<SiftDescriptor> descriptors_2, double sift_threshold);

//Find the transformations T applied on set1 to get set2, i.e. set1 * T = set2
CImg<double> get_transformation(CImg<double> set1, CImg<double> set2);

CImg<double> ransac(vector<d_pairs> desc_pairs, double ransac_threshold, int ransac_iterations);

//Draw lines on combined image for SIFT matching pairs
void drawLines(CImg<double> combined_image, int img1_width, vector<d_pairs> desc_pairs, const char *filename);

//Draw lines on combined image for matching pairs after RANSAC
void drawLines(CImg<double> combined_image, int img1_width, vector<d_pairs> desc_pairs, CImg<double> final_transformation, double ransac_threshold, const char *filename);

int main(int argc, char **argv)
{
  try {
      // RANSAC
    	string if1 = argv[1];
		string if2 = argv[2];
		string inputFile_1 = "images/" + if1;
		string inputFile_2 = "images/" + if2;
		CImg<double> image_1(inputFile_1.c_str());
		CImg<double> image_2(inputFile_2.c_str());
		CImg<double> gray_1 = image_1.get_RGBtoHSI().get_channel(2);
		vector<SiftDescriptor> descriptors_1 = Sift::compute_sift(gray_1);
		CImg<double> gray_2 = image_2.get_RGBtoHSI().get_channel(2);
		vector<SiftDescriptor> descriptors_2 = Sift::compute_sift(gray_2);
		double sift_threshold = 0.65;
		double ransac_threshold = 3;
		int ransac_iterations = 2000;
		vector<d_pairs> desc_pairs = siftMatchingPairs(descriptors_1, descriptors_2, sift_threshold);
		CImg<double> combined_image = combineImages(image_1, image_2);
		if(desc_pairs.size() == 0) {
			cout << "No SIFT matches found..!!" << endl;
		} else if(desc_pairs.size() < 4) {
			drawLines(combined_image, image_1.width(), desc_pairs, "part3_sift_matches.jpg");
			cout << "Drawing Lines for SIFT matching pairs and writing to an output image" << endl;
			cout << "Less Than 4 SIFT matches found. Cannot proceed with RANSAC as we need atleast 4 points to find the projective transformation." << endl;
		} else {
			int num_pairs = desc_pairs.size();
			if(num_pairs > 20) {
				ransac_iterations = min(num_pairs*100,4000);
			}
			drawLines(combined_image, image_1.width(), desc_pairs, "part3_sift_matches.jpg");
			cout << "Drawing Lines for SIFT matching pairs and writing to part3_sift_matches.jpg" << endl;
			CImg<double> final_transformation = ransac(desc_pairs, ransac_threshold, ransac_iterations);
			drawLines(combined_image, image_1.width(), desc_pairs, final_transformation, ransac_threshold, "part3_ransac_matches.jpg");
			cout << "Drawing Lines for matching pairs after RANSAC and writing to part3_ransac_matches.jpg" << endl;
		}
	
  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}

CImg<double> combineImages(CImg<double> image_1, CImg<double> image_2) {
	int h1 = image_1.height();
	int w1 = image_1.width();
	int h2 = image_2.height();
	int w2 = image_2.width();
	int comb_h = max(h1, h2);
	int comb_w = w1 + w2;
	CImg<double> combined_img(comb_w, comb_h, 1, 3, 0);
	for(int i = 0; i < comb_h; i++) {
		if(i < h1) {
			for(int k = 0; k < w1; k++) {
				combined_img(k, i, 0, 0) = image_1(k, i, 0, 0);
				combined_img(k, i, 0, 1) = image_1(k, i, 0, 1);
				combined_img(k, i, 0, 2) = image_1(k, i, 0, 2);
			}
		}else {
			for(int k = 0; k < w1; k++) {
				combined_img(k, i, 0, 0) = 0;
				combined_img(k, i, 0, 1) = 0;
				combined_img(k, i, 0, 2) = 0;
			}
		}
		if(i < h2) {
			for(int l = 0; l < w2; l++) {
				combined_img(l + w1, i, 0, 0) = image_2(l, i, 0, 0);
				combined_img(l + w1, i, 0, 1) = image_2(l, i, 0, 1);
				combined_img(l + w1, i, 0, 2) = image_2(l, i, 0, 2);
			}
		}else {
			for(int l = 0; l < w2; l++) {
				combined_img(l + w1, i, 0, 0) = 0;
				combined_img(l + w1, i, 0, 1) = 0;
				combined_img(l + w1, i, 0, 2) = 0;
			}
		}
	}
	return combined_img;
}

double eucdist(vector<float> x, vector<float> y) {
	double sq_sum = 0;
	for(int d = 0; d < 128; d++) {
		sq_sum += pow(x[d] - y[d], 2);
	}
	sq_sum = sqrt(sq_sum);
	//cout << "SQ_SUM::" << sq_sum << endl;
	return sq_sum;
}

vector<d_pairs> siftMatchingPairs(vector<SiftDescriptor> descriptors_1, vector<SiftDescriptor> descriptors_2, double sift_threshold) {
	int n1 = descriptors_1.size();
	int n2 = descriptors_2.size();
	int count = 0;
	double dist;
	vector<d_pairs> desc_pairs;
	for(int i = 0; i < n1; i++) {
		double first_min = INT_MAX;
		double second_min = INT_MAX;
		int min_j = 0;
		d_pairs p;
		for(int j = 0; j < n2; j++) {
			dist = eucdist(descriptors_1[i].descriptor, descriptors_2[j].descriptor);
			if(dist < first_min && dist < second_min) {
				second_min = first_min;
				first_min = dist;
				min_j = j;
			}else if(dist > first_min && dist < second_min) {
				second_min = dist;
			}
		}
		p.dist = first_min;
		p.ssd_ratio = first_min/second_min;
		p.d1 = descriptors_1[i];
		p.d2 = descriptors_2[min_j];
		if(p.ssd_ratio < sift_threshold) {
			desc_pairs.push_back(p);
			count++;
		}
	}
	cout << "Number of Matches = " << count << endl;
	return desc_pairs;
}

/* Get the transformation matrix used to change the 4 points in set1 to the 4 points in set2
 * set1 and set2 are 8d vectors alternating x1 y1 x2 y2 ...
 * set1's x1' y1' correspond with set2's x1 y1
 * Resource used:  http://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/EPSRC_SSAZ/node11.html
*/
CImg<double> get_transformation(CImg<double> set1, CImg<double> set2) {
  CImg<double> result(3, 3, 1, 1, 1);

  // Create linear system
  CImg<double> system(8, 8, 1, 1, 0);
  system(0, 0, 0, 0) = set1(0, 0, 0, 0);  system(1, 0, 0, 0) = set1(0, 1, 0, 0);  system(2, 0, 0, 0) = 1; /* 0 0 0 */ system(6, 0, 0, 0) = -1 * set2(0, 0, 0, 0) * set1(0, 0, 0, 0);  system(7, 0, 0, 0) = -1 * set2(0, 0, 0, 0) * set1(0, 1, 0, 0);
  /* 0 0 0 */ system(3, 1, 0, 0) = set1(0, 0, 0, 0);  system(4, 1, 0, 0) = set1(0, 1, 0, 0);  system(5, 1, 0, 0) = 1; system(6, 1, 0, 0) = -1 * set2(0, 1, 0, 0) * set1(0, 0, 0, 0);  system(7, 1, 0, 0) = -1 * set2(0, 1, 0, 0) * set1(0, 1, 0, 0);

  system(0, 2, 0, 0) = set1(0, 2, 0, 0);  system(1, 2, 0, 0) = set1(0, 3, 0, 0);  system(2, 2, 0, 0) = 1; /* 0 0 0 */ system(6, 2, 0, 0) = -1 * set2(0, 2, 0, 0) * set1(0, 2, 0, 0);  system(7, 2, 0, 0) = -1 * set2(0, 2, 0, 0) * set1(0, 3, 0, 0);
  /* 0 0 0 */ system(3, 3, 0, 0) = set1(0, 2, 0, 0);  system(4, 3, 0, 0) = set1(0, 3, 0, 0);  system(5, 3, 0, 0) = 1; system(6, 3, 0, 0) = -1 * set2(0, 3, 0, 0) * set1(0, 2, 0, 0);  system(7, 3, 0, 0) = -1 * set2(0, 3, 0, 0) * set1(0, 3, 0, 0);

  system(0, 4, 0, 0) = set1(0, 4, 0, 0);  system(1, 4, 0, 0) = set1(0, 5, 0, 0);  system(2, 4, 0, 0) = 1; /* 0 0 0 */ system(6, 4, 0, 0) = -1 * set2(0, 4, 0, 0) * set1(0, 4, 0, 0);  system(7, 4, 0, 0) = -1 * set2(0, 4, 0, 0) * set1(0, 5, 0, 0);
  /* 0 0 0 */ system(3, 5, 0, 0) = set1(0, 4, 0, 0);  system(4, 5, 0, 0) = set1(0, 5, 0, 0);  system(5, 5, 0, 0) = 1; system(6, 5, 0, 0) = -1 * set2(0, 5, 0, 0) * set1(0, 4, 0, 0);  system(7, 5, 0, 0) = -1 * set2(0, 5, 0, 0) * set1(0, 5, 0, 0);

  system(0, 6, 0, 0) = set1(0, 6, 0, 0);  system(1, 6, 0, 0) = set1(0, 7, 0, 0);  system(2, 6, 0, 0) = 1; /* 0 0 0 */ system(6, 6, 0, 0) = -1 * set2(0, 6, 0, 0) * set1(0, 6, 0, 0);  system(7, 6, 0, 0) = -1 * set2(0, 6, 0, 0) * set1(0, 7, 0, 0);
  /* 0 0 0 */ system(3, 7, 0, 0) = set1(0, 6, 0, 0);  system(4, 7, 0, 0) = set1(0, 7, 0, 0);  system(5, 7, 0, 0) = 1; system(6, 7, 0, 0) = -1 * set2(0, 7, 0, 0) * set1(0, 6, 0, 0);  system(7, 7, 0, 0) = -1 * set2(0, 7, 0, 0) * set1(0, 7, 0, 0);

  // Solve linear system
  CImg<double> solutions = set2.solve(system);

  // Plug in solutions from linear system into transformation
  result(0, 0, 0, 0) = solutions(0, 0, 0, 0); result(1, 0, 0, 0) = solutions(0, 1, 0, 0); result(2, 0, 0, 0) = solutions(0, 2, 0, 0);
  result(0, 1, 0, 0) = solutions(0, 3, 0, 0); result(1, 1, 0, 0) = solutions(0, 4, 0, 0); result(2, 1, 0, 0) = solutions(0, 5, 0, 0);
  result(0, 2, 0, 0) = solutions(0, 6, 0, 0); result(1, 2, 0, 0) = solutions(0, 7, 0, 0); result(2, 2, 0, 0) = 1;

  return result;
}

CImg<double> ransac(vector<d_pairs> desc_pairs, double ransac_threshold, int ransac_iterations) {
	int num_pairs = desc_pairs.size();
	CImg<double> final_transformation(3,3,1,1);
	CImg<double> set1(1,8,1,1,1);
	CImg<double> set2(1,8,1,1,1);
	CImg<double> pixel(1,3,1,1,1);
	srand(time(0));
	double max_inliner_ratio = 0;
	for(int iter = 0; iter < ransac_iterations; iter++) {
		int r1 = rand()%(num_pairs-0 + 1) + 0;
		int r2 = rand()%(num_pairs-0 + 1) + 0;
		while(r1 == r2){
			r2 = rand()%(num_pairs-0 + 1) + 0;
		}
		int r3 = rand()%(num_pairs-0 + 1) + 0;
		while(r1 == r3 or r2 ==r3){
			r3 = rand()%(num_pairs-0 + 1) + 0;
		}
		int r4 = rand()%(num_pairs-0 + 1) + 0;
		while(r1==r4 or r2==r4 or r3==r4){
			r4 = rand()%(num_pairs-0 + 1) + 0;
		}

		set1(0,0,0,0) = desc_pairs[r1].d1.col;
		set1(0,1,0,0) = desc_pairs[r1].d1.row;
		set1(0,2,0,0) = desc_pairs[r2].d1.col;
		set1(0,3,0,0) = desc_pairs[r2].d1.row;
		set1(0,4,0,0) = desc_pairs[r3].d1.col;
		set1(0,5,0,0) = desc_pairs[r3].d1.row;
		set1(0,6,0,0) = desc_pairs[r4].d1.col;
		set1(0,7,0,0) = desc_pairs[r4].d1.row;
		set2(0,0,0,0) = desc_pairs[r1].d2.col;
		set2(0,1,0,0) = desc_pairs[r1].d2.row;
		set2(0,2,0,0) = desc_pairs[r2].d2.col;
		set2(0,3,0,0) = desc_pairs[r2].d2.row;
		set2(0,4,0,0) = desc_pairs[r3].d2.col;
		set2(0,5,0,0) = desc_pairs[r3].d2.row;
		set2(0,6,0,0) = desc_pairs[r4].d2.col;
		set2(0,7,0,0) = desc_pairs[r4].d2.row;

		CImg<double> transformation = get_transformation(set1, set2);
		int count = 0;
		double dist = 0;
		for(int m = 0; m < num_pairs; m++) {
			pixel(0,0,0,0) = desc_pairs[m].d1.col;
			pixel(0,1,0,0) = desc_pairs[m].d1.row;
			pixel(0,2,0,0) = 1;
			pixel = transformation * pixel;
			pixel(0,0,0,0) /= pixel(0,2,0,0);
			pixel(0,1,0,0) /= pixel(0,2,0,0);
			dist = sqrt(pow(desc_pairs[m].d2.col - pixel(0,0,0,0), 2) + pow(desc_pairs[m].d2.row - pixel(0,1,0,0), 2));
			if(dist < ransac_threshold) {
				count++;
			}
		}
		double inliner_ratio = static_cast<double>(count)/num_pairs;
		if(inliner_ratio > max_inliner_ratio) {
			max_inliner_ratio = inliner_ratio;
			final_transformation = transformation;
		}
	}
	return final_transformation;
}

void drawLines(CImg<double> combined_image, int img1_width, vector<d_pairs> desc_pairs, const char *filename) {
	const unsigned char color[] = { 0,255,0 };
	int num_pairs = desc_pairs.size();
	double dist = 0;
	for(int i = 0; i < num_pairs; i++) {
		int tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0;
		int sx1 = 0, sy1 = 0, sx2 = 0, sy2 = 0;
		double color_point[] = {255.0, 255.0, 0};
		for(int x=-2; x<3; x++) {
			for(int y=-2; y<3; y++) {
				if(x==0 || y==0) {
					for(int c=0; c<3; c++) {
						tx1 = (desc_pairs[i].d1.col + y - 1);
						ty1 = (desc_pairs[i].d1.row + x - 1);
						sx1 = (img1_width + desc_pairs[i].d2.col + y - 1);
						sy1 = (desc_pairs[i].d2.row + x - 1);
						combined_image( tx1, ty1, 0, c) = color_point[c];
						combined_image( sx1, sy1, 0, c) = color_point[c];
					}
				}
			}
		}
		float c1 = desc_pairs[i].d1.col;
		float r1 = desc_pairs[i].d1.row;
		float c2 = img1_width + desc_pairs[i].d2.col;
		float r2 = desc_pairs[i].d2.row;
		combined_image.draw_line(c1,r1,c2,r2,color);
	}
	combined_image.save(filename);
}

void drawLines(CImg<double> combined_image, int img1_width, vector<d_pairs> desc_pairs, CImg<double> final_transformation, double ransac_threshold, const char *filename) {
	const unsigned char color[] = { 0,255,0 };
	CImg<double> pixel_coords(1,3,1,1,1);
	int num_pairs = desc_pairs.size();
	double dist = 0;
	for(int i = 0; i < num_pairs; i++) {
		pixel_coords(0,0,0,0) = desc_pairs[i].d1.col;
		pixel_coords(0,1,0,0) = desc_pairs[i].d1.row;
		pixel_coords(0,2,0,0) = 1;
		pixel_coords = final_transformation * pixel_coords;
		pixel_coords(0,0,0,0) /= pixel_coords(0,2,0,0);
		pixel_coords(0,1,0,0) /= pixel_coords(0,2,0,0);
		dist = sqrt(pow(desc_pairs[i].d2.col - pixel_coords(0,0,0,0), 2) + pow(desc_pairs[i].d2.row - pixel_coords(0,1,0,0), 2));
		if(dist < ransac_threshold) {
			int tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0;
			int sx1 = 0, sy1 = 0, sx2 = 0, sy2 = 0;
			double color_point[] = {255.0, 255.0, 0};
			for(int x=-2; x<3; x++) {
				for(int y=-2; y<3; y++) {
					if(x==0 || y==0) {
						for(int c=0; c<3; c++) {
							tx1 = (desc_pairs[i].d1.col + y - 1);
							ty1 = (desc_pairs[i].d1.row + x - 1);
							sx1 = (img1_width + desc_pairs[i].d2.col + y - 1);
							sy1 = (desc_pairs[i].d2.row + x - 1);
							combined_image( tx1, ty1, 0, c) = color_point[c];
							combined_image( sx1, sy1, 0, c) = color_point[c];
						}
					}
				}
			}
			float c1 = desc_pairs[i].d1.col;
			float r1 = desc_pairs[i].d1.row;
			float c2 = img1_width + desc_pairs[i].d2.col;
			float r2 = desc_pairs[i].d2.row;
			combined_image.draw_line(c1,r1,c2,r2,color);
		}
	}
	combined_image.save(filename);
}

