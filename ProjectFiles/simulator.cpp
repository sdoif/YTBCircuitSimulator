#include "netlist_reader.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <cmath>
#include <map>

using namespace Eigen;

bool netlist_sort(vector<string> &a, vector<string> &b);
bool component(char a, char b);
double ctod(string v);

int main()
{
  map<string, double> charges;
  map<string, double> induct_i;
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
          if(line[6].rfind('-',0)==0){
            line[6].erase(0,1);
          }else{
            line[6]="-"+line[6];
          }
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
    con_s = MatrixXd::Zero(node_max, node_max);
  }
  Matrix<double, Dynamic, Dynamic, 0, 16, 1> i_s;
  if(node_max<17){
    i_s = VectorXd::Zero(node_max);
  }
  Matrix<double, Dynamic, Dynamic, 0, 16, 1> v_s;
  if(node_max<17){
    v_s = VectorXd::Zero(node_max);
  }
  MatrixXd con_l;
  if(node_max>16){
    con_l = MatrixXd::Zero(node_max,node_max);
  }
  VectorXd v_l;
  if(node_max>16){
    v_l = VectorXd::Zero(node_max);
  }
  VectorXd i_l;
  if(node_max>16){
    i_l = VectorXd::Zero(node_max);
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
    if((stoi(line[1])!=0)&&(stoi(line[2])!=0)){
      con_s(stoi(line[1])-1, stoi(line[2])-1) -= r_con;
    }
//since we changed the input processing this wont be needed
    //con_s(stoi(line[2])-1, stoi(line[1])-1) = -r_con;

  }
    if(line[0].find('C')==0){
      //Add charge value to map
      charges[line[0]] = 0;
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
//Initialise current mapping to 0
    //induct_i[line[0]]=0;
    }
    if(line[0].find('V')==0){
      //Check if connected to reference node
      if(stoi(line[2]) == 0){
        //Inserting 1 into respective node
          con_s(stoi(line[1])-1, stoi(line[1])-1) = 1;
          //Insert value of source into current vector if DC source
          if(line.size() < 5){
            i_s(stoi(line[1])-1) = ctod(line[3]);
          }
          //Initialise current vector for sine source at DC offset
          else{
            double t = 0;
            i_s(stoi(line[1])-1) = ctod(line[4])+(ctod(line[5])*sin(2*M_PI*t*ctod(line[6])));


          }
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
          //Place value of source into current vector
          //Check for if DC source
          if(line.size() < 5){
            i_s(stoi(line[2])-1) = ctod(line[3]);
          }
          //Initialise current vector for sine source at DC offset
            else{
              double t = 0;
              i_s(stoi(line[2])-1) = ctod(line[4])+(ctod(line[5])*sin(2*M_PI*t*ctod(line[6])));


            }


          }

        }


    }
    if(line[0].find('I')==0){
      int node = stoi(line[2]);
      //If a current source is found, the value of its current will be added to respective node
        if(node!=0){
          //If sinusoidal then at t=0, only DC offset value
          if(line[3]=="SINE"){
            i_s((node-1), 0) += ctod(line[4]);
          }else{
            i_s((node-1), 0) += ctod(line[3]);
          }

      }
    }
  }

  //Remove duplicate lines, keeping smallest node first (except ref node)
  input.erase(remove_if(input.begin(), input.end(), [](vector<string> a){return (a[1]>a[2] && a[2]!="0");}), input.end());

  //Transient analysis
  double stopTime = ctod(tran[2]);
  double timeStep = ctod(tran[4]);
  for(double t=0; t<=stopTime; t+=timeStep){
    if(node_max<17){
      v_s = con_s.ColPivHouseholderQR().solve(i_s);
      //Cycle  through elements to find ones that need to be updated every cycle
      for(int y=0; y=input.size(); y++){
        vector<string> line = input[y];

        if(line[0].find('L')==0){
          //Initialising variables for node numbers, inductance and PD across inductor
          int l_node1 = stoi(line[1]);
          int l_node2 = stoi(line[2]);
          double induct_val = ctod(value[3]);
          double l_pd;
          //Finding l_pd, the PD across inductor
          if(l_node1 == 0){
            l_pd = v_s((l_node2-1),0);
        }else if(l_node2 == 0){
            l_pd = -(v_s((l_node1-1),0));
        }else{
            l_pd = (v_s((l_node2-1),0))-(v_s((l_node1-1),0));
        }
        //Finding di, the change in current and the inductors contribution to that node's current
        double di_l = (l_pd*timeStep)/induct_val;
        if(l_node2!=0){
          i_s((l_node2 - 1), 0) += di_l;
        }else if(l_node1!=0){
          i_s((l_node1 -1), 0) -= di_l;
        }
        //Find add di to current going through inductor
        induct_i[line[0]]+=di_l;

        }

        if(line[0].find('I')==0 && line[3]=="SINE"){
          int node2 = stoi(line[2]);
          int node1 = stoi(line[1]);
          //If a current source is found, the value of its current will be added to respective node
          if(t!=0){
            if(node2 != 0){
              i_s((node2 -1),0)+= (ctod(line[5])*sin((t)*ctod(line[6])))-(ctod(line[5])*sin((t-timeStep)*ctod(line[6])));
            }else if(node1 != 0){
              i_s((node1 -1),0)-= (ctod(line[5])*sin((t)*ctod(line[6])))-(ctod(line[5])*sin((t-timeStep)*ctod(line[6])));
            }
          }
        }

        if(line[0].find('C')==0){
          //Check for reference node
          if(stoi(line[2])==0){
            //Find difference in voltage nodes (in this case value of one node) and multiply by capacitance to get charges
            double cc = 0;
            cc = (v_s(stoi(line[1])-1)*ctod(line[3]);
            charges[line[0]] += cc;
            //Divide total charge by capacitance to update current vector value
            i_s(stoi(line[1])-1) = charges[line[0]]/timeStep;

            }
          else{
            //Find difference in voltage nodes and multiply by capacitance to get charges
            double cc = 0;
            cc = (v_s(stoi(line[1])-1) - v_s(stoi(line[2])-1))*ctod(line[3]);
            charges[line[0]] += cc;
            //Divide total charge by capacitance to update current vector value
            i_s(stoi(line[1])-1) = charges[line[0]]/timeStep;

            }
          }


        if(line[0].find('V')==0 && line[4]=="SINE"){
          i_s(stoi(line[1])-1) = ctod(line[4])+(ctod(line[5])*sin(2*M_PI*t*ctod(line[6])));
          }
        }
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
      return true;
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
