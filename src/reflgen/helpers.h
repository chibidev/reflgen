//
//  helpers.h
//  reflgen
//
//  Created by Zsolt Erhardt on 2018. 07. 12..
//  Copyright © 2018. Zsolt Erhardt. All rights reserved.
//

#pragma once

#include <string>

#include "ranges.h"
#include "linq.h"

using namespace std::literals;

namespace reflgen {
    const std::string export_attribute_name = "reflect"s;

    template<typename CppEntity>
    bool is_exported(CppEntity&& entity) {
        return ranges::any_of(entity.attributes(), [](auto&& attribute) {
            return attribute.name() == export_attribute_name;
        });
    }

    template<typename CppRange>
    decltype(auto) exported(CppRange&& range) {
        return range.where([](auto&& enum_ent) {
            return is_exported(enum_ent);
        });
    }
}
