#ifndef NetlistReader_hpp
#define NetlistReader_hpp

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

class NetlistReader
{
private:
  istream &src;

public:
  /*Creates a new netlist reader using the given input stream
  To create an instance: NetlistReader <name>(cin);
  */
  NetlistReader(istream &src);

  /*Reads a line of from the associated stream.
  Should skip comment lines '*'
  Once '.end' is reached returns an empty vector and should not be called again
  */
  vector<string> get_line();
};

#endif
