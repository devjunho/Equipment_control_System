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
using System.Windows.Navigation;
using System.Windows.Shapes;
using Dobot_OpenCV.Model;

namespace Dobot_OpenCV.ViewModel
{
    public partial class First : Page
    {

        public static Data data = new Data();
        public static CONNECT con = new CONNECT();
        public First()
        {
            InitializeComponent();
        }

        private void btn_pay_Click(object sender, RoutedEventArgs e)
        {
            Uri uri = new Uri("/ViewModel/Main.xaml", UriKind.Relative);
            NavigationService.Navigate(uri);
        }

        private void btn_manager_Click(object sender, RoutedEventArgs e)
        {
            Manager_Password manaerWindow = new Manager_Password();
            manaerWindow.Show();
        }
    }
}
