#include "netlist_reader.hpp"
#include <algorithm>

bool component(char a, char b);
bool netlist_sort(vector<string> &a, vector<string> &b);

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
      swap(line[1], line[2]);
      input.push_back(line);
    }
  }
  sort(input.begin(), input.end(), netlist_sort);

  if(tran.empty()){
    cerr<<"No simulation settings specified"<<endl;
    return 1;
  }

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
