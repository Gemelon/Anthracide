using AnthracideUI.Model;
using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics.Arm;
using System.Windows;

namespace AnthracideUI.ViewModel
{
    internal class FrontpanelViewModel : BaseViewModel
    {
        static string TAG = "FrontpanelViewModel";

        UDPLib UDP;

        private int _MainVolume = -80;

        public RelayCommand ExitCommand { get; set; }

        private string _IPAddressTextBox;

        public string IPAddressTextBox
        {
            get
            {
                if (UDP != null)
                {
                    return UDP.IPAddress;
                }
                return _IPAddressTextBox;
            }
            set
            {
                _IPAddressTextBox = value;
                if (UDP != null)
                {
                    UDP.IPAddress = value;
                }
                OnPropertyChanged();
            }
        }

        private string _PortTextBox;

        public string PortTextBox
        {
            get
            {
                if (UDP != null)
                {
                    return UDP.Port.ToString();
                }
                return _PortTextBox;
            }
            set
            {
                _PortTextBox = value;
                if (UDP != null)
                {
                    int port = 0;
                    int.TryParse(value, out port);
                    UDP.Port= port;
                }
                OnPropertyChanged();
            }
        }


        public int MainVolume
        {
            get
            {
                UDP.SendData("volume", _MainVolume.ToString());
                return _MainVolume;
            }
            set
            {
                if (_MainVolume != value)
                {
                    _MainVolume = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_45Hz;

        public int Equalizer_45Hz
        {
            get
            {
                UDP.SendData("Equalizer_45Hz", _Equalizer_45Hz.ToString());
                return _Equalizer_45Hz;
            }
            set
            {
                if (_Equalizer_45Hz != value)
                {
                    _Equalizer_45Hz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_64Hz;

        public int Equalizer_64Hz
        {
            get
            {
                UDP.SendData("Equalizer_64Hz", _Equalizer_64Hz.ToString());
                return _Equalizer_64Hz;
            }
            set
            {
                if (_Equalizer_64Hz != value)
                {
                    _Equalizer_64Hz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_125Hz;

        public int Equalizer_125Hz
        {
            get
            {
                UDP.SendData("Equalizer_125Hz", _Equalizer_125Hz.ToString());
                return _Equalizer_125Hz;
            }
            set
            {
                if (_Equalizer_125Hz != value)
                {
                    _Equalizer_125Hz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_250Hz;

        public int Equalizer_250Hz
        {
            get
            {
                UDP.SendData("Equalizer_250Hz", _Equalizer_250Hz.ToString());
                return _Equalizer_250Hz;
            }
            set
            {
                if (_Equalizer_250Hz != value)
                {
                    _Equalizer_250Hz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_500Hz;

        public int Equalizer_500Hz
        {
            get
            {
                UDP.SendData("Equalizer_500Hz", _Equalizer_500Hz.ToString());
                return _Equalizer_500Hz;
            }
            set
            {
                if (_Equalizer_500Hz != value)
                {
                    _Equalizer_500Hz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_1KHz;

        public int Equalizer_1KHz
        {
            get
            {
                UDP.SendData("Equalizer_1KHz", _Equalizer_1KHz.ToString());
                return _Equalizer_1KHz;
            }
            set
            {
                if (_Equalizer_1KHz != value)
                {
                    _Equalizer_1KHz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_2KHz;

        public int Equalizer_2KHz
        {
            get
            {
                UDP.SendData("Equalizer_2KHz", _Equalizer_2KHz.ToString());
                return _Equalizer_2KHz;
            }
            set
            {
                if (_Equalizer_2KHz != value)
                {
                    _Equalizer_2KHz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_4KHz;

        public int Equalizer_4KHz
        {
            get
            {
                UDP.SendData("Equalizer_4KHz", _Equalizer_4KHz.ToString());
                return _Equalizer_4KHz;
            }
            set
            {
                if (_Equalizer_4KHz != value)
                {
                    _Equalizer_4KHz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_8KHz;

        public int Equalizer_8KHz
        {
            get
            {
                UDP.SendData("Equalizer_8KHz", _Equalizer_8KHz.ToString());
                return _Equalizer_8KHz;
            }
            set
            {
                if (_Equalizer_8KHz != value)
                {
                    _Equalizer_8KHz = value;
                    OnPropertyChanged();
                }
            }
        }

        private int _Equalizer_16KHz;

        public int Equalizer_16KHz
        {
            get
            {
                UDP.SendData("Equalizer_16KHz", _Equalizer_16KHz.ToString());
                return _Equalizer_16KHz;
            }
            set
            {
                if (_Equalizer_16KHz != value)
                {
                    _Equalizer_16KHz = value;
                    OnPropertyChanged();
                }
            }
        }

        private string _StatusTextBox;

        public string StatusTextBox
        {
            get
            {
                return _StatusTextBox;
            }
            set
            {
                if (_StatusTextBox != value)
                {
                    _StatusTextBox = value;
                    OnPropertyChanged();
                }
            }
        }

        public FrontpanelViewModel()
        {
            IPAddressTextBox = "192.168.178.70";
            UDP = new UDPLib();
            UDP.IPAddress = IPAddressTextBox;
            UDP.UDPDataReceivedEvent += UDP_UDPDataReceivedEvent;

            this.ExitCommand = new RelayCommand((o) =>
            {
                UDP.SendData("clientclose", "1");
                Application.Current.Shutdown();
            });

            StatusTextBox = "Anthracide gestartet";
        }

        private void UDP_UDPDataReceivedEvent(object sender, string ReceivedData)
        {
            string command = "";
            string value = "";
            int intValue = -80;

            AnthracideLib.LOGI(TAG, String.Format("UDP received event = {0}", ReceivedData));
            if (ReceivedData.IndexOf("=", StringComparison.OrdinalIgnoreCase) >= 0)
            {
                command = ReceivedData.Substring(0, ReceivedData.IndexOf("=", StringComparison.OrdinalIgnoreCase));
                value = ReceivedData.Substring(ReceivedData.IndexOf("=", StringComparison.OrdinalIgnoreCase) + 1);
            }

            StatusTextBox = ReceivedData;

            switch (command)
            {
                case "Volume":
                    if (!int.TryParse(value, out intValue))
                    {
                        MainVolume = -80;
                    }
                    MainVolume = intValue;
                    break;

                case "Equalizer_45Hz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_45Hz = 0;
                    }
                    Equalizer_45Hz = intValue;
                    break;

                case "Equalizer_64Hz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_64Hz = 0;
                    }
                    Equalizer_64Hz = intValue;
                    break;

                case "Equalizer_125Hz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_125Hz = 0;
                    }
                    Equalizer_125Hz = intValue;
                    break;

                case "Equalizer_250Hz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_250Hz = 0;
                    }
                    Equalizer_250Hz = intValue;
                    break;

                case "Equalizer_500Hz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_500Hz = 0;
                    }
                    Equalizer_500Hz = intValue;
                    break;

                case "Equalizer_1KHz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_1KHz = 0;
                    }
                    Equalizer_1KHz = intValue;
                    break;

                case "Equalizer_2KHz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_2KHz = 0;
                    }
                    Equalizer_2KHz = intValue;
                    break;

                case "Equalizer_4KHz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_4KHz = 0;
                    }
                    Equalizer_4KHz = intValue;
                    break;

                case "Equalizer_8KHz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_8KHz = 0;
                    }
                    Equalizer_8KHz = intValue;
                    break;

                case "Equalizer_16KHz":
                    if (!int.TryParse(value, out intValue))
                    {
                        Equalizer_16KHz = 0;
                    }
                    Equalizer_16KHz = intValue;
                    break;

                default:
                    break;
            }
        }
    }
}
