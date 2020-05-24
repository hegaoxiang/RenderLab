using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GWinformLib
{
    public partial class Viewport: UserControl
    {
        public Viewport()
        {
           

           
            InitializeComponent();
        }


        public void Application_Idle(object sender, EventArgs e)
        {
            try
            {
                MessageBox.Show("qwe");
                // Render the scene if we are idle
                IGCore.RenderFrame();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
           
        }
        protected override void WndProc(ref Message m)
        {
            
            IGCore.MsgProc(m.HWnd, m.Msg, m.WParam, m.LParam);
            base.WndProc(ref m);
        }
    }
}
