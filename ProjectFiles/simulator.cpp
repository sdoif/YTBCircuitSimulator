#include "netlist_reader.hpp"
#include <Eigen/Dense>

using namespace Eigen;

double ctod(string v);

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
Matrix<double, Dynamic, Dynamic, 0, 16, 1> i_s;
if(node_max<17){
 i_s.resize(node_max, 1);
}
Matrix<double, Dynamic, Dynamic, 0, 16, 1> v_s;
if(node_max<17){
 v_s.resize(node_max, 1);
}
MatrixXd con_l;
if(node_max>16){
 con_l.resize(node_max,node_max);
}
VectorXd v_l;
if(node_max>16){
 v_l.resize(node_max,1);
}
VectorXd i_l;
if(node_max>16){
 i_l.resize(node_max,1);
}

  /*TO-DO: Update the values of the conductance matrix / current / voltage vectors for each component
  Potential to move to another hpp file netlist_process? */
  for(int l=0; l<input.size(); l++){
    vector<string> line = input[l];
//The 0th index of the line vector contains the designator and hence looking at the 0th char of the string
//will tell us what component it is
if(line[0].find('R')==0){
    double r_con = 1/(ctod(line[3]));
    //Adding to total conductances indicies
      if(stoi(line[1])!=0){
        con_s(stoi(line[1])-1, stoi(line[1])-1) += r_con;
      }
//since we changed the input processing this wont be needed
    /* if(stoi(line[2])!=0){
        con_s(stoi(line[2])-1, stoi(line[2])-1) += r_con;
      }*/
    //Allocate respective index in matrix
    con_s(stoi(line[1])-1, stoi(line[2])-1) -= r_con;
//since we changed the input processing this wont be needed
    //con_s(stoi(line[2])-1, stoi(line[1])-1) = -r_con;

  }
    if(line[0].find('C')==0){

    }
    if(line[0].find('L')==0){

    }
    if(line[0].find('V')==0){

    }
    if(line[0].find('I')==0){
      int node = stoi(line[1]);
      //If a current source is found, the value of its current will be added to respective node
      if(node!=0){
        i_s((node-1), 0) += ctod(line[3]);
      }
    }
  }
}
