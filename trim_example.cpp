//  Boost string_algo library example file  ---------------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <iostream>
#include <string>

using namespace std;
// XXX using namespace boost;

int main()
{
    cout << "* Trim Example *" << endl << endl;

    string str1("     1x \t x \t x \t x1     ");
    const string str2("<>trim<>");
    const string str3("123abs343");

    // Simple left trim
    cout << "trim_left_copy of str1: "
         << "\"" << boost::trim_left_copy(str1) << "\"" << endl;

    // Inplace right trim
    boost::trim_right(str1);
    cout << "trim_right on str1: "
         << "\"" << str1 << "\"" << endl;

    cout << "trim_all_copy on str1: "
         << "\"" << boost::trim_all_copy(str1) << "\"" << endl;

    // Parametric trim. 'Space' is defined using is_any_of predicate
    cout << "trimmed copy of str4 ( space='<>' ): "
         << "\"" << trim_copy_if(str2, boost::is_any_of("<>")) << "\"" << endl;

    // Parametric trim. 'Space' is defined using is_digit predicate
    cout << "trimmed copy of str5 ( space=digit ): "
         << "\"" << boost::trim_copy_if(str3, boost::is_digit()) << "\""
         << endl;

    cout << endl;

    // iterator demo
    string str;
    const string str4("     1x \t x \t x \t x1     ");

#if 1
    str = boost::trim_all_copy(str4);
    cout << "trimmed all copy of str4 ( with iterator ): "
         << "\"";
    boost::trim_copy_if(ostream_iterator<char>(cout, ""), str4,
                        boost::is_space());
    cout << "\"" << endl;
#else
    boost::trim_copy_if(std::back_inserter(str), str4, boost::is_space());
#endif

    cout << "trimmed all copy of str4 ( with iterator ): "
         << "\"" << str << "\"" << endl;
    cout << endl;
    str.clear();

    return 0;
}
