#include "netlist_reader.hpp"
#include <Eigen/Dense>
#include <algorithm>

using namespace Eigen;

bool netlist_sort(vector<string> &a, vector<string> &b);
bool component(char a, char b);

int main()
{
  //Netlist management
  vector<vector<string>> input;
  vector<string> tran;
  int node_max;
  NetlistReader reader(cin);
  while(1)
  {
    vector<string> line = reader.get_line();
    if(line.empty()){
      break;
    }
    if(line[0]==".tran"){
      tran = line;
    }
    else{
      int node;
      if(line[1].rfind('N',0)==0){
        line[1].erase(0,1);
      }
      node=stoi(line[1]);
      if(node>node_max){
        node_max = node;
      }
      if(line[2].rfind('N',0)==0){
        line[2].erase(0,1);
      }
      node=stoi(line[2]);
      if(node>node_max){
        node_max = node;
      }
      input.push_back(line);
      swap(line[1],line[2]);
      input.push_back(line);
    }
  }
  if(tran.empty()){
    cerr<<"No simulation settings specified"<<endl;
    return 1;
  }

  sort(input.begin(), input.end(), netlist_sort);

  //Creating appropriate matrices / vectors
//  int nodes = node_max+1;
  Matrix<double, Dynamic, Dynamic, 0, 16, 16> con_s;
  if(node_max<17){
    con_s.resize(node_max, node_max);
  }
  MatrixXd con_l;
  if(node_max>16){
    con_l.resize(node_max,node_max);
  }
  VectorXd v(node_max);
  VectorXd i(node_max);

  /*TO-DO: Update the values of the conductance matrix / current / voltage vectors for each component
  Potential to move to another hpp file netlist_process? */
  for(int l=0; l<input.size(); l++){
    vector<string> line = input[l];
//The 0th index of the line vector contains the designator and hence looking at the 0th char of the string
//will tell us what component it is
    if(line[0].find('R')==0){
      double r_con = 1/(line[3]);
      //Adding to total conductances indicies
        if(line[1]!==0){
          con_s(line[1]-1, line[1]-1) += r_con;
        }
        if(line[2]!==0){
          con_s(line[2]-1, line[2]-1) += r_con;
        }
      //Allocate respectie index in matrix
      con_s(line[1], line[2]) = r_cons;
      con_s(line[2], line[1]) = -r_cons;

    }
    if(line[0].find('C')==0){

    }
    if(line[0].find('L')==0){

    }
    if(line[0].find('V')==0){

    }
    if(line[0].find('I')==0){

    }
  }
}

bool component(char a, char b)
{
  if(a=='V' && b!='V'){
    return true;
  }
  if(a=='C' && b!='V' && b!='C'){
    return true;
  }
  if(a=='I' && b!='V' && b!='C' && b!='I'){
    return true;
  }
  if(a=='L' && b=='R'){
    return true;
  }
  return false;
}

bool netlist_sort(vector<string> &a, vector<string> &b)
{
  if(a[1]<b[1]){
    return true;
  }
  if(a[1]==b[1]){
    if(component(a[0][0], b[0][0])){
      return true
    }
    if(a[0][0] == b[0][0]){
      return a[2]<b[2];
    }
  }
  return false;
}
