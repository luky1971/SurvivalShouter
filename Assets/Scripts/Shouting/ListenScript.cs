using UnityEngine;
using System.Runtime.InteropServices;
using System.Text;

public class ListenScript : MonoBehaviour {
    /*
    [DllImport("splistener", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    */

    // [DllImport("splistener", CallingConvention = CallingConvention.Cdecl)]

    // import splistener library functions
    [DllImport("splistener")]
    private static extern bool spInitListener(string model_path, string mic, int sample_rate);
    [DllImport("splistener")]
    private static extern bool spListen(StringBuilder words, int len);
    [DllImport("splistener")]
    private static extern void spCleanUp();
    [DllImport("splistener")]
    private static extern string spGetError();


    void Awake() {
        if (spInitListener(Application.dataPath + "/libs/pocketsphinx/model/en-us/", null, 16000)) {
            Debug.Log("splistener failed to initialize!");
            Debug.Log("splistener error: " + spGetError());
        }
        else {
            // start voice command coroutine
            Debug.Log("splistener initialized successfully!");
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
