using System.Collections;
using System.Diagnostics;
using System.Reflection.Metadata;
using System.Runtime.ConstrainedExecution;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Text.Unicode;
using System.Windows.Forms;
using System.Windows.Forms.Design.Behavior;
using System.Xml.Linq;
using static System.Windows.Forms.CheckedListBox;

namespace lab_2_cs
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            proclist_ItemActivate();
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr GetCurrentProcess();
        [DllImport("kernel32.dll")]
        static extern int GetCurrentProcessId();
        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool DuplicateHandle(IntPtr hSourceProcessHandle,
        IntPtr hSourceHandle, IntPtr hTargetProcessHandle, out IntPtr lpTargetHandle,
        uint dwDesiredAccess, [MarshalAs(UnmanagedType.Bool)] bool bInheritHandle, uint dwOptions);
        [Flags]
        public enum DuplicateOptions : uint
        {
            DUPLICATE_CLOSE_SOURCE = (0x00000001),// Closes the source handle. This occurs regardless of any error status returned.
            DUPLICATE_SAME_ACCESS = (0x00000002), //Ignores the dwDesiredAccess parameter. The duplicate handle has the same access as the source handle.
        }
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr OpenProcess(
             uint processAccess,
             bool bInheritHandle,
             uint processId
        );
        [Flags]
        public enum ProcessAccessFlags : uint
        {
            All = 0x001F0FFF,
            Terminate = 0x00000001,
            CreateThread = 0x00000002,
            VirtualMemoryOperation = 0x00000008,
            VirtualMemoryRead = 0x00000010,
            VirtualMemoryWrite = 0x00000020,
            DuplicateHandle = 0x00000040,
            CreateProcess = 0x000000080,
            SetQuota = 0x00000100,
            SetInformation = 0x00000200,
            QueryInformation = 0x00000400,
            QueryLimitedInformation = 0x00001000,
            Synchronize = 0x00100000
        }
        [DllImport("kernel32.dll", SetLastError = true)]
        [ReliabilityContract(Consistency.WillNotCorruptState, Cer.Success)]
        [SuppressUnmanagedCodeSecurity]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool CloseHandle(IntPtr hObject);

        public ProcessThreadCollection Threads { get; }
        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern IntPtr GetModuleHandle([MarshalAs(UnmanagedType.LPWStr)] string lpModuleName);
        [DllImport("kernel32.dll", SetLastError = true)]
        [PreserveSig]
        public static extern uint GetModuleFileName
        (
            [In]
            IntPtr hModule,

            [Out]
            StringBuilder lpFilename,

            [In]
            [MarshalAs(UnmanagedType.U4)]
            int nSize
        );
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern int GetWindowTextLength(IntPtr hWnd);
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        [return: MarshalAs(UnmanagedType.U4)]
        private static extern int GetLongPathName(
        [MarshalAs(UnmanagedType.LPTStr)]
        string lpszShortPath,
        [MarshalAs(UnmanagedType.LPTStr)]
        StringBuilder lpszLongPath,
        [MarshalAs(UnmanagedType.U4)]
        int cchBuffer);




        private void proclist_ItemActivate()
        {
            //заполняем таблицу процессорв
            foreach (Process process in Process.GetProcesses())
            {
                
                dataGridView1.Rows.Add(process.ProcessName, process.Id, process.Threads.Count,process.PagedMemorySize64);

            }

            //заполняем таблицу потоков
            foreach (Process i in Process.GetProcesses())
            {
                foreach (ProcessThread thread in i.Threads)
                {
                    dataGridView2.Rows.Add(i.Id, i.ProcessName);
                }
            }

            Process process1 = Process.GetCurrentProcess();
            
            foreach (ProcessModule i in process1.Modules)
            {
                listBox1.Items.Add(i.FileName);
            }


            //идентификатор текущего процесса 
            label1.Text = "идентификатор процесса: " + GetCurrentProcessId();
            //псевдодескриптор текущего процесса
            label2.Text = "псевдодескриптор текущего процесса: " + GetCurrentProcess();
            //дескриптор текущего процесса
            IntPtr hCurrentIdDub;
            DuplicateHandle(
                Process.GetCurrentProcess().Handle,
                Process.GetCurrentProcess().Handle,
                Process.GetCurrentProcess().Handle,
                out hCurrentIdDub,
                0,
                false,
                (uint)DuplicateOptions.DUPLICATE_SAME_ACCESS);
            label3.Text = "дескриптор текущего процесса: " + hCurrentIdDub;
            //копия дескриптора текущего процесса
            IntPtr hCurrentIdOpen = OpenProcess(
                (uint)ProcessAccessFlags.DuplicateHandle, 
                true, 
                (uint)GetCurrentProcessId());
            label4.Text = "копия дескриптора текущего процесса: " + hCurrentIdOpen;
            //закрытие дескриптора, полученного функцией DuplicateHandl.
            CloseHandle(hCurrentIdDub);
            //закрытие дескриптора, полученного функцией OpenProcess.
            CloseHandle(hCurrentIdOpen);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            


        }

        private void button1_Click(object sender, EventArgs e)
        {
            foreach (string item in checkedListBox1.CheckedItems)
            {
                if (item == "дескриптор")
                {
                    string handle = textBox1.Text;
                    IntPtr sptr = new IntPtr(long.Parse(handle));

                    StringBuilder filePath = new StringBuilder(255);
                    GetModuleFileName(sptr, filePath, filePath.Capacity);
                    textBox3.Text = "path = " + filePath;

                    textBox2.Text = "name = " + Path.GetFileName(filePath.ToString());
                }
                if (item == "имя")
                {
                    string name = textBox1.Text;
                    Process[] vsProcs = Process.GetProcessesByName(name);

                    textBox2.Text = "handle = " + GetModuleHandle(name);

                    StringBuilder filePath = new StringBuilder(255);
                    GetModuleFileName(GetModuleHandle(name), filePath, filePath.Capacity);
                    textBox3.Text = "path = " + filePath;

                }
                if (item == "полное имя модуля")
                {
                    string shortPath = textBox1.Text;
                    //StringBuilder builder = new StringBuilder(255);
                    //int result = GetLongPathName(shortPath, builder, builder.Capacity);
                    //label6.Text = builder.ToString();
                    textBox3.Text = "name = " + Path.GetFileName(shortPath);
                    string path = Path.GetFileName(shortPath);
                    textBox2.Text = "handle = " + GetModuleHandle(path);

                }
            }
        }

        private void checkedListBox1_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            textBox2.Text = " ";
            textBox3.Text = " ";
            if (e.NewValue == CheckState.Unchecked )
            {
                if (checkedListBox1.CheckedItems.Count == 1)
                {
                    button1.Enabled = false;
                }
            }
            else
            {
                button1.Enabled = true;
            }
        }
    }
}