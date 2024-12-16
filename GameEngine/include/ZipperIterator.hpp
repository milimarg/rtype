/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     ZipperIterator                                                                   *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef ZIPPERITERATOR_HPP
    #define ZIPPERITERATOR_HPP

    #include <tuple>
    #include <iterator>
    #include "Using.hpp"

namespace ecs
{
    template <class... Containers>
    class ZipperIterator {
        template <class Container>
        using iterator_t = decltype(std::declval<Container>().begin());

        template <class Container>
        using itReference_t = typename std::iterator_traits<iterator_t<Container>>::reference;

        public:
            using valueType = std::tuple<itReference_t<Containers>...>;
            using reference = valueType;
            using pointer = void;
            using differenceType = size_t;
            using iteratorCategory = std::input_iterator_tag;
            using iteratorTuple = std::tuple<iterator_t<Containers>...>;

            template <class...>
            friend class Zipper;

            ZipperIterator(iteratorTuple const &itTuple, size_t max, size_t idx = 0);

            ZipperIterator(ZipperIterator const &z);

            ZipperIterator operator++();
            ZipperIterator operator++(int);
            ZipperIterator operator+(size_t n) const;
            ZipperIterator &operator+=(size_t n);

            valueType operator*();
            valueType operator->();

            size_t getIdx() const;

        private:
            size_t _idx;
            size_t _max;
            iteratorTuple _current;

            template <size_t... Is>
            valueType toValue(std::index_sequence<Is...>);
            template <size_t... Is>
            void incrAll(std::index_sequence<Is...>);
            template <size_t... Is>
            bool allSet(std::index_sequence<Is...>);

            static constexpr std::index_sequence_for<Containers...> _seq{};
    };
};

USING(ecs::ZipperIterator);

template <class... Containers>
ZipperIterator<Containers...>::ZipperIterator(iteratorTuple const &itTuple, size_t max, size_t idx)
    : _idx(std::min(max, idx)), _max(max), _current(itTuple)
{
    if (!allSet(std::index_sequence_for<Containers...>{}))
        ++(*this);
}

template <class... Containers>
ZipperIterator<Containers...>::ZipperIterator(ZipperIterator const &z)
    : _idx(z._idx), _max(z._max), _current(z._current) {}

template <class... Containers>
ZipperIterator<Containers...> ZipperIterator<Containers...>::operator++()
{
    incrAll(std::index_sequence_for<Containers...>{});
    return *this;
}

template <class... Containers>
ZipperIterator<Containers...> ZipperIterator<Containers...>::operator++(int)
{
    ZipperIterator tmp = *this;
    ++(*this);
    return tmp;
}

template <class... Containers>
ZipperIterator<Containers...> ZipperIterator<Containers...>::operator+(size_t n) const
{
    ZipperIterator result = *this;
    for (size_t i = 0; i < n; ++i)
        result.operator++();
    return result;
}

template <class... Containers>
ZipperIterator<Containers...>& ZipperIterator<Containers...>::operator+=(size_t n)
{
    for (size_t i = 0; i < n; ++i)
        incrAll(std::index_sequence_for<Containers...>{});
    return *this;
}

template <class... Containers>
typename ZipperIterator<Containers...>::valueType ZipperIterator<Containers...>::operator*()
{
    if (_idx >= _max)
        throw std::out_of_range("Dereferencing past the end of ZipperIterator");
    return toValue(std::index_sequence_for<Containers...>{});
}

template <class... Containers>
typename ZipperIterator<Containers...>::valueType ZipperIterator<Containers...>::operator->()
{
    if (_idx >= _max)
        throw std::out_of_range("Dereferencing past the end of ZipperIterator");
    return toValue(std::index_sequence_for<Containers...>{});
}

template <class... Containers>
bool operator==(ZipperIterator<Containers...> const &lhs, ZipperIterator<Containers...> const &rhs)
{
    return lhs.getIdx() == rhs.getIdx();
}

template <class... Containers>
bool operator!=(ZipperIterator<Containers...> const &lhs, ZipperIterator<Containers...> const &rhs)
{
    return !(lhs == rhs);
}

template <class... Containers>
size_t ZipperIterator<Containers...>::getIdx() const
{
    return _idx;
}

template <class... Containers>
template <size_t... Is>
typename ZipperIterator<Containers...>::valueType ZipperIterator<Containers...>::toValue(std::index_sequence<Is...>)
{
    return std::tie(
        (std::get<Is>(_current)->has_value() ? std::get<Is>(_current)->value() : throw std::logic_error("Empty optional"))...
    );
}

template <class... Containers>
template <size_t... Is>
void ZipperIterator<Containers...>::incrAll(std::index_sequence<Is...>)
{
    if (_idx >= _max)
        return;

    do {
        (++std::get<Is>(_current), ...);
        ++_idx;
    } while (_idx < _max && !allSet(std::index_sequence_for<Containers...>{}));
}

template <class... Containers>
template <size_t... Is>
bool ZipperIterator<Containers...>::allSet(std::index_sequence<Is...>)
{
    if (_idx >= _max)
        return false;

    return (... && std::get<Is>(_current)->has_value());
}

#endif /* ZIPPERITERATOR_HPP */
