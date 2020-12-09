#include <iostream>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <utility>
#include <memory>

class Base {};

class Derived1 : public Base {};

class Derived2 : public Base {};

template <class ...Args>
auto variable_tuple_of_type_indexes() {
	return std::make_tuple(std::type_index(typeid(Args))...);
}

template <typename return_type, class ...Args>
class Override {
	public:
	template <class ...FunctionArgs>
	void setOverride(return_type (*override)(FunctionArgs...)) {
		auto tuple = std::make_tuple(std::type_index(typeid(FunctionArgs))...);
		m_funcs[tuple] = (return_type (*)(...))override;
	}

	template <class ...FunctionArgs>
  return_type operator()(FunctionArgs... args) {
		auto tuple = std::make_tuple(std::type_index(typeid(FunctionArgs))...);
    auto func = m_funcs[tuple];
		return func(args...);
  }

	private:
  std::map<decltype(variable_tuple_of_type_indexes<Args...>()),
           return_type (*)(...)>
      m_funcs;
};

float testing_base(Base *x, Base *y, Base *z) {
	std::cout << "testing base?\n";
	return 1.0f;
}

float testing_derived1(Derived1 *x, Derived1 *y, Derived1 *z) {
	std::cout << "testing derived1?\n";
	return 1.0f;
}

float testing_derived2(Derived2  *x, Derived2 *y, Derived2 *z) {
	std::cout << "testing derived2?\n";
	return 1.0f;
}

int main() {
  std::cout << "Hello, world.\n";
	Base base;
	Derived1 derived1;
	Derived2 derived2;

	Override<float, Base*, Base*, Base *> over;
	over.setOverride(testing_base);
	over.setOverride(testing_derived1);
	over.setOverride(testing_derived2);

	over(&base, &base, &base);
	over(&derived1, &derived1, &derived1);
	over(&derived2, &derived2, &derived2);
	over(&derived2, &derived2, &derived2);
	over(&derived1, &derived1, &derived1);
	over(&base, &base, &base);

  return 1;
}
