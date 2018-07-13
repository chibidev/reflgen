//
//  extensions.h
//  reflgen
//
//  Created by Zsolt Erhardt on 2018. 01. 19..
//  Copyright © 2018. Zsolt Erhardt. All rights reserved.
//

#pragma once

#include <utility>
#include <cppast/cpp_class.hpp>
#include <cppast/cpp_enum.hpp>
#include <cppast/cpp_function.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_member_function.hpp>

#include "linq.h"

namespace cppast {
    template<typename Range, typename AstType>
    decltype(auto) children_of_type(Range&& range) {
        return linq::query(std::forward<Range>(range))
        .where([](auto&& item) {
            return item.kind() == AstType::kind();
        }).transform([](auto&& item) -> const AstType& {
            return static_cast<const AstType&>(item);
        });
    }
    
    template<typename Range>
    decltype(auto) classes(Range&& range) {
        return children_of_type<Range, cppast::cpp_class>(std::forward<Range>(range));
    }
    template<typename Range>
    decltype(auto) members(Range&& range) {
        return children_of_type<Range, cppast::cpp_member_variable>(std::forward<Range>(range));
    }

    template<typename T>
    bool is_public(T&& t) {
        return t.access_specifier() == cppast::cpp_access_specifier_kind::cpp_public;
    }
}
