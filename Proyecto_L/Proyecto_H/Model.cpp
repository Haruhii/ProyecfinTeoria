#include "Model.h"



Model::Model()
{}

void Model::LoadModel(const std::string & fileName)
{
	time_t initial_time = time(NULL);
	Assimp::Importer importer;//					Pasa de Polygons y Quads a triangulos, modifica orden para el origen, generar normales si el  objeto no tiene, trata v�rtices iguales como 1 solo
	scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{	
		printf("Falló en cargar el modelo: %s Error: %s\n", fileName.c_str(), importer.GetErrorString());
		return;
	}
	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
	importer.FreeScene();
	time_t final_time = time(NULL);
	printf("Tiempo: %ld\n", final_time - initial_time);
}

void Model::LoadModel(const std::string & fileName, const std::string & fileNameOut) {
	Assimp::Importer importer;//					Pasa de Polygons y Quads a triangulos, modifica orden para el origen, generar normales si el  objeto no tiene, trata v�rtices iguales como 1 solo
	Assimp::Exporter exporter;
	const aiExportFormatDesc* format = exporter.GetExportFormatDescription(14);
	for (int i = 0; i <= 20; i++) {
		printf("%d %s\n",i,exporter.GetExportFormatDescription(i)->id);
	}
	scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("Falló en cargar el modelo: %s Error: %s\n", fileName.c_str(), importer.GetErrorString());
		return;
	}
	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
	exporter.Export(scene, format->id, fileNameOut, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	importer.FreeScene();
}

void Model::ClearModel()
{
	for (size_t i = 0; i < MeshList.size(); i++)
	{
		if (MeshList[i])
		{
			delete MeshList[i];
			MeshList[i] = nullptr;

		}
	}

	for (size_t i = 0; i < TextureList.size(); i++)
	{
		if (TextureList[i])
		{
			delete TextureList[i];
			TextureList[i] = nullptr;
		}
	}

}

void Model::RenderModel(){
	for (size_t i = 0; i < MeshList.size(); i++)
	{
		unsigned int materialIndex = meshTotex[i];
		if (((!materialIndex) < TextureList.size()) && TextureList[materialIndex])
		{
			TextureList[materialIndex]->UseTexture();
		}
		MeshList[i]->RenderMesh();

	}
}

Model::~Model()
{
}

void Model::LoadNode(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i <node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh * mesh, const aiScene * scene)
{

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y ,mesh->mVertices[i].z });
		//UV
		if (mesh->mTextureCoords[0])//si tiene coordenadas de texturizado
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y});
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f,0.0f });
		}
		//Normals importante, las normales son negativas porque la luz interact�a con ellas de esa forma, c�mo se vio con el dado/cubo
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,-mesh->mNormals[i].y ,-mesh->mNormals[i].z });
	}
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	MeshList.push_back(newMesh);
	meshTotex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene * scene)
{
	TextureList.resize(scene->mNumMaterials);
	for (size_t i = 0; i < scene ->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		TextureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE	))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE,0,&path)==AI_SUCCESS)
			{
				int idx = int(std::string(path.data).rfind("\\"));//para quitar del path del modelo todo lo que este antes del \ de ubicaci�n de directorio
				fs::path p = std::string(path.data).substr(idx + 1);
				p = fs::absolute(p).filename();
				std::string pstr = p.string();
				std::string filename = pstr;
				std::string tga ="tga";
				std::string png = "png";
				std::size_t existetga = filename.find(tga);
				std::size_t existepng= filename.find(png);
				std::string texPath = std::string("Textures/") + filename;
				TextureList[i] = new Texture(texPath.c_str());
				if (existetga != std::string::npos || existepng != std::string::npos)
				{
					if (!TextureList[i]->LoadTextureA())
					{
						printf("Fall� en cargar la Textura :%s\n", texPath.c_str());
						delete TextureList[i];
						TextureList[i] = nullptr;
					}
				}
				else
				{
					if (!TextureList[i]->LoadTexture())
					{
						printf("Fall� en cargar la Textura :%s\n", texPath.c_str());
						delete TextureList[i];
						TextureList[i] = nullptr;
					}
				}
			}
		}
		if (!TextureList[i])
		{
			TextureList[i] = new Texture("Textures/plain.png"); //textura que se aplicar� a los modelos si no tienen textura o la textura no se puede cargar
			TextureList[i]->LoadTextureA();
		}

	}
}
