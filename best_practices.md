## Design Thinking, shifting how you perceive, approach and breakdown the problem.
# Mental Models and Principles
- First Principles Thinking: Breaking complex problems down to their most basic, fundamental truths to rebuild innovative solutions. Asking why iteratively.
- Inversion: Thinking about a problem in reverse. Instead of asking how to succeed, ask, "How could this system fail?" or "What could go wrong?" to identify risks early.
- Second-Order Thinking: Considering the consequences of consequences. It involves looking beyond the immediate impact of a technical decision (e.g., adding a cache) to understand long-term effects (e.g., cache invalidation issues).
- Systems Thinking: Viewing software as a complex, interconnected system rather than isolated parts, understanding how components affect each other.
- Pareto Principle (80/20 Rule): Identifying the 20% of code that provides 80% of the value or causes 80% of the bugs, allowing for prioritized effort.
- Occam's Razor: Choosing the simplest solution with the fewest assumptions when faced with multiple design choices. 

# Process and Decision Frameworks
- Model-Based Systems Engineering (MBSE): Uses visual models rather than long written documents to manage complex systems, often utilizing languages like SysML. (Matlab Simulink)
- Failure Modes and Effects Analysis (FMEA): A step-by-step method to figure out what could go wrong, its severity, and its likelihood, used for robust design in critical systems.
- Type 1 vs. Type 2 Decisions (One-Way/Two-Way Doors): Classifying decisions based on reversibility. Irreversible (Type 1) decisions require deep thought, while reversible (Type 2) decisions favor speed



## Best practices
1. Keep hardcoded values for testing in the cpp file while the prod values should go in the hpp file. 
2. Use scoped enums for switches instead of bool vals (no semantic meaning);
3. Use consistenly named namespaces for each module
4. Global helpers module should contain functions that are really global --> Does opencv or other third party libs not have this helper function that you have to implement yourself?
5. In terms of naming conventions --> The larger the scope, the more distinct and readable the names for objects, rvalues has to be.

## How to write comments


## Specific guidelines for this project
- Make sure to upload into git after each iteration.
- Make sure to use claude CLI with claude getting updated context to MVP.md to answer specifically to context. 
- cv::Mat optimises copies by only copying headers of the matrix instead of entire matrix --> works like a shared pointer
    - Header (stack): rows, cols, type, step, and a uchar* data     pointer                                      
    - Data (heap): the actual pixel buffer, with a int* refcount alongside it 

1. Take in cv::Mat by ref --> To prevent unnecessary copy of headfer during param initialation
    Use const if you are not modifying the image directly, but instead creating a copy
2. Return cv::Mat directly --> Return value optimsed, anyways shallow header copy, dont use out_params unless another value has to be returned also.