using UnityEngine;
using System.Collections;

public class walk : MonoBehaviour {
    public bool rightSide = true;
	// Use this for initialization
	void Start () {
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = 5;
        //Gravity needs to be 1. normal speed -> 9
        //slow jump 3.0f, fast jump 9.0f
        //GetComponent<Animator>().StartPlayback();
    }
	
	// Update is called once per frame
	void Update () {

        if (Input.GetKey("d"))
        {
            GetComponent<Animator>().SetBool("isWalking", true);
            //GetComponent<Animator>().StopPlayback();
            transform.Translate(new Vector2(0.1f,0));
            if (!rightSide)
            {
                GetComponent<SpriteRenderer>().flipX = false;
                rightSide = true;
            }
        }
        else if(Input.GetKey("a"))
        {
            GetComponent<Animator>().SetBool("isWalking", true);
            //GetComponent<Animator>().StopPlayback();
            transform.Translate(new Vector2(-0.1f, 0));
            if (rightSide)
            {
                GetComponent<SpriteRenderer>().flipX = true;
                rightSide = false;
            }
        }
        if(Input.GetKeyUp("d") || Input.GetKeyUp("a"))
        {
            GetComponent<Animator>().SetBool("isWalking", false);
            //GetComponent<Animator>().StartPlayback();
        }
        if (Input.GetKeyDown("w"))
        {
            GetComponent<Rigidbody2D>().AddForce(new Vector2(0, 3.0f), ForceMode2D.Impulse);
        }

        if (Input.GetKeyDown("o"))
        {      
            Application.targetFrameRate = 5;
        }
        else if (Input.GetKeyDown("p"))
        {
            Application.targetFrameRate = -1;
        }

    }
}
