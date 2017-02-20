#pragma once
#include "DS_Model.h"

namespace DSModel {
	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Export : public Model<TIdx, TId> {
	private:
		void write_(std::ostream &f, DSLib::Table<TIdx, TId>& input);
		DSTypes::String quote_;
		DSTypes::String delim_;
		DSTypes::String newline_;
		DSTypes::String filename_;
		bool header_ = true;		
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

		Export() {};
		Export(const DSTypes::String filename, const bool includeTypes = true, const DSTypes::String delim=" ", const DSTypes::String newline = "\n", const DSTypes::String quote="\"");
		~Export() {};

		Export(const Export &other);
		Export(Export &&other);
		Export(const Model<TIdx, TId> &other);
		Export(Model<TIdx, TId> &&other);
		Export & operator=(const Export &other);
		Export & operator=(Export &&other);
		void clone(const Export &other);
		void clone(Export &&other);

		Export operator()(const TIdx count = 0);
	};
}