#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int main( int argc, char **argv ) {
  // TODO: implement

  ifstream f("README.txt"); // open file

  if (!f.is_open()) { // check for errors opening the file
    cerr << "ERROR OPENING TRACE FILE";
    return 1;
  }

  string line; // hold 1 line of the file at a time
  while (getline(f, line)) {

    stringstream line_stream(line);
    string field;

    // first field is load/store
    getline(line_stream, field, ' '); 
    string op_string = field;
    
    // second field is memory address
    getline(line_stream, field, ' ');
    string address_string = field;

    // ignore third field.

    // do operations
  }

  f.close();
  return 0;
}
