using GalaSoft.MvvmLight;
using SBBotMobile.Communication.Enums;

namespace SBBotMobile.ViewModel
{
    public class MainViewModel : ViewModelBase
    {
        private bool _isConnected;
        private RobotMode _currentRobotMode;

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

        public MainViewModel()
        {
            _isConnected = false;
            _currentRobotMode = RobotMode.Manual;
        }
    }
}