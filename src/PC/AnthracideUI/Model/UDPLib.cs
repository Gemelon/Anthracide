using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Windows.Input;
using System.Windows.Threading;
using System.Xml.Serialization;

namespace AnthracideUI.Model
{
    internal class UDPLib
    {
        static string TAG = "UDPLib";
        RegistryKey Key;

        UdpClient udpClient;

        public delegate void UDPDataReceivedEventHandler(object sender, string ReceivedData);
        public event UDPDataReceivedEventHandler? UDPDataReceivedEvent;
        private static string _loggingEvent = string.Empty;

        private string _IPAddress;

        public string IPAddress
        {
            get
            {
                Key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\Anthracide");
                _IPAddress = Key.GetValue("IPAddress").ToString();
                Key.Close();
                return _IPAddress;
            }
            set
            {
                _IPAddress = value;
                Key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\Anthracide");
                Key.SetValue("IPAddress", _IPAddress);
                Key.Close();
            }
        }

        private int _Port;

        public int Port
        {
            get
            {
                Key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\Anthracide");
                int port = 0;
                int.TryParse(Key.GetValue("Port").ToString(), out port);
                _Port = port;
                Key.Close();
                return _Port;
            }
            set
            {
                _Port = value;
                Key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\Anthracide");
                Key.SetValue("Port", _Port.ToString()) ;
                Key.Close();
            }
        }


        public void SendData(string Command, string Data)
        {
            string DataToSend = "";

            DataToSend = Command;
            DataToSend += "=";
            DataToSend += Data;
            DataToSend += ";";

            if (udpClient.Client.Connected)
            {
                byte[] sendBytes = Encoding.ASCII.GetBytes(DataToSend);
                udpClient.Send(sendBytes, sendBytes.Length);
            }
        }

        public string loggingEvent
        {
            get => _loggingEvent;
            set
            {
                AnthracideLib.LOGI(TAG, string.Format("loggingEvent = {0}", value));
                _loggingEvent = value;
                if (UDPDataReceivedEvent != null)
                {
                    UDPDataReceivedEvent?.Invoke(this, _loggingEvent);
                }
            }
        }

        public UDPLib()
        {
            Key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\Anthracide");
            if (Key.GetValue("IPAddress") != null)
            {
                IPAddress = Key.GetValue("IPAddress").ToString();
            }
            else
            {
                Key.SetValue("IPAddress", "192.168.178.70");
                IPAddress = "192.168.178.70";
            }

            Key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\Anthracide");
            if (Key.GetValue("Port") != null)
            {
                int port = 0;
                int.TryParse(Key.GetValue("Port").ToString(), out port);
                Port = port;
            }
            else
            {
                Key.SetValue("Port", "3333");
                Port = 3333;
            }

            Key.Close();

            Debug.WriteLine("Construktor UDPLib");
            if (udpClient == null)
            {
                udpClient = new UdpClient(Port);

                try
                {
                    udpClient.Connect(IPAddress, Port);

                    if (udpClient.Client.Connected)
                    {
                        Debug.WriteLine("Connected");
                        // Sends a message to the host to which you have connected.
                        byte[] sendBytes = Encoding.ASCII.GetBytes("clientstart=1;");
                        udpClient.Send(sendBytes, sendBytes.Length);
                    }

                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.ToString());
                }

                UDPListener();
            }
        }

        private void UDPListener()
        {
            Task.Run(async () =>
            {
                loggingEvent = ""; // todo: name ändern, ist völlig unsinnig.
                while (true)
                {
                    var receivedResults = await udpClient.ReceiveAsync();
                    loggingEvent = Encoding.ASCII.GetString(receivedResults.Buffer);
                }
            });
        }
    }
}
