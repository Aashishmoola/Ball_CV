# Creating a minimum viable product for my ball tracking product.


## Core modules:
1. Ball Detection (fine, takes longer, Contour detection(Elipse)): Image, timestamp(in image base name) --> Position of centre of ball (coordinates x, y), timestamp
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


## Status of each module/ General TODO: 
### Mark with TODO for what has to be done today.

TODO: Should all constants be grouped together in the same file, to be tuned while those that are static unless architecture change remain in the modules

1. --> Have to improve
    - TODO Implement circles to ball state conversion function (Convert to metric also after camera calibration)
    - Clean-up the helper functions; rename semantically
3. --> Not necessary for actual implementation. Will only be used if data has to be steamed as a video instead of a jpeg.
7. --> Have to link -> test
    - Implement main loop itself
    - Return velocity estimations (full state)
    - Plot filter with matplot++ to see how many frames it takes to converge.
8. --> Have to link -> test
    - TODO Remove convert to metric ball option.



## Trade Offs
1. For fast moving objects --> Exposure length will be too short to capture sufficient frames to calculate drag coeffcient --> Use Drag lookup drag coeffcient values.
2. Will using a large open_source libraries, like opnecv create large binaries like opencv which will not be able to run on esp32 performantly. 
3. Tradeoffs between Image type for fast processing applications. 
4. Diff types of ball detection algo arcitecture:
    1. Hough transform(raw, blurred, greyscale image) : Slow, inaccurate does not fully utilise countour finding from del B images, performs well on occluded and eliptical ball artifacts
    2. Skytrax: Faster, used del B image, does not perform well on occluded and eliptical ball artifacts
        raw image -> Background sub -> Thresholding -> Contouring --> approx radius, centre coords + ROI
        Sobel Edge Detection on ROI --> edges -> dir of edges
        PPV on direction of edges --> accurate radius, centre coords


## General Notes
1. Should use a middleware (layered semantically) architecture so data manipulation and modelling are done in layers and can be abstracted away/validated/tested/ interated upon easily --> with muliple benifts

2. Getting a good threshold value that leads to maximum seperation between foreground and background for delta B image:
    Best: OTSU algorithm (Generates histogram for mat --> for all range of thresholds computes interclass variance --> returns threshold value with greatest variance)
    Ok: Using threshold that is (two) std dev away from mean

cv::Scalar --> maps to double[4], usually required to hold values for RGBA pixel values



