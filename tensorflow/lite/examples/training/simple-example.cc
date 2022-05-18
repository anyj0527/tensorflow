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

#include <cstdio>
#include <string>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <getopt.h>
#include <ctime>

#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/interpreter_builder.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/signature_runner.h"


#define BATCH_SIZE 32

inline bool is_little_indian() {
	int val = 1;
	return *reinterpret_cast<char *>(&val) != 0;
}

uint32_t reverse_indian(uint32_t val) {
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

int main(int argc, char* argv[]) {

	std::string command = argv[1];
	const char* model_filename = argv[2];

	//Loading the tflite retrainable model
	std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(model_filename);
	if (model == nullptr) {
		fprintf(stderr, "FlatBufferModel could not be created from given model argument.");
		return 1;
	}
	tflite::ops::builtin::BuiltinOpResolver resolver;
	tflite::InterpreterBuilder builder(*model, resolver);

	//Building the interpreter
	std::unique_ptr<tflite::Interpreter> interpreter;
	builder(&interpreter);

	//Getting the list of signatures
	auto signature_defs = interpreter->signature_keys();

    const std::string image_filename = argv[3];
    const std::string label_filename = argv[4];
    const int num_epochs = atoi(argv[5]);

    tflite::SignatureRunner* train_runner = interpreter->GetSignatureRunner("train");
    if (train_runner == nullptr) {
        fprintf(stderr, "Couldn't create train runner from the signature of the interpreter.");
    return 1;
    }

    //Getting the input and output tensors names
    const std::vector<const char*>& input_names = train_runner->input_names();
    const std::vector<const char*>& output_names = train_runner->output_names();

    TfLiteTensor* train_input_x = train_runner->input_tensor((std::string(input_names[0])).c_str());
    TfLiteTensor* train_input_y = train_runner->input_tensor((std::string(input_names[1])).c_str());

    //Allocating tensors
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        fprintf(stderr, "Interpreter couldn't allocate tensors.");
        return 1;
    }

    if (train_runner->AllocateTensors() != kTfLiteOk) {
        fprintf(stderr, "Training runner couldn't allocate tensors.");
        return 1;
    }

    std::cout << "lkahsdkflhalskdfhlkasdhf;klhadsfhlas;dfhklashdflashflhasfdklh" << std::endl;
    // Parsing dataset
    std::ifstream imagefile(image_filename, std::ios::in | std::ios::binary);
    std::ifstream labelfile(label_filename, std::ios::in | std::ios::binary);

    uint32_t magic_number;
    uint32_t magic_number_label;
    uint32_t num_images;
    uint32_t num_labels;
    uint32_t num_rows;
    uint32_t num_cols;

    imagefile.read(reinterpret_cast<char *>(&magic_number), 4);
    imagefile.read(reinterpret_cast<char *>(&num_images), 4);
    imagefile.read(reinterpret_cast<char *>(&num_rows), 4);
    imagefile.read(reinterpret_cast<char *>(&num_cols), 4);
    labelfile.read(reinterpret_cast<char *>(&magic_number_label), 4);
    labelfile.read(reinterpret_cast<char *>(&num_labels), 4);

    std::cout << "num_image: " << num_images << std::endl;
    std::cout << "num_label: " << num_labels << std::endl;
    std::cout << "num_row: " << num_rows << std::endl;
    std::cout << "num_col: " << num_cols << std::endl;
    //The first 4 bytes in dataset file are dedicated to the magic number
    // MNIST Dataset is in Big-Endian format
    if (is_little_indian()){
        magic_number = reverse_indian(magic_number);
        num_images = reverse_indian(num_images);
        num_labels = reverse_indian(num_labels);
        num_rows = reverse_indian(num_rows);
        num_cols = reverse_indian(num_cols);
    }

    std::cout << "num_image: " << num_images << std::endl;
    std::cout << "num_label: " << num_labels << std::endl;
    std::cout << "num_row: " << num_rows << std::endl;
    std::cout << "num_col: " << num_cols << std::endl;

    char label;
    const uint32_t num_pixels = num_rows * num_cols;
    const uint32_t num_batches = num_images / BATCH_SIZE;
    std::cout << "[DEBUG][METADATA] Number of batches: " << num_batches << std::endl;
    std::cout << "[DEBUG][TRAINING] Training launched ... " << std::endl;
    for (int epoch = 0; epoch < num_epochs; ++epoch){
        float current_loss = 0;
        std::cout << "[DEBUG][TRAINING] Current epoch: " << epoch << std::endl;
        for (uint32_t batch_index = 0; batch_index < num_batches; ++batch_index){
            char *pixels{ new char[num_pixels * sizeof(float)]{}};
            float *fpixels{ new float[num_pixels]{}};
            for (int index = 0; index < BATCH_SIZE; ++index){
                float labels[10] = { 0.0 };
                imagefile.read(pixels, num_pixels);
                labelfile.read(&label, 1);
                // if (index == 0) {
                //     std::cout << "label: " << (int) label << " " << (int) reverse_indian(label) << std::endl;
                // }
                // label = reverse_indian(label);
                labels[(int) label] = 1.0;
                for (uint32_t i=0; i<num_pixels; ++i){
                    fpixels[i] = float(float(*(pixels + i))/255.0f);
                }
                train_input_y->data.f = labels;
                train_input_x->data.f = fpixels;
                if (train_runner->Invoke() != kTfLiteOk){
                    fprintf(stderr, "ERROR: Training runner couldn't run Invoke of subgraph.\n");
                    return 1;
                }
                if (batch_index == 0) {
                    const TfLiteTensor* output_tensor = train_runner->output_tensor((std::string(output_names[0])).c_str());
                    float* output = output_tensor->data.f;
                    current_loss = *(output);
                    std::cout << index << " loss: " << current_loss << std::endl;
                }
            }
            delete[] pixels;
            delete[] fpixels;
            const TfLiteTensor* output_tensor = train_runner->output_tensor((std::string(output_names[0])).c_str());
            float* output = output_tensor->data.f;
            current_loss = *(output);
        }
        std::cout << "] - Current Loss: " << current_loss << std::endl;
    }
    std::cout << "[DEBUG][TRAINING] Training finished successfully." << std::endl;

	return 0;
}

