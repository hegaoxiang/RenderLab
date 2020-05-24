using GEditor.import;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace GEditor
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private GWinformLib.Viewport viewport = new GWinformLib.Viewport();

        private System.Windows.Threading.DispatcherTimer dt = new DispatcherTimer();

        bool isInit = false;

        void dt_Tick(object sender, EventArgs e)
        {
            //要执行的任务
            IGCore.RenderFrame();
        }
        public MainWindow()
        {
            InitializeComponent();

           
            viewport.BorderStyle = BorderStyle.None;

            wfHost.Child = viewport;
        }

        private void wfHost_ChildChanged(object sender, System.Windows.Forms.Integration.ChildChangedEventArgs e)
        {

            IntPtr hwnd = viewport.Handle;
            double h = this.wfHost.Height;
            double w = this.wfHost.Width;
            IGCore.InitD3D(hwnd, w, h);

            dt.Interval = TimeSpan.FromMilliseconds(1000);
            dt.Tick += new EventHandler(dt_Tick);
            dt.Start();

            
        }



 
    }
}
