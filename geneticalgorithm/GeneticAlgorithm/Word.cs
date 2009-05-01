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
    public class Word : IComparable<Word>
    {
        char[] _letters;
        int _fitness;

        public char[] Letters
        {
            get { return _letters; }
            set { _letters = value; }
        }

        public int Fitness
        {
            get { return _fitness; }
            set { _fitness = value; }
        }

        public Word(char[] letters)
        {
            _letters = letters;
        }

        public int CompareTo(Word other)
        {
            if (this.Fitness < other.Fitness)
                return 1;
            else if (this.Fitness > other.Fitness)
                return -1;
            else
                return 0;
        }

        public override string ToString()
        {
            StringBuilder word = new StringBuilder();
            foreach (char letter in _letters)
                word.Append(letter);
            return word.ToString();
        }
    }
}
