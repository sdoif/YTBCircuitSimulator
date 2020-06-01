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
  bool small = x<17;
  MatrixXd con_l;
  Matrix<double, Dynamic, Dynamic, 0, 16, 16> (small ? con : con_s);

  con_s.resize(3,3);
  con_s << 1,2,3, 4,5,6, 7,8,9;
  cout<<"success"<<endl;

}
