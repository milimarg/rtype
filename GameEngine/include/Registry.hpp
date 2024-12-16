/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Registry                                                                         *
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

#ifndef REGISTRY_HPP
    #define REGISTRY_HPP

    #include <unordered_map>
    #include <any>
    #include <typeindex>
    #include <functional>
    #include "Storage.hpp"
    #include "SparseArray.hpp"
    #include "Entity.hpp"
    #include "MoveOnlyFunction.hpp"

namespace ecs
{
    class Registry {
        public:
            static Registry &getInstance();

            template <class Component>
            Storage<Component> &registerComponent();

            template <class Component>
            Storage<Component> &getComponents();

            template <class Component>
            const Storage<Component> &getComponents() const;

            template <class Component>
            bool hasComponent(entity_t e) const;

            template <class Component>
            bool hasComponent(size_t entityIndex) const;

            template <class Component>
            Component &getComponent(entity_t e);

            template <class Component>
            Component &getComponent(size_t entityIndex);

            entity_t spawnEntity();
            entity_t entityFromIndex(std::size_t idx) const;
            bool isValidEntity(size_t idx) const;
            void killEntity(entity_t const &e);
            void killEntity(size_t entityIndex);

            template <typename Component>
            void addComponent(entity_t const &to, Component &&c);

            template <typename Component, typename... Params>
            Registry &emplaceComponent(entity_t const &to, Params &&...p);

            template <typename Component, typename... Params>
            Registry &emplaceComponent(size_t idx, Params &&...p);

            template <typename Component, typename... Params>
            void safeEmplaceComponent(size_t idx, Params &&...p);

            template <typename Component>
            void removeComponent(entity_t const &from);

            template <typename Component>
            void removeComponent(size_t idx);

            template <class... Components, typename Function>
            Registry &addSystem(Function &&f);

            Registry &runSystems();
            Registry &optimizeStorages();
            Registry &update();

        private:
            Registry();
            ~Registry() = default;

            Registry(const Registry&) = delete;
            Registry(Registry&&) = delete;
            Registry &operator=(const Registry&) = delete;
            Registry &operator=(Registry&&) = delete;

            Registry &updateDeadEntities();
            Registry &updatePendingChanges();

            std::unordered_map<std::type_index, std::any> _componentsArrays;
            std::unordered_map<std::type_index, std::function<void(std::any&)>> _optimizers;
            std::unordered_map<std::type_index, std::function<void(Registry&, entity_t const&)>> _componentEraseFunctions;
            std::vector<entity_t> _freeEntities;
            std::vector<entity_t> _entitiesToFree;
            size_t _nextEntityId = 0;

            std::vector<std::function<void(Registry&)>> _systems;

            std::vector<MoveOnlyFunction<void()>> _pendingChanges;
    };
};

USING(ecs::Registry, ecs::Storage, ecs::entity_t);

template <class Component>
Storage<Component> &Registry::registerComponent()
{
    auto &array = _componentsArrays[typeid(Component)];
    array = Storage<Component>();

    _optimizers[typeid(Component)] = [](std::any &storage) {
        auto &componentArray = std::any_cast<Storage<Component>&>(storage);
        componentArray.optimizeStorage();
    };

    _componentEraseFunctions[typeid(Component)] = [](Registry &registry, entity_t const &entity) {
        registry.removeComponent<Component>(entity);
    };

    return std::any_cast<Storage<Component>&>(array);
}

template <class Component>
Storage<Component> &Registry::getComponents()
{
    auto it = _componentsArrays.find(typeid(Component));
    if (it == _componentsArrays.end())
        return registerComponent<Component>();
    return std::any_cast<Storage<Component>&>(it->second);
}

template <class Component>
const Storage<Component> &Registry::getComponents() const
{
    auto it = _componentsArrays.find(typeid(Component));
    if (it == _componentsArrays.end())
        throw std::out_of_range("Component not registered");
    return std::any_cast<const Storage<Component>&>(it->second);
}

template <class Component>
bool Registry::hasComponent(entity_t e) const
{
    auto &storage = getComponents<Component>();
    return storage.size() > e && storage[e].has_value();
}

template <class Component>
bool Registry::hasComponent(size_t entityIndex) const
{
    if (!isValidEntity(entityIndex))
        return false;

    auto &storage = getComponents<Component>();
    return storage.size() > entityIndex && storage[entityFromIndex(entityIndex)].has_value();
}

template <class Component>
Component &Registry::getComponent(entity_t e)
{
    auto &component = getComponents<Component>()[e];
    if (!component.has_value())
        throw std::out_of_range("Invalid component");
    return component.value();
}

template <class Component>
Component &Registry::getComponent(size_t entityIndex)
{
    if (!isValidEntity(entityIndex))
        throw std::out_of_range("Invalid component");

    auto &component = getComponents<Component>()[entityFromIndex(entityIndex)];
    if (!component.has_value())
        throw std::out_of_range("Invalid component");
    return component.value();
}

template <typename Component>
void Registry::addComponent(const entity_t &to, Component &&c)
{
    _pendingChanges.emplace_back([this, to, c = std::forward<Component>(c)]() mutable {
        auto &components = getComponents<Component>();
        components.insertAt(to, std::move(c));
    });
}

template <typename Component, typename... Params>
Registry &Registry::emplaceComponent(entity_t const &to, Params &&...p)
{
    _pendingChanges.emplace_back([this, to, params = std::make_tuple(std::move(p)...)]() mutable {
        auto &components = getComponents<Component>();
        std::apply([&components, to](auto &&...args) {
            components.emplaceAt(to, std::forward<decltype(args)>(args)...);
        }, std::move(params));
    });

    return *this;
}

template <typename Component, typename... Params>
Registry &Registry::emplaceComponent(size_t idx, Params &&...p)
{
    _pendingChanges.emplace_back([this, idx, params = std::make_tuple(std::move(p)...)]() mutable {
        if (!isValidEntity(idx))
            throw std::out_of_range("Invalid component");

        auto &components = getComponents<Component>();
        std::apply([this, &components, idx](auto &&...args) {
            components.emplaceAt(entityFromIndex(idx), std::forward<decltype(args)>(args)...);
        }, std::move(params));
    });

    return *this;
}

template <typename Component, typename... Params>
void Registry::safeEmplaceComponent(size_t idx, Params &&...p)
{
    _pendingChanges.emplace_back([this, idx, params = std::make_tuple(std::forward<Params>(p)...)]() mutable {
        if (!isValidEntity(idx))
            return;

        auto &components = getComponents<Component>();
        std::apply([this, &components, idx](auto &&...args) {
            components.emplaceAt(entityFromIndex(idx), std::forward<decltype(args)>(args)...);
        }, std::move(params));
    });
}

template <typename Component>
void Registry::removeComponent(entity_t const &from)
{
    _pendingChanges.emplace_back([this, from]() {
        Storage<Component> &components = getComponents<Component>();
        components.erase(from);
    });
}

template <typename Component>
void Registry::removeComponent(size_t idx)
{
    _pendingChanges.emplace_back([this, idx]() {
        if (!isValidEntity(idx))
            return;

        Storage<Component> &components = getComponents<Component>();
        components.erase(entityFromIndex(idx));
    });
}

template <class... Components, typename Function>
Registry &Registry::addSystem(Function &&f)
{
    _systems.emplace_back([func = std::forward<Function>(f)](Registry &r) {
        func(r, r.getComponents<Components>()...);
    });

    return *this;
}

#endif /* REGISTRY_HPP */
