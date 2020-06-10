#include "netlist_reader.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>

int main()
{
  int x, y;
  cin>>x>>y;
  vector<vector<string>> input;
  vector<string> l = {"yes", "hi"};
  for(int i=0; i<y; i++){
    input.push_back(l);
  }
  for(int l=1; l<=x; l++){
    cout<<"N"<<setfill('0')<<setw(3)<<l<<",";
  }
  for(int l=0; l<y; l++){
    cout<<(input[l])[0]<<",";
  }
  cout<<"\n";
}
