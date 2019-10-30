#ifndef _2895ad6daed649248d128abdc057778f
#define _2895ad6daed649248d128abdc057778f

#include <regex>

namespace std_experimental
 {

  template < class charT, class traits = std::regex_traits<charT> >
   class lex_simple  //!< {options:[lex_simple-name:lex|flex|Lex|lex_basic]}
    {
     public:
       typedef charT char_type;
       typedef std::size_t size_type;
       typedef int int_type;

       typedef std::basic_regex< charT, traits > regex_type;

       lex_simple();

       size_type push( regex_type const& );
       size_type size()const;

       void clear();

       bool compile();
       int_type  consumed()const;
       bool restart();

       bool eat( char_type const& c );
       bool flush();
       size_type token()const;
     private:
       typedef std::vector<char> buffer_type;
       int_type m_consumed;
       buffer_type m_buffer;
       struct data_struct
        {
         regex_type m_rx;
         bool m_pass;
        };
       std::vector< data_struct >  m_list;
       size_type m_token;
    };

  template < class charT, class traits >
   lex_simple<charT,traits>::lex_simple()
    {
     this->clear();
    }

  template < class charT, class traits >
   typename lex_simple<charT,traits>::size_type 
   lex_simple<charT,traits>::push( regex_type const& rx )
    {
     if( 0 != this->m_consumed )
      {
       return this->m_list.size();
      }

     this->m_list.emplace_back( data_struct{} );
     this->m_list.back().m_rx = rx;
     this->m_list.back().m_pass = false;
     this->m_token = m_list.size();
     return this->m_list.size()-1;
    }

  template < class charT, class traits >
   typename lex_simple<charT,traits>::size_type 
   lex_simple<charT,traits>::size()const
    {
     return m_list.size();
    }

  template < class charT, class traits >
   void lex_simple<charT,traits>::clear()
    {
     this->m_list.clear();
     this->m_consumed = 0;
     this->m_token = 0;
     this->m_buffer.clear();
     return ;
    }

  template < class charT, class traits >
   bool lex_simple<charT,traits>::compile()
    {
     this->m_consumed = 0;
     for( auto& item: m_list )
      {
       item.m_pass= false;
      }
     return true;
    }

  template < class charT, class traits >
   int  lex_simple<charT,traits>::consumed()const
    {
     return this->m_consumed;
    }

  template < class charT, class traits >
   bool lex_simple<charT,traits>::restart()
    {
     if( this->m_consumed < 0 )
      {
       return false;
      }
     this->m_consumed = 0;
     for( auto& item: m_list )
      {
       item.m_pass= false;
      }
     this->m_buffer.clear();

     return true;
    }

  template < class charT, class traits >
   bool lex_simple<charT,traits>::eat( char_type const& c )
    {
     if( this->m_consumed < 0 )
      {
       return false;
      }

     ++this->m_consumed;
     m_buffer.push_back( c );
     size_type count = 0;;
     size_type index = 0;
     size_type candidate = 0;

     this->m_token = m_list.size();
     for( auto& item: m_list )
      {
       if( true == std::regex_match( m_buffer.begin(), m_buffer.end(), item.m_rx ) )
        {
         ++count;
         candidate = index;
        }
       ++index;
      }

    if( 0 == count )
     {
      this->m_token = m_list.size();
      return false;
     }

    this->m_token = candidate;

     return true;
    }

  template < class charT, class traits >
   bool 
   lex_simple<charT,traits>::flush()
    {
     if( this->consumed() < 0 )
      {
       return false;
      }

     this->m_token = this->m_list.size();
     if( 0 == this->consumed() )
      { // Trivial case
       for( size_type index = 0; index < this->m_list.size(); ++index  )
        {
         if( true == std::regex_match( this->m_buffer.begin(),this->m_buffer.end(),  this->m_list[ index ].m_rx ) )
          {
           this->m_token = index;
           return true;
          }
        }
      }

     for( size_type index = 0; index < this->m_list.size(); ++index  )
      {
       if( true == this->m_list[ index ].m_pass )
        {
         this->m_token = index;
         return true;
        }
      }
     return true;
    }

  template < class charT, class traits >
   typename lex_simple<charT,traits>::size_type 
   lex_simple<charT,traits>::token()const
    {
     return this->m_token;
    }
 }

#endif