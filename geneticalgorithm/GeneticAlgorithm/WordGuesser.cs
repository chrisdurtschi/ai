/*
 * Chris Durtschi
 * Artificial Intelligence
 * Genetic Algorithm
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeneticAlgorithm
{
    public class WordGuesser
    {
        int _populationSize;
        int _maxGenerations;
        double _crossoverProbability;
        double _mutationProbability;
        int _totalFitness;

        string _word;
        char[] _letters;
        List<Word> _population;
        Random _rng = new Random();

        public WordGuesser(int populationSize, int maxGenerations, double crossoverProbability, double mutationProbability)
        {
            if (populationSize <= 0)
                throw new ArgumentException("Population size and max generations must be greater than 0");
            if (crossoverProbability < 0.0 || crossoverProbability > 1.0 ||
                mutationProbability < 0.0 || mutationProbability > 1.0)
                throw new ArgumentException("Probabilities must be between 0.0 and 1.0");

            _populationSize = populationSize;
            _maxGenerations = maxGenerations;
            _crossoverProbability = crossoverProbability;
            _mutationProbability = mutationProbability;
        }

        public string Guess(string word)
        {
            _word = word;
            _letters = _word.ToLower().ToCharArray();
            _population = this.InitializePopulation();
            this.CalculateFitnessOfPopulation();

            for (int i = 0; i < _maxGenerations; i++)
            {
                this.Reproduce();
                int crossovers = this.Crossover();
                int mutations = this.Mutate();
                this.CalculateFitnessOfPopulation();
                Console.Out.WriteLine(String.Format("Crossovers: {0}; Mutations: {1}; Fitness: {2}; Best: {3}", crossovers, mutations, _population[0].Fitness, _population[0].ToString()));
                if (_population[0].ToString() == _word)
                {
                    Console.Out.WriteLine("Solution found in " + (i + 1) + " generations");
                    break;
                }
            }

            return String.Empty;
        }

        List<Word> InitializePopulation()
        {
            List<Word> population = new List<Word>();
            for (int i = 0; i < _populationSize; i++)
            {
                population.Add(this.GetRandomWord());
            }
            return population;
        }

        void CalculateFitnessOfPopulation()
        {
            _totalFitness = 0;
            foreach (Word word in _population)
            {
                word.Fitness = this.Fitness(word.Letters);
                _totalFitness += word.Fitness;
            }
            _population.Sort();
        }

        void Reproduce()
        {
            int cutoff = _rng.Next(_totalFitness);
            int totalFitness = 0;
            int i;

            for (i = 0; i < _populationSize; i++)
            {
                totalFitness += _population[i].Fitness;
                if (cutoff < totalFitness)
                    break;
            }
            
            for (int j = i; j < _populationSize; j++)
                _population[j] = this.GetRandomWord();
        }

        int Crossover()
        {
            int crossovers = 0;

            foreach (Word word in _population)
            {
                if (_rng.NextDouble() > _crossoverProbability)
                    continue;

                Word randomWord = null;
                do
                {
                    randomWord = this.GetRandomMemberOfPopulation(_population);
                } while (randomWord == word);

                crossovers++;
                int crossover = _rng.Next(1, _letters.Length - 1);
                for (int i = 0; i < _letters.Length; i++)
                {
                    if (i >= crossover)
                    {
                        char letter = word.Letters[i];
                        word.Letters[i] = randomWord.Letters[i];
                        randomWord.Letters[i] = letter;
                    }
                }
            }

            return crossovers;
        }

        int Mutate()
        {
            int mutations = 0;

            foreach (Word word in _population)
            {
                if (_rng.NextDouble() > _mutationProbability)
                    continue;

                mutations++;
                int randomIndex = _rng.Next(0, _letters.Length - 1);
                word.Letters[randomIndex] = this.GetRandomLetter();
            }

            return mutations;
        }

        int Fitness(char[] letters)
        {
            int fitness = 0;
            for (int i = 0; i < _letters.Length; i++)
            {
                //fitness += Math.Abs(Convert.ToInt32(letters[i]) - Convert.ToInt32(_letters[i]));
                if (letters[i] == _letters[i])
                    fitness += 1;
            }
            return fitness;
        }

        Word GetRandomMemberOfPopulation(List<Word> population)
        {
            int randomIndex = _rng.Next(0, population.Count - 1);
            return population[randomIndex];
        }

        Word GetRandomMemberOfPopulation(List<Word> population, int totalFitness)
        {
            int randomFitness = _rng.Next(totalFitness);
            int fitnessCounter = 0;
            foreach (Word word in population)
            {
                fitnessCounter += word.Fitness;
                if (fitnessCounter >= randomFitness)
                    return word;
            }
            return null;
        }

        Word GetRandomWord()
        {
            char[] word = new char[_letters.Length];
            for (int j = 0; j < _letters.Length; j++)
                word[j] = this.GetRandomLetter();
            return new Word(word);
        }

        char GetRandomLetter()
        {
            // a = 97, z = 122
            return Convert.ToChar(_rng.Next(97, 122));
        }
    }
}
