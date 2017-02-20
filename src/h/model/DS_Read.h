#pragma once
#include "DS_Model.h"

namespace DSModel {
	//************//
	//** Import **//
	//************//
	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Import : public Model<TIdx, TId> {
	private:
		DSLib::Matrix<DSTypes::String> head_;
		std::map<std::pair<DSTypes::ContentType, DSTypes::DataType>, DSLib::Matrix<TIdx>> colIdx_;
		std::map<std::pair<DSTypes::ContentType, DSTypes::DataType>, DSLib::Matrix<TId>> colId_;

		void prepare_(std::istream &f, DSLib::Table<TIdx, TId> &proto, TIdx &samples);
		void read_(std::istream &f, DSLib::Table<TIdx, TId> & output);
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

		Import() {};
		Import(const DSTypes::String filename, const DSTypes::String delim = " ", const DSTypes::String newline = "\n", const DSTypes::String quote = "\"");
		~Import() {};

		Import(const Import &other);
		Import(Import &&Import);
		Import(const Model<TIdx, TId> &other);
		Import(Model<TIdx, TId> &&other);
		Import & operator=(const Import &other);
		Import & operator=(Import &&other);
		void clone(const Import &other);
		void clone(Import &&other);

		Import operator()(const TIdx count = 0);
	};

	//**********//
	//** Read **//
	//**********//
	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Read : public Model<TIdx, TId> {
	private:
		void prepare_(std::istream &f, DSLib::Table<TIdx, TId> &proto, TIdx &samples);
		void read_(std::istream &f, DSLib::Table<TIdx, TId> & output);
		DSTypes::String filename_;
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

		Read() {};
		Read(const DSTypes::String filename);
		~Read() {};

		Read(const Read &other);
		Read(Read &&Import);
		Read(const Model<TIdx, TId> &other);
		Read(Model<TIdx, TId> &&other);
		Read & operator=(const Read &other);
		Read & operator=(Read &&other);
		void clone(const Read &other);
		void clone(Read &&other);

		Read operator()(const TIdx count = 0);
	};
}