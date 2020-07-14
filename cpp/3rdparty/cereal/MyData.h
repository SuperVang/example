//
// Created by books on 2020/6/8.
//

#pragma once

#include <memory>

struct MyDATA {
  int x, y;
  double z;
  std::string name;

  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(CEREAL_NVP(x), CEREAL_NVP(y),  CEREAL_NVP(z),  CEREAL_NVP(name));
  }
};

struct MyType {
  double length;
  double width;
  MyDATA data;

  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(CEREAL_NVP(length), CEREAL_NVP(width), CEREAL_NVP(data));
  }
};

struct MyPointer {
  MyPointer(int _x, int _y) : x(_x), y(_y) {}

  template <class Archive>
  static void load_and_construct(Archive &ar,
                                 cereal::construct<MyPointer> &construct)
  {
    int _x, _y;
    ar(_x, _y);
    construct(_x, _y);
  }

  int x;
  int y;

  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(x, y);
  }
};



