#pragma once

#include <algorithm>
#include <initializer_list>
#include <stdexcept>

#include "array_ptr.h"

class ReserveProxyObj {
 public:
  ReserveProxyObj() noexcept = default;

  ReserveProxyObj(size_t capacity_to_reserve)
      : capacity_to_reserve_(capacity_to_reserve) {}

  size_t GetCapacity() const noexcept { return capacity_to_reserve_; }

 private:
  size_t capacity_to_reserve_ = 0;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
  return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
 public:
  using Iterator = Type*;
  using ConstIterator = const Type*;

  SimpleVector() noexcept = default;

  SimpleVector(ReserveProxyObj capacity) {
    Reserve(capacity.GetCapacity());
    size_ = 0;
  }

  explicit SimpleVector(size_t size)
      : arr_(size), size_(size), capacity_(size) {
    std::generate(begin(), end(), []() { return Type(); });
  }

  SimpleVector(size_t size, const Type& value) : SimpleVector(size) {
    std::fill(begin(), end(), value);
  }

  SimpleVector(std::initializer_list<Type> init)
      : arr_(init.size()), size_(init.size()), capacity_(init.size()) {
    std::move(init.begin(), init.end(), begin());
  }

  SimpleVector(const SimpleVector& other) {
    if (&other != this) {
      SimpleVector new_arr(other.size_);
      std::copy(other.begin(), other.end(), new_arr.begin());
      swap(new_arr);
    }
  }

  SimpleVector(SimpleVector&& rhs)
      : arr_(std::exchange(rhs.arr_, {})),
        size_(std::exchange(rhs.size_, 0)),
        capacity_(std::exchange(rhs.capacity_, 0)) {}

  SimpleVector& operator=(const SimpleVector& rhs) {
    if (this != &rhs) {
      SimpleVector tmp(rhs);
      swap(tmp);
    }
    return *this;
  }

  SimpleVector& operator=(SimpleVector&& rhs) {
    if (this != &rhs) {
      SimpleVector tmp(std::move(rhs));
      swap(tmp);
    }
    return *this;
  }

  void Reserve(size_t new_capacity) {
    if (capacity_ < new_capacity) {
      ArrayPtr<Type> new_arr(new_capacity);
      std::move(begin(), end(), new_arr.Get());
      arr_.swap(new_arr);
      capacity_ = new_capacity;
    }
  }

  void PushBack(const Type& item) {
    Iterator it = MakeIterToPushBack();
    *it = item;
  }

  void PushBack(Type&& item) {
    Iterator it = MakeIterToPushBack();
    *it = std::move(item);
  }

  Iterator Insert(ConstIterator position, const Type& value) {
    assert(position >= begin() && position <= end());
    if (size_ != capacity_) {
      std::copy_backward(const_cast<Iterator>(position), end(), end() + 1);
      arr_[position - arr_.Get()] = value;
      ++size_;
      return const_cast<Iterator>(position);
    } else {
      auto new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
      const auto insert_pos = position - arr_.Get();
      ArrayPtr<Type> new_vector(new_capacity);
      std::copy(begin(), const_cast<Iterator>(position), new_vector.Get());
      new_vector[insert_pos] = value;
      std::copy(const_cast<Iterator>(position), end(),
                new_vector.Get() + insert_pos + 1);
      arr_.swap(new_vector);
      ++size_;
      capacity_ = new_capacity;
      return arr_.Get() + insert_pos;
    }
  }
  
  Iterator Insert(ConstIterator position, Type&& value) {
    assert(position >= begin() && position <= end());
    if (size_ != capacity_) {
      std::move_backward(const_cast<Iterator>(position), end(), end() + 1);
      arr_[position - arr_.Get()] = std::move(value);
      ++size_;
      return const_cast<Iterator>(position);
    } else {
      auto new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
      const auto insert_pos = position - arr_.Get();
      ArrayPtr<Type> new_vector(new_capacity);
      std::move(begin(), const_cast<Iterator>(position), new_vector.Get());
      new_vector[insert_pos] = std::move(value);
      std::move(const_cast<Iterator>(position), end(),
                new_vector.Get() + insert_pos + 1);
      arr_.swap(new_vector);
      ++size_;
      capacity_ = new_capacity;
      return arr_.Get() + insert_pos;
    }
  }

  void PopBack() noexcept {
    assert(!IsEmpty());
    --size_;
  }

  Iterator Erase(ConstIterator pos) { return MakeIterForErase(pos); }

  Iterator Erase(Iterator pos) { return MakeIterForErase(pos); }

  void swap(SimpleVector& other) noexcept {
    arr_.swap(other.arr_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  size_t GetSize() const noexcept { return size_; }

  size_t GetCapacity() const noexcept { return capacity_; }

  bool IsEmpty() const noexcept { return size_ == 0; }

  Type& operator[](size_t index) noexcept {
    assert(index <= size_);
    return arr_[index];
  }

  const Type& operator[](size_t index) const noexcept {
    assert(index <= size_);
    return arr_[index];
  }

  Type& At(size_t index) {
    if (index >= size_) {
      throw std::out_of_range("Index is out of range");
    }
    return arr_[index];
  }

  const Type& At(size_t index) const {
    if (index >= size_) {
      throw std::out_of_range("Index is out of range");
    }
    return arr_[index];
  }

  void Clear() noexcept { size_ = 0; }

  void Resize(size_t new_size) {
    if (new_size <= size_) {
      size_ = new_size;
      return;
    } else if (new_size > capacity_) {
      Reserve(new_size);
      std::generate(begin() + size_, end(), []() { return Type{}; });
    } else {
      std::generate(begin() + size_, end() + new_size, []() { return Type{}; });
    }
    size_ = new_size;
  }

  Iterator begin() noexcept { return arr_.Get(); }

  Iterator end() noexcept { return arr_.Get() + size_; }

  ConstIterator begin() const noexcept { return arr_.Get(); }

  ConstIterator end() const noexcept { return arr_.Get() + size_; }

  ConstIterator cbegin() const noexcept { return arr_.Get(); }

  ConstIterator cend() const noexcept { return arr_.Get() + size_; }

 private:
  ArrayPtr<Type> arr_{};
  size_t size_ = 0;
  size_t capacity_ = 0;

  size_t GetNewCapacity(const size_t& new_size) const {
    size_t new_capacity =
        capacity_ != 0 ? new_size > capacity_ ? new_size * 2 : capacity_ : 1;
    return new_capacity;
  }

  bool IsNotAvailableCapacity(const size_t& new_size) const {
    return new_size > capacity_;
  }

  Iterator MakeIterToPushBack() {
    size_t new_size = size_ + 1;
    size_t new_capacity = GetNewCapacity(new_size);
    if (IsNotAvailableCapacity(new_size)) {
      Reserve(new_capacity);
    }
    size_ = new_size;
    capacity_ = new_capacity;
    size_t current_pos = size_ - 1;
    return Iterator{arr_.Get() + current_pos};
  }

  template <typename Iter>
  Iterator MakeIterToInsert(Iter pos) {
    Iterator it = Iterator(pos);
    auto current_pos = it - begin();
    size_t new_capacity = GetNewCapacity(size_ + 1);
    if (IsNotAvailableCapacity(size_ + 1)) {
      Reserve(new_capacity);
    }
    ++size_;
    it = begin() + current_pos;
    std::move_backward(it, end(), end() + 1);
    return it;
  }

  template <typename Iter>
  Iterator MakeIterForErase(Iter pos) {
    Iterator it = Iterator(pos);
    auto current_pos = begin() + std::distance(begin(), it);
    if (size_ > 0) {
      std::move(it + 1, end(), current_pos);
      --size_;
    }
    return begin() + std::distance(begin(), it);
  }
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs,
                       const SimpleVector<Type>& rhs) {
  return (lhs.GetSize() == rhs.GetSize()) &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs,
                       const SimpleVector<Type>& rhs) {
  return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs,
                      const SimpleVector<Type>& rhs) {
  return std::lexicographical_compare(
      lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
      [](const auto& lhs, const auto& rhs) { return lhs < rhs; });
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs,
                       const SimpleVector<Type>& rhs) {
  return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs,
                      const SimpleVector<Type>& rhs) {
  return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs,
                       const SimpleVector<Type>& rhs) {
  return !(lhs < rhs);
}