#include <type_traits>
#include <variant>

template<typename T, typename Variant>
constexpr bool is_same(const Variant& obj) {
    bool ret { false };
    std::visit(
        [&ret](auto&& e) {
            if constexpr(std::is_same_v<std::decay_t<decltype(e)>, T> ) {
                ret = true;
            }
        },
        obj
    );
    return ret;
}