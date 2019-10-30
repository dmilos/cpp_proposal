#ifndef _40cadb88fd5348c08977a9492adfd203
#define _40cadb88fd5348c08977a9492adfd203

#include "./lex_simple.h"

namespace std_experimental
 {

  template< typename iteratorT, typename charT, typename traits = regex_traits<charT>  >
   class token_iterator
    {
     public:
     typedef __implementation-defined__ iterator_type;
     typedef lex_simple<charT, traits > lex_type;
     token_iterator( iteratorT const& begin, iteratorT const& end, lex_type &l );

     iterator_type begin();
     iterator_type end();
    };

   // TODO std::begin()
   // TODO std::end()

 }

#endif