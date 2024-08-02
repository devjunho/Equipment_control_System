using Dobot_OpenCV.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Navigation;

namespace Dobot_OpenCV.ViewModel
{
    public partial class Manager : Page
    {
        public static Money money = new Money();
        public CONNECT manage_con = new CONNECT();

        public Manager()
        {
            InitializeComponent();
        }

        private void btn_plusmoney_Click(object sender, RoutedEventArgs e)
        {

            money.PAPER_10000 = tbx_10000.Text.ToString();
            money.PAPER_5000 = tbx_5000.Text.ToString();
            money.PAPER_1000 = tbx_1000.Text.ToString();
            money.COIN_500 = tbx_500.Text.ToString();
            money.COIN_100 = tbx_100.Text.ToString();

            int result = manage_con.InputMoney();

            if (result == (int)TYPE.FAIL)
            {
                MessageBox.Show("오류가 발생했습니다.\n잔고에 넣은 지폐, 동전 갯수를 다시 입력해주세요.");
            }

            tbx_50000.Clear();
            tbx_10000.Clear();
            tbx_5000.Clear();
            tbx_1000.Clear();
            tbx_500.Clear();
            tbx_100.Clear();
        }

        private void btn_return_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Window.GetWindow(this).Close();
        }
    }
}
