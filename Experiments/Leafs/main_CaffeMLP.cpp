#include "stdafx.h"
#include <direct.h>

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;

enum Day {Day1, Day2};
enum Side {Front, Back};
enum Defect {Alternaria, Ozone};

void selectSet(Table<> &trainset, Table<> &evalset, const String &folder, Day day, Side side, UInt32 r, UInt32 eval) {
	Table<> data, select;
	
	//Prepare set
	data << SS(folder << "\\_r" << r << ".ddsl");
	select = data((data["Day"] == "Day1" &&  data["Side"] == "Back"));
	cout << "Sample count: " << select.rows.count() << endl;
	Matrix<MatrixIdx> training;
	Matrix<MatrixIdx> evaluation = select["Sample"] == SS(eval);
	for (UInt32 s=0;s<5;s++) {
		//Leave one out
		if (s != eval) 
			training ^ (select["Sample"] == SS(s));
	}
	trainset = select(training);
	evalset = select(evaluation);
}

void select_LDA_PCA_Set(Table<> &trainset, Table<> &evalset, const String &folder, Day day, Side side, UInt32 r, UInt32 eval) {
	Table<> data, select;
	
	//Prepare set
	data << SS(folder << "\\_lda2_pca3_r" << r << ".ddsl");
	select = data((data["Day"] == "Day1" &&  data["Side"] == "Back"));
	cout << "Sample count: " << select.rows.count() << endl;
	Matrix<MatrixIdx> training;
	Matrix<MatrixIdx> evaluation = select["Sample"] == SS(eval);
	for (UInt32 s=0;s<5;s++) {
		//Leave one out
		if (s != eval) 
			training ^ (select["Sample"] == SS(s));
	}
	trainset = select(training);
	evalset = select(evaluation);
}

template <typename T, typename TIdx>
void divConfusion(Matrix<T, TIdx> &in, Matrix<Double> &out, TIdx digits = 1, Double mult = 100) {
	if (in.rows.count() != in.cols.count())
		throw Error(ecIncompatible, "divConfusion", "Not a confusion matrix");
	convert(out, in);
	for (TIdx c=0;c<out.cols.count();c++) {		
		DSFunc::round((out[c] / sum(out[c])) * mult, digits);
	}
}

template <typename T, typename TIdx>
void evalConfusion(Matrix<T, TIdx> &confTrain, Matrix<T, TIdx> &confEval) {
	Matrix<Double> confTrainDbl, confEvalDbl, conf12TrainDbl, conf12EvalDbl, conf23TrainDbl, conf23EvalDbl, confTrainLDDbl, confEvalLDDbl;

	Matrix<T, TIdx> confTrainLeafDefect(2,2);
	confTrainLeafDefect.val(0,0) = confTrain.val(0,0);
	confTrainLeafDefect.val(1,0) = sum(confTrain(1,2,0,1));
	confTrainLeafDefect.val(0,1) = sum(confTrain(0,1,1,2));
	confTrainLeafDefect.val(1,1) = sum(confTrain(1,2,1,2));

	Matrix<T, TIdx> confEvalLeafDefect(2,2);
	confEvalLeafDefect.val(0,0) = confEval.val(0,0);
	confEvalLeafDefect.val(1,0) = sum(confEval(1,2,0,1));
	confEvalLeafDefect.val(0,1) = sum(confEval(0,1,1,2));
	confEvalLeafDefect.val(1,1) = sum(confEval(1,2,1,2));

	divConfusion(confTrain, confTrainDbl, 1U, 100U);
	divConfusion(confEval, confEvalDbl, 1U, 100U);
	divConfusion(confTrainLeafDefect, confTrainLDDbl, 1U, 100U);
	divConfusion(confEvalLeafDefect, confEvalLDDbl, 1U, 100U);
	divConfusion(confTrain(1,2,1,2), conf23TrainDbl, 1U, 100U);
	divConfusion(confEval(1,2,1,2), conf23EvalDbl, 1U, 100U);

	cout << "Training (samples): " << endl;	
	confTrain++;
	cout << "Training (percentage class Leaf/Alternaria/Ozone): " << endl;
	confTrainDbl++;
	cout << "Training (percentage class Leaf/Desease): " << endl;
	confTrainLDDbl++;
	cout << "Training (percentage class Alternaria/Ozone): " << endl;
	conf23TrainDbl++;

	cout << "Evaluation (samples): " << endl;	
	confEval++;
	cout << "Evaluation (percentage class Leaf/Alternaria/Ozone): " << endl;
	confEvalDbl++;
	cout << "Evaluation (percentage class Leaf/Desease): " << endl;
	confEvalLDDbl++;
	cout << "Evaluation (percentage class Alternaria/Ozone): " << endl;
	conf23EvalDbl++;
}

void makeSummary(const String &model, Table<> &dst, UInt32 evalSample, UInt32 r, Table<> &trainset, Table<> &evalset, Matrix<UInt32> &confTrain, Matrix<UInt32> &confEval, Double timeTrain, Double timeEval) {
	
	Matrix<Double> confTrainDbl;
	Matrix<Double> confEvalDbl;
	convert(confTrainDbl, confTrain);
	convert(confEvalDbl, confEval);
	Matrix<Double> confVector = *confTrainDbl[0] ^ *confTrainDbl[1] ^ *confTrainDbl[2] ^ *confEvalDbl[0] ^ *confEvalDbl[1] ^ *confEvalDbl[2];
	confVector += confVector;
	
	evalConfusion(confTrain, confEval);

	Matrix<String> colNames = (dtString | "Model" | "EvalSample" | "r" | "TrainSamples" | "EvalSamples" | "TimeTrain" | "TimeEval" | "TrainConf_0_0" | "TrainConf_1_0" | "TrainConf_2_0" | "TrainConf_0_1" | "TrainConf_1_1" | "TrainConf_2_1" | "TrainConf_0_2" | "TrainConf_1_2" | "TrainConf_2_2" | "EvalConf_0_0" | "EvalConf_1_0" | "EvalConf_2_0" | "EvalConf_0_1" | "EvalConf_1_1" | "EvalConf_2_1" | "EvalConf_0_2" | "EvalConf_1_2" | "EvalConf_2_2");
	dst ^	(colNames ^ (
				(ctParameter | (dtString | model)) |
				(ctParameter | (dtUInt32 | evalSample)) | 
				(ctParameter | (dtUInt32 | r)) | 
				(ctParameter | (dtUInt32 | trainset.rows.count())) | 
				(ctParameter | (dtUInt32 | evalset.rows.count())) |
				(ctParameter | (dtDouble | timeTrain)) |
				(ctParameter | (dtDouble | timeEval)) |
				(ctConfusion | confVector)
			));
}

void testMLPa(Table<> &trainset, Table<> &evalset, Matrix<UInt32> &confTrain, Matrix<UInt32> &confEval, Double &timeTrain, Double &timeEval, UInt32 type, const String &filename="") {
	cout << "MLPa: " << endl;
	Matrix<UInt32> classes = (dtUInt32|1U|2U|3U);	
	ZScore<> mlp;
	UInt32 epochs = 100000;

	//best
	if (type == 1) {
		mlp = ZScore<>() | +CaffeMLP<>(classes, caffe::Caffe::GPU, 0.001, 0.01, (dtUInt32|4096U), epochs, 200, lrpFixed, 80000, 0.1, 1, 0.01, utReLU, stSGD, Matrix<UInt32>()) | -Confusion<>(classes);	
	} else if (type == 2) {
		mlp = ZScore<>() | +CaffeMLP<>(classes, caffe::Caffe::GPU, 0.001, 0.01, (dtUInt32|8U|8U|8U|8U), epochs, 200, lrpFixed, 80000, 0.1, 1, 0.01, utReLU, stSGD, Matrix<UInt32>()) | -Confusion<>(classes);	
	} else if (type == 3) {
		mlp = ZScore<>() | +CaffeMLP<>(classes, caffe::Caffe::GPU, 0.001, 0.01, (dtUInt32|32U|16U|4U|2U), epochs, 200, lrpFixed, 80000, 0.1, 1, 0.01, utReLU, stSGD, Matrix<UInt32>()) | -Confusion<>(classes);	
	} else if (type == 4) {
		mlp = ZScore<>() | +CaffeMLP<>(classes, caffe::Caffe::GPU, 0.001, 0.01, (dtUInt32|4096U), epochs, 200, lrpFixed, 80000, 0.1, 1, 0, utTanH, stSGD, Matrix<UInt32>()) | -Confusion<>(classes);	
	} else if (type == 5) {
		mlp = ZScore<>() | +CaffeMLP<>(classes, caffe::Caffe::GPU, 0.001, 0.01, (dtUInt32|8U|8U|8U|8U), epochs, 200, lrpFixed, 80000, 0.1, 1, 0, utTanH, stSGD, Matrix<UInt32>()) | -Confusion<>(classes);	
	} else if (type == 6) {
		mlp = ZScore<>() | +CaffeMLP<>(classes, caffe::Caffe::GPU, 0.001, 0.01, (dtUInt32|32U|16U|4U|2U), epochs, 200, lrpFixed, 80000, 0.1, 1, 0, utTanH, stSGD, Matrix<UInt32>()) | -Confusion<>(classes);	
	}
	
	confTrain.clear();
	confEval.clear();

	confTrain = trainset > !mlp;
	mlp.progressLoop();
	confEval = evalset >> !mlp;
	mlp.progressLoop();
	timeTrain = mlp.timeTrain();
	timeEval = mlp.timeApply();
	if (filename != "")
		mlp.write(filename);
}

Table<> selectPCs(Table<> &in, UInt32 sample) {
	return (in[dtString | SS("PC1_" << sample)  | SS("PC2_" << sample) | SS("PC3_" << sample) | "Target" | "x" | "y" | "Day" | "Side" | "Defect" | "Sample"]);
}

Table<> selectLDs(Table<> &in, UInt32 sample) {
	return (in[dtString | SS("LD1_" << sample)  | SS("LD2_" << sample) | "Target" | "x" | "y" | "Day" | "Side" | "Defect" | "Sample"]);
}

Table<> selectFreq(Table<> &in, UInt32 r) {
	return (in[dtString | SS("F_520_" << r)  | SS("F_540_" << r) | SS("F_680_" << r) | "Target" | "x" | "y" | "Day" | "Side" | "Defect" | "Sample"]);
}

void Experiment22_09_2016() {
	String folder = "D:\\DATASETS\\Leafs";
	UInt32 samples = 2000;
	UInt32 waves = 28;
	bool exp = true;
	//makeDataSet(folder, samples, waves);
	//mergeSets(folder, samples*2, exp);
	Table<> summary, summarylda, summarypca, summary3band;
	String filename;
	//UInt32 r=0;
	//UInt32 evalSample=0;
	//UInt32 type=1;
	for (UInt32 r=0;r<3;r++) {
		for (UInt32 evalSample=0;evalSample<5;evalSample++) {
			//All tests
			Table<> trainset, evalset;
			Matrix<UInt32> confTrain, confEval;
			Double timeTrain, timeEval;
			selectSet(trainset, evalset, folder, Day1, Back, r, evalSample);

			for (UInt32 type=1;type<7;type++) {
				//Test classifiers
				filename = SS(folder << "\\Classifiers\\" << "MLP\\All_r" << r << "_s" << evalSample << "_t" << type << ".ddsl");
				testMLPa(trainset, evalset, confTrain, confEval, timeTrain, timeEval, type, filename);
				makeSummary(SS("MLP" << type), summary, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);	
				summary > Export<>(folder + "\\Summary_MLP_All.txt", false);
				summary >> SS(folder << "\\Summary_MLP_All.ddsl");

				//3Band test
				selectSet(trainset, evalset, folder, Day1, Back, r, evalSample);
				trainset = selectFreq(trainset, r);
				evalset = selectFreq(evalset, r);
				filename = SS(folder << "\\Classifiers\\" << "MLP\\3Band_r" << r << "_s" << evalSample << "_t" << type << ".ddsl");
				testMLPa(trainset, evalset, confTrain, confEval, timeTrain, timeEval, type, filename);		
				makeSummary(SS("MLP" << type), summary3band, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);
				summary3band > Export<>(folder + "\\Summary_MLP_3Band.txt", false);
				summary3band >> SS(folder << "\\Summary_MLP_3Band.ddsl");

				//LDA tests
				select_LDA_PCA_Set(trainset, evalset, folder, Day1, Back, r, evalSample);
				trainset = selectLDs(trainset, evalSample);
				evalset = selectLDs(evalset, evalSample);
				filename = SS(folder << "\\Classifiers\\" << "MLP\\LDA_r" << r << "_s" << evalSample << "_t" << type << ".ddsl");
				testMLPa(trainset, evalset, confTrain, confEval, timeTrain, timeEval, type, filename);
				makeSummary(SS("MLP" << type), summarylda, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);				
				summarylda > Export<>(folder + "\\Summary_MLP_LDA.txt", false);
				summarylda >> SS(folder << "\\Summary_MLP_LDA.ddsl");

				//PCA tests
				select_LDA_PCA_Set(trainset, evalset, folder, Day1, Back, r, evalSample);
				trainset = selectPCs(trainset, evalSample);
				evalset = selectPCs(evalset, evalSample);				
				filename = SS(folder << "\\Classifiers\\" << "MLP\\PCA_r" << r << "_s" << evalSample << "_t" << type << ".ddsl");
				testMLPa(trainset, evalset, confTrain, confEval, timeTrain, timeEval, type, filename);
				makeSummary(SS("MLP" << type), summarypca, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);
				summarypca > Export<>(folder + "\\Summary_MLP_PCA.txt", false);
				summarypca >> SS(folder << "\\Summary_MLP_PCA.ddsl");
			}
		}
	}
}

void initGlog() {
	//Start logging for caffe
	google::InitGoogleLogging("DDSL");
	google::SetLogDestination(google::GLOG_INFO, "glog/log");
}

int main(int argc, char **) {
	try {
		initGlog();
		Experiment22_09_2016();
	} catch (Error &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}
