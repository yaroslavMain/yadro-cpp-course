#pragma once
#include <iostream>

template <typename Context> class Action {
  struct VTable {
    void (*dst)(void *) = nullptr;
    void (*mv)(void *, void *) = nullptr;
    void (*exec)(const void *, Context &) = nullptr;
  };

  void reset() {
    if (vtable) {
      vtable->dst(ptr);
      if (ptr != storage) {
        operator delete(ptr);
      }
    }
    ptr = nullptr;
    vtable = nullptr;
  }

  const VTable *vtable = nullptr;

  static constexpr size_t StorageSize = 32;
  alignas(max_align_t) char storage[StorageSize]{}; // For small object
  void *ptr = nullptr;                              // For huge objects

public:
  Action() = default;
  ~Action() { reset(); }

  Action(const Action &) = delete;
  Action &operator=(const Action &) = delete;

  Action(Action &&other) noexcept { *this = std::move(other); }
  Action &operator=(Action &&other) noexcept {
    if (this != &other) {
      reset();
      if (other.vtable) {
        vtable = other.vtable;
        if (other.ptr == other.storage) {
          ptr = storage;
          vtable->mv(other.ptr, ptr);
        } else {
          ptr = other.ptr;
        }
        other.ptr = nullptr;
        other.vtable = nullptr;
      }
    }
    return *this;
  }

  template <typename T, typename = std::enable_if_t<
                            !std::is_same_v<std::decay_t<T>, Action>>>
  Action(T &&obj) {
    using HelpType = std::decay_t<T>;

    static constexpr VTable vtable = {
        [](void *p) { static_cast<HelpType *>(p)->~HelpType(); },
        [](void *src, void *dist) {
          ::new (dist) HelpType(*static_cast<HelpType *>(src));
        },
        [](const void *p, Context &ctx) {
          static_cast<const HelpType *>(p)->execute(ctx);
        }};
    this->vtable = &vtable;

    if constexpr (sizeof(HelpType) <= StorageSize) {
      ptr = storage;
    } else {
      ptr = operator new(sizeof(HelpType));
    }
    ::new (ptr) HelpType(std::forward<T>(obj));
  }

  void operator()(Context &ctx) {
    if (vtable && vtable->exec)
      vtable->exec(ptr, ctx);
  }

  explicit operator bool() const noexcept { return vtable != nullptr; }
};
