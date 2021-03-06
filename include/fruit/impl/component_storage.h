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

#ifndef FRUIT_COMPONENT_STORAGE_H
#define FRUIT_COMPONENT_STORAGE_H

#include "metaprogramming.h"
#include "type_info.h"
#include "component.utils.h"
#include "normalized_component_storage.h"

namespace fruit {
  
namespace impl {

template <typename AnnotatedSignature>
struct BindAssistedFactory;

/**
 * A component where all types have to be explicitly registered, and all checks are at runtime.
 * Used to implement Component<>, don't use directly.
 * 
 * This class handles the creation of types of the forms:
 * - shared_ptr<C>, [const] C*, [const] C&, C (where C is an atomic type)
 * - Injector<T1, ..., Tk> (with T1, ..., Tk of the above forms).
 */
class ComponentStorage {
private:
  using BindingData = NormalizedComponentStorage::BindingData;
  using BindingDataForMultibinding = NormalizedComponentStorage::BindingDataForMultibinding;
  using BindingDataSetForMultibinding = NormalizedComponentStorage::BindingDataSetForMultibinding;
  
  // Small "single-class" components usually have 2 bindings: a registerConstructor and a bind.
  static constexpr size_t max_num_immediate_bindings = 2;
  
  // The first `max_num_immediate_bindings' bindings are stored here, to avoid a memory allocation if the component is small.
  std::array<std::pair<const TypeInfo*, BindingData>, max_num_immediate_bindings> typeRegistryArray;
  size_t typeRegistryArray_numUsed = 0;
  
  // Flushes the bindings stored in typeRegistryArray (if any) into typeRegistry.
  // Returns *this for convenience.
  ComponentStorage& flushBindings();
  
  // Maps the type index of a type T to the corresponding BindingData object.
  std::vector<std::pair<const TypeInfo*, BindingData>> typeRegistry;
  
  // Maps the type index of a type T to a set of the corresponding BindingData objects (for multibindings).
  std::vector<std::pair<const TypeInfo*, BindingDataForMultibinding>> typeRegistryForMultibindings;
 
  void createBindingData(const TypeInfo* typeInfo,
                         BindingData::create_t create, 
                         BindingData::createArgument_t createArgument);
  
  void createBindingData(const TypeInfo* typeInfo,
                         BindingData::object_t storedSingleton,
                         BindingData::destroy_t deleteOperation);
  
  void createBindingDataForMultibinding(const TypeInfo* typeInfo,
                                        BindingData::create_t create,
                                        BindingData::createArgument_t createArgument,
                                        std::shared_ptr<char>(*createSet)(InjectorStorage&));
  
  void createBindingDataForMultibinding(const TypeInfo* typeInfo,
                                        BindingData::object_t storedSingleton,
                                        BindingData::destroy_t deleteOperation,
                                        std::shared_ptr<char>(*createSet)(InjectorStorage&));
  
  template <typename C>
  static std::shared_ptr<char> createSingletonSet(InjectorStorage& storage);
  
  template <typename... Ts>
  friend class fruit::Injector;
  
  friend class NormalizedComponentStorage;
  
public:
  operator NormalizedComponentStorage() &&;
  
  // I, C must not be pointers.
  template <typename I, typename C>
  void bind();
  
  template <typename C>
  void bindInstance(C& instance);
  
  template <typename C, typename... Args>
  void registerProvider(C* (*provider)(Args...));
  
  template <typename C, typename... Args>
  void registerProvider(C (*provider)(Args...));
  
  template <typename C, typename... Args>
  void registerConstructor();
  
  // List<Args...> must be equal to RequiredArgsForAssistedFactory<AnnotatedSignature>.
  template <typename AnnotatedSignature, typename... Args>
  void registerFactory(SignatureType<AnnotatedSignature>(*factory)(Args...));
  
  template <typename I, typename C>
  void addMultibinding();
  
  template <typename C>
  void addInstanceMultibinding(C& instance);
  
  template <typename C, typename... Args>
  void registerMultibindingProvider(C* (*provider)(Args...));
  
  template <typename C, typename... Args>
  void registerMultibindingProvider(C (*provider)(Args...));
  
  void install(ComponentStorage other);
};

} // namespace impl
} // namespace fruit

#include "component_storage.inlines.h"
#include "component_storage.templates.h"

#endif // FRUIT_COMPONENT_STORAGE_H
