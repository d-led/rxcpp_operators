using System;
using System.Linq;
using System.Reactive.Linq;
using System.Text;

namespace csharp_versions
{
    class WindowingExample
    {
        public void Run()
        {
            int counter = 0, count = 0;

            var values = Observable.Range(1, 1000000)
                            .Window(TimeSpan.FromSeconds(1), 100000);

            values.Subscribe(window =>
            {
                int id = counter++;
                Console.WriteLine("[window {0}] Created window", id);

                window.Count()
                    .Subscribe(c =>
                    {
                        Console.WriteLine("Count in window: {0}", c);
                    });

                window.Scan(new StringBuilder(), (w, val) =>
                {
                    w.Append(val);
                    return w;
                })
                .StartWith(new StringBuilder())
                .TakeLast(1)
                .Catch(Observable.Empty<StringBuilder>())
                .Subscribe(
                    w =>
                    {
                        Console.WriteLine("Len: {0} ({1}...)", w.Length, w.Length>0 ? w.ToString().Substring(0, 42) : "");
                    }
                );

                window.Subscribe(
                    v => count++
                    ,
                    () => Console.WriteLine("[window {0}] OnCompleted: {1}", id, count)
                );
            });
        }
    }
}
