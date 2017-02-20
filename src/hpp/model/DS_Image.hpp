#pragma once

#include "h/model/DS_Image.h"

#include "hpp/model/DS_Model.hpp"
#include "hpp/lib/DS_Image.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	//****************//
	//** FileFetch **//
	//****************//

	template<typename TImage, typename TIdx, typename TId>
	FileFetch<TImage, TIdx, TId>::FileFetch(const DSTypes::String &filename) {		
		std::ifstream ifs(filename);
		if (!ifs)
			throw Error(ecNotFound, "FileFetch::FileFetch()", SS("File not found: " << filename));
		this->readParams(ifs);
		this->readModel(ifs, filename);
	}

	template<typename TImage, typename TIdx, typename TId>
	FileFetch<TImage, TIdx, TId>::FileFetch(const bool prefetch, const String folder) {
		Table<TIdx, TId> tab = ((Matrix<Literal, TIdx>() | "Val") |
									((Matrix<Literal, TIdx>() | "Prefetch" | "Folder") ^
										(
											(ctParameter | (dtUInt8 | (prefetch?UInt8(1):UInt8(0)))) |
											(ctParameter | (dtString | folder))
										)
									)
								);
		this->registerParameters(tab);
	};

	template<typename TImage, typename TIdx, typename TId>
	void FileFetch<TImage, TIdx, TId>::updateParameters() {
		this->template parameterValueById<UInt8>("Prefetch");
	}

	template<typename TImage, typename TIdx, typename TId>
	void FileFetch<TImage, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
		this->registerInput(dtString, ctImage);		
	}

	template<typename TImage, typename TIdx, typename TId>
	void FileFetch<TImage, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		if (input.cols.count() == 0)
			throw Error(ecIncompatible, "FileFetch.registerOutputs", "Should have at least one ctImage / dtString col");
		Matrix<TImage, TIdx> mat(0, input.cols.count());
		Table<TIdx, TId> output = (ctFeature | mat);
		this->addStages(dtString | "Read");
		output.resizeRows(0);
		this->registerOutput(output);
	}

	template<typename TImage, typename TIdx, typename TId>
	void FileFetch<TImage, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {		
		TImage t;
		bool pf = (this->template parameterValueById<UInt8>("Prefetch") > 0);
		String folder = this->template parameterValueById<String>("Folder");
		if (folder != "")
			folder = folder + OS_PATH_SEP;
		Matrix<String, TIdx> in = input(dtString, ctImage);
		Matrix<TImage, TIdx> out = output(dataType(t), ctFeature);
		this->setStage("Read");
		this->setMinProgress(0);
		this->setMaxProgress(out.rows.count());
		auto out_row_it = out.rows.begin();
		for (auto in_row_it = in.rows.begin(); in_row_it != in.rows.end(); in_row_it++, out_row_it++) {
			auto out_cell_it = out_row_it->begin();
			for (auto in_cell_it = in_row_it->begin(); in_cell_it != in_row_it->end(); in_cell_it++, out_cell_it++) {
				out_cell_it->setFilename(SS(folder << *in_cell_it));
				if (pf) 
					out_cell_it->loadImage();
			}
			this->incProgress();
		}
	}

	template<typename TImage, typename TIdx, typename TId>
	void FileFetch<TImage, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		train(table, input, output);
	}

	template<typename TImage, typename TIdx, typename TId>
	void FileFetch<TImage, TIdx, TId>::clone(const FileFetch<TImage, TIdx, TId> & other) {
		Model<TIdx, TId>::clone(other);
	}

	template<typename TImage, typename TIdx, typename TId>
	void FileFetch<TImage, TIdx, TId>::clone(FileFetch<TImage, TIdx, TId> && other) {
		Model<TIdx, TId>::clone(std::move(other));
	}

	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(FileFetch, TImage, TIdx, TId)
	template<typename TImage, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(FileFetch, TImage, TIdx, TId)
};
