using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BackPropagation
{
    public class Layer
    {
        double _bias;
        List<Neuron> _neurons = new List<Neuron>();

        public List<Neuron> Neurons
        {
            get { return _neurons; }
        }

        public Layer(int numNeurons, double bias)
        {
            _bias = bias;

            for (int i = 0; i < numNeurons; i++)
            {
                _neurons.Add(new Neuron());
            }
        }
    }
}
