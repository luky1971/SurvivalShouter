using UnityEngine;
using System.Runtime.InteropServices;

public class ListenScript : MonoBehaviour {
    /*
    [DllImport("splistener", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    */

    // [DllImport("splistener", CallingConvention = CallingConvention.Cdecl)]

    // import splistener library functions
    [DllImport("splistener")]
    private static extern int spInitListener(string model_path);
    [DllImport("splistener")]
    private static extern void spCleanUp();


    void Awake() {
        int res = spInitListener(Application.dataPath + "/libs/pocketsphinx/model/en-us/");
        Debug.Log(res);
        if (res < 1) {
            Debug.Log("splistener failed to initialize!");
        }
        else {
            // start voice command coroutine
        }
    }

	// Use this for initialization
	void Start() {

	}
	
	// Update is called once per frame
	void Update() {
	
	}

    void OnApplicationQuit() {
        spCleanUp();
    }
}
