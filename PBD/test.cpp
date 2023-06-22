#include <iostream>

#include "VtEngine.hpp"
#include "GameInstance.hpp"
#include "Resource.hpp"
#include "Scene.hpp"
#include "Helper.hpp"

using namespace Velvet;

std::vector<glm::vec3> vertices;
vector<glm::vec3> normals;
vector<glm::vec2> uvs;
vector<unsigned int> indices;


int binomialCoefficient(int n, int k)
{
    if (k == 0 || k == n) {
        return 1;
    }

    return binomialCoefficient(n - 1, k - 1) + binomialCoefficient(n - 1, k);
}

glm::vec3 calculateBezierPoint(const std::vector<glm::vec3>& controlPoints, float t)
{
    int n = controlPoints.size() - 1;
    glm::vec3 point(0.0f);

    for (int i = 0; i <= n; i++) {
        float coeff = binomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
        point += controlPoints[i] * coeff;
    }

    return point;
}

void createCurvedCylinder(const std::vector<glm::vec3>& controlPoints, float radius, int numSlices, int numStacks)
{
    const float angleIncrement = 2 * glm::pi<float>() / numSlices;
    const float tIncrement = 1.0f / numStacks;

    for (int i = 0; i <= numStacks; i++) {
        float t = i * tIncrement;

        // Calculate the position of the vertex using the Bezier curve
        glm::vec3 position = calculateBezierPoint(controlPoints, t);

        for (int j = 0; j <= numSlices; j++) {
            // Calculate the angle for the current slice
            float angle = j * angleIncrement;

            // Calculate the position of the vertex on the circle
            float x = position.x + radius * cos(angle);
            float y = position.y;
            float z = position.z + radius * sin(angle);

            vertices.push_back(glm::vec3(x, y, z));

            // Calculate the normal vector at the vertex
            glm::vec3 normal = glm::normalize(glm::vec3(x - position.x, 0.0f, z - position.z));
            normals.push_back(normal);

            // Calculate the texture coordinates at the vertex
            float s = static_cast<float>(j) / numSlices;
            float t = static_cast<float>(i) / numStacks;
            uvs.push_back(glm::vec2(s, t));
        }
    }

    for (int i = 0; i < numStacks; i++) {
        for (int j = 0; j < numSlices; j++) {
            // Calculate the indices of the vertices that make up the current face
            int index0 = i * (numSlices + 1) + j;
            int index1 = index0 + 1;
            int index2 = (i + 1) * (numSlices + 1) + j;
            int index3 = index2 + 1;

            // Add the indices in the correct order for face culling
            indices.push_back(index2);
            indices.push_back(index1);
            indices.push_back(index0);

            indices.push_back(index2);
            indices.push_back(index3);
            indices.push_back(index1);
        }
    }
}





void creatMesh()
{
    const float radius = 0.5f;
    const float height = 5.0f;
    const int numSlices = 32;
    const int numStacks = 4;

    const float angleIncrement = 2 * 3.1415926f / numSlices;
    const float heightIncrement = height / numStacks;

    for (int i = 0; i <= numStacks; i++) {
        for (int j = 0; j <= numSlices; j++) {
            // Calculate the position of the vertex
            float x = radius * cos(j * angleIncrement);
            float y = i * heightIncrement - height / 2.0f;
            float z = radius * sin(j * angleIncrement);
            vertices.push_back(glm::vec3(x, y, z));

            // Calculate the normal vector at the vertex
            glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));
            normals.push_back(normal);

            // Calculate the texture coordinates at the vertex
            float s = (float)j / numSlices;
            float t = (float)i / numStacks;
            uvs.push_back(glm::vec2(s, t));
        }
    }

    for (int i = 0; i < numStacks; i++) {
        for (int j = 0; j < numSlices; j++) {
            // Calculate the indices of the vertices that make up the current face
            int index0 = i * (numSlices + 1) + j;
            int index1 = index0 + 1;
            int index2 = (i + 1) * (numSlices + 1) + j;
            int index3 = index2 + 1;

            // 一定注意这个顺序，不然使用面剔除的话，正好和结果相反
            indices.push_back(index2);
            indices.push_back(index1);
            indices.push_back(index0);

            indices.push_back(index2);
            indices.push_back(index3);
            indices.push_back(index1);
        }
    }
}

class SceneRope : public Scene
{
public:
	SceneRope() { name = "Cloth / Rope"; }

	void PopulateActors(GameInstance* game)  override
	{
		SpawnCameraAndLight(game);
		SpawnInfinitePlane(game);

		auto sphere = SpawnSphere(game);
		float radius = 0.5f;
		sphere->GetComponent<Collider>()->enabled = false;
		sphere->Initialize(glm::vec3(0, radius, 0), glm::vec3(radius));

        std::vector<glm::vec3> controlPoints;
        controlPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));      // 第一个控制点
        controlPoints.push_back(glm::vec3(1.0f, 2.0f, 0.0f));      // 第二个控制点
        controlPoints.push_back(glm::vec3(3.0f, 2.0f, 0.0f));      // 第三个控制点
        controlPoints.push_back(glm::vec3(4.0f, 0.0f, 0.0f));      // 第四个控制点

        float ra = 0.5f;
        int numSlices = 32;
        int numStacks = 4;

        createCurvedCylinder(controlPoints, ra, numSlices, numStacks);

        //auto mesh =  Resource::LoadMesh("cube.obj");
        
        auto mesh = make_shared<Mesh>(vertices, normals, uvs, indices);

        std::cout << vertices.size() << std::endl;
        std::cout << normals.size() << std::endl;
        std::cout << uvs.size() << std::endl;

        auto cube = SpawnTestRope(game,mesh);
        cube->Initialize(glm::vec3(0.0f, 2.0f, 0.0f));
	}
};


int main() 
{	
    //=====================================
        // 1. Create graphics
        //=====================================
    auto engine = make_shared<VtEngine>();

    //=====================================
    // 2. Instantiate actors
    //=====================================

    vector<shared_ptr<Scene>> scenes = {
        make_shared<SceneRope>(),
        //make_shared<ScenePremitiveRendering>(),
    };
    engine->SetScenes(scenes);

    //=====================================
    // 3. Run graphics
    //=====================================
    return engine->Run();
}