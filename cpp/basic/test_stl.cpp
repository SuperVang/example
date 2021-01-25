#include <iostream>

#include <gtest/gtest.h>

class AB {
 public:
  int a = 1;
  int b = 2;
  friend std::ostream &operator<<(std::ostream &output, const AB &D);
};

std::ostream &operator<<(std::ostream &output, const AB &D)
{
  output <<"( "<< D.a << ", " << D.b<<" )";
  return output;
}

TEST(CPP_STL, unique)
{
  {
    std::vector<int> repeated = {1, 3, 2, 2, 2, 2, 1, 1, 1, 4, 4, 4, 3};
    std::sort(repeated.begin(), repeated.end());
    std::copy(repeated.begin(), repeated.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    repeated.erase(std::unique(repeated.begin(), repeated.end()), repeated.end());
    std::copy(repeated.begin(), repeated.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
  }
  
  {


    std::vector<AB> repeated;
    repeated.push_back({1,1});
    repeated.push_back({1,2});
    repeated.push_back({1,1});
    repeated.push_back({1,2});

    std::sort(repeated.begin(), repeated.end(), [](const AB &ab1, const AB &ab2) {
          return std::make_tuple(ab1.a, ab1.b) < std::make_tuple(ab2.a, ab2.b);
        });
    std::copy(repeated.begin(), repeated.end(), std::ostream_iterator<AB>(std::cout, " "));
    std::cout << std::endl;
    repeated.erase(std::unique(repeated.begin(), repeated.end(),
                               [](const AB &ab1, const AB &ab2) {
                                 return std::make_tuple(ab1.a, ab1.b) ==
                                        std::make_tuple(ab2.a, ab2.b);
                               }),
                   repeated.end());
  
    std::copy(repeated.begin(), repeated.end(), std::ostream_iterator<AB>(std::cout, " "));
    std::cout << std::endl;
  }
  
  
}