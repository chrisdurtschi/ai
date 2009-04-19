using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BackPropagation
{
    public class LayerList : List<Layer>
    {
        public LayerList() : base() { }

        public Layer InputLayer
        {
            get { return this[0]; }
        }

        public Layer OutputLayer
        {
            get { return this[this.Count - 1]; }
        }

        public List<Layer> HiddenLayers
        {
            get { return this.GetRange(1, this.Count - 2); }
        }
    }
}
