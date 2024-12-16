/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     IndexedZipper                                                                    *
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

#ifndef INDEXEDZIPPER_HPP
    #define INDEXEDZIPPER_HPP

    #include "IndexedZipperIterator.hpp"
    #include <tuple>
    #include <algorithm>
    #include "Using.hpp"

namespace ecs
{
    template <class... Containers>
    class IndexedZipper {
        public:
            using iterator = IndexedZipperIterator<Containers...>;
            using iteratorTuple = typename iterator::iteratorTuple;

            explicit IndexedZipper(Containers &...cs);
            IndexedZipper(size_t startIdx, Containers &...cs);

            iterator begin();
            iterator end();

        private:
            static size_t _computeSize(Containers &...containers);
            static iterator _computeBegin(Containers &...containers, const size_t size);
            static iterator _computeEnd(Containers &...containers, const size_t size);

        private:
            size_t _size;
            iterator _begin;
            iterator _end;
    };
};

USING(ecs::IndexedZipper);

template <class... Containers>
IndexedZipper<Containers...>::IndexedZipper(Containers &...cs)
    : _size(_computeSize(cs...)),
    _begin(_computeBegin(cs..., _size)),
    _end(_computeEnd(cs..., _size)) {}

template <class... Containers>
IndexedZipper<Containers...>::IndexedZipper(size_t startIdx, Containers &...cs)
    : IndexedZipper(cs...)
{
    if (startIdx) {
        size_t idx = std::min(startIdx, _size);
        for (size_t i = 0; i < idx; ++i)
            ++_begin;
    }
}

template <class... Containers>
typename IndexedZipper<Containers...>::iterator IndexedZipper<Containers...>::begin()
{
    return _begin;
}

template <class... Containers>
typename IndexedZipper<Containers...>::iterator IndexedZipper<Containers...>::end()
{
    return _end;
}

template <class... Containers>
size_t IndexedZipper<Containers...>::_computeSize(Containers &...containers)
{
    return std::min({containers.size()...});
}

template <class... Containers>
typename IndexedZipper<Containers...>::iterator IndexedZipper<Containers...>::_computeBegin(Containers &...containers, const size_t size)
{
    return iterator(std::tuple(containers.begin()...), size);
}

template <class... Containers>
typename IndexedZipper<Containers...>::iterator IndexedZipper<Containers...>::_computeEnd(Containers &...containers, const size_t size)
{
    return iterator(std::tuple(containers.end()...), size, size);
}

#endif /* INDEXEDZIPPER_HPP */
