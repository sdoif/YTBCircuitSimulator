#include "netlist_reader.hpp"

NetlistReader::NetlistReader(istream &_src)
    : src(_src)
{

}

vector<string> NetlistReader::next()
{
  vector<string> line;
  string in;
  getline(src, in);
  if(in.empty()){
    cerr<<"No input found"<<endl;
    exit(1);
  }
  //Continues extracting lines until 1st char is not *
  while(in[0]=='*'){
    getline(src, in);
  }
  //Returns empty vector if line is '.end'
  if(in==".end"){
    return line;
  }
  //All valid syntax lines of netlist contain ' ' except '.end'
  if(in.find(' ')==string::npos){
    cerr<<"Invalid syntax"<<endl;
    exit(1);
  }
  //Splits up the input into params and pushes them back into the vector
  while(in.find(' ')!=string::npos){
    int index = in.find(' ');
    string param = in.substr(0, index);
    in = in.substr(index+1, in.size()-(index+1));
    line.push_back(param);
  }
  return line;
}
