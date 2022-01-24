#include "httplib.hpp"
#include <iostream>

using namespace std;

int main(void) {

  httplib::Client cli("localhost", 8080);

  if ( auto res = cli.Get("/attacker/4") ) {
    cout << res->status << endl;
    cout << res->get_header_value("Content-Type") << endl;
    cout << res->body << endl;
  } else {
    cout << "error code: " << res.error() << std::endl;
  }

  if ( auto res = cli.Get("/blockers/1,2,3") ) {
    cout << res->status << endl;
    cout << res->get_header_value("Content-Type") << endl;
    cout << res->body << endl;
  } else {
    cout << "error code: " << res.error() << std::endl;
  }

  return 0;
}
