/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2024                                                     *
 * Title           - B-CPP-500-STG-5-1-bsrtype-augustin.grosnon                         *
 * Description     -                                                                    *
 *     Registry                                                                         *
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

#include "Registry.hpp"
#include "Using.hpp"
#include "TimerContainer.hpp"

USING(
    ecs::Registry,
    ecs::cmp::TimerContainer
);

Registry &Registry::getInstance()
{
    static thread_local Registry instance;
    return instance;
}

Registry::Registry() : _componentsArrays{}, _componentEraseFunctions({}) {};

// ? spawns an entity with a timer container
entity_t Registry::spawnEntity()
{
    if (_freeEntities.empty()) {
        entity_t newEntity(_nextEntityId++);
        emplaceComponent<TimerContainer>(newEntity);
        return newEntity;
    }

    entity_t id = _freeEntities.back();
    _freeEntities.pop_back();
    emplaceComponent<TimerContainer>(id);
    return id;
}

entity_t Registry::entityFromIndex(std::size_t idx) const
{
    if (!isValidEntity(idx))
        throw std::out_of_range("Invalid entity index: " + std::to_string(idx));

    return entity_t(idx);
}

bool Registry::isValidEntity(size_t idx) const
{
    return (
        idx < _nextEntityId
        && std::ranges::find(_freeEntities, entity_t(idx)) == _freeEntities.end()
    );
}

void Registry::killEntity(entity_t const &e)
{
    if (std::ranges::find(_freeEntities, e) != _freeEntities.end() || std::ranges::find(_entitiesToFree, e) != _entitiesToFree.end())
        return;
    _entitiesToFree.emplace_back(e);
    // TODO: resize storage down if destroying the last entity
}

void Registry::killEntity(size_t entityIndex)
{
    if (!isValidEntity(entityIndex))
        return;

    killEntity(entityFromIndex(entityIndex));
}

Registry &Registry::updateDeadEntities()
{
    for (auto e : _entitiesToFree) {
        for (const auto &[typeIndex, eraseFunction] : _componentEraseFunctions)
            eraseFunction(*this, e);
        _freeEntities.emplace_back(e);
    }
    _entitiesToFree.clear();

    return *this;
}

Registry &Registry::updatePendingChanges()
{
    for (auto &change : _pendingChanges)
        change();
    _pendingChanges.clear();

    return *this;
}

Registry &Registry::update()
{
    updatePendingChanges();
    updateDeadEntities();

    return *this;
}

Registry &Registry::runSystems()
{
    for (const auto &sys : _systems)
        sys(*this);

    return *this;
}

Registry &Registry::optimizeStorages()
{
    for (auto &[typeIndex, storage] : _componentsArrays) {
        auto it = _optimizers.find(typeIndex);
        if (it != _optimizers.end())
            it->second(storage);
        else
            std::cerr << "No optimizer registered for component type: " << typeIndex.name() << std::endl;
    }

    return *this;
}
