#pragma once
#include "DS_Model.h"

namespace DSModel {
	template<typename TClassType = DSTypes::UInt32, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Random : public Model<TIdx, TId> {
	protected:
		void updateParameters() override;
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
	public:
		virtual DSTypes::String modelName() const override {return "Random";}

		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;

		Random(const DSLib::Matrix<TClassType, TIdx> &classes);		
				
		M_HDR_MODEL_DESTRUCTOR(Random)
		M_HDR_MODEL_CONSTRUCTOR(Random, TClassType, TIdx, TId)
		M_HDR_MODEL_CLONE(Random, TClassType, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(Random, TClassType, TIdx, TId)
		M_HDR_MODEL_OPERATOR(Random, TClassType, TIdx, TId)
	};

	template<typename TClassType = DSTypes::UInt32, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class NaiveBayes : public Model<TIdx, TId> {
	private:
		std::map<TClassType, DSLib::Matrix<DSTypes::Double, TIdx>> mean_;
		std::map<TClassType, DSLib::Matrix<DSTypes::Double, TIdx>> var_;
		std::map<TClassType, DSTypes::Double> prior_;

		DSTypes::Double classify(TClassType &res, const DSLib::CellsContinuous<DSTypes::Double, TIdx> &sample);
	protected:
		void updateParameters() override;
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;

		void resetModel();
		void readModel(std::istream &input, const DSTypes::String &filename) override;
		void writeModel(std::ostream &output, const DSTypes::String &filename) override;
	public:
		virtual DSTypes::String modelName() const override {return "NaiveBayes";}

		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;

		NaiveBayes(const DSLib::Matrix<TClassType, TIdx> &classes);
		NaiveBayes(const DSTypes::String &filename);

		M_HDR_MODEL_DESTRUCTOR(NaiveBayes)
		M_HDR_MODEL_CONSTRUCTOR_NO_FILE(NaiveBayes, TClassType, TIdx, TId)
		M_HDR_MODEL_CLONE(NaiveBayes, TClassType, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(NaiveBayes, TClassType, TIdx, TId)
		M_HDR_MODEL_OPERATOR(NaiveBayes, TClassType, TIdx, TId)
	};

}