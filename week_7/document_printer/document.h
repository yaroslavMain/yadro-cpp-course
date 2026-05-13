#pragma once

class Document {
  struct Base {
    virtual ~Base() = default;
    [[nodiscard]] virtual std::unique_ptr<Base> clone() const = 0;
    virtual void serialize(std::ostream &os) const = 0;
  };

  template <typename T> struct Model final : Base {
    T obj;

    explicit Model(T value) : obj(std::move(value)) {}
    void serialize(std::ostream &os) const override {
      if constexpr (requires(T t, std::ostream &os) { t.render(os); }) {
        obj.render(os);
      } else if constexpr (requires(T t, std::ostream &os) { t.print(os); }) {
        obj.print(os);
      } else if constexpr (requires(T t, std::ostream &os) { t.info(os); }) {
        obj.info(os);
      } else if constexpr (requires(T t, std::ostream &os) { t.text(os); }) {
        obj.text(os);
      } else if constexpr (requires(T t, std::ostream &os) { os << t; }) {
        os << obj;
      } else {
        throw std::logic_error("Failed to serialize object");
      }
    }

    [[nodiscard]] std::unique_ptr<Base> clone() const override {
      return std::make_unique<Model>(obj);
    }
  };

  std::unique_ptr<Base> pimpl;

public:
  template <typename T>
  Document(T value) : pimpl(std::make_unique<Model<T>>(std::move(value))) {}

  Document(const Document &other) {
    if (other.pimpl) {
      this->pimpl = other.pimpl->clone();
    }
  }
  Document &operator=(const Document &other) {
    if (this != &other) {
      this->pimpl = other.pimpl->clone();
    }
    return *this;
  }
  Document(Document &&other) noexcept = default;
  Document &operator=(Document &&other) noexcept = default;
  ~Document() = default;

  void serialize(std::ostream &os) const {
    if (pimpl)
      pimpl->serialize(os);
  }
};