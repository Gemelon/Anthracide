using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace AnthracideUI.ViewModel
{
    internal class BaseViewModel: INotifyPropertyChanged
    {
        #region INotifyPropertyChanged Members  

        public event PropertyChangedEventHandler? PropertyChanged;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="propertyName"> 
        /// [CallerMemberName] = Wenn der PropertyName nicht explicid mitgegeben wird dann wird automatisch der name der 
        /// aufrufenden Funktion oder des Property mitgegeben
        /// </param>
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = "")
        {
            if (!string.IsNullOrEmpty(propertyName))
            {
                this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
        }
        #endregion
    }
}
