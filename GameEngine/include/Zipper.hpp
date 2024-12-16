/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Zipper                                                                           *
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

#ifndef ZIPPER_HPP
    #define ZIPPER_HPP

    #include <algorithm>
    #include "ZipperIterator.hpp"

namespace ecs
{
    template <class... Containers>
    class Zipper {
        public:
            using iterator = ZipperIterator<Containers...>;
            using iteratorTuple = typename iterator::iteratorTuple;

            explicit Zipper(Containers &...cs);

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

USING(ecs::Zipper);

template <class... Containers>
Zipper<Containers...>::Zipper(Containers &...cs) :
    _size(_computeSize(cs...)),
    _begin(_computeBegin(cs..., _size)),
    _end(_computeEnd(cs..., _size)) {}

template <class... Containers>
typename Zipper<Containers...>::iterator Zipper<Containers...>::begin()
{
    return _begin;
}

template <class... Containers>
typename Zipper<Containers...>::iterator Zipper<Containers...>::end()
{
    return _end;
}

template <class... Containers>
size_t Zipper<Containers...>::_computeSize(Containers &...containers)
{
    return std::min({containers.size()...});
}

template <class... Containers>
typename Zipper<Containers...>::iterator Zipper<Containers...>::_computeBegin(Containers &...containers, const size_t size)
{
    return iterator(std::make_tuple(containers.begin()...), size);
}

template <class... Containers>
typename Zipper<Containers...>::iterator Zipper<Containers...>::_computeEnd(Containers &...containers, const size_t size)
{
    return iterator(std::make_tuple(containers.end()...), size, size);
    // ? instead of .end, we might need to do .begin + .size

    // return iterator(std::make_tuple((containers.begin() + size)...), size, size);
}

#endif /* ZIPPER_HPP */
