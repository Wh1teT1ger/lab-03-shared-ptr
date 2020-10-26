// Copyright 2020 Burylov Denis <burylov01@mail.ru>

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <atomic>
#include <cstdio>
#include <utility>

template <typename T>
class SharedPtr {
  T* data;
  class ControlBlock {
    size_t counter;

   public:
    ControlBlock() : counter(0) {}

    int increment() {
      ++counter;
      return counter;
    }

    int decrement() {
      --counter;
      return counter;
    }

    int get() { return counter; }
  } * control_block;

 public:
  SharedPtr();
  explicit SharedPtr(T* ptr);
  SharedPtr(const SharedPtr& r);
  SharedPtr(SharedPtr&& r) noexcept;
  ~SharedPtr();
  auto operator=(const SharedPtr& r) -> SharedPtr&;
  auto operator=(SharedPtr&& r) noexcept -> SharedPtr&;
  operator bool() const;
  auto operator*() const -> T&;
  auto operator->() const -> T*;

  auto get() -> T*;
  void reset();
  void reset(T* ptr);
  void swap(SharedPtr& r);
  auto use_count() const -> size_t;
};

template <typename T>
SharedPtr<T>::SharedPtr() : data(nullptr), control_block(nullptr) {}

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : data(ptr) {
  control_block = new ControlBlock;
  control_block->increment();
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& r) {
  data = r.data;
  control_block = r.control_block;
  control_block->increment();
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& r) noexcept
    : data(std::move(r.data)), control_block(std::move(r.control_block)) {}

template <typename T>
SharedPtr<T>::~SharedPtr() {
  if (control_block != nullptr) {
    if (!control_block->decrement()) {
      delete data;
      delete control_block;
    }
  }
}

template <typename T>
auto SharedPtr<T>::operator=(const SharedPtr& r) -> SharedPtr& {
  data = r.data;
  control_block = r.control_block;
  control_block->increment();
  return *this;
}

template <typename T>
auto SharedPtr<T>::operator=(SharedPtr&& r) noexcept -> SharedPtr& {
  std::swap(data, r.data);
  std::swap(control_block, r.control_block);
  return *this;
}

template <typename T>
SharedPtr<T>::operator bool() const {
  return data != nullptr;
}

template <typename T>
auto SharedPtr<T>::operator*() const -> T& {
  return *data;
}

template <typename T>
auto SharedPtr<T>::operator->() const -> T* {
  return data;
}

template <typename T>
auto SharedPtr<T>::get() -> T* {
  return data;
}

template <typename T>
void SharedPtr<T>::reset() {
  if (control_block != nullptr && control_block->decrement() == 0) {
    delete data;
    delete control_block;
    data = nullptr;
    control_block = nullptr;
  }
}

template <typename T>
void SharedPtr<T>::reset(T* ptr) {
  reset();
  data = ptr;
  control_block = new ControlBlock;
  control_block->increment();
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr& r) {
  if (data != r.data) {
    std::swap(data, r.data);
    std::swap(control_block, r.control_block);
  }
}

template <typename T>
auto SharedPtr<T>::use_count() const -> size_t {
  return control_block->get();
}

#endif  // INCLUDE_SHAREDPTR_HPP_
