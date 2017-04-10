using SBBotDesktop.Annotations;
using SBBotDesktop.Communication.Enums;
using SBBotDesktop.Models;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Windows.Input;
using SBBotDesktop.Communication;

namespace SBBotDesktop.ViewModels
{
    class MainWindowViewModel : INotifyPropertyChanged
    {
        private bool _isConnected = false;
        private RobotWiFiMode _currentRobotWiFiMode;
        private RobotMode _currentRobotMode;
        private string _robotIp;

        private bool _isForward = false;
        private bool _isLeft = false;
        private bool _isRight = false;
        private bool _isBackward = false;

        private ObservableCollection<LogEntry> _log;

        public ICommand CConnect => new RelayCommand(p => this.CConnectExecute(), p => this.ConnectCanExecute());
        public ICommand CChangeMode => new RelayCommand(p => this.CChangeModeExecute(), p => this.ChangeModeCanExecute());

        public ICommand CForwardStart => new RelayCommand(p => this.CForwadStartExecute(), p => this.MovementCanExecute());
        public ICommand CForwardStop => new RelayCommand(p => this.CForwadStopExecute(), p => this.MovementCanExecute());
        public ICommand CLeftStart => new RelayCommand(p => this.CLeftStartExecute(), p => this.MovementCanExecute());
        public ICommand CLeftStop => new RelayCommand(p => this.CLeftStopExecute(), p => this.MovementCanExecute());
        public ICommand CRightStart => new RelayCommand(p => this.CRightStartExecute(), p => this.MovementCanExecute());
        public ICommand CRightStop => new RelayCommand(p => this.CRightStopExecute(), p => this.MovementCanExecute());
        public ICommand CBackwardStart => new RelayCommand(p => this.CBackwardStartExecute(), p => this.MovementCanExecute());
        public ICommand CBackwardStop => new RelayCommand(p => this.CBackwardStopExecute(), p => this.MovementCanExecute());

    

        public event PropertyChangedEventHandler PropertyChanged;

        public ObservableCollection<LogEntry> Log
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

        public void AddToLog(string message)
        {
            if (Log == null) Log = new ObservableCollection<LogEntry>();

            var le = new LogEntry
            {
                dateTime = DateTime.Now,
                robotEvent = message
            };
            Log.Add(le);
        }

        private async void CForwadStartExecute()
        {
            if (!_isConnected) return;
            _isForward = true;
            var uco = new UdpCommOperations(_robotIp);
            while (_isForward)
                await Task.Run(() => uco.SendCommand(UdpRobotCommand.Forward));
        }

        private void CForwadStopExecute()
        {
            _isForward = false;
        }


        private async void CLeftStartExecute()
        {
            if (!_isConnected) return;
            _isLeft = true;
            var uco = new UdpCommOperations(_robotIp);
            while(_isLeft)
                await Task.Run(() => uco.SendCommand(UdpRobotCommand.Left));
        }

        private void CLeftStopExecute()
        {
            _isLeft = false;
        }

        private async void CRightStartExecute()
        {
            if (!_isConnected) return;
            _isRight = true;
            var uco = new UdpCommOperations(_robotIp);
            while(_isRight)
                await Task.Run(() => uco.SendCommand(UdpRobotCommand.Right));
        }

        private void CRightStopExecute()
        {
            _isRight = false;
        }

        private async void CBackwardStartExecute()
        {
            if (!_isConnected) return;
            _isBackward = true;
            var uco = new UdpCommOperations(_robotIp);
            while(_isBackward)
                await Task.Run(() => uco.SendCommand(UdpRobotCommand.Backward));
        }

        private void CBackwardStopExecute()
        {
            _isBackward = false;
        }

        private void CConnectExecute()
        {
            ConnectRobot();
        }

        private void CChangeModeExecute()
        {
            ChangeRobotMode();
        }

        private void ChangeRobotMode()
        {
            if (!_isConnected) return;

            var uco = new UdpCommOperations(_robotIp);
            if (CurrentRobotMode == RobotMode.Automatic)
            {
                AddToLog("Switching mode to MANUAL");
                uco.SendCommand(UdpRobotCommand.Manual);
            }
            if (CurrentRobotMode == RobotMode.Manual)
            {
                AddToLog("Switching mode to AUTO");
                uco.SendCommand(UdpRobotCommand.Auto);
            }

            var wo = new WebOperations(_robotIp);
            CurrentRobotMode = wo.GetCurrentRobotMode();
        }

        private async void ConnectRobot()
        {
            AddToLog("Searching for robot ...");

            var zco = new ZeroConfOperations();
            var robotIp = await zco.GetRobotIp();

            AddToLog($"Robot found at: {robotIp}");

            AddToLog("Checking connection...");

            _robotIp = robotIp;

            var wo = new WebOperations(robotIp);
            var rmode = wo.GetCurrentRobotMode();

            if (rmode == RobotMode.Error)
            {
                AddToLog("Connection ERROR !");
            }
            else
            {
                IsConnected = true;
                CurrentRobotMode = rmode;
                AddToLog("Connection succesfull");
            }
        }

        private bool MovementCanExecute()
        {
            return true;
        }

        private bool ConnectCanExecute()
        {
            return true;
        }

        private bool ChangeModeCanExecute()
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
