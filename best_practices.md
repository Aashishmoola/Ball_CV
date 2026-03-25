## Best practices
1. Keep hardcoded values for testing in the cpp file while the prod values should go in the hpp file. 
2. Use scoped enums for switches instead of bool vals (no semantic meaning);
3. Use consistenly named namespaces for each module
4. Global helpers module should contain functions that are really global --> Does opencv or other third party libs not have this helper function that you have to implement yourself?

## How to write comments