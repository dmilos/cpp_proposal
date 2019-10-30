#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include "./lex_simple.h"

#include "./yacc.h"

int main( int argc, char argv[] )
 { 
  typedef  std_experimental::lex_simple<char> clex_t;
  typedef  yacc yacc_t;

  clex_t l;

  yacc_t y;

  l.push( clex_t::regex_type("[0-9]+") );
  l.push( clex_t::regex_type("\n")     );
  l.push( clex_t::regex_type(",")      );
  l.compile();

  std::ifstream  ifs("./test1.csv");

  while( false == ifs.eof() )
  {
      if( false == l.eat( ifs.get() ) )
          continue;

      if( true == ifs.eof() )
          l.flush();

      clex_t::size_type t = l.token();
      if( l.size() != l.token() )
      {
          // Shorter/Fastest variant but no diagnostics: `y.accept( l.token() );`
          switch( l.token() )
          {
              case( 0 ): std::cout << "number";    y.accept(0); break;
              case( 1 ): std::cout << "new-line";  y.accept(1); break;
              case( 2 ): std::cout << "separator"; y.accept(2); break;
          }
      }
  }
  return EXIT_SUCCESS;
 }