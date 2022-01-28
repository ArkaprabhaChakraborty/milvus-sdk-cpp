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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TypeUtils.h"
#include "mocks/MilvusMockedTest.h"

using ::milvus::StatusCode;
using ::milvus::proto::common::SegmentState;
using ::milvus::proto::milvus::GetPersistentSegmentInfoRequest;
using ::milvus::proto::milvus::GetPersistentSegmentInfoResponse;
using ::testing::_;
using ::testing::ElementsAre;
using ::testing::Property;

TEST_F(MilvusMockedTest, GetPersistentSegmentInfoWithoutConnection) {
    milvus::ConnectParam connect_param{"127.0.0.1", server_.ListenPort()};
    std::string collection = "foo";
    milvus::SegmentsInfo segments_info;
    auto status = client_->GetPersistentSegmentInfo(collection, segments_info);

    EXPECT_FALSE(status.IsOk());
    EXPECT_EQ(status.Code(), StatusCode::NOT_CONNECTED);
}

TEST_F(MilvusMockedTest, GetPersistentSegmentInfoFoo) {
    milvus::ConnectParam connect_param{"127.0.0.1", server_.ListenPort()};
    client_->Connect(connect_param);

    std::string collection = "foo";
    milvus::SegmentsInfo segments_info;

    milvus::SegmentsInfo segments_info_expected = {milvus::SegmentInfo{1, 1, 1, 1, milvus::SegmentState::FLUSHED},
                                                   milvus::SegmentInfo{2, 3, 4, 5, milvus::SegmentState::DROPPED}};

    EXPECT_CALL(service_,
                GetPersistentSegmentInfo(_, Property(&GetPersistentSegmentInfoRequest::collectionname, collection), _))
        .WillOnce([&segments_info_expected](::grpc::ServerContext*, const GetPersistentSegmentInfoRequest*,
                                            GetPersistentSegmentInfoResponse* response) {
            for (const auto& info_exp : segments_info_expected) {
                auto* info = response->add_infos();
                info->set_collectionid(info_exp.CollectionID());
                info->set_partitionid(info_exp.PartitionID());
                info->set_segmentid(info_exp.SegmentID());
                info->set_num_rows(info_exp.RowCount());
                info->set_state(milvus::SegmentStateCast(info_exp.State()));
            }
            return ::grpc::Status{};
        });
    auto status = client_->GetPersistentSegmentInfo(collection, segments_info);

    EXPECT_TRUE(status.IsOk());
    EXPECT_EQ(segments_info, segments_info_expected);
}
