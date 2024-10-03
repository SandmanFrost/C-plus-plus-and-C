#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>

#include "simple_vector.h"

using namespace std;

class X {
 public:
  X() : X(5) {}
  X(size_t num) : x_(num) {}
  X(const X& other) = delete;
  X& operator=(const X& other) = delete;
  X(X&& other) { x_ = exchange(other.x_, 0); }
  X& operator=(X&& other) {
    x_ = exchange(other.x_, 0);
    return *this;
  }
  size_t GetX() const { return x_; }

 private:
  size_t x_;
};

SimpleVector<int> GenerateVector(size_t size) {
  SimpleVector<int> v(size);
  iota(v.begin(), v.end(), 1);
  return v;
}

void TestTemporaryObjConstructor() {
  cout << "TestTemporaryObjConstructor: \t"s;
  const size_t size = 1000000;
  SimpleVector<int> moved_vector(GenerateVector(size));
  assert(moved_vector.GetSize() == size);
  cout << "OK!"s << endl;
}

void TestTemporaryObjOperator() {
  cout << "TestTemporaryObjOperator: \t"s;
  const size_t size = 1000000;
  SimpleVector<int> moved_vector;
  assert(moved_vector.GetSize() == 0);
  moved_vector = GenerateVector(size);
  assert(moved_vector.GetSize() == size);
  cout << "OK!"s << endl;
}

void TestNamedMoveConstructor() {
  cout << "TestNamedMoveConstructor: \t"s;
  const size_t size = 1000000;
  SimpleVector<int> vector_to_move(GenerateVector(size));
  assert(vector_to_move.GetSize() == size);
  SimpleVector<int> moved_vector(std::move(vector_to_move));
  assert(moved_vector.GetSize() == size);
  assert(vector_to_move.GetSize() == 0);
  cout << "OK!"s << endl;
}

void TestNamedMoveOperator() {
  cout << "TestNamedMoveOperator: \t\t"s;
  const size_t size = 1000000;
  SimpleVector<int> vector_to_move(GenerateVector(size));
  assert(vector_to_move.GetSize() == size);
  SimpleVector<int> moved_vector = std::move(vector_to_move);
  assert(moved_vector.GetSize() == size);
  assert(vector_to_move.GetSize() == 0);
  cout << "OK!"s << endl;
}

void TestNoncopiableMoveConstructor() {
  cout << "TestNoncopiableMoveConstructor: "s;
  const size_t size = 5;
  SimpleVector<X> vector_to_move;
  for (size_t i = 0; i < size; ++i) {
    vector_to_move.PushBack(X(i));
  }
  SimpleVector<X> moved_vector = std::move(vector_to_move);
  assert(moved_vector.GetSize() == size);
  assert(vector_to_move.GetSize() == 0);
  for (size_t i = 0; i < size; ++i) {
    assert(moved_vector[i].GetX() == i);
  }
  cout << "OK!"s << endl;
}

void TestNoncopiablePushBack() {
  const size_t size = 5;
  cout << "TestNoncopiablePushBack: \t"s;
  SimpleVector<X> v;
  for (size_t i = 0; i < size; ++i) {
    v.PushBack(X(i));
  }
  assert(v.GetSize() == size);
  for (size_t i = 0; i < size; ++i) {
    assert(v[i].GetX() == i);
  }
  cout << "OK!"s << endl;
}

void TestNoncopiableInsert() {
  const size_t size = 5;
  cout << "TestNoncopiableInsert: \t\t"s;
  SimpleVector<X> v;
  for (size_t i = 0; i < size; ++i) {
    v.PushBack(X(i));
  }

  v.Insert(v.begin(), X(size + 1));
  assert(v.GetSize() == size + 1);
  assert(v.begin()->GetX() == size + 1);
  v.Insert(v.end(), X(size + 2));
  assert(v.GetSize() == size + 2);
  assert((v.end() - 1)->GetX() == size + 2);
  v.Insert(v.begin() + 3, X(size + 3));
  assert(v.GetSize() == size + 3);
  assert((v.begin() + 3)->GetX() == size + 3);
  cout << "OK!"s << endl;
}

void TestNoncopiableErase() {
  const size_t size = 3;
  cout << "TestNoncopiableErase: \t\t"s;
  SimpleVector<X> v;
  for (size_t i = 0; i < size; ++i) {
    v.PushBack(X(i));
  }

  auto it = v.Erase(v.begin());
  assert(it->GetX() == 1);
  cout << "OK!"s << endl;
}

void TestResize() {
  const size_t size = 3;
  cout << "TestResize: \t\t\t"s;
  SimpleVector<X> v;
  for (size_t i = 0; i < size; ++i) {
    v.PushBack(X(5));
  }
  v.Resize(7);
  assert(std::all_of(v.begin() + 3, v.end(),
                     [](const X& item) { return item.GetX() == 5; }));
  assert(v.GetSize() == 7);
  assert(v.GetCapacity() >= v.GetSize());

  SimpleVector<X> v2(5);
  v2.Resize(8);
  assert(v2.GetSize() == 8);
  assert(v2.GetCapacity() >= v2.GetSize());
  assert(std::all_of(v.begin() + 3, v.end(),
                     [](const X& item) { return item.GetX() == 5; }));

  cout << "OK!"s << endl;
}

void TestDeleteObj() {
  cout << "TestDeleteObj: \t\t\t"s;
  SimpleVector<X> v;
  v.Clear();
  assert(v.GetSize() == 0);
  assert(v.GetCapacity() == 0);
  assert(v.IsEmpty());
  cout << "OK!"s << endl;
}

void TestDefaultConstructor() {
  cout << "TestDefaultConstructor: \t"s;
  SimpleVector<int> v(3);
  assert(std::all_of(v.begin(), v.end(),
                     [](const int& item) { return item == 0; }));

  SimpleVector<X> vx(3);
  assert(std::all_of(vx.begin(), vx.end(),
                     [](const X& item) { return item.GetX() == 5; }));
  cout << "OK!"s << endl;
}

inline void Test1() {
  {
    SimpleVector<int> v;
    assert(v.GetSize() == 0u);
    assert(v.IsEmpty());
    assert(v.GetCapacity() == 0u);
  }
  {
    SimpleVector<int> v(5);
    assert(v.GetSize() == 5u);
    assert(v.GetCapacity() == 5u);
    assert(!v.IsEmpty());
    for (size_t i = 0; i < v.GetSize(); ++i) {
      assert(v[i] == 0);
    }
  }
  {
    SimpleVector<int> v(3, 42);
    assert(v.GetSize() == 3);
    assert(v.GetCapacity() == 3);
    for (size_t i = 0; i < v.GetSize(); ++i) {
      assert(v[i] == 42);
    }
  }
  {
    SimpleVector<int> v{1, 2, 3};
    assert(v.GetSize() == 3);
    assert(v.GetCapacity() == 3);
    assert(v[2] == 3);
  }
  {
    SimpleVector<int> v(3);
    assert(&v.At(2) == &v[2]);
    try {
      v.At(3);
      assert(false);
    } catch (const std::out_of_range&) {
    } catch (...) {
      assert(false);
    }
  }
  {
    SimpleVector<int> v(10);
    const size_t old_capacity = v.GetCapacity();
    v.Clear();
    assert(v.GetSize() == 0);
    assert(v.GetCapacity() == old_capacity);
  }
  {
    SimpleVector<int> v(3);
    v[2] = 17;
    v.Resize(7);
    assert(v.GetSize() == 7);
    assert(v.GetCapacity() >= v.GetSize());
    assert(v[2] == 17);
    assert(v[3] == 0);
  }
  {
    SimpleVector<int> v(3);
    v[0] = 42;
    v[1] = 55;
    const size_t old_capacity = v.GetCapacity();
    v.Resize(2);
    assert(v.GetSize() == 2);
    assert(v.GetCapacity() == old_capacity);
    assert(v[0] == 42);
    assert(v[1] == 55);
  }
  {
    const size_t old_size = 3;
    SimpleVector<int> v(3);
    v.Resize(old_size + 5);
    v[3] = 42;
    v.Resize(old_size);
    v.Resize(old_size + 2);
    assert(v[3] == 0);
  }
  {
    {
      SimpleVector<int> v;
      assert(v.begin() == nullptr);
      assert(v.end() == nullptr);
    }
    {
      SimpleVector<int> v(10, 42);
      assert(v.begin());
      assert(*v.begin() == 42);
      assert(v.end() == v.begin() + v.GetSize());
    }
  }
}

inline void Test2() {
  {
    SimpleVector<int> v(1);
    v.PushBack(42);
    assert(v.GetSize() == 2);
    assert(v.GetCapacity() >= v.GetSize());
    assert(v[0] == 0);
    assert(v[1] == 42);
  }
  {
    SimpleVector<int> v(2);
    v.Resize(1);
    const size_t old_capacity = v.GetCapacity();
    v.PushBack(123);
    assert(v.GetSize() == 2);
    assert(v.GetCapacity() == old_capacity);
  }
  {
    SimpleVector<int> v{0, 1, 2, 3};
    const size_t old_capacity = v.GetCapacity();
    const auto old_begin = v.begin();
    v.PopBack();
    assert(v.GetCapacity() == old_capacity);
    assert(v.begin() == old_begin);
    assert((v == SimpleVector<int>{0, 1, 2}));
  }
  {
    SimpleVector<int> numbers{1, 2};
    auto numbers_copy(numbers);
    assert(&numbers_copy[0] != &numbers[0]);
    assert(numbers_copy.GetSize() == numbers.GetSize());
    for (size_t i = 0; i < numbers.GetSize(); ++i) {
      assert(numbers_copy[i] == numbers[i]);
      assert(&numbers_copy[i] != &numbers[i]);
    }
  }
  {
    assert((SimpleVector{1, 2, 3} == SimpleVector{1, 2, 3}));
    assert((SimpleVector{1, 2, 3} != SimpleVector{1, 2, 2}));

    assert((SimpleVector{1, 2, 3} < SimpleVector{1, 2, 3, 1}));
    assert((SimpleVector{1, 2, 3} > SimpleVector{1, 2, 2, 1}));

    assert((SimpleVector{1, 2, 3} >= SimpleVector{1, 2, 3}));
    assert((SimpleVector{1, 2, 4} >= SimpleVector{1, 2, 3}));
    assert((SimpleVector{1, 2, 3} <= SimpleVector{1, 2, 3}));
    assert((SimpleVector{1, 2, 3} <= SimpleVector{1, 2, 4}));
  }
  {
    SimpleVector<int> v1{42, 666};
    SimpleVector<int> v2;
    v2.PushBack(0);
    v2.PushBack(1);
    v2.PushBack(2);
    const int* const begin1 = &v1[0];
    const int* const begin2 = &v2[0];

    const size_t capacity1 = v1.GetCapacity();
    const size_t capacity2 = v2.GetCapacity();

    const size_t size1 = v1.GetSize();
    const size_t size2 = v2.GetSize();

    static_assert(noexcept(v1.swap(v2)));
    v1.swap(v2);
    assert(&v2[0] == begin1);
    assert(&v1[0] == begin2);
    assert(v1.GetSize() == size2);
    assert(v2.GetSize() == size1);
    assert(v1.GetCapacity() == capacity2);
    assert(v2.GetCapacity() == capacity1);
  }
  {
    SimpleVector<int> src_vector{1, 2, 3, 4};
    SimpleVector<int> dst_vector{1, 2, 3, 4, 5, 6};
    dst_vector = src_vector;
    assert(dst_vector == src_vector);
  }
  {
    SimpleVector<int> v{1, 29, 3, 22};
    v.Insert(v.begin() + 2, 42);
    assert((v == SimpleVector<int>{1, 29, 42, 3, 22}));
    v.Insert(v.begin(), 111);
    assert((v == SimpleVector<int>{111, 1, 29, 42, 3, 22}));
    v.Insert(v.begin() + 3, 222);
    assert((v == SimpleVector<int>{111, 1, 29, 222, 42, 3, 22}));
    v.Insert(v.end(), 99);
    assert((v == SimpleVector<int>{111, 1, 29, 222, 42, 3, 22, 99}));
  }
  {
    SimpleVector<int> v{1, 2, 3, 4};
    v.Erase(v.cbegin() + 2);
    assert((v == SimpleVector<int>{1, 2, 4}));
  }
}

void TestReserveConstructor() {
  using namespace std;
  cout << "TestReserveConstructor: \t"s;
  SimpleVector<int> v(Reserve(5));
  assert(v.GetCapacity() == 5);
  assert(v.IsEmpty());
  cout << "OK!"s << endl;
}

void TestReserveMethod() {
  using namespace std;
  cout << "TestReserveMethod: \t\t"s;
  SimpleVector<int> v;
  v.Reserve(5);
  assert(v.GetCapacity() == 5);
  assert(v.IsEmpty());
  v.Reserve(1);
  assert(v.GetCapacity() == 5);
  for (int i = 0; i < 10; ++i) {
    v.PushBack(i);
  }
  assert(v.GetSize() == 10);
  v.Reserve(100);
  assert(v.GetSize() == 10);
  assert(v.GetCapacity() == 100);
  for (int i = 0; i < 10; ++i) {
    assert(v[i] == i);
  }
  cout << "OK!"s << endl;
}

void AllTests() {
  TestDefaultConstructor();
  cout << "Test1: \t\t\t\t"s;
  Test1();
  cout << "OK!"s << endl << "Test2: \t\t\t\t"s;
  Test2();
  cout << "OK!"s << endl;
  TestReserveConstructor();
  TestReserveMethod();
  TestTemporaryObjConstructor();
  TestTemporaryObjOperator();
  TestNamedMoveConstructor();
  TestNamedMoveOperator();
  TestNoncopiableMoveConstructor();
  TestNoncopiablePushBack();
  TestNoncopiableInsert();
  TestNoncopiableErase();
  TestResize();
  TestDeleteObj();
}

int main() {
  AllTests();
  return 0;
}