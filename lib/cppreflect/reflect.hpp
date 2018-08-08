#include <tuple>
#include <utility> // index_sequence, forward

namespace std {
    namespace detail {
        template<typename Tuple>
        struct index_sequence_for_tuple;

        template<typename... Args>
        struct index_sequence_for_tuple<std::tuple<Args...>> {
            using type = std::index_sequence_for<Args...>;
        };

        template<typename... Args>
        struct index_sequence_for_tuple<const std::tuple<Args...>> {
            using type = std::index_sequence_for<Args...>;
        };

        template<typename... Args>
        struct index_sequence_for_tuple<std::tuple<Args...>&> {
            using type = std::index_sequence_for<Args...>;
        };

        template<typename... Args>
        struct index_sequence_for_tuple<std::tuple<Args...>&&> {
            using type = std::index_sequence_for<Args...>;
        };

        template<typename... Args>
        struct index_sequence_for_tuple<const std::tuple<Args...>&> {
            using type = std::index_sequence_for<Args...>;
        };
    }

    template<typename Tuple>
    using index_sequence_for_tuple = typename detail::index_sequence_for_tuple<Tuple>::type;

    namespace detail {
        template<typename Tuple, template<typename T> typename Transformer, typename Indices>
        struct tuple_transform;

        template<typename Tuple, template<typename T> typename Transformer, size_t... Idx>
        struct tuple_transform<Tuple, Transformer, std::index_sequence<Idx...>> {
            using type = std::tuple<Transformer<decltype(std::get<Idx>(std::declval<Tuple>()))>...>;
        };
    }

    template<typename Tuple, template<typename T> typename Transformer>
    using tuple_transform = detail::tuple_transform<Tuple, Transformer, index_sequence_for_tuple<Tuple>>;

    namespace detail {
        template<typename T, typename F, size_t... Is>
        decltype(auto) for_each(T&& t, F&& f, std::index_sequence<Is...>) {
            if constexpr (std::is_same<decltype(f(std::get<0>(t))), void>::value)
                [[maybe_unused]] auto l = { (f(std::get<Is>(t)), 0)... };
            else
                return std::make_tuple(f(std::get<Is>(t))...);
        }
    }
 
    template<typename Tuple, typename F>
    decltype(auto) for_each_in_tuple(Tuple&& t, F&& f)
    {
        return detail::for_each(std::forward<Tuple>(t), std::forward<F>(f), std::index_sequence_for_tuple<Tuple>{});
    }
}

namespace reflection {
    template<typename T>
    struct class_info;

    template<typename InstanceType, typename MemberType>
    struct member {
        using member_type = MemberType;
        
        const char* const name;
        MemberType InstanceType::* const ptr;

        constexpr decltype(auto) value(const InstanceType* const instance) const {
            return instance->*ptr;
        }

        constexpr decltype(auto) value(const InstanceType& instance) const {
            return value(&instance);
        }

        constexpr decltype(auto) get(const InstanceType* const instance) const {
            return value(instance);
        }

        constexpr decltype(auto) get(const InstanceType& instance) const {
            return value(instance);
        }

        constexpr void set(InstanceType* const instance, const MemberType& value) const {
            instance->*ptr = value;
        }

        constexpr void set(InstanceType& instance, const MemberType& value) const {
            set(&instance, value);
        }
    };
    
    template<typename Type, typename Callable>
    decltype(auto) for_each_member_of(Callable&& c) {
        return std::for_each_in_tuple(class_info<typename std::decay<Type>::type>::members, std::forward<Callable>(c));
    }
}

#ifdef _MSC_VER
#elif defined(__GNUC__)
#   if defined(__clang__)
#       pragma clang diagnostic ignored "-Wunknown-attributes"
#   else
#       pragma gcc diagnostic ignored "-Wunknown-attributes"
#   endif
#endif