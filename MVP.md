# Creating a minimum viable product that seems to be working.

## Core modules:
1. Ball Detection (fine, takes longer, eliptical? hugh transform): Image, timestamp(in image base name) --> Position of centre of ball (coordinates x, y), timestamp
    - Preprocessing: Folder of images, timestamp labelled --> Folder of images, t have features extracted maximally. 
    - Hugh Transform: Feature extracted images, t --> Position of centre of ball (coordinates x, y), t

2. Ball Detection (course, radar module): Sufficient radar interference by fast moving obj

3. Video Extraction: Compressed Video file -> Folder of images, timestamp labelled

4. Trigger: Polling Ball Detection (course) --> 2++ successful hits --> Start video

5. Trigger Detatching: Inverse Trigger Attatching

6. Data validation (course): N pos, N timestamp --> N-- pos, N-- timestamp (if pos > acceptable spread remove data)

7. Data validation & Processing (fine, kalman filter): N pos, N timestamp --> Pedictive algo (feedback error correction/mitigation) --> Accurate pos/vel coords

8. Metrics Calculation: 
    1. accurate pos/ vel coords, camera distance --> Map pixel to real world distance --> pos/vel coords in real world distance.
    2. pos/ ve in m --> --> Calculating drag coefficient from filtered data. Algorithmic approach to Euler's method (No spin --> Magnus effect) --> Getting max distance and height in m. 


## Status of each module: 

1. --> Have to improve
TODO: Dynamic threshold value calculation needs to be implemented and tested
TODO: Histogram plot should be changed to matplot++
3. --> Not necessary for actual implementation. Will only be used if data has to be steamed as a video instead of a jpeg.
7. --> Have to test and implement.
TODO: Return velocity estimations (full state)
TODO: Plot filter with matplot++ to see how many frames it takes to converge.
8. --> Implementing


## Trade Offs
1. For fast moving objects --> Exposure length will be too short to capture sufficient frames to calculate drag coeffcient --> Use Drag lookup drag coeffcient values.
2. Will using a large open_source libraries, like opnecv create large binaries like opencv which will not be able to run on esp32 performantly. 
3. Tradeoffs between Image type for fast processing applications. 

## General TODO

  

## General Notes
Should use a middleware (layered semantically) architecture so data manipulation and modelling are done in layers and can be abstracted away/validated/tested/ interated upon easily --> with muliple benifts

Getting a good threshold value that leads to maximum seperation between foreground and background for delta B image:
    Best: OTSU algorithm (Generates histogram for mat --> for all range of thresholds computes interclass variance --> returns threshold value with greatest variance)
    Ok: Using threshold that is two std dev away from mean

cv::Scalar --> maps to double[4], usually required to hold values for RGBA pixel values

