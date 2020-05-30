#include "netlist_reader.hpp"
#include <Eigen/Dense>

using namespace Eigen;

int main()
{
  //Netlist management
  vector<vector<string>> input;
  vector<string> tran;
  string node_max;
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
      if(line[1]>node_max){
        node_max = line[1];
      }
      if(line[2]>node_max){
        node_max = line[2];
      }
      input.push_back(line);
    }
  }
  if(tran.empty()){
    cerr<<"No simulation settings specified"<<endl;
    return 1;
  }

  //Creating appropriate matrices / vectors
  int nodes;
  if(node_max.find('N')!=string::npos){
    node_max.erase('N');
  }
  nodes = stoi(node_max)+1;
  Matrix<double, Dynamic, Dynamic, 0, 16, 16> con_s;
  if(nodes<17){
    con_s.resize(nodes, nodes);
  }
  MatrixXd con_l;
  if(nodes>16){
    con_l.resize(nodes,nodes);
  }
  VectorXd v(nodes);
  VectorXd i(nodes);

  //TO-DO: Update the values of the conductance matrix / current / voltage vectors for each component

}
