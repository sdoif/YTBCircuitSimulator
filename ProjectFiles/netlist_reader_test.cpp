#include "netlist_reader.hpp"

int main()
{
  vector<vector<string>> input;
  vector<string> tran;
  int node_max;
  NetlistReader reader(cin);
  cout<<1<<endl;
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

cout<<"you made it"<<endl;
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
