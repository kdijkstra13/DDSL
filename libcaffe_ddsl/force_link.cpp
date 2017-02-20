#include "caffe/caffe.hpp"

#include "caffe/layers/absval_layer.hpp"
#include "caffe/layers/accuracy_layer.hpp"
#include "caffe/layers/argmax_layer.hpp"
#include "caffe/layers/batch_norm_layer.hpp"
#include "caffe/layers/batch_reindex_layer.hpp"
#include "caffe/layers/bias_layer.hpp"
#include "caffe/layers/bnll_layer.hpp"
#include "caffe/layers/concat_layer.hpp"
#include "caffe/layers/contrastive_loss_layer.hpp"
#include "caffe/layers/crop_layer.hpp"
#include "caffe/layers/data_layer.hpp"
#include "caffe/layers/deconv_layer.hpp"
#include "caffe/layers/conv_layer.hpp"
#include "caffe/layers/dropout_layer.hpp"
#include "caffe/layers/dummy_data_layer.hpp"
#include "caffe/layers/eltwise_layer.hpp"
#include "caffe/layers/elu_layer.hpp"
#include "caffe/layers/embed_layer.hpp"
#include "caffe/layers/euclidean_loss_layer.hpp"
#include "caffe/layers/exp_layer.hpp"
#include "caffe/layers/filter_layer.hpp"
#include "caffe/layers/flatten_layer.hpp"
#include "caffe/layers/hdf5_data_layer.hpp"
#include "caffe/layers/hdf5_output_layer.hpp"
#include "caffe/layers/hinge_loss_layer.hpp"
#include "caffe/layers/im2col_layer.hpp"
#include "caffe/layers/image_data_layer.hpp"
#include "caffe/layers/infogain_loss_layer.hpp"
#include "caffe/layers/inner_product_layer.hpp"
#include "caffe/layers/input_layer.hpp"
#include "caffe/layers/log_layer.hpp"
#include "caffe/layers/memory_data_layer.hpp"
#include "caffe/layers/multinomial_logistic_loss_layer.hpp"
#include "caffe/layers/mvn_layer.hpp"
#include "caffe/layers/parameter_layer.hpp"
#include "caffe/layers/power_layer.hpp"
#include "caffe/layers/prelu_layer.hpp"
#include "caffe/layers/reduction_layer.hpp"
#include "caffe/layers/reshape_layer.hpp"
#include "caffe/layers/scale_layer.hpp"
#include "caffe/layers/sigmoid_cross_entropy_loss_layer.hpp"
#include "caffe/layers/sigmoid_layer.hpp"
#include "caffe/layers/silence_layer.hpp"
#include "caffe/layers/slice_layer.hpp"
#include "caffe/layers/softmax_layer.hpp"
#include "caffe/layers/softmax_loss_layer.hpp"
#include "caffe/layers/split_layer.hpp"
#include "caffe/layers/pooling_layer.hpp"
#include "caffe/layers/spp_layer.hpp"
#include "caffe/layers/threshold_layer.hpp"
#include "caffe/layers/tile_layer.hpp"
#include "caffe/layers/window_data_layer.hpp"
#include "caffe/layers/relu_layer.hpp"
#include "caffe/layers/tanh_layer.hpp"

#include "caffe/sgd_solvers.hpp"

namespace caffe
{
	REGISTER_LAYER_CLASS(AbsVal);
	REGISTER_LAYER_CLASS(Accuracy);
	REGISTER_LAYER_CLASS(ArgMax);
	REGISTER_LAYER_CLASS(BatchNorm);
	REGISTER_LAYER_CLASS(Bias);
	REGISTER_LAYER_CLASS(BatchReindex);
	REGISTER_LAYER_CLASS(BNLL);
	REGISTER_LAYER_CLASS(Concat);
	REGISTER_LAYER_CLASS(ContrastiveLoss);
	REGISTER_LAYER_CLASS(Crop);
	REGISTER_LAYER_CLASS(Data);
	REGISTER_LAYER_CLASS(Convolution);
	REGISTER_LAYER_CLASS(Deconvolution);
	REGISTER_LAYER_CLASS(Dropout);
	REGISTER_LAYER_CLASS(DummyData);
	REGISTER_LAYER_CLASS(Eltwise);
	REGISTER_LAYER_CLASS(ELU);
	REGISTER_LAYER_CLASS(Embed);
	REGISTER_LAYER_CLASS(EuclideanLoss);
	REGISTER_LAYER_CLASS(Exp);
	REGISTER_LAYER_CLASS(Filter);
	REGISTER_LAYER_CLASS(HDF5Data);
	REGISTER_LAYER_CLASS(HDF5Output);
	REGISTER_LAYER_CLASS(HingeLoss);
	REGISTER_LAYER_CLASS(Im2col);
	REGISTER_LAYER_CLASS(ImageData);
	REGISTER_LAYER_CLASS(InfogainLoss);
	REGISTER_LAYER_CLASS(InnerProduct);
	REGISTER_LAYER_CLASS(Input);
	REGISTER_LAYER_CLASS(Log);
	REGISTER_LAYER_CLASS(Flatten);
	REGISTER_LAYER_CLASS(MemoryData);
	REGISTER_LAYER_CLASS(MultinomialLogisticLoss);
	REGISTER_LAYER_CLASS(MVN);
	REGISTER_LAYER_CLASS(Parameter);
	REGISTER_LAYER_CLASS(Power);
	REGISTER_LAYER_CLASS(PReLU);
	REGISTER_LAYER_CLASS(Reduction);
	REGISTER_LAYER_CLASS(Reshape);
	REGISTER_LAYER_CLASS(Scale);
	REGISTER_LAYER_CLASS(SigmoidCrossEntropyLoss);
	REGISTER_LAYER_CLASS(Sigmoid);
	REGISTER_LAYER_CLASS(Silence);
	REGISTER_LAYER_CLASS(Slice);
	REGISTER_LAYER_CLASS(Softmax);
	REGISTER_LAYER_CLASS(SoftmaxWithLoss);
	REGISTER_LAYER_CLASS(Split);
	REGISTER_LAYER_CLASS(SPP);
	REGISTER_LAYER_CLASS(Threshold);
	REGISTER_LAYER_CLASS(Tile);
	REGISTER_LAYER_CLASS(ReLU);
	REGISTER_LAYER_CLASS(TanH);
	REGISTER_LAYER_CLASS(Pooling);
	REGISTER_LAYER_CLASS(WindowData);
	REGISTER_SOLVER_CLASS(AdaDelta);
	REGISTER_SOLVER_CLASS(AdaGrad);
	REGISTER_SOLVER_CLASS(Adam);
	REGISTER_SOLVER_CLASS(Nesterov);
	REGISTER_SOLVER_CLASS(RMSProp);
	REGISTER_SOLVER_CLASS(SGD);
}

