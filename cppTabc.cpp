#include <Rcpp.h>
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//
#include <algorithm>
#include <ctime>
#include <vector>
#include <string>
//#include <random>
//#include <iostream>

using namespace std;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//

// Calculate median of a c++ vector
double cpp_median(const std::vector<double>& y){
  std::vector<double> x = y;
  std::size_t size = x.size();
  std::sort(x.begin(), x.end());
  if (size  % 2 == 0) return (x[size / 2 - 1] + x[size / 2]) / 2.0;
  return x[size / 2];
}

// Creates a unique c++ vector of tuples containing the original element, 
//  an indicator variable for Tb test, and the rank of the element
vector<pair<double,pair<bool, int> > > createTestVector(const std::vector<double>& x){
  double med = cpp_median(x);
  unsigned n = x.size();
  vector<pair<double,pair<bool, int> > > res(n);
  vector<pair<double, int> > v(n);
  for (unsigned i=0; i<n; ++i){
    v[i].first=x[i];
    v[i].second=i;
  }
  sort(v.begin(), v.end());
  for (unsigned i=0; i<n; ++i){
    res[v[i].second].first = v[i].first;
    res[v[i].second].second.second = i+1;
    res[v[i].second].second.first = res[v[i].second].first >= med ;
  }
  return res;
}

// Performs the Ta test on the c++ vector of tuples
double  Ta(const vector<pair<double,pair<bool, int> > >& x, unsigned n1, const vector<int>& indexes ){
  int  tot=0;
  unsigned i = 0;
  for ( ; i < n1;++i){
    tot += x[indexes[i]].first;
  }
  for ( ; i < x.size(); ++i){
    tot -= x[indexes[i]].first;
  }
  return tot;
}

// Performs the Tb test on the c++ vector of tuples
int Tb(const vector<pair<double,pair<bool, int> > >& x, unsigned n1, const vector<int>& indexes){
  int tot = 0;
  for (unsigned i=0 ; i < n1; ++i){
    tot+=x[indexes[i]].second.first;
  }
  for (unsigned i = n1 ; i < x.size(); ++i){
    tot-=x[indexes[i]].second.first;
  }
  return tot;
}

// Performs the Tc test on the c++ vector of tuples
int Tc (const vector<pair<double,pair<bool, int> > >& x, unsigned n1 , const vector<int>& indexes){
  int tot = 0;
  for (unsigned i = 0 ; i < n1; ++i){ tot += x[indexes[i]].second.second; }
  for (unsigned i = n1; i < x.size(); ++i){ tot-= x[indexes[i]].second.second; }
  return tot;
}

// Permutes a vector of integers (used for index permutation)
vector<int> permute(const vector<int>& x ){
  vector<int> res = x;
  int n = res.size();
  for (vector<int>::iterator it = res.begin(); it != res.end(); ++it){
    swap(*it, res[rand()%n]);
  }
  return res;
}

// Comparison function using indicator variables to signal the alternative [optimizable]
int compare(bool l, bool g, bool t, double x, double y){
  return l*(x<=y)+g*(x>=y)+t*(std::abs(x)>=std::abs(y));
}
 
/*
int compare(bool l, bool g, bool t, int x, int y){
  return l*(x<=y)+g*(x>=y)+t*(std::abs(x)>=std::abs(y));
}
 */

// [[Rcpp::export]]
double cppTabc(const NumericVector& x1,const NumericVector& x2 , const string& alt, unsigned B = 1000000){
  bool g,l,t = false; // Indicator variables for the alternatives
  unsigned n1 = x1.size(); // Size of first input vector
  unsigned n = n1 + x2.size(); // Size of both input vectors
  std::vector<double> input = std::vector<double>(n);
  // Creates a c++ vector with first input vector (easier to handle withoud in-depth knowledge of Rcpp library)
  for (unsigned i=0; i < n1; ++i){
    input[i] = x1[i];
  }
  // Adds the second input vector's element to the c++ vector
  for (unsigned i=0; i<x2.size(); ++i){
    input[n1+i] = (x2[i]);
  }
  // Set the alternative's indicator variables
  if(alt[0] == 'l'){ l = true;}
  else if (alt[0] == 'g'){g = true;}
  else if (alt[0] == 't'){t = true;}
  // Create a c++ vector of tuples
  vector<pair<double,pair<bool, int> > > x = createTestVector(input);
  // Create a vector of indexes used for permutations
  vector<int> indexes(n);
  for (unsigned i=0; i<n;++i){
    indexes[i]=i;
  }
  // Calculate the objective of the respective tests
  double Ta_ob = Ta(x, n1, indexes);
  double Tb_ob = Tb(x, n1, indexes);
  double Tc_ob = Tc(x, n1, indexes);
  // Create a vector of permutations
  vector<double> Ta_perm(B);
  vector<double> Tb_perm(B);
  vector<double> Tc_perm(B);
  // Define p-value variables representing the "p-value times B" of respective test
  double pv_Ta, pv_Tb, pv_Tc = 0;
  // Populate permutation vectors and initialize p-values above
  for (unsigned b = 0; b < B; ++b){
    vector<int> permutation = permute(indexes);
    Ta_perm[b] = Ta(x, n1, permutation);
    Tb_perm[b] = Tb(x, n1, permutation);
    Tc_perm[b] = Tc(x, n1, permutation);
    pv_Ta+= compare(l,g,t,Ta_perm[b],Ta_ob);
    pv_Tb+= compare(l,g,t,Tb_perm[b],Tb_ob);
    pv_Tc+= compare(l,g,t,Tc_perm[b],Tc_ob);
  }
  // Initialize objective p-value to minimum of the p-values above
  double pv_Tabc_ob = min( pv_Ta, min(pv_Tb, pv_Tc) );
  // Initialize result variable to 0 times B
  double pv_Tabc = 0;
  // Assign correct value to the result
  for (unsigned b = 0; b < B; ++b){
    // Initialize p-value of permutations
    double pv_Ta_perm = 0;
    double pv_Tb_perm = 0;
    double pv_Tc_perm = 0;
    // Assign correct permutation Ta p-value
    for (unsigned k=0; k<B;++k){
      pv_Ta_perm+= compare(l,g,t,Ta_perm[k],Ta_perm[b]);
    }
    // Avoid computation if possible
    if (pv_Ta_perm <= pv_Tabc_ob ){
      pv_Tabc+=1;
    }
    // Or else assign correct permutation to Tb p-value
    else {
      for (unsigned k=0; k<B; ++k){
        pv_Tb_perm +=compare(l,g,t,Tb_perm[k],Tb_perm[b]);
      }
      // // Avoid computation if possible, again
      if (pv_Tb_perm <= pv_Tabc_ob ){
      pv_Tabc+=1;
      }
      // Or,as above, assign correct permutation to Tc p-value
      else{
        for(unsigned k=0; k<B;++k){
          pv_Tc_perm+= compare(l,g,t,Tc_perm[k],Tc_perm[b]);
        }
        if(pv_Tc_perm<=pv_Tabc_ob){
          pv_Tabc+=1;
        }
      }
    }
  }
  // Finally, divide result by B
  return pv_Tabc/B;
}