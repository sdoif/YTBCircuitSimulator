1st Year Project -- LT Spice Circuit Simulator

Troubleshooting:
Currently testing netlist_reader, whose aim is to read a netlist file format (example in netlist_ex1.txt), ignore any commented lines starting with the * character, and returning an empty vector onces '.end' is read in from the netlist document (aditionally info can be found in Netlist_Spec.txt). The aim is to store the information from each line in a vector of strings and removing unnecessary characters.

 Currently experiencing an error when running the compiled program created by the netlist_reader_test.sh script:
 terminate called after throwing an instance of 'std::out_of_range'
  what():  basic_string::erase: __pos (which is 78) > this->size() (which is 4)
netlist_reader_test.sh: line 8: 26000 Aborted                 (core dumped)
However, logically I can't find where i'm calling the erase on an element outside the string's size.
