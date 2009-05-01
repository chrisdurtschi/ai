/*
 * Chris Durtschi
 * Artificial Intelligence
 * Genetic Algorithm
 * 
 * 
 * 
 * This program attemps to evolve a word the user chooses
 * starting from a population of random words.
 * It weighs the fitness of a word by how many letters
 * are *exactly* correct.  A correct letter gets a 1,
 * and incorrect letter gets a 0.
 * 
 * This algorithm seems to work fine for short words,
 * of 5 letters or less.  For larger words, it didn't do so well.
 * 
 * This is probably not the ideal application for a genetic algorithm.
 * It has a very specific goal (there is a best solution - find the correct word).
 * There is only a single property to compare the fitness of the solution,
 * but 26 different variations on that property.
 * 
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace GeneticAlgorithm
{
    class Program
    {
        static void Main(string[] args)
        {
            string word = String.Empty;
            while (String.IsNullOrEmpty(word))
            {
                Console.Out.Write("Enter a word, any word: ");
                string input = Console.In.ReadLine();
                if (Regex.IsMatch(input, "([a-z]|[A-Z])+"))
                    word = input;
                else
                    Console.Out.WriteLine("Word must contain only letters A-Z");
            }

            WordGuesser guesser = new WordGuesser(1000, 10000, 0.2, 0.6);
            guesser.Guess(word);

            Console.In.ReadLine();
        }
    }
}
