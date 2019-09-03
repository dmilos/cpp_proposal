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
    Implementation will have only one class ( [class-name:lex|flex|Lex] ).

  ## 2. Class
   The class it self.

  ### 2.1 Definition
   Class definitions.

```c++
    template < class charT >
      class lex
       {
        typedef charT char_type;
        typedef std::size_t size_type;

        typedef std::regex<character_type> regex_type;

        void push( regex_type const& );
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

  ### 2.2. Conditions
    `size_t push( regex_type const& );`
      - pre-con: consumed() should return 0;
       return number same as size()
      - post-con: Description: Add regular expression in internal list
      - post-con: size() will increase for one
      - post-con: return value is equal to `size() -1`.
      - post-con: if some eaten sequence match this regular expression `token` will return number returned by `push`

    `size_type size()const;`
       - Description: return number of pushed regular expression
       - pre-con: Does not matter.
       - post-con: No change

    `void clear();`
       - Description: remove all regex from internal lists
       - pre-con: Does not matter.
       - post-con: consumed() will return -1
       - post-con: size() will return 0
       - post-con: token() will return 0
       - post-con: eat() will return `false`

    `bool compile();`
     - Description: prepare internal data so the parsing can begin
     - pre-con:
     - `consumed()` will return 0.

    `int consumed()const;`
     - Description: return number of successfully processed characters.
     - pre-con: nothing
     - post-con: no effect
     - -1 - can not parse,
     -  0 - ready to parse
     -  + number - number of parsed characters.

    `bool restart();`
     - Description: Clear internal state and prepare for new parsing.
     - pre-con: `consumed()` is negative ; return false;
     - pre-con: `consumed()` is zero or positive . return true.
       - post-con: `consumed()` return 0;

    `bool eat( char_type const& c );` {options:[eat-one-name:eat|parse]}
      -  return true - character is properly processed,
      -  pre-con: `consumed()` is zero or positive . false -  return character rejected. Internal state is unchanged. Can proceed with new character.
      -  pre-con: `consumed()` is negative. return false.

    `bool flush();`
      - Description: Force internal state that is no more input and set internal state so token may return value different than `size()`
      -  pre-con: `consumed()` is zero or positive.
      -- post-con: return true;
      -- post-con: `consumed()` is zero
      -- post-con: `token()` may return value less than `size()`.
      - pre-con:
      -- retun false

    `size_type token();`
    //! equal to size() - waiting for more character.
    //! not equal to size() - have complete token, last eaten character is not part of parsed token

  ### 3. Examples

  #### Make instance:

Just make one instance of `lex` for `char` type.
```c++
    typedef  std::lex<char> clex_t;
    clex_t l;

    //!< Important in manner that `lex` should be able to easily cooperate with classes that is able to handle Type-2 or Type-3 grammars.
    yacc_t y;
```

#### Fill with information
Parse stream that contains lines of comma separated numbers.

``` c++
    l.push( std::regex("[0-9]+") );
    l.push( std::regex("\n")     );
    l.push( std::regex(",")      );
    l.compile();
```

#### Parsing
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
