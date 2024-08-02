using Dobot_OpenCV.ViewModel;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Markup;
using System.IO;
using Newtonsoft.Json;
using System.Net;


namespace Dobot_OpenCV.Model
{
    public class MANAGEMENT
    {
        public bool MachPassword(int password)
        {
            if (Manager_Password.manage.Password == password)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    public class CONNECT
    {

        private TcpClient Client;
        private NetworkStream Stream;

        private string IP = "10.10.21.114";
        private int Port = 5001;


        public CONNECT() { }

        private int ConnectServer()
        {
            try
            {
                Client = new TcpClient(IP, Port);
                Stream = Client.GetStream();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return 0;
            }

            return 1;
        }

        private void DisconnectServer()
        {
            Stream.Close();
            Client.Close();
        }

        public int ISBN_BookInfo()
        {
            if (ConnectServer() == 0)
            {
                return 0;
            }

            Data Send = new Data()
            {
                Type = (int)TYPE.INQUIRY_BOOK,
                ISBN = First.data.ISBN
            };

            string SendJson = JsonConvert.SerializeObject(Send, Formatting.Indented);
            byte[] SendByte = Encoding.UTF8.GetBytes(SendJson);
            Stream.Write(SendByte, 0, SendByte.Length);

            byte[] Buffer = new byte[2048];
            int ReadByte = Stream.Read(Buffer, 0, Buffer.Length);
            string ReadJson = Encoding.UTF8.GetString(Buffer, 0, ReadByte);

            try
            {
                Data Result = JsonConvert.DeserializeObject<Data>(ReadJson);


                if (Result.Type == (int)TYPE.SUCCEED)
                {
                    First.data.BasketList = Result.BasketList;
                }

                DisconnectServer();

                return Result.Type;
            }
            catch (JsonException jsonEx)
            {
                DisconnectServer();

                Debug.WriteLine($"JSON Exception: {jsonEx.Message}");

                return (int)TYPE.FAIL;
            }
        }

        public int Payment()
        {
            if (ConnectServer() == 0)
            {
                return 0;
            }

            Data Send = new Data()
            {
                Type = (int)TYPE.CALCULATE_COUNT,
                BASKET = First.data.BASKET,   
                RECEIVED = First.data.RECEIVED    
            };

            string SendJson = JsonConvert.SerializeObject(Send, Formatting.Indented);
            byte[] SendByte = Encoding.UTF8.GetBytes(SendJson);
            Stream.Write(SendByte, 0, SendByte.Length);


            byte[] Buffer = new byte[8192];
            int ReadByte = Stream.Read(Buffer, 0, Buffer.Length);
            string ReadJson = Encoding.UTF8.GetString(Buffer, 0, ReadByte);

            try
            {
                Data Result = JsonConvert.DeserializeObject<Data>(ReadJson);

                if (Result.Type == (int)TYPE.SUCCEED)
                {

                    First.data.BALANCE = Result.BALANCE;
                    First.data.Result10000 = Result.Result10000;
                    First.data.Result5000 = Result.Result5000;
                    First.data.Result1000 = Result.Result1000;
                    First.data.Result500 = Result.Result500;
                    First.data.Result100 = Result.Result100;
                }

                DisconnectServer();

                return Result.Type;
            }
            catch (JsonException jsonEx)
            {
                DisconnectServer();

                Debug.WriteLine($"JSON Exception: {jsonEx.Message}");

                return (int)TYPE.FAIL;
            }
        }

        public int InputMoney()
        {
            if (ConnectServer() == 0)
            {
                return 0;
            }

            Money Send = new Money()
            {
                Type = (int)TYPE.DEPOSIT,
                PAPER_10000 = Manager.money.PAPER_10000,
                PAPER_5000 = Manager.money.PAPER_5000,
                PAPER_1000 = Manager.money.PAPER_1000,
                COIN_500 = Manager.money.COIN_500,
                COIN_100 = Manager.money.COIN_100
            };

            string SendJson = JsonConvert.SerializeObject(Send, Formatting.Indented);
            byte[] SendByte = Encoding.UTF8.GetBytes(SendJson);
            Stream.Write(SendByte, 0, SendByte.Length);

            byte[] Buffer = new byte[1024];
            int ReadByte = Stream.Read(Buffer, 0, Buffer.Length);
            string ReadJson = Encoding.UTF8.GetString(Buffer, 0, ReadByte);

            try
            {
                Data Result = JsonConvert.DeserializeObject<Data>(ReadJson);

                DisconnectServer();

                return Result.Type;
            }
            catch (JsonException jsonEx)
            {
                DisconnectServer();
               
                Debug.WriteLine($"JSON Exception: {jsonEx.Message}");

                return (int)TYPE.FAIL;
            }
        }


        private string PyIP = "10.10.21.129";
        private int PyPort = 5001;

        private int ConnectPython()
        {
            try
            {
                Client = new TcpClient(PyIP, PyPort);
                Stream = Client.GetStream();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return 0;
            }

            return 1;
        }

        private void DisconnectPython()
        {
            Stream.Close();
            Client.Close();
        }

        public int PythonCon()
        {
            if (ConnectPython() == 0)
            {
                return 0;
            }

            Data Send = new Data()
            {
                Result10000 = First.data.Result10000,
                Result5000 = First.data.Result5000,
                Result1000 = First.data.Result1000,
                Result500 = First.data.Result500,
                Result100 = First.data.Result100,
            };

            string SendJson = JsonConvert.SerializeObject(Send, Formatting.Indented);
            byte[] SendByte = Encoding.UTF8.GetBytes(SendJson);
            Stream.Write(SendByte, 0, SendByte.Length); ;

            byte[] Buffer = new byte[8192];
            int ReadByte = Stream.Read(Buffer, 0, Buffer.Length);
            string ReadJson = Encoding.UTF8.GetString(Buffer, 0, ReadByte);

            try
            {
                Data Result = JsonConvert.DeserializeObject<Data>(ReadJson);

                DisconnectServer();

                return Result.Type;
            }
            catch (JsonException jsonEx)
            {
                DisconnectServer();

                Debug.WriteLine($"JSON Exception: {jsonEx.Message}");

                return (int)TYPE.FAIL;
            }
        }
    }
}
