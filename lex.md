LEX

2019/09/10
Dejan D.M. Milosavljevic
(dmilos at gmail dot com)

Table of Contents.
==================
  * I. Introduction
  * II. Motivation and Scope
  * III. Solutions
  * IV. Design Decisions
  * V. Impact On the Standard
  * VI. References

I. Introduction
===============
  Lex it self offer better handling and manipulation of regular expressions when stream parsing is needed. In here will be presented one possible implementation of lex.

II. Motivation and Scope
========================
  There is a trend to add more features to existing regular expression. \
Main reason for this i that regular expression can ( and must ) be implemented only by using finite state machines.
And there should be guarantee that implementation use finite state machines.
With this it is possible to guarantee time and complexity of regex execution( if not ).
See: Chomsky_hierarchy.

III. Solutions
===============

Add lex.

    Code sample:
    typedef  std::lex<int > lex_t;
    lex_t l;

    l.token( std::regex("[0-9]+"), []( lex_t::match_type const& mt )->void{ std::cout void{ std::cout void{ std::cout ( ifs ), std::istream_iterator() );
    // No need for reset or flush. everything is automatic inside eat( begin, end )

    //Feed lex one by one character. Condition: [lambda-have:(true)]
    std::ifstream ifs("test1.txt")
    while( ifs.eof() )
    {
        l.eat( ifs.get() );
    }
    l.flush(); // No more input. Call appropriate lambda if unique exists. Clear internal state.

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
                case( 0 ): std::cout



    template
      class lex //[name:lex|flex|Lex]
       {
        typedef character_name char_type;
        typedef std::size_t size_type;

        typedef std::regex regex_type;
        typedef --TODO-- match_type;
        typedef std::function action_type; //!
         iterator_name eat( iterator_name const& begin iterator_name const& end ); //! processing sequence, return last unprocessed or equal to end.
        void flush(); No more input. Finish parsing. Also clear internal state, same as restart

        // eqial to size() - waiting for more character.
        // not equal to size() - have complete token, last eaten character is not part of parsed token
        size_type token(); //[lambda-have:(false)],

        bool good()const; //! No push/remove performed after compile
        bool restart(); //! clear internal parse state
       };

IV. Summary of options
=====================
name.
 TODO.
 lambda-have.
 TODO.
 
IV. Design Decisions
=====================

  * TODO.
  
V. Impact On the Standard
===
 Core
   None. This is library level.
  Library
   None. This is new feature.
 Existing code.
  None. New class will be under std namespace.
  
VI. Links
=========

 Core
    # . Chomsky_hierarchy - https://en.wikipedia.org/wiki/Chomsky_hierarchy 3. A bit of history - https://en.wikipedia.org/wiki/Lex_%28software%29
------------------------------------
