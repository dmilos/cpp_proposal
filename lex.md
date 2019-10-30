LEX

2019/09/10
Dejan D.M. Milosavljevic
(dmilos at gmail dot com)

# Table of Contents.

   - I. Introduction
   - II. Motivation
   - III. Solutions
   - IV. Summary of options
   - IV. Design Decisions
   - V. Impact On the Standard
   - VI. Links

# I. Introduction
  1. Form
    This document is designed in the form to so that reader can easily express their view by choosiness given options.
    Example: {options:[lex_simple-name:lex|flex|Lex|lex_basic]}} give option that name might be lex or flex, etc..
    So the viewer at the end can express opinion like:  [match_type:string], [eat-one-name:process]
  2. About
    Lex it self offer better handling and manipulation of regular expressions when stream parsing is needed.
    In here it will be presented one possible implementation.

# II. Motivation
  There is a trend to add more features to existing regular expression. Like Lookaheads or Inline Modifiers.

  Disadvantages of adding extra feature are:
   - Greater complexity during applying `regex` search/match, implementation might use something that is not finite state machine.
     - Can not relay on execution complexity.
   - Harder do learn all extra features
   - Possible overlap of old and extra features
   - Harder to add new feature without risk of that new feature may overlap with existing
   - Harder maintain code that use extra features
   - Harder to add new classes/function on the top of `regexp` e.g. `lex`
   - Risk of not complying with Chomsky hierarchy

# III. Solutions
## III.1. Description
  Add well know Lex. \
  See: [Lex](https://en.wikipedia.org/wiki/Lex_%28software%29)
  Due to deliberate dependency from `std::regex` compilation will occur in run-time.

## III.2. Classes
   The classes.

### III.2.1 Simple lex
   Imperative is that final code must be fast as possible avoiding everything what slowing down execution paring process.\
   Send character one by one to `lex`.\
   When lex discover unique token function `lex:token` will return number different than `lex::size()`.\
   Only output is newly discovered token.

   In here it is utilized:
  - avoid call of `vector::operator[]`.
  - avoid call of lambda.
  - direct pass of token to other parties.
  - avoid character counting.
  - avoid assembling value of token in to `std::string`
    - Avoid call of system allocation function

#### III.2.1.A definition

```c++
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

          bool eat( char_type const& c ); //!< {options:[eat-one-name:eat|parse]}
          size_type token();
       };
```

#### III.2.1.B Conditions

  - `size_type push( regex_type const& )`
    - description: Add regular expression in internal list
    - complexity: same as `vector<regex_type>::push_back`
    - pre-con: `consumed()` return non-zero;
      - effect: return number same as size()
    - pre-con: `consumed()` return zero;
      - effect: `size()` will increase for one
      - effect: return value is equal to `size() -1`.
      - effect: if some eaten sequence match this regular expression `token` will return number returned by `push`

 - `size_type size()const`
   - description: return number of pushed regular expression
   - complexity: same as vector<regex_type>::size
   - pre-con: Irrelevant.
   - effect: No change

 - `void clear();`
   - description: remove all regex from internal lists
   - complexity: implementation defined.
   - pre-con: Irrelevant.
     - effect: `consumed()` will return -1
     - effect: `size(`) will return 0
     - effect: `token()` will return 0
     - effect: `eat()` will return `false`
     - effect: `compile() will return `false`

 - `bool compile();`
      - Description: prepare internal data so the parsing can begin
       - complexity: implementation defined
      - pre-con: Irrelevant.
        - effect: `consumed()` will return 0.

 - `int consumed()const;`
      - Description: return number of successfully processed characters.
      - complexity: constant.
      - pre-con: Irrelevant.
        - effect: return negative  - can not parse,
        - effect: return  zero     - ready to parse
        - effect: return  positive - number of parsed characters.

 - `bool restart();`
      - Description: Clear internal state and prepare for new parsing.
      - Complexity: constant
      - pre-con: `consumed()` is negative ;
       - effect: return `false`;
      - pre-con: `consumed()` is zero or positive
        - effect: `consumed()` return true;

    `bool eat( char_type const& c );`
      - Description: Process given character by changing internal state.
      - Complexity: constant.
      -  pre-con: `consumed()` is zero or positive .
       - effect: return `false`. Supplied character rejected. Internal state is unchanged. Can proceed with new character.
      -  pre-con: `consumed()` is negative.
       - effect: return `false`.

 - `bool flush();`
      - Description: Force internal state that is no more input and set internal state so `token()` may return value different than `size()`
      - Complexity: constant.
      -  pre-con: `consumed()` is zero or positive.
        - effect: return `true`;
        - effect: `consumed()` is zero.
        - effect: `token()` may return value less than `size()`.
      - pre-con: `consumed()` is negative.
        -effect: return `false`;

 - `size_type token();`
      - Description: return index of uniquely matched regex
      - complexity: constant
      -pre-con: `consumed()` is zero or positive
        - effect: return value equal to `size()`
          - note: waiting for more character.
        - effect: return value is less than `size()`
          - note: have unique sequence that match only one regular expression.
          - note: waiting for more character.
      - pre-con: `consumed()` is negative.
        - effect:  return `size()`

#### III.2.1.C Examples

##### III.2.1.C.- Make instance

Just make one instance of `lex` for `char` type.
```c++
    typedef  std::lex_simple<char> clex_t;
    clex_t l;

    //!< Important in manner that `lex` should be able to easily cooperate with classes that is able to handle Type-2 or Type-3 grammars.
    yacc_t y;
```

##### III.2.1.C.- Fill with information
Exect stream that contains lines of comma separated numbers.

``` c++
    l.push( clex_t::regex_type("[0-9]+") );
    l.push( clex_t::regex_type("\n")     );
    l.push( clex_t::regex_type(",")      );
    l.compile();
```

##### III.2.1.C.- Parsing
   Parse and pass
```c++
    while( false == ifs.eof() )
    {
        if( false == l.eat( ifs.get() ) )
            break;

        if( true == ifs.eof() )
            l.flush();

        lex_t::size_type t = l.token();
        if( l.size() == l.token() )
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
```

### III.2.2 Lambda
   Lambda in the best. Offers rapid code development and flexibility of maintenance.

#### 2III.2.A Definition

```c++
    template < class charT, class traits = regex_traits<charT>, class Alloc = allocator<charT> >
      class lex_lambda //!< {options:[lex_lambda-name:lex_lambda|...]}
       {
        public:
          typedef charT char_type;
          typedef lex_basic<charT, traits > lex_basic_type;
          typedef std::regex<character_type, traits> regex_type;
          typedef std::basic_string< char_type, traits, Alloc > match_type; //{options:[match_type:string|match_results|vector|tuple<size_t,size_t,string>...]}

          typedef std::function< void( match_type const& ) > action_type;

          void push( regex_type const&, action_type const& );
          void clear();

          template < typename iteratorT >
           iteratorT parse( iteratorT const& begin iteratorT const& end );

          bool compile();
          bool good()const{ return 0 != m_lex.consumed(); }

        protected:
          lex_basic_type m_lex; //!< exposition only
       };
```

#### III.2.2.B Conditions

 - `void push( regex_type const&, action_type const&)`
      - description: associate lambda to given `regex`. Just push in to internal list.
      - pre-con: same as `vector<regex_type>::push_back`
        - effect: `good()` will return `false`.
      - complexity: constant.

 - `iteratorT parse( iteratorT const& begin, iteratorT const& end )`
      - description: parse input sequence given by two iterators and call appropriate lambda.
      - complexity: linear.

 - `void clear();`
      - description: remove all regex ( and lambdas ) from internal lists. Allow instance to be reused.
      - complexity: implementation defined
      - pre-con: Irrelevant.
        - effect: same as `lex_basic_type::clear()`

 - `bool compile();`
     - note: same as `lex_basic_type::compile`

 - `good()`
     - note: same as `lex_basic_type::good`

#### III.2.2.C Examples

##### III.2.2.C.- Make instance

Just make one instance of `lex_lambda` for `char` type.
```c++
    typedef  std::lex_lambda<char> cllex_t;
    cllex_t l;

    yacc_t y;
```

##### III.2.2.C.- Fill with information
Expect stream that contains lines of comma separated numbers.

``` c++
    l.push( std::regex("[0-9]+"), [&y](cllex_t::match_type const& m)->void{ y.accept(0); } );
    l.push( std::regex("\n")    , [&y](cllex_t::match_type const& m)->void{ y.accept(1); } );
    l.push( std::regex(",")     , [&y](cllex_t::match_type const& m)->void{ y.accept(2); } );
    l.compile();
```

##### III.2.2.C.- Parsing
   Do everything automatically.

```c++
    std::ifstream ifs0( "some-file.txt" );
    l.parse( std::istream_iterator( ifs0 ), std::istream_iterator() );

    std::ifstream ifs1( "other-file.txt" );
    l.parse( std::istream_iterator( ifs1 ), std::istream_iterator() );
```

#### III.2.3 Range adapter

#### III.2.3.A Description
   `token_iterator` ofers ability to iterate by using range-based for loop.
   Main benefit is easy use of `lex_simple` and fast execution.
   Implementation might be by using function or class.

#### III.2.3.B (Possible) Definition
   In here possible definition is given as `class`. Implementation can make this as function also if more feasible.

```c++
    template< typename iteratorT, typename charT, typename traits = regex_traits<charT>  >
     class token_iterator
      {
       public:
       typedef __implementation-defined__ iterator_type;
       token_iterator( iteratorT const& begin, iteratorT const& end, lex_simple<charT, traits > &l );

       iterator_type begin();
       iterator_type end();
      };
```

#### III.2.1.D Example
 Example given in III.2.1.A.-.- can be rewritten in form like

```c++
    typedef  std::lex_simple<char> clex_t;
    clex_t l;
    l.push( clex_t::regex_type("[0-9]+") );
    l.push( clex_t::regex_type("\n")     );
    l.push( clex_t::regex_type(",")      );
    l.compile();

    yacc_t y;
    std::vector<char> v;

    for( auto token : token_iterator( v.begin(), v.end(), l ) )
     {
      switch( token )
       {
        case( 0 ): std::cout << "number";    y.accept(0); break;
        case( 1 ): std::cout << "new-line";  y.accept(1); break;
        case( 2 ): std::cout << "separator"; y.accept(2); break;
       }
     }
```

# IV. Summary of options
  * {options:[lex_simple-name:lex|flex|Lex|lex_basic]}.\
    Just name of class.
  * {options:[eat-one-name:eat|parse]}\
    This is minor issues. I use `eat` just to make difference from `parse`.
   * {options:[match_type:string|match_results|vector|tuple<size_t,size_t,string>|...]}\
    Not so minor issue. For diagnostic purpose sometimes we really need begin/end of parsed token.
   * {options:[lex_lambda-name:lex_lambda|...]}\
    Another minor issue.

# IV. Design Decisions
  Main goal is to make number of member functions of classes as small as possible.\
  Classes are designed in manner that is possible to easy connect with `yacc`-like features.

  - Run time vs Compile time?\
    Original lex use compile time compilation. This one relays on `std::regex` which is run time. Direct consequence is that this extension must be run time.

  - No iteration or deleting of pushed regular expressions?\
      This will make this library more complicate. Focus is on parsing not on container maintenance. If desired this functionality can be easily added.
     `lex_basic::push` already return number, `lex_basic::erase( size_t )` this is like opposite of iterator idea.
     Tendency is to erase regex associate to token and this will be forced explicitly through `size_t`.

  - `size_t` vs. `enum` for token type.\
    Adding `enum` as custom token numbering will increase parameter list of `lex_lambda`.
    Number of temple parameter is too large in `lex_lambda`. If added to lex_basic this will have domino effect to `lex_lambda`.

    Here is obvious benefit of token being` enum`:
    ```c++
      enum CSVToken{ FIELD, COMMA, NEWL };
      lex_basic< char, CSVToken > l1; //!< Traits and allocator are default

      yacc<CVSToken> y1;

      enum SpaceSVToken{ FIELD, SPACE, NEWL };
      lex_basic<char,SpaceSVToken > l2;

      yacc<SpaceSVToken> y2;

      y1.accept( l2.token() ); //!< Error!!
    ```

  - Where is implementation of this?\
     It would be very helpful to have implementation of this proposal. To test and see behavior.\
     Unfortunately any implementation that satisfies above conditions must relay of specific properties of each compiler vendor.\
     Those specific properties can change from version to version and are very different from vendor to vendor.\
     Any possible implementation by 3rd party will disobey complexity requirements.
     In sub-folder is implementation that disobey complexity requirements.

# V. Impact On the Standard
  * Core\
    None. This is library level.

  * Library\
    None. This is new feature.

  * Existing code\
    None. New class will be under `std` namespace.

# VI. Links
   1. [Chomsky hierarchy](https://en.wikipedia.org/wiki/Chomsky_hierarchy )
   2. [Lex](https://en.wikipedia.org/wiki/Lex_%28software%29)
   3. [Regular Grammar](https://en.wikipedia.org/wiki/Regular_grammar)
