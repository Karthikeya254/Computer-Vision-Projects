# Image Matching using SIFT descriptors and RANSAC Algorithm

#### Running the Code
Run make to compile the program and then input the command
```
./match eiffel_18.jpg eiffel_19.jpg
```
with eiffel_18.jpg being source image and eiffel_19.png being destination image between which the matching points needs to be computed. It outputs two images one with SIFT matching pairs "part3_sift_matches.jpg" and another with matching pairs after applying RANSAC "part3_ransac_matches.jpg"

#### Overview
In this task we are trying to find the matching feature points between two given images so as to identify if both the images belong to same location/place/area. First we apply SIFT on both the images and retrieve feature points (128-dimensional descriptor) in each image. For each of the descriptor in first image we find the ratio of the Euclidean distances between closest descriptor and next closest descriptor from the second image. The range of these ratio values will be between 0 and 1, values closer to 0 depicting similar points and values closer to 1 depicting least similarity. We tried a set of values to check the empirical threshold which maximizes the probability of selecting correct matches at the same time eliminating the noisy matches. We decided this threshold to be **_0.65_** and named it as **_sift_threshold_**, considering any threshold more than that though increases the probability of finding correct matches also increases noisy matches.

After filtering out the SIFT matching descriptor-pairs, we used the RANSAC algorithm to check the geometric arrangement of these descriptor-pairs. The implementation of this algorithm goes with selecting 4 random descriptor-pairs with which the projective transformation (homography) is computed. The first points in a descriptor-pair is transfered and the transformed point is decided to be an inliner if it is in a certain Euclidean distance threshold from the original second point in the pair. This threshold is decided to be **_3_** and named as **_ransac_threshold_**. This procedure is repeated multiple times so as to find the best projective transformation that results in maximum number of inliners. Number of **_iterations_** are fixed as **_2000_** if number of SIFT matching pairs is less than 20, and if it greater than 20 number of iterations is computed as min(num_pairs\*100, 4000).

#### Qulaitative Tests

##### Test_1:
```
./match eiffel_18.jpg eiffel_19.jpg
```
Number of Matches = 37  
Drawing Lines for SIFT matching pairs and writing to part3_sift_matches.jpg  
Drawing Lines for matching pairs after RANSAC and writing to part3_ransac_matches.jpg  

**sift_matches_eiffel**
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Matching%20RANSAC/part3_sift_matches_eiffel.jpg "part3_sift_matches_eiffel")

**ransac_matches_eiffel**
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Matching%20RANSAC/part3_ransac_matches_eiffel.jpg "part3_ransac_matches_eiffel")

##### Test_2:
```
./match bigben_2.jpg bigben_3.jpg
```
Number of Matches = 26  
Drawing Lines for SIFT matching pairs and writing to part3_sift_matches.jpg  
Drawing Lines for matching pairs after RANSAC and writing to part3_ransac_matches.jpg  

**sift_matches_bigben**
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Matching%20RANSAC/part3_sift_matches_bigben.jpg "part3_sift_matches_bigben")

**ransac_matches_bigben**
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Image%20Matching%20RANSAC/part3_ransac_matches_bigben.jpg "part3_ransac_matches_bigben")

##### Test_3:
```
./match eiffel_18.jpg bigben_3.jpg
```
Number of Matches = 0  
No SIFT matches found..!!
