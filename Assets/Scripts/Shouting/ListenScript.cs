using UnityEngine;
using System.Collections;
using System.IO;
using System.Runtime.InteropServices;

public class ListenScript : MonoBehaviour {

    /*
    [DllImport("splistener", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    */

    [DllImport("splistener")]
    private static extern float initListener();

    void Awake() {
        Debug.Log(initListener());
    }

	// Use this for initialization
	void Start () {
        StreamWriter S = new StreamWriter(new FileStream("log.txt", FileMode.OpenOrCreate, FileAccess.Write));
        S.WriteLine(Application.dataPath);
        S.Close();
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
