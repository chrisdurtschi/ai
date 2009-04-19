using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BackPropagation
{
    public class Pattern
    {
        List<double> _input = new List<double>();
        List<double> _output = new List<double>();

        public List<double> Input 
        {
            get { return _input; }
            set { _input = value; }
        }

        public List<double> Output
        {
            get { return _output; }
            set { _output = value; }
        }
    }
}
