/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     DenseIterator                                                                    *
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

#ifndef DENSEITERATOR_HPP
    #define DENSEITERATOR_HPP

    #include <vector>
    #include <optional>
    #include <type_traits>
    #include <iterator>
    #include <string>
    #include "Using.hpp"

namespace ecs
{
    template<typename Component, bool IsConst>
    class DenseIterator {
        public:
            using DenseStorage = typename std::conditional_t<IsConst, const std::vector<std::optional<Component>>, std::vector<std::optional<Component>>>;
            using IdStorage = typename std::conditional_t<IsConst, const std::vector<size_t>, std::vector<size_t>>;

            using iterator_category = std::forward_iterator_tag;
            using value_type = std::optional<Component>;
            using difference_type = std::ptrdiff_t;
            using pointer = std::conditional_t<IsConst, const std::optional<Component>*, std::optional<Component>*>;
            using reference = typename std::conditional_t<IsConst, const std::optional<Component>&, std::optional<Component>&>;

            DenseIterator(IdStorage &ids, DenseStorage &components, size_t totalIds, size_t currentIdx);

            DenseIterator &operator++();
            DenseIterator operator++(int);
            DenseIterator operator+(size_t n) const;
            DenseIterator &operator+=(size_t n);

            bool operator!=(const DenseIterator &other) const;
            reference operator*() const;
            pointer operator->() const;

        private:
            IdStorage &_ids;
            DenseStorage &_components;
            size_t _totalIds;
            size_t _currentIdx;

            reference getValue(size_t idx);
            reference getValue(size_t idx) const;
    };
}

USING(ecs::DenseIterator);

template<typename Component, bool isConst>
DenseIterator<Component, isConst>::DenseIterator(IdStorage &ids, DenseStorage &components, size_t totalIds, size_t currentIdx)
    : _ids(ids), _components(components), _totalIds(totalIds), _currentIdx(currentIdx) {}

template<typename Component, bool isConst>
DenseIterator<Component, isConst> &DenseIterator<Component, isConst>::operator++()
{
    ++_currentIdx;
    return *this;
}

template<typename Component, bool isConst>
DenseIterator<Component, isConst> DenseIterator<Component, isConst>::operator++(int)
{
    DenseIterator temp = *this;
    ++_currentIdx;
    return temp;
}

template<typename Component, bool isConst>
DenseIterator<Component, isConst> DenseIterator<Component, isConst>::operator+(size_t n) const
{
    DenseIterator result = *this;
    for (size_t i = 0; i < n; ++i)
        result.operator++();
    return result;
}

template<typename Component, bool isConst>
DenseIterator<Component, isConst>& DenseIterator<Component, isConst>::operator+=(size_t n)
{
    for (size_t i = 0; i < n; ++i)
        ++_currentIdx;
    return *this;
}

template<typename Component, bool isConst>
bool DenseIterator<Component, isConst>::operator!=(const DenseIterator &other) const
{
    return _currentIdx != other._currentIdx;
}

template<typename Component, bool isConst>
typename DenseIterator<Component, isConst>::reference DenseIterator<Component, isConst>::operator*() const
{
    return getValue(_currentIdx);
}

template<typename Component, bool isConst>
typename DenseIterator<Component, isConst>::pointer DenseIterator<Component, isConst>::operator->() const
{
    return &(getValue(_currentIdx));
}

template<typename Component, bool isConst>
typename DenseIterator<Component, isConst>::reference DenseIterator<Component, isConst>::getValue(size_t idx)
{
    if (idx >= _totalIds)
        throw std::out_of_range(std::string("Trying to get value out of range at index ") + std::to_string(idx));

    auto it = std::ranges::find(_ids, idx);
    if (it != _ids.end()) {
        size_t compIdx = std::distance(_ids.begin(), it);
        return _components[compIdx];
    } else {
        static thread_local std::optional<Component> empty;
        return empty;
    }
}

template<typename Component, bool isConst>
typename DenseIterator<Component, isConst>::reference DenseIterator<Component, isConst>::getValue(size_t idx) const
{
    auto it = std::ranges::find(_ids, idx);
    if (it != _ids.end()) {
        size_t compIdx = std::distance(_ids.begin(), it);
        return _components[compIdx];
    } else {
        static thread_local std::optional<Component> empty;
        return empty;
    }
}

#endif
