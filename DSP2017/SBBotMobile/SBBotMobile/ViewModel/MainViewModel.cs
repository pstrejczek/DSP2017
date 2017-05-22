using GalaSoft.MvvmLight;
using System.Windows.Input;
using System.Collections.ObjectModel;
using System.Threading;
using SBBotMobile.Communication.Enums;
using SBBotMobile.Communication;
using SBBotMobile.Models;
using SBBotMobile.DataContainers;
using GalaSoft.MvvmLight.Command;
using Xamarin.Forms;

namespace SBBotMobile.ViewModel
{
    public class MainViewModel : ViewModelBase
    {
        private bool _isConnected;
        private RobotMode _currentRobotMode;

        private ObservableCollection<LogEntry> _log;
        private UdpCommOperations _udpCommOps;
        private string _robotIp;

        public ICommand CConnect => new RelayCommand(ConnectRobot);
        public ICommand CChangeMode => new RelayCommand(ChangeRobotMode);
        public ICommand CForward => new RelayCommand(ManualForward);
        public ICommand CBackward => new RelayCommand(ManualBackward);
        public ICommand CLeft => new RelayCommand(ManualLeft);
        public ICommand CRight => new RelayCommand(ManualRight);

        public bool IsConnected
        {
            get { return _isConnected; }
            set
            {
                _isConnected = value;
                RaisePropertyChanged();
            }
        }

        public RobotMode CurrentRobotMode
        {
            get { return _currentRobotMode; }
            set
            {
                _currentRobotMode = value;
                RaisePropertyChanged();
            }
        }

        public ObservableCollection<LogEntry> Log
        {
            get { return _log; }
            set
            {
                _log = value;
                RaisePropertyChanged(nameof(Log));
            }
        }

        public MainViewModel()
        {
            _isConnected = false;
            _currentRobotMode = RobotMode.Manual;

            MessagingCenter.Subscribe<SetParametersViewModel>(this, "disconnect", (Disconect) =>
            {
                IsConnected = false;
            });
        }

        private void ManualForward()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Forward);
        }

        private void ManualLeft()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Left);
        }

        private void ManualRight()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Right);
        }

        private void ManualBackward()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Backward);
        }

        private void StopMovement()
        {
            if (!_isConnected) return;
            _udpCommOps.SendCommand(UdpRobotCommand.Stop);
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

            WebOperations.Initialize(_robotIp);

            var rmode = await WebOperations.GetCurrentRobotMode();

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

        public async void ChangeRobotMode()
        {
            if (!_isConnected) return;

            var lastRobotMode = CurrentRobotMode;

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

            // wait for state change

            var counter = 0;

            while (CurrentRobotMode == lastRobotMode && counter < 10)
            {
                CurrentRobotMode = await WebOperations.GetCurrentRobotMode();
                counter++;
            }
        }

        public void AddToLog(string message)
        {
            if (Log == null) Log = new ObservableCollection<LogEntry>();

            var le = new LogEntry
            {
                DateTime = System.DateTime.Now,
                RobotEvent = message
            };
            Log.Add(le);
        }
    }
}