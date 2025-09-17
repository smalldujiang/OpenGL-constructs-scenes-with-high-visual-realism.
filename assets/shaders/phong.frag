#version 330 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;

uniform vec3 cameraPosition;

//͸����
uniform float opacity;


//������
uniform vec3 ambientColor;

uniform float shiness;

struct SpotLight
{
	vec3 position;
	vec3 targetDirection;
	vec3 color;
	float outerLine;
	float innerLine;
	float specularIntensity;
};
uniform SpotLight spotLight;

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float specularIntensity;
};
uniform DirectionalLight directionalLight;

struct PointLight
{
	vec3 position;
	vec3 color;
	float specularIntensity;

	float K2;
	float K1;
	float Kc;
};
#define POINT_LIGHT_NUM 4
uniform PointLight pointLights[POINT_LIGHT_NUM];

//�������������
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal)
{
	//����diffuse
	float diffuse = clamp(dot(-lightDir, normal), 0.0, 1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;

	return diffuseColor;
}

//���㾵�淴�����
vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity)
{
	//����specular
	//��ֹ�����Ч��
	float dotResult = dot(-lightDir, normal);
	float flag = step(0.0, dotResult);

	vec3 lightReflect = normalize(reflect(lightDir, normal));
	float specular = max(dot(lightReflect, -viewDir), 0.0);


	//ʹ��Blinn-Phongģ��

	//vec3 halfwayDir = normalize(lightDir + viewDir);
	//float specular = max(dot(normal, halfwayDir), 0.0);

	//���ƹ�ߴ�С
	specular = pow(specular, shiness);

	float specularMask = texture(specularMaskSampler, uv).r;

	vec3 specularColor = lightColor * specular* intensity * specularMask * flag;

	return specularColor;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	//��ȡ���嵱ǰ���ص���ɫ
	vec3 objectColor = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(worldPosition - light.position);
	vec3 targetDir = normalize(light.targetDirection);

	//����spotLight�����䷶Χ
	float cGamma = dot(lightDir, targetDir);
	float intensity = clamp( (cGamma - light.outerLine) / (spotLight.innerLine - light.outerLine), 0.0, 1.0);

	//����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

	//����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

	return (diffuseColor + specularColor) *  intensity;
}

vec3 calculateDirectionLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	//��ȡ���嵱ǰ���ص���ɫ
	vec3 objectColor = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(light.direction);

	//����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);

	//����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

	return diffuseColor + specularColor;

}
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	//��ȡ���嵱ǰ���ص���ɫ
	vec3 objectColor = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(worldPosition - light.position);
	//����˥��ֵ
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0 / (light.K2 * dist * dist + light.K1 * dist + light.Kc); 
	//����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);
	//����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

	return diffuseColor + specularColor;
}

void main()
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	//��ȡ���嵱ǰ���ص���ɫ
	vec3 objectColor = texture(sampler, uv).xyz;
	float alpha = texture(sampler, uv).a;


	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(worldPosition - spotLight.position);
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	vec3 targetDirN = normalize(spotLight.targetDirection);

	//result += calculateSpotLight(spotLight, normalN, viewDir);
	result += calculateDirectionLight(directionalLight, normalN, viewDir);

	for(int i = 0 ; i < POINT_LIGHT_NUM; i++){

		result += calculatePointLight(pointLights[i], normalN, viewDir);

	}
	//���������
	vec3 ambientColor = objectColor * ambientColor;

	vec3 finalColor = result + ambientColor;

	FragColor = vec4(finalColor, alpha * opacity);

}
