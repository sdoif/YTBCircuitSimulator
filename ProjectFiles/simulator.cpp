#include "netlist_reader.hpp"
#include <Eigen/Dense>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>

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

  if(node_max<17){
  //Intialisation
  for(int l=0; l<input.size(); l++){
    vector<string> line = input[l];
    //Resistor processing
    if(line[0].find('R')==0){
      double r_con = 1/(ctod(line[3]));
      //Adding to total conductances indicies
      if(stoi(line[1])!=0){
        con_s(stoi(line[1])-1, stoi(line[1])-1) += r_con;
      }
      //Allocate respective index in matrix
      if((stoi(line[1])!=0)&&(stoi(line[2])!=0)){
        con_s(stoi(line[1])-1, stoi(line[2])-1) -= r_con;
      }
    }

    //Capacitor processing
    if(line[0].find('C')==0){
      //Add charge value to map
      charges[line[0]] = 0;
      //Check if connected to reference node
      if(stoi(line[2]) == 0){
        //loop to make all indexes in row=0
        for(int x=0; x<con_s.cols(); x++){
          con_s(stoi(line[1])-1, x) = 0;
        }
        //Inserting 1 into respective node
        con_s(stoi(line[1])-1, stoi(line[1])-1) = 1;
      }
      //All other cases when it is connected to 2 non-reference nodes
      else{
        //Check for second time voltage source appears
        if(stoi(line[1]) > stoi(line[2])){
          //Copying values from first row into second row and overwrite first row
          for(int x=0; x<con_s.cols(); x++){
            con_s(stoi(line[1])-1, x) += con_s(stoi(line[2])-1, x);
            con_s(stoi(line[2])-1, x) = 0;
            }
          }

          //Making of supernode means 0 conductance between nodes
          con_s(stoi(line[1])-1, stoi(line[2])-1) = 0;
          //Add in 1 and -1 to first row to represent voltage source
          con_s(stoi(line[2])-1, stoi(line[2])-1) = 1;
          con_s(stoi(line[2])-1, stoi(line[1])-1) = -1;
          //Move current vector value from first row into second row if current source present
          i_s(stoi(line[1])-1) += i_s(stoi(line[2])-1);
        }
      }


    //Voltage processing
    if(line[0].find('V')==0){
      //Check if connected to reference node
      if(stoi(line[2]) == 0){
        //loop to make all indexes in row=0
        for(int x=0; x<con_s.cols(); x++){
          con_s(stoi(line[1])-1, x) = 0;
        }
        //Inserting 1 into respective node
          con_s(stoi(line[1])-1, stoi(line[1])-1) = 1;
          //Insert value of source into current vector if DC source
          if(line[3]!="SINE"){
            i_s(stoi(line[1])-1) = ctod(line[3]);
          }
          //Initialise current vector for sine source at DC offset
          else{
            i_s(stoi(line[1])-1) = ctod(line[4]);
          }
      }
      //All other cases when it is connected to 2 non-reference nodes
      else{
        //Check for second time voltage source appears
        if(stoi(line[1]) > stoi(line[2])){
          //Copying values from first row into second row and overwrite first row
          for(int x=0; x<con_s.cols(); x++){
            con_s(stoi(line[1])-1, x) += con_s(stoi(line[2])-1, x);
            con_s(stoi(line[2])-1, x) = 0;
          }
          //Move current vector value from first row into second row if current source present
          i_s(stoi(line[1])-1) += i_s(stoi(line[2])-1);
          //Add in 1 and -1 to first row to represent voltage source
          con_s(stoi(line[2])-1, stoi(line[2])-1) = 1;
          con_s(stoi(line[2])-1, stoi(line[1])-1) = -1;
          //Place value of source into current vector
          //Check for if DC source
          if(line[3]!="SINE"){
            i_s(stoi(line[2])-1) = ctod(line[3])*(-1);
          }
          //Initialise current vector for sine source at DC offset
            else{
              i_s(stoi(line[2])-1) = ((-1)*ctod(line[4]));
            }
          }
        }
    }

    //Current source processing
    if(line[0].find('I')==0){
      int node = stoi(line[1]);
      //cout<<node<<endl;
      //If a current source is found, the value of its current will be added to respective node
        if(node!=0){
        //  cout<<"here if"<<endl;
          //If sinusoidal then at t=0, only DC offset value
          if(line[3]=="SINE"){
            i_s((node-1), 0) += ctod(line[4]);
          }else{
            i_s((node-1), 0) += ctod(line[3]);
        //    cout<<"here else"<<endl;
          }
      }
    }
  }
  }

  if(node_max>16){
    //Intialisation
    for(int l=0; l<input.size(); l++){
      vector<string> line = input[l];
      //Resistor processing
      if(line[0].find('R')==0){
        double r_con = 1/(ctod(line[3]));
        //Adding to total conductances indicies
        if(stoi(line[1])!=0){
          con_l(stoi(line[1])-1, stoi(line[1])-1) += r_con;
        }
        //Allocate respective index in matrix
        if((stoi(line[1])!=0)&&(stoi(line[2])!=0)){
          con_l(stoi(line[1])-1, stoi(line[2])-1) -= r_con;
        }
      }

      //Capacitor processing
      if(line[0].find('C')==0){
        //Add charge value to map
        charges[line[0]] = 0;
        //Check if connected to reference node
        if(stoi(line[2]) == 0){
          //loop to make all indexes in row=0
          for(int x=0; x<con_l.cols(); x++){
            con_l(stoi(line[1])-1, x) = 0;
          }
          //Inserting 1 into respective node
          con_l(stoi(line[1])-1, stoi(line[1])-1) = 1;
        }
        //All other cases when it is connected to 2 non-reference nodes
        else{
          //Check for second time voltage source appears
          if(stoi(line[1]) > stoi(line[2])){
            //Copying values from first row into second row and overwrite first row
            for(int x=0; x<con_l.cols(); x++){
              if(con_l(stoi(line[2])-1)!=0){
                con_l(stoi(line[1])-1, x) = con_l(stoi(line[2])-1, x);
                con_l(stoi(line[2])-1, x) = 0;
                con_l(stoi(line[1])-1, stoi(line[1])-1) += abs(con_l(stoi(line[1])-1, x));
              }
            }

            //Making of supernode means 0 conductance between nodes
            con_l(stoi(line[1])-1, stoi(line[2])-1) = 0;
            //Add in 1 and -1 to first row to represent voltage source
            con_l(stoi(line[2])-1, stoi(line[2])-1) = 1;
            con_l(stoi(line[2])-1, stoi(line[1])-1) = -1;
            //Move current vector value from first row into second row if current source present
            i_l(stoi(line[1])-1) += i_l(stoi(line[2])-1);
          }
        }
      }

      //Voltage processing
      if(line[0].find('V')==0){
        //Check if connected to reference node
        if(stoi(line[2]) == 0){
          //loop to make all indexes in row=0
          for(int x=0; x<con_l.cols(); x++){
            con_l(stoi(line[1])-1, x) = 0;
          }
          //Inserting 1 into respective node
            con_l(stoi(line[1])-1, stoi(line[1])-1) = 1;
            //Insert value of source into current vector if DC source
            if(line[3]!="SINE"){
              i_l(stoi(line[1])-1) = ctod(line[3]);
            }
            //Initialise current vector for sine source at DC offset
            else{
              i_l(stoi(line[1])-1) = ctod(line[4]);
            }
        }
        //All other cases when it is connected to 2 non-reference nodes
        else{
          //Check for second time voltage source appears
          double sc = 0;
          if(stoi(line[1]) > stoi(line[2])){
            //Copying values from first row into second row and overwrite first row
            for(int x=0; x<con_l.cols(); x++){
              if(con_l(stoi(line[2])-1)!=0){
              con_l(stoi(line[1])-1, x) += con_l(stoi(line[2])-1, x);
              con_l(stoi(line[2])-1, x) = 0;
              con_l(stoi(line[1])-1, stoi(line[1])-1) += abs(con_l(stoi(line[1])-1, x));
              }
            }
            //find total conductance connected positive end of supernode
            for(int y=0; y<con_l.cols(); y++){
              if(y < stoi(line[1])-1){
                sc += con_l(stoi(line[1])-1, y);
              }
            }
            //Current vector value equal to value of source multiplied by conductance at positive terminal
            i_l(stoi(line[1])-1) = ctod(line[3])*sc;
            //Move current vector value from first row into second row if current source present
            i_l(stoi(line[1])-1) += i_l(stoi(line[2])-1);
            //Making of supernode means 0 conductance between nodes
            con_l(stoi(line[1])-1, stoi(line[2])-1) = 0;
            //Add in 1 and -1 to first row to represent voltage source
            con_l(stoi(line[2])-1, stoi(line[2])-1) = 1;
            con_l(stoi(line[2])-1, stoi(line[1])-1) = -1;
            //Place value of source into current vector
            //Check for if DC source
            if(line[3]!="SINE"){
              i_l(stoi(line[2])-1) = ctod(line[3])*(-1);
            }
            //Initialise current vector for sine source at DC offset
              else{
                i_l(stoi(line[2])-1) = ((-1)*ctod(line[4]));
              }
            }
          }
      }

      //Current source processing
      if(line[0].find('I')==0){
        int node = stoi(line[1]);
        //cout<<node<<endl;
        //If a current source is found, the value of its current will be added to respective node
          if(node!=0){
          //  cout<<"here if"<<endl;
            //If sinusoidal then at t=0, only DC offset value
            if(line[3]=="SINE"){
              i_l((node-1), 0) += ctod(line[4]);
            }else{
              i_l((node-1), 0) += ctod(line[3]);
          //    cout<<"here else"<<endl;
            }
        }
      }
    }
  }

  //Remove duplicate lines, keeping smallest node first (except ref node)
  input.erase(remove_if(input.begin(), input.end(), [](vector<string> a){return (a[1]>a[2] && a[2]!="0");}), input.end());

  //Transient analysis
  double stopTime = ctod(tran[2]);
  double timeStep = ctod(tran[4]);
  //CSV Output
  cout<<con_s<<endl;
  cout<<i_s<<endl;
  cout<<"Time,";
  for(int l=1; l<=node_max; l++){
    cout<<"N"<<setfill('0')<<setw(3)<<l<<",";
  }
  for(int l=0; l<input.size(); l++){
    cout<<(input[l])[0]<<",";
  }
  cout<<"\n";
  if(node_max<17){
    for(double t=0.0; t<=stopTime; t+=timeStep){
      cout<<t<<",";
      v_s = con_s.partialPivLu().solve(i_s);
      for(int l=0; l<v_s.size(); l++){
        cout<<v_s(l)<<",";
      }
      //Cycle  through elements to find ones that need to be updated every cycle
      for(int y=0; y<input.size(); y++){
        vector<string> line = input[y];
        //Inductor processing
        if(line[0].find('L')==0){
          cout<<induct_i[line[0]]<<",";
          //Initialising variables for node numbers, inductance and PD across inductor
          int l_node1 = stoi(line[1]);
          int l_node2 = stoi(line[2]);
          double induct_val = ctod(line[3]);
          double l_pd;
          //Finding l_pd, the PD across inductor
          if(l_node1 == 0){
            l_pd = -v_s((l_node2-1),0);
        }else if(l_node2 == 0){
            l_pd = (v_s((l_node1-1),0));
        }else{
            l_pd = (v_s((l_node1-1),0))-(v_s((l_node2-1),0));
        }
        cout<<"PD across inductor = "<<l_pd<<endl;
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

        //Current source processing
        if(line[0].find('I')==0){
          if(line[3]=="SINE"){
            int node2 = stoi(line[2]);
            int node1 = stoi(line[1]);
            cout<<(ctod(line[4])+(ctod(line[5])*sin(2*M_PI*ctod(line[6])*t)))<<",";
            //If a current source is found, the value of its current will be added to respective node
            if(t!=0){
              //Initialise variable delta_i which calculates the increase or decrease in current from the previous instance to current one
              //it will then increment
              double delta_i = (ctod(line[5])*sin(2*M_PI*(t)*ctod(line[6])))-(ctod(line[5])*sin(2*M_PI*(t-timeStep)*ctod(line[6])));
              if(node2 != 0){
                i_s((node2 -1),0)+= delta_i;
              }else if(node1 != 0){
              i_s((node1 -1),0)-= delta_i;
              }
            }
          }else{
            cout<<ctod(line[3])<<",";
          }
        }

        //Capacitor processing
        if(line[0].find('C')==0){
          //Check for reference node
          double total = 0;
          if(stoi(line[2])==0){
            //multiply all conductances going into capacitor node by voltage difference of nodes to get total current into node
            for(int y=0; y<con_s.cols(); y++){
              if(y != stoi(line[1])-1){
                double i = (con_s(y, stoi(line[1])-1))*(v_s(stoi(line[1])-1)-v_s(y));
                //sum of currents
                total += i;
              }
            }

            //output current into capacitor
            cout<<total<<",";
            //multiply current by timestep to get additional charge stored on capacitor
            charges[line[0]] += (total*timeStep);
            //Divide total charge by capacitance to update current vector value for voltage provided by capacitor
            i_s(stoi(line[1])-1) = charges[line[0]]/ctod(line[3]);
            }
          else{
            //Find difference in voltage nodes and multiply by capacitance to get charges
            for(int y=0; y<con_s.cols(); y++){
              if(y < stoi(line[1])-1){
                double i = (y, con_s(stoi(line[2])-1))*(v_s(stoi(line[1])-1)-v_s(y));
                //sum of currents
                total += i;
              }
            }
            for(int y=0; y<con_s.cols(); y++){
              if(y > stoi(line[2])-1){
                double i = (y, con_s(stoi(line[2])-1))*(v_s(stoi(line[2])-1)-v_s(y));
                //sum of currents
                total -= i;
              }
            }
            cout<<total<<",";
            charges[line[0]] += (total*timeStep);
            //Divide total charge by capacitance to update current vector value
            i_s(stoi(line[1])-1) = charges[line[0]]/ctod(line[3]);
            //find total conductance connected positive end of supernode

            }
          }

        //Voltage source processing
        if(line[0].find('V')==0){
          cout<<0<<",";
          if(line[3]=="SINE"){
            i_s(stoi(line[1])-1) = ctod(line[4])+(ctod(line[5])*sin(2*M_PI*t*ctod(line[6])));
            }
          }

        //Resistor processing
        if(line[0].find('R')==0){
          if(line[2]=="0"){
            double current = v_s(stoi(line[1])-1)/ctod(line[3]);
            cout<<current<<",";
          }else{
            double current = (v_s(stoi(line[1])-1) - v_s(stoi(line[2])-1))/ctod(line[3]);
            cout<<current<<",";
          }
        }
      }
      cout<<"\n";
    }
  }
  if(node_max>16){
    for(double t=0.0; t<=stopTime; t+=timeStep){
      cout<<t<<",";
      v_l = con_l.partialPivLu().solve(i_l);
      for(int l=0; l<v_l.size(); l++){
        cout<<v_l(l)<<",";
      }
      //Cycle  through elements to find ones that need to be updated every cycle
      for(int y=0; y<input.size(); y++){
        vector<string> line = input[y];
        //Inductor processing
        if(line[0].find('L')==0){
          cout<<induct_i[line[0]]<<",";
          //Initialising variables for node numbers, inductance and PD across inductor
          int l_node1 = stoi(line[1]);
          int l_node2 = stoi(line[2]);
          double induct_val = ctod(line[3]);
          double l_pd;
          //Finding l_pd, the PD across inductor
          if(l_node1 == 0){
            l_pd = v_l((l_node2-1),0);
        }else if(l_node2 == 0){
            l_pd = -(v_l((l_node1-1),0));
        }else{
            l_pd = (v_l((l_node2-1),0))-(v_l((l_node1-1),0));
        }
        cout<<"PD across inductor = "<<l_pd<<endl;
        //Finding di, the change in current and the inductors contribution to that node's current
        double di_l = (l_pd*timeStep)/induct_val;
        if(l_node2!=0){
          i_l((l_node2 - 1), 0) += di_l;
        }else if(l_node1!=0){
          i_l((l_node1 -1), 0) -= di_l;
        }
        //Find add di to current going through inductor
        induct_i[line[0]]+=di_l;
        }

        //Current source processing
        if(line[0].find('I')==0){
          if(line[3]=="SINE"){
            int node2 = stoi(line[2]);
            int node1 = stoi(line[1]);
            cout<<(ctod(line[4])+(ctod(line[5])*sin(2*M_PI*ctod(line[6])*t)))<<",";
            //If a current source is found, the value of its current will be added to respective node
            if(t!=0){
              //Initialise variable delta_i which calculates the increase or decrease in current from the previous instance to current one
              //it will then increment
              double delta_i = (ctod(line[5])*sin(2*M_PI*(t)*ctod(line[6])))-(ctod(line[5])*sin(2*M_PI*(t-timeStep)*ctod(line[6])));
              if(node2 != 0){
                i_l((node2 -1),0)+= delta_i;
              }else if(node1 != 0){
              i_l((node1 -1),0)-= delta_i;
              }
            }
          }else{
            cout<<ctod(line[3])<<",";
          }
        }

        //Capacitor processing
        if(line[0].find('C')==0){
          //Check for reference node
          double total = 0;
          if(stoi(line[2])==0){
            //multiply all conductances going into capacitor node by voltage difference of nodes to get total current into node
            for(int y=0; y<con_l.cols(); y++){
              if(y != stoi(line[1])-1){
                double i = (con_l(y, stoi(line[1])-1))*(v_l(stoi(line[1])-1)-v_l(y));
                //sum of currents
                total += i;
              }
            }

            //output current into capacitor
            cout<<total<<",";
            //multiply current by timestep to get additional charge stored on capacitor
            charges[line[0]] += (total*timeStep);
            //Divide total charge by capacitance to update current vector value for voltage provided by capacitor
            i_l(stoi(line[1])-1) = charges[line[0]]/ctod(line[3]);
            }
          else{
            //Find difference in voltage nodes and multiply by capacitance to get charges
            double sc = 0;
            for(int y=0; y<con_l.cols(); y++){
              if(y < stoi(line[1])-1){
                double i = (y, con_l(stoi(line[2])-1))*(v_l(stoi(line[1])-1)-v_l(y));
                //sum of currents
                total += i;
              }
            }
            for(int y=0; y<con_l.cols(); y++){
              if(y > stoi(line[2])-1){
                double i = (y, con_l(stoi(line[2])-1))*(v_l(stoi(line[2])-1)-v_l(y));
                //sum of currents
                total -= i;
              }
            }
            cout<<total<<",";
            charges[line[0]] += (total*timeStep);
            //Divide total charge by capacitance to update current vector value
            i_l(stoi(line[1])-1) = charges[line[0]]/ctod(line[3]);
            //find total conductance connected positive end of supernode
            for(int y=0; y<con_l.cols(); y++){
              if(y < stoi(line[1])-1){
                sc -= con_l(stoi(line[1])-1, y);
              }
            }
            //Current vector value equal to value of voltage multiplied by conductance at positive terminal
            i_l(stoi(line[2])-1) = i_l(stoi(line[2])-1)*sc;
            }
          }

        //Voltage source processing
        if(line[0].find('V')==0){
          cout<<0<<",";
          if(line[3]=="SINE"){
            i_l(stoi(line[1])-1) = ctod(line[4])+(ctod(line[5])*sin(2*M_PI*t*ctod(line[6])));
            }
          }

        //Resistor processing
        if(line[0].find('R')==0){
          if(line[2]=="0"){
            double current = v_l(stoi(line[1])-1)/ctod(line[3]);
            cout<<current<<",";
          }else{
            double current = (v_l(stoi(line[1])-1) - v_l(stoi(line[2])-1))/ctod(line[3]);
            cout<<current<<",";
          }
        }
      }
      cout<<"\n";
    }
  }
}

bool component(char a, char b)
{
  if(a=='I' && b!='I'){
    return true;
  }
  if(a=='L' && b!='I' && b!='L'){
    return true;
  }
  if(a=='R' && b!='I' && b!='L' && b!='R'){
    return true;
  }
  if(a=='V' && b=='C'){
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
