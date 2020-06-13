#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(){
  ofstream output;
  vector<vector<string>> netlist;
  vector<string> tran = {".tran","0","10ms","0","1us" };
  netlist.push_back(tran);
  vector<string> v = {"V1","N001","0","5"};
  netlist.push_back(v);
  vector<string> r = {"R1","0","N001","100"};
  netlist.push_back(r);
  vector<string> c = {"C1","0","N001","1u"};
  netlist.push_back(c);
  for(int i=1; i<101; i++){
    string filename = "test";
    filename += to_string(i);
    filename += ".txt";
    output.open(filename);
    for(int m=0; m<netlist.size(); m++){
      int n = 0;
      for(n; n<netlist[m].size()-1; n++){
        output<<netlist[m][n]<<" ";
      }
      output<<netlist[m][n]<<endl;
    }
    output<<".end"<<endl;
    output.close();

    //Resistor
    string des = "R" + to_string(i);
    r[0] = des;
    string n1 = "N";
    if(to_string(i).size()==1){
      n1 += ("00"+to_string(i));
    }
    if(to_string(i).size()==2){
      n1 += ("0"+to_string(i));
    }
    r[1] = n1;
    string n2 = "N";
    if(to_string(i+1).size()==1){
      n2 += ("00"+to_string(i+1));
    }
    if(to_string(i+1).size()==2){
      n2 += ("0"+to_string(i+1));
    }
    if(to_string(i+1).size()==3){
      n2 += to_string(i+1);
    }
    r[2] = n2;
    netlist[2*i] = r;
    vector<string> r_n = r;
    des = "R" + to_string(i+1);
    r_n[0] = des;
    r_n[1] = "0";
    r_n[2] = n2;
    netlist.push_back(r_n);

    //Capacitor
    des = "C" + to_string(i);
    c[0] = des;
    n1 = "N";
    if(to_string(i).size()==1){
      n1 += ("00"+to_string(i));
    }
    if(to_string(i).size()==2){
      n1 += ("0"+to_string(i));
    }
    c[1] = n1;
    n2 = "N";
    if(to_string(i+1).size()==1){
      n2 += ("00"+to_string(i+1));
    }
    if(to_string(i+1).size()==2){
      n2 += ("0"+to_string(i+1));
    }
    if(to_string(i+1).size()==3){
      n2 += to_string(i+1);
    }
    c[2] = n2;
    netlist[2*i+1] = c;
    vector<string> c_n = c;
    des = "C" + to_string(i+1);
    c_n[0] = des;
    c_n[1] = "0";
    c_n[2] = n2;
    netlist.push_back(c_n);
  }

}
