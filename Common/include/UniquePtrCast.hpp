#include <memory>

template <typename T, typename U>
auto static_pointer_cast(std::unique_ptr<U>&& p) {
    auto d = static_cast<T*>(p.release());
    return std::unique_ptr<T>{d};
}

template <typename T, template <typename...> class Container, typename U, typename... Us>
auto static_pointer_container_cast(Container<U,Us...>&& cont) {
    Container<std::unique_ptr<T>> ret;
    ret.reserve(cont.size());
    for (auto&& el : cont) {
        ret.push_back(static_pointer_cast<T>(std::move(el)));
    }
    return ret;
}

template <template <typename...> class Container, typename T, typename... Us>
auto pointer_container_get_values(Container<std::unique_ptr<T>,Us...>&& cont) {
    Container<T> ret;
    ret.reserve(cont.size());
    for (auto&& el : cont) {
        ret.push_back(*el);
    }
    return ret;
}
