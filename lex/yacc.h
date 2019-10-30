#ifndef _b1ddccc386684f32bae622a2245c8245
#define _b1ddccc386684f32bae622a2245c8245

// Just mock.

#include <iostream>
#include <iomanip>

  class yacc
   {
    public:
     typedef std::size_t size_type;
     void accept( size_type const& t)
      {
       std::cout << "received token: " << t << std::endl;
      }
   };

#endif