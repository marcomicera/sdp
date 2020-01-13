/*
 * Scrivere una classe C# con all'interno un evento publico
 * e una proprietà di tipo string.
 * Ogni volta che la proprietà viene modificata,
 * se il nuovo valore è diverso da quello salvato nella
 * proprietà, si attiva l'evento passandoli come parametro.
 *
 * Fiddle: https://dotnetfiddle.net/tRln7c
*/

using System;

public delegate void Handler(Object source, string newString);

public class Observed {

	public event Handler Click;

	private string _s;

	public string s {
		get {
			return _s;
		}
		set {
			if (value != _s) {
				Click(this, value);
				_s = value;
			}
		}
	}

	public Observed(string init) {
		_s = init;
	}
}

public class Program {
	public static void MyHandler(Object source, string newString) {
		Console.WriteLine("New value: " + newString);	
	}

	public static void Main(String[] args) {
		Observed o = new Observed("Initial string");
		o.Click += new Handler(MyHandler);
		o.s = "new string";
		o.s = "new string 2";
	}
}
