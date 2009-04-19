using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BackPropagation
{
    public class Network
    {
        LayerList _layers = new LayerList();
        List<Pattern> _patterns = new List<Pattern>();
        List<List<List<double>>> _weights = new List<List<List<double>>>();
        Random _rng = new Random();

        public Network(params int[] layers)
        {
            // Create each layer with specified number of neurons
            foreach (int neurons in layers)
            {
                _layers.Add(new Layer(neurons, 1.0));
            }

            // Add weighting for connections between neurons
            for (int i = 0; i < _layers.Count - 1; i++)
            {
                _weights.Add(new List<List<double>>());

                for (int j = 0; j < _layers[i].Neurons.Count; j++)
                {
                    _weights[i].Add(new List<double>());

                    for (int k = 0; k < _layers[i + 1].Neurons.Count; k++)
                        _weights[i][j].Add(this.RandomWeight());
                }
            }
        }

        public void AddPattern(Pattern pattern)
        {
            if (pattern.Input.Count != _layers.InputLayer.Neurons.Count ||
                pattern.Output.Count != _layers.OutputLayer.Neurons.Count)
                throw new ArgumentException("The pattern must match the number of neurons in the input and output layers");

            _patterns.Add(pattern);
        }

        public void Cycle()
        {
            this.Cycle(1);
        }

        public void Cycle(int times)
        {
            foreach (Pattern pattern in _patterns)
            {
                this.Forward(pattern);
            }
        }

        void Forward(Pattern pattern)
        {
            this.SetInputLayer(pattern);

            for (int i = 0; i < _weights.Count; i++)
            {
                // Calculate Net.j = SUM.i  W.i.j * a.i + Bias.j 
                double net = 0.0;
                
        
            }
        }

        void SetInputLayer(Pattern pattern)
        {
            for (int i = 0; i < _layers.InputLayer.Neurons.Count; i++)
            {
                _layers.InputLayer.Neurons[i].ActivationValue = pattern.Input[i];
            }
        }

        double ActivationFunction(double netInput)
        {
            return 1.0 / (1.0 + (Math.Exp((-1.0 * netInput))));
        }

        double RandomWeight()
        {
            return (_rng.NextDouble() * 2.0) - 1.0;
        }
    }
}
