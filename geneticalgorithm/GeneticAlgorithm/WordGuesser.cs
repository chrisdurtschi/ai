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

        char[] _letters;
        List<Word> _population;
        Random _rng = new Random();

        public WordGuesser(int populationSize, int maxGenerations, double replacementProbability, double crossoverProbability, double mutationProbability)
        {
            if (populationSize <= 0)
                throw new ArgumentException("Population size and max generations must be greater than 0");
            if (replacementProbability < 0.0 || replacementProbability > 1.0 ||
                crossoverProbability < 0.0 || crossoverProbability > 1.0 ||
                mutationProbability < 0.0 || mutationProbability > 1.0)
                throw new ArgumentException("Probabilities must be between 0.0 and 1.0");

            _populationSize = populationSize;
            _maxGenerations = maxGenerations;
            _crossoverProbability = crossoverProbability;
            _mutationProbability = mutationProbability;
        }

        public string Guess(string word)
        {
            _letters = word.ToLower().ToCharArray();
            this.InitializePopulation();
            this.CalculateFitnessOfPopulation();
            this.Reproduce();


            return String.Empty;
        }

        void InitializePopulation()
        {
            // a = 97, z = 122
            _population = new List<Word>();
            for (int i = 0; i < _populationSize; i++)
            {
                char[] word = new char[_letters.Length];
                for (int j = 0; j < _letters.Length; j++)
                    word[j] = Convert.ToChar(_rng.Next(97, 122));
                _population.Add(new Word(word));
            }
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
            // Select random fitness at which to survive.
            // Replace all losers with random copies of winners.

            int randomFitness = _rng.Next(_totalFitness);
            int cutoff;

            for (cutoff = 0; cutoff < _populationSize; cutoff++)
            {
                if (_population[cutoff].Fitness > cutoff)
                    break;
            }

            for (int i = cutoff; i < _populationSize; i++)
                _population[i] = this.GetRandomMemberOfPopulation(_population.GetRange(0, cutoff));
        }

        void Crossover()
        {
            foreach (Word word in _population)
            {
                if (_rng.NextDouble() > _crossoverProbability)
                    continue;

                Word randomWord = null;
                do
                {
                    randomWord = this.GetRandomMemberOfPopulation(_population);
                } while (randomWord == word);

                int crossover = _rng.Next(1, _letters.Length - 2);
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
        }

        void Mutate()
        {

        }

        int Fitness(char[] letters)
        {
            int fitness = 0;
            for (int i = 0; i < _letters.Length; i++)
            {
                fitness += Math.Abs(Convert.ToInt32(letters[i]) - Convert.ToInt32(_letters[i]));
            }
            return fitness;
        }

        Word GetRandomMemberOfPopulation(List<Word> population)
        {
            int randomIndex = _rng.Next(population.Count - 1);
            return population[randomIndex];
        }
    }
}
