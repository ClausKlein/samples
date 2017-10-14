//
//  Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <iomanip>
#include <string>
#include <set>

#if defined(unix) && !defined(__CYGWIN__)
    #include <boost/locale.hpp>
#else
    #include <boost/locale/utf.hpp>
    #include <boost/locale/encoding_errors.hpp>
#endif


int main()
{
    using namespace boost::locale;

#if defined(unix) && !defined(__CYGWIN__)
    generator gen;
    std::locale::global(gen(""));

    /// Set global locale to requested
    /// Create a set that includes all strings sorted according to ABC order
    /// std::locale can be used as object for comparison
    typedef std::set<std::string, std::locale> set_type;
    set_type all_strings;
#else
    //FIXME! undefined reference to `boost::locale::generator::generator()'
#endif

    /// Read all strings into the set
    while (!std::cin.eof())
    {
        std::string tmp;
        std::getline(std::cin, tmp);
        try
        {
#if defined(unix) && !defined(__CYGWIN__)
            std::wstring s = conv::to_utf<wchar_t>("\xFF\xFF", "UTF-8", conv::stop);
            // Throws because this string is illegal in UTF-8
#else
            for (std::string::iterator p = tmp.begin(); p != tmp.end(); /* NOTE: do not ++p! */)
            {
                std::cout << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (0xff & static_cast<uint32_t>(*p));

                // Read one code point from the range [p,e) and return it.
                utf::code_point result = utf::utf_traits<char>::decode(p, tmp.end());
                std::string error;
                if (result == utf::illegal)
                {
                    error = "illegal";
                }
                else if (result == utf::incomplete)
                {
                    error = "incomplete";
                }
                if (!error.empty())
                {
                    std::cerr << std::endl << error << " utf8 code before[0x"
                              << std::hex << std::distance(tmp.begin(), p) << "] = ";
                    if (p != tmp.end())
                    {
                        std::cerr << *p << std::endl;
                    }

                    throw conv::conversion_error();
                }
            }
#endif
        }
        catch (std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
            continue;
        }

#if defined(unix) && !defined(__CYGWIN__)
        all_strings.insert(tmp);
#else
        std::cout << std::endl << tmp << std::endl;
#endif

    }

#if defined(unix) && !defined(__CYGWIN__)
    /// Print them out
    std::cout << std::endl << "sorted lines:" << std::endl;
    for (set_type::iterator p = all_strings.begin(); p != all_strings.end(); ++p)
    {
        std::cout << *p << std::endl;
    }
#endif

    return 0;
}

/***

klein_cl@WD002402 ~/workspace/boost_example
$ cat ../c/utf8checker/testtext.txt | ./collate
\x40\x7e\x7b\xe2\xc2\xc2\xc2\x5e\xc2\x21\x5c\x22\xc2\x24\x25\x26\x2f\x3d\x3f\xc2\x60\xc3\xc3\xc3\xc3\xc3\xc3\xc3\x7d\x54\x65\x73\x74\x54\x65\x78\x74
@~{€µ²³^°!\"§$%&/=?´`ÜÖÄüöäß}TestText

***/

// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
