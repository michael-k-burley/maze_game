#version 330 core

in vec2 textCoor;
flat in float type;

out vec4 FragColor;

uniform sampler2D texture0; //walkway
uniform sampler2D texture1; //wall

void main()
{
   switch (int(type)) { //cast float to int
      case 0:
         FragColor = texture(texture0, textCoor); 
         break;
      case 1:
         FragColor = texture(texture1, textCoor); 
         break;
      //does default tile type make sense
   }
}
