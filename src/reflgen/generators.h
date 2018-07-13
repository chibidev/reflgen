//
//  generators.h
//  reflgen
//
//  Created by Zsolt Erhardt on 2018. 07. 12..
//  Copyright © 2018. Zsolt Erhardt. All rights reserved.
//

#pragma once

#include <functional>

#include "linq.h"
#include "../cppast/extensions.h"
#include "../utilities/string.h"

#include "helpers.h"

namespace reflgen {
    template<typename CppRange>
    decltype(auto) generate_classes(CppRange&& range, const std::string& prefix = {}) {
        auto&& classes = cppast::classes(range);
        auto&& exported_classes = reflgen::exported(classes);

        ranges::for_each(exported_classes, [&](auto&& cl) {
            auto&& class_name = cl.name();
            auto&& generated_class_name = class_name + "_class_info";

            {
                // Generate class declaration
                std::cout << "namespace reflection {" << std::endl;
                std::cout << "    struct " << generated_class_name << " {" << std::endl;
            }

            {
                // Generate members
                auto&& public_members = cppast::members(cl).where([](auto&& member) {
                    return cppast::is_public(member);
                });

                std::cout << "        static constexpr auto members = std::make_tuple(" << std::endl;
                
                auto&& member_declaration_line = public_members.transform([&](auto&& member) {
                    auto&& member_name = member.name();
                    return "            member<"s + prefix + class_name + ", "s + cppast::to_string(member.type()) + ">{\""s + member_name + "\", &"s + prefix + class_name + "::"s + member_name + "}"s;
                });
                std::cout << utilities::string::join_with(member_declaration_line, ",\n") << std::endl;
                std::cout << "        );" << std::endl;
            }

            {
                // Close the declaration
                std::cout << "    };" << std::endl;
                std::cout << "    template<>" << std::endl;
                std::cout << "    struct reflect<" << prefix << class_name << "> {" << std::endl;
                std::cout << "        using reflected_type = " << generated_class_name << ";" << std::endl;
                std::cout << "    };" << std::endl;
                std::cout << "}" << std::endl;
            }

            {
                // Recursing into subdeclarations so that subtypes also get generated
                generate_classes(cl, prefix + class_name + "::");
            }
        });
    }
}
