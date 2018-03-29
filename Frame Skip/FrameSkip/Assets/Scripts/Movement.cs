using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

    public bool rightSide = true;
    public bool slowFPS = false;
    float moveSpeed = 0.01f;
    // Use this for initialization
    void Start ()
    {
        QualitySettings.vSyncCount = 0;
        Application.targetFrameRate = -1;
        GetComponent<Animator>().SetFloat("speedMultiplier", 0.5f);
        GetComponent<Rigidbody2D>().freezeRotation = true;

    }
	
	// Update is called once per frame
	void Update () {

        if (Input.GetKey("d") || Input.GetKey(KeyCode.RightArrow))
        {
            GetComponent<Animator>().SetBool("isWalking", true);
            GetComponent<Rigidbody2D>().AddForce(new Vector2(moveSpeed, 0), ForceMode2D.Impulse);
            if (!rightSide)
            {
                GetComponent<SpriteRenderer>().flipX = false;
                rightSide = true;
            }
        }
        else if (Input.GetKey("a") || Input.GetKey(KeyCode.LeftArrow))
        {
            GetComponent<Animator>().SetBool("isWalking", true);
            GetComponent<Rigidbody2D>().AddForce(new Vector2(-moveSpeed, 0),ForceMode2D.Impulse);
            if (rightSide)
            {
                GetComponent<SpriteRenderer>().flipX = true;
                rightSide = false;
            }
        }

        if (Input.GetKeyUp("d") || Input.GetKeyUp(KeyCode.RightArrow) || Input.GetKeyUp("a") || Input.GetKeyUp(KeyCode.LeftArrow))
        {
            GetComponent<Animator>().SetBool("isWalking", false);
            //moveSpeed = 0.01f;
        }

        if (Input.GetKeyDown("w") || Input.GetKeyDown(KeyCode.UpArrow) || Input.GetKeyDown(KeyCode.Space))
        {
            GetComponent<Rigidbody2D>().AddForce(new Vector2(0, 6.5f), ForceMode2D.Impulse);
        }
        Debug.Log(transform.position.x);
        if (transform.position.x >= -19.0f && transform.position.x <= -18.0f)
        {
            Application.targetFrameRate = -1;
            GetComponent<Animator>().SetFloat("speedMultiplier", 0.2f);
        }
        if (transform.position.x >= 2.0f && transform.position.x <= 3.0f)
        {
            Application.targetFrameRate = 5;
            moveSpeed = 1.0f;
            slowFPS = true;
        }
    }
}
