/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Oct, 2024                                                     *
 * Title           - GameEngine                                                        *
 * Description     -                                                                    *
 *     MoveOnlyFunction                                                                 *
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

#ifndef INCLUDED_MOVEONLYFUNCTION_HPP
    #define INCLUDED_MOVEONLYFUNCTION_HPP

    #include <memory>
    #include <utility>

template <typename T>
class MoveOnlyFunction;

template <typename Ret, typename... Args>
class MoveOnlyFunction<Ret(Args...)> {
    std::unique_ptr<void, void(*)(void*)> ptr;
    Ret (*invoke)(void*, Args...);

    template <typename F>
    static Ret call(void *p, Args ...args)
    {
        return (*static_cast<F*>(p))(std::forward<Args>(args)...);
    }

    public:
        template <typename F>
        MoveOnlyFunction(F &&f)
            : ptr(new std::decay_t<F>(std::forward<F>(f)), [](void *p) { delete static_cast<std::decay_t<F>*>(p); }),
            invoke(&call<std::decay_t<F>>) {}

        Ret operator()(Args ...args) const
        {
            return invoke(ptr.get(), std::forward<Args>(args)...);
        }

        MoveOnlyFunction(MoveOnlyFunction&&) = default;
        // MoveOnlyFunction &operator=(MoveOnlyFunction&&) = default;

        MoveOnlyFunction &operator=(MoveOnlyFunction &&other) noexcept
        {
            ptr = std::move(other.ptr);
            invoke = other.invoke;
            other.ptr.reset();
            other.invoke = nullptr;
            return *this;
        }

        MoveOnlyFunction(const MoveOnlyFunction&) = delete;
        MoveOnlyFunction &operator=(const MoveOnlyFunction&) = delete;
};

#endif /* INCLUDED_MOVEONLYFUNCTION_HPP */
