#pragma once
#include "DS_Model.h"

namespace DSModel {
	template<typename T=DSTypes::Double, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class GenGauss : public Model<TIdx, TId> {
	private:
		std::normal_distribution<T> nd_;

		void makeDistrib_(const DSLib::Matrix<T, TIdx> &eigenVectors, const DSLib::Matrix<T, TIdx> &eigenValues, const DSLib::Matrix<T, TIdx> &mean);
	protected:
		void updateParameters() override;
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
	public:
		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;

		Model<TIdx, TId> * clone() const override;
		Model<TIdx, TId> * move() override;

		GenGauss() {};
		GenGauss(const DSLib::Matrix<T, TIdx> &eigenVectors, const DSLib::Matrix<T, TIdx> &eigenValues, const DSLib::Matrix<T, TIdx> &mean);
		GenGauss(const DSLib::Matrix<T, TIdx> &deviation, const DSLib::Matrix<T, TIdx> &mean);
		~GenGauss() {};

		GenGauss(const GenGauss &other);
		GenGauss(GenGauss &&other);
		GenGauss(const Model<TIdx, TId> &other);
		GenGauss(Model<TIdx, TId> &&other);
		GenGauss & operator=(const GenGauss &other);
		GenGauss & operator=(GenGauss &&other);
		void clone(const GenGauss &other);
		void clone(GenGauss &&other);

		GenGauss operator()(const TIdx count = 0);
	};
}