using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BackPropagation
{
    class Program
    {
        static void Main(string[] args)
        {
            //for (double i = -9.0; i < 9.0; i += 0.1)
            //{
            //    Console.Out.WriteLine(Network.ActivationFunction(i));
            //}

            //Console.Out.WriteLine(Network.ActivationFunction(0.47));

            Network network = new Network(2, 2, 1);
            network.AddPattern(new Pattern { Input = { 0, 0 }, Output = { 0 } });

            Random random = new Random();
            for (int i = 0; i < 1000; i++)
                Console.Out.WriteLine((random.NextDouble() * 2.0) - 1.0);

            Console.In.ReadLine();
        }
    }
}
