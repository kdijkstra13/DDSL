#include "ddsl.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;

void testGauss() {
	Matrix<String> classes = (dtString | "a" | "b" | "c" | "d");	
	const int c = 1000;
	cout << "Make table" << endl;
	//Create features in parallel
	GenGauss<> gg_a = GenGauss<>((dtDouble|10.0|10.0),(dtDouble|20.0|20.0)) ^ GenGauss<>((dtDouble|10.0|10.0),(dtDouble|20.0|20.0));
	GenGauss<> gg_b = GenGauss<>((dtDouble|10.0|10.0),(dtDouble|40.0|40.0)) ^ GenGauss<>((dtDouble|10.0|10.0),(dtDouble|40.0|40.0));
	GenGauss<> gg_c = GenGauss<>((dtDouble|10.0|10.0),(dtDouble|60.0|60.0)) ^ GenGauss<>((dtDouble|10.0|10.0),(dtDouble|60.0|60.0));
	GenGauss<> gg_d = GenGauss<>((dtDouble|10.0|10.0),(dtDouble|80.0|80.0)) ^ GenGauss<>((dtDouble|10.0|10.0),(dtDouble|80.0|80.0));
	auto ga = Table<>(c) > !gg_a;
	auto gb = Table<>(c) > !gg_b;
	auto gc = Table<>(c) > !gg_c;
	auto gd = Table<>(c) > !gg_d;	
	while (!(gg_a.last().wait(100) && gg_b.last().wait(100) && gg_c.last().wait(100) && gg_d.last().wait(100))) {
		gg_a.printProgress(cout);
		gg_b.printProgress(cout);
		gg_c.printProgress(cout);
		gg_d.printProgress(cout);
	}
	gg_a.printProgress(cout);
	gg_b.printProgress(cout);
	gg_c.printProgress(cout);
	gg_d.printProgress(cout);

	auto t = ctTarget ^ (Matrix<String>(c, 1, "a") ^ Matrix<String>(c, 1, "b") ^ Matrix<String>(c, 1, "c") ^ Matrix<String>(c, 1, "d"));
	Table<> in = (ga ^ gb ^ gc ^ gd) | t;
	in--;
	//cout << "Shuffle table" << endl;
	//in = in(shuffle((dtUInt32 | 0U || in.rows.count()-1), 5));
	
/*	cout << "Alloc pipeline" << endl;
	//train and write model to disk
	ZScore<> p =  ZScore<>() | +CaffeMLP<String>(classes,  caffe::Caffe::GPU, 0.001, 0.01, 1000, 40000, 1000) | -Confusion<String>(classes) | +Export<>("C:\\Projects\\DSL\\datasets\\Gauss\\CaffeMLP.txt", false);

	Table<> out = in > !p;
	p.progressLoop();
	out++;
	
	_chdir(string("c:\\temp\\caffemodel").c_str());
	//TODO: p.writeAll("Gauss"); write with prefix

	p[0].write("Gauss.0.ZScore.ddsl");
	p[1].write("Gauss.1.CaffeMLP.ddsl");
	p[2].write("Gauss.2.Confusion.ddsl");

	//read model from disk and apply
	ZScore<> p2 = ZScore<>("Gauss.0.ZScore.ddsl") | +CaffeMLP<String>("Gauss.1.CaffeMLP.ddsl", caffe::Caffe::CPU, true) | -Confusion<String>("Gauss.2.Confusion.ddsl");	
	
	Table<> out2 = in > !p2;
	p2.progressLoop();
	out2++;
	*/
}

void testTableSlicing() {
	//Create a table with Double and Int32 features. Also assign a target and a split column.
	Table<> t = ((dtString|"F1"|"F2"|"F3"|"F4"|"F5"|"F6"|"Target"|"Split") ^
				((ctFeature |
					(dtDouble | 0.0 | 0.1 | 0.2) ^
					(dtDouble | 1.0 | 1.1 | 1.2) ^
					(dtDouble | 2.0 | 2.1 | 2.2) ^
					(dtDouble | 3.0 | 3.1 | 3.2)
				) | (ctFeature |
					(dtInt32 | 00 | 01 | 02) ^
					(dtInt32 | 10 | 11 | 12) ^
					(dtInt32 | 20 | 21 | 22) ^
					(dtInt32 | 30 | 31 | 32)
				) | (ctTarget |
					dtString ^ "a" ^ "a" ^ "b" ^ "b"
				) | (ctSplit |
					dtUInt32 ^ 0U ^ 1U ^ 0U ^ 1U
				)));
	t++;
	//Slice row 4
	t(1)++;
	//Slice from row 1, a length of 3 rows
	t(1,3)++;
	//Slice all but the last row, and column 6
	t(0,3,6)++;
	//Slice all rows, and from column 2 to length 6
	t(0,4,2,6)++;

	//Slice all rows, and column 1 to column "F5"
	t(0, 4, 1, "F5")++;
	//Slice all rows, and column "F3" to column "Target"
	t(0, 4, "F3", "Target")++;
	//Slice row 2 and columns of content type ctFeature
	t(2, ctFeature)++;
	//Slice all rows and columns with data type dtDouble and dtInt32
	t(0, 4, dtDouble, dtInt32)++;

	//Slice column 3
	t[3]++;
	//Slice column id F3
	t["F3"]++;
	//Slice column data type dtDouble
	t[dtDouble]++;
	//Slice column content type ctSplit
	t[ctSplit]++;

	//Slice on multiple data types
	t(dtDouble, dtUInt32, dtString)++;
	//Slice on multiple content types
	t(ctTarget, ctSplit)++;

	//Slice on content type and data type
	t(ctFeature, dtInt32)++;
	//Slice on multiple content types and data types
	t(ctFeature, dtInt32, ctSplit, dtUInt32)++;
	//Slice all dtFeature content types with either data type dtInt32 or data type dtDouble
	t(ctFeature, dtDouble, dtInt32)++;

	//Slice rows 1,3
	t(dtUInt32|1U|3U)++;
	//Slice rows 1,3, and columns 2 and 6
	t(dtUInt32|1U|3U, dtUInt32|2U|6U)++;
	//Slice rows 1 through 3, and columns 2 and 6
	t(dtUInt32|1U||3U, dtUInt32|2U|6U)++;
	//Slice all rows , and column "F1", "F6" and "Target"
	t(0,t.rows.count(), dtString|"F1"|"F6"|"Target")++;
	//Slice all rows, and data types types dtDouble and dtUInt32
	t(0, ~t[0], (dtDataType|dtDouble|dtUInt32))++;
	//Slice all rows, and content type dtTarget
	t(dtUInt32|0U||~t[0]-1, (dtContentType|ctTarget))++;
	//Slice all rows and all columns
	t(t.rows.idxs(), t.cols.ids())++;

	//Slice the table on the split and extract features and targets.
	Table<> train = t(t[ctSplit] == 0U, ctFeature, ctTarget)++;
	Table<> test = t(t[ctSplit] == 1U, ctFeature, ctTarget)++;
}

void testTableTest() {
	//Create a table with Double and Int32 features. Also assign a target and a split column.
	Table<> t = ((dtString|"F1"|"F2"|"F3"|"F4"|"F5"|"F6"|"Target"|"Split") ^
				((ctFeature |
					(dtDouble | 0.0 | 0.1 | 0.2) ^
					(dtDouble | 1.0 | 1.1 | 1.2) ^
					(dtDouble | 2.0 | 2.1 | 2.2) ^
					(dtDouble | 3.0 | 3.1 | 3.2)
				) | (ctFeature |
					(dtInt32 | 00 | 01 | 02) ^
					(dtInt32 | 10 | 11 | 12) ^
					(dtInt32 | 20 | 21 | 22) ^
					(dtInt32 | 30 | 31 | 32)
				) | (ctTarget |
					dtString ^ "a" ^ "a" ^ "b" ^ "b"
				) | (ctSplit |
					dtUInt32 ^ 0U ^ 1U ^ 0U ^ 1U
				)));

	//Return all rows indices where F1 is larger than greater equal to 2 and F2 is greater equal to 3
	(t >= ((dtString|"F1"|"F3") ^ (ctFeature|(dtDouble|1.0|2.0))))++;
	//Return row indices where which test positive to column with id "Target" == "a" and column id "Split" == 1
	(t == ((dtString|"Target"|"Split") ^ ((ctTarget|(dtString|"a")) | (ctSplit|(dtUInt32|1U)))))++;
	//Test which rows are equal to row 1
	(t == t(1))++;

	//Tests which columns of the double features in table t are equal to
	(t(dtDouble, ctFeature) == (dtDouble ^ 0.1 ^ 1.1 ^ 2.1 ^ 3.1))++;
	//Tests which values of the int 32 features in table t are greater than 21
	(t(dtInt32, ctFeature) > (dtInt32|21))++;
}

void testTableArith() {
		Table<> t = ((dtString|"F1"|"F2"|"F3"|"F4"|"F5"|"F6"|"Split") ^
				((ctFeature |
					(dtDouble | 0.0 | 0.1 | 0.2) ^
					(dtDouble | 1.0 | 1.1 | 1.2) ^
					(dtDouble | 2.0 | 2.1 | 2.2) ^
					(dtDouble | 3.0 | 3.1 | 3.2)
				) | (ctFeature |
					(dtInt32 | 00 | 01 | 02) ^
					(dtInt32 | 10 | 11 | 12) ^
					(dtInt32 | 20 | 21 | 22) ^
					(dtInt32 | 30 | 31 | 32)
				) | (ctSplit |
					dtUInt32 ^ 0U ^ 1U ^ 0U ^ 1U
				)));
		//Adds one to the matrix in the table with the corresponding data type (in this case F4, F5 and F6)
		(t + 1)++;
		//Adds one to the matrix in the table with the corresponding data type (in this case F1, F2 and F3)
		(t + 10.0)++;
		//Adds one to the matrix in the table with the corresponding data type (in this case Split)
		(t + 100U)++;

		//Subtracts this row from each matrix row with the corresponding data type
		(t - (dtDouble|10.0|5.0|2.0))++;
		//Subtracts this column from each matrix column with the corresponding data type
		(t - (dtInt32^20^30^40^50))++;
}

void testTableStream() {
	Table<> a = ((dtString|"F1"|"F2"|"F3"|"F4"|"F5"|"F6"|"Target"|"Split") ^
			((ctFeature |
				(dtDouble | 0.0 | 0.1 | 0.2) ^
				(dtDouble | 1.0 | 1.1 | 1.2) ^
				(dtDouble | 2.0 | 2.1 | 2.2) ^
				(dtDouble | 3.0 | 3.1 | 3.2)
			) | (ctFeature |
				(dtInt32 | 00 | 01 | 02) ^
				(dtInt32 | 10 | 11 | 12) ^
				(dtInt32 | 20 | 21 | 22) ^
				(dtInt32 | 30 | 31 | 32)
			) | (ctTarget |
				dtString ^ "a" ^ "a" ^ "b" ^ "b"
			) | (ctSplit |
				dtUInt32 ^ 0U ^ 1U ^ 0U ^ 1U
			)));
	Table<> b = (ctFeature |
					(dtDouble | 0.0 | 0.1 | 0.2) ^
					(dtDouble | 1.0 | 1.1 | 1.2) ^
					(dtDouble | 2.0 | 2.1 | 2.2) ^
					(dtDouble | 3.0 | 3.1 | 3.2)
				);
	Table<> c,d,e;

	//Stream table a to filestream
	ofstream ofs("c:\\temp\\test1.ddsl");
	a >> ofs;
	//Stream table b tot filestream, add 10 and stream again.
	((b >> ofs) + 10.0) >> ofs;
	ofs.close();

	ifstream ifs("c:\\temp\\test1.ddsl");
	//Stream to table c (equal to table a)
	c << ifs;
	//Stream to table d (equal to table b)
	d << ifs;
	//Stream to table e (equal to table b + 10.0)
	e << ifs;
	ifs.close();
	c++;d++;e++;

	//Stream table a to file
	a >> "c:\\temp\\test2.ddsl";
	//Stream to table c (equal to table a)
	c << "c:\\temp\\test2.ddsl";
	c++;
}

void testModel() {
	Table<> tab = (dtString|"X"|"Y"|"Target"|"Set") ^ (
		((ctFeature|(dtDouble|1.0|1.0)) | (ctTarget|(dtString|"a")) | (ctSplit|(dtString|"t"))) ^
		((ctFeature|(dtDouble|2.0|2.0)) | (ctTarget|(dtString|"a")) | (ctSplit|(dtString|"t"))) ^
		((ctFeature|(dtDouble|3.0|1.0)) | (ctTarget|(dtString|"b")) | (ctSplit|(dtString|"t"))) ^
		((ctFeature|(dtDouble|4.0|2.0)) | (ctTarget|(dtString|"b")) | (ctSplit|(dtString|"t"))));
		
	Matrix<String> classes = (dtString|"a"|"b");
	
	//Stream the table through the model
	(tab > NaiveBayes<String>(classes))++;

	//Passthrough only scaled inputs
	Table<> res_zs = tab > +ZScore<>();
	res_zs++;
	
	//Passthrough all inputs
	Table<> res_nb = res_zs > +NaiveBayes<String>(classes);
	res_nb++;

	//Passthrough none of the inputs (because the dimensions of confusion are different)
	Table<> res_c = res_nb > -Confusion<String>(classes);
	res_c++;
}

void testModel2() {
	//Create a small set where the last two samples cannot be inferred from the training set
	Table<> tab = (dtString|"X"|"Y"|"Target"|"Set") ^ (	
		((ctFeature|(dtDouble|1.0|1.0)) | (ctTarget|(dtString|"a")) | (ctSplit|(dtString|"t"))) ^
		((ctFeature|(dtDouble|2.0|1.0)) | (ctTarget|(dtString|"a")) | (ctSplit|(dtString|"e"))) ^
		((ctFeature|(dtDouble|2.0|2.0)) | (ctTarget|(dtString|"a")) | (ctSplit|(dtString|"t"))) ^
		((ctFeature|(dtDouble|1.0|2.0)) | (ctTarget|(dtString|"a")) | (ctSplit|(dtString|"e"))) ^
		((ctFeature|(dtDouble|3.0|1.0)) | (ctTarget|(dtString|"b")) | (ctSplit|(dtString|"t"))) ^
		((ctFeature|(dtDouble|4.0|1.0)) | (ctTarget|(dtString|"b")) | (ctSplit|(dtString|"e"))) ^
		((ctFeature|(dtDouble|4.0|2.0)) | (ctTarget|(dtString|"b")) | (ctSplit|(dtString|"t"))) ^
		((ctFeature|(dtDouble|3.0|2.0)) | (ctTarget|(dtString|"b")) | (ctSplit|(dtString|"e"))) ^
		((ctFeature|(dtDouble|3.5|1.5)) | (ctTarget|(dtString|"a")) | (ctSplit|(dtString|"e"))) ^
		((ctFeature|(dtDouble|1.5|1.5)) | (ctTarget|(dtString|"b")) | (ctSplit|(dtString|"e"))));
	
	//Define classes
	Matrix<String> classes = (dtString|"a"|"b");

	//Extract the training part and the evaluation part.
	Table<> train = tab(tab[ctSplit] == "t");
	Table<> eval = tab(tab[ctSplit] == "e");
	train++;
	eval++;
	
	//Create a naive bayes classifier
	NaiveBayes<String> nb = +NaiveBayes<String>(classes);

	//Train with the training set
	(train > nb)++;
	
	//Apply model with the evaluation set
	auto out = eval >> nb;
	
	//Calculate confusion
	(out >> -Confusion<String>(classes))++;

	
}

void testModelWrite() {
	//Generate 2d point clouds from 2 random Guassian distributions. Assign a target and a random splitting column
	const UInt32 cnt = 100;
	Table<> tab =	((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
					(ctTarget|(Matrix<String>(cnt,1,"a") ^ Matrix<String>(cnt,1,"b"))) |
					(ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U))));
	
	//Define classes
	Matrix<String> classes = (dtString|"a"|"b");

	//Extract the training part and the evaluation part.
	Table<> train = tab(tab[ctSplit] == 0U);
	Table<> eval = tab(tab[ctSplit] == 1U);
	
	//Create a naive bayes classifier, train it, and then save it
	NaiveBayes<String> nb = +NaiveBayes<String>(classes);
	train > nb;
	nb >> "c:\\temp\\nb.ddsl";
	
	//Load naive bayes classifier
	NaiveBayes<String> nb2;
	nb2 << "c:\\temp\\nb.ddsl";
	
	//Apply model with the evaluation set	
	(eval >> nb2 >> -Confusion<String>(classes))++;

	((ctFeature|((dtDouble|2.0|1.0)^(dtDouble|4.0|2.0))) > ZScore<>())++;
}

void testSyncPipe() {
	//Generate a random 2d point cloud from a Gaussian distribution. Calculate the Z Score and apply a Random classifier (just output Random classes)
	Table<>(10) > GenGauss<>(dtDouble|10.0|10.0, dtDouble|0.0|0.0) > ZScore<>() > Random<String>(dtString|"a"|"b");
	//Also show the input features by using passthrough operator '+' (all)
	Table<>(10) > GenGauss<>(dtDouble|10.0|10.0, dtDouble|0.0|0.0) > +ZScore<>() > Random<String>(dtString|"a"|"b");

	//Create 200 random points with two classes and two sets.
	const UInt32 cnt = 100;
	Matrix<String> classes = (dtString|"a"|"b");
	Table<> tab =	((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
					(ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) |
					(ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U))));
	
	//Create the pipeline
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) | -Confusion<String>(classes);
	
	//Train pipeline and display output (in this case confusion between classes)
	(tab(tab[ctSplit] == 0U) > p)++;

	//Evaluate pipeline and display output (in this case confusion between classes)
	(tab(tab[ctSplit] == 1U) >> p)++;
	
}

void testAsyncPipe() {
	//Create a data table with 200K samples
	const UInt32 cnt = 100000;
	Matrix<String> classes = (dtString|"a"|"b");
	Table<> tab =	((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
					(ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) |
					(ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U))));

	//Create a pipeline
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) | -Confusion<String>(classes);

	//Execute pipeline and save data to the deferred output (out)
	Table<> out = tab > !p;
	cout << "out is currently filling with data!" << endl;
	//Re-attach (wait for) pipeline
	!p;
	//Show deferred output
	out++;

	//Execute pipeline and save data to the deferred output while showing progress(out)
	Table<> out2 = tab > !p;
	p.progressLoop();
	//Show deferred output
	out++;
}

void testAsyncPipe2() {
	//Create a data table with 200K samples
	const UInt32 cnt = 100000;
	Matrix<String> classes = (dtString|"a"|"b");
	Table<> tab =	((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
					(ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) |
					(ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U))));

	//Create a pipeline
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes);

	//Alloc and execute pipeline and show curently produced output data.
	Table<> out = tab > !p;
	//Display intermediate confusion
	do {		
		(out > -Confusion<String>(classes))++;
	} while (!p.last().wait(10));
	//Display the final confusion
	(out > -Confusion<String>(classes))++;
}

void testAsyncModel() {
	Matrix<String> classes = (dtString|"a"|"b");	
	const int c = 1000;	
	//Create features in parallel (using the asynchronous pipeline operator '^')
	GenGauss<> gauss_p_a = GenGauss<>((dtDouble|10.0|10.0),(dtDouble|20.0|20.0)) ^ GenGauss<>((dtDouble|10.0|10.0),(dtDouble|20.0|20.0));
	GenGauss<> gauss_p_b = GenGauss<>((dtDouble|10.0|10.0),(dtDouble|40.0|40.0)) ^ GenGauss<>((dtDouble|10.0|10.0),(dtDouble|40.0|40.0));
	
	//Execute both pipelines asynchronous
	auto gauss_a = Table<>(c) > !gauss_p_a;
	auto gauss_b = Table<>(c) > !gauss_p_b;
	//Wait for both pipelines
	!gauss_p_a;
	!gauss_p_b;

	//Create final table
	Table<> tab = (gauss_a ^ gauss_b) | (ctTarget ^ (Matrix<String>(c, 1, classes[0]) ^ Matrix<String>(c, 1, classes[1])));

	//Classify with an ensemble of 2 classifiers (Both classifiers work parallel with the asynchronous pipeline operator '^')
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) ^ +Random<String>(classes);

	//Execute pipeline
	auto out = tab > p;

	//Show result of Random
	((out[ctResult][0] | out[ctTarget]) > -Confusion<String>(classes))++;

	//Show result of NaiveBayes
	((out[ctResult][1] | out[ctTarget]) > -Confusion<String>(classes))++;
}

void moveAndCopyPipe() {
	//Create 2000 random samples drawn from a gaussian distribution
	const UInt32 cnt = 1000;
	Matrix<String> classes = (dtString|"a"|"b");
	Table<> tab =	((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
					(ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) |
					(ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U))));
	
	//The pipeline which is constructed at the rhs is moved to the lhs (deep move)
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) | -Confusion<String>(classes);
	//Train pipeline
	(tab > p)++;

	//Copy ZScore, NaiveBayes and Confusion to p2
	ZScore<> p2 = p[0];
	p2 | p[1] | p[2];

	//Show output of the copied p2
	(tab >> p2)++;

}

void slicingPipe() {
		//Create 2000 random samples drawn from a gaussian distribution
	const UInt32 cnt = 1000;
	Matrix<String> classes = (dtString|"a"|"b");
	Table<> tab =	((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
					(ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) |
					(ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U))));
	
	//The pipeline which is constructed at the rhs is moved to the lhs (deep move)
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) | -Confusion<String>(classes);
	tab > p;

	//Slice model 2 (Confusion) and show result
	p[2].tab()++;
	
	//slice model 1 (NaiveBayes) and show first 2 rows
	p[1].tab()(0,2)++;

	//Assign confusion model of p to a model reference (This means that conf will still reference the Confusion instance in the pipeline)
	Model<> & conf = p[2];

	//Assign confusion output to conf_res. Because tables are not assigned but referenced, conf_res will be a reference to the output of the Confusion model
	Table<> conf_res = p[2];
	conf_res++;

	//Assign the constant 3U to all UInt32 matrices inside the output table of model 2
	p[2].tab() = 3U;

	//The action above also changes conf_res because that is a reference
	conf_res++;

	//The slice operator for models will force a deep copy (this will copy all models)
	Model<> & p2 = p();
	(tab >> p2)++;

	//Copy the first 2 models and execute our own confusion inline
	Model<> & p3 = p(2);
	((tab >> p3) > -Confusion<String>(classes))++;

	//Printing
	p++;
	p--;
}

void rwPipe() {
	const UInt32 cnt = 1000;
	Matrix<String> classes = (dtString|"a"|"b");
	Table<> tab =	((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
					(ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) |
					(ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U))));
	
	//Construct and train pipeline
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) | -Confusion<String>(classes);
	(tab > p)++;

	//Save individual models
	p[0] >> "c:\\temp\\zs.ddsl";
	p[1] >> "c:\\temp\\nb.ddsl";
	p[2] >> "c:\\temp\\conf.ddsl";

	//Reconstruct pipeline and load models
	ZScore<> p2 = ZScore<>("c:\\temp\\zs.ddsl") | NaiveBayes<String>("c:\\temp\\nb.ddsl") | Confusion<String>("c:\\temp\\conf.ddsl");
	//Apply pipeline
	(tab >> p2)++;
}

void pipelineExec() {
	//Create a table and split in 4 parts
	const UInt32 cnt = 1000;
	Matrix<String> classes = (dtString|"a"|"b");
	Table<> tab =	((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
					(ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) |
					(ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U|2U|3U))));
	
	//Construct pipeline
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) | -Confusion<String>(classes) | +Export<>("c:\\temp\\conf.txt");
	//Train pipeline
	(tab > p)++;
	//Apply pipeline on one subset but skip Export (place breakpoint and check file)
	(tab(tab[ctSplit] == 0U) << p[3])++;
	
	//Only apply ZScore (execute from 0 until 1, not including 1). Use print less operator (--) because of big table
	auto out = (tab << p[1])--;
	
	//Apply only the NaiveBayes, Confusion and Export but skip ZScore. ZScore has already been calculated in the Table out
	(out >> p[1])++;
	
	//Skip ZScore in training and applying (Effectively removes ZScore calculation)
	(tab > p[1])++;
	//The apply operator (>>) will append to the file.
	(tab >> p[1])++;
}

void preparePipe() {
	const UInt32 cnt = 1000;
	Matrix<String> classes = (dtString|"a"|"b"); 
	Table<> tab = ((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0))^(Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | (ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) | (ctSplit|(random(2*cnt, 1U, (dtUInt32|0U|1U|2U|3U)))); 
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) | -Confusion<String>(classes);
	//Applying the train and apply operator to a pipeline will prepare it only once because the table tab does not change.
	tab > p; 
	tab >> p; 
	//However, training with one slice and applying with a second will prepare twice because the data is different
	(tab(tab[ctSplit] == 0U || tab[ctSplit] == 1U) > p)++;
	(tab(tab[ctSplit] == 1U || tab[ctSplit] == 2U) >> p)++;
}

void dontPreparePipe() {
	//Generate a dataset with 4 batches
	//Each batch should be equal, so first create equal batches, and then randomize content
	const UInt32 cnt = 1000;
	Matrix<String> classes = (dtString|"a"|"b"); 
	Table<> tab = ((Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|0.0|0.0)) ^ (Table<>(cnt) > GenGauss<>(dtDouble|10.0|10.0,dtDouble|20.0|20.0))) | 
		(ctTarget|(Matrix<String>(cnt,1,classes[0]) ^ Matrix<String>(cnt,1,classes[1]))) |
		(ctSplit|shuffle(Matrix<UInt32>(cnt/2,1,0U,oColMajor)^Matrix<UInt32>(cnt/2,1,1U,oColMajor)^Matrix<UInt32>(cnt/2,1,2U,oColMajor)^Matrix<UInt32>(cnt/2,1,3U,oColMajor)));	
	ZScore<> p = ZScore<>() | +NaiveBayes<String>(classes) | -Confusion<String>(classes);
	
	//batch zero is the batch included in the pipeline, and thus should be explicitly copied.
	Table<> batch0 = *tab(tab[ctSplit] == 0U);
	Table<> batch1 = tab(tab[ctSplit] == 1U);
	Table<> batch2 = tab(tab[ctSplit] == 2U);
	Table<> batch3 = tab(tab[ctSplit] == 3U);
	
	//Show distribution of classes in the batches
	cout << "batch 0: #a=" << ~(batch0[ctTarget] == "a") << ", #b=" << ~(batch0[ctTarget] == "b") << endl;
	cout << "batch 1: #a=" << ~(batch1[ctTarget] == "a") << ", #b=" << ~(batch1[ctTarget] == "b") << endl;
	cout << "batch 2: #a=" << ~(batch2[ctTarget] == "a") << ", #b=" << ~(batch2[ctTarget] == "b") << endl;
	cout << "batch 3: #a=" << ~(batch3[ctTarget] == "a") << ", #b=" << ~(batch3[ctTarget] == "b") << endl;

	//train and prepare with batch0
	(batch0 > p)++;
	//Because batches have equal sizes the data will be copied (direct slice assignment).
	batch0 = batch1;
	//The memory location is the same so not preparing again
	(batch0 >> p)++;
	//Same for batch 2
	batch0 = batch2;
	(batch0 >> p)++;
	//Same for batch 3
	batch0 = batch3;
	(batch0 >> p)++;
}

void advanced() {
	Matrix<Double, UInt64> m(10, 10, 5.5);
	m++;
}

void matrixTest() {
	Matrix<Double> a;
	Matrix<Double> b;
	b = a;

	//Creates a row major Matrix with one row and 3 columns containing doubles.
	Matrix<Double> m1 = (dtDouble | 1.0 | 2.0 | 3.0);
	//This example with the range operator creates a matrix with one row containing double values 1, 2, 7, 8, 9, 10.
	Matrix<Double> m2 = (dtDouble | 1.0 | 2.0 | 7.0 || 10.0);
	//Creates a column major Matrix with one row and three columns containing unsigned 32 bits integers. 
	Matrix<UInt32> m3 = (dtUInt32 ^ 1U ^ 2U ^ 3U);
	//Creates a row major Matrix with three rows and one column containing signed 32 bits integers. 
	Matrix<Int32> m4 = ((dtInt32 | 1) ^ 2 ^ 3);
	//Creates a col major Matrix with one row and three columns containing floats.
	Matrix<Float> m5 = ((dtFloat ^ 1.0f) | 2.0f | 3.0f);
	//Creates a row major 3 x 3 identity matrix containing unsigned 32 bits integers.
	Matrix<Int32> m6 = (dtInt32 | 1 | 0 | 0) ^ (dtInt32 | 0 | 1 | 0) ^ (dtInt32 | 0 | 0 | 1);
	//Creates a row major 2 x 1 matrix containing a row major 1 x 3 matrix and a row major 1 x 4 matrix.
	Matrix<Matrix<Int32>> m7 = (dtMatrixInt32 | (dtInt32 | 1 | 2 | 3)) ^ (dtMatrixInt32 | (dtInt32 | 4 | 5 | 6 | 7));
	//Use ++ to print the content of a Matrix
	m1++;m2++;m3++;m4++;m5++;m6++;m7++;

}

void matrixTest2() {
	//Reference matrix a to b.
	Matrix<Int32> a = (dtInt32 | 1 | 2 | 3) ^ (dtInt32 | 4 | 5 | 6) ^ (dtInt32 | 7 | 8 | 9);
	Matrix<Int32> b = a;
	//print a
	a++;
	//This will also change a
	b.val(1,1) = 25;
	a++;b++;
	//This forces the connection between a and b to break. And b will be copied from a
	!b;
	//This will now only change b
	b.val(1,1) = 35;
	a++;b++;
	//To duplicate a to c use dereference
	Matrix<Int32> c = *a;
	//This will now only change c
	c.val(1,1) = 40;
	a++;c++;
	//If two matrices of the same size are assigned they will be copied. This is used for direct slice assignment (see Slicing).
	Matrix<Int32> d(3,3);
	d = a;
	//This will now only change d
	d.val(1,1) = 42;
	a++;d++;
	//This will assign the value 1 to each matrix element
	a = 1;
	a++;
	//This will assign the top left value of a to the variable e.
	Int32 e = a(0,1,0,1);
	cout << e;
}

void matrixTest3() {
	Matrix<Int32> a = (dtInt32 | 1 | 2 | 3) ^ (dtInt32 | 4 | 5 | 6) ^ (dtInt32 | 7 | 8 | 9);
	Matrix<Int32> b;
	//This will transpose matrix a and assign it to matrix b (a is unchanged)
	b += a;
	b++;
	//This will matrix multiply b with a and assign the result to b
	b *= a;
	b++;
	//This forces b to column major after copying from a
	b |= a;
	b++;
	//This forces b to row major
	b ^= b;
	b++;
}

void matrixTest4() {
	Matrix<Int32> a = (dtInt32 | 1 | 2 | 3) ^ (dtInt32 | 4 | 5 | 6) ^ (dtInt32 | 7 | 8 | 9);
	//Return a slice of the first row
	a(0)++;
	//Return a copy of a slice of the second row
	*a(1)++;
	//Return a slice with 2 rows from row 1, and all columns
	a(1, 2)++;
	//Return a slice with 2 rows from 1, and only column 1
	a(1, 2, 1)++;
	//Return a slice with 2 rows from row 1, and two columns from column 2
	a(1, 2, 0, 2)++;
	//Return a slice of column 1, and all rows
	a[1]++;
	//Return a slice from row 0,1,2 and column 1 and 2
	a(dtUInt32 | 0U | 1U | 2U, dtUInt32 | 1U | 2U)++;
	//The example below will create new matrices because the slices are not continuous rectangular regions
	//Return a slice of row 0 and 2, and all columns
	a(dtUInt32 | 0U | 2U)++;
	//Return a slice of row 0 and 2, and column 1 and 2
	a(dtUInt32 | 0U | 2U, dtUInt32 | 1U | 2U)++;
	//Return a slice of row 0 and 2, and column 1
	a(dtUInt32 | 0U | 2U, 1)++;
	//Return a slice of row 0 and 2, and column 0 to column 1
	a(dtUInt32 | 0U | 2U, 0, 2)++;
	//Return a slice of row 1, and column 0 and column 2
	a(1, dtUInt32 | 0U | 2U)++;
	//Return a slice of row 0 to row 2, and column 0 and column 2
	a(0, 3, dtUInt32 | 0U | 2U)++;
}

void matrixTest5() {
	Matrix<Int32> a = (dtInt32 | 1 | 2 | 3) ^ (dtInt32 | 4 | 5 | 6) ^ (dtInt32 | 7 | 8 | 9);
	Matrix<Int32> b = (dtInt32 | 10 | 11 | 12) ^ (dtInt32 | 13 | 14 | 15) ^ (dtInt32 | 16 | 17 | 18);
	a++;b++;
	//Assign a slice of b to a slice of a (direct slice assignment)
	a(0,2,0,2) = b(1,2,1,2);
	a++;b++;
	//Assign a value of 25 to each element of the slice of b (this will not change a, but will change part of b)
	b(0,2,0,2) = 25;
	a++;b++;
}

void matrixTestTest1() {
	Matrix<Int32> a = (dtInt32 | 1 | 2 | 3) ^ (dtInt32 | 4 | 5 | 6) ^ (dtInt32 | 7 | 8 | 9);
	//Test which rows are equal to the given row. Example returns row vector with the value 0
	(a == (dtInt32 | 1 | 2 | 3))++;
	//Test which rows are greater than the given row (for each value in the corrosponding row). Example returns row vector with the value 1 and 2
	(a > (dtInt32 | 1 | 2 | 3))++;
	//Other examples using >=, <= and != operators are trivial

	Matrix<Int32> b = (dtInt32 | 1 | 2 | 3) ^ (dtInt32 | 4 | 5 | 6) ^ (dtInt32 | 7 | 8 | 9);
	//Test which columns are equal to the given columns. Example returns column vector with the value 0
	(b == (dtInt32 ^ 1 ^ 4 ^ 7))++;
	//Test which columns are greater than the given columns (for each value in the corrosponding row). Example returns column vector with the value 1 and 2
	(b > (dtInt32 ^ 1 ^ 4 ^ 7))++;
	//Other examples using >=, <= and != operators are trivial

	Matrix<Int32> c = (dtInt32 ^ 1 ^ 2 ^ 3);
	//Test which values in the column vector are equal to the given value. Example returns column vector with the value 0
	(c == 1)++;
	//Test which values in the column vector are greater than the given value. Example returns column vector with the value 1 and 2
	(c > 1)++;
	//Other examples using >=, <= and != operators are trivial

	Matrix<Int32> d = (dtInt32 | 1 | 2 | 3);
	//Test which values in the row vector are equal to the given value. Example returns row vector with the value 0
	(d == 1)++;
	//Test which values in the row vector are greater than the given value. Example returns row vector with the value 1 and 2
	(d > 1)++;
	//Other examples using >=, <= and != operators are trivial

	Matrix<Int32> e = (dtInt32 | 1 | 2 | 3) ^ (dtInt32 | 4 | 5 | 6) ^ (dtInt32 | 7 | 8 | 9);
	Matrix<Int32> f = (dtInt32 | 10 | 20 | 30) ^ (dtInt32 | 40 | 50 | 60) ^ (dtInt32 | 70 | 80 | 90);
	//Test if values are equal. Example returns an empty matrix
	(e == f)++;
	//Test if all values in a are greater than all values in b. Example returns a 1 x 1 matrix
	(f > e)++;
	//Other examples using >=, <= and != operators are trivial

	Matrix<Int32> g = (dtInt32 | 1 | 2 | 3) ^ (dtInt32 | 4 | 2 | 6) ^ (dtInt32 | 7 | 2 | 9);
	//Test if values are equal. Example returns a 3 x 2 matrix containing (0 | 1) ^ (1 | 1) ^ (2 | 1)
	(g == 2)++;
	//Other examples using >=, <= and != operators are trivial
}

void matrixSetLogic() {
	Matrix<Int32> a = (dtInt32 | 1 | 2 | 3 | 4);
	Matrix<Int32> b = (dtInt32 | 3 | 4 | 5 | 6);
	a++;b++;
	//Union between a and b
	(a || b)++;	
	//Intersection between a and b
	(a && b)++;
	Matrix<Int32> c = (dtInt32 | 1 | 2 | 3)^(dtInt32 | 4 | 5 | 6)^(dtInt32 | 7 | 8 | 9)^(dtInt32 | 1 | 2 | 3);
	//Slice and select rows which equal either one of the given rows
	c((c == (dtInt32 | 1 | 2 | 3)) || (c == (dtInt32 | 7 | 8 | 9)))++;
}

void matrixArith() {
	Matrix<Double> a = (dtDouble | 1.0 | 2.0 | 3.0)++;
	Matrix<Int32> b = (dtInt32 | 1 | 2 | 3)++;
	//Negate all values of a	
	(-a)++;
	//Add 2 to each element of a
	(a + 21.0)++;
	//Modulo each element of b with 2
	(b % 2)++;
	//Square each element of a
	pow(a, 2.0)++;
	//Square root each element of a
	pow(a, 0.5)++;
	//Multiply one matrix with another element wise
	(a * (dtDouble | 2.0 | 4.0 | 6.0))++;

}

void matrixCount() {
	Matrix<Double> a = (dtDouble | 1.0 | 2.0) ^ (dtDouble | 3.0 | 4.0) ^ (dtDouble | 5.0 | 6.0);
	Matrix<Double> b = (dtDouble | 1.0 | 2.0) ^ (dtDouble | 3.0 | 4.0) ^ (dtDouble | 5.0 | 6.0);
	//Count elements of a. Example returns 6.
	cout << ~a << endl;
	//Count rows of a. Example returns 3.
	cout << ~a[1] << endl;
	//Count columns of a. Example returns 2.
	cout << ~a(1) << endl;
	//Return true if a and b are equal.
	cout << (~(a == b) == 1) << endl;
	//Return true if a and b are not equal.
	cout << (~(a == b) == 0) << endl;
	cout << (~(a != b) == 1) << endl;
}

void matPrinting() {
	Matrix<Double> a = (dtDouble | 1.0 | 2.0) ^ (dtDouble | 3.0 | 4.0) ^ (dtDouble | 5.0 | 6.0);
	//Print matrix a.
	a++;
	//Print metadata of matrix a.
	a--;
	//Print the first print n, then print the first row of m, then print the first rows of m where 2 has been subtracted, then print m (with adjusted first row).
	Matrix<Double> m = (dtDouble | 1.0 | 2.0) ^ (dtDouble | 3.0 | 4.0) ^ (dtDouble | 5.0 | 6.0);
	(m(m++ == m(0))++ - 2.0)++;
	m++;	
}

void matStream() {
	Matrix<Double> a = (dtDouble | 1.0 | 2.0) ^ (dtDouble | 3.0 | 4.0) ^ (dtDouble | 5.0 | 6.0);
	Matrix<Double> b = (dtDouble | 7.0 | 8.0) ^ (dtDouble | 9.0 | 10.0) ^ (dtDouble | 11.0 | 12.0);
	Matrix<Double> c,d,e,f;
	//Stream a and b to file stream.
	ofstream ofs("c:\\temp\\test.ddsl");
	ofs << a << b;
	ofs.close();

	//Stream a to filestream, add 2 to each element of a, and then stream to file again.
	ofstream ofs2("c:\\temp\\test2.ddsl");
	((a >> ofs2) + 2.0) >> ofs2;
	ofs2.close();

	//Stream c and d from file stream.
	ifstream ifs("c:\\temp\\test.ddsl");
	ifs >> c >> d;
	ifs.close();
	c++;d++;

	//Stream e and f from file stream and subtract f from e.
	ifstream ifs2("c:\\temp\\test2.ddsl");
	(e << ifs2) - (f << ifs2);
	ifs2.close();
	e++;f++;

	Matrix<Double> g = (dtDouble | 10.0 | 20.0) ^ (dtDouble | 30.0 | 40.0) ^ (dtDouble | 50.0 | 60.0);
	Matrix<Double> h;
	//Stream a to file.
	g >> "c:\\temp\\test3.ddsl";
	//Stream b from file.
	h << "c:\\temp\\test3.ddsl";
	h++;
}

void matAccess() {
	//Create a 3 x 3 identity Matrix
	Matrix<Int32> m = (dtInt32 | 1 | 0 | 0) ^ (dtInt32 | 0 | 1 | 0) ^ (dtInt32 | 0 | 0 | 1);
	//Access row 3 and column 1 of matrix m
	UInt32 a = m.val(2,1);
	//Assign the value 3 to row 3 and column 1 of matrix m
	m.val(2,1) = 3;
	//Create a 1 x 3 Matrix
	Matrix<Int32> v = (dtInt32 | 1 | 2 | 3);
	//Access the matrix as a vector and access the second element
	UInt32 b = v.vec(1);
	//Access the matrix as a vector and access the third element
	UInt32 c = v.vec().val(3);
	//Assign 3 to the last vector element
	v.vec(2) = 3;
	cout << a << " " << b << " " << c << endl;
	m++;
	v++;
}

void matIter() {

	//Create the matrix
	Matrix<Double> m = (dtDouble | 1.0 | 2.0 | 3.0) ^ (dtDouble | 4.0 | 5.0 | 6.0);
	MatrixIdx r = 1;
	MatrixIdx c = 1;
	//get Matrix::Index::Iterator to the second row
	m.rows[1];
	//get CellsContinuous::Iterator from row begin to row end
	m.rows[r][c];
	//get CellsContinuous::Iterator from col begin to col end
	m.cols[c][r];
	//get CellsContinuous::Iterator from row begin to row end
	m.rows[r]->begin();
	m.rows[r]->end();
	//get CellsContinuous::Iterator from col begin to col end
	m.cols[r]->begin();
	m.cols[r]->end();
	//get CellsRegion::Iterator from first row begin to last row end. Iterator traverses rows
	m.rows->begin();
	m.rows->end();
	//get CellsRegion::Iterator from first col begin to last col end. Iterator traverses cols
	m.cols->begin();
	m.cols->end();
	//get Matrix::Index::Iterator from first row to last row
	m.rows.begin();
	m.rows.end();
	//get Matrix::Index::Iterator from first col to last col
	m.cols.begin();
	m.cols.end();
	//same as Matrix.rows[0]->begin()
	m.rows.begin()->begin();
	//same as Matrix.rows[0]->end()
	m.rows.end()->end();
	//Pitfall: Traversing these will not always give the desired effect (CellsContinuous iterators are not region aware)
	m.rows[0]->begin();
	m.rows[5]->end();
	//Traversing these will give the desired effect (CellsRegion iterators are region aware)	
	dynamic_cast<CellsRegion<Double>&>(m.rows[0].cells(citRegion)).begin();
	dynamic_cast<CellsRegion<Double>&>(m.rows[5].cells(citRegion)).end();
}

void matIter2() {
	//Create the matrix and the std::vector
	Matrix<Double> m = (dtDouble | 1.0 | 2.0 | 3.0) ^ (dtDouble | 4.0 | 5.0 | 6.0);
	vector<Double> v;	

	//Copy the entire matrix m to a std::vector v using std::copy
	v.resize(~m);
	copy(m.rows->begin(), m.rows->end(), v.begin());
	Matrix<Double>(v)++;
	
	//Copy the entire matrix m to a std::vector v using a for loop (but this time running over the columns which will be faster if the matrix is column major aligned)
	v.resize(~m);
	auto v_it = v.begin();
	for (auto m_it = m.cols->begin(); m_it != m.cols->end(); m_it++, v_it++)
	  *v_it = *m_it;
	Matrix<Double>(v)++;

	//Copy the first row to std::vector v using std::copy
	v.resize(m.cols.count());
	copy(m.rows[0]->begin(), m.rows[0]->end(), v.begin());
	Matrix<Double>(v)++;

	//Copy the second column as a Vector to std::vector v using std::copy
	v.resize(m.rows.count());
	copy(m[1].vec().begin(), m[1].vec().end(), v.begin());
	Matrix<Double>(v)++;

	//Copy the entire matrix m to a std::vector v using a for loop (but this time running over the rows which will be faster if the matrix is row major aligned)
	v.resize(~m);
	v_it = v.begin();
	for (auto m_it = m.rows->begin(); m_it != m.rows->end(); m_it++, v_it++)
	  *v_it = *m_it;
	Matrix<Double>(v)++;

	//Copy the entire matrix m to a std::vector v using two for loops (this will force the use of the faster CellsContinuous::Iterator)
	v_it = v.begin();
	for (auto idx_it = m.rows.begin(); idx_it != m.rows.end(); idx_it++)
	   for (auto c_it = idx_it->begin(); c_it != idx_it->end(); c_it++, v_it++)
		*v_it = *c_it;
	Matrix<Double>(v)++;

}

void tabStaticInit() {
	//Create a table with one matrix under key dtDouble/ctFeature
	Table<> a = ctFeature | ((dtDouble | 1.0 | 2.0 | 3.0) ^ (dtDouble | 4.0 | 5.0 | 6.0));
	//Create a table with two matrices under key dtDouble/ctFeature and dtString/ctTarget
	Table<> b = (ctFeature | ((dtDouble | 1.0 | 2.0 | 3.0) ^ (dtDouble | 4.0 | 5.0 | 6.0))) | (ctTarget | (dtString ^ "a" ^ "b"));
	a++;b++;

	Table<> c = (ctFeature | ((dtDouble | 1.0 | 2.0 | 3.0) ^ (dtDouble | 4.0 | 5.0 | 6.0))) | (ctTarget | (dtString ^ "a" ^ "b"));
	//Add column ids to a table
	(dtString | "feature1" | "feature2" | "feature3" | "target") ^ c;
	//Add row ids to a table
	(dtString | "sample1" | "sample2") | c; 
	c++;

	Table<> d = (dtString | "Feature1" | "Feature2") ^ (ctFeature | (((dtInt32|0)^1^2^3^4) | ((dtInt32|5)^6^7^8^9)));
	//Note the flipped "Feature1" and "Feature2"
	Table<> e = (dtString | "Feature2" | "Feature1") ^ (ctFeature | (((dtInt32|5)^6^7^8^9) | ((dtInt32|0)^1^2^3^4)));
	//When adding extra rows to table d from table e, columns with corresponding ids are merged.
	d ^ e;
	//This will fail because it would produce duplicate column ids.
	//d | e;
	d++;

	Table<> f = (dtString | "Row1"|"Row2"|"Row3"|"Row4"|"Row5") | (ctFeature | (((dtInt32|0)^1^2^3^4) | ((dtInt32|5)^6^7^8^9)));
	//Note the flipped "Row1" and "Row2"
	Table<> g = (dtString | "Row2"|"Row1"|"Row3"|"Row4"|"Row5") | (ctFeature | (((dtInt32|6)^5^7^8^9) | ((dtInt32|1)^0^2^3^4)));
	//When adding extra columns to table f from table g, rows with corresponding ids are merged.
	f | g;
	//This will fail because it would produce duplicate row ids.
	//f ^ g;
	f++;
}

void tableAssign() {
	Table<> a = (dtString | "Feature1" | "Feature2" | "Target") ^ ((ctFeature | ((dtInt32^0^1^2^3^4) | (dtInt32^5^6^7^8^9))) | (ctTarget | (dtString^"a"^"a"^"b"^"b"^"c")));
	//This will slice the columns by key (content type and data type). Because this is guaranteed to produce a table with one matrix it can be assigned to a new matrix (implicit cast).
	Matrix<Int32> b = a(ctFeature, dtInt32);
	//Because the matrix inside the table is only referenced, this will assign the value 5 to the top 3 values of each of the two features inside the table a.
	b(0,3,0,2) = 5;
	a++;
	//Assign a to c (creates a reference)
	Table<> c = a;
	//For copy of a to c (just like Matrices)
	!c;
	//Duplicate a and then assign to c (just like Matrices).
	c = *a;
}

void testPNG() {
	auto mat = (dtImagePNGDouble | ImagePNG<Double>(String("test1.png")));
	mat++;
	mat.write("tab.ddsl");
	Matrix<ImagePNG<Double>> mat2;
	mat2 << "tab.ddsl";
	mat2.val(0,0).setFilename("test2.png");
	mat2.val(0,0).saveImage();
	mat2++;
}

int main(int argc, char **) {

	try {
		//testLeafsCaffe();
		testGauss();
		testTableTest();
		testTableArith();
		testTableStream();
		testModel();
		testModel2();
		testModelWrite();
		testAsyncPipe();
		testAsyncPipe();
		testAsyncModel();
		moveAndCopyPipe();
		slicingPipe();
		rwPipe();
		pipelineExec();
		preparePipe();
		dontPreparePipe();
		advanced();
		matrixTest();
		matrixTest2();
		matrixTest3();
		matrixTest4();
		matrixTest5();
		matrixTestTest1();
		matrixSetLogic();
		matrixArith();
		matrixCount();
		matPrinting();
		matStream();
		matIter();
		matIter2();
		tabStaticInit();
		tableAssign();
		testPNG();
	} catch (Error &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}
