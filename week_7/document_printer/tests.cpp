#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "document.h"
#include "objs.h"

struct CustomType {
  friend std::ostream &operator<<(std::ostream &os, const CustomType &) {
    return os << "custom type called";
  }
};
struct UnsupportedType {};

TEST(DocumentTests, Basic) {
  Excel excel;
  Textedit edit1;
  Textedit edit2;
  PDF pdf;

  std::vector<Document> vec;
  vec.emplace_back(excel);
  vec.emplace_back(edit1);
  vec.emplace_back(edit2);
  vec.emplace_back(pdf);

  std::stringstream ss;
  vec[0].serialize(ss);
  vec[1].serialize(ss);
  vec[2].serialize(ss);
  ASSERT_EQ(ss.str(),
            "Hello from excel\nHello from textedit\nHello from textedit\n");
}

TEST(DocumentTests, MoveSemanticsAndEmptyState) {
  Excel excel;
  Document doc1(excel);
  Document doc2 = std::move(doc1);

  std::stringstream ss1;
  doc2.serialize(ss1);
  EXPECT_EQ(ss1.str(), "Hello from excel\n");

  std::stringstream ss2;
  doc1.serialize(ss2);
  EXPECT_TRUE(ss2.str().empty());
}

TEST(DocumentTests, StreamOperatorFallback) {
  CustomType custom_obj;
  Document doc(custom_obj);
  std::stringstream ss;
  doc.serialize(ss);
  EXPECT_EQ(ss.str(), "custom type called");
}

TEST(DocumentTests, UnsupportedTypeThrowsException) {
  constexpr UnsupportedType invalid_obj;
  const Document doc(invalid_obj);
  std::stringstream ss;
  EXPECT_THROW(doc.serialize(ss), std::logic_error);
}
