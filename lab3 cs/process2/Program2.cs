using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using lab3_cs;

namespace process2
{
    public class Proc2
    {
        public Proc2()
        {

        }
        public static Mutex mutex = new Mutex(false, "MutexDiff");
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
    internal class Program2
    {
        static void Main(string[] args)
        {
            Console.Read();
            Proc2.Mutexx('*');
            //proc.currentThread = new Thread(() => { proc.Mutexx('*'); });

            //proc.currentThread.Start();

            Console.Read();
        }
    }
}
