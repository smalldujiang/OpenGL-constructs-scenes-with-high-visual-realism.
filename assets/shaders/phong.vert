#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 model;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat3 normalMatrix;


void main()
{
    //������Ķ���λ�ã�ת��Ϊ�������
    vec4 transformPosition = vec4(aPos, 1.0);

    transformPosition = model * transformPosition;

    worldPosition = transformPosition.xyz;

    vec4 position = vec4(aPos, 1.0);
    gl_Position = projectionMatrix * viewMatrix * transformPosition;    

    uv = aUV;
    //��������λ�÷���ƫ�����ŵ�������ᵼ�·����������ı䣬������Ҫ������������ֵ
    normal = normalMatrix * aNormal;
}
