#include <iostream>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <utility>
#include <memory>

class Base {};

class Derived1 : public Base {};

class Derived2 : public Base {};

template <typename return_type>
class Override {
	public:
	template <class PT1, class PT2>
	void setOverride(return_type (*override)(PT1 *, PT2 *)) {
		auto pair = std::make_pair(std::type_index(typeid(PT1)), std::type_index(typeid(PT2)));
		m_funcs[pair] = (return_type (*)(void*, void*))override;
	}

	template <class PT1, class PT2>
  return_type operator()(PT1 *p1, PT2 *p2) {
		auto pair = std::make_pair(std::type_index(typeid(PT1)), std::type_index(typeid(PT2)));
    auto func = m_funcs[pair];
		return func((void*)p1, (void*)p2);
  }

	private:
  std::map<std::pair<std::type_index, std::type_index>,
           return_type (*)(void*, void*)>
      m_funcs;
};

float testing_base(Base *x, Base *y) {
	std::cout << "testing base?\n";
	return 1.0f;
}

float testing_derived(Base *x, Derived1 *y) {
	std::cout << "testing derived?\n";
	return 1.0f;
}

int main() {
  std::cout << "Hello, world.\n";
	Base one, two;
	Derived1 three;
	Override<float> over;
	over.setOverride(testing_base);
	over.setOverride(testing_derived);

	over(&one, &two);
	over(&one, &three);
  return 1;
}
