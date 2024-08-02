using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace Dobot_OpenCV.Model
{
    public class Manage
    {
        public string MachinName = "기계1";
        public int Password = 1234;
    }

    public class Data
    {
        public int Type { get; set; }
        public string ISBN { get; set; }
        public string RECEIVED { get; set; }
        public string BASKET { get; set; }
        public string BALANCE { get; set; }

        public List<Book_Info> BasketList { get; set; }   
        public List<Money> CountList { get; set; } 
        public List<Change> Result100 { get; set; }
        public List<Change> Result500 { get; set; }
        public List<Change> Result1000 { get; set; } 
        public List<Change> Result5000 { get; set; } 
        public List<Change> Result10000 { get; set; } 
    }

    public class Change
    {
        public int Count { get; set; }
        public int Rest { get; set; }
    }

    public class Money
    {
        public int Type { get; set; }
        public string PAPER_10000 { get; set; }
        public string PAPER_5000 { get; set; }
        public string PAPER_1000 { get; set; }
        public string COIN_500 { get; set; }
        public string COIN_100 { get; set; }
    }


    public class Book_Info
    {
        public string TITLE { get; set; }
        public string PRE_PRICE { get; set; } 
        public int Book_Count { get; set; } 
        public int Total_PRICE 
        {
            get
            {
                int price;
                if (Int32.TryParse(PRE_PRICE, out price))
                {
                    return price * Book_Count;
                }
                return 0;
            }
        }
        static public int Sum_TotalPrice_Method(ObservableCollection<Book_Info> bookList)
        {
            int Sum_TotalPrice = 0;
            foreach (var item in bookList)
            {
                Sum_TotalPrice += item.Total_PRICE;
            }
            return Sum_TotalPrice;
        }
    }

    enum TYPE
    {
        // 0번
        CONNECT_FAIL = 0,

        // 10번
        INQUIRY_BOOK = 10, 

        // 20번
        CALCULATE_MONEY = 20,

        // 30번
        MANAGE_MONEY = 30, 

        // 40번
        CALCULATE_COUNT = 40,

        // 50번
        SUCCEED = 50,

        // 60번
        FAIL = 60,

        // 70번
        EMPTY = 70,

        // 80번
        DEPOSIT = 80,
    }
}