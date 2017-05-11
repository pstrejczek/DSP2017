using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows.Input;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using SBBotMobile.Communication;
using Xamarin.Forms;

namespace SBBotMobile.ViewModel
{
    public class SetParametersViewModel : ViewModelBase
    {
        private List<string> _avaiableNetworks;
        private string _password;
        private string _selectedNetwork;
        private bool _isLoading;
        private bool _loaded;

        public ICommand CUpdateParameters => new RelayCommand(UpdateParameters);
        public ICommand CClearEeprom => new RelayCommand(ClearEeprom);

        public bool IsLoading
        {
            get { return _isLoading; }
            set
            {
                _isLoading = value;
                RaisePropertyChanged();
            }
        }

        public bool Loaded
        {
            get { return _loaded; }
            set
            {
                _loaded = value;
                RaisePropertyChanged();
            }
        }

        public List<string> AvaiableNetworks
        {
            get { return _avaiableNetworks; }
            set
            {
                _avaiableNetworks = value;
                RaisePropertyChanged();
            }
        }

        public string Password
        {
            get { return _password; }
            set
            {
                _password = value;
                RaisePropertyChanged();
            }
        }

        public string SelectedNetwork
        {
            get { return _selectedNetwork; }
            set
            {
                _selectedNetwork = value;
                RaisePropertyChanged();
            }
        }

        public SetParametersViewModel()
        {
            _isLoading = true;
            _loaded = false;
            GetAvaiableNetworks();
        }

        private async void GetAvaiableNetworks()
        {
            AvaiableNetworks = await WebOperations.GetAvaiableNetworks();
            IsLoading = false;
            Loaded = true;
        }

        private async void ClearEeprom()
        {
            AvaiableNetworks = await WebOperations.GetAvaiableNetworks();

            //await WebOperations.ClearEeprom();
            //MessagingCenter.Send(this, "disconnect");
            // Send disconnect message
        }

        private async void UpdateParameters()
        {
            await WebOperations.SetNetworkCredentials(_selectedNetwork, _password);
            MessagingCenter.Send(this, "disconnect");
            // Send disconnect message
        }

    }
}
