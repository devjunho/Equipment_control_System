using OpenCvSharp;
using OpenCvSharp.Extensions;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Text.RegularExpressions;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using Tesseract;
using Dobot_OpenCV.Model;
using OpenCvSharp.WpfExtensions;
using System.Threading;
using OpenCvSharp.Flann;
using System.Runtime.InteropServices;
using System.Collections.ObjectModel;
using System.Windows.Markup;

namespace Dobot_OpenCV.ViewModel
{
    public partial class Main : System.Windows.Controls.Page
    {
        private VideoCapture videoCapture;
        private bool isCameraRunning = false;
        private TesseractEngine ocr;

        private string send_msg;

        private bool Money_Check = false;

        ObservableCollection<Book_Info> dataList;


        public Main()
        {
            InitializeComponent();
            dataList = new ObservableCollection<Book_Info>();
            Collation_BookList.ItemsSource = dataList;
            Start_Camera();
        }
        public Main(ObservableCollection<Book_Info> book_Info) : this()
        {
            dataList = book_Info;
            Collation_BookList.ItemsSource = dataList;
        }

        private void List_Update()
        {
            Collation_BookList.ItemsSource = null;
            Collation_BookList.ItemsSource = dataList;
            Payment.Text = Book_Info.Sum_TotalPrice_Method(dataList).ToString();
        }
        private void List_Update(List<Book_Info> data)
        {
            int index = 0;
            Book_Title.Text = data[0].TITLE;
            Book_Price.Text = data[0].PRE_PRICE;
            foreach (var item in dataList)
            {
                if (data[0].TITLE == item.TITLE)
                {
                    dataList[index].Book_Count++;
                    return;
                }
                index++;
            }
            data[0].Book_Count++;
            dataList.Add(data[0]);
            Collation_BookList.ItemsSource = null;
            Collation_BookList.ItemsSource = dataList;
            Payment.Text = Book_Info.Sum_TotalPrice_Method(dataList).ToString();
        }

        private void Purchase_Button_Click(object sender, RoutedEventArgs e)
        {
            
            if (MessageBox.Show("계산하시겠습니까?", "알림", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
            {
                videoCapture.Release();
                videoCapture.Dispose();
                isCameraRunning = false;
                First.data.RECEIVED = Money.Text.ToString();
                First.data.BASKET = Payment.Text.ToString();
                int result = First.con.Payment();
                if(result == (int)TYPE.SUCCEED)
                {

                    MessageBox.Show($"잔돈 : {First.data.BALANCE}원");
                    int resultPy = First.con.PythonCon();
                    if(resultPy == 50)
                    {
                        MessageBox.Show($"계산이 완료되었습니다. 감사합니다.");

                        Uri uri = new Uri("/ViewModel/First.xaml", UriKind.Relative);
                        NavigationService.Navigate(uri);
                    }
                    else
                    {
                        MessageBox.Show("죄송합니다. 오류가 발생했습니다.\n기계 옆 관리자 호출 버튼을 눌러주세요.");
                    }
                }
                else if(result == (int)TYPE.FAIL)
                {
                    MessageBox.Show("결제 요청에 실패하였습니다.");
                }
                else if (result == (int)TYPE.EMPTY)
                {
                    MessageBox.Show("잔돈이 부족합니다.\n기계 옆 관리자 호출 버튼을 눌러주세요.");
                }
            }
        }

        private void Delete_Button_Click(object sender, RoutedEventArgs e)
        {
            Button deletebutton = sender as Button;
            if (deletebutton != null)
            {
                Book_Info data = deletebutton.DataContext as Book_Info;
                if (data != null)
                {
                    dataList.Remove(data);
                    List_Update();
                }
            }
        }

        private void Money_GotFocus(object sender, RoutedEventArgs e)
        {
            if (!Money_Check)
            {
                Money.Text = "";
                Money_Check = true;
                Money.Foreground = System.Windows.Media.Brushes.Black;
            }
        }

        private void Money_LostFocus(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(Money.Text))
            {
                Money.Text = "여기에 금액을 입력해주세요";
                Money.Foreground = System.Windows.Media.Brushes.Gray;
                Money_Check = false;
            }
        }

        private void Money_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Keyboard.ClearFocus();
                e.Handled = true;
            }
        }

        private async void Start_Camera()
        {
            videoCapture = new VideoCapture(0);
            isCameraRunning = true;

            await Task.Run(() =>
            {
                while (isCameraRunning)
                {
                    using (var frame = new Mat())
                    {
                        videoCapture.Read(frame);
                        Image_Update(frame.Empty(), frame.Clone());
                    }
                }
            });
        }

        private void Image_Update(bool check, Mat frame)
        {
            if (!check)
            {
                BitmapImage bitmapimage = new BitmapImage();
                bitmapimage.BeginInit();
                Bitmap bitmap = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(frame);
                bitmapimage = BitmapToBitmapImage(bitmap);
                Dispatcher.Invoke(() =>
                {
                    Image_Box.Source = bitmapimage;
                    if (ExVideo(frame))
                    {
                        if (MessageBox.Show(send_msg + "가 맞습니까?", "확인", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                        {
                            First.data.ISBN = send_msg;
                            int result = First.con.ISBN_BookInfo();
                            if (result == (int)TYPE.SUCCEED)
                            {
                               List_Update(First.data.BasketList);
                            }
                        }
                    }
                });
            }
        }

        private void Minus_btn_Click(object sender, RoutedEventArgs e)
        {
            Button minusbutton = sender as Button;
            if (minusbutton != null)
            {
                Book_Info data = minusbutton.DataContext as Book_Info;
                if (data != null)
                {
                    if (data.Book_Count == 0)
                    {
                        return;
                    }
                    else
                    {
                        data.Book_Count -= 1;
                        List_Update();
                    }
                }
            }
        }
        private void Plus_btn_Click(object sender, RoutedEventArgs e)
        {
            Button plusbutton = sender as Button;
            if (plusbutton != null)
            {
                Book_Info data = plusbutton.DataContext as Book_Info;
                if (data != null)
                {
                    data.Book_Count += 1;
                    List_Update();
                }
            }
        }

        private bool ExVideo(Mat frame)
        {
            var engine = new TesseractEngine(@"C:\Users\uy122\OneDrive\바탕 화면\다운로드\새 폴더\4team\bin\Debug\tessdata", "eng", EngineMode.Default);
            Bitmap bitmap;
            Mat framecopy = frame.Clone();
            using (MemoryStream ms = framecopy.ToMemoryStream())
            {
                bitmap = new Bitmap(ms);
                var image = PixConverter.ToPix(bitmap);
                var page = engine.Process(image);
                string str = page.GetText();

                string parttern = @"ISBN\s([0-9\-]+)";
                Match match = Regex.Match(str, parttern);

                if (match.Success)
                {
                    send_msg = match.Groups[1].Value; ;
                    send_msg = Regex.Replace(send_msg, @"[^0-9]", "");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        private BitmapImage BitmapToBitmapImage(Bitmap bitmap)
        {
            BitmapImage bitmapImage = new BitmapImage();
            using (MemoryStream ms = new MemoryStream())
            {
                bitmap.Save(ms, System.Drawing.Imaging.ImageFormat.Bmp);
                ms.Position = 0;
                bitmapImage.BeginInit();
                bitmapImage.StreamSource = ms;
                bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                bitmapImage.EndInit();
                bitmapImage.Freeze();
            }
            return bitmapImage;
        }
    }
}
