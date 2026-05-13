#pragma once
#include <functional>
#include <memory>

template <typename Signature> class AnyFunc;

template <typename Out, typename... Args> class AnyFunc<Out(Args...)> {
  struct Callable {
    virtual ~Callable() = default;
    virtual Out call(Args...) = 0;
    virtual std::unique_ptr<Callable> clone() const = 0;
  };

  template <typename F> struct CallableImpl final : Callable {
    F func;

    explicit CallableImpl(F func) : func(std::move(func)) {}
    Out call(Args... args) override {
      return func(std::forward<Args>(args)...);
    }

    std::unique_ptr<Callable> clone() const override {
      return std::make_unique<CallableImpl>(func);
    }
  };

  std::unique_ptr<Callable> callable;

public:
  AnyFunc() : callable(nullptr) {}

  template <typename F>
  AnyFunc(F func)
      : callable(std::make_unique<CallableImpl<F>>(std::move(func))) {}

  AnyFunc(const AnyFunc &other) {
    if (other.callable)
      callable = other.callable->clone();
  }

  AnyFunc &operator=(const AnyFunc &other) {
    if (this != &other)
      callable = other.callable ? other.callable->clone() : nullptr;
    return *this;
  }

  AnyFunc(AnyFunc &&other) = default;

  AnyFunc &operator=(AnyFunc &&other) = default;

  Out operator()(Args... args) const {
    if (!callable) {
      throw std::bad_function_call();
    }
    return callable->call(std::forward<Args>(args)...);
  }

  explicit operator bool() const noexcept { return callable != nullptr; }
};
