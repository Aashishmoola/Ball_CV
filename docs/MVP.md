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

MVP 1: Done 
Cleanup 1: 
DONE: Readup on how imports should be managed with this modular code structure.
TODO Some types, stdlib seem to be leaking from the header file to th associated cpp file 
TODO Most suitable approach to error handling
    1. Wrap the main running function in a try catch block that catches the errors and log it in final catch all layers
    2. Each moudule must have its own robust yet flexible error handling, if they raise an error to main, then it means that the error is programme breaking.
    Change all logging errors to thrown errors if required or just log in non-breaking
    Use assert to check alignment of data within modules. Should raise errors within modules as they can be serious.
TODO Consider per module imports in CMakeLists.txt. (10 mins)
TODO Understang how testing is done in c++. (30 mins)


Some production level improvemnts to the code:
1. Make programme so it can be portable (run in a external environment)
2. Error handling and input and output boundaries
3. Use a standardised method of logging
4. Start creating tests for each single moudule an then a full end-to-end regression test
5. Implement necessary compile options and sanitizer build
6. FIX FIXME's and dead branch in kf_2D
7. Reconfigure to naming convetions using claude, implement going forward
8. Create a github action workflow to build and test before pushing to github
9. Run a stress test metrics and see if certain efficiency parameters can be improved.


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



