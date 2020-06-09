#include "netlist_reader.hpp"
#include <algorithm>
#include <cmath>

bool netlist_sort(vector<string> &a, vector<string> &b);
bool component(char a, char b);

int main()
{
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

  cout<<"Input"<<endl;
  for(int i=0; i<input.size(); i++)
  {
    vector<string> line = input[i];
    for(int j=0; j<line.size(); j++)
    {
      cout<<line[j]<<" ";
    }
    cout<<endl;
  }
  for(int i=0; i<tran.size(); i++)
  {
    cout<<tran[i]<<" ";
  }
  cout<<endl;
  //Remove duplicate lines, keeping smallest node first (except ref node)
  input.erase(remove_if(input.begin(), input.end(), [](vector<string> a){return (a[1]>a[2] && a[2]!="0");}), input.end());

  cout<<"Output"<<endl;
  for(int i=0; i<input.size(); i++)
  {
    vector<string> line = input[i];
    for(int j=0; j<line.size(); j++)
    {
      cout<<line[j]<<" ";
    }
    cout<<endl;
  }
  for(int i=0; i<tran.size(); i++)
  {
    cout<<tran[i]<<" ";
  }
  cout<<endl;
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
