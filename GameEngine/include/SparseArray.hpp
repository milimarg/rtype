/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     SparseArray                                                                      *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef SPARSEARRAY_HPP
    #define SPARSEARRAY_HPP

    #include <vector>
    #include <optional>
    #include <memory>
    #include <iterator>
    #include <iostream>
    #include <algorithm>
    #include <stdexcept>
    #include "Using.hpp"

namespace ecs
{
    template <typename Component, typename Allocator = std::allocator<std::optional<Component>>>
    class SparseArray {
        public:
            using valueType = std::optional<Component>;
            using referenceType = valueType&;
            using constReferenceType = valueType const&;
            using container_t = std::vector<valueType, Allocator>;
            using sizeType = typename container_t::size_type;

            using iterator = typename container_t::iterator;
            using constIterator = typename container_t::const_iterator;

            SparseArray();

            SparseArray(const SparseArray&);
            SparseArray(SparseArray&&) noexcept;
            ~SparseArray() = default;

            SparseArray &operator=(const SparseArray&);
            SparseArray &operator=(SparseArray&&) noexcept;

            referenceType operator[](size_t idx);
            constReferenceType operator[](size_t idx) const;

            iterator begin();
            constIterator begin() const;
            constIterator cbegin() const;

            iterator end();
            constIterator end() const;
            constIterator cend() const;

            sizeType size() const;

            referenceType insertAt(sizeType pos, const Component&);
            referenceType insertAt(sizeType pos, Component&&);

            template <class... Params>
            referenceType emplaceAt(sizeType pos, Params&&...);

            void erase(sizeType pos);

            sizeType getIndex(const valueType&) const;

        private:
            container_t _data;
    };
};



USING(ecs::SparseArray);

template <typename Component, typename Allocator>
SparseArray<Component, Allocator>::SparseArray() : _data{} {}

template <typename Component, typename Allocator>
SparseArray<Component, Allocator>::SparseArray(const SparseArray &other) : _data(other._data) {}

template <typename Component, typename Allocator>
SparseArray<Component, Allocator>::SparseArray(SparseArray &&other) noexcept : _data(std::move(other._data)) {}

template <typename Component, typename Allocator>
SparseArray<Component, Allocator> &SparseArray<Component, Allocator>::operator=(const SparseArray &other)
{
    if (this != &other)
        _data = other._data;
    return *this;
}

template <typename Component, typename Allocator>
SparseArray<Component, Allocator> &SparseArray<Component, Allocator>::operator=(SparseArray &&other) noexcept
{
    if (this != &other)
        _data = std::move(other._data);
    return *this;
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::referenceType SparseArray<Component, Allocator>::operator[](size_t idx)
{
    if (idx >= _data.size())
        _data.resize(idx + 1, std::nullopt);
    if (!_data[idx].has_value())
        _data[idx] = std::nullopt;
    return _data[idx];
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::constReferenceType SparseArray<Component, Allocator>::operator[](size_t idx) const
{
    if (idx >= _data.size())
        throw std::out_of_range("SparseArray::operator[] index out of range");
    return _data[idx];
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::iterator SparseArray<Component, Allocator>::begin()
{
    return _data.begin();
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::constIterator SparseArray<Component, Allocator>::begin() const
{
    return _data.begin();
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::constIterator SparseArray<Component, Allocator>::cbegin() const
{
    return _data.cbegin();
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::iterator SparseArray<Component, Allocator>::end()
{
    return _data.end();
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::constIterator SparseArray<Component, Allocator>::end() const
{
    return _data.end();
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::constIterator SparseArray<Component, Allocator>::cend() const
{
    return _data.cend();
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::sizeType SparseArray<Component, Allocator>::size() const
{
    return _data.size();
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::referenceType SparseArray<Component, Allocator>::insertAt(sizeType pos, const Component &value)
{
    if (pos >= _data.size())
        _data.resize(pos + 1, std::nullopt);

    _data[pos] = value;
    return _data[pos];
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::referenceType SparseArray<Component, Allocator>::insertAt(sizeType pos, Component &&value)
{
    if (pos >= _data.size())
        _data.resize(pos + 1, std::nullopt);

    _data[pos] = std::move(value);
    return _data[pos];
}

template <typename Component, typename Allocator>
template <class... Params>
typename SparseArray<Component, Allocator>::referenceType SparseArray<Component, Allocator>::emplaceAt(sizeType pos, Params&&... params)
{
    if (pos >= _data.size())
        _data.resize(pos + 1, std::nullopt);

    auto &optComponent = _data[pos];

    if (optComponent.has_value()) {
        Allocator alloc;
        std::allocator_traits<Allocator>::destroy(alloc, std::addressof(optComponent.value()));
    }

    optComponent.emplace(std::forward<Params>(params)...);

    return _data[pos];
}

template <typename Component, typename Allocator>
void SparseArray<Component, Allocator>::erase(sizeType pos)
{
    if (pos < _data.size())
        _data[pos].reset();
}

template <typename Component, typename Allocator>
typename SparseArray<Component, Allocator>::sizeType SparseArray<Component, Allocator>::getIndex(const valueType &value) const
{
    auto it = std::ranges::find_if(_data, [&value](const valueType &entry) {
        return entry.has_value() && std::addressof(entry.value()) == std::addressof(value.value());
    });

    if (it != _data.end())
        return std::distance(_data.begin(), it);
    else
        return _data.size();
}

#endif /* SPARSEARRAY_HPP */
