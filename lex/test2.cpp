#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <regex>

#include "./lex_lambda.h"
#include "./yacc.h"

int main( int argc, char *argv[] )
 {
  typedef  std_experimental::lex_lambda<char> cllex_t;
  typedef  yacc yacc_t;

  cllex_t l;

  yacc_t y;
  l.push( std::regex("[0-9]+"), [&y](cllex_t::match_type const& m)->void{ y.accept(0); } );
  l.push( std::regex("\n")    , [&y](cllex_t::match_type const& m)->void{ y.accept(1); } );
  l.push( std::regex(",")     , [&y](cllex_t::match_type const& m)->void{ y.accept(2); } );
  l.compile();

  std::ifstream ifs0( "some-file.txt" );
  l.parse( std::istream_iterator<char>( ifs0 ), std::istream_iterator<char>() );

  std::ifstream ifs1( "other-file.txt" );
  l.parse( std::istream_iterator<char>( ifs1 ), std::istream_iterator<char>() );


  return EXIT_SUCCESS;
 }

