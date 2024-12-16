/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                      *
 * Description     -                                                                    *
 *     IndexedZipperIterator                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INDEXEDZIPPERITERATOR_HPP
    #define INDEXEDZIPPERITERATOR_HPP

    #include <tuple>
    #include <iterator>
    #include "Using.hpp"

namespace ecs
{
    template <class... Containers>
    class IndexedZipperIterator {
        public:
            using iteratorTuple = std::tuple<decltype(std::declval<Containers>().begin())...>;

            using valueType = std::tuple<size_t, typename std::iterator_traits<decltype(std::declval<Containers>().begin())>::reference...>;
            using reference = valueType;
            using pointer = void;
            using differenceType = size_t;
            using iteratorCategory = std::input_iterator_tag;

            IndexedZipperIterator(iteratorTuple const &itTuple, size_t max, size_t idx = 0);

            IndexedZipperIterator(IndexedZipperIterator const &z);

            IndexedZipperIterator operator++();
            IndexedZipperIterator operator++(int);
            IndexedZipperIterator operator+(size_t n) const;
            IndexedZipperIterator &operator+=(size_t n);

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
    };
};

USING(ecs::IndexedZipperIterator);

template <class... Containers>
IndexedZipperIterator<Containers...>::IndexedZipperIterator(iteratorTuple const &itTuple, size_t max, size_t idx)
    : _idx(std::min(max, idx)), _max(max), _current(itTuple)
{
    if (!allSet(std::index_sequence_for<Containers...>{}))
        ++(*this);
}

template <class... Containers>
IndexedZipperIterator<Containers...>::IndexedZipperIterator(IndexedZipperIterator const &z)
    : _idx(z._idx), _max(z._max), _current(z._current) {}

template <class... Containers>
IndexedZipperIterator<Containers...> IndexedZipperIterator<Containers...>::operator++()
{
    incrAll(std::index_sequence_for<Containers...>{});
    return *this;
}

template <class... Containers>
IndexedZipperIterator<Containers...> IndexedZipperIterator<Containers...>::operator++(int)
{
    IndexedZipperIterator tmp = *this;
    ++(*this);
    return tmp;
}

template <class... Containers>
IndexedZipperIterator<Containers...> IndexedZipperIterator<Containers...>::operator+(size_t n) const
{
    IndexedZipperIterator result = *this;
    for (size_t i = 0; i < n; ++i)
        result.operator++();
    return result;
}

template <class... Containers>
IndexedZipperIterator<Containers...>& IndexedZipperIterator<Containers...>::operator+=(size_t n)
{
    for (size_t i = 0; i < n; ++i)
        incrAll(std::index_sequence_for<Containers...>{});
    return *this;
}

template <class... Containers>
typename IndexedZipperIterator<Containers...>::valueType IndexedZipperIterator<Containers...>::operator*()
{
    if (_idx >= _max)
        throw std::out_of_range("Dereferencing past the end of IndexedZipperIterator");

    return toValue(std::index_sequence_for<Containers...>{});
}

template <class... Containers>
typename IndexedZipperIterator<Containers...>::valueType IndexedZipperIterator<Containers...>::operator->()
{
    if (_idx >= _max)
        throw std::out_of_range("Dereferencing past the end of IndexedZipperIterator");

    return toValue(std::index_sequence_for<Containers...>{});
}

template <class... Containers>
bool operator==(IndexedZipperIterator<Containers...> const &lhs, IndexedZipperIterator<Containers...> const &rhs)
{
    return lhs.getIdx() == rhs.getIdx();
}

template <class... Containers>
bool operator!=(IndexedZipperIterator<Containers...> const &lhs, IndexedZipperIterator<Containers...> const &rhs)
{
    return !(lhs == rhs);
}

template <class... Containers>
size_t IndexedZipperIterator<Containers...>::getIdx() const
{
    return _idx;
}

template <class... Containers>
template <size_t... Is>
typename IndexedZipperIterator<Containers...>::valueType IndexedZipperIterator<Containers...>::toValue(std::index_sequence<Is...>)
{
    return std::tuple_cat(
        std::make_tuple(_idx),
        std::tie(
            (std::get<Is>(_current)->has_value() ? std::get<Is>(_current)->value() : throw std::logic_error("Empty optional"))...
        )
    );
}

template <class... Containers>
template <size_t... Is>
void IndexedZipperIterator<Containers...>::incrAll(std::index_sequence<Is...>)
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
bool IndexedZipperIterator<Containers...>::allSet(std::index_sequence<Is...>)
{
    if (_idx >= _max)
        return false;

    return (... && std::get<Is>(_current)->has_value());
}

#endif /* INDEXEDZIPPERITERATOR_HPP */
