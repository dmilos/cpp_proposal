#ifndef _40cadb88fd5348c08977a9492adfd203
#define _40cadb88fd5348c08977a9492adfd203

#include "./lex_simple.h"

namespace std_experimental
 {

  template< typename iteratorT, typename charT, typename traits = regex_traits<charT>  >
   class token_iterator
    {
     public:
     typedef int iterator_type;
     typedef lex_simple<charT, traits > lex_type;
     token_iterator( iteratorT const& begin, iteratorT const& end, lex_type &l ){}

     iterator_type begin(){ return 0; }
     iterator_type end(){ return 10;}
    };

 }

  template< typename iteratorT, typename charT, typename traits >
   typename std_experimental::token_iterator< iteratorT, charT, traits >::iterator_type
    begin( std_experimental::token_iterator< iteratorT, charT, traits > & ti )
     {
      return ti.begin();
     }

   template< typename iteratorT, typename charT, typename traits >
   typename std_experimental::token_iterator< iteratorT, charT, traits >::iterator_type
    end( std_experimental::token_iterator< iteratorT, charT, traits > & ti )
     {
      return ti.end();
     }
#endif