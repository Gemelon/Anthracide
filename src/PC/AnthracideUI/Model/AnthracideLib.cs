using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AnthracideUI.Model
{
    static class AnthracideLib
    {
        static public void LOGI(string tag, string message)
        {
            Debug.WriteLine(string.Format("{0}: {1}", tag, message));
        }
    }
}
