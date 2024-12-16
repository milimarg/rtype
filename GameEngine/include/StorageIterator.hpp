/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     StorageIterator                                                                  *
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

#ifndef STORAGEITERATOR_HPP
    #define STORAGEITERATOR_HPP

    #include <variant>
    #include <vector>
    #include <optional>
    #include "DenseIterator.hpp"

namespace ecs
{
    template <typename Component, bool isConst>
    class StorageIterator {
        public:
            using VectorIterator = typename std::conditional_t<isConst, typename std::vector<std::optional<Component>>::const_iterator, typename std::vector<std::optional<Component>>::iterator>;
            using DenseIt = DenseIterator<Component, isConst>;

            explicit StorageIterator(VectorIterator vectorIter);
            explicit StorageIterator(DenseIt denseIter);

            StorageIterator &operator++();
            StorageIterator operator++(int);
            StorageIterator operator+(size_t n) const;

            auto operator*();
            auto operator->();

            bool operator==(const StorageIterator &other) const;

        private:
            std::variant<VectorIterator, DenseIt> _iterator;
    };
};

namespace std {
    template <typename Component, bool isConst>
    struct iterator_traits<ecs::StorageIterator<Component, isConst>> {
        using iterator_category = std::forward_iterator_tag;
        using value_type = Component;
        using difference_type = std::ptrdiff_t;
        using pointer = typename std::conditional_t<isConst, const Component*, Component*>;
        using reference = typename std::conditional_t<isConst, const Component&, Component&>;
    };
}

USING(ecs::StorageIterator);

template <typename Component, bool isConst>
StorageIterator<Component, isConst>::StorageIterator(VectorIterator vectorIter) : _iterator(vectorIter) {}

template <typename Component, bool isConst>
StorageIterator<Component, isConst>::StorageIterator(DenseIt denseIter) : _iterator(denseIter) {}

template <typename Component, bool isConst>
auto StorageIterator<Component, isConst>::operator*()
{
    return std::visit([](const auto &iter) { return *iter; }, _iterator);
}

template <typename Component, bool isConst>
auto StorageIterator<Component, isConst>::operator->()
{
    return std::visit([](auto &iter) { return iter.operator->(); }, _iterator);
}

template <typename Component, bool isConst>
StorageIterator<Component, isConst> &StorageIterator<Component, isConst>::operator++()
{
    std::visit([](auto &iter) { ++iter; }, _iterator);
    return *this;
}

template <typename Component, bool isConst>
StorageIterator<Component, isConst> StorageIterator<Component, isConst>::operator++(int)
{
    StorageIterator temp = *this;
    std::visit([](auto &iter) { iter++; }, _iterator);
    return temp;
}

template <typename Component, bool isConst>
StorageIterator<Component, isConst> StorageIterator<Component, isConst>::operator+(size_t n) const
{
    StorageIterator result = *this;
    std::visit([n](auto &iter) { iter += n; }, result._iterator);
    return result;
}

template <typename Component, bool isConst>
bool StorageIterator<Component, isConst>::operator==(const StorageIterator &other) const
{
    return _iterator == other._iterator;
}

#endif /* STORAGEITERATOR_HPP */
