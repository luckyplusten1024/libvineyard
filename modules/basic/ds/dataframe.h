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

#ifndef MODULES_BASIC_DS_DATAFRAME_H_
#define MODULES_BASIC_DS_DATAFRAME_H_

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "basic/ds/array.h"
#include "basic/ds/arrow.h"
#include "basic/ds/dataframe.vineyard.h"
#include "basic/ds/tensor.h"
#include "client/client.h"
#include "client/ds/blob.h"
#include "client/ds/i_object.h"
#include "common/util/ptree.h"

namespace vineyard {

/**
 * @brief DataFrameBuilder is used for construcing dataframes that supported by
 * vineyard
 *
 */
class DataFrameBuilder : public DataFrameBaseBuilder {
 public:
  explicit DataFrameBuilder(Client& client) : DataFrameBaseBuilder(client) {}

  /**
   * @brief Get the partition index of the global dataframe.
   *
   * @return The pair of the partition_index on rows and the partition_index on
   * columns.
   */
  const std::pair<size_t, size_t> partition_index() const;

  /**
   * @brief Set the index in the global dataframe.
   *
   * @param partition_index_row The row index.
   * @param partition_index_column The column index.
   */
  void set_partition_index(size_t partition_index_row,
                           size_t partition_index_column);

  /**
   * @brief Get the column of the given column name.
   *
   * @param column The given column name.
   * @return The shared pointer to the column tensor.
   */
  std::shared_ptr<ITensorBuilder> Column(std::string const& column) const;

  /**
   * @brief Add a column to the dataframe by registering a tensor builder to the
   * column name. When the dataframe is built, the tensor builder will be
   * employed to build the column.
   *
   * @param column The name of the column.
   * @param builder The tensor builder for the column.
   */
  void AddColumn(std::string const& column,
                 std::shared_ptr<ITensorBuilder> builder);

  /**
   * @brief Drop the column with the given column name.
   *
   * @param column The name of column to be dropped.
   */
  void DropColumn(std::string const& column);

  /**
   * @brief Build the dataframe object.
   * @param client The client connected to the vineyard server.
   */
  Status Build(Client& client) override;

 private:
  std::vector<std::string> columns_;
  std::unordered_map<std::string, std::shared_ptr<ITensorBuilder>> values_;
};

/**
 * @brief GlobalDataFrameBuilder is designed for building global dataframes
 *
 */
class GlobalDataFrameBuilder : public GlobalDataFrameBaseBuilder {
 public:
  explicit GlobalDataFrameBuilder(Client& client)
      : GlobalDataFrameBaseBuilder(client), object_set_builder_(client) {}

  ~GlobalDataFrameBuilder() = default;

  /**
   * @brief Get the partition shape of the global dataframe.
   *
   * @return The pair of <number_of_partitions_on_rows,
   * number_of_partitions_on_columns>.
   */
  const std::pair<size_t, size_t> partition_shape() const;

  /**
   * @brief Set the partition shape of the global dataframe.
   *
   * @param partition_shape_row The number of partitions on rows.
   * @param partition_shape_column The number of partitions on columns.
   */
  void set_partition_shape(size_t partition_shape_row,
                           size_t partition_shape_column);

  /**
   * @brief Add a partition in the vineyard instance to the global dataframe.
   *
   * @param instance_id The ID of the vineyard instance.
   * @param partition_id The ObjectID of the partition to added.
   *
   */
  void AddPartition(const InstanceID instance_id, ObjectID const partition_id);

  /**
   * @brief Add a group of partitions in the vineyard instance
   * to the global dataframe.
   *
   * @param instance_id The ID of the vineyard instance.
   * @param partition_id The vector of ObjectIDs for the
   * group of partitions to added.
   *
   */
  void AddPartitions(const InstanceID instance_id,
                     const std::vector<ObjectID>& partition_ids);

  /**
   * @brief Seal the meta data of the global dataframe.
   * When creating a global dataframe, clients from different
   * machines that are connected
   * to different vineyard instances will sync the partition info
   * to seal the meta data for the global dataframe.
   *
   * @param client The client connected to the vineyard server.
   */
  std::shared_ptr<Object> _Seal(Client& client) override;

  /**
   * @brief Build the global dataframe object.
   *
   * @param client The client connected to the vineyard server.
   */
  Status Build(Client& client) override;

 protected:
  std::shared_ptr<ObjectSet> objects_;
  ObjectSetBuilder object_set_builder_;

 private:
  size_t partition_shape_row_;
  size_t partition_shape_column_;
};

}  // namespace vineyard

#endif  // MODULES_BASIC_DS_DATAFRAME_H_
