/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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
#include <cstdio>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/signature_runner.h"
#include <iostream>

// This is an example that is minimal to read a model
// from disk and perform inference. There is no data being loaded
// that is up to you to add as a user.
//
// NOTE: Do not add any dependencies to this that cannot be built with
// the minimal makefile. This example must remain trivial to build with
// the minimal build tool.
//
// Usage: minimal <tflite model>


// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754
// SHOULD CHECK https://github.com/tensorflow/tensorflow/issues/55754


#define TFLITE_MINIMAL_CHECK(x)                              \
  if (!(x)) {                                                \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                                 \
  }

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "minimal <tflite model>\n");
    return 1;
  }
  const char* filename = argv[1];

  // Load model
  std::unique_ptr<tflite::FlatBufferModel> model =
      tflite::FlatBufferModel::BuildFromFile(filename);
  TFLITE_MINIMAL_CHECK(model != nullptr);

  // Build the interpreter with the InterpreterBuilder.
  // Note: all Interpreters should be built with the InterpreterBuilder,
  // which allocates memory for the Interpreter and does various set up
  // tasks so that the Interpreter can read the provided model.
  tflite::ops::builtin::BuiltinOpResolver resolver;
  tflite::InterpreterBuilder builder(*model, resolver);
  std::unique_ptr<tflite::Interpreter> interpreter;
  builder(&interpreter);
  TFLITE_MINIMAL_CHECK(interpreter != nullptr);

  auto signature_defs = interpreter->signature_keys();
  for (int i = 0; i < signature_defs.size(); ++i) {
    std::cout << i << " " << signature_defs.at(i)->c_str() << std::endl;
  }

  TFLITE_MINIMAL_CHECK(interpreter->AllocateTensors() == kTfLiteOk);

  tflite::SignatureRunner* infer_runner =
    interpreter->GetSignatureRunner("infer");
  printf("infer_runner: %p\n", infer_runner);

  const std::vector<const char*>& infer_input_names = infer_runner->input_names();
  const std::vector<const char*>& infer_output_names = infer_runner->output_names();

  std::cout << "input_names of infer_runner" << std::endl;
  for (int i = 0; i < infer_input_names.size(); ++i) {
    std::cout << i << " " << infer_input_names.at(i) << std::endl;
  }

  std::cout << "output_names of infer_runner" << std::endl;
  for (int i = 0; i < infer_output_names.size(); ++i) {
    std::cout << i << " " << infer_output_names.at(i) << std::endl;
  }

  TFLITE_MINIMAL_CHECK(infer_runner->AllocateTensors() == kTfLiteOk);
  TFLITE_MINIMAL_CHECK(infer_runner->Invoke() == kTfLiteOk);

  tflite::SignatureRunner* train_runner =
    interpreter->GetSignatureRunner("train");
  printf("train_runner: %p\n", train_runner);

  const std::vector<const char*>& train_input_names = train_runner->input_names();
  const std::vector<const char*>& train_output_names = train_runner->output_names();

  std::cout << "input_names of train_runner" << std::endl;
  for (int i = 0; i < train_input_names.size(); ++i) {
    std::cout << i << " " << train_input_names.at(i) << std::endl;
  }

  std::cout << "output_names of train_runner" << std::endl;
  for (int i = 0; i < train_output_names.size(); ++i) {
    std::cout << i << " " << train_output_names.at(i) << std::endl;
  }

  TFLITE_MINIMAL_CHECK(train_runner->AllocateTensors() == kTfLiteOk);
  TFLITE_MINIMAL_CHECK(train_runner->Invoke() == kTfLiteOk);




  // Test save_runner and restore_runner

  // tflite::SignatureRunner* save_runner =
  //   interpreter->GetSignatureRunner("save");
  // printf("save_runner: %p\n", save_runner);

  // const std::vector<const char*>& save_input_names = save_runner->input_names();
  // const std::vector<const char*>& save_output_names = save_runner->output_names();

  // std::cout << "input_names of save_runner" << std::endl;
  // for (int i = 0; i < save_input_names.size(); ++i) {
  //   std::cout << i << " " << save_input_names.at(i) << std::endl;
  // }

  // std::cout << "output_names of save_runner" << std::endl;
  // for (int i = 0; i < save_output_names.size(); ++i) {
  //   std::cout << i << " " << save_output_names.at(i) << std::endl;
  // }

  // TfLiteTensor* save_runner_input = save_runner->input_tensor("checkpoint_path");
  // char save_path[100] = "save_runner.ckpt";
  // save_runner_input->data.raw = save_path;

  // TFLITE_MINIMAL_CHECK(save_runner->AllocateTensors() == kTfLiteOk);
  // std::cout << "sfasdf" << std::endl;
  // TFLITE_MINIMAL_CHECK(save_runner->Invoke() == kTfLiteOk);
  // std::cout << "sfasdf" << std::endl;

  // tflite::SignatureRunner* restore_runner =
  //   interpreter->GetSignatureRunner("restore");
  // printf("restore_runner: %p\n", restore_runner);

  // const std::vector<const char*>& restore_input_names = restore_runner->input_names();
  // const std::vector<const char*>& restore_output_names = restore_runner->output_names();

  // std::cout << "input_names of restore_runner" << std::endl;
  // for (int i = 0; i < restore_input_names.size(); ++i) {
  //   std::cout << i << " " << restore_input_names.at(i) << std::endl;
  // }

  // std::cout << "output_names of restore_runner" << std::endl;
  // for (int i = 0; i < restore_output_names.size(); ++i) {
  //   std::cout << i << " " << restore_output_names.at(i) << std::endl;
  // }

  // TFLITE_MINIMAL_CHECK(restore_runner->AllocateTensors() == kTfLiteOk);
  // TFLITE_MINIMAL_CHECK(restore_runner->Invoke() == kTfLiteOk);

  return 0;
}
