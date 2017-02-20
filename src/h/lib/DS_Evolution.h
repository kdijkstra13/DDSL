#pragma once

#include "h/DS_Types.h"

#include <vector>
#include <functional>

namespace DSEvolution {
	//****************//
	//** Individual **//
	//****************//
	class Individual {
	public:
		void clone(const Individual &a);
		void clone(Individual &&a);
		~Individual(){};
		Individual(){};
		Individual(const Individual &a) {clone(a);};
		Individual(Individual &&a) {clone(std::move(a));};
		Individual & operator=(const Individual &a) {clone(a);}

		virtual void randomize() = 0;
	};

	template <typename T>
	class RealIndividual : public Individual {
	private:
		std::vector<T> mins_;
		std::vector<T> maxs_;
		std::vector<T> values_;
		std::vector<DSTypes::String> names_;
	public:
		void clone(const RealIndividual<T> &a);
		void clone(RealIndividual<T> &&a);
		~RealIndividual(){};
		RealIndividual(){};
		RealIndividual(const RealIndividual<T> &a) {clone(a);};
		RealIndividual(RealIndividual<T> &&a) {clone(std::move(a));};
		RealIndividual<T> & operator=(const RealIndividual<T> &a) {clone(a);return *this;}
		RealIndividual<T> operator=(RealIndividual<T> &&a) {clone(std::move(a));return *this;}

		virtual void randomize() override;

		std::vector<T> getNames() const;
		std::vector<T> getValues() const;
		std::vector<T> getMins() const;
		std::vector<T> getMaxs() const;
		T getMin(const DSTypes::UInt32 idx) const;
		T getMax(const DSTypes::UInt32 idx) const;
		DSTypes::String getName(const DSTypes::UInt32 idx) const;
		T getValue(const DSTypes::UInt32 idx) const;
		T getValue(const DSTypes::String &s) const;
		void setValue(const DSTypes::UInt32 idx, const T value);

		void setParams(const std::vector<DSTypes::String> &names, const std::vector<T> &mins, const std::vector<T> &maxs, const std::vector<T> &values);
		void setParam(const DSTypes::UInt32 idx, const DSTypes::String &name, const T &mins, const T &maxs, const T &value);
		void addParam(const DSTypes::String &name, const T &min, const T &max, const T &value);

		DSTypes::UInt32 size() const;
		bool check(const RealIndividual<T> &b, const bool except = true) const;
		static RealIndividual<T> crossover(const RealIndividual<T> &a, const RealIndividual<T> &b, const DSTypes::Double prob = 0.5);
		static RealIndividual<T> mutate(const RealIndividual<T> &a, const DSTypes::Double prob = 0.1);
		static RealIndividual<T> mutateGauss(const RealIndividual<T> &a, const DSTypes::Double prob = 0.1);

		void print(std::ostream &s = std::cout) const;
		
		T operator[](const DSTypes::String &name) const;
		T operator[](const DSTypes::UInt32 &idx) const;
	};

	//***************//
	//** Objective **//
	//***************//
	class Objective {
	public:
		void clone(const Objective &a);
		void clone(Objective &&a);
		~Objective(){};
		Objective(){};
		Objective(const Objective &a) {clone(a);};
		Objective(Objective &&a) {clone(std::move(a));};
		Objective & operator=(const Objective &a) {clone(a);}
	};

	template <typename T>
	class MultiObjective : public Objective {
	private:
		std::vector<T> scores_;
	public:
		void clone(const MultiObjective<T> &a);
		void clone(MultiObjective<T> &&a);
		~MultiObjective(){};
		MultiObjective(){};
		MultiObjective(const MultiObjective<T> &a) {clone(a);};
		MultiObjective(MultiObjective<T> &&a) {clone(std::move(a));};
		MultiObjective<T> & operator=(const MultiObjective<T> &a) {clone(a);}
		MultiObjective<T> operator=(MultiObjective<T> &&a) {clone(std::move(a));}

		void setScores(const std::vector<T> &os);
		std::vector<T> getScores() const;
		void setScore(const DSTypes::UInt32 idx, const T s);
		T getScore(const DSTypes::UInt32 idx) const;
		void addScore(const T s);
	};

	template <typename T>
	class SingleObjective : public Objective {
	private:
		T score_;
	public:
		void clone(const SingleObjective<T> &a);
		void clone(SingleObjective<T> &&a);
		~SingleObjective(){};
		SingleObjective(){};
		SingleObjective(const SingleObjective<T> &a) {clone(a);};
		SingleObjective(SingleObjective<T> &&a) {clone(std::move(a));};
		SingleObjective<T> & operator=(const SingleObjective<T> &a) {clone(a);return *this;}
		SingleObjective<T> operator=(SingleObjective<T> &&a) {clone(std::move(a));return *this;}

		bool operator>(const SingleObjective &a) const;
		bool operator<(const SingleObjective &a) const;
		bool operator>=(const SingleObjective &a) const;
		bool operator<=(const SingleObjective &a) const;
		bool operator==(const SingleObjective &a) const;
		bool operator!=(const SingleObjective &a) const;

		void operator=(const T &s);

		void setScore(const T s);
		T getScore() const;
		void print(std::ostream &s = std::cout) const;
	};

	//****************//
	//** Population **//
	//****************//	
	class Population {
	public:
		void clone(const Population &a);
		void clone(Population &&a);
		~Population(){};
		Population(){};
		Population(const Population &a) {clone(a);};
		Population(Population &&a) {clone(std::move(a));};
		Population & operator=(const Population &a) {clone(a);}
	};

	template <class TIndividual, class TObjective>
	class StandardPopulation : public Population {
	private:
		std::vector<TIndividual> individuals_;
		std::vector<TObjective> objectives_;
		std::function<TObjective(const TIndividual &)> fitness_;
		std::function<StandardPopulation<TIndividual, TObjective>(const StandardPopulation<TIndividual, TObjective> &)> evaluation_;
	public:
		DSTypes::UInt32 size() const;
		void clear();
		void print(std::ostream &s = std::cout) const;
		void add(const StandardPopulation<TIndividual, TObjective> &p);
		
		void setFitnessFunction(const std::function<TObjective(const TIndividual &)> &f);
		const std::function<TObjective(const TIndividual &)> & getFitnessFunction() const;
		
		void setEvaluationFunction(const std::function<StandardPopulation<TIndividual, TObjective>(const StandardPopulation<TIndividual, TObjective> &)> &f);
		const std::function<StandardPopulation<TIndividual, TObjective>(const StandardPopulation<TIndividual, TObjective> &)> & getEvaluationFunction() const;

		void addIndividual(const TIndividual &a, const TObjective &o);
		void addIndividual(const std::pair<TIndividual, TObjective> &i);
		std::pair<TIndividual, TObjective> getIndividual(const DSTypes::UInt32 idx) const;
		void getIndividual(const DSTypes::UInt32 idx, TIndividual &a, TObjective &o) const;
		void getIndividuals(std::vector<TIndividual> &a, std::vector<TObjective> &o) const;
		std::vector<std::pair<TIndividual, TObjective>> getIndividuals() const;
		void addIndividuals(const std::vector<std::pair<TIndividual, TObjective>> &a);

		void clone(const StandardPopulation<TIndividual, TObjective> &a);
		void clone(StandardPopulation<TIndividual, TObjective> &&a);
		~StandardPopulation(){};
		StandardPopulation(){};
		StandardPopulation(const std::function<TObjective(const TIndividual &)> &fitness);

		StandardPopulation(const StandardPopulation<TIndividual, TObjective> &a) {clone(a);};
		StandardPopulation(StandardPopulation<TIndividual, TObjective> &&a) {clone(std::move(a));};
		StandardPopulation<TIndividual, TObjective> & operator=(const StandardPopulation<TIndividual, TObjective> &a) {clone(a);return *this;}
		StandardPopulation<TIndividual, TObjective> operator=(StandardPopulation<TIndividual, TObjective> &&a) {clone(std::move(a));return *this;}
		
		static StandardPopulation<TIndividual, TObjective> sort(const StandardPopulation<TIndividual, TObjective> &a);
		static StandardPopulation<TIndividual, TObjective> select(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::UInt32 start, const DSTypes::UInt32 count);
		static StandardPopulation<TIndividual, TObjective> evaluate(const StandardPopulation<TIndividual, TObjective> &a, const StandardPopulation<TIndividual, TObjective> &evaluator);

		static StandardPopulation<TIndividual, TObjective> initPopulation(const StandardPopulation<TIndividual, TObjective> &proto, const DSTypes::UInt32 n = 10);
		static StandardPopulation<TIndividual, TObjective> selectElite(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::UInt32 n = 10);
		static StandardPopulation<TIndividual, TObjective> selectElite(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::Double prob = 0);
		static void selectParents(const StandardPopulation<TIndividual, TObjective> &a, StandardPopulation<TIndividual, TObjective> &fathers, StandardPopulation<TIndividual, TObjective> &mothers, const DSTypes::UInt32 n = 100);
		static StandardPopulation<TIndividual, TObjective> selectChildren(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::UInt32 n = 10);

		static StandardPopulation<TIndividual, TObjective> crossover(const StandardPopulation<TIndividual, TObjective> &fathers, const StandardPopulation<TIndividual, TObjective> &mothers, const DSTypes::Double crossoverProb = 0.5);
		static StandardPopulation<TIndividual, TObjective> crossover(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::Double crossoverProb = 0.5);
		static StandardPopulation<TIndividual, TObjective> mutate(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::Double mutationProb = 0.1);		
		static StandardPopulation<TIndividual, TObjective> mutateGauss(const StandardPopulation<TIndividual, TObjective> &a, const DSTypes::Double sigma = 0.1);
	};

	//***************//
	//** Evolution **//
	//***************//
	class Evolution {
	private:
		DSTypes::UInt32 currentGeneration_;
		DSTypes::UInt32 maxGenerations_;
		DSTypes::UInt32 lastGeneration_;
		std::future<void> future_;
		std::mutex lockMaxGen_;
		std::mutex lockCurrGen_;
		void run_();
	protected:
		void incCurrentGeneration();
		virtual void init() = 0;
		virtual void runOnce() = 0;
	public:
		void setMaxGenerations(const DSTypes::UInt32 mg);
		DSTypes::UInt32 getMaxGenerations();
		DSTypes::UInt32 getCurrentGeneration();
		bool hasNewData();

		void clone(const Evolution &a);
		void clone(Evolution &&a);
		virtual ~Evolution(){};
		Evolution();
		Evolution(const Evolution &a) {clone(a);};
		Evolution(Evolution &&a) {clone(std::move(a));};
		Evolution & operator=(const Evolution &a) {clone(a);}

		void run(DSTypes::UInt32 generations);
		void stop();
		bool wait(DSTypes::UInt32 ms = 0);
	};

	template <class TPopulation>
	class GAEvolution : public Evolution {
	private:
		TPopulation proto_; //The proto population (should only contain one Individual or parentCount individuals)
		TPopulation population_; //The current population

		DSTypes::Double crossoverProb_; //Probability of a param crossing over (def=0.5) 
		DSTypes::Double mutationProb_; //Factor of param range used as the sigma for a gaussian distribution (def=0.1)

		DSTypes::UInt32 eliteCount_; //The number of individuals to copy to the child population (ordered)
		DSTypes::UInt32 parentCount_; //The number of individuals after selection
		DSTypes::UInt32 childCount_; //The number individuals before selection (if 0 then parentCount ^ 2)

		//childCount should be bigger than parentCount, otherwise the population will die out.
		//eliteProb and eliteCount can be used simultanuously to add additional parents to the child population.		
		//if childCount is zero, it will be set to parentCount^2 and parent selection will be disabled.
		std::mutex lockPop_;
	protected:
		virtual void init() override;
		virtual void runOnce() override;		
	public:
		void clone(const GAEvolution<TPopulation> &a);
		void clone(GAEvolution<TPopulation> &&a);
		~GAEvolution(){};
		GAEvolution(){};
		GAEvolution(const TPopulation &proto,
					const DSTypes::Double crossoverProb = 0.5, 
					const DSTypes::Double mutationProb = 0.1, 
					const DSTypes::UInt32 eliteCount = 1, 
					const DSTypes::UInt32 parentCount = 10, 
					const DSTypes::UInt32 childCount = 50);
		GAEvolution(const GAEvolution<TPopulation> &a) {clone(a);};
		GAEvolution(GAEvolution<TPopulation> &&a) {clone(std::move(a));};
		GAEvolution<TPopulation> & operator=(const GAEvolution<TPopulation> &a) {clone(a);}
		GAEvolution<TPopulation> operator=(GAEvolution<TPopulation> &&a) {clone(std::move(a));}

		TPopulation getPopulation();
		void setPopulation(const TPopulation &p);

		void setMutationProb(const DSTypes::Double &p) {mutationProb_ = p;}
		DSTypes::Double getMutationProb() {return mutationProb_;}
	};
}