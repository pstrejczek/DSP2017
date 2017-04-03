using SBBotDesktop.Annotations;
using SBBotDesktop.Communication.Enums;
using SBBotDesktop.Models;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Input;

namespace SBBotDesktop.ViewModels
{
    class MainWindowViewModel : INotifyPropertyChanged
    {
        private bool _isConnected = false;
        private RobotWiFiMode _currentRobotWiFiMode;
        private RobotMode _currentRobotMode;

        private ObservableCollection<LogEntry> _log;

        public ICommand CConnect => new RelayCommand(p => this.CConnectExecute(), p => this.ConnectCanExecute());
        public ICommand CChangeMode => new RelayCommand(p => this.CChangeModeExecute(), p => this.ChangeModeCanExecute());

        public ICommand CForward => new RelayCommand(p => this.CForwadExecute(), p => this.MovementCanExecute());
        public ICommand CLeft => new RelayCommand(p => this.CLeftExecute(), p => this.MovementCanExecute());
        public ICommand CRight => new RelayCommand(p => this.CRightExecute(), p => this.MovementCanExecute());
        public ICommand CBackward => new RelayCommand(p => this.CBackwardExecute(), p => this.MovementCanExecute());

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
            var le = new LogEntry
            {
                dateTime = DateTime.Now,
                robotEvent = message
            };
            Log.Add(le);
        }

        private void CForwadExecute()
        {
            throw new System.NotImplementedException();
        }

        private void CLeftExecute()
        {
            throw new System.NotImplementedException();
        }

        private void CRightExecute()
        {
            throw new System.NotImplementedException();
        }

        private void CBackwardExecute()
        {
            throw new System.NotImplementedException();
        }

        private void CConnectExecute()
        {
            throw new System.NotImplementedException();
        }

        private void CChangeModeExecute()
        {
            throw new System.NotImplementedException();
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

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
