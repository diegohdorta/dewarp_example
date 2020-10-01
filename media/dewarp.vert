attribute vec4 vPosition;     
attribute vec2 my_Texcoor;   
uniform   mat4 u_TransMatrix;     
varying vec2 vTexcoor;   

void main()                   
{
vTexcoor = my_Texcoor;
gl_Position = u_TransMatrix*vPosition;
}                         
