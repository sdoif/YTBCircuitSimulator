#include <iostream>
#include <string>
#include <cmath>

using namespace std;

double ctod(string v)
{
  double value;
  if(v.rfind('p')!=string::npos){
    value = stod(v.substr(0,v.rfind('p'))) * pow(10,-12);
    return value;
  }
  if(v.find('n')!=string::npos){
    value = stod(v.substr(0,v.rfind('n'))) * pow(10,-9);
    return value;
  }
  if(v.find('u')!=string::npos){
    value = stod(v.substr(0,v.rfind('u'))) * pow(10,-6);
    return value;
  }
  if(v.find('m')!=string::npos){
    value = stod(v.substr(0,v.rfind('m'))) * pow(10,-3);
    return value;
  }
  if(v.find('k')!=string::npos){
    value = stod(v.substr(0,v.rfind('k'))) * pow(10,3);
    return value;
  }
  if(v.find("Meg")!=string::npos){
    value = stod(v.substr(0,v.rfind("Meg"))) * pow(10,6);
    return value;
  }
  if(v.find('G')!=string::npos){
    value = stod(v.substr(0,v.rfind('G'))) * pow(10,9);
    return value;
  }
  value = stod(v);
  return value;
}

int main()
{
  string x;
  cin>>x;
  cout<<ctod(x)<<endl;

}
