//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "ddsl.hpp"
//#include <direct.h>

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;
using namespace DSImage;
using namespace DSModel;
/*
void initGlog() {
	//Start logging for caffe
	cout << "Init google log" << endl;
	google::InitGoogleLogging("DDSL");
	google::SetLogDestination(google::GLOG_INFO, "/tmp/DDSL");
}

void initCaffe(UInt32 ngpus) {
	cout << "Init caffe" << endl;
	caffe::Caffe::set_mode(caffe::Caffe::GPU);
	caffe::Caffe::set_solver_count(ngpus);
}
*/
int main(void) {
	try {
		/*
		initGlog();
		UInt32 ngpus = 4;
		initCaffe(ngpus);
		cout << "Start" << endl;
		#ifdef _MSC_VER
			CHDIR("d:\\DATASETS\\PlastcisSWIR2");
		#else
			CHDIR("/media/public_data/PlastcisSWIR2");
		#endif
		cout << "Load truth image" << endl;
		//Load ground truth
		Matrix<ImagePNG<Float>> truth = (dtImagePNGFloat | ImagePNG<Float>("GT_RGB.png", true).getChannel(0));

		UInt32 w = truth.val(0, 0).cols.count();
		UInt32 h = truth.val(0, 0).rows.count();
		ImagePNG<Float> f1150("1150_1.png", true);
		ImagePNG<Float> f1200("1200_1.png", true);
		ImagePNG<Float> f1400("1400_1.png", true);
		ImagePNG<Float> f1600("1600_1.png", true);
		Matrix<ImagePNG<Float>> images = Matrix<ImagePNG<Float>>() | f1150 | f1200 | f1400 | f1600;

		//Generate patches
		Matrix<ImagePNG<Float>> trainPatches, evalPatches;
		Matrix<Float> trainTargets, evalTargets;
		Matrix<UInt32> trainCoords, evalCoords;
		UInt32 trainSampleCount = 384 * 6;
		UInt32 evalSampleCount = 192 * 6;
		UInt32 patchWidth = 28U;
		UInt32 patchHeight = 28U;
		matricesToUniformPatches(images, truth, trainPatches, trainTargets, trainCoords, trainSampleCount, patchHeight, patchWidth);
		matricesToUniformPatches(images, truth, evalPatches, evalTargets, evalCoords, evalSampleCount, patchHeight, patchWidth);

		//Replace targets
		replace(trainTargets.rows->begin(), trainTargets.rows->end(), 10.0f, 0.0f);
		replace(trainTargets.rows->begin(), trainTargets.rows->end(), 20.0f, 1.0f);
		replace(trainTargets.rows->begin(), trainTargets.rows->end(), 30.0f, 2.0f);
		replace(evalTargets.rows->begin(), evalTargets.rows->end(), 10.0f, 0.0f);
		replace(evalTargets.rows->begin(), evalTargets.rows->end(), 20.0f, 1.0f);
		replace(evalTargets.rows->begin(), evalTargets.rows->end(), 30.0f, 2.0f);

		//Create table
		Table<> trainTab = (ctFeature | trainPatches) | (ctTarget | trainTargets) | (ctSplit | (Matrix<UInt32>(trainSampleCount, 1U, 0U)));
		Table<> evalTab = (ctFeature | evalPatches) | (ctTarget | evalTargets) | (ctSplit | (Matrix<UInt32>(evalSampleCount, 1U, 1U)));
		//Randomize patches
		trainTab = trainTab(shuffle(trainTab.rows.idxs(), 3.0f));
		evalTab = evalTab(shuffle(evalTab.rows.idxs(), 3.0f));
		Table<> tab = trainTab ^ evalTab;

		Matrix<ImagePNG<Float>> feat = tab[ctFeature];

		//Save patches to DDSL file
		cout << "Output DDSL file" << endl;
		tab >> "out/plastics.ddsl";

		//Save LMDB using a model
		//DSModel::ExportDB<> pt = DSModel::ExportDB<>("out/train", beLMDB);
		//tab(tab[ctSplit] == 0U) > !pt;
		//pt.progressLoop();

		//pt[0].setParameter<String>("Filename", "out/eval");
		//tab(tab[ctSplit] == 1U) >> !pt;
		//pt.progressLoop();

		//Matrix<ImagePNG<Float>> sourceTrainImages, sourceEvalImages;
		//Matrix<Float> sourceTrainTargets, sourceEvalTargets;
		//readImageDB(sourceTrainImages, sourceTrainTargets, "out/train");
		//readImageDB(sourceEvalImages, sourceEvalTargets, "out/eval");

		//Train Caffe
		#ifdef _MSC_VER
			CHDIR("c:\\Projects\\DDSL_Apps\\Plastics\\Models");
		#else
			CHDIR("/home/klaas/Projects/DDSL_Apps/Plastics/Models");
		#endif
		cout << "Make CAFFE" << endl;
		Matrix<Float> classes = (dtFloat|0.0f|1.0f|2.0f);
		String model = "LeNet_28x28_Model.prototxt";
		String solver = "LeNet_28x28_Solver.prototxt";
		
		DSModel::Caffe<Float> p = +DSModel::Caffe<Float>(classes, model, solver, ngpus) | -DSModel::Confusion<Float>(classes);

		cout << "Train CAFFE" << endl;
		Table<> trainScore = tab(tab[ctSplit] == 0U) > !p;
		p.progressLoop();
		confusionFrac(trainScore)++;

		cout << "Eval CAFFE" << endl;
		Table<> evalScore = tab(tab[ctSplit] == 1U) >> !p;
		p.progressLoop();
		confusionFrac(evalScore)++;

		//Draw results
		DSModel::Caffe<Float> production = p(1); //copy first models to create a production pipeline

		//Convert hyperspectral image pixels to samples	
		Matrix<ImagePNG<Float>> features;
		Matrix<UInt32> coords;

		//matricesToUniformPatches(images, features, coords, 30000U, patchWidth, patchHeight);
		matricesToPatches(images, features, coords, patchHeight, patchWidth);

		//Classify part of the image as samples
		Table<> tab2 = (ctFeature | features);
		Table<> out = tab2 >> !production;
		production.progressLoop();
		cout << "Classified image in " << production.timeAllApply() << "ms";

		#ifdef _MSC_VER
			CHDIR("d:\\DATASETS\\PlastcisSWIR2");
		#else
			CHDIR("/media/public_data/PlastcisSWIR2");
		#endif
		//Put samples back in the image and save image
		ImagePNG<Float> pngOut = images.vec(0);
		samplesToMatrix((out(ctResult).mat<Float>(0) + 1.0f) * 80.0f, coords, pngOut, w, h);
		pngOut.saveImage("out/ResultImageCNN.png", itM8);

		p.clearAll();
		production.clearAll();
		images.clearAll();
		features.clearAll();
*/
		ImagePNG<Float> png("C:\\Projects\\DSL\\Experiments\\Test.png", true);
		png(0,10,0,10)++;

		DSModel::ArrayFire::PCA<>("test");
	} catch (const char * e) {
		cout << e;
	} catch (const Error &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}
