#include <iostream>
#include <string>
#include <cmath>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main()
{
  int node_max;
  cin>>node_max;
  Matrix<double, Dynamic, Dynamic, 0, 16, 16> con_s;
  if(node_max<17){
    con_s = MatrixXd::Zero(node_max, node_max);
    cout<<con_s<<endl;
    cout<<con_s.size()<<endl;
  }
  Matrix<double, Dynamic, Dynamic, 0, 16, 1> i_s;
  if(node_max<17){
    i_s = VectorXd::Zero(node_max);
    cout<<i_s<<endl;
    cout<<i_s.size()<<endl;
  }
  Matrix<double, Dynamic, Dynamic, 0, 16, 1> v_s;
  if(node_max<17){
    v_s = VectorXd::Zero(node_max);
    cout<<v_s<<endl;
    cout<<v_s.size()<<endl;
  }
  MatrixXd con_l;
  if(node_max>16){
    con_l = MatrixXd::Zero(node_max,node_max);
    cout<<con_l<<endl;
    cout<<con_l.size()<<endl;
  }
  VectorXd v_l;
  if(node_max>16){
    v_l = VectorXd::Zero(node_max);
    cout<<v_l<<endl;
    cout<<v_l.size()<<endl;
  }
  VectorXd i_l;
  if(node_max>16){
    i_l = VectorXd::Zero(node_max);
    cout<<i_l<<endl;
    cout<<i_l.size()<<endl;
  }

}
