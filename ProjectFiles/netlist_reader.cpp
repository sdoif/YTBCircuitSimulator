#include "netlist_reader.hpp"

NetlistReader::NetlistReader(istream &_src)
    : src(_src)
{

}

vector<string> NetlistReader::next()
{
  vector<string> line;
  string in;
  while(!scr.eof()){
    getline(src, in);
    //Continues extracting lines until 1st char is not *
    while(in[0]=='*'){
      getline(src, in);
    }
    
  }
}
