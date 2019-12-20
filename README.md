### Rules for the Code Run

1) speed race is not compulsory
2) it is not a group project
3) the function that you should improve is attached [included in the test.R file](Tabc test using 1000000 permutations), your function should run in R.
4) your function should be statistically correct
5) if you win and your code is faster than mine for at least 10% of duration time you will get a two question bonus in the written exam.
6) if you win but your code is faster than mine for less than 10% of duration time you will get a one question bonus in the written exam.
7) if you do not win but your code is faster than mine for at least 10% of duration time you will get a one question bonus in the written exam.
8) deadline: last lesson of the course December, 19th, during with you will briefly discuss your function and show the computing time of it using the following code.

``` R
start.time = proc.time()

# your procedure

duration.time = proc.time() - start.time
```
Write a function performing the tri-aspect permutation test.

The function must work in the R environment, be statistically correct, and should be at least 10% more efficient than the given Tabc function.

### Results:

Some tests were done in R and I saw an improvement of 10% by eliminating superfluous conditional statements and uneccessary operations.
It was soon clear that a lower level language was needed to improve the code further.
I adopted the Rcpp library to speed up computation.
The function is statistically correct and takes 15% of the time of the unefficient R one.
The code is compatible with C++03, therefore some modern functionalities are missing (tuples, lambda functions, etc...)

### To Do

It is known that `rand() % n` does not generate a uniform distribution; C++ offers ways to generate uniform distributions that are included in the C++11 standard and were not used in this project, but they can and should be adopted in stead of the current method.

Verify if the following improve computation:
- Avoid usage of pairs and tuples altoghether.
- Parallelize computation through RcppThread or similar libraries.
- Use Rccp types and syntactic sugar.
