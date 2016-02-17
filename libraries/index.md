---
layout: library
title: libraries
entities:
  - kind: class
    name: future
    declaration: |
        template <typename T>
        class future {
          public:
            future();
            ~future();

            future(const future&);
            future(future&&) noexcept;
            future& operator=(const future&);
            future& operator=(future&& x) noexcept;

            template <typename F>
            auto then(F&& f) -> future<std::result_of_t<F(T)>> const&;

            template <typename S, typename F>
            auto then(S&& s, F&& f) -> future<std::result_of_t<F(T)>> const&;

            template <typename F>
            auto then(F&& f) -> future<std::result_of_t<F(T)>> &&;

            template <typename S, typename F>
            auto then(S&& s, F&& f) -> future<std::result_of_t<F(T)>> &&;

            bool cancel_try();

            auto get_try() const&; // -> T == void ? bool : optional<T>
            auto get_try() &&; // -> T == void ? bool : optional<T>
        };
    description:
---
