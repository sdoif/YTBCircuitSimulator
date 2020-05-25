#ifndef NetlistReader_hpp
#define NetlistReader_hpp

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class NetlistReader
{
private:
  istream &src;

public:
  //Creattes a new netlist reader using the given input stream
  NetlistReader(istream &src);

  /*Reads a line of from the associated stream.
  Once '.end' is reached returns an empty vector and should not be called again
  */
  vector<string> next();
};

#endif