#ifndef _32c812042ec84d78b130fb04eca1ba49
#define _32c812042ec84d78b130fb04eca1ba49

#include <functional>
#include "./lex_simple.h"

namespace std_experimental
 {

  template < class charT, class traits = std::regex_traits<charT>, class Alloc = std::allocator<charT> >
   class lex_lambda //!< {options:[lex_lambda-name:lex_lambda|...]}
    {
     public:
       typedef charT char_type;
       
       typedef std_experimental::lex_simple< char_type, traits > lex_basic_type;
       
       typedef std::basic_regex<char_type, traits> regex_type;
       typedef std::basic_string< char_type, traits, Alloc > match_type; //{options:[match_type:string|match_results|vector|tuple<size_t,size_t,string>...]}
       
       typedef std::function< void( match_type const& ) > action_type;
       
       void push( regex_type const&, action_type const& );
       void clear();
       
       template < typename iteratorT >
        iteratorT parse( iteratorT const& begin, iteratorT const& end ){ return iteratorT{}; }
       
       bool compile();
       bool good()const{ return 0 != m_lex.consumed(); }
       
     protected:
       lex_basic_type m_lex; //!< exposition only
    };


    template < class charT, class traits, class Alloc  >
        void lex_lambda<charT, traits, Alloc>:: push( regex_type const&, action_type const& )
        {

        }

    template < class charT, class traits, class Alloc  >
        void lex_lambda<charT, traits, Alloc>::clear()
        {

        }
    
    //template < class charT, class traits, class Alloc  >
    // template < typename iteratorT >
    //    iteratorT lex_lambda<charT, traits, Alloc>::parse( iteratorT const& begin iteratorT const& end )
    //    {
    //
    //    }

    template < class charT, class traits, class Alloc  >
        bool lex_lambda<charT, traits, Alloc>::compile()
        {
         return true;
        }

 }

#endif
