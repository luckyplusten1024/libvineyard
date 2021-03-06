/** Copyright 2020 Alibaba Group Holding Limited.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef MODULES_BASIC_STREAM_DATAFRAME_STREAM_H_
#define MODULES_BASIC_STREAM_DATAFRAME_STREAM_H_

#include <memory>
#include <vector>

#include "basic/stream/dataframe_stream.vineyard.h"
#include "client/client.h"

namespace vineyard {

/**
 * @brief DataframeStreamBuilder is used for building streams of dataframes
 *
 */
class DataframeStreamBuilder : public DataframeStreamBaseBuilder {
 public:
  explicit DataframeStreamBuilder(Client& client)
      : DataframeStreamBaseBuilder(client) {}

  std::shared_ptr<Object> Seal(Client& client) {
    auto bstream = DataframeStreamBaseBuilder::Seal(client);
    VINEYARD_CHECK_OK(client.CreateStream(bstream->id()));
    return std::static_pointer_cast<Object>(bstream);
  }
};
}  // namespace vineyard

#endif  // MODULES_BASIC_STREAM_DATAFRAME_STREAM_H_
