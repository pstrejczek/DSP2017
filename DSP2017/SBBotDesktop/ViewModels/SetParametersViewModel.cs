using System.ComponentModel;
using System.Runtime.CompilerServices;
using SBBotDesktop.Annotations;

namespace SBBotDesktop.ViewModels
{
    class SetParametersViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
