LEX

2019/09/10
Dejan D.M. Milosavljevic
(dmilos at gmail dot com)

# Table of Contents.
  * I. Introduction
  * II. Motivation
  * III. Solutions
    * 1 Description
    * 2 Class
    * 2 Examples
  * IV. Design Decisions
  * V. Impact On the Standard
  * VI. References

# I. Introduction
  Lex it self offer better handling and manipulation of regular expressions when stream parsing is needed. In here will be presented one possible implementation.

# II. Motivation
  There is a trend to add more features to existing regular expression. Like Lookaheads or Inline Modifiers.
  Disatvantages of adding extra feature are: 
   - Greater complexity during aplaying regex search/match, implementaion might use something that is no finit state machine.
   - Harder do learn all extra features
   - Posiible overlap of old and extra features 
   - Harder to add new feature without risk of that new feature may overlap with existing
   - Harder maintaine code that use extra features
   - Harder to add new classes/function on the top of regexp e.g. 'lex'
  
# III. Solutions
  ## 1. Description
    Ad well know feature lex. See: [Lex](https://en.wikipedia.org/wiki/Lex_%28software%29)
    Implementation will have only one class ( [name:lex|flex|Lex] ).  
  
  ## 2. Class
```c++
    template < class charT,
               class traits = char_traits<charT>,    // basic_string::traits_type
               class Alloc = allocator<charT>        // basic_string::allocator_type
             > 
      class lex
       {
        typedef charT char_type;
        typedef std::size_t size_type;

        typedef std::regex<character_type> regex_type;
        typedef std::basic_string< charT, traits, Alloc > match_type; //{options[match_type:string|match_results]}
        
        typedef std::function< void( match_type const& )  > action_type;{condition[lambda-have:(true)]}
        
        void push( regex_type const& ); //!< {condition[lambda-have:(false)]}
        void push( regex_type const&, action_type const&); //!< {condition[lambda-have:(true)]}
        
        // true -character is processed, in case of {condition[lambda-have:(true)]} appropriate lambda will be called.
        // false - charcter rejected. In case of {condition[lambda-have:(true)]} no lambda called.
        bool eat( char_type const& c );
        
         template < typename iteratorT > // {condtition[lambda-have:(true)]}
         iterator_name eat( iteratorT const& begin iteratorT const& end ); //! processing sequence, return last unprocessed or equal to end.
        void flush(); //No more input. Finish parsing. Also clear internal state, also call restart(). In case of {condition[lambda-have:(true)]} call appropriate lambda

        //! eqial to size() - waiting for more character.
        //! not equal to size() - have complete token, last eaten character is not part of parsed token
        size_type token(); //!< {condition[lambda-have:(false)]}

        bool good()const; //!< No push/remove performed after compile
        bool restart(); //!< Slear internal parse state. Object is ready for new parsing.
       };
```

## 3. Examples

### Make instance:
```c++
    typedef  std::lex<char> clex_t;
    clex_t l;    
```

### Fill with information
``` c++
    l.push( std::regex("[0-9]+"), []( lex_t::match_type const& mt )->void{ std::cout << "number"; });
    l.push( std::regex("\n"),     []( lex_t::match_type const& mt )->void{ std::cout << "new-line"; });
    l.push( std::regex(","),      []( lex_t::match_type const& mt )->void{ std::cout << "separator"; });
    l.compile();
 ```
### Parsing: Simpliest variant
```c++
    //Simpliest variant. {condition:[lambda-have:(true)]}
    std::ifstream ifs("test2.txt")
    l.eat( std::istream_iterator( ifs ), std::istream_iterator() );
    // No need for reset or flush. everything is automatic inside eat( begin, end )
```
### Parsing: Manual feed
```c++
    //Feed lex one by one character. {condition:[lambda-have:(true)]}
    std::ifstream ifs("test1.txt")
    while( ifs.eof() )
    {
        l.eat( ifs.get() );
    }
    l.flush(); // No more input. Call appropriate lambda if unique exists. Clear internal state.
```
### Parsing: full controll
```c++
    // [lambda-have:(false)] Most complicated variant, Probably the fastest comparing to [lambda-have:(true)]
    do
    {
        if( true == ifs.eof() )
            l.flush();
        else
            if( false == l.eat( ifs.get() ) )
                break;

        lex_t::size_type  t = l.token();
        if( l.size() == l.token() )
            switch( l.token() )
            {
                case( 0 ): std::cout << "number";    break;
                case( 1 ): std::cout << "new-line";  break;
                case( 2 ): std::cout << "separator"; break;
            }
    }while( false == ifs.eof() )
```

# IV. Summary of options
 name.
  TODO.
 lambda-have.
  TODO.
 
# IV. Design Decisions
  * TODO.
  
# V. Impact On the Standard 
  * Core
    
    None. This is library level.
  * Library
   
   None. This is new feature.
  * Existing code
    
    None. New class will be under std namespace.
  
# VI. Links
   1. [Chomsky hierarchy](https://en.wikipedia.org/wiki/Chomsky_hierarchy )
   2. [Lex](https://en.wikipedia.org/wiki/Lex_%28software%29)
   3. [Regular Grammar](https://en.wikipedia.org/wiki/Regular_grammar)
