#pragma once
#include "DS_Model.h"

#include "ArrayFireDDSL.h"

namespace DSModel {
	namespace ArrayFire {
		template<typename TClassType = DSTypes::UInt32, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
		class Correlation : public Model<TIdx, TId> {
		private:
			DSLib::Matrix<Float, TIdx> patterns_; //in order of the classes param
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
			using Model<TIdx, TId>::train;
			using Model<TIdx, TId>::apply;

			DSLib::Matrix<Float, TIdx> patterns() const {return patterns_;}

			Correlation(const DSLib::Matrix<TClassType, TIdx> &classes);
			Correlation(const DSTypes::String &filename);

			M_HDR_MODEL_DESTRUCTOR(Correlation)
			M_HDR_MODEL_CONSTRUCTOR_NO_FILE(Correlation, TClassType, TIdx, TId)
			M_HDR_MODEL_CLONE(Correlation, TClassType, TIdx, TId)
			M_HDR_MODEL_ASSIGNMENT(Correlation, TClassType, TIdx, TId)
			M_HDR_MODEL_OPERATOR(Correlation, TClassType, TIdx, TId)
		};
	}
}