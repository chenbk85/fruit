/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FRUIT_INJECTOR_H
#define FRUIT_INJECTOR_H

#include "component.h"

namespace fruit {
  
namespace impl {

class ComponentStorage;

} // namespace impl;

template <typename... P>
class Injector {
private:
  using Ps = fruit::impl::List<P...>;
  using M = Component<P...>;

  FruitDelegateCheck(fruit::impl::CheckNoRequirementsInInjector<typename M::Rs>);
  FruitDelegateChecks(fruit::impl::CheckClassType<P, fruit::impl::GetClassForType<P>>);  
    
  std::shared_ptr<fruit::impl::ComponentStorage> unsafeComponent;
  
  friend class fruit::impl::ComponentStorage;
  
  template <typename C>
  friend struct fruit::impl::GetHelper;
  
  Injector(fruit::impl::ComponentStorage& unsafeComponent);
  
public:
  Injector() = delete;
  
  // Injectors can't be copied, they own resources (singletons).
  Injector(const Injector&) = delete;
  
  // Move is ok.
  Injector(Injector&&) = default;
  
  Injector(const M& m);
  Injector(M&& m);
  
  template <typename T>
  T get();
  
  /**
   * This is a convenient way to call get(). E.g.:
   * 
   * MyInterface* x(injector);
   * 
   * is equivalent to:
   * 
   * MyInterface* x = injector.get<MyInterface*>();
   */
  template <typename T>
  explicit operator T();
};

} // namespace fruit


#include "impl/injector.templates.h"

#endif // FRUIT_INJECTOR_H