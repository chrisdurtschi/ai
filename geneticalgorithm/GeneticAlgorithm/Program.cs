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

            WordGuesser guesser = new WordGuesser(10, 20, 0.5, 0.5, 0.5);
            guesser.Guess(word);

            Console.In.ReadLine();
        }
    }
}
