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
            /*
             * XOR problem.
             * I seemed to have issues with only 2 neurons in the hidden layer.
             * It never seemed to converge well, with the error calculation on the output
             * oscillating between positive and negative.
             */
            Network network = new Network(0.25, 1.0, 0, 2, 4, 1);
            network.AddPattern(new Pattern { Input = { 0, 0 }, Output = { 0 } });
            network.AddPattern(new Pattern { Input = { 0, 1 }, Output = { 1 } });
            network.AddPattern(new Pattern { Input = { 1, 0 }, Output = { 1 } });
            network.AddPattern(new Pattern { Input = { 1, 1 }, Output = { 0 } });

            network.Cycle(10000);
            network.PrintOutput();



            /*
             * Recognize the digits 0 - 9
             * Represent digits with 4 x 5 grid, making input layer 20 neurons large.
             * Output layer has 10 neurons, one for each digit.
             * Again, this experiment is not entirely successful.
             * The setup below only recognized 0, 1, 2, 5, 6, 7
             */

            //Network network = new Network(0.1, 1.0, 0, 20, 40, 10);
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 1, 1, 1,
            //              1, 0, 0, 1,
            //              1, 0, 0, 1,
            //              1, 0, 0, 1,
            //              1, 1, 1, 1 },
            //    Output = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 0, 0, 0, 1,
            //              0, 0, 0, 1,
            //              0, 0, 0, 1,
            //              0, 0, 0, 1,
            //              0, 0, 0, 1 },
            //    Output = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 1, 1, 1,
            //              0, 0, 0, 1,
            //              1, 1, 1, 1,
            //              1, 0, 0, 0,
            //              1, 1, 1, 1 },
            //    Output = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 1, 1, 1,
            //              0, 0, 0, 1,
            //              1, 1, 1, 1,
            //              0, 0, 0, 1,
            //              1, 1, 1, 1 },
            //    Output = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 0, 0, 1,
            //              1, 0, 0, 1,
            //              1, 1, 1, 1,
            //              0, 0, 0, 1,
            //              0, 0, 0, 1 },
            //    Output = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 1, 1, 1,
            //              1, 0, 0, 0,
            //              1, 1, 1, 1,
            //              0, 0, 0, 1,
            //              1, 1, 1, 1 },
            //    Output = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 1, 1, 1,
            //              1, 0, 0, 0,
            //              1, 1, 1, 1,
            //              1, 0, 0, 1,
            //              1, 1, 1, 1 },
            //    Output = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 1, 1, 1,
            //              0, 0, 0, 1,
            //              0, 0, 0, 1,
            //              0, 0, 0, 1,
            //              0, 0, 0, 1 },
            //    Output = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 1, 1, 1,
            //              1, 0, 0, 1,
            //              1, 1, 1, 1,
            //              1, 0, 0, 1,
            //              1, 1, 1, 1 },
            //    Output = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }
            //});
            //network.AddPattern(new Pattern
            //{
            //    Input = { 1, 1, 1, 1,
            //              1, 0, 0, 1,
            //              1, 1, 1, 1,
            //              0, 0, 0, 1,
            //              0, 0, 0, 1 },
            //    Output = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }
            //});

            //network.Cycle(10000);

            //network.PrintOutput();

            Console.In.ReadLine();
        }
    }
}
