#pragma once

#include "h/lib/DS_Evolution.h"
#include <random>
#include <thread>
#include <chrono>

#include "hpp/DS_Types.hpp"

namespace DSEvolution {
	#define ASYNCPOLICY std::launch::async
	//#define ASYNCPOLICY std::launch::deferred

	//************//
	//** Random **//
	//************//
	namespace {
		class Random {
		protected:
			std::mt19937 gen_;
		public:
			Random() {
				std::random_device rd;
				gen_.seed(rd());
			}

			void seed(std::random_device &rd) {
				gen_.seed(rd());
			}

			void seed(const DSTypes::UInt32 &s) {
				gen_.seed(s);
			}
		};

        template <typename T>
        class RandomReal : public Random {
        public:
            T genUniformUnit() {
                std::uniform_real_distribution<T> r(0, 1);
                return r(this->gen_);
            }

            T genUniformRange(const T min, const T max) {
                std::uniform_real_distribution<T> r(min, max);
                return r(this->gen_);
            }

            T genGauss() {
                std::normal_distribution<T> r(0, 1);
                return r(this->gen_);
            }

            T genGaussParam(const T mean, const T sigma) {
                std::normal_distribution<T> r(mean, sigma);
                return r(this->gen_);
            }

            std::vector<T> genGauss(const DSTypes::UInt32 n) {
                std::normal_distribution<T> r(0, 1);
                std::vector<T> v;
                for (DSTypes::UInt32 i=0;i<n;i++) {
                    v.push_back(r(this->gen_));
                }
                return v;
            }

            std::vector<T> genGaussParam(const DSTypes::UInt32 n, const T mean, const T sigma) {
                std::normal_distribution<T> r(mean, sigma);
                std::vector<T> v;
                for (DSTypes::UInt32 i=0;i<n;i++) {
                    v.push_back(r(this->gen_));
                }
                return v;
            }

            std::vector<T> genUniformUnit(const DSTypes::UInt32 n) {
                std::uniform_real_distribution<T> r(0, 1);
                std::vector<T> v;
                for (DSTypes::UInt32 i=0;i<n;i++) {
                    v.push_back(r(this->gen_));
                }
                return v;
            }

            std::vector<T> genUniformRange(const DSTypes::UInt32 n, const T min, const T max) {
                std::vector<T> v;
                std::uniform_real_distribution<T> r(min, max);
                for (DSTypes::UInt32 i=0;i<n;i++) {
                    v.push_back(r(this->gen_));
                }
                return v;
            }
        };

        template <typename T>
        class RandomInt : public Random {
        public:
            T genUniformUnit() {
                std::uniform_int_distribution<T> r(0, 1);
                return r(this->gen_);
            }

            T genUniformRange(const T min, const T max) {
                std::uniform_int_distribution<T> r(min, max);
                return r(this->gen_);
            }

            std::vector<T> genUniformUnit(const T n) {
                std::uniform_int_distribution<T> r(0, 1);
                std::vector<T> v;
                for (DSTypes::UInt32 i=0;i<n;i++) {
                    v.push_back(r(this->gen_));
                }
                return v;
            }

            std::vector<T> genUniformRange(const T n, const T min, const T max) {
                std::vector<T> v;
                std::uniform_int_distribution<T> r(min, max);
                for (DSTypes::UInt32 i=0;i<n;i++) {
                    v.push_back(r(this->gen_));
                }
                return v;
            }
        };

        template <typename T>
        static RandomReal<T> & rndr() {throw Error(DSTypes::ecIncompatible, "rndr", "No RNG for the specified type");}

        static RandomReal<DSTypes::Double> rndd_;
        template <> RandomReal<DSTypes::Double> & rndr() {return rndd_;}

        static RandomReal<DSTypes::Float> rndf_;
        template <> RandomReal<DSTypes::Float> & rndr() {return rndf_;}

        template <typename T>
        static RandomInt<T> & rndi() {throw Error(DSTypes::ecIncompatible, "rndi", "No RNG for the specified type");}

        static RandomInt<DSTypes::UInt32> rndui32_;
        template <> RandomInt<DSTypes::UInt32> & rndi() {return rndui32_;}

        //****************//
        //** Individual **//
        //****************//
		void Individual::clone(const Individual &) {}
		void Individual::clone(Individual &&) {}

        //********************//
        //** RealIndividual **//
        //********************//

        template <typename T>
        void RealIndividual<T>::print(std::ostream &s) const {
            for (DSTypes::UInt32 i=0; i<size(); i++)
                s << names_[i] <<  ": " << mins_[i] << " < " << values_[i] << " < " << maxs_[i] << std::endl;
        }

        template <typename T>
        void RealIndividual<T>::randomize() {
            for (UInt32 i=0;i<size();i++)
                values_[i] = rndr<DSTypes::Double>().genUniformRange(mins_[i], maxs_[i]);
        }

        template <typename T>
        void RealIndividual<T>::clone(const RealIndividual<T> &a) {
            Individual::clone(a);
            names_ = a.names_;
            mins_ = a.mins_;
            maxs_ = a.maxs_;
            values_ = a.values_;
        }

        template <typename T>
        void RealIndividual<T>::clone(RealIndividual<T> &&a) {
            names_ = std::move(a.names_);
            mins_ = std::move(a.mins_);
            maxs_ = std::move(a.maxs_);
            values_ = std::move(a.values_);
        }

        template <typename T>
        std::vector<T> RealIndividual<T>::getValues() const {
            return values_;
        }

        template <typename T>
        std::vector<T> RealIndividual<T>::getNames() const {
            return names_;
        }

        template <typename T>
        std::vector<T> RealIndividual<T>::getMins() const {
            return mins_;
        }

        template <typename T>
        std::vector<T> RealIndividual<T>::getMaxs() const {
            return maxs_;
        }

        template <typename T>
        T RealIndividual<T>::getMin(const DSTypes::UInt32 idx) const {
            if (idx >= mins_.size())
                throw Error(DSTypes::ecRangeError, "getMin", SS("Out of range " << idx));
            return mins_[idx];
        }

        template <typename T>
        T RealIndividual<T>::getMax(const DSTypes::UInt32 idx) const {
            if (idx >= maxs_.size())
                throw Error(DSTypes::ecRangeError, "getMax", SS("Out of range " << idx));
            return maxs_[idx];
        }

        template <typename T>
        DSTypes::String RealIndividual<T>::getName(const DSTypes::UInt32 idx) const {
            if (idx >= names_.size())
                throw Error(DSTypes::ecRangeError, "getName", SS("Out of range " << idx));
            return names_[idx];
        }

        template <typename T>
        T RealIndividual<T>::getValue(const DSTypes::UInt32 idx) const {
            if (idx >= values_.size())
                throw Error(DSTypes::ecRangeError, "getValue", SS("Out of range " << idx));
            return values_[idx];
        }

        template <typename T>
        T RealIndividual<T>::getValue(const DSTypes::String &name) const {
            for (DSTypes::UInt32 i=0;i<names_.size(); i++) {
                if (names_[i] == name)
                    return values_[i];
            }
            throw Error(DSTypes::ecNotFound, "Individual::getValue", SS("Name " << name << " not found"));
        }


        template <typename T>
        void RealIndividual<T>::setValue(const DSTypes::UInt32 idx, const T value) {
            if (idx >= values_.size())
                throw Error(DSTypes::ecRangeError, "setValue", SS("Out of range " << idx));
            if (value < mins_[idx]) {
                values_[idx] = mins_[idx];
                return;
            }
            if (value > maxs_[idx]) {
                values_[idx] = maxs_[idx];
                return;
            }
            values_[idx] = value;
        }

        template <typename T>
        void RealIndividual<T>::setParams(const std::vector<DSTypes::String> &names, const std::vector<T> &mins, const std::vector<T> &maxs, const std::vector<T> &values) {
            names_ = names;
            mins_ = mins;
            maxs_ = maxs;
            for (DSTypes::UInt32 idx=0;idx<values.size();idx++) {
                if (values[idx] < mins_[idx]) values[idx] = mins_[idx];
                if (values[idx] > maxs_[idx]) values[idx] = maxs_[idx];
            }
            values_ = values;
        }

        template <typename T>
        void RealIndividual<T>::setParam(const DSTypes::UInt32 idx, const DSTypes::String &name, const T &min, const T &max, const T &value) {
            if (idx >= names_.size())
                throw Error(DSTypes::ecRangeError, "setParam", SS("Out of range " << idx));
            names_[idx] = name;
            mins_[idx] = min;
            maxs_[idx] = max;
            if (value < min)  {
                values_[idx] = min;
                return;
            }
            if (value > max)  {
                values_[idx] = max;
                return;
            }
            values_[idx] = value;
        }

        template <typename T>
        void RealIndividual<T>::addParam(const DSTypes::String &name, const T &min, const T &max, const T &value) {
            names_.push_back(name);
            mins_.push_back(min);
            maxs_.push_back(max);
            if (value < min)  {
                values_.push_back(min);
                return;
            }
            if (value > max)  {
                values_.push_back(max);
                return;
            }
            values_.push_back(value);
        }

        template <typename T>
        DSTypes::UInt32 RealIndividual<T>::size() const {
            return (DSTypes::UInt32) names_.size();
        }

        template <typename T>
        bool RealIndividual<T>::check(const RealIndividual<T> &b, const bool except) const {
            const DSTypes::String f = "check";
            if (size() != b.size())
                return except ? throw Error(DSTypes::ecRangeError, f, SS("Invalid number of parameters " << size() << " != " << b.size())) : false;
            for (DSTypes::UInt32 i=0; i<size(); i++) {
                if (names_[i] != b.names_[i])
                    return except ? throw Error(DSTypes::ecIncompatible, f, SS("Parameter names are not equal " << names_[i] << " != " << b.names_[i])) : false;
                if (mins_[i] != b.mins_[i]) return except ? throw Error(DSTypes::ecIncompatible, f, SS("Parameter mins are not equal " << mins_[i] << " != " << b.mins_[i])) : false;
                if (maxs_[i] != b.maxs_[i]) return except ? throw Error(DSTypes::ecIncompatible, f, SS("Parameter maxs are not equal " << maxs_[i] << " != " << b.maxs_[i])) : false;
            }
            return true;
        }

        template <typename T>
        RealIndividual<T> RealIndividual<T>::crossover(const RealIndividual<T> &a, const RealIndividual<T> &b, const DSTypes::Double prob) {
            a.check(b);
            vector<T> numbers = rndr<T>().genUniformUnit(a.size());
            DSTypes::UInt32 idx=0;
            RealIndividual<T> ret = a;
            for (auto num = numbers.begin(); num != numbers.end(); num++, idx++) {
                if (*num < prob)
                    ret.setValue(idx, b.getValue(idx));
            }
            return ret;
        }

        template <typename T>
        RealIndividual<T> RealIndividual<T>::mutate(const RealIndividual<T> &a, const DSTypes::Double prob) {
            vector<T> numbers = rndr<T>().genUniformUnit(a.size());
            DSTypes::UInt32 idx=0;
            RealIndividual<T> ret = a;
            for (auto num = numbers.begin(); num != numbers.end(); num++, idx++) {
                if (*num < prob)
                    ret.setValue(idx, rndr<T>().genUniformRange(a.getMin(idx), a.getMax(idx)));
            }
            return ret;
        }

        template <typename T>
        RealIndividual<T> RealIndividual<T>::mutateGauss(const RealIndividual<T> &a, const DSTypes::Double prob) {
            vector<T> numbers = rndr<T>().genGaussParam(a.size(), 0, prob);
            DSTypes::UInt32 idx=0;
            RealIndividual<T> ret = a;
            for (auto num = numbers.begin(); num != numbers.end(); num++, idx++) {
                T mutate = (ret.getMax(idx) - ret.getMin(idx)) * numbers[idx];
                ret.setValue(idx, ret.getValue(idx) + mutate);
            }
            return ret;
        }

        template <typename T>
        T RealIndividual<T>::operator[](const DSTypes::String &name) const {
            for (DSTypes::UInt32 i=0;i<names_.size(); i++) {
                if (names_[i] == name)
                    return values_[i];
            }
            throw Error(DSTypes::ecNotFound, "Individual::operator[]", SS("Name " << name << " not found"));
        }

        template <typename T>
        T RealIndividual<T>::operator[](const DSTypes::UInt32 &idx) const {
            if (idx >= values_.size())
                throw Error(DSTypes::ecRangeError, "getValue", SS("Out of range " << idx));
            return values_[idx];
        }


        //***************//
        //** Objective **//
        //***************//
		void Objective::clone(const Objective &a) {}
		void Objective::clone(Objective &&a) {}

        //********************//
        //** MultiObjective **//
        //********************//
        template <typename T>
        void MultiObjective<T>::clone(const MultiObjective<T> &a) {
            scores_ = a.scores_;
        }

        template <typename T>
        void MultiObjective<T>::clone(MultiObjective<T> &&a) {
            scores_ = std::move(a.scores_);
        }

        template <typename T>
        void MultiObjective<T>::setScores(const std::vector<T> &os) {
            scores_ = os;
        }

        template <typename T>
        std::vector<T> MultiObjective<T>::getScores() const {
            return scores_;
        }

        template <typename T>
        void MultiObjective<T>::setScore(const DSTypes::UInt32 idx, const T s) {
            if (idx >= scores_.size)
                throw Error(DSTypes::ecRangeError, "setScore", SS("Out of range " << idx));
            scores_[idx] = s;
        }

        template <typename T>
        T MultiObjective<T>::getScore(const DSTypes::UInt32 idx) const {
            if (idx >= scores_.size)
                throw Error(DSTypes::ecRangeError, "getScore", SS("Out of range " << idx));
            return scores_[idx];
        }

        template <typename T>
        void MultiObjective<T>::addScore(const T s) {
            scores_.push_back(s);
        }

        //*********************//
        //** SingleObjective **//
        //*********************//

        template <typename T>
        void SingleObjective<T>::print(std::ostream &s) const {
            s << "Score: " << score_;
        }

        template <typename T>
        void SingleObjective<T>::clone(const SingleObjective<T> &a) {
            score_ = a.score_;
        }

        template <typename T>
        void SingleObjective<T>::clone(SingleObjective<T> &&a) {
            score_ = std::move(a.score_);
        }

        template <typename T>
        void SingleObjective<T>::setScore(const T s) {
            score_ = s;
        }

        template <typename T>
        void SingleObjective<T>::operator=(const T &s) {
            score_ = s;
        }


        template <typename T>
        T SingleObjective<T>::getScore() const {
            return score_;
        }

        template <typename T>
        bool SingleObjective<T>::operator>(const SingleObjective &a) const {
            return score_ > a.score_;
        }

        template <typename T>
        bool SingleObjective<T>::operator<(const SingleObjective &a) const {
            return score_ < a.score_;
        }

        template <typename T>
        bool SingleObjective<T>::operator>=(const SingleObjective &a) const {
            return score_ >= a.score_;
        }

        template <typename T>
        bool SingleObjective<T>::operator<=(const SingleObjective &a) const {
            return score_ <= a.score_;
        }

        template <typename T>
        bool SingleObjective<T>::operator==(const SingleObjective &a) const {
            return score_ == a.score_;
        }

        template <typename T>
        bool SingleObjective<T>::operator!=(const SingleObjective &a) const {
            return score_ != a.score_;
        }

        //****************//
        //** Population **//
        //****************//
		void Population::clone(const Population &a) {}
		void Population::clone(Population &&a) {}

        //************************//
        //** StandardPopulation **//
        //************************//

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::print(std::ostream &s) const {
            for (DSTypes::UInt32 i=0; i<size(); i++) {
                s << "(" << i+1 << " of " << size() << ")" << std::endl;
                individuals_[i].print(s);
                objectives_[i].print(s);
                s << std::endl;
            }
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective>::StandardPopulation(const std::function<TObjective(const TIndividual &)> &fitness) {
            fitness_ = fitness;
        }

        template <class TIndividual, class TObjective>
        DSTypes::UInt32 StandardPopulation<TIndividual, TObjective>::size() const {
            if (individuals_.size() != objectives_.size())
                throw Error(DSTypes::ecInternal, "size", "Individuals and Objectives differ in size.");
            return (DSTypes::UInt32)individuals_.size();
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::add(const StandardPopulation<TIndividual, TObjective> &p) {
            addIndividuals(p.getIndividuals());
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::setFitnessFunction(const std::function<TObjective(const TIndividual &)> &f) {
            fitness_ = f;

        }

        template <class TIndividual, class TObjective>
        const std::function<TObjective(const TIndividual &)> & StandardPopulation<TIndividual, TObjective>::getFitnessFunction() const {
            return fitness_;
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::setEvaluationFunction(const std::function<StandardPopulation<TIndividual, TObjective>(const StandardPopulation<TIndividual, TObjective> &)> &f) {
            evaluation_ = f;
        }

        template <class TIndividual, class TObjective>
        const std::function<StandardPopulation<TIndividual, TObjective>(const StandardPopulation<TIndividual, TObjective> &)> & StandardPopulation<TIndividual, TObjective>::getEvaluationFunction() const{
            return evaluation_;
        }


        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::addIndividual(const TIndividual &a, const TObjective &o) {
            individuals_.push_back(a);
            objectives_.push_back(o);
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::getIndividual(const DSTypes::UInt32 idx, TIndividual &a, TObjective &o) const {
            if (idx >= individuals_.size())
                throw Error(DSTypes::ecRangeError, "getIndividual", SS("Out of range " << idx));
            a = individuals_[idx];
            o = objectives_[idx];
        }

        template <class TIndividual, class TObjective>
        std::pair<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::getIndividual(const DSTypes::UInt32 idx) const {
            TIndividual a;
            TObjective o;
            getIndividual(idx, a, o);
            return pair<TIndividual, TObjective>(a, o);
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::getIndividuals(std::vector<TIndividual> &a, std::vector<TObjective> &o) const {
            a = individuals_;
            o = objectives_;
        }

        template <class TIndividual, class TObjective>
        std::vector<std::pair<TIndividual, TObjective>> StandardPopulation<TIndividual, TObjective>::getIndividuals() const {
            std::vector<std::pair<TIndividual, TObjective>> ret;
            for (DSTypes::UInt32 i=0;i<size();i++)
                ret.push_back(pair<TIndividual, TObjective>(individuals_[i], objectives_[i]));
            return ret;
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::addIndividuals(const std::vector<std::pair<TIndividual, TObjective>> &a) {
            for(auto i=a.begin(); i!=a.end();i++)
                addIndividual(i->first, i->second);
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::addIndividual(const std::pair<TIndividual, TObjective> &i) {
            addIndividual(i.first, i.second);
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::clear() {
            individuals_.clear();
            objectives_.clear();
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::clone(const StandardPopulation<TIndividual, TObjective> &a) {
            individuals_ = a.individuals_;
            objectives_ = a.objectives_;
            fitness_ = a.fitness_;
            evaluation_ = a.evaluation_;
        }

        template<class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::clone(StandardPopulation<TIndividual, TObjective> &&a) {
            individuals_ = std::move(a.individuals_);
            objectives_ = std::move(a.objectives_);
            fitness_ = std::move(a.fitness_);
            evaluation_ = a.evaluation_;
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::sort(const StandardPopulation<TIndividual, TObjective> &a) {
            StandardPopulation<TIndividual, TObjective> ret;
            std::vector<std::pair<TIndividual, TObjective>> p = a.getIndividuals();

            //Create and sort using std::function
            std::function<bool(const std::pair<TIndividual, TObjective> &, const std::pair<TIndividual, TObjective> &)> f =
                [](const std::pair<TIndividual, TObjective> &elm1, const std::pair<TIndividual, TObjective> &elm2) {
                    return elm1.second > elm2.second;
                };
            std::sort(p.begin(), p.end(), f);

            for (auto i=p.begin();i!=p.end();i++)
                ret.addIndividual(i->first, i->second);
            return ret;
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::select(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::UInt32 start, const DSTypes::UInt32 count) {
            StandardPopulation<TIndividual, TObjective> ret;
            for (DSTypes::UInt32 i=start;i<start+count;i++)
                ret.addIndividual(a.getIndividual(i));
            return ret;
        }

        template <class TIndividual, class TObjective>
        void StandardPopulation<TIndividual, TObjective>::selectParents(const StandardPopulation<TIndividual, TObjective> &a, StandardPopulation<TIndividual, TObjective> &fathers, StandardPopulation<TIndividual, TObjective> &mothers, const DSTypes::UInt32 n) {
            for (DSTypes::UInt32 i=0; i<n; i++) {
                fathers.addIndividual(a.getIndividual(rndi<DSTypes::UInt32>().genUniformRange(0, a.size()-1)));
                mothers.addIndividual(a.getIndividual(rndi<DSTypes::UInt32>().genUniformRange(0, a.size()-1)));
            }
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::crossover(const StandardPopulation<TIndividual, TObjective> &fathers, const StandardPopulation<TIndividual, TObjective> &mothers, const DSTypes::Double crossoverProb) {
            if (fathers.size() != mothers.size())
                throw Error(DSTypes::ecIncompatible, "crossover", "Not an equal amount of parents");
            StandardPopulation<TIndividual, TObjective> ret;
            for (DSTypes::UInt32 i=0;i<fathers.size();i++) {
                TIndividual x, y;
                TObjective o;
                fathers.getIndividual(i, x, o);
                mothers.getIndividual(i, y, o);
                TIndividual xy = TIndividual::crossover(x, y, crossoverProb);
                ret.addIndividual(xy, o);
            }
            return ret;
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::crossover(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::Double crossoverProb) {
            StandardPopulation<TIndividual, TObjective> ret;
            for (DSTypes::UInt32 x=0;x<a.size();x++) {
                for (DSTypes::UInt32 y=0;y<a.size();y++) {
                    TIndividual xx, yy;
                    TObjective o;
                    a.getIndividual(x, xx, o);
                    a.getIndividual(y, yy, o);
                    TIndividual i = TIndividual::crossover(xx, yy, crossoverProb);
                    ret.addIndividual(i, o);
                }
            }
            return ret;
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::mutate(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::Double mutationProb) {
            StandardPopulation<TIndividual, TObjective> ret;
            for (DSTypes::UInt32 x=0;x<a.size();x++) {
                TIndividual i;
                TObjective o;
                a.getIndividual(x, i, o);
                i = TIndividual::mutate(i, mutationProb);
                ret.addIndividual(i, o);
            }
            return ret;
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::mutateGauss(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::Double mutationProb) {
            StandardPopulation<TIndividual, TObjective> ret;
            for (DSTypes::UInt32 x=0;x<a.size();x++) {
                TIndividual i;
                TObjective o;
                a.getIndividual(x, i, o);
                i = TIndividual::mutateGauss(i, mutationProb);
                ret.addIndividual(i, o);
            }
            return ret;
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::initPopulation(const StandardPopulation<TIndividual, TObjective> &proto, const DSTypes::UInt32 n) {
            if (proto.size() == 0)
                throw Error(DSTypes::ecIncompatible, "initPopulation", "No proto individual supplied");
            if (proto.size() > 1)
                throw Error(DSTypes::ecIncompatible, "initPopulation", "More than one proto individual supplied");
            StandardPopulation<TIndividual, TObjective> ret;
            for (DSTypes::UInt32 i=0;i<n;i++) {
                TIndividual a;
                TObjective o;
                proto.getIndividual(0, a, o);
                a.randomize();
                ret.addIndividual(a, o);
            }
            return ret;
        }


        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::selectChildren(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::UInt32 n) {
            if (n > a.size())
                throw Error(ecRangeError, "selectChildren", SS("Cannot select " << n << "children from a population of " << a.size()));
            StandardPopulation<TIndividual, TObjective> ret = sort(a);
            return select(ret, 0, n);
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::selectElite(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::UInt32 n) {
            auto b = sort(a);
            return select(b, 0, n);
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::selectElite(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::Double prob) {
            StandardPopulation<TIndividual, TObjective> ret;
            for (DSTypes::UInt32 i=0;i<a.size();i++) {
                if (rndr<DSTypes::Double>().genUniformUnit() < prob)
                    ret.addIndividual(a.getIndividual(i));
            }
            return ret;
        }

        template <class TIndividual, class TObjective>
        StandardPopulation<TIndividual, TObjective> StandardPopulation<TIndividual, TObjective>::evaluate(const StandardPopulation<TIndividual, TObjective> &a, const StandardPopulation<TIndividual, TObjective> &evaluator) {
            //Evaluate population as a whole
            if (evaluator.getEvaluationFunction()) {
                return evaluator.getEvaluationFunction()(a);
            }

            //Evaluate per individual
            StandardPopulation<TIndividual, TObjective> ret;
            std::vector<std::future<TObjective>> futures;
            futures.resize(a.size());
            for (DSTypes::UInt32 i=0; i<a.size();i++) {
                TIndividual v;
                TObjective o;
                a.getIndividual(i, v, o);
                futures[i] = std::async(ASYNCPOLICY, evaluator.getFitnessFunction() , v);
            }
            for (DSTypes::UInt32 i=0; i<a.size();i++) {
                TIndividual v;
                TObjective o;
                a.getIndividual(i, v, o);
                o = futures[i].get();
                ret.addIndividual(v, o);
            }
            return ret;
        }

        //***************//
        //** Evolution **//
        //***************//
		Evolution::Evolution() {
			currentGeneration_ = 0;
			maxGenerations_ = 0;
			lastGeneration_ = 0;
		}

		void Evolution::clone(const Evolution &a) {
			maxGenerations_ = a.maxGenerations_;
			currentGeneration_ = a.currentGeneration_;
		}

		void Evolution::clone(Evolution &&a) {
			maxGenerations_ = a.maxGenerations_;
			currentGeneration_ = a.currentGeneration_;
		}

		bool Evolution::hasNewData() {
			DSTypes::UInt32 old = lastGeneration_;
			lastGeneration_ = getCurrentGeneration();
			return old != lastGeneration_;
		}

		void Evolution::setMaxGenerations(const DSTypes::UInt32 mg) {
			std::lock_guard<std::mutex> lock(lockMaxGen_);
			maxGenerations_ = mg;
		}

		DSTypes::UInt32 Evolution::getMaxGenerations() {
			std::lock_guard<std::mutex> lock(lockMaxGen_);
			return maxGenerations_;
		}

		DSTypes::UInt32 Evolution::getCurrentGeneration() {
			std::lock_guard<std::mutex> lock(lockCurrGen_);
			return currentGeneration_;
		}

		void Evolution::incCurrentGeneration() {
			std::lock_guard<std::mutex> lock(lockCurrGen_);
			currentGeneration_++;
		}

		void Evolution::run_() {
			while (getCurrentGeneration() < getMaxGenerations()) {
				runOnce();
				incCurrentGeneration();
			}
		}

		void Evolution::run(DSTypes::UInt32 generations) {
			if (currentGeneration_ == 0) {
				init();
			}
			maxGenerations_ = generations;
			auto f = [this](){run_();};
			future_ = std::async(ASYNCPOLICY, f);
		}

		void Evolution::stop() {
			//Gracefully stop thread
			setMaxGenerations(getCurrentGeneration());
		}

		bool Evolution::wait(DSTypes::UInt32 ms) {
			if (ms == 0) {
				if (!future_.valid())
					return true;
				future_.get();
				return true;
			} else {
				if (!future_.valid() || future_.wait_for(std::chrono::milliseconds(ms)) == std::future_status::ready) {
					future_.get();
					return true;
				} else
					return false;
			}
		}
	}

	//*****************//
	//** GAEvolution **//
	//*****************//
	template <class TPopulation>
	GAEvolution<TPopulation>::GAEvolution(const TPopulation &proto,
					const DSTypes::Double crossoverProb,
					const DSTypes::Double mutationProb,
					const DSTypes::UInt32 eliteCount,
					const DSTypes::UInt32 parentCount,
					const DSTypes::UInt32 childCount) {
		proto_ = proto;
		crossoverProb_ = crossoverProb;
		mutationProb_ = mutationProb;
		eliteCount_ = eliteCount;
		parentCount_ = parentCount;
		childCount_ = childCount;
	}

	template <class TPopulation>
	void GAEvolution<TPopulation>::clone(const GAEvolution<TPopulation> &a) {
		proto_ = a.proto_;
		crossoverProb_ = a.crossoverProb_;
		mutationProb_ = a.mutationProb_;
		eliteCount_ = a.eliteCount_;
		parentCount_ = a.parentCount_;
		childCount_ = a.childCount_;
	}

	template <class TPopulation>
	void GAEvolution<TPopulation>::clone(GAEvolution<TPopulation> &&a) {
		proto_ = std::move(a.proto_);
		crossoverProb_ = a.crossoverProb_;
		mutationProb_ = a.mutationProb_;
		eliteCount_ = a.eliteCount_;
		parentCount_ = a.parentCount_;
		childCount_ = a.childCount_;
	}

	template<class TPopulation>
	void GAEvolution<TPopulation>::init() {
		if (proto_.size() == parentCount_)
			setPopulation(proto_);
		else
			setPopulation(TPopulation::initPopulation(proto_, parentCount_));

		setPopulation(TPopulation::evaluate(getPopulation(), proto_));
	}

	template<class TPopulation>
	TPopulation GAEvolution<TPopulation>::getPopulation() {
		std::lock_guard<std::mutex> lock(lockPop_);
		return population_;
	}

	template<class TPopulation>
	void GAEvolution<TPopulation>::setPopulation(const TPopulation &p) {
		std::lock_guard<std::mutex> lock(lockPop_);
		population_ = p;
	}

	template <class TPopulation>
	void GAEvolution<TPopulation>::runOnce() {
		//Syncronized
		TPopulation population = getPopulation();

		//Add the elite
		TPopulation elite;
		if (eliteCount_ > 0)
			elite = TPopulation::selectElite(population, eliteCount_);

		//Select offspring
		TPopulation offspring;
		if (childCount_ == 0) {
			offspring = TPopulation::crossover(population, crossoverProb_);
		} else {
			TPopulation fathers, mothers;
			TPopulation::selectParents(population, fathers, mothers, childCount_);
			offspring = TPopulation::crossover(fathers, mothers, crossoverProb_);
		}

		if (mutationProb_ > 0)
			offspring = TPopulation::mutateGauss(offspring, mutationProb_);

		//Evaluate fitness
		offspring = TPopulation::evaluate(offspring, proto_);

		//Select children
		offspring = TPopulation::selectChildren(offspring, parentCount_ - elite.size());

		population = offspring;
		population.add(elite);

		population = TPopulation::sort(population);

		//Syncronized
		setPopulation(population);
	}
}
