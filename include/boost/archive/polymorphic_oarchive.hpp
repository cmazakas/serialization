#ifndef BOOST_ARCHIVE_POLYMORPHIC_OARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// polymorphic_oarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>

#include <string>
#include <boost/cstdint.hpp>

#include <boost/pfto.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/oserializer.hpp>
#include <boost/archive/detail/interface_oarchive.hpp>

namespace boost { 
namespace archive {
namespace detail {
    class basic_oarchive;
}

class polymorphic_oarchive :
    public detail::interface_oarchive<polymorphic_oarchive>
{
    friend class detail::interface_oarchive<polymorphic_oarchive>;
    friend class save_access;
private:
    // primitive types the only ones permitted by polymorphic archives
    virtual void save(const bool t) = 0;

    virtual void save(const char t) = 0;
    virtual void save(const signed char t) = 0;
    virtual void save(const unsigned char t) = 0;
    #ifndef BOOST_NO_CWCHAR
    #ifndef BOOST_NO_INTRINSIC_WCHAR_T
    virtual void save(const wchar_t t) = 0;
    #endif
    #endif
    virtual void save(const short t) = 0;
    virtual void save(const unsigned short t) = 0;
    virtual void save(const int t) = 0;
    virtual void save(const unsigned int t) = 0;
    virtual void save(const long t) = 0;
    virtual void save(const unsigned long t) = 0;
    #ifndef BOOST_NO_INT64
    virtual void save(const int64_t t) = 0;
    virtual void save(const uint64_t t) = 0;
    #endif
    virtual void save(const float t) = 0;
    virtual void save(const double t) = 0;

    // string types are treated as primitives
    virtual void save(const std::string & t) = 0;
    #ifndef BOOST_NO_STD_WSTRING
    virtual void save(const std::wstring & t) = 0;
    #endif

public:
    // these are used by the serialization library implementation.
    // should be private but it aint that easy
    virtual void save_object(
        const void *x, 
        const detail::basic_oserializer & bos
    ) = 0;
    virtual void save_pointer(
        const void * t,
        const detail::basic_pointer_oserializer * bpos_ptr
    ) = 0;
    virtual void save_null_pointer() = 0;
    // used for xml and other tagged formats
    virtual void save_start(const char * name) = 0;
    virtual void save_end(const char * name) = 0;
    virtual void register_basic_serializer(const detail::basic_oserializer & bos) = 0;

public:
    virtual unsigned int library_version() const = 0;
    virtual void end_preamble() = 0;
    // utility function implemented by all legal archives
    virtual void save_binary(const void * t, size_t size) = 0;

	// msvc and borland won't automatically pass these to the base class so
	// make it explicit here
    template<class T>
    void save_override(const T & t, BOOST_PFTO int)
    {
        archive::save(* this, t);
    }
    // special treatment for name-value pairs.
    template<class T>
    void save_override(const ::boost::serialization::nvp<T> & t, int)
    {
        save_start(t.name());
        archive::save(* this, t.value());
 		save_end(t.name());
    }
};

} // namespace archive
} // namespace boost

// required by smart_cast for compilers not implementing 
// partial template specialization
BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION(boost::archive::polymorphic_oarchive)

#endif // BOOST_ARCHIVE_POLYMORPHIC_OARCHIVE_HPP