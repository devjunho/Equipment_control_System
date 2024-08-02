using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.UI;
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
using OpenCvSharp;

namespace Dobot_OpenCV.ViewModel
{
    public partial class Manager_Password : System.Windows.Window
    {
        public static Manage manage = new Manage();
        public static MANAGEMENT management = new MANAGEMENT();

        public Manager_Password()
        {
            InitializeComponent();
        }

        private void btn_return_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Window.GetWindow(this).Close();
        }

        private void btn_enter_Click(object sender, RoutedEventArgs e)
        {
            int password = Convert.ToInt32(manage_password.Password.ToString());
            bool check = management.MachPassword(password);
            
            if (check)
            {
                Manager manager_page = new Manager();
                manager_page.Title = "Manager"; 
                this.Content = manager_page;
            }
            else
            {
                MessageBox.Show("비밀번호가 일치하지 않습니다.");
                manage_password.Clear();
            }
        }
    }
}
