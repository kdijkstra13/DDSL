#pragma once
#include "DS_Model.h"

#include "caffe/caffe.hpp"

namespace DSTypes {

	//    - fixed: always return base_lr.
	//    - step: return base_lr * gamma ^ (floor(iter / step))
	//    - exp: return base_lr * gamma ^ iter
	//    - inv: return base_lr * (1 + gamma * iter) ^ (- power)
	//    - multistep: similar to step but it allows non uniform steps defined by
	//      stepvalue
	//    - poly: the effective learning rate follows a polynomial decay, to be
	//      zero by the max_iter. return base_lr (1 - iter/max_iter) ^ (power)
	//    - sigmoid: the effective learning rate follows a sigmod decay
	//      return base_lr ( 1/(1 + exp(-gamma * (iter - stepsize))))
	enum LearnRatePolicy {lrpFixed, lrpStep, lrpExp, lrpInv, lrpMultistep, lrpPoly, lrpSigmoid};
	enum UnitType {utTanH, utSigmoid, utReLU};
	enum SolverType {stSGD, stAdaDelta, stAdaGrad, stAdam, stNesterov, stRMSProp};
	enum Backend  { beLMDB, beLevelDB };
	enum DBMode { dbmAppend, dbmNew, dbmAppendOrNew };
}

namespace DSFunc {
	const int CHUNK_BYTES = 100 * 1024 * 1024; //100 MB
	typedef function<void(DSTypes::Double min, DSTypes::Double max, DSTypes::Double progress)> ProgressCallback;

	//Images (Channel aware, deferred loading)
	template <typename T, typename TInt, template <typename, typename> class TImg>
	void writeImageDB(DSLib::Matrix<TImg<T, TInt>, TInt> &images, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback = [](Double min, Double max, Double progress){}, const DSTypes::DBMode mode = dbmNew, const DSTypes::Backend be = beLMDB);
	
	template <typename T, typename TInt, template <typename, typename> class TImg>
	void readImageDB(DSLib::Matrix<TImg<T, TInt>, TInt> &images, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const DSTypes::String &cacheFolder = "", const ProgressCallback &progressCallback = [](Double min, Double max, Double progress){}, const DSTypes::Backend be = beLMDB);

	//Regular matrices for arbitrary 2D data
	template <typename T, typename TInt, template <typename, typename> class TMat>
	void writeDB(DSLib::Matrix<TMat<T, TInt>, TInt> &matrices, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filenam, const ProgressCallback &progressCallback = [](Double min, Double max, Double progress){}, const DSTypes::DBMode mode = dbmNew, const DSTypes::Backend be = beLMDB);

	template <typename T, typename TInt, template <typename, typename> class TMat>
	void readDB(DSLib::Matrix<TMat<T, TInt>, TInt> &matrices, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback = [](Double min, Double max, Double progress){}, const DSTypes::Backend be = beLMDB);

	//Regular matrices for arbitrary 1D data
	template <typename T, typename TInt>
	void writeDB(DSLib::Matrix<T, TInt> &mat, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback = [](Double min, Double max, Double progress){}, const DSTypes::DBMode mode = dbmNew, const DSTypes::Backend be = beLMDB);

	template <typename T, typename TInt>
	void readDB(DSLib::Matrix<T, TInt> &mat, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback = [](Double min, Double max, Double progress){}, const DSTypes::Backend be = beLMDB);

}

namespace DSModel {

	//**************//
	//** ExportDB **//
	//**************//
	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class ExportDB : public Model<TIdx, TId> {
	private:
		void getData_(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Matrix<Float, TIdx> &outputFloat, Matrix<Matrix<Float, TIdx>, TIdx> &outputMatrixFloat, Matrix<ImagePNG<Float, TIdx>, TIdx> &outputImagePNGFloat, Matrix<Float, TIdx> &outputTargetFloat);
	protected:
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;

		void readModel(std::istream &input, const DSTypes::String &filename) override;
		void writeModel(std::ostream &output, const DSTypes::String &filename) override;
	public:
		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;

		void resetModel();
		ExportDB(const DSTypes::String &filename);
		ExportDB(const string &filename, DSTypes::Backend be);

		M_HDR_MODEL_DESTRUCTOR(ExportDB)
		M_HDR_MODEL_CONSTRUCTOR_NO_FILE(ExportDB, TIdx, TId)
		M_HDR_MODEL_CLONE(ExportDB, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(ExportDB, TIdx, TId)
		M_HDR_MODEL_OPERATOR(ExportDB, TIdx, TId)
	};

	template<typename TClassType = DSTypes::UInt32, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Caffe : public Model<TIdx, TId> {
	private:
		
		Matrix<TIdx, TIdx> blobWidth_;
		Matrix<TIdx, TIdx> blobHeight_;
		Matrix<TIdx, TIdx> blobChannels_;
		Matrix<String, TIdx> blobName_;
		Matrix<Matrix<Float, TIdx>, TIdx> blobData_;

		Matrix<String, TIdx> memoryDataLayerName_;
		Matrix<String, TIdx> dataBlobName_;
		Matrix<String, TIdx> labelBlobName_;

		TIdx batchSize_;

		map<TClassType, DSTypes::Float> classToNum_;
		Matrix<TClassType> classes_;		
		
		//TIdx blobWidth_;
		//TIdx blobHeight_;
		//TIdx blobChannels_;
		//Matrix<Float, TIdx> blobData_;

		caffe::Solver<DSTypes::Float> * solver_;
		caffe::Net<DSTypes::Float> * net_;		
		TIdx gpuDevices_;
		TIdx iterDone_;

		DSTypes::String solverProtoFile_;
		DSTypes::String netProtoFile_;
		DSTypes::String snapshotModelFile_;

		void freeNet_(caffe::Net<Float>** net);
		void freeSolver_(caffe::Solver<Float>** net);

		caffe::Solver<Float> * readSolver_(const String &filename, const String snapshot = "");
		caffe::Net<Float> * readNet_(const DSTypes::String &filename, const DSTypes::String snapshot = "");
		void shareWeights_(caffe::Net<Float> &netFrom, caffe::Net<Float> &netTo);
		void copyWeights_(const caffe::Net<Float> &netFrom, caffe::Net<Float> &netTo);

		void loadCaffeModel_(caffe::Net<Float> **net, caffe::Solver<Float> **solver);
		void init_();

		void parseInputName_(const std::string &name, ContentType &ct, DataType &dt);
		void parseOutputName_(const std::string &name, ContentType &ct, DataType &dt);
		void parseSecondaryOutputName_(const std::string &name, ContentType &ct, DataType &dt);
		
		boost::shared_ptr<caffe::Net<Float>> activeNet_;
		void setActiveNet_(boost::shared_ptr<caffe::Net<Float>> &n);
		boost::shared_ptr<caffe::Net<Float>> getActiveNet_();

		template<typename T> void addBlobData_(const String &blobName, Matrix<T, TIdx> &in);
		template<typename T> void addBlobData_(const String &blobName, Matrix<ImagePNG<T>, TIdx> &in);
		template<typename T> void addBlobData_(const String &blobName, Matrix<Matrix<T>, TIdx> &in);
		void addLabelBlobData_(const String &blobName, Matrix<TClassType, TIdx> &in);

		template<typename T> void getBlobData_(const String &blobName, Matrix<T, TIdx> &out);
		template<typename T> void getBlobData_(const String &blobName, Matrix<ImagePNG<T, TIdx>, TIdx> &out);
		template<typename T> void getBlobData_(const String &blobName, Matrix<Matrix<T, TIdx>, TIdx> &out);
		template<typename T> void getResultBlobData_(const String &blobName, Matrix<TClassType, TIdx> &result);
		template<typename T> void getResultBlobData_(const String &blobName, Matrix<TClassType, TIdx> &result, Matrix<T, TIdx> &conf);		

		void setMemoryDataInput_(Table<TIdx, TId> &input, const String &layerName, const String &dataName, const String &labelName, const ContentType dataCT, const DataType dataDT, const ContentType labelCT, const DataType labelDT);
		void getLayerOutput_(Table<TIdx, TId> &output, const String &blobName, const ContentType ct, const DataType dt);

		void clearInputData_();
		void setInputData_(Table<TIdx, TId> &input);		
		void getOutputData_(Table<TIdx, TId> &output);

		Float * getBlobDataByName_(const String &name);
		void fillMemoryDataLayer_();
	protected:
		void updateParameters() override;
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void readModel(std::istream &input, const DSTypes::String &filename) override;
		void writeModel(std::ostream &output, const DSTypes::String &filename) override;
	public:
		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;

		void resetModel() override;

		void loadCaffeModel();
		void clearCaffeModel();
		void updateCaffeModel(bool copyWeights=true);

		//caffe::Caffe::Brew & mode() const { return mode_; }
		
		Caffe(const DSLib::Matrix<TClassType, TIdx> &classes, const DSTypes::String netProtoFile, const DSTypes::String solverProtoFile, TIdx gpuDevices = 0);
		Caffe(const DSTypes::String &filename, TIdx gpuDevices = 0, const DSTypes::String snapshotModelFile = "");

		M_HDR_MODEL_DESTRUCTOR(Caffe)
		M_HDR_MODEL_CONSTRUCTOR_NO_FILE(Caffe, TClassType, TIdx, TId)
		M_HDR_MODEL_CLONE(Caffe, TClassType, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(Caffe, TClassType, TIdx, TId)
		M_HDR_MODEL_OPERATOR(Caffe, TClassType, TIdx, TId)
	};

	template<typename TClassType = DSTypes::UInt32, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class CaffeMLP : public Model<TIdx, TId> {
	private:
		caffe::Solver<DSTypes::Double> * solver_;
		caffe::Net<DSTypes::Double> * net_;
		TIdx gpuDevices_;
		DSTypes::Double learnRate_;
		DSTypes::Double momentum_;
		DSLib::Matrix<DSTypes::UInt32> units_;
		TIdx iter_;
		TIdx iterDone_;
		TIdx batchSize_;
		TIdx featureCount_;
		DSTypes::LearnRatePolicy lrp_;
		Double gamma_;
		Double weightDecay_;
		Double power_;
		DSTypes::UnitType unitType_;
		DSTypes::SolverType solverType_;
		DSLib::Matrix<DSTypes::UInt32> stepValue_;
		DSTypes::UInt32 stepSize_;

		DSLib::Matrix<TClassType> classes_;
		std::map<TClassType, DSTypes::Double> classToNum_;

		DSTypes::String solverProto_;
		DSTypes::String netProto_;
		void genSolverProto_();
		void genNetProto_();
		void createSolver_();
		void createSolverFromFile_(const DSTypes::String &filename);
		void createNet_();
		void createNetFromSolver_();
		void createNetFromFile_(const DSTypes::String &filename);

		void writeSolverToFile_(const DSTypes::String &filename);
		void writeNetToFile_(const DSTypes::String &filename);

		void init_();
	protected:
		void updateParameters() override;
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void readModel(std::istream &input, const DSTypes::String &filename) override;
		void writeModel(std::ostream &output, const DSTypes::String &filename) override;
	public:
		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;

		void resetModel() override;

		void clearCaffeModel();
		void updateCaffeModel(bool copyWeights=true);

		//caffe::Caffe::Brew mode() const { return mode_; }
		//void mode(caffe::Caffe::Brew mode) { mode_ = mode; }

		void readModel(const DSTypes::String &filename, bool loadSolver, bool loadNet);
	
		CaffeMLP(const DSLib::Matrix<TClassType, TIdx> &classes, TIdx gpuDevices = 0, DSTypes::Double learnRate = 0.001, DSTypes::Double momentum = 0.01, Matrix<UInt32> units = (dtUInt32|100), TIdx iter = 1000, TIdx batchSize = 10, DSTypes::LearnRatePolicy lrp = lrpFixed, TIdx stepSize = 100, Double gamma = 0.1, Double power = 1, Double weightDecay = 0, DSTypes::UnitType unitType = utTanH, DSTypes::SolverType solverType_ = stSGD, Matrix<UInt32> stepValue_ = (dtUInt32|0));
		CaffeMLP(const DSTypes::String &filename, TIdx gpuDevices = 0, bool loadSolver = false, bool loadNet = true); //indicate if the net and solver state should be loaded

		M_HDR_MODEL_DESTRUCTOR(CaffeMLP)
		M_HDR_MODEL_CONSTRUCTOR_NO_FILE(CaffeMLP, TClassType, TIdx, TId)
		M_HDR_MODEL_CLONE(CaffeMLP, TClassType, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(CaffeMLP, TClassType, TIdx, TId)
		M_HDR_MODEL_OPERATOR(CaffeMLP, TClassType, TIdx, TId)
	};

}

