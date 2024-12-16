/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Storage                                                                          *
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

#ifndef STORAGE_HPP
    #define STORAGE_HPP

    #include <variant>
    #include <vector>
    #include <iostream>
    #include <algorithm>
    #include <stdexcept>
    #include <ranges>
    #include "SparseArray.hpp"
    #include "DenseIterator.hpp"
    #include "StorageIterator.hpp"

namespace ecs
{
    template<typename Component, typename Allocator = std::allocator<std::optional<Component>>>
    class Storage {
        public:
            using SparseStorage = SparseArray<Component>;

            using DenseStorage = struct {
                std::vector<size_t> ids;
                std::vector<std::optional<Component>> components;
                size_t totalIds;
            };

            using StorageVariant = std::variant<SparseStorage, DenseStorage>;

            using valueType = std::optional<Component>;
            using referenceType = valueType&;
            using constReferenceType = const valueType&;
            using container_t = std::vector<valueType, Allocator>;

            using iterator = StorageIterator<Component, false>;
            using constIterator = StorageIterator<Component, true>;

            Storage();

            referenceType operator[](size_t idx);
            constReferenceType operator[](size_t idx) const;

            iterator begin();
            constIterator begin() const;
            constIterator cbegin() const;

            iterator end();
            constIterator end() const;
            constIterator cend() const;

            size_t size() const;

            referenceType insertAt(size_t entityId, const Component &component);

            template<typename... Params>
            referenceType emplaceAt(size_t entityId, Params &&...params);

            void erase(size_t entityId);

            void optimizeStorage();

        private:
            void switchToDenseIfNeeded();
            void switchToSparseIfNeeded();

            StorageVariant _storage;
    };
};

USING(ecs::Storage);

template<typename Component, typename Allocator>
Storage<Component, Allocator>::Storage() : _storage(SparseStorage{}) {}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::referenceType
Storage<Component, Allocator>::operator[](size_t idx)
{
    return std::visit([idx](auto &storage) -> referenceType {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return storage[idx];
        } else {
            auto it = std::ranges::find(storage.ids, idx);
            if (it != storage.ids.end()) {
                size_t index = std::distance(storage.ids.begin(), it);
                return storage.components[index];
            } else {
                throw std::out_of_range("Entity not found in dense storage");
            }
        }
    }, _storage);
}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::constReferenceType
Storage<Component, Allocator>::operator[](size_t idx) const
{
    return std::visit([idx](const auto &storage) -> constReferenceType {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return storage[idx];
        } else {
            auto it = std::ranges::find(storage.ids, idx);
            if (it != storage.ids.end()) {
                size_t index = std::distance(storage.ids.begin(), it);
                return storage.components[index];
            } else {
                throw std::out_of_range("Entity not found in dense storage");
            }
        }
    }, _storage);
}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::referenceType
Storage<Component, Allocator>::insertAt(size_t entityId, const Component &component)
{
    return std::visit([entityId, &component](auto &storage) -> referenceType {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return storage.insertAt(entityId, component);
        } else {
            auto it = std::ranges::find(storage.ids, entityId);
            size_t index = std::distance(storage.ids.begin(), it);

            if (it == storage.ids.end()) {
                storage.ids.push_back(entityId);
                storage.components.push_back(component);
                ++storage.totalIds;
            } else {
                storage.components[index] = component;
            }

            return storage.components[index];
        }
    }, _storage);
}

template<typename Component, typename Allocator>
template<typename... Params>
typename Storage<Component, Allocator>::referenceType
Storage<Component, Allocator>::emplaceAt(size_t entityId, Params &&...params)
{
    return std::visit([entityId, &params...](auto &storage) -> referenceType {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return storage.emplaceAt(entityId, std::forward<Params>(params)...);
        } else {
            storage.ids.push_back(entityId);

            // ? we could remove optionals for storage
            // ? -> we will need to handle retrieving values by always sending back a correct value OR throwing.
            // ? -> In the parent we then catch the error and handle logic accordingly (if an error was thrown, we skip)

            // ! doing this should fix this optional creation issue

            if (entityId >= storage.components.size())
                storage.components.resize(entityId + 1, std::nullopt);
            storage.components[entityId].emplace(std::forward<Params>(params)...);

            ++storage.totalIds; // ? instead of doing "++" and "--" we could only store the highest one
            return storage.components.back();
        }
    }, _storage);
}

template<typename Component, typename Allocator>
void Storage<Component, Allocator>::erase(size_t entityId)
{
    std::visit([entityId](auto &storage) {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            storage.erase(entityId);
        } else {
            auto it = std::ranges::find(storage.ids, entityId);
            if (it != storage.ids.end()) {
                size_t index = std::distance(storage.ids.begin(), it);
                storage.ids.erase(it);
                storage.components.erase(storage.components.begin() + index);
            }
        }
    }, _storage);
}

template<typename Component, typename Allocator>
size_t Storage<Component, Allocator>::size() const
{
    return std::visit([](const auto &storage) -> size_t {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return storage.size();
        } else {
            return storage.totalIds;
        }
    }, _storage);
}

template<typename Component, typename Allocator>
void Storage<Component, Allocator>::switchToDenseIfNeeded()
{
    if (std::holds_alternative<SparseStorage>(_storage)) {
        SparseStorage &sparse = std::get<SparseStorage>(_storage);
        size_t populatedCount = std::ranges::count_if(sparse, [](const auto &entry) {
            return entry.has_value();
        });

        if (populatedCount < sparse.size() / 5) {
            DenseStorage dense{};
            dense.totalIds = sparse.size();
            for (size_t i = 0; i < sparse.size(); ++i) {
                if (sparse[i].has_value()) {
                    dense.ids.push_back(i);
                    dense.components.push_back(sparse[i].value());
                }
            }
            _storage = std::move(dense);
        }
    }
}

template<typename Component, typename Allocator>
void Storage<Component, Allocator>::switchToSparseIfNeeded()
{
    if (std::holds_alternative<DenseStorage>(_storage)) {
        DenseStorage &dense = std::get<DenseStorage>(_storage);

        if (dense.components.size() > dense.totalIds / 5) {
            SparseStorage sparse;

            for (size_t i = 0; i < dense.ids.size(); ++i) {
                auto &optionalComponent = dense.components[i];
                if (optionalComponent.has_value())
                    sparse.insertAt(dense.ids[i], optionalComponent.value());
            }

            _storage = std::move(sparse);
            // std::clog << "Switched to sparse storage" << std::endl;
        }
    }
}

template<typename Component, typename Allocator>
void Storage<Component, Allocator>::optimizeStorage()
{
    switchToDenseIfNeeded();
    switchToSparseIfNeeded();
}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::iterator
Storage<Component, Allocator>::begin()
{
    return std::visit([](auto &storage) -> iterator {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return iterator(storage.begin());
        } else {
            return iterator(DenseIterator<Component, false>(storage.ids, storage.components, storage.totalIds, 0));
        }
    }, _storage);
}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::constIterator
Storage<Component, Allocator>::begin() const
{
    return std::visit([](const auto &storage) -> constIterator {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return constIterator(storage.begin());
        } else {
            return constIterator(DenseIterator<Component, true>(storage.ids, storage.components, storage.totalIds, 0));
        }
    }, _storage);
}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::constIterator
Storage<Component, Allocator>::cbegin() const
{
    return std::visit([](const auto &storage) -> constIterator {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return constIterator(storage.cbegin());
        } else {
            return constIterator(storage.components.cbegin()); // TODO: handle cbegin
        }
    }, _storage);
}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::iterator
Storage<Component, Allocator>::end()
{
    return std::visit([](auto &storage) -> iterator {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return iterator(storage.end());
        } else {
            return iterator(DenseIterator<Component, false>(storage.ids, storage.components, storage.totalIds, storage.totalIds));
        }
    }, _storage);
}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::constIterator
Storage<Component, Allocator>::end() const
{
    return std::visit([](const auto &storage) -> constIterator {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return constIterator(storage.end());
        } else {
            return constIterator(DenseIterator<Component, true>(storage.ids, storage.components, storage.totalIds, storage.totalIds));
        }
    }, _storage);
}

template<typename Component, typename Allocator>
typename Storage<Component, Allocator>::constIterator
Storage<Component, Allocator>::cend() const
{
    return std::visit([](const auto &storage) -> constIterator {
        using T = std::decay_t<decltype(storage)>;
        if constexpr (std::is_same_v<T, SparseStorage>) {
            return constIterator(storage.cend());
        } else {
            return constIterator(storage.components.cend()); // TODO: handle cend
        }
    }, _storage);
}

#endif /* STORAGE_HPP */
