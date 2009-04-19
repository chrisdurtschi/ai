using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BackPropagation
{
    public class Neuron
    {
        double _activationValue = 1.0;

        public double ActivationValue
        {
            get { return _activationValue; }
            set { _activationValue = value; }
        }
    }
}
