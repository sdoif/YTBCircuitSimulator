#include "netlist_reader.hpp"
#include <Eigen/Dense>

using namespace Eigen;

int main()
{
  vector<vector<string>> input;
  NetlistReader reader(cin);
  while(1)
  {
    vector<string> line = reader.get_line();
    if(line.empty()){
      break;
    }
    input.push_back(line);
  }
}
