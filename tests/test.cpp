// Copyright 2020 Burylov Denis <burylov01@mail.ru>

#include <gtest/gtest.h>

#include <SharedPtr.hpp>
#include <string>

TEST(SharedPtr, EmptyTest) { EXPECT_TRUE(true); }

TEST(SharedPtr, Empty) {
  SharedPtr<std::string> null{};
  EXPECT_FALSE(null);
}

TEST(SharedPtr, Copy) {
  SharedPtr<int> somePtr{new int(5)};
  EXPECT_EQ(*somePtr, 5);
  EXPECT_EQ(somePtr.use_count(), 1);

  auto copy1 = somePtr;
  EXPECT_TRUE(copy1);
  EXPECT_TRUE(somePtr);
  EXPECT_EQ(*copy1, 5);
  EXPECT_EQ(somePtr.use_count(), 2);

  auto copy2(copy1);
  EXPECT_TRUE(copy2);
  EXPECT_EQ(*copy2, 5);
  EXPECT_EQ(copy2.use_count(), 3);
  EXPECT_EQ(somePtr.get(), copy2.get());
}

TEST(SharedPtr, move) {
  SharedPtr<std::string> ptr1(new std::string("hello"));
  SharedPtr<std::string> ptr2 = new std::string("world");
  EXPECT_EQ(*ptr1, "hello");
  EXPECT_EQ(ptr1.use_count(), 1);
  ptr1 = std::move(ptr2);
  EXPECT_EQ(*ptr1, "world");
  EXPECT_EQ(ptr1.use_count(), 1);
}

TEST(SharedPtr, swap) {
  SharedPtr<int> ptr1(new int(1));
  SharedPtr<int> ptr2(new int(2));
  EXPECT_EQ(*ptr1, 1);
  ptr1.swap(ptr2);
  EXPECT_EQ(*ptr1, 2);
  EXPECT_EQ(ptr1.use_count(), 1);
  EXPECT_EQ(*ptr2, 1);
  EXPECT_EQ(ptr1.use_count(), 1);
}

TEST(SharedPtr, reset) {
  SharedPtr<int> ptr1(new int(1));
  EXPECT_EQ(*ptr1, 1);
  ptr1.reset();
  EXPECT_FALSE(ptr1);
  ptr1.reset(new int(5));
  EXPECT_EQ(*ptr1, 5);
  EXPECT_EQ(ptr1.use_count(), 1);
}
