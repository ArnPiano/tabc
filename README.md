### Assignment:
Write a function performing the tri-aspect permutation test.

The function must work in the R environment, be statistically correct, and should be at least 10% more efficient than the given Tabc function.

### Results:
The function is statistically correct and takes 15% of the time of the unefficient R one.
The code is compatible with C++03, therefore some modern functionalities are missing (tuples, lambda functions, etc...)

### To Do
Verify if the following improve computation:
- Avoid usage of pairs and tuples altoghether.
- Parallelize computation through RcppThread or similar libraries.
- Use Rccp types and syntactic sugar.
