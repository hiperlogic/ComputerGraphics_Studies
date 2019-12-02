    #version 150 core

    out vec4 currColor;

    uniform vec4 recColor;

    void main()
    {
        currColor = recColor;
    }

