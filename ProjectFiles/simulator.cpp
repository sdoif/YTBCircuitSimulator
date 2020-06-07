#include "netlist_reader.hpp"
#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;

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
      double r_con = 1/(stoi(line[3]));
      //Adding to total conductances indicies
        if(stoi(line[1])!=0){
          con_s(stoi(line[1])-1, stoi(line[1])-1) += r_con;
        }
        if(stoi(line[2])!=0){
          con_s(stoi(line[2])-1, stoi(line[2])-1) += r_con;
        }
      //Allocate respective index in matrix
      con_s(stoi(line[1]), stoi(line[2])) = r_con;
      con_s(stoi(line[2]), stoi(line[1])) = -r_con;

    }
    if(line[0].find('C')==0){
      //Check if connected to reference node
      if(stoi(line[2])) == 0){
        //Inserting 1 into respective node
          con_s(stoi(line[1]), stoi(line[1])) = 1;

      }
      //All other cases when it is connected to 2 non-reference nodes
      else{
        //Check for second time voltage source appears
        if(stoi(line[1]) > stoi(line[2])){
          //Copying values from first row into second row and overwrite first row
          for(int x=1; x<con_s.cols(); x++){
            con_s(stoi(line[1]), x) = con_s(stoi(line[2]), x);
            con_s(stoi(line[2]), x) = 0;

          }
          //Move current vector value from first row into second row as well
          i(stoi(line[1])) = i(stoi(line[2]));
          //Making of supernode means 0 conductance between nodes
          con_s(stoi(line[1]), stoi(line[2])) = 0;
          //Add in 1 and -1 to first row to represent voltage source
          con_s(stoi(line[2]), stoi(line[2])) = 1;
          con_s(stoi(line[2]), stoi(line[1])) = -1;

        }

      }


    }
    if(line[0].find('L')==0){

    }
    if(line[0].find('V')==0){
      //Check if connected to reference node
      if(stoi(line[2])) == 0){
        //Inserting 1 into respective node
          con_s(stoi(line[1]), stoi(line[1])) = 1;
          //Insert value of source into voltage vector
          i(stoi(line[1])) = ctof(line[3]);

      }
      //All other cases when it is connected to 2 non-reference nodes
      else{
        //Check for second time voltage source appears
        if(stoi(line[1]) > stoi(line[2])){
          //Copying values from first row into second row and overwrite first row
          for(int x=1; x<con_s.cols(); x++){
            con_s(stoi(line[1]), x) = con_s(stoi(line[2]), x);
            con_s(stoi(line[2]), x) = 0;

          }
          //Move current vector value from first row into second row as well
          i(stoi(line[1])) = i(stoi(line[2]));
          //Making of supernode means 0 conductance between nodes
          con_s(stoi(line[1]), stoi(line[2])) = 0;
          //Add in 1 and -1 to first row to represent voltage source
          con_s(stoi(line[2]), stoi(line[2])) = 1;
          con_s(stoi(line[2]), stoi(line[1])) = -1;
          //Place value of sourve into voltage vector, but negative as in first row nodes are flipped
          i(stoi(line[2])) = ctof(line[3])*-1;

        }

      }

    }
    if(line[0].find('I')==0){

    }
  }
}
