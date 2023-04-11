using Microsoft.Win32;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms.VisualStyles;
using static lab1.WinApi;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;


namespace lab1
{
    public partial class WinApi : Form
    {
        public WinApi()
        {
            InitializeComponent();
        }
        public enum SystemMetric
        {
            SM_CXSCREEN = 0,  // 0x00
            SM_CYSCREEN = 1,  // 0x01
            SM_CXVSCROLL = 2,  // 0x02
            SM_CYHSCROLL = 3,  // 0x03
            SM_CYCAPTION = 4,  // 0x04
            SM_CXBORDER = 5,  // 0x05
            SM_CYBORDER = 6,  // 0x06
            SM_CXDLGFRAME = 7,  // 0x07
            SM_CXFIXEDFRAME = 7,  // 0x07
            SM_CYDLGFRAME = 8,  // 0x08
            SM_CYFIXEDFRAME = 8,  // 0x08
            SM_CYVTHUMB = 9,  // 0x09
            SM_CXHTHUMB = 10, // 0x0A
            SM_CXICON = 11, // 0x0B
            SM_CYICON = 12, // 0x0C
            SM_CXCURSOR = 13, // 0x0D
            SM_CYCURSOR = 14, // 0x0E
            SM_CYMENU = 15, // 0x0F
            SM_CXFULLSCREEN = 16, // 0x10
            SM_CYFULLSCREEN = 17, // 0x11
            SM_CYKANJIWINDOW = 18, // 0x12
            SM_MOUSEPRESENT = 19, // 0x13
            SM_CYVSCROLL = 20, // 0x14
            SM_CXHSCROLL = 21, // 0x15
            SM_DEBUG = 22, // 0x16
            SM_SWAPBUTTON = 23, // 0x17
            SM_CXMIN = 28, // 0x1C
            SM_CYMIN = 29, // 0x1D
            SM_CXSIZE = 30, // 0x1E
            SM_CYSIZE = 31, // 0x1F
            SM_CXSIZEFRAME = 32, // 0x20
            SM_CXFRAME = 32, // 0x20
            SM_CYSIZEFRAME = 33, // 0x21
            SM_CYFRAME = 33, // 0x21
            SM_CXMINTRACK = 34, // 0x22
            SM_CYMINTRACK = 35, // 0x23
            SM_CXDOUBLECLK = 36, // 0x24
            SM_CYDOUBLECLK = 37, // 0x25
            SM_CXICONSPACING = 38, // 0x26
            SM_CYICONSPACING = 39, // 0x27
            SM_MENUDROPALIGNMENT = 40, // 0x28
            SM_PENWINDOWS = 41, // 0x29
            SM_DBCSENABLED = 42, // 0x2A
            SM_CMOUSEBUTTONS = 43, // 0x2B
            SM_SECURE = 44, // 0x2C
            SM_CXEDGE = 45, // 0x2D
            SM_CYEDGE = 46, // 0x2E
            SM_CXMINSPACING = 47, // 0x2F
            SM_CYMINSPACING = 48, // 0x30
            SM_CXSMICON = 49, // 0x31
            SM_CYSMICON = 50, // 0x32
            SM_CYSMCAPTION = 51, // 0x33
            SM_CXSMSIZE = 52, // 0x34
            SM_CYSMSIZE = 53, // 0x35
            SM_CXMENUSIZE = 54, // 0x36
            SM_CYMENUSIZE = 55, // 0x37
            SM_ARRANGE = 56, // 0x38
            SM_CXMINIMIZED = 57, // 0x39
            SM_CYMINIMIZED = 58, // 0x3A
            SM_CXMAXTRACK = 59, // 0x3B
            SM_CYMAXTRACK = 60, // 0x3C
            SM_CXMAXIMIZED = 61, // 0x3D
            SM_CYMAXIMIZED = 62, // 0x3E
            SM_NETWORK = 63, // 0x3F
            SM_CLEANBOOT = 67, // 0x43
            SM_CXDRAG = 68, // 0x44
            SM_CYDRAG = 69, // 0x45
            SM_SHOWSOUNDS = 70, // 0x46
            SM_CXMENUCHECK = 71, // 0x47
            SM_CYMENUCHECK = 72, // 0x48
            SM_SLOWMACHINE = 73, // 0x49
            SM_MIDEASTENABLED = 74, // 0x4A
            SM_MOUSEWHEELPRESENT = 75, // 0x4B
            SM_XVIRTUALSCREEN = 76, // 0x4C
            SM_YVIRTUALSCREEN = 77, // 0x4D
            SM_CXVIRTUALSCREEN = 78, // 0x4E
            SM_CYVIRTUALSCREEN = 79, // 0x4F
            SM_CMONITORS = 80, // 0x50
            SM_SAMEDISPLAYFORMAT = 81, // 0x51
            SM_IMMENABLED = 82, // 0x52
            SM_CXFOCUSBORDER = 83, // 0x53
            SM_CYFOCUSBORDER = 84, // 0x54
            SM_TABLETPC = 86, // 0x56
            SM_MEDIACENTER = 87, // 0x57
            SM_STARTER = 88, // 0x58
            SM_SERVERR2 = 89, // 0x59
            SM_MOUSEHORIZONTALWHEELPRESENT = 91, // 0x5B
            SM_CXPADDEDBORDER = 92, // 0x5C
            SM_DIGITIZER = 94, // 0x5E
            SM_MAXIMUMTOUCHES = 95, // 0x5F

            SM_REMOTESESSION = 0x1000, // 0x1000
            SM_SHUTTINGDOWN = 0x2000, // 0x2000
            SM_REMOTECONTROL = 0x2001, // 0x2001


            SM_CONVERTIBLESLATEMODE = 0x2003,
            SM_SYSTEMDOCKED = 0x2004,
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct SYSTEMTIME
        {
            [MarshalAs(UnmanagedType.U2)] public short Year;
            [MarshalAs(UnmanagedType.U2)] public short Month;
            [MarshalAs(UnmanagedType.U2)] public short DayOfWeek;
            [MarshalAs(UnmanagedType.U2)] public short Day;
            [MarshalAs(UnmanagedType.U2)] public short Hour;
            [MarshalAs(UnmanagedType.U2)] public short Minute;
            [MarshalAs(UnmanagedType.U2)] public short Second;
            [MarshalAs(UnmanagedType.U2)] public short Milliseconds;

        }
        public struct POINT
        {
            public int X;
            public int Y;

            public POINT(int x, int y)
            {
                this.X = x;
                this.Y = y;
            }

            public static implicit operator System.Drawing.Point(POINT p)
            {
                return new System.Drawing.Point(p.X, p.Y);
            }

            public static implicit operator POINT(System.Drawing.Point p)
            {
                return new POINT(p.X, p.Y);
            }

            public override string ToString()
            {
                return $"X: {X}, Y: {Y}";
            }
        }

        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern bool GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, StringBuilder lpBuffer, ref uint lpnSize);
        [DllImport("advapi32.dll", SetLastError = true)]
        static extern bool GetUserName(System.Text.StringBuilder sb, ref Int32 length);
        [DllImport("kernel32.dll")]
        static extern uint GetSystemDirectory([Out] StringBuilder lpBuffer, uint uSize);
        [DllImport("user32.dll")]
        static extern int GetSystemMetrics(SystemMetric smIndex);
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool SystemParametersInfo(uint uiAction, uint uiParam, StringBuilder pvParam, uint fWinIni);
        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool SystemParametersInfo(uint uiAction, uint uiParam, ref uint pvParam, uint fWinIni);
        [DllImport("user32.dll")]
        static extern uint GetSysColor(int nIndex);
        [DllImport("user32.dll")]
        static extern bool SetSysColors(int cElements, int[] lpaElements,
        int[] lpaRgbValues);
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool GetSystemTime(out SYSTEMTIME lpSystemTime);
        [DllImport("kernel32.dll")]
        static extern IntPtr GetCurrentThread();
        [DllImport("kernel32.dll")]
        static extern uint GetTickCount();
        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool GetCursorPos(out POINT point);
        [DllImport("kernel32.dll")]
        static extern uint GetUserDefaultLCID();
        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool GetKeyboardState(byte[] lpKeyState);

        private void button1_Click(object sender, EventArgs e)
        {
            //вывод имени пк
            bool success;
            StringBuilder name = new StringBuilder(260);
            uint size = 260;
            {
                success = GetComputerNameEx(COMPUTER_NAME_FORMAT.ComputerNameDnsDomain, name, ref size);
            }
            Encoding textEnc = new ASCIIEncoding();
            label1.Text = "name pc: " + name.ToString();

            //вывод имени пользователя
            StringBuilder Buffer = new StringBuilder(64);
            int nSize = 64;
            GetUserName(Buffer, ref nSize);
            label2.Text = "user name: " + Buffer.ToString();

            //пути к системным каталогам
            StringBuilder sbSystemDir = new StringBuilder(256);
            GetSystemDirectory(sbSystemDir, 256);
            label4.Text = "System dir: " + sbSystemDir.ToString();

            //Версия операционной системы
            label3.Text = "System version: " + Environment.OSVersion;

            //системные метрики
            switch (GetSystemMetrics(SystemMetric.SM_CLEANBOOT))
            {
                case 0:
                    label6.Text = "Тип загрузки: " + "Normal boot";
                    break;
                case 1:
                    label6.Text = "Тип загрузки: " + "Fail safe boot";
                    break ;
                case 2:
                    label6.Text = "Тип загрузки: " + "Fail safe boot with networking";
                    break;
            }
            label5.Text = "Разрешение экрана: " + GetSystemMetrics(SystemMetric.SM_CXSCREEN) + " x" + 
                GetSystemMetrics(SystemMetric.SM_CYSCREEN);

            //системные параметры
            StringBuilder pathWallaper = new StringBuilder(1024);
            const uint SPT_GETWP = 0x0073;
            SystemParametersInfo(SPT_GETWP, 1024, pathWallaper, 0);
            label8.Text = "Путь к обоям: " + pathWallaper;

            const uint SPI_GETFONTS = 0x200C;
            uint contrast = 0;
            SystemParametersInfo(SPI_GETFONTS, 0, ref contrast, 0);
            label7.Text = "контрастность: " + contrast;

            //системные цвета
            const int COLOR_BACKGROUND = 1;
            int color1 = Convert.ToInt32(GetSysColor(COLOR_BACKGROUND));
            Color color = Color.FromArgb(color1 & 0xFF, (color1 & 0xFF00) >> 8, (color1 & 0xFF0000) >> 16);
            label10.Text = "Цвет текста: " + "[R= " + color.R + ",G= " + color.G + ",B= " + color.B + "]";

            //системное время
            SYSTEMTIME sYSTEMTIME;
            GetSystemTime(out sYSTEMTIME);
            label9.Text = "Дата: " + sYSTEMTIME.Day + "/" + sYSTEMTIME.Month + "/" + sYSTEMTIME.Year;

            //api функции на выбор
            //вpемя, пpошедшее с момента запуска системы.
            label11.Text = "вpемя, пpошедшее с момента запуска системы: " + GetTickCount();

            //положение курсора
            POINT point = new POINT();
            GetCursorPos(out point);
            label12.Text = "Положение курсора: " + Convert.ToString(point.X) + ";" + Convert.ToString(point.Y);

            //Возвращает идентификатор языкового стандарта пользователя по умолчанию
            label13.Text = "идентификатор языкового стандарта: " + GetUserDefaultLCID();

            //состояние клавиш 
            //Массив размером 256 байтов, который получает
            //данные о состоянии для каждого виртуального ключа.
            byte[] keys = new byte[256];
            label14.Text = "Состояние клавиш: " + GetKeyboardState(keys);

        }

        private void button2_Click(object sender, EventArgs e)
        {
            //меняем цвет окна
            const int COLOR_BACKGROUND = 1;
            Color icCol = Color.FromArgb(Convert.ToInt32(textBox1.Text),
                Convert.ToInt32(textBox2.Text), Convert.ToInt32(textBox3.Text));
            int[] elements = { COLOR_BACKGROUND };
            int[] colors = { ColorTranslator.ToWin32(icCol) };
            SetSysColors(elements.Length, elements, colors);
        }
    }
}