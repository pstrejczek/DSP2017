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
using SBBotDesktop.DataContainers;
using SBBotDesktop.Views;

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
        private UdpCommOperations _udpCommOps;

        public ICommand CConnect => new RelayCommand(p => this.CConnectExecute(), p => this.ConnectCanExecute());
        public ICommand CSetParameters => new RelayCommand(p => this.CSetParametersExecute(), p => this.CSetParametersCanExecute());
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

        private void CForwadStartExecute()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Forward);
        }

        private void CForwadStopExecute()
        {
            StopMovement();
        }


        private void CLeftStartExecute()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Left);
        }

        private void CLeftStopExecute()
        {
            StopMovement();
        }

        private void CRightStartExecute()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Right);
        }

        private void CRightStopExecute()
        {
            _isRight = false;
            StopMovement();
        }

        private void CBackwardStartExecute()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Backward);
        }

        private void StopMovement()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Stop);
        }

        private void CBackwardStopExecute()
        {
            _isBackward = false;
            StopMovement();
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
            var lastMode = CurrentRobotMode;

            if (CurrentRobotMode == RobotMode.Automatic)
            {
                AddToLog("Switching mode to MANUAL");
                _udpCommOps.SendCommand(UdpRobotCommand.Manual);
            }
            if (CurrentRobotMode == RobotMode.Manual)
            {
                AddToLog("Switching mode to AUTO");
                _udpCommOps.SendCommand(UdpRobotCommand.Auto);
            }

            var wo = new WebOperations(_robotIp);
            var counter = 0;

            while (lastMode == CurrentRobotMode && counter < 10)
            {
                CurrentRobotMode = wo.GetCurrentRobotMode();
                counter++;
            }
        }

        private async void ConnectRobot()
        {
            AddToLog("Searching for robot ...");

            var zco = new ZeroConfOperations();
            var robotIp = await zco.GetRobotIp();

            if (string.IsNullOrEmpty(robotIp))
            {
                AddToLog("Robot not found. Maybe it is in ACCESS POINT mode. Check networks for SANDWICHBOXBOT");
                return;
            }

            ConnectionParameters.RobotIp = robotIp;

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
                _udpCommOps = new UdpCommOperations(robotIp);
                AddToLog("Connection succesfull");
            }
        }

        private async void CSetParametersExecute()
        {
            if (string.IsNullOrEmpty(ConnectionParameters.RobotIp))
            {
                var zco = new ZeroConfOperations();
                ConnectionParameters.RobotIp = await zco.GetRobotIp();
            }
            if (string.IsNullOrEmpty(ConnectionParameters.RobotIp))
            {
                AddToLog("Robot not found. Maybe it is in ACCESS POINT mode. Check networks for SANDWICHBOXBOT");
                return;
            }

            var spw = new SetParametersWindow();
            spw.ShowDialog();
        }

        private bool CSetParametersCanExecute()
        {
            return true;
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
