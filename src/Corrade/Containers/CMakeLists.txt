#
#   This file is part of Corrade.
#
#   Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
#               2017, 2018, 2019 Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

set(CorradeContainers_HEADERS
    Array.h
    ArrayView.h
    ArrayViewStl.h
    ArrayViewStlSpan.h
    Containers.h
    EnumSet.h
    EnumSet.hpp
    GrowableArray.h
    LinkedList.h
    Optional.h
    OptionalStl.h
    Pointer.h
    PointerStl.h
    Reference.h
    ScopeGuard.h
    StaticArray.h
    StridedArrayView.h
    Tags.h)

set(CorradeContainers_PRIVATE_HEADERS
    Implementation/RawForwardList.h)

if(BUILD_DEPRECATED)
    list(APPEND CorradeContainers_HEADERS ScopedExit.h)
endif()

# Force IDEs to display all header files in project view
add_custom_target(CorradeContainers-headers SOURCES
    ${CorradeContainers_HEADERS}
    ${CorradeContainers_PRIVATE_HEADERS})
set_target_properties(CorradeContainers-headers PROPERTIES FOLDER "Corrade/Containers")

install(FILES ${CorradeContainers_HEADERS} DESTINATION ${CORRADE_INCLUDE_INSTALL_DIR}/Containers)

if(BUILD_TESTS)
    add_subdirectory(Test)
endif()

# Corrade::Containers target alias for superprojects
add_library(CorradeContainers INTERFACE)
target_link_libraries(CorradeContainers INTERFACE CorradeUtility)
add_library(Corrade::Containers ALIAS CorradeContainers)
