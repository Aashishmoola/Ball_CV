# Creating a minimum viable product that seems to be working.

## Core modules:
    - Ball Detection (fine, takes longer): Image, timestamp(in image base name) --> Position (coordinates x, y), timestamp
    - Ball Detection (course): Sufficient radar interference by fast moving obj
    - Video Extraction: Compressed Video file -> Folder of images, timestamp labelled
    - Trigger Attatching: Polling Ball Detection (course) --> 2++ successful hits --> Start video
    - Trigger Detatching: Inverse Trigger Attatching
    - Data validation (course): N pos, N timestamp --> N-- pos, N-- timestamp (if pos > acceptable spread remove data)
    - Data validation & Processing (fine): N pos, N timestamp --> Pedictive algo (feedback error correction/mitigation) --> dist-time Eqn (graph, gradient --> velocity)
    - Metrics Calculation: dist-time Eqn, camera_pos --> Getting x - intercepts --> Max dist, Max height

  
Should use a middleware architecture so validation of cv::Mat types as they differ are done in layers and can be abstracted away/reused easily --> with muliple benifts


