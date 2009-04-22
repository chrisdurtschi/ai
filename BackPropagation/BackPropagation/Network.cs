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
        List<List<double>> _deltas = new List<List<double>>();
        double _learningRate;
        double _weightFactor;
        int _printErrorRate;
        Random _rng = new Random();

        /// <summary>
        /// Initializes the neural network
        /// </summary>
        /// <param name="learningRate">
        /// Sets the learning rate that is applied when weightings are modified on backpropagation.
        /// Should be around 0.1 - 0.25
        /// </param>
        /// <param name="weightFactor">
        /// Sets the range for the initial random weightings.
        /// A weightFactor of 2.0 will produce random weights in the range of -2.0 to 2.0
        /// </param>
        /// <param name="printErrorRate">
        /// You can print the error between the pattern output and the actual output every so many cycles.
        /// Enter 0 to not print errors.
        /// </param>
        /// <param name="layers">
        /// The number of neurons per layer.
        /// To create a network with 2 neurons in the input layer, 
        /// two hidden layers with 3 and 2 neurons, 
        /// and an output layer with 1 neuron, you would enter: 
        /// 2, 3, 2, 1
        /// </param>
        public Network(double learningRate, double weightFactor, int printErrorRate, params int[] layers)
        {
            _learningRate = learningRate;
            _weightFactor = weightFactor;
            _printErrorRate = printErrorRate;

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

            // Initialize learning deltas to 0.0
            for (int i = 1; i < _layers.Count; i++)
            {
                _deltas.Add(new List<double>());
                for (int j = 0; j < _layers[i].Neurons.Count; j++)
                {
                    _deltas[i - 1].Add(0.0);
                }
            }
        }

        public void PrintOutput()
        {
            foreach (Pattern pattern in _patterns)
            {
                this.MoveForward(pattern);

                string display = "Input = [";
                foreach (double input in pattern.Input)
                    display += String.Format("{0:0.000}", input) + ",";
                display = display.Remove(display.Length - 1, 1);
                display += "];\nOutput = [";
                foreach (double output in _layers.OutputLayer.Neurons)
                    display += String.Format("{0:0.000}", output) + ",";
                display = display.Remove(display.Length - 1, 1);
                display += "]\n";
                Console.Out.WriteLine(display);
            }
        }

        void PrintError(Pattern pattern)
        {
            double error = 0.0;
            for (int num = 0; num < pattern.Output.Count; num++)
                error += pattern.Output[num] - _layers.OutputLayer.Neurons[num];
            error = error / pattern.Output.Count;
            Console.Out.WriteLine("Error = " + error);
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
            for (int i = 0; i < times; i++)
            {
                List<Pattern> patterns = this.ShufflePatterns();

                foreach (Pattern pattern in patterns)
                {
                    this.MoveForward(pattern);
                    this.MoveBackward(pattern);
                    this.CalculateAdjustedWeightings();

                    if (_printErrorRate != 0 && i % _printErrorRate == 0)
                        this.PrintError(pattern);
                }
            }
        }

        List<Pattern> ShufflePatterns()
        {
            return _patterns.OrderBy(x => Guid.NewGuid()).ToList();
        }

        void MoveForward(Pattern pattern)
        {
            this.SetInputLayer(pattern);

            for (int i = 0; i < _weights.Count; i++)
            {
                // Calculate Net.j = SUM.i  W.i.j * a.i + Bias.j 
                double netInput = 0.0;
                
                for (int j = 0; j < _layers[i + 1].Neurons.Count; j++)
                {
                    for (int k = 0; k < _layers[i].Neurons.Count; k++)
                    {
                        netInput += _layers[i].Neurons[k] * _weights[i][k][j]; // +_layers[i + 1].Bias;
                    }

                    // Calculate a.j = f(Net.j)
                    _layers[i + 1].Neurons[j] = this.ActivationFunction(netInput);
                }
            }
        }
                
        void MoveBackward(Pattern pattern)
        {
            this.CalculateLearningDeltas(pattern);
            this.BackpropagateErrors();
        }

        void CalculateAdjustedWeightings()
        {
            // DELTA W.i.j = EPSILON d.j * a.i
            // DELTA Bias.j = EPSILON d.j

            for (int n = _weights.Count - 1; n >= 0; n--)
            {
                for (int i = 0; i < _weights[n].Count; i++)
                {
                    for (int j = 0; j < _weights[n][i].Count; j++)
                    {
                        _weights[n][i][j] += _learningRate * _deltas[n][j] * _layers[n].Neurons[i];
                        //_layers[n + 1].Bias += _learningRate * _deltas[n][j];
                    }
                }
            }
        }

        void CalculateLearningDeltas(Pattern pattern)
        {
            List<double> outputDelta = _deltas[_deltas.Count - 1];

            for (int i = 0; i < _layers.OutputLayer.Neurons.Count; i++)
            {
                // Calculate error.k = t.p - a.k
                double error = pattern.Output[i] - _layers.OutputLayer.Neurons[i];
                
                // d.k = f'(Net.k) * error.k = a.k * (1 - a.k) * error.k
                outputDelta[i] = this.DeltaFunction(_layers.OutputLayer.Neurons[i]) * error;
            }
        }

        void BackpropagateErrors()
        {
            // calculate deltas for hidden layers
            for (int i = _layers.Count - 2; i > 0; i--)
            {
                for (int j = 0; j < _layers[i].Neurons.Count; j++)
                {
                    // d.j = a.j * (1 - a.j) * SUM.k  W.j.k * d.k
                    double error = 0.0;
                    for (int k = 0; k < _layers[i + 1].Neurons.Count; k++)
                    {
                        error += _weights[i][j][k] * _deltas[i][k];
                    }

                    _deltas[i - 1][j] = this.DeltaFunction(_layers[i].Neurons[j]) * error;
                }
            }
        }

        void SetInputLayer(Pattern pattern)
        {
            for (int i = 0; i < _layers.InputLayer.Neurons.Count; i++)
            {
                _layers.InputLayer.Neurons[i] = pattern.Input[i];
            }
        }

        // f(x) = 1 / 1 + e ^ -x
        double ActivationFunction(double netInput)
        {
            return 1.0 / (1.0 + (Math.Exp((-1.0 * netInput))));
        }

        //  f'(Net.k) = a.k * (1 - a.k)
        double DeltaFunction(double activationValue)
        {
            return activationValue * (1.0 - activationValue);
        }

        // used to initialize weights to random value
        double RandomWeight()
        {
            return (_rng.NextDouble() * (2.0 * _weightFactor)) - _weightFactor;
        }
    }
}
