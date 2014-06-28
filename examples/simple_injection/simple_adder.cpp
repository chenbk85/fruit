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

#include "simple_adder.h"

class SimpleAdder : public Adder {
public:
  INJECT(SimpleAdder()) = default;
  
  // Not really needed, just to silence compiler warnings.
  virtual ~SimpleAdder() = default;
  
  virtual int add(int x, int y) override {
    return x + y;
  }
};

fruit::Component<Adder> getSimpleAdderComponent() {
  return fruit::createComponent()
    .bind<Adder, SimpleAdder>();
}