using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace AnthracideUI.ViewModel
{
    internal class RelayCommand : ICommand
    {
        readonly Action<object> execute;
        readonly Predicate<object> canExecute;

        public RelayCommand(Action<object> execute, Predicate<object>? canExecute) =>
            (this.execute, this.canExecute) = (execute, canExecute);

        public RelayCommand(Action<object> execute) : this(execute, null) { }

        public event EventHandler? CanExecuteChanged;

        public void RaiseCanExecuteChanged() => this.CanExecuteChanged?.Invoke(this, EventArgs.Empty);

        public bool CanExecute(object? parameter) => this.canExecute?.Invoke(parameter) ?? true;


        public void Execute(object? parameter) => this.execute?.Invoke(parameter);
    }
}
