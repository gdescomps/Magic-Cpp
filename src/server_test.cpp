#include "httplib.hpp"
#include <iostream>

using namespace std;

int main(void) {

  httplib::Client cli("localhost", 8080);

  // if (auto res = cli.Get("/numbers/12")) {
  //   cout << res->status << endl;
  //   cout << res->get_header_value("Content-Type") << endl;
  //   cout << res->body << endl;
  // } else {
  //   cout << "error code: " << res.error() << std::endl;
  // }

  // if ( auto res = cli.Post("/duel", "attacker=2245&blockers=23,42,1212,28,565,45", "application/x-www-form-urlencoded") ) {
  //   cout << res->status << endl;
  //   cout << res->get_header_value("Content-Type") << endl;
  //   cout << res->body << endl;
  // } else {
  //   cout << "error code: " << res.error() << std::endl;
  // }

  if ( auto res = cli.Get("/blockers/23,42,1212,28,565,45") ) {
    cout << res->status << endl;
    cout << res->get_header_value("Content-Type") << endl;
    cout << res->body << endl;
  } else {
    cout << "error code: " << res.error() << std::endl;
  }


  return 0;
}
