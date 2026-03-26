## Best practices
1. Keep hardcoded values for testing in the cpp file while the prod values should go in the hpp file. 
2. Use scoped enums for switches instead of bool vals (no semantic meaning);
3. Use consistenly named namespaces for each module
4. Global helpers module should contain functions that are really global --> Does opencv or other third party libs not have this helper function that you have to implement yourself?
5. In terms of naming conventions --> The larger the scope, the more distinct and readable the names for objects, rvalues has to be.

## How to write comments


## Specific guidelines for this project
- cv::Mat optimises copies by only copying headers of the matrix instead of entire matrix --> works like a shared pointer
    - Header (stack): rows, cols, type, step, and a uchar* data     pointer                                      
    - Data (heap): the actual pixel buffer, with a int* refcount alongside it 

1. Take in cv::Mat by ref --> To prevent unnecessary copy of headfer during param initialation
    Use const if you are not modifying the image directly, but instead creating a copy
2. Return cv::Mat directly --> Return value optimsed, anyways shallow header copy, dont use out_params unless another value has to be returned also.