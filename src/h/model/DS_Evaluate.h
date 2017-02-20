#pragma once
#include "h/model/DS_Model.h"
#include "hpp/lang/DS_ModelLang.hpp"

namespace DSFunc {
	template <typename TIdx, typename TId>
	Table<TIdx, TId> confusionFrac(Table<TIdx, TId> &conf, TIdx digits = (TIdx)1, Double mult = 100.0, bool perClass = true);
}

namespace DSModel {
	
	//***************//
	//** Confusion **//
	//***************//
	template<typename TClassType = DSTypes::UInt32, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Confusion : public Model<TIdx, TId> {
	protected:		
		void updateParameters() override;
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;		
	public:
		virtual DSTypes::String modelName() const override {return "Confusion";}

		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;
		
		Confusion(const Matrix<TClassType, TIdx> &classes);

		M_HDR_MODEL_DESTRUCTOR(Confusion)
		M_HDR_MODEL_CONSTRUCTOR(Confusion, TClassType, TIdx, TId)
		M_HDR_MODEL_CLONE(Confusion, TClassType, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(Confusion, TClassType, TIdx, TId)
		M_HDR_MODEL_OPERATOR(Confusion, TClassType, TIdx, TId)
	};
}