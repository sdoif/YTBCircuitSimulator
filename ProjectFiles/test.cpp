#include <iostream>
#include <string>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main()
{
  string in;
  cin>>in;
  int x = stoi(in);
  cout<<x<<endl;
  MatrixXd con_l;
  cout<<"test: "<<con_l<<endl;
  if(x<17){
    con_l.resize(x,x);
  }
  con_l << 1,2,3,4,5,6,7,8,9;
  cout<<con_l<<endl;
}
