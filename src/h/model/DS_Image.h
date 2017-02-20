#pragma once
#include "DS_Model.h"

namespace DSModel {
	template<typename TImage = ImagePNG<Double>, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class FileFetch : public Model<TIdx, TId> {
	private:
	protected:
		void updateParameters() override;
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
	public:
		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;

		FileFetch(const bool prefetch = true, const String folder="");
		FileFetch(const DSTypes::String &filename);

		M_HDR_MODEL_DESTRUCTOR(FileFetch)

		M_HDR_MODEL_CONSTRUCTOR_COPY(FileFetch, TImage, TIdx, TId)
		M_HDR_MODEL_CONSTRUCTOR_MOVE(FileFetch, TImage, TIdx, TId)
		M_HDR_MODEL_CONSTRUCTOR_MODEL_COPY(FileFetch)
		M_HDR_MODEL_CONSTRUCTOR_MODEL_MOVE(FileFetch)
		M_HDR_MODEL_CLONE(FileFetch, TImage, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(FileFetch, TImage, TIdx, TId)
		M_HDR_MODEL_OPERATOR(FileFetch, TImage, TIdx, TId)
	};
}