using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace GWinformLib
{
    class IGCore
    {
        [DllImport("Core1.dll")]
        public static extern void MsgProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam);

        [DllImport(@"Core1.dll")]
        public static extern int RenderFrame();
    }
}
