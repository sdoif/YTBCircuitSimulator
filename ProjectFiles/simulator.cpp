#include "netlist_reader.hpp"
#include <Eigen/Dense>
#include <algorithm>

using namespace Eigen;

bool netlist_sort(vector<string> &a, vector<string> &b);
bool component(char a, char b);
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
      if(line[2].find('s')!=string::npos){
        line[2].erase(line[2].find('s'),1);
      }
      if(line[4].find('s')!=string::npos){
        line[4].erase(line[4].find('s'),1);
      }
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
      if(line[1]!="0"){
        input.push_back(line);
      }
      swap(line[1], line[2]);
      if(line[0].rfind('V',0)==0 || line[0].rfind('I',0)==0){
        if(line.size()==4){
          if(line[3].rfind('-',0)==0){
            line[3].erase(0,1);
          }else{
            line[3]="-"+line[3];
          }
        }
        if(line.size()==7){
          if(line[5].rfind('-',0)==0){
            line[5].erase(0,1);
          }else{
            line[5]="-"+line[5];
          }
          line.push_back("1");
        }
      }
      if(line[1]!="0"){
        input.push_back(line);
      }
    }
  }
  if(tran.size()!=5){
    cerr<<"Simulation settings incorrectly specified"<<endl;
    return 1;
  }

  sort(input.begin(), input.end(), netlist_sort);

  //Creating appropriate matrices / vectors
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
      double r_con = 1/(stoi(line[3]));
      //Adding to total conductances indicies
        if(stoi(line[1])!=0){
          con_s(stoi(line[1])-1, stoi(line[1])-1) += r_con;
        }
        if(stoi(line[2])!=0){
          con_s(stoi(line[2])-1, stoi(line[2])-1) += r_con;
        }
      //Allocate respective index in matrix
      con_s(stoi(line[1])-1, stoi(line[2])-1) = r_con;
      con_s(stoi(line[2])-1, stoi(line[1])-1) = -r_con;

    }
    if(line[0].find('C')==0){
      //Check if connected to reference node
      if(stoi(line[2]) == 0){
        //Inserting 1 into respective node
          con_s(stoi(line[1])-1, stoi(line[1])-1) = 1;

      }
      //All other cases when it is connected to 2 non-reference nodes
      else{
        //Check for second time voltage source appears
        if(stoi(line[1]) > stoi(line[2])){
          //Copying values from first row into second row and overwrite first row
          for(int x=0; x<con_s.cols(); x++){
            con_s(stoi(line[1])-1, x) = con_s(stoi(line[2])-1, x);
            con_s(stoi(line[2])-1, x) = 0;

          }
          //Move current vector value from first row into second row as well
          i_s(stoi(line[1])-1) = i_s(stoi(line[2])-1);
          //Making of supernode means 0 conductance between nodes
          con_s(stoi(line[1])-1, stoi(line[2])-1) = 0;
          //Add in 1 and -1 to first row to represent voltage source
          con_s(stoi(line[2])-1, stoi(line[2])-1) = 1;
          con_s(stoi(line[2])-1, stoi(line[1])-1) = -1;

        }

      }


    }
    if(line[0].find('L')==0){

    }
    if(line[0].find('V')==0){
      //Check if connected to reference node
      if(stoi(line[2]) == 0){
        //Inserting 1 into respective node
          con_s(stoi(line[1])-1, stoi(line[1])-1) = 1;
          //Insert value of source into voltage vector
          i_s(stoi(line[1])-1) = ctod(line[3]);

      }
      //All other cases when it is connected to 2 non-reference nodes
      else{
        //Check for second time voltage source appears
        if(stoi(line[1]) > stoi(line[2])){
          //Copying values from first row into second row and overwrite first row
          for(int x=0; x<con_s.cols(); x++){
            con_s(stoi(line[1])-1, x) = con_s(stoi(line[2])-1, x);
            con_s(stoi(line[2])-1, x) = 0;

          }
          //Move current vector value from first row into second row as well
          i_s(stoi(line[1])-1) = i_s(stoi(line[2])-1);
          //Making of supernode means 0 conductance between nodes
          con_s(stoi(line[1])-1, stoi(line[2])-1) = 0;
          //Add in 1 and -1 to first row to represent voltage source
          con_s(stoi(line[2])-1, stoi(line[2])-1) = 1;
          con_s(stoi(line[2])-1, stoi(line[1])-1) = -1;
          //Place value of source into voltage vector, but negative as in first row nodes are flipped
          //Check for if DC source
          if(line.size() < 4){
            i_s(stoi(line[2])-1) = ctod(line[3]);
          }
          //Sine source needs to be inverted in different way
          else{
            i_s(stoi(line[2])-1) = ctod(line[4])+(ctod(line[5])*sin(2*M_PI*t*ctod(line[6])));


          }


        }

      }

    }
    if(line[0].find('I')==0){

    }
  }

  //Transient analysis
  double stopTime = ctod(tran[2]);
  double timeStep = ctod(tran[4]);
  for(double t=0; t<=stopTime; t+=timeStep){
    if(node_max<17){
      v_s = con_s.ColPivHouseholderQR().solve(i_s);
    }
    if(node_max>16){
      v_l = con_l.PartialPivLU().solve(i_l);
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

double ctod(string v)
{
  double value;
  if(v.rfind('p')!=string::npos){
    value = stod(v.substr(0,v.rfind('p'))) * pow(10,-12);
    return value;
  }
  if(v.find('n')!=string::npos){
    value = stod(v.substr(0,v.rfind('n'))) * pow(10,-9);
    return value;
  }
  if(v.find('u')!=string::npos){
    value = stod(v.substr(0,v.rfind('u'))) * pow(10,-6);
    return value;
  }
  if(v.find('m')!=string::npos){
    value = stod(v.substr(0,v.rfind('m'))) * pow(10,-3);
    return value;
  }
  if(v.find('k')!=string::npos){
    value = stod(v.substr(0,v.rfind('k'))) * pow(10,3);
    return value;
  }
  if(v.find("Meg")!=string::npos){
    value = stod(v.substr(0,v.rfind("Meg"))) * pow(10,6);
    return value;
  }
  if(v.find('G')!=string::npos){
    value = stod(v.substr(0,v.rfind('G'))) * pow(10,9);
    return value;
  }
  value = stod(v);
  return value;
}
