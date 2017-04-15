using SBBotDesktop.Annotations;
using SBBotDesktop.Communication;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Input;
using SBBotDesktop.DataContainers;

namespace SBBotDesktop.ViewModels
{
    class SetParametersViewModel : INotifyPropertyChanged
    {
        private List<string> _avaiableNetworks;
        private string _password;
        private string _selectedNetwork;

        public ICommand CUpdateParameters => new RelayCommand(p => this.CUpdateParametersExecute(), p => this.AlwaysTrue());
        public ICommand CClearEeprom => new RelayCommand(p => this.CClearEepromExecute(), p => this.AlwaysTrue());
        public ICommand CCloseWindow => new RelayCommand(p => this.CCloseWindowExecute(), p => this.AlwaysTrue());

        public List<string> AvaiableNetworks
        {
            get { return _avaiableNetworks; }
            set
            {
                _avaiableNetworks = value;
                OnPropertyChanged(nameof(AvaiableNetworks));
            }
        }

        public string Password
        {
            get { return _password; }
            set
            {
                _password = value;
                OnPropertyChanged(nameof(Password));
            }
        }

        public string SelectedNetwork
        {
            get { return _selectedNetwork; }
            set
            {
                _selectedNetwork = value;
                OnPropertyChanged(nameof(SelectedNetwork));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

  
        public SetParametersViewModel()
        {
            _avaiableNetworks = GetAvaiableNetworks();
        }

        private List<string> GetAvaiableNetworks()
        {
            var wo = new WebOperations(ConnectionParameters.RobotIp);
            return wo.GetAvaiableNetworks();
        }

        private void CCloseWindowExecute()
        {
            Application.Current.Windows.Cast<Window>().Single(w => w.DataContext == this).Close();
        }

        private void CClearEepromExecute()
        {
            var wo = new WebOperations(ConnectionParameters.RobotIp);
            wo.ClearEeprom();
        }

        private void CUpdateParametersExecute()
        {
            var wo = new WebOperations(ConnectionParameters.RobotIp);
            wo.SetNetworkCredentials(_selectedNetwork, _password);
        }

        private bool AlwaysTrue()
        {
            return true;
        }

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

    }
}
