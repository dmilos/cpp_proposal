#ifndef _2895ad6daed649248d128abdc057778f
#define _2895ad6daed649248d128abdc057778f

namespace std_experimental
 {

  template < class charT, class traits = regex_traits<charT> >
   class lex_simple  //!< {options:[lex_simple-name:lex|flex|Lex|lex_basic]}
    {
     public:
       typedef charT char_type;
       typedef std::size_t size_type;

       typedef std::regex<charT, traits > regex_type;

       size_type push( regex_type const& );
       size_type size()const;

       void clear();

       bool compile();
       int  consumed()const;
       bool restart();

       bool eat( char_type const& c );
       size_type token();
     private:

    };

  template < class charT, class traits = regex_traits<charT> >
   size_type lex_simple<charT,traits>::push( regex_type const& )
    {
     return ;
    }

  template < class charT, class traits = regex_traits<charT> >
   typename lex_simple<charT,traits>::size_type lex_simple<charT,traits>::size()const
    {
     return ;
    }

  template < class charT, class traits = regex_traits<charT> >
   void lex_simple<charT,traits>::clear()
    {
     return ;
    }

  template < class charT, class traits = regex_traits<charT> >
   bool lex_simple<charT,traits>::compile()
    {
     return ;
    }

  template < class charT, class traits = regex_traits<charT> >
   int  lex_simple<charT,traits>::consumed()const
    {
     return ;
    }
  template < class charT, class traits = regex_traits<charT> >
   bool lex_simple<charT,traits>::restart()
    {
     return true;
    }

  template < class charT, class traits = regex_traits<charT> >
   bool lex_simple<charT,traits>::eat( char_type const& c )
    {
     return false;
    }

  template < class charT, class traits = regex_traits<charT> >
   lex_simple<charT,traits>::size_type lex_simple<charT,traits>::token()
    {
     return 0;
    }

 }

#endif