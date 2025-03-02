#pragma once

#include <tuple>
#include <utility>

template<typename Sig>
struct Signature;
template<typename Ret, typename...Args>
struct Signature<Ret(Args...)>{
	using type=std::tuple<Args...>;
};

template<typename Ret, typename Obj, typename...Args>
struct Signature<Ret(Obj::*)(Args...)>{
	using type=std::tuple<Args...>;
};
template<typename Ret, typename Obj, typename...Args>
struct Signature<Ret(Obj::*)(Args...)const>{
	using type=std::tuple<Args...>;
};
template<typename Fun>
concept is_fun=std::is_function_v<Fun>;

template<typename Fun>
concept is_mem_fun=std::is_member_function_pointer_v<std::decay_t<Fun>>;

template<typename Fun>
concept is_functor=std::is_class_v<std::decay_t<Fun>>&& requires(Fun&& t){
	&std::decay_t<Fun>::operator();
};

template<is_functor T>
auto Arguments(T&& t) -> typename Signature<decltype(&std::decay_t<T>::operator())>::type;

template<is_functor T>
auto Arguments(const T& t) -> typename Signature<decltype(&std::decay_t<T>::operator())>::type;

template<is_fun T>
auto Arguments(T&& t) -> typename Signature<T>::type;

template<is_fun T>
auto Arguments(const T& t) -> typename Signature<T>::type;

template<is_mem_fun T>
auto Arguments(T&& t) -> typename Signature<std::decay_t<T>>::type;

template<is_mem_fun T>
auto Arguments(const T& t) -> typename Signature<std::decay_t<T>>::type;