#pragma once

#include "h/DS_Types.h"
#include "h/DS_Util.h"
#include "h/lib/DS_Table.h"
#include "h/lib/DS_TableFunc.h"
#include "h/lib/DS_Matrix.h"
#include "h/lib/DS_MatrixFunc.h"
#include "h/lang/DS_TableLang.h"
#include "h/lang/DS_MatrixLang.h"

namespace DSModel {	
	
	namespace {
		class ProgressBar {
		private:		
			DSTypes::Double min_;
			DSTypes::Double max_;
			DSTypes::Double currentProgress_;
			DSLib::Matrix<DSTypes::String> stages_;
			DSTypes::UInt32 currentStage_;
			DSTypes::String head_;
			DSTypes::String tail_;
			DSTypes::String full_;
			DSTypes::String empty_;
			DSTypes::UInt32 len_;
			DSTypes::String old_;
			std::mutex lock_;
			void init_(DSTypes::Double min, DSTypes::Double max, const DSLib::Matrix<DSTypes::String> &stages);
		public:
			void clone(const ProgressBar &other);
			void clone(ProgressBar &&other);
			ProgressBar(const ProgressBar &other) {clone(other);};
			ProgressBar(ProgressBar &&other) {clone(std::move(other));};
			ProgressBar & operator=(const ProgressBar &other) {clone(other);return *this;};
			ProgressBar & operator=(ProgressBar &&other) {clone(std::move(other));return *this;};

			ProgressBar();
			ProgressBar(DSTypes::Double min, DSTypes::Double max, DSTypes::UInt32 stages=0);
			ProgressBar(DSTypes::Double min, DSTypes::Double max, const DSLib::Matrix<DSTypes::String> &stages);
			void addStages(const DSLib::Matrix<DSTypes::String> &stages);

			void setStage(DSTypes::UInt32 stage);
			void setStage(const DSTypes::String &stage);
			DSTypes::String getStage();
			DSTypes::UInt32 getCurrentStage();
			DSTypes::UInt32 getStageCount();

			void min(DSTypes::Double &min);
			void max(DSTypes::Double &max);
			DSTypes::Double min();
			DSTypes::Double max();
		
			void clear();
			void firstStage();
			void lastStage();
			void nextStage();
			void start();
			void done();
			void stageDone();
			bool isDone();
			void progress(DSTypes::Double);
			void progress(DSTypes::Double min, DSTypes::Double max, DSTypes::Double progress);
			void incProgress(DSTypes::Double amount = 1);
		
			DSTypes::Double progress();
			void print(std::ostream &os);
			void print(DSTypes::Double min, DSTypes::Double max, DSTypes::Double progress, std::ostream &os);
			DSTypes::String print();
		};
	};

	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Model : public DSLib::Table<TIdx, TId> {
	private:		
		ProgressBar progress_;

		DSLib::Matrix<DSTypes::ContentType> inputContentTypes_;
		DSLib::Matrix<DSTypes::DataType> inputDataTypes_;		

		DSLib::Matrix<TIdx, TIdx> inputIndices_;
		DSLib::Matrix<TIdx, TIdx> outputIndices_;

		DSLib::Table<TIdx, TId> inputSlice_; //contains the main input slice
		DSLib::Table<TIdx, TId> outputSlice_; //contains the main output slice

		DSTypes::PassThroughType ptt_;
		DSTypes::ExecType execType_;

		DSLib::Table<TIdx, TId> srcTable_;
		
		Model<TIdx, TId> * next_;
		Model<TIdx, TId> * prev_;
		TIdx sampleCount_;
		Double timeTrain_, timeApply_;

		//cols = ParamName1, ParamName2 ..., rows = Value, Min, Max, Increment, Perm1, Perm2, ...
		DSLib::Table<TIdx, DSTypes::Literal> parms_;

		void print_(std::ostream &output, const Model<TIdx, TId> &printer) const;
		void printLess_(std::ostream &output) const;

		TIdx count_(TIdx &count) const;
		TIdx modelIndex_(TIdx &idx) const;

		Model<TIdx, TId> & copy_(TIdx count, Model<TIdx, TId> &dst);
		void resetAll_();
		void clearAll_();
		bool prepare_ = true;
		bool inputAll_ = false;

		std::future<void> future_;
		
		void printProgress_(std::ostream &output);
		void train_(const DSLib::Table<TIdx, TId> &table);
		void train_(const DSLib::Table<TIdx, TId> &table, const Model<TIdx, TId> &till);
		void train_(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output);
		void apply_(const DSLib::Table<TIdx, TId> &table);
		void apply_(const DSLib::Table<TIdx, TId> &table, const Model<TIdx, TId> &till);
	    void apply_(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output);
	protected:
		virtual void updateParameters() {}; //Parameter update event
		void registerParameters(const DSLib::Table<TIdx, DSTypes::Literal> &parms);		
		template<typename T> T & parameterValueById(const DSTypes::Literal &id) {return parms_[id].template mat<T>(0).vec(0);};
		template<typename T> T & parameterMinById(const DSTypes::Literal &id) { return parms_[id].template mat<T>(0).vec(1);};
		template<typename T> T & parameterMaxById(const DSTypes::Literal &id) { return parms_[id].template mat<T>(0).vec(2);};		
		template<typename T> T & parameterIncById(const DSTypes::Literal &id) { return parms_[id].template mat<T>(0).vec(3);};
		template<typename T> DSLib::Matrix<T, TIdx> & parameterPermsById(const DSTypes::Literal &id) {return parms_(4, parms_.rows.count()-4, id).template mat<T>(0);};
		DSLib::Table<TIdx, DSTypes::Literal> & parameters() {return parms_;};

		void registerOutput(DSLib::Table<TIdx, TId> &proto);
		void registerInput(const DSTypes::DataType dt, const DSTypes::ContentType ct); //Only strict dt/ct pair, can be called multiple times
		void registerInput(const DSTypes::DataType dt, const DSLib::Matrix<DSTypes::ContentType> cts); //Strict dt, but any of the cts
		void registerInput(const DSLib::Matrix<DSTypes::DataType> dts, const DSTypes::ContentType ct); //strict ct, but any of the dts
		void registerInput(const DSLib::Matrix<DSTypes::ContentType> cts); //Any of cts, any dt
		void registerInput(const DSLib::Matrix<DSTypes::DataType> dts); //Any of dts, any ct

		void registerInput(const DSTypes::ContentType ct); //strict ct, but any dt
		void registerInput(const DSTypes::DataType ct); //strict dt, but any ct
		void registerInputAll(); //Take any column as input

		virtual void registerInputs(const DSLib::Table<TIdx, TId> &table) = 0;
		virtual void registerOutputs(const DSLib::Table<TIdx, TId> &input) = 0;
		virtual void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) = 0; //Called on train() (Source Table, Input and output slice are passed)
		virtual void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) = 0; //called on apply() (Source Table, Input and output slice are passed)

		TIdx & sampleCount() {return sampleCount_;};
		void sampleCount(TIdx sampleCount) {sampleCount_ = sampleCount;};		

		void addStages(const DSLib::Matrix<DSTypes::String> &stages);
		void setMinProgress(DSTypes::Double min);
		void setMaxProgress(DSTypes::Double max);
		void setStage(const DSTypes::String &stage);
		void setStage(const DSTypes::String &stage, DSTypes::Double min, DSTypes::Double max);
		void setStage(const DSTypes::String &stage, DSTypes::Double min, DSTypes::Double max, DSTypes::Double progress);

		void nextStage();
		void setStageDone();
		void incProgress(DSTypes::Double amount = 1);
		void setProgress(DSTypes::Double progress);
		ProgressBar & progress() {return progress_;};

		void readParams(const DSTypes::String &filename);
		void readParams(std::istream &input);
		void writeParams(const DSTypes::String &filename);
		void writeParams(std::ostream &output);

		virtual void resetModel() {};
		virtual void writeModel(std::ostream &output, const DSTypes::String &filename) {};
		virtual void readModel(std::istream &input, const DSTypes::String &filename) {};
	public:
		Model();
		Model(const DSLib::Table<TIdx, DSTypes::Literal> &parms);
		Model(const Model<TIdx, TId> &other);
		Model(Model<TIdx, TId> &&other);
		
		template<typename T> void setParameter(const DSTypes::Literal &id, const T &value) {
			parameterValueById<T>(id) = value;
			updateParameters();
		}
		template<typename T> T & getParameter(const DSTypes::Literal &id) {
			return parameterValueById(id);
		}
		
		Model<TIdx, TId> & operator=(const Model<TIdx, TId> &other);
		Model<TIdx, TId> & operator=(Model<TIdx, TId> &&other);
		virtual void clone(const Model<TIdx, TId> &other);
		virtual void clone(Model<TIdx, TId> &&other);
		virtual Model<TIdx, TId> * clone() const = 0; //Copy *this to a new pointer
		virtual Model<TIdx, TId> * move() = 0; //Move *this to new pointer

		void read(const DSTypes::String &filename);
		void write(const DSTypes::String &filename);

		Model<TIdx, TId> & next() {return *next_;};
		Model<TIdx, TId> & prev() { return *prev_;};
		void next(const Model<TIdx, TId> & next); //Shallow copy
		void next(Model<TIdx, TId> && next); //Deep move
		void prev(const Model<TIdx, TId> & prev); //Shallow copy
		void prev(Model<TIdx, TId> && prev); //Deep move
		Model<TIdx, TId> & last();
		Model<TIdx, TId> & first();
		Model<TIdx, TId> & next(TIdx index);
		Model<TIdx, TId> & copy(TIdx count, Model<TIdx, TId> &dst);
		
		TIdx count() const;
		Double timeApply() const {return timeApply_;};
		Double timeAllApply();

		Double timeTrain() const {return timeTrain_;};
		Double timeAllTrain();

		DSLib::Table<TIdx, TId> & tab() {return *this;}
		void wait();
		bool wait(DSTypes::UInt32 ms);
		void progressLoop(std::ostream &output=std::cout, DSTypes::UInt32 updateTimeMs=100);
		void reset();
		void resetAll();

		virtual ~Model();

		void print(std::ostream &output) const;
		void printLess(std::ostream &output) const;

		void printProgress(std::ostream &output);

		DSTypes::String print() const;
		virtual DSTypes::String modelName() const;
		TIdx modelIndex() const;

		virtual void clear() override;
		virtual void clearAll() override;

		Table<TIdx, TId> & checkInput(Table<TIdx, TId> &tab, const String &name);
		Table<TIdx, TId> && checkInput(Table<TIdx, TId> &&tab, const String &name);
		Table<TIdx, TId> & checkOutput(Table<TIdx, TId> &tab, const String &name);
		Table<TIdx, TId> && checkOutput(Table<TIdx, TId> &&tab, const String &name);
		
		void prepareOutput(DSLib::Table<TIdx, TId> &srcTable);
		void prepareInput(DSLib::Table<TIdx, TId> &srcTable);
		virtual void prepare(const DSLib::Table<TIdx, TId> &table);

		void passThroughType(DSTypes::PassThroughType ptt){ptt_=ptt;};
		DSTypes::PassThroughType passThroughType() {return ptt_;};

		void execType(DSTypes::ExecType execType) { execType_ = execType; };
		DSTypes::ExecType execType() { return execType_; };

		DSLib::Table<TIdx, TId> & train(const DSLib::Table<TIdx, TId> &table);
		DSLib::Table<TIdx, TId> & train(const DSLib::Table<TIdx, TId> &table, const Model<TIdx, TId> &till);
		DSLib::Table<TIdx, TId> & apply(const DSLib::Table<TIdx, TId> &table);
		DSLib::Table<TIdx, TId> & apply(const DSLib::Table<TIdx, TId> &table, const Model<TIdx, TId> &till);

		//DSLang
		Model<TIdx, TId> & operator[](const TIdx index);
	};

	//HDR FILE MACROS
	#define M_HDR_MODEL_DESTRUCTOR_DEFAULT(NAME) ~NAME();
	#define M_HDR_MODEL_DESTRUCTOR(NAME)	M_HDR_MODEL_DESTRUCTOR_DEFAULT(NAME)
	
	#define M_HDR_MODEL_CONSTRUCTOR_DEFAULT(NAME) NAME();
	#define M_HDR_MODEL_CONSTRUCTOR_COPY(NAME, ...) NAME(const NAME<__VA_ARGS__> &other);
	#define M_HDR_MODEL_CONSTRUCTOR_MOVE(NAME, ...) NAME(NAME<__VA_ARGS__> &&other);
	#define M_HDR_MODEL_CONSTRUCTOR_MODEL_COPY(NAME) NAME(const Model<TIdx, TId> &other);
	#define M_HDR_MODEL_CONSTRUCTOR_MODEL_MOVE(NAME) NAME(Model<TIdx, TId> &&other);
	#define M_HDR_MODEL_CONSTRUCTOR_FILE(NAME) NAME(const DSTypes::String &filename);
	#define M_HDR_MODEL_CONSTRUCTOR(NAME, ...)	M_HDR_MODEL_CONSTRUCTOR_DEFAULT(NAME) \
												M_HDR_MODEL_CONSTRUCTOR_COPY(NAME, __VA_ARGS__) \
												M_HDR_MODEL_CONSTRUCTOR_MOVE(NAME, __VA_ARGS__) \
												M_HDR_MODEL_CONSTRUCTOR_MODEL_COPY(NAME) \
												M_HDR_MODEL_CONSTRUCTOR_MODEL_MOVE(NAME) \
												M_HDR_MODEL_CONSTRUCTOR_FILE(NAME)
	#define M_HDR_MODEL_CONSTRUCTOR_NO_FILE(NAME, ...)	M_HDR_MODEL_CONSTRUCTOR_DEFAULT(NAME) \
														M_HDR_MODEL_CONSTRUCTOR_COPY(NAME, __VA_ARGS__) \
														M_HDR_MODEL_CONSTRUCTOR_MOVE(NAME, __VA_ARGS__) \
														M_HDR_MODEL_CONSTRUCTOR_MODEL_COPY(NAME) \
														M_HDR_MODEL_CONSTRUCTOR_MODEL_MOVE(NAME)

	#define M_HDR_MODEL_CONSTRUCTOR_NO_FILE_NO_DEFAULT(NAME, ...)	M_HDR_MODEL_CONSTRUCTOR_COPY(NAME, __VA_ARGS__) \
																	M_HDR_MODEL_CONSTRUCTOR_MOVE(NAME, __VA_ARGS__) \
																	M_HDR_MODEL_CONSTRUCTOR_MODEL_COPY(NAME) \
																	M_HDR_MODEL_CONSTRUCTOR_MODEL_MOVE(NAME)

	#define M_HDR_MODEL_ASSIGNMENT_COPY(NAME, ...) NAME<__VA_ARGS__> & operator=(const NAME<__VA_ARGS__> &other);
	#define M_HDR_MODEL_ASSIGNMENT_MOVE(NAME, ...) NAME<__VA_ARGS__> & operator=(NAME<__VA_ARGS__> &&other);
	#define M_HDR_MODEL_ASSIGNMENT(NAME, ...)	M_HDR_MODEL_ASSIGNMENT_COPY(NAME, __VA_ARGS__) \
												M_HDR_MODEL_ASSIGNMENT_MOVE(NAME, __VA_ARGS__)

	#define M_HDR_MODEL_CLONE_COPY(NAME, ...) void clone(const NAME<__VA_ARGS__ > &other);
	#define M_HDR_MODEL_CLONE_MOVE(NAME, ...) void clone(NAME<__VA_ARGS__> &&other);
	#define M_HDR_MODEL_CLONE_MODEL_COPY Model<TIdx, TId> * clone() const override;
	#define M_HDR_MODEL_CLONE_MODEL_MOVE Model<TIdx, TId> * move() override;
	#define M_HDR_MODEL_CLONE_MODEL_COPY_2 void clone(const Model<TIdx, TId> & other) override;
	#define M_HDR_MODEL_CLONE_MODEL_MOVE_2 void clone(Model<TIdx, TId> && other) override;
	#define M_HDR_MODEL_CLONE(NAME, ...)	M_HDR_MODEL_CLONE_COPY(NAME, __VA_ARGS__) \
											M_HDR_MODEL_CLONE_MOVE(NAME, __VA_ARGS__) \
											M_HDR_MODEL_CLONE_MODEL_COPY \
											M_HDR_MODEL_CLONE_MODEL_MOVE \
											M_HDR_MODEL_CLONE_MODEL_COPY_2 \
											M_HDR_MODEL_CLONE_MODEL_MOVE_2

	#define M_HDR_MODEL_OPERATOR_SLICE(NAME, ...)	NAME<__VA_ARGS__>  operator()(const TIdx count = 0);
	#define M_HDR_MODEL_OPERATOR(NAME, ... )	M_HDR_MODEL_OPERATOR_SLICE(NAME, __VA_ARGS__)

	//CPP FILE MACROS
	#define M_CPP_MODEL_DESTRUCTOR_DEFAULT(NAME, ...) NAME<__VA_ARGS__>::~NAME() {}

	#define M_CPP_MODEL_CONSTRUCTOR_DEFAULT(NAME, ...) NAME<__VA_ARGS__>::NAME() {}
	#define M_CPP_MODEL_CONSTRUCTOR_COPY(NAME, ...) NAME<__VA_ARGS__>::NAME(const NAME<__VA_ARGS__> & other) {clone(other);}
	#define M_CPP_MODEL_CONSTRUCTOR_MOVE(NAME, ...) NAME<__VA_ARGS__>::NAME(NAME<__VA_ARGS__> && other) {clone(std::move(other));}
	#define M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(NAME, ...) NAME<__VA_ARGS__>::NAME(const Model<TIdx, TId>& other) {try {clone(dynamic_cast<const NAME<__VA_ARGS__> &>(other));}catch (exception &e) {throw Error(ecGeneral, "In copy constructor with Model<>", e.what());}}
	#define M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(NAME, ...) NAME<__VA_ARGS__>::NAME(Model<TIdx, TId>&& other) {try {clone(std::move(dynamic_cast<NAME<__VA_ARGS__> &>(other)));}catch (exception &e) {throw Error(ecGeneral, "In move constructor with Model<>", e.what());}}
	#define M_CPP_MODEL_CONSTRUCTOR_FILE(NAME, ...) NAME<__VA_ARGS__>::NAME(const DSTypes::String &filename) {Model<TIdx, TId>::read(filename);}

	#define M_CPP_MODEL_ASSIGNMENT_COPY(NAME, ...)	NAME<__VA_ARGS__> & NAME<__VA_ARGS__>::operator=(const NAME<__VA_ARGS__> & other) {clone(other);return *this;}
	#define M_CPP_MODEL_ASSIGNMENT_MOVE(NAME, ...)	NAME<__VA_ARGS__> & NAME<__VA_ARGS__>::operator=(NAME<__VA_ARGS__> && other) {clone(std::move(other));return *this;}

	#define M_CPP_MODEL_CLONE_MODEL_COPY(NAME, ...) Model<TIdx, TId>* NAME<__VA_ARGS__>::clone() const {auto * m = new NAME<__VA_ARGS__>();m->clone(*this);return m;}
	#define M_CPP_MODEL_CLONE_MODEL_MOVE(NAME, ...) Model<TIdx, TId>* NAME<__VA_ARGS__>::move() {auto * m = new NAME<__VA_ARGS__>();m->clone(std::move(*this));return m;}

	#define M_CPP_MODEL_CLONE_MODEL_COPY_2(NAME, ...) void NAME<__VA_ARGS__>::clone(const Model<TIdx, TId> & other) {try {clone(dynamic_cast<const NAME<__VA_ARGS__> &>(other));}catch (exception &e) {throw Error(ecGeneral, "In clone with Model<>", e.what());}}
	#define M_CPP_MODEL_CLONE_MODEL_MOVE_2(NAME, ...) void NAME<__VA_ARGS__>::clone(Model<TIdx, TId> && other) {try {clone(std::move(dynamic_cast<NAME<__VA_ARGS__> &>(other)));}catch (exception &e) {throw Error(ecGeneral, "In clone with Model<>", e.what());}}

	#define M_CPP_MODEL_CLONE_COPY(NAME, ...) void NAME<__VA_ARGS__>::clone(const NAME<__VA_ARGS__> & other) {Model<TIdx, TId>::clone(other);}
	#define M_CPP_MODEL_CLONE_MOVE(NAME, ...) void NAME<__VA_ARGS__>::clone(NAME<__VA_ARGS__> && other) {Model<TIdx, TId>::clone(std::move(other));}

	#define M_CCP_MODEL_OPERATOR_SLICE(NAME, ...) NAME<__VA_ARGS__> NAME<__VA_ARGS__>::operator()(const TIdx count) {NAME<__VA_ARGS__> m;this->copy(count, m);	return std::move(m);}

};

