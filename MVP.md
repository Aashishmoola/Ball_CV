# Creating a minimum viable product that seems to be working.

## Core modules:
1. Ball Detection (fine, takes longer): Image, timestamp(in image base name) --> Position of centre of ball (coordinates x, y), timestamp
    - Preprocessing: Folder of images, timestamp labelled --> Folder of images, t have features extracted maximally. 
    - Hugh Transform: Feature extracted images, t --> Position of centre of ball (coordinates x, y), t

2. Ball Detection (course): Sufficient radar interference by fast moving obj

3. Video Extraction: Compressed Video file -> Folder of images, timestamp labelled

4. Trigger Attatching: Polling Ball Detection (course) --> 2++ successful hits --> Start video

5. Trigger Detatching: Inverse Trigger Attatching

6. Data validation (course): N pos, N timestamp --> N-- pos, N-- timestamp (if pos > acceptable spread remove data)

7. Data validation & Processing (fine): N pos, N timestamp --> Pedictive algo (feedback error correction/mitigation) --> dist-time Eqn (graph, gradient --> velocity)

8. Metrics Calculation: dist-time Eqn, camera_pos --> Getting x - intercepts --> Max dist, Max height


## Status of each module: 

1. --> Have to test, dynamic threshold value calculation needs to be implemented and tested, histogram plot should be changed to matplot++
3. --> Have to test
6. --> Currently active


## General TODO
 TODO: Clean up ball detection.

  

## General Notes
Should use a middleware architecture so validation of cv::Mat types as they differ are done in layers and can be abstracted away/reused easily --> with muliple benifts

Getting a good threshold value that leads to maximum seperation between foreground and background for delta B image:
    Best: OTSU algorithm (Generates histogram for mat --> for all range of thresholds computes interclass variance --> returns threshold value with greatest variance)
    Ok: Using threshold that is two std dev away from mean

cv::Scalar --> maps to double[4], usually required to hold values for RGBA pixel values

