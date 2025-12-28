//
// Created by Matteo Ranzi on 28/12/25.
//

#ifndef PIPEX_MY_TYPE_TRAITS_H
#define PIPEX_MY_TYPE_TRAITS_H

#include <type_traits>

template<template<typename...> class Template, typename T>
struct is_specialization_of : std::false_type {};

template<template<typename...> class Template, typename... Args>
struct is_specialization_of<Template, Template<Args...>> : std::true_type {};

#endif //PIPEX_MY_TYPE_TRAITS_H