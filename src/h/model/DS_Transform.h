#pragma once
#include "DS_Model.h"

namespace DSModel {

	//***********//
	//** Paste **//
	//***********//
	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Paste : public Model<TIdx, TId> {
	private:
		Table<TIdx, TId> tab_;
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
		Paste(const DSTypes::String &filename);
		Paste(const Table<TIdx, TId> &tab);

		M_HDR_MODEL_DESTRUCTOR(Paste)
		M_HDR_MODEL_CONSTRUCTOR_NO_FILE(Paste, TIdx, TId)
		M_HDR_MODEL_CLONE(Paste, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(Paste, TIdx, TId)
		M_HDR_MODEL_OPERATOR(Paste, TIdx, TId)
	};

	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class ZScore : public Model<TIdx, TId> {
	private:
		DSLib::Table<TIdx, TId> mean_;
		DSLib::Table<TIdx, TId> sd_;

		template <typename T, template <typename, typename> class TMat>
		void calcMatFeatures_(Matrix<TMat<T, TIdx>, TIdx> &&in, Matrix<TMat<T, TIdx>, TIdx> &&out, bool train=false);
		template <typename T>
		void calcFeatures_(Matrix<T, TIdx> &&in, Matrix<T, TIdx> &&out, bool train=false);
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

		Model<TIdx, TId> * clone() const override;
		Model<TIdx, TId> * move() override;

		ZScore() {};
		ZScore(const DSTypes::String &filename);
		~ZScore() {};

		ZScore(const ZScore &other);
		ZScore(ZScore &&other);
		ZScore(const Model<TIdx, TId> &other);
		ZScore(Model<TIdx, TId> &&other);
		ZScore & operator=(const ZScore &other);
		ZScore & operator=(ZScore &&other);
		void clone(const ZScore &other);
		void clone(ZScore &&other);

		ZScore operator()(const TIdx count=0);
	};

	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class MMScale : public Model<TIdx, TId> {
	private:
		//DSLib::Matrix<DSTypes::Double, TIdx> min_;
		//DSLib::Matrix<DSTypes::Double, TIdx> diff_;
		DSLib::Table<> min_;
		DSLib::Table<> diff_;

		template <typename T, template <typename, typename> class TMat>
		void calcMatFeatures_(Matrix<TMat<T, TIdx>, TIdx> &&in, Matrix<TMat<T, TIdx>, TIdx> &&out, bool train=false);
		template <typename T>
		void calcFeatures_(Matrix<T, TIdx> &&in, Matrix<T, TIdx> &&out, bool train=false);
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

		Model<TIdx, TId> * clone() const override;
		Model<TIdx, TId> * move() override;

		MMScale(){};
		~MMScale(){};

		MMScale(const MMScale &other);		
		MMScale(MMScale &&other);
		MMScale(const Model<TIdx, TId> &other);
		MMScale(Model<TIdx, TId> &&other);
		MMScale & operator=(const MMScale &other);
		MMScale & operator=(MMScale &&other);
		void clone(const MMScale &other);
		void clone(MMScale &&other);
		
		MMScale operator()(const TIdx count=0);
	};
}
