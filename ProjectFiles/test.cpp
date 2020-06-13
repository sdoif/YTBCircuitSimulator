#include "netlist_reader.hpp"
#include <algorithm>
#include <cmath>

int main()
{
  double x,y,z;
  cin>>x>>y>>z;
  double stopTime = 0.01;
  double timeStep = 1e-6;
  for(double t=0; t<stopTime; t+=timeStep){
    cout<<(x+y*sin(z*t*2*M_PI))<<endl;
  }
}
