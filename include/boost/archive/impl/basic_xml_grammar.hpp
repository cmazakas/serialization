#ifndef BOOST_ARCHIVE_BASIC_XML_GRAMMAR_HPP
#define BOOST_ARCHIVE_BASIC_XML_GRAMMAR_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_xml_grammar.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// this module is derived from simplexml.cpp - an example shipped as part of
// the spirit parser.  This example contains the following notice:
/*=============================================================================
    simplexml.cpp

    Spirit V1.3
    URL: http://spirit.sourceforge.net/

    Copyright (c) 2001, Daniel C. Nuffer

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the copyright holder be held liable for
    any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute
    it freely, subject to the following restrictions:

    1.  The origin of this software must not be misrepresented; you must
        not claim that you wrote the original software. If you use this
        software in a product, an acknowledgment in the product documentation
        would be appreciated but is not required.

    2.  Altered source versions must be plainly marked as such, and must
        not be misrepresented as being the original software.

    3.  This notice may not be removed or altered from any source
        distribution.
=============================================================================*/
#include <string>

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

// supress noise
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
#  pragma warning (disable : 4786) // too long name, harmless warning
#endif

//#define BOOST_SPIRIT_DEBUG
//#include <boost/spirit/core.hpp>
#include <boost/spirit/core/non_terminal/rule.hpp>
#include <boost/spirit/core/non_terminal/grammar.hpp>

// the following hack is to evade a bogus error generated by using the
// word "arg" when bind.hpp has been included
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
#define arg xarg
#endif

#include <boost/spirit/utility/chset.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
#undef arg
#endif

#include <boost/archive/basic_archive.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/version.hpp>

namespace boost {
namespace archive {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// XML grammar parsing

template<class CharType>
class basic_xml_grammar : public boost::spirit::grammar<basic_xml_grammar<CharType> >
{
private:
    typedef BOOST_DEDUCED_TYPENAME std::basic_istream<CharType> IStream;
    typedef BOOST_DEDUCED_TYPENAME std::basic_string<CharType> StringType;
    typedef BOOST_DEDUCED_TYPENAME boost::spirit::chset<CharType> chset_t;
    typedef BOOST_DEDUCED_TYPENAME boost::spirit::chlit<CharType> chlit_t;
    typedef BOOST_DEDUCED_TYPENAME boost::spirit::scanner<
        BOOST_DEDUCED_TYPENAME  std::basic_string<CharType>::iterator
    > scanner_t;
    typedef BOOST_DEDUCED_TYPENAME boost::spirit::rule<scanner_t> rule_t;
    // Start grammar definition
    rule_t    
        Reference,
        Eq, 
        STag,
        ETag,
        AttValue, 
        CharRef1, 
        CharRef2, 
        CharRef, 
        AmpRef,
        LTRef,
        GTRef,
        AposRef,
        QuoteRef,
        CharData,
        content,
        Name,
        XMLDecl,
        DocTypeDecl,
        ClassIDAttribute,
        ObjectIDAttribute,
        ClassNameAttribute,
        TrackingAttribute,
        VersionAttribute,
		UnusedAttribute,
        Attribute,
        SignatureAttribute,
        SerializationWrapper,
        S;

    // XML Character classes
    chset_t
        BaseChar,
        Ideographic,
        Char, 
        Letter, 
        Digit,
        CombiningChar,
        Extender, 
        Sch,
        NameChar;

	void init_chset();

    bool my_parse(
        IStream & is,
        const rule_t &rule_, 
        const CharType delimiter = L'>'
    );
public:
    struct return_values {
        StringType object_name;
        StringType contents;
        class_id_type class_id;
        object_id_type object_id;
        version_type version;
        tracking_type tracking_level;
        StringType class_name;
        return_values() :
            version(0),
            tracking_level(false)
        {}
    } rv;
    bool parse_start_tag(IStream & is) ;
    bool parse_end_tag(IStream & is);
    bool parse_string(IStream & is, StringType & s);
    void init(IStream & is);
    void windup(IStream & is);
    basic_xml_grammar();
};


} // namespace archive
} // namespace boost

#endif // BOOST_ARCHIVE_BASIC_XML_GRAMMAR_HPP