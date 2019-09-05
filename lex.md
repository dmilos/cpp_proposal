LEX

2019/09/10
Dejan D.M. Milosavljevic
(dmilos at gmail dot com)

# Table of Contents.
  * I. Introduction
  *  1  Form
  *  2  Introduction
  * II. Motivation
  * III. Solutions
    * 1 Description
    * 2 Class
    * 2.1 Definition
    * 2.1 Conditions
    * 3 Examples
  * IV. Design Decisions
  * V. Impact On the Standard
  * VI. References

# I. Introduction
  1. Form
    This document is designed in the form to so that reader can easily express their view by crossing given options.
    Example: {option[lambda-have:true|false]} give option to have ( option: true ) lambdas not have ( option: false).
    So the viewer at the end can express opinion like: [lambda-have:true], [class-name:lexical_parser], [ match_type:string], [eat-one-name:process]
  2. About
    Lex it self offer better handling and manipulation of regular expressions when stream parsing is needed.
  In here will be presented one possible implementation.

# II. Motivation
  There is a trend to add more features to existing regular expression. Like Lookaheads or Inline Modifiers.
  Disadvantages of adding extra feature are:
   - Greater complexity during applying `regex` search/match, implementation might use something that is not finite state machine.
   - Harder do learn all extra features
   - Possible overlap of old and extra features
   - Harder to add new feature without risk of that new feature may overlap with existing
   - Harder maintain code that use extra features
   - Harder to add new classes/function on the top of `regexp` e.g. `lex`

# III. Solutions
  ## 1. Description
    Add well know Lex. See: [Lex](https://en.wikipedia.org/wiki/Lex_%28software%29)

  ## 2. Class
   The classes.

  ### 2.1 Definition

  #### 2.1.A Simple
  ##### 2.1.A.a definition

```c++
    template < class charT >
      class lex_basic  //!< [class-name:lex|flex|Lex|lex_basic]
       {
        typedef charT char_type;
        typedef std::size_t size_type;

        typedef std::regex<character_type> regex_type;

        size_type push( regex_type const& );
        size_type size()const;

        void clear();

        bool compile();
        int consumed()const;
        bool restart();

        bool eat( char_type const& c ); //!< {options:[eat-one-name:eat|parse]}
        bool flush();
        size_type token();
       };
```
  ##### 2.1.A.b Conditions

    `size_t push( regex_type const& );`
       - description: Add regular expression in internal list
       - complexity: same as `vector<regex_type>::push_back`
      - pre-con: consumed() return non-zero;
        - effect: return number same as size()
      - pre-con: consumed() return zero;
        - effect: size() will increase for one
        - effect: return value is equal to `size() -1`.
        - effect: if some eaten sequence match this regular expression `token` will return number returned by `push`

    `size_type size()const;`
       - description: return number of pushed regular expression
      - complexity: same as vector<regex_type>::size
       - pre-con: Does not matter.
       - post-con: No change

    `void clear();`
       - description: remove all regex from internal lists
       - complexity: implementation defined
       - pre-con: Does not matter.
       - effect: consumed() will return -1
       - effect: size() will return 0
       - effect: token() will return 0
       - effect: eat() will return `false`

    `bool compile();`
      - Description: prepare internal data so the parsing can begin
       - complexity: implementation defined

      - pre-con:
        -- effect:
      - post-con:
        - effect: `consumed()` will return 0.

    `int consumed()const;`
     - Description: return number of successfully processed characters.
      - complexity: constant
     - pre-con: nothing
     - post-con: no effect
     - negative  - can not parse,
     -  zero     - ready to parse
     -  positive - number of parsed characters.

    `bool restart();`
     - Description: Clear internal state and prepare for new parsing.
      - complexity: constant
     - pre-con: `consumed()` is negative ;
      - effect: return false;
     - pre-con: `consumed()` is zero or positive . return true.
       - effect: `consumed()` return 0;

    `bool eat( char_type const& c );` {options:[eat-one-name:eat|parse]}
      - Description: Process given character by changing internal state.
      - complexity: constant
      -  return true - character is properly processed,
      -  pre-con: `consumed()` is zero or positive .
       - effect: false -  return character rejected. Internal state is unchanged. Can proceed with new character.
      -  pre-con: `consumed()` is negative.
       - effect: return false.

    `bool flush();`
      - Description: Force internal state that is no more input and set internal state so token may return value different than `size()`
      - complexity: constant
      -  pre-con: `consumed()` is zero or positive.
      -- post-con: return true;
      -- post-con: `consumed()` is zero
      -- post-con: `token()` may return value less than `size()`.
      - pre-con:
      -- return false

    `size_type token();` TODO, size() !=0 etc
      - Description: return discovered token
      - complexity: constant
      - pre-con:- equal to size()
      - note: waiting for more character.
      - effect: none
      - pre-con: not equal to size()
      - effect:  have complete token, last eaten character is not part of parsed token
      - effect: none

  ##### 2.1.A.b Examples

  ###### Make instance:

Just make one instance of `lex` for `char` type.
```c++
    typedef  std::lex_basic<char> clex_t;
    clex_t l;

    //!< Important in manner that `lex` should be able to easily cooperate with classes that is able to handle Type-2 or Type-3 grammars.
    yacc_t y;
```

###### Fill with information
Parse stream that contains lines of comma separated numbers.

``` c++
    l.push( std::regex("[0-9]+") );
    l.push( std::regex("\n")     );
    l.push( std::regex(",")      );
    l.compile();
```

###### Parsing
   In this version `lex` does not have ability to call lambda.it is up to user to take some action when token is parsed.
   Send character one by one to `lex`. When lex have token function `lex:token` will return number different than `lex::size()`.
```c++
    do
    {
        if( true == ifs.eof() )
            l.flush();
        else
            if( false == l.eat( ifs.get() ) )
                break;

        lex_t::size_type t = l.token();
        if( l.size() == l.token() )
        {
            // Shorter variant but no diagnostics: `y.accept( l.token() );`
            switch( l.token() )
            {
                case( 0 ): std::cout << "number";    y.accept(0); break;
                case( 1 ): std::cout << "new-line";  y.accept(1); break;
                case( 2 ): std::cout << "separator"; y.accept(2); break;
            }
       }
    }while( false == ifs.eof() )
```


  #### 2.1.B Lambda

  #### 2.1.B.a definition
   Utilize ability of lambda
```c++
    template < class charT >
      class lex_lambda
       {
        typedef charT char_type;
        typedef std::size_t size_type;

        typedef std::regex<character_type> regex_type;
        typedef std::basic_string< charT, traits, Alloc > match_type; //{options:[match_type:string|match_results|...]}

        typedef std::function< void( match_type const& )  > action_type;

        void push( regex_type const& );
        void push( regex_type const&, action_type const&);

        bool eat( char_type const& c );

        template < typename iteratorT >
         iterator_name eat( iteratorT const& begin iteratorT const& end );

        void flush();

        size_type token();

        bool good()const;
        bool restart();

        protected:
            lex_t m_lex; //!< exposition only
       };

```

  #### 2.1.B.b Conditions
   TODO

  #### 2.1.B.c Examples
  ###### Make instance:

Just make one instance of `lex` for `char` type.
```c++
    typedef  std::lex_lambda<char> clex_t;
    clex_t l;

    yacc_t y;
```

###### Fill with information
Parse stream that contains lines of comma separated numbers.

``` c++
    l.push( std::regex("[0-9]+"), [&y](clex_t::match_type const& m)->void{ y.eat(0); } );
    l.push( std::regex("\n")    , [&y](clex_t::match_type const& m)->void{ y.eat(1); } );
    l.push( std::regex(",")     , [&y](clex_t::match_type const& m)->void{ y.eat(2); } );
    l.compile();
```
###### Parsing
   Do everything automatically
      ```c++
        std::ifstream ifs( "some-file.txt" );

         l.eat( std::istream_iterator( ifs ), std::istream_iterator() );

          // No need for reset or flush. Everything is automatic inside eat( begin, end );

      ```


# IV. Summary of options
  * class-name.
    Name of class it self. My proposal is to use `lex`
  * eat-one-name
    This is minor issues. I use `eat` just to make difference from `parse`.
   * parse-name
    Another minor issue.

# IV. Design Decisions
  Main goal is to make number of member functions of `lex` as small as possible. Specially number of classes or functions.\
  Everything is reduced to one class.\
  Usually, from my experience, that some extras appear as appendix and it is rarely used.
  `lex` class is designed in manner that is possible to easy connect with `yacc`-like features.

  - No iteration or deleting of pushed regular expressions?
    TODO

# V. Impact On the Standard
  * Core

    None. This is library level.

  * Library

   None. This is new feature.

  * Existing code

    None. New class will be under `std` namespace.

# VI. Links
   1. [Chomsky hierarchy](https://en.wikipedia.org/wiki/Chomsky_hierarchy )
   2. [Lex](https://en.wikipedia.org/wiki/Lex_%28software%29)
   3. [Regular Grammar](https://en.wikipedia.org/wiki/Regular_grammar)
