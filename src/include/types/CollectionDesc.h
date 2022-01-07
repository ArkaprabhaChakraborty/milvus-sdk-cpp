// Licensed to the LF AI & Data foundation under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership. The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <string>
#include <vector>

#include "CollectionSchema.h"

namespace milvus {

/**
 * @brief Collection schema and runtime information returned by DescribeCollection().
 */
class CollectionDesc {
 public:
    CollectionDesc() = default;
    CollectionDesc(const CollectionSchema& schema, int64_t collection_id, std::vector<std::string>& alias,
                   uint64_t create_utc_timestamp)
        : schema_{schema}, collection_id_{0}, alias_{0}, created_utc_timestamp_{0} {
    }

    CollectionSchema
    Schema() const {
        return schema_;
    }

    int64_t
    CollectionID() const {
        return collection_id_;
    }

    const std::vector<std::string>&
    Alias() const {
        return alias_;
    }

    uint64_t
    CreatedTime() const {
        return created_utc_timestamp_;
    }

 private:
    /**
     * @brief Collection schema defined by CreateCollection().
     */
    CollectionSchema schema_;

    /**
     * @brief Internal id of this collection.
     */
    int64_t collection_id_;

    /**
     * @brief Alias of this collection.
     */
    std::vector<std::string> alias_;

    /**
     * @brief The utc timestamp calculated by created_timestamp.
     */
    uint64_t created_utc_timestamp_ = 0;
};

}  // namespace milvus
