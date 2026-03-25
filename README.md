# Ball_CV

## Objectives/Plan for the project:
First Interation: 2D, no spin calculation.


# Ball Detection
    1. Preprocessing for maximal hough transform results
        - Validation (Ensure that data is correct of the same size and type for each image)
        - Backgroud Subtraction 
            1. Create map of delta brightness(del B) btw current and next frames -> Track (quick moving) artifacts -> faster motion greater change in del B (Maybe want to use background image for initiation). 
            2. Threshold to increase contrast between foreground(moving) and background(stationary)
            3. <!-- TODO: Artifact reduction -->
        - Thresholding (Is is better to use color for threshold detection)
    2. Hough Transform
    <!-- TODO : Feedback to hough transform with high conf val to crop to ROI to increase correct detection rate in futher iterations, ROI will move with expected velocity calculations. 


# Postion & velocity Calculation

# Position Projection

