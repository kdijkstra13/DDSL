#include "ddsl.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;
using namespace DSImage;

enum Day {Day1, Day2};
enum Side {Front, Back};
enum Defect {Alternaria, Ozone};

template <typename T, typename TIdx>
void readSample(Matrix<T, TIdx> &wave, const String folder, Day day, Side side, Defect defect, unsigned int sample, unsigned int wavelength) {
	String waveFile = SS(folder << ((day==Day1)?"\\Day1\\":"\\Day2\\") << ((side==Front)?"Front\\":"Back\\") << ((defect==Alternaria)?"Alternaria\\":"Ozone\\") << sample << "_" << wavelength << ".png");
	readPNG(wave, waveFile);	
};

template <typename T, typename TIdx>
void readRef(Matrix<T, TIdx> &ref, const String folder, Day day, Side side, Defect defect, unsigned int sample) {
	String refFile = SS(folder << ((day==Day1)?"\\Day1\\":"\\Day2\\") << ((side==Front)?"Front\\":"Back\\") << "Reference\\" << ((defect==Alternaria)?"Alternaria_":"Ozone_") << sample << ".png");
	readPNG(ref, refFile);
}

template <typename T, typename TIdx>
void readBack(Matrix<T, TIdx> &back, const String folder, unsigned int wavelength) {
	String backFile = SS(folder << "\\Background\\" << wavelength << ".png");
	readPNG(back, backFile);
}

template <typename T, typename TIdx>
void makeWindowSamples(Matrix<T, TIdx> &image, Matrix<T, TIdx> &result, const TIdx r, const TIdx border) {
	TIdx h = image.rows.count();
	TIdx w = image.cols.count();
	int b = (border==0)?r:border;
	TIdx samples = (w - 2*b) * (h - 2*b);
	int rr = r;
	result.resize(samples, ((2*rr)+1) * ((2*rr)+1));
	TIdx row=0;
	TIdx col=0;	
	for (TIdx y=b;y<h-b;y++) {
		for (TIdx x=b;x<w-b;x++) {
			for (int wy=-rr;wy<=rr;wy++) {
				for (int wx=-rr;wx<=rr;wx++) {
					int xx = (int)x + (int)wx;
					int yy = (int)y + (int)wy;
					result.val(row, col) = image.val(yy, xx);					
					col++;
				}
			}
			row++;
			col=0;
		}
	}
}

template <typename T, typename TIdx>
void makeWindowCoords(Matrix<T, TIdx> &image, Matrix<UInt32, TIdx> &coords, const TIdx border) {
	TIdx h = image.rows.count();
	TIdx w = image.cols.count();
	TIdx samples = (w - 2*border) * (h - 2*border);
	coords.resize(samples, 2);
	TIdx row=0;
	for (TIdx y=border;y<h-border;y++) {
		for (TIdx x=border;x<w-border;x++) {
			coords.val(row, 0) = (UInt32)x;
			coords.val(row, 1) = (UInt32)y;
			row++;
		}
	}
}

template <typename T, typename TIdx>
void makeWindowRef(Matrix<T, TIdx> &image, Matrix<UInt32, TIdx> &result, const TIdx border) {
	TIdx h = image.rows.count();
	TIdx w = image.cols.count();
	TIdx samples = (w - 2*border) * (h - 2*border);
	result.resize(samples, 1);
	Matrix<T, TIdx> ref(samples, 1);
	TIdx i=0;
	for (TIdx y=border;y<h-border;y++) {
		for (TIdx x=border;x<w-border;x++) {
			ref.val(i,0) = image.val(y,x);
			i++;
		}
	}
	convert(result, ref);
}

void saveTable(Table<> &wave, const String folder, Day day, Side side, Defect defect, unsigned int sample, unsigned int r) {
	 String infix = SS(((day==Day1)?"_Day1":"_Day2") << ((side==Front)?"_Front":"_Back") << ((defect==Alternaria)?"_A":"_B") << "_s" << sample << "_r" << r << "_n" << wave.rows.count()/1000 << "K");
	 wave >> SS(folder << "\\Table" << infix << ".ddsl");	 
}

void readTable(Table<> &wave, const String folder, Day day, Side side, Defect defect, unsigned int sample, unsigned int r, unsigned int subsample) {
	 String infix = SS(((day==Day1)?"_Day1":"_Day2") << ((side==Front)?"_Front":"_Back") << ((defect==Alternaria)?"_A":"_B") << "_s" << sample << "_r" << r << "_n" << subsample/1000 << "K");
	 wave << SS(folder << "\\Table" << infix << ".ddsl");	 
}

void exportTable(Table<> &wave, const String folder, Day day, Side side, Defect defect, unsigned int sample, unsigned int r, unsigned int subsample) {
	String infix = SS(((day==Day1)?"_Day1":"_Day2") << ((side==Front)?"_Front":"_Back") << ((defect==Alternaria)?"_A":"_B") << "_s" << sample << "_r" << r << "_n" << subsample/1000 << "K");
	wave > Export<>(SS(folder << "\\Export" << infix << ".txt"), true);
}

void getTarget(Matrix<UInt32> &target, Matrix<UInt32> &coords, Matrix<UInt32> &rows, const String folder, Day day, Side side, Defect defect, UInt32 sample, UInt32 border, UInt32 subsample = 0) {
	//Read reference
	Matrix<Double> ref;
	readRef(ref, folder, day, side, defect, sample);	
	makeWindowRef(ref, target, border);
	makeWindowCoords(ref, coords, border);
		
	if (subsample > 0) {
		Matrix<UInt32> t1 = (target == 1U);
		Matrix<UInt32> t2 = (target == 2U);
		Matrix<UInt32> t3 = (target == 3U);
		Matrix<UInt32> set1, set2, set3;
		if (~t1 != 0) set1 = DSFunc::random(1U, subsample, (dtUInt32|0U||(~t1)-1));
		if (~t2 != 0) set2 = DSFunc::random(1U, subsample, (dtUInt32|0U||(~t2)-1));
		if (~t3 != 0) set3 = DSFunc::random(1U, subsample, (dtUInt32|0U||(~t3)-1));
		rows = t1(set1) || t2(set2) || t3(set3);
	} else {
		rows = (target == 1U) || (target == 2U) || (target == 3U);
	}
	
	//Slice
	target = target(rows);
	coords = coords(rows);	
}

void genTableForSample(Table<> &data, Matrix<UInt32> &rows, Matrix<UInt32> &target, Matrix<UInt32> &coords, const String folder, Day day, Side side, Defect defect, UInt32 sample, UInt32 r, UInt32 border, UInt32 waves=28) {
	Matrix<Double> feat;
	Matrix<String> ids;
		
	//For each wavelength
	Matrix<Double> wave;
	for (UInt32 i=0; i<waves;i++) {
		UInt32 wl = (i * 10) + 450;			

		//Read and apply bg correction
		wave.clear();
		readSample(wave, folder, day, side, defect, sample, wl);			
		Matrix<Double> back;
		readBack(back, folder, wl);
		back + 0.1;
		wave / back;

		//Make sliding window samples
		Matrix<Double> samples;
		makeWindowSamples(wave, samples, (MatrixIdx)r, border);
			
		feat | samples(rows);
		ids | genIds(SS("F_" << wl << "_"), (MatrixIdx)0, samples.cols.count()); 
		cout << wl << "..";
	}

	data = (ids ^ (ctFeature | feat)) | ((dtString|"Target") ^ (ctTarget | target)) | ((dtString|"x"|"y") ^ (ctIndex | coords));
	cout << endl;
}

void mergeSets(String folder, UInt32 subsample, bool exp=true) {
	int rs[3] = {0, 1, 2};
	for (int r = 0; r<3;r++) {
		Table<> data;
		for (int day = Day1; day<=Day2;day++) {			
			for (int side = Front; side<=Back;side++) {
				for (int sample = 0; sample<5;sample++) {
					for (int defect = Alternaria; defect<=Ozone;defect++) {
						if (!(day == 0 && defect == 1 && side == 0 && sample == 4)) {
							cout << ((day==Day1)?"_Day1":"_Day2") << ((side==Front)?"_Front":"_Back") << ((defect==Alternaria)?"_A":"_B") << "_s" << sample << "_r" << rs[r] << ".." << endl;
							Table<> readdata;
							readTable(readdata, folder + "\\DDSL", (Day)day, (Side)side, (Defect)defect, sample, rs[r], subsample);
							String sday = (day==Day1)?"Day1":"Day2";
							String sside = (side==Front)?"Front":"Back";
							String sdefect = (defect==Alternaria)?"A":"B";
							String ssample = SS(sample);
							UInt32 c = readdata.rows.count();
							Table<> tmpdata = (dtString | "Day" | "Side" | "Defect" | "Sample") ^ 
												(ctParameter | 
													(Matrix<String>(c, 1, sday) |  Matrix<String>(c, 1, sside) | Matrix<String>(c, 1, sdefect) | Matrix<String>(c, 1, ssample))
												);
							data ^ (readdata | tmpdata);
						}
					}
				}
			}
		}
		data >> SS(folder << "\\" << "_r" << rs[r] << ".ddsl");
		if (exp) data > Export<>(SS(folder << "\\" << "_r" << rs[r] << ".txt"), true);
	}
}

void makeDataSet(String folder, UInt32 subsample=0, UInt32 waves=28) {			
	int rs[3] = {0, 1, 2};	
	int border = 10;
	for (int day = Day1; day<=Day2;day++) {			
		for (int side = Front; side<=Back;side++) {
			for (int sample = 0; sample<5;sample++) {				
				for (int defect = Alternaria; defect<=Ozone;defect++) {
					if (!(day == 0 && defect == 1 && side == 0 && sample == 4)) {
						Matrix<UInt32> target;
						Matrix<UInt32> rows;
						Matrix<UInt32> coords;
						getTarget(target, coords, rows, folder, (Day)day, (Side)side, (Defect)defect, sample, border, subsample);
						for (int r = 0; r<3;r++) {
							if (!(day == 0 && defect == 1 && side == 0 && sample == 4)) {
								Table<> data;							
								cout << ((day==Day1)?"_Day1":"_Day2") << ((side==Front)?"_Front":"_Back") << ((defect==Alternaria)?"_A":"_B") << "_s" << sample << "_r" << rs[r] << endl;
								genTableForSample(data, rows, target, coords, folder, (Day)day, (Side)side, (Defect)defect, sample, rs[r], border, waves);
								cout << "_n" << subsample/1000 << "K" << endl;
								saveTable(data, folder + "\\DDSL", (Day)day, (Side)side, (Defect)defect, sample, rs[r]);
							}
						}
					}
				}
			}
		}
	}
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

void testNB(Table<> trainset, Table<> evalset, Matrix<UInt32> &confTrain, Matrix<UInt32> &confEval, Double &timeTrain, Double &timeEval, const String &filename="") {
	cout << "NaiveBayes: " << endl;
	Matrix<UInt32> classes = (dtUInt32|1U|2U|3U);	
	NaiveBayes<> nb = +NaiveBayes<>(classes) | -Confusion<>(classes);	
	confTrain = trainset > nb;
	confEval = evalset >> nb;
	timeTrain = nb.timeTrain();
	timeEval = nb.timeApply();
	if (filename != "")
		nb.write(filename);
}

void testSVM(Table<> trainset, Table<> evalset, Matrix<UInt32> &confTrain, Matrix<UInt32> &confEval, Double &timeTrain, Double &timeEval, const String &filename="") {
	cout << "SVM: " << endl;
	Matrix<UInt32> classes = (dtUInt32|1U|2U|3U);	
	SVM<> svm = +SVM<>(classes) | -Confusion<>(classes);	
	confTrain = trainset > svm;
	confEval = evalset >> svm;
	timeTrain = svm.timeTrain();
	timeEval = svm.timeApply();
	if (filename != "")
		svm.write(filename);
}

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

void Experiment19_09_2016(){
		String folder = "D:\\DATASETS\\Leafs";
		UInt32 samples = 2000;
		UInt32 waves = 28;
		bool exp = true;
		//makeDataSet(folder, samples, waves);
		//mergeSets(folder, samples*2, exp);
		Table<> summary;
		String filename;
		for (UInt32 r=0;r<3;r++) {
			for (UInt32 evalSample=0;evalSample<5;evalSample++) {
				//Slice set
				Table<> trainset, evalset;
				Matrix<UInt32> confTrain, confEval;
				Double timeTrain, timeEval;
				selectSet(trainset, evalset, folder, Day1, Back, r, evalSample);

				//Test classifiers
				filename = SS(folder << "\\Classifiers\\" << "NB_All_r" << r << "_s" << evalSample << ".ddsl");
				testNB(trainset, evalset, confTrain, confEval, timeTrain, timeEval, filename);
				makeSummary("NB", summary, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);
				
				filename = SS(folder << "\\Classifiers\\" << "SVM_All_r" << r << "_s" << evalSample << ".ddsl");
				testSVM(trainset, evalset, confTrain, confEval, timeTrain, timeEval, filename);		
				makeSummary("SVM", summary, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);
			}
		}
		summary > Export<>(folder + "\\Summary_All.txt", false);
		summary >> SS(folder << "\\Summary_All.ddsl");
}

void ImportAndWrite(const String &infile, String &outfile) {
	Table<> data;
	Import<> i = -Import<>(infile, " ");	
	Table<> r = data > !i;
	i.progressLoop();
	r >> outfile;
}

void Experiment20_09_2016() {
	String folder = "D:\\DATASETS\\Leafs";
	ImportAndWrite(folder + "\\_lda2_pca3_r0.txt", folder + "\\_lda2_pca3_r0.ddsl");
	ImportAndWrite(folder + "\\_lda2_pca3_r1.txt", folder + "\\_lda2_pca3_r1.ddsl");
	ImportAndWrite(folder + "\\_lda2_pca3_r2.txt", folder + "\\_lda2_pca3_r2.ddsl");
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

void Experiment21_09_2016(){
		String folder = "D:\\DATASETS\\Leafs";
		//UInt32 samples = 2000;
		UInt32 waves = 28;
		bool exp = true;
		//makeDataSet(folder, samples, waves);
		//mergeSets(folder, samples*2, exp);
		Table<> summarylda;
		Table<> summarypca;
		UInt32 r=0;
		UInt32 evalSample=0;
		String filename;
		for (UInt32 r=0;r<3;r++) {
			for (UInt32 evalSample=0;evalSample<5;evalSample++) {

				//Slice set
				Table<> trainset, evalset;
				Matrix<UInt32> confTrain, confEval;
				Double timeTrain, timeEval;
			
				//Test classifiers for LDA projection
				select_LDA_PCA_Set(trainset, evalset, folder, Day1, Back, r, evalSample);
				trainset = selectLDs(trainset, evalSample);
				evalset = selectLDs(evalset, evalSample);
				filename = SS(folder << "\\Classifiers\\" << "NB_LDA_r" << r << "_s" << evalSample << ".ddsl");				
				testNB(trainset, evalset, confTrain, confEval, timeTrain, timeEval, filename);
				makeSummary("NB", summarylda, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);				
				filename = SS(folder << "\\Classifiers\\" << "SVM_LDA_r" << r << "_s" << evalSample << ".ddsl");
				testSVM(trainset, evalset, confTrain, confEval, timeTrain, timeEval, filename);		
				makeSummary("SVM", summarylda, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);

				//Test classifiers for PCA projection
				select_LDA_PCA_Set(trainset, evalset, folder, Day1, Back, r, evalSample);
				trainset = selectPCs(trainset, evalSample);
				evalset = selectPCs(evalset, evalSample);				
				filename = SS(folder << "\\Classifiers\\" << "NB_PCA_r" << r << "_s" << evalSample << ".ddsl");
				testNB(trainset, evalset, confTrain, confEval, timeTrain, timeEval, filename);
				makeSummary("NB", summarypca, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);
				filename = SS(folder << "\\Classifiers\\" << "SVM_PCA_r" << r << "_s" << evalSample << ".ddsl");
				testSVM(trainset, evalset, confTrain, confEval, timeTrain, timeEval, filename);		
				makeSummary("SVM", summarypca, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);
			}
		}
		summarylda > Export<>(folder + "\\Summary_LDA.txt", false);
		summarylda >> SS(folder << "\\Summary_LDA.ddsl");
		summarypca > Export<>(folder + "\\Summary_PCA.txt", false);
		summarypca >> SS(folder << "\\Summary_PCA.ddsl");
}

void mergeSummary(const String &folder) {
	Table<> all, lda, pca, band3;
	all << SS(folder << "\\Summary_All.ddsl");
	lda << SS(folder << "\\Summary_LDA.ddsl");
	pca << SS(folder << "\\Summary_PCA.ddsl");
	band3 << SS(folder << "\\Summary_3Band.ddsl");

	all | ((dtString | "Select") ^ (ctParameter | Matrix<String>(all.rows.count(), 1, "All")));
	lda | ((dtString | "Select") ^ (ctParameter | Matrix<String>(all.rows.count(), 1, "LDA")));
	pca | ((dtString | "Select") ^ (ctParameter | Matrix<String>(all.rows.count(), 1, "PCA")));
	band3 | ((dtString | "Select") ^ (ctParameter | Matrix<String>(all.rows.count(), 1, "3Band")));

	all ^ lda ^ pca ^ band3;
	all >> SS(folder << "\\Summary_ALL_LDA_PCA.ddsl");
	all > Export<>(SS(folder << "\\Summary_ALL_LDA_PCA.txt"), false);
}

void Experiment21_09_2016_002(){
		String folder = "D:\\DATASETS\\Leafs";
		UInt32 samples = 2000;
		UInt32 waves = 28;
		bool exp = true;
		//makeDataSet(folder, samples, waves);
		//mergeSets(folder, samples*2, exp);
		Table<> summary;
		String filename;
		UInt32 r = 0;
		UInt32 evalSample = 0;
		for (UInt32 r=0;r<3;r++) {
			for (UInt32 evalSample=0;evalSample<5;evalSample++) {
				//Slice set
				Table<> trainset, evalset;
				Matrix<UInt32> confTrain, confEval;
				Double timeTrain, timeEval;				

				//Test classifiers
				selectSet(trainset, evalset, folder, Day1, Back, r, evalSample);
				trainset = selectFreq(trainset, r);
				evalset = selectFreq(evalset, r);
				filename = SS(folder << "\\Classifiers\\" << "NB_3Band_r" << r << "_s" << evalSample << ".ddsl");
				testNB(trainset, evalset, confTrain, confEval, timeTrain, timeEval, filename);
				makeSummary("NB", summary, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);

				selectSet(trainset, evalset, folder, Day1, Back, r, evalSample);
				trainset = selectFreq(trainset, r);
				evalset = selectFreq(evalset, r);
				filename = SS(folder << "\\Classifiers\\" << "SVM_3Band_r" << r << "_s" << evalSample << ".ddsl");
				testSVM(trainset, evalset, confTrain, confEval, timeTrain, timeEval, filename);		
				makeSummary("SVM", summary, evalSample, r, trainset, evalset, confTrain, confEval, timeTrain, timeEval);
			}
		}
		summary > Export<>(folder + "\\Summary_3Band.txt", false);
		summary >> SS(folder << "\\Summary_3Band.ddsl");
}


int main(int argc, char **) {
	try {		
		String folder = "D:\\DATASETS\\Leafs";
		//Experiment19_09_2016();
		//Experiment21_09_2016();
		//Experiment21_09_2016_002();
		mergeSummary(folder);
	} catch (Error &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}
