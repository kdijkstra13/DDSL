#pragma once
#include "DS_Model.h"

namespace DSModel {
	namespace ArrayFire {
		//*********//
		//** PCA **//
		//*********//
		template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
		class PCA : public Model<TIdx, TId> {
		private:
			DSLib::Table<TIdx, TId> vectors_;
			DSLib::Table<TIdx, TId> mean_;
			DSLib::Table<TIdx, TId> sd_;

			template <typename T, template <typename, typename> class TMat>
			void calcMatFeatures_(Matrix<TMat<T, TIdx>, TIdx> &&in, Matrix<TMat<T, TIdx>, TIdx> &&out, bool train = false);
			template <typename T> 
			void calcFeatures_(Matrix<T, TIdx> &&in, Matrix<T, TIdx> &&out, bool train = false);
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
			PCA(const DSTypes::String &filename);
			PCA(const bool center = true, const bool scale = true, const UInt32 keepDims=0);

			M_HDR_MODEL_DESTRUCTOR(PCA)
			M_HDR_MODEL_CONSTRUCTOR_NO_FILE_NO_DEFAULT(PCA, TIdx, TId)
			M_HDR_MODEL_CLONE(PCA, TIdx, TId)
			M_HDR_MODEL_ASSIGNMENT(PCA, TIdx, TId)
			M_HDR_MODEL_OPERATOR(PCA, TIdx, TId)
		};

		//*********//
		//** LDA **//
		//********//
		template<typename TClassType = DSTypes::UInt32, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
		class LDA : public Model<TIdx, TId> {
		private:
			DSLib::Table<TIdx, TId> vectors_;
			DSLib::Table<TIdx, TId> mean_;
			DSLib::Table<TIdx, TId> sd_;
			std::map<TClassType, Float> classToNum_;

			template <typename T, template <typename, typename> class TMat>
			void calcMatFeatures_(Matrix<TMat<T, TIdx>, TIdx> &&in, Matrix<TClassType, TIdx> &&target, Matrix<TMat<T, TIdx>, TIdx> &&out, bool train = false);
			template <typename T> 
			void calcFeatures_(Matrix<T, TIdx> &&in, Matrix<TClassType, TIdx> &&target, Matrix<T, TIdx> &&out, bool train);
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

			LDA(const DSTypes::String &filename);
			LDA(const DSLib::Matrix<TClassType, TIdx> &classes, const bool center = true, const bool scale = true, const UInt32 keepDims=0);

			M_HDR_MODEL_DESTRUCTOR(LDA)
			M_HDR_MODEL_CONSTRUCTOR_NO_FILE(LDA, TClassType, TIdx, TId)
			M_HDR_MODEL_CLONE(LDA, TClassType, TIdx, TId)
			M_HDR_MODEL_ASSIGNMENT(LDA, TClassType, TIdx, TId)
			M_HDR_MODEL_OPERATOR(LDA, TClassType, TIdx, TId)
		};

	}
}
