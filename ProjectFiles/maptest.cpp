#include<iostream>
#include<map>

using namespace std;

int main(){
  map<string, double> x;
  x["hello"] = 5;
  cout<<x["hello"].first<<"="<<x["hello"].second<<endl;
}
