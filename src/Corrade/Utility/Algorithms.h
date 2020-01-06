#ifndef Corrade_Utility_Algorithms_h
#define Corrade_Utility_Algorithms_h
/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Function @ref Corrade::Utility::copy()
 * @m_since_latest
 */

#include "Corrade/Containers/StridedArrayView.h"
#include "Corrade/Utility/visibility.h"

namespace Corrade { namespace Utility {

/**
@brief Copy an array view to another
@m_since_latest

Calls @ref std::memcpy() on the contents. Expects that both arrays have the
same size.
*/
CORRADE_UTILITY_EXPORT void copy(const Containers::ArrayView<const void>& src, const Containers::ArrayView<void>& dst);

/* Any chance these two below wouldn't need to be here? Otherwise copy() on
   ArrayView<char> is ambiguous :( OTOH I definitely don't want these delegated
   into the complex strided copy, as that'd be a lot of code emitted for simple
   cases. */

/**
@brief Copy an array view to another
@m_since_latest

Casts views into a @cpp void @ce type and delegates into
@ref copy(const Containers::ArrayView<const void>&, const Containers::ArrayView<void>&).
Expects that both arrays have the same size and @p T is a trivially copyable
type.
*/
template<class T> inline void copy(const Containers::ArrayView<const T>& src, const Containers::ArrayView<T>& dst) {
    static_assert(
        #ifdef CORRADE_STD_IS_TRIVIALLY_TRAITS_SUPPORTED
        std::is_trivially_copyable<T>::value
        #else
        __has_trivial_copy(T) && __has_trivial_destructor(T)
        #endif
        , "types must be trivially copyable");

    return copy(Containers::ArrayView<const void>(src), Containers::ArrayView<void>(dst));
}

/**
 * @overload
 * @m_since_latest
 */
template<class T> inline void copy(const Containers::ArrayView<T>& src, const Containers::ArrayView<T>& dst) {
    return copy(Containers::ArrayView<const T>(src), dst);
}

/**
@brief Copy a strided array view to another
@m_since_latest

Optimized to call @ref std::memcpy() on largest contiguous sub-dimensions,
looping over the non-contiguous dimensions. The function has specializations
for 1D, 2D, 3D and 4D, higher dimensions recurse into these. Expects that both
arrays have the same size.
@see @ref Containers::StridedArrayView::isContiguous()
*/
template<unsigned dimensions> void copy(const Containers::StridedArrayView<dimensions, const char>& src, const Containers::StridedArrayView<dimensions, char>& dst);

/**
 * @overload
 * @m_since_latest
 */
CORRADE_UTILITY_EXPORT void copy(const Containers::StridedArrayView1D<const char>& src, const Containers::StridedArrayView1D<char>& dst);

/**
 * @overload
 * @m_since_latest
 */
CORRADE_UTILITY_EXPORT void copy(const Containers::StridedArrayView2D<const char>& src, const Containers::StridedArrayView2D<char>& dst);

/**
 * @overload
 * @m_since_latest
 */
CORRADE_UTILITY_EXPORT void copy(const Containers::StridedArrayView3D<const char>& src, const Containers::StridedArrayView3D<char>& dst);

/**
 * @overload
 * @m_since_latest
 */
CORRADE_UTILITY_EXPORT void copy(const Containers::StridedArrayView4D<const char>& src, const Containers::StridedArrayView4D<char>& dst);

/**
@brief Copy a strided array view to another
@m_since_latest

Casts views into a @cpp char @ce type of one dimension more (where the last
dimension has a size of @cpp sizeof(T) @ce and delegates into
@ref copy(const Containers::StridedArrayView<dimensions, const char>&, const Containers::StridedArrayView<dimensions, char>&).
Expects that both arrays have the same size and @p T is a trivially copyable
type.
*/
template<unsigned dimensions, class T> void copy(const Containers::StridedArrayView<dimensions, const T>& src, const Containers::StridedArrayView<dimensions, T>& dst) {
    static_assert(
        #ifdef CORRADE_STD_IS_TRIVIALLY_TRAITS_SUPPORTED
        std::is_trivially_copyable<T>::value
        #else
        __has_trivial_copy(T) && __has_trivial_destructor(T)
        #endif
        , "types must be trivially copyable");

    return copy(Containers::arrayCast<dimensions + 1, const char>(src),
                Containers::arrayCast<dimensions + 1, char>(dst));
}

/**
 * @overload
 * @m_since_latest
 */
template<unsigned dimensions, class T> void copy(const Containers::StridedArrayView<dimensions, T>& src, const Containers::StridedArrayView<dimensions, T>& dst) {
    return copy(Containers::StridedArrayView<dimensions, const T>{src}, dst);
}

template<unsigned dimensions> void copy(const Containers::StridedArrayView<dimensions, const char>& src, const Containers::StridedArrayView<dimensions, char>& dst) {
    CORRADE_ASSERT(src.size() == dst.size(),
        "Utility::Algorithms::copy(): sizes" << src.size() << "and" << dst.size() << "don't match", );

    for(std::size_t i = 0, max = src.size()[0]; i != max; ++i)
        copy(src[i], dst[i]);
}

}}

#endif
