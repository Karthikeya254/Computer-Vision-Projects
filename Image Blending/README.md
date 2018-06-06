# Image Blending

#### Running the Code
In order to run the code enter the following commands in the project directory
```
make
./blend image1.png image2.png mask.png
```
with image1, image2, and mask all being the same size images.

#### Overview
In order to make all the output images clear and human viewable, they have been normalized between 0 and 255.

A few helper functions have also been implemented to make the code more readable, including

- upscale, for the gaussian and laplacian pyramids

- half_mask, for automatically generating masks split down the middle for two images

- half_blend, for atomatically blending two images down the middle

- half_blend_panel, for creating a display of two images blended down the middle


- gaussian_pyramid, returning a list containing each level of the gaussian pyramid

- laplacian_pyramid, returning a list containing each level of the laplacian pyramid

- blended_pyramid, returning a list containing the non-combined blended pyramid


The basic function for blending two images is

- blend_images, which takes both images, a mask, a filter, and the number of pyramid levels

which returns the final blended image.

The blend_images function only requires the mask to be the same size as the smallest image in order to function properly, if the two images are different sizes, the larger one is resized to the smaller one's size and centered around the middle of the image.

The blending working on the apple and orange example can be seen below, along with the intermediate pyramids.

![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/apple_dis.jpg "apple")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/blended_dis.png "blended")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/orange_dis.jpg "orange")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/mask_dis.jpg "mask")

Apple pyramids

![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau1_000000.png "gaussian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau1_000001.png "gaussian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau1_000002.png "gaussian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau1_000003.png "gaussian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau1_000004.png "gaussian pyramid")


![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap1_000000.png "laplacian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap1_000001.png "laplacian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap1_000002.png "laplacian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap1_000003.png "laplacian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap1_000004.png "laplacian pyramid")


Orange pyramids

![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau2_000000.png "gaussian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau2_000001.png "gaussian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau2_000002.png "gaussian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau2_000003.png "gaussian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/gau2_000004.png "gaussian pyramid")

![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap2_000000.png "laplacian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap2_000001.png "laplacian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap2_000002.png "laplacian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap2_000003.png "laplacian pyramid")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/lap2_000004.png "laplacian pyramid")

Another interesting application of this blending program is the prototyping of future wardrobe options, as can be seen below.

![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/tru_dis.png "trump")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/blendedtrump_good_dis.png "blended")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/mel_dis.png "melania")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/trumpmask_good_dis.png "mask")

The mask made a huge difference in the quality of the result, as can be seen with the same images blended with a more simple mask.

![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/tru_dis.png "trump")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/blendedtrump_half_dis.png "blended")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/mel_dis.png "melania")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Blending/images/trumpmask_half_dis.png "mask")
