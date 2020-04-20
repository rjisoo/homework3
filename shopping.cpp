#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int shopping(int[], int[], int, int, vector<int>&);
void outputString(string, ofstream&);

int main() {
  /* inputs
    testCase:     number of test cases
    items:        number of items
    family:       number of family members
    maximum:      maximum weight which can be carried
    price[100]:   prices of items
    weight[100]:  weights of items
    vec(100):     vector to track the record
  */
  int testCase, items, family, maximum = 0, maxPrice = 0;
  int price[100], weight[100];
  vector<vector<int> > vec(100);
  ifstream inputFile;
  ofstream outFile;

  // read numbers from the input file
  inputFile.open("shopping.txt");
  inputFile >> testCase;
  outFile.open("results.txt");

  // read the number of items, price and weight, family
  for (int i = 0; i < testCase; i++) {
    inputFile >> items;
    for (int j = 0; j < items; j++) {
      inputFile >> price[j];
      inputFile >> weight[j];
    }
    inputFile >> family;

    // keep the record of maxPrice value of items,
    // which can be carried by family,
    // for dynamic programin
    for (int famIndex = 0; famIndex < family; famIndex++) {
      inputFile >> maximum;
      maxPrice = maxPrice + shopping(weight, price, items, maximum, vec[famIndex]);
    }

    // output total price result
    string totalOutput = "Test Case " + to_string(i + 1)
      + "\nTotal Price " + to_string(maxPrice) + "\nMember Items\n";
    outputString(totalOutput, outFile);

    // output individual carriage result, using vec
    for (int l = 0; l < family; l++) {
      sort(vec[l].begin(), vec[l].end());
      string memberOutput = "   " + to_string(l + 1) + ":  ";
      outputString(memberOutput, outFile);

      for (int m = 0; m < vec[l].size(); m++) {
          string costOutput = to_string(vec[l][m]) + " ";
          outputString(costOutput, outFile);
      }
      outputString("\n", outFile);
    }

    outputString("\n", outFile);
  }

  inputFile.close();
  outFile.close();
  return 0;
}

// fine the maximum price that one person can carry
int shopping(int weight[], int price[], int items, int maximum, vector<int> &v) {
  int knapsack[items + 1][maximum + 1];

  // track maximum price results to knapsack[][] array
  for (int i = 0; i <= items; i++) {
    for (int j = 0; j <= maximum; j++) {
      if (i == 0 || j == 0) {
        // base case
        knapsack[i][j] = 0;
      } else if (weight[i - 1] <= j) {
        // if the weight new calculated weight is lighter than 
        // current j value, find a new max value
        knapsack[i][j] = max(price[i - 1] + knapsack[i - 1][j - weight[i - 1]], knapsack[i - 1][j]);
      } else {
        // otherwise, bring the same value from the value in the previous row
        knapsack[i][j] = knapsack[i - 1][j];
      }
    }
  }

  int res = knapsack[items][maximum];
  int maxVal = maximum;
  for (int k = items; k > 0 && res > 0; k--) {
    if (res == knapsack[k - 1][maxVal]) {
      // if the final value is same the current tracking value
      // do nothing
      continue;
    } else {
      // otherwise, push the value to vec, where tracking
      // value that individual member carries
      // then adjust the value
      v.push_back(k);
      res = res - price[k - 1];
      maxVal = maxVal - weight[k - 1];
    }
  }

  // the fuction returns the maximum price that can be
  // carried by one member
  return knapsack[items][maximum];
}

// handle output to console and output file
void outputString(string outputString, ofstream& oFile) {
  cout << outputString;
  oFile << outputString;
}