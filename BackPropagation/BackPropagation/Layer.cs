using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BackPropagation
{
    public class Layer
    {
        double _bias;
        List<double> _neurons = new List<double>();

        public double Bias
        {
            get { return _bias; }
            set { _bias = value; }
        }

        public List<double> Neurons
        {
            get { return _neurons; }
        }

        public Layer(int numNeurons, double bias)
        {
            _bias = bias;

            for (int i = 0; i < numNeurons; i++)
            {
                _neurons.Add(1.0);
            }
        }
    }
}
