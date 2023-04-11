using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Messaging;
using System.Security.Cryptography.X509Certificates;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


namespace lab3_cs
{
    
    public class MyClass
    {
        //крит секция
        object block = new object();
        //мьютекс
        public static Mutex mutex = new Mutex(false, "MutexDiff");
        static int x = 0;
        //события
        //public static Thread MutexThread = new Thread(() => { Mutexx('#'); });
        public Thread Thrd;
        ManualResetEvent mre;
        // создаем семафор
        static Semaphore sem = new Semaphore(2, 2);
        

        public MyClass()
        {
            
        }
        public MyClass(string name, ManualResetEvent evt)
        {
            Thrd = new Thread(this.eventt);
            Thrd.Name = name;
            mre = evt;
            Thrd.Start();
        }

        /// <summary>
        /// семафоры
        /// </summary>
        public void semaphore()
        {
            sem.WaitOne();  // ожидаем, когда освободиться место

            Console.WriteLine($"поток {Thread.CurrentThread.GetHashCode()} начинает работу");
            Console.WriteLine($"поток {Thread.CurrentThread.GetHashCode()} работает");
            
            Thread.Sleep(500);

            Console.WriteLine($"поток {Thread.CurrentThread.GetHashCode()} заканчивает работу");

            sem.Release();  // освобождаем место

        }


        /// <summary>
        /// СОБЫТИЯ
        /// </summary>
        public void eventt()
        {
            Console.WriteLine("Внутри потока " + Thrd.Name);

            for (int i = 0; i < 5; i++)
            {
                Console.WriteLine(Thrd.Name);
                Thread.Sleep(500);
            }

            Console.WriteLine(Thrd.Name + " завершен!");

            // Уведомление о событии. Сигнальное состояние
            mre.Set();
        }
        /// <summary>
        /// КРИТИЧЕСКАЯ СЕКЦИЯ
        /// </summary>
        public void Crit_section()
        {
            //опеределяем, какой поток выполняет крит секцию
            int hash = Thread.CurrentThread.GetHashCode();

            //lock - крит секция
            lock (block)
            {
                Console.WriteLine("поток начал работу");
                for (int i = 0; i < 5; i++)
                {
                    Console.WriteLine($"Поток {hash} : итерация цикла: {i}");
                    Thread.Sleep(100);
                }
                
                //Console.WriteLine(new string('-', 20));
                Console.WriteLine("поток завершил работу");
            }
            
        }
        /// <summary>
        /// МЬЮТЕКСЫ
        /// </summary>
        public static void Mutexx(char symbol)
        {
            
            mutex.WaitOne();
            int posX = Console.CursorTop;
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    Console.SetCursorPosition(j, i + posX);
                    Console.WriteLine("{0}", symbol);
                    Thread.Sleep(100);
                }
            }
            mutex.ReleaseMutex();
        }

    }
    unsafe internal class Program
    {
        [DllImport("kernel32.dll")]
        static extern IntPtr CreateEvent(IntPtr lpEventAttributes, bool bManualReset, bool bInitialState, string lpName);
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern UInt32 WaitForSingleObject(IntPtr hHandle, UInt32 dwMilliseconds);
        const UInt32 INFINITE = 0xFFFFFFFF;
        [DllImport("kernel32", CharSet = CharSet.Ansi)]
        public static extern IntPtr CreateThread(IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress,
        IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);
        [DllImport("kernel32.dll")]
        static extern bool SetEvent(IntPtr hEvent);
        static void Main(string[] args)
        {
            Console.SetWindowSize(80, 40);
            
            MyClass instance = new MyClass();

            

            while (true)
            {
                Console.ReadKey();
                Console.Write("1. критические секции\r\n2. мьютексы\r\n3. события\r\n4. семафоры или ждущие таймеры\r\nвведите число: ");
                string number = Console.ReadLine();
                //Console.Write(number);


                switch (number)
                {
                    case "1":
                        
                        for (int i = 0; i < 3; i++)
                        {
                            new Thread(instance.Crit_section).Start();
                        }
                        break;
                    case "2":

                        //Thread MutexThread = new Thread(() =>{instance.Mutexx('#');});MutexThread.Start();
                        
                        MyClass.Mutexx('#');
                        
                        break;
                    case "3":

                        ManualResetEvent evtObj = new ManualResetEvent(false);

                        MyClass mt1 = new MyClass("Событийный поток 1", evtObj);

                        Console.WriteLine("Основной поток ожидает событие");

                        evtObj.WaitOne();

                        Console.WriteLine("Основной поток получил уведомление о событии от первого потока");

                        evtObj.Reset();

                        mt1 = new MyClass("Событийный поток 2", evtObj);

                        evtObj.WaitOne();

                        Console.WriteLine("Основной поток получил уведомление о событии от второго потока");
                        Console.ReadLine();
                        break;
                    case "4":
                        for (int i = 0; i < 6; i++)
                        {
                            new Thread(instance.semaphore).Start();
                        }
                        break;
                    default:
                        Console.WriteLine("неверное значение");
                        break;

                }
                
            }
        }
        
    }

}
