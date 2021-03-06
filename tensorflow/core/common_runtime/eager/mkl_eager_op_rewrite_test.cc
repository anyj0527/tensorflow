/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifdef INTEL_MKL

#include "tensorflow/core/common_runtime/device_mgr.h"
#include "tensorflow/core/common_runtime/eager/eager_op_rewrite_registry.h"
#include "tensorflow/core/framework/rendezvous.h"
#include "tensorflow/core/platform/test.h"
#include "tensorflow/core/util/mkl_util.h"

namespace tensorflow {

class EagerOpRewriteTest : public ::testing::Test {
 public:
  EagerOpRewriteTest() {}

  // Creates a new op to be used as input to MKL eager rewrite.
  static std::unique_ptr<tensorflow::EagerOperation> CreateOp(
      const string op_name) {
    std::unique_ptr<DeviceMgr> device_mgr =
        absl::make_unique<StaticDeviceMgr>(DeviceFactory::NewDevice(
            "CPU", {}, "/job:localhost/replica:0/task:0/device:CPU:0"));
    bool async = false;
    bool lazy_remote_tensor_copy = false;
    tensorflow::Rendezvous* rendezvous =
        new tensorflow::IntraProcessRendezvous(device_mgr.get());
    tensorflow::EagerContext* eager_ctx = new tensorflow::EagerContext(
        SessionOptions(),
        tensorflow::ContextDevicePlacementPolicy::DEVICE_PLACEMENT_SILENT,
        tensorflow::ContextMirroringPolicy::MIRRORING_NONE, async,
        lazy_remote_tensor_copy, device_mgr.get(), false, rendezvous,
        GetDefaultCustomKernelCreator());

    EagerExecutor executor_(false);
    std::unique_ptr<tensorflow::EagerOperation> op(
        new tensorflow::EagerOperation(eager_ctx));
    EXPECT_EQ(Status::OK(),
              op.get()->Reset(op_name.c_str(), nullptr, false, &executor_));
    eager_ctx->Unref();
    return op;
  }

  // Validates the result of MKL eager rewrite.
  static void CheckRewrite(EagerOperation* orig_op, string expected_op_name) {
    std::unique_ptr<tensorflow::EagerOperation> out_op;
    EagerOpRewriteRegistry::Global()->RunRewrite(
        EagerOpRewriteRegistry::PRE_EXECUTION, orig_op, &out_op);

    // actual_op_name is same as original op name if rewrite didn't happen.
    string actual_op_name = orig_op->Name();
    if (out_op) {
      actual_op_name = out_op->Name();
    }

    EXPECT_EQ(actual_op_name, expected_op_name);
  }
};

#define REGISTER_TEST(NAME, T, INPUT)                 \
  TEST_F(EagerOpRewriteTest, NAME##_##T) {            \
    auto orig_op = CreateOp("Conv2D");                \
    orig_op->MutableAttrs()->Set("T", T);             \
    orig_op->MutableAttrs()->Set("padding", "VALID"); \
    CheckRewrite(orig_op.get(), "_MklNativeConv2D");  \
  }
REGISTER_TEST_ALL_TYPES(Conv2D);
#undef REGISTER_TEST

#define REGISTER_TEST(NAME, T, INPUT)                    \
  TEST_F(EagerOpRewriteTest, NAME##_##T) {               \
    auto orig_op = CreateOp("Conv2D");                   \
    orig_op->MutableAttrs()->Set("T", T);                \
    orig_op->MutableAttrs()->Set("padding", "EXPLICIT"); \
    CheckRewrite(orig_op.get(), "Conv2D");               \
  }
REGISTER_TEST_ALL_TYPES(Conv2D_Explicit_Padding);
#undef REGISTER_TEST

#define REGISTER_TEST(NAME, T, INPUT)                             \
  TEST_F(EagerOpRewriteTest, NAME##_##T) {                        \
    auto orig_op = CreateOp("Conv2DBackpropInput");               \
    orig_op->MutableAttrs()->Set("T", T);                         \
    orig_op->MutableAttrs()->Set("padding", "VALID");             \
    CheckRewrite(orig_op.get(), "_MklNativeConv2DBackpropInput"); \
  }
REGISTER_TEST_ALL_TYPES(Conv2DBackpropInput);
#undef REGISTER_TEST

#define REGISTER_TEST(NAME, T, INPUT)                              \
  TEST_F(EagerOpRewriteTest, NAME##_##T) {                         \
    auto orig_op = CreateOp("Conv2DBackpropFilter");               \
    orig_op->MutableAttrs()->Set("T", T);                          \
    orig_op->MutableAttrs()->Set("padding", "VALID");              \
    CheckRewrite(orig_op.get(), "_MklNativeConv2DBackpropFilter"); \
  }
REGISTER_TEST_ALL_TYPES(Conv2DBackpropFilter);
#undef REGISTER_TEST

#define REGISTER_TEST(NAME, T, INPUT)               \
  TEST_F(EagerOpRewriteTest, NAME##_##T) {          \
    auto orig_op = CreateOp("BatchMatMul");         \
    orig_op->MutableAttrs()->Set("T", T);           \
    CheckRewrite(orig_op.get(), "_MklBatchMatMul"); \
  }
REGISTER_TEST_ALL_TYPES(BatchMatMul);
#undef REGISTER_TEST

#define REGISTER_TEST(NAME, T, INPUT)          \
  TEST_F(EagerOpRewriteTest, NAME##_##T) {     \
    auto orig_op = CreateOp("MatMul");         \
    orig_op->MutableAttrs()->Set("T", T);      \
    CheckRewrite(orig_op.get(), "_MklMatMul"); \
  }
REGISTER_TEST_ALL_TYPES(MatMul);
#undef REGISTER_TEST

}  // namespace tensorflow

#endif  // INTEL_MKL
