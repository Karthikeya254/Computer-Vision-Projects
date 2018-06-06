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


CImgList<double> gaussian_pyramid(const CImg<double> image, int levels, CImg<double> filter);

CImgList<double> laplacian_pyramid(const CImg<double> image, int levels, CImg<double> filter);

CImg<double> blend_images(CImg<double> image1, CImg<double> image2, const CImg<double> mask, const CImg<double> filter, int levels)

CImg<double> half_blend(CImg<double> image1, CImg<double> image2, const CImg<double> filter);

CImgDisplay half_blend_panel(CImg<double> image1, CImg<double> image2, const CImg<double> filter);

int main(int argc, char **argv)
{
  try {
	  
      // Blending
      double g[25] = {1, 4, 6, 4, 1,
                        4, 16, 24, 16, 4,
                        6, 24, 36, 24, 6,
                        4, 16, 24, 16, 4,
                        1, 4, 6, 4, 1};

      for (int i = 0; i < 25; i++){
        g[i] = g[i]/256.0;
      }

      CImg<double> gaus(g, 5, 5, 1, 1);

      char* file1 = argv[1];
      char* file2 = argv[2];
      char* maskfile = argv[3];
      CImg<double> image1(file1);
      CImg<double> image2(file2);
      CImg<double> mask_assign(maskfile);

      CImgList<double> gau1 = gaussian_pyramid(image1, 5, gaus);
      CImgList<double> gau2 = gaussian_pyramid(image2, 5, gaus);
      for (int i=0; i < gau1.size(); i++){
        gau1(i).normalize(0, 255);
        gau2(i).normalize(0, 255);
      }
      gau1.save("images/gau1.png");
      gau2.save("images/gau2.png");

      CImgList<double> lap1 = laplacian_pyramid(image1, 5, gaus);
      CImgList<double> lap2 = laplacian_pyramid(image2, 5, gaus);
      for (int i=0; i < lap1.size(); i++){
        lap1(i).normalize(0, 255);
        lap2(i).normalize(0, 255);
      }
      lap1.save("images/lap1.png");
      lap2.save("images/lap2.png");

      CImg<double> b = blend_images(image1, image2, mask_assign, gaus, 5);
      b.normalize(0, 255);
      b.save("images/blended.png");
  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}


CImg<double> half_mask(const CImg<double> image){
  CImg<double> value(image, false);
  value = 0.0;
  for (int r=0; r < value.height(); r++){
    for (int c=0; c < value.width()/2; c++){
      for (int s=0; s < value.spectrum(); s++){
        value(c, r, 0, s) = 255;
      }
    }
  }
  return value;
}

CImg<double> upscale(const CImg<double> image, const CImg<double> size){
  double g[25] = {1, 4, 6, 4, 1,
                    4, 16, 24, 16, 4,
                    6, 24, 36, 24, 6,
                    4, 16, 24, 16, 4,
                    1, 4, 6, 4, 1};

  for (int i = 0; i < 25; i++){
    g[i] = g[i]/256.0;
  }
  CImg<double> gaus(g, 5, 5, 1, 1);
  gaus*=4.0;

  CImg<double> return_v(size.width(), size.height(), size.depth(), size.spectrum());
  return_v = 0.0;
  for (int w=0; w < image.width(); w++){
    for (int h=0; h < image.height(); h++){
      for (int d=0; d < image.depth(); d++){
        for (int s=0; s < image.spectrum(); s++){
          return_v(2*w, 2*h, d, s) = image(w, h, d, s);
        }
      }
    }
  }
  return_v.convolve(gaus, false, false);
  return return_v;
}

CImg<double> upscale(const CImg<double> image){
  double g[25] = {1, 4, 6, 4, 1,
                    4, 16, 24, 16, 4,
                    6, 24, 36, 24, 6,
                    4, 16, 24, 16, 4,
                    1, 4, 6, 4, 1};

  for (int i = 0; i < 25; i++){
    g[i] = g[i]/256.0;
  }
  CImg<double> gaus(g, 5, 5, 1, 1);
  gaus*=4.0;

  CImg<double> return_v(image.width() * 2, image.height() * 2, image.depth(), image.spectrum());
  return_v = 0.0;
  for (int w=0; w < image.width(); w++){
    for (int h=0; h < image.height(); h++){
      for (int d=0; d < image.depth(); d++){
        for (int s=0; s < image.spectrum(); s++){
          return_v(2*w, 2*h, d, s) = image(w, h, d, s);
        }
      }
    }
  }
  return_v.convolve(gaus, false, false);
  return return_v;
}

CImgList<double> gaussian_pyramid(const CImg<double> image, int levels, CImg<double> filter){
  CImg<double> current(image, false);
  CImgList<double> result((unsigned int) 0);
  for (int i=0; i < levels; i++){
    result.push_back(current);
    current.convolve(filter, false, false);

    CImg<double> new_image(current.width()/2 , current.height()/2, current.depth(), current.spectrum());
    for (int r=0; r < new_image.width(); r++){
      for (int c=0; c < new_image.height(); c++){
        for (int s=0; s < new_image.spectrum(); s++){
          new_image(r, c, 0, s) = current(2*r, 2*c, 0, s);
        }
      }
    }
    current = new_image;
  }
  return result;
}

CImgList<double> laplacian_pyramid(const CImg<double> image, int levels, CImg<double> filter){
  CImgList<double> gaussians = gaussian_pyramid(image, levels, filter);
  CImg<double> current;
  CImg<double> current_blurred;

  CImgList<double> result((unsigned int) 0);

  for (int i=0; i < (levels - 1); i++){
    current = gaussians(i);
    current_blurred = upscale(gaussians(i + 1), current);
    current-=current_blurred;
    result.push_back(current);
  }
  result.push_back(gaussians(levels - 1));
  return result;
}

CImgList<double> blended_pyramid(const CImg<double> image2, const CImg<double> image1, const CImg<double> mask, const CImg<double> filter, int levels){
  CImgList<double> result((unsigned int) 0);
  CImgList<double> lap_pyr1 = laplacian_pyramid(image1, levels, filter);
  CImgList<double> lap_pyr2 = laplacian_pyramid(image2, levels, filter);
  CImgList<double> mask_gaus_pyr = gaussian_pyramid(mask, levels, filter);

  for (int i=0; i < levels; i++){
    mask_gaus_pyr(i).normalize(0, 1);
    CImg<double> current(lap_pyr1(i), false);
    for (int r=0; r < current.width(); r++){
      for (int c=0; c < current.height(); c++){
        for (int s=0; s < current.spectrum(); s++){
          //assignment of pixels for each level
          current(r, c, 0, s) = (mask_gaus_pyr(i, r, c, 0, 0) * lap_pyr1(i, r, c, 0, s)) + ((1.0 - mask_gaus_pyr(i, r, c, 0, 0)) * lap_pyr2(i, r, c, 0, s));
        }
      }
    }
    result.push_back(current);
  }

  return result;
}

CImg<double> blend_images(CImg<double> image1, CImg<double> image2, const CImg<double> mask, const CImg<double> filter, int levels){
  if (image1.size() < image2.size()){
    image2.resize(image1, 1, 0, 0.5, 0.5, 0, 0);
  }
  else{
    image1.resize(image2, 1, 0, 0.5, 0.5, 0, 0);
  }

  CImgList<double> blend = blended_pyramid(image1, image2, mask, filter, levels);
  CImgList<double> gaussians = gaussian_pyramid(image1, levels, filter);
  CImg<double> cur;
  CImg<double> new_filter = filter*4.0;

  cur = blend(levels - 1);
  for (int i=(levels - 2); i >= 0; i--){
    CImg<double> doubled = upscale(cur, blend(i));
    doubled+=blend(i);
    cur = doubled;
  }
  return cur;
}

CImg<double> half_blend(CImg<double> image1, CImg<double> image2, const CImg<double> filter){
  if (image1.size() < image2.size()){
    image2.resize(image1, 1, 0, 0.5, 0.5, 0, 0);
  }
  else{
    image1.resize(image2, 1, 0, 0.5, 0.5, 0, 0);
  }
  CImg<double> mask = half_mask(image1);
  CImg<double> result = blend_images(image1, image2, mask, filter, 5);
  return result;
}

CImgDisplay half_blend_panel(CImg<double> image1, CImg<double> image2, const CImg<double> filter){
  if (image1.size() < image2.size()){
    image2.resize(image1, 1, 0, 0.5, 0.5, 0, 0);
  }
  else{
    image1.resize(image2, 1, 0, 0.5, 0.5, 0, 0);
  }
  CImg<double> mask = half_mask(image1);
  CImg<double> result = blend_images(image2, image1, mask, filter, 5);
  result.normalize(0, 255);
  result.save("images/cat_blend.png");

  CImgDisplay f((image1, result, image2), "panel");
  return f;
}
