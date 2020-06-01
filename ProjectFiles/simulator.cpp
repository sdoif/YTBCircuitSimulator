#include "netlist_reader.hpp"
#include "matrix_processor.hpp"

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
      if(line[1].find('N')!=string::npos){
        line[1].erase('N');
      }
      node=stoi(line[1]);
      if(node>node_max){
        node_max = node;
      }
      if(line[2].find('N')!=string::npos){
        line[2].erase('N');
      }
      node=stoi(line[2]);
      if(node>node_max){
        node_max = node;
      }
      input.push_back(line);
    }
  }
  if(tran.empty()){
    cerr<<"No simulation settings specified"<<endl;
    return 1;
  }

  //Creating appropriate matrices / vectors
  int nodes = node_max+1;
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

  /*TO-DO: Update the values of the conductance matrix / current / voltage vectors for each component
  Potential to move to another hpp file netlist_process? */
  for(int l=0; l<input.size(); l++){
    vector<string> line = input[l];
    if(line[0].find('R')==0){

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
