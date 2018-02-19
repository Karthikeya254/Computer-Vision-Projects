## Embedding and Detecting Watermark in Frequency Domain

### Visualizing Image in Frequency Domain
#### Running the Code
Run make to compile the program and then input the command
```
./watermark 1.1 input_image.png out_image.png
```
with input_image as the image to create the spectogram, with out_image being the name of the resulting png file.

#### Overview
In order to make the output png clear, the magnitude of the image is normalized to have values between 0 and 255.

You can see a sample spectogram of Yoshi below.

![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Embedding%20Watermark/yoshi.png "yoshi")
![alt text](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Embedding%20Watermark/yoshi-spec.png "yoshi spectogram")

### Watermark
#### Running the Code
Run make to compile the program and then input the command

To add watermark:
```
./watermark 1.3 Lenna.png marked_Lenna.png add 333
```

To check watermark:
```
./watermark 1.3 marked_Lenna.png marked_Lenna.png check 333
```

To test the robustness:
```
./watermark 1.3 Lenna.png marked_Lenna.png quant_test 333
```


#### Overview
We are injecting the watermark by generating a random binary vector by seeding a given N, which makes surte that the results are reproducable and also to fetch the same binary vector to check an elready embedded watermark. To embed the water mark first the image is converted to its fourier domain, and then the binary vector is used to update the selected bins in the real portion of DFT.

The bins selected are equidistant from each other across a circle of radius *r* and every bin has a corresponding bin selected which is symmetric about the origin. So, for a fixed *l* (length of the binary vector) we have updated *2l* bins using the formula R'(x,y) = R(x,y) + alpha\*|R(x,y)|\*v<sub>i</sub>, for a selected *alpha*. As the updated bin values are a linear combination of the original bin values and the embedded binary vector, we decided to use either [Pearson Correlation Coefficient](https://en.wikipedia.org/wiki/Pearson_correlation_coefficient) or [Point-Biserial Coefficient](https://en.wikipedia.org/wiki/Point-biserial_correlation_coefficient) (which is better when calculating correlation between a continuous vector and a dichotomous vector). Both these metrics gave almost similar results, so we went ahead with Pearson Correlation Coefficient. Below are the optimum values used for the parameters to handle this task.

- *r* = 8
- *l* = 32
- *alpha* = 0.9
- *t* = 0.3

#### Parameter Selection and Issues:
- As *r* values increases the bins are selected from a sparse area and is very difficult to detect the modified values after embedding. At the same time if *r* is very small it limited the number of bins (*l*) that we select because the points on these circles are finite and the maximum *l* that can be selected reduces with *r*. So we searched on a grid of values for different images and fixed the *r* value to be 8.
- Similarly small values for *l* made it difficult to find the embedded vectors which increased False negatives. Increasing *l* increased the probability of finding the embedded vectors, but a higher *l* had a lot of False Positives.
- Selecting *alpha* turned out to be very interesting, because having higher values gave very good results both in terms of accuracy and reducing False Positives, but higher values of *alpha* (eg. 2, 3 etc.) introduced noticeable distortions onto the image. So we have fixed *alpha* value to be 0.9 as a tradeoff between the accuracy and image distortion.
- Correlation coefficient threshold is the easiest one to select as a lot of insights were drawn with all the earlier results while selecting remaining parameters. Having *t* = 0.4 along with higher alpha gave tremendous results, but as we had to lower *alpha* value we tried a lower treshold and fixed to 0.3. Much lower values of the threshold increased both False Negatives and False Positives.

#### Qualitative Test:

**Before Embedding watermark**
![](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Embedding%20Watermark/Lenna.png) 

**After Embedding watermark**
![](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Embedding%20Watermark/marked_Lenna.png)

```
./watermark 1.3 Lenna.png marked_Lenna.png add 333

./watermark 1.3 marked_Lenna.png marked_Lenna.png check 333
```

In: marked_Lenna.png  Out: Lenna.png  
image rows: 512  
image columns: 512  
N value: 333  
1.3  
Watermark detected..!  


**Before Embedding watermark**
![](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Embedding%20Watermark/cleaned.png) 

**After Embedding watermark**
![](https://github.com/Karthikeya254/Computer-Vision-Projects/blob/master/Embedding%20Watermark/marked_cleaned.png)

```
./watermark 1.3 cleaned.png marked_cleaned.png add 333

./watermark 1.3 marked_cleaned.png marked_cleaned.png check 333
```

In: marked_cleaned.png  Out: marked_cleaned.png  
image rows: 512  
image columns: 512  
N value: 333  
1.3  
Watermark detected..!  


#### Quantitative Test:
To check the robustness of this water mark detection we embedded a water mark corresponding to a given number N, and checked whether check_image function can detect this watermark. Later we took 100 random N's and cheked whether check_image function detects any of these 100 water marks, if detected they are counted as False Positives.

```
./watermark 1.3 Lenna.png marked_Lenna.png quant_test 333
```
Watermark Survived..!  
False Positves = 13

```
./watermark 1.3 cleaned.png marked_cleaned.png quant_test 333
```
Watermark Survived..!  
False Positives = 16

