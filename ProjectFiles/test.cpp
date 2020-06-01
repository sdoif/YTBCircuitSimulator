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
  small ? Matrix<double, Dynamic, Dynamic, 0, 16, 16> con; : MatrixXd con;
  Matrix<double, Dynamic, Dynamic, 0, 16, 16> con_s;
  MatrixXd con_l;

  con_s.resize(3,3);
  con_s << 1,2,3, 4,5,6, 7,8,9;
  cout<<"success"<<endl;

}
