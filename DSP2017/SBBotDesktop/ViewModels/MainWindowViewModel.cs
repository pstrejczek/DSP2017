using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using SBBotDesktop.Annotations;
using SBBotDesktop.Communication;
using SBBotDesktop.Models;

namespace SBBotDesktop.ViewModels
{
    class MainWindowViewModel : INotifyPropertyChanged
    {
        private bool _isConnected = false;
        private RobotWiFiMode _currentRobotWiFiMode;
        private RobotMode _currentRobotMode;

        private ObservableCollection<LogData> _log;

        public ObservableCollection<LogData> Log
        {
            get { return _log; }
            set
            {
                _log = value;
                OnPropertyChanged(nameof(Log));
            }
        }

        public RobotMode CurrentRobotMode
        {
            get { return _currentRobotMode; }
            set
            {
                _currentRobotMode = value;
                OnPropertyChanged(nameof(CurrentRobotMode));
            }
        }

        public RobotWiFiMode CurrentRobotWiFiMode
        {
            get { return _currentRobotWiFiMode; }
            set
            {
                _currentRobotWiFiMode = value;
                OnPropertyChanged(nameof(CurrentRobotWiFiMode));
            }
        }

        public bool IsConnected
        {
            get { return _isConnected; }
            set
            {
                _isConnected = value;
                OnPropertyChanged(nameof(IsConnected));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
