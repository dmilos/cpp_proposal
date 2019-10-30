#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include "./token_iterator.h"
#include "./yacc.h"

int main( int argc, char *argv[] )
 {
  typedef  std_experimental::lex_simple<char> clex_t;
  typedef  yacc yacc_t;


  clex_t l;
  l.push( clex_t::regex_type("[0-9]+") );
  l.push( clex_t::regex_type("\n")     );
  l.push( clex_t::regex_type(",")      );
  l.compile();

  yacc_t y;
  std::vector<char> v;

  for( auto token : std_experimental::token_iterator( v.begin(), v.end(), l ) )
   {
    switch( token )
     {
      case( 0 ): std::cout << "number";    y.accept(0); break;
      case( 1 ): std::cout << "new-line";  y.accept(1); break;
      case( 2 ): std::cout << "separator"; y.accept(2); break;
     }
   }

  return EXIT_SUCCESS;
 }


