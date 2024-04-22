#include "anepch.h"
#include "MeshLoader.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Engine
{
    Assimp::Importer MeshLoader::Importer;

    MeshAsset MeshLoader::LoadMesh(const char* path)
    {
        constexpr int flags =
            aiProcess_FlipWindingOrder | // We're using clockwise winding.
            aiProcess_Triangulate |
            aiProcess_PreTransformVertices |
            aiProcess_CalcTangentSpace |
            aiProcess_GenSmoothNormals |
            aiProcess_MakeLeftHanded |
            aiProcess_GlobalScale |
            aiProcessPreset_TargetRealtime_MaxQuality;
        return LoadMesh(path, flags);
    }

    MeshAsset MeshLoader::LoadMesh(const char* path, const int flags)
    {
        const aiScene* scene = Importer.ReadFile(path, flags);
        if (!scene)
        {
            ANE_ELOG_INFO("Unable to load mesh {}: {}", path, Importer.GetErrorString());
            return { };
        }

        return InitScene(scene);
    }

    MeshAsset MeshLoader::InitScene(const aiScene* scene)
    {
        MeshAsset meshAsset = { };
        meshAsset.SubMeshes.resize(scene->mNumMeshes);

        for (unsigned int meshIndex = 0; meshIndex < meshAsset.SubMeshes.size(); meshIndex++)
        {
            meshAsset.SubMeshes[meshIndex].StartOffset = meshAsset.NumVertices;
            meshAsset.NumVertices += scene->mMeshes[meshIndex]->mNumVertices;
        }

        for (unsigned int meshIndex = 0; meshIndex < meshAsset.SubMeshes.size(); meshIndex++)
        {
            const aiMesh* mesh = scene->mMeshes[meshIndex];
            meshAsset.SubMeshes[meshIndex] = InitMesh(mesh, scene);
        }

        return meshAsset;
    }

    Mesh MeshLoader::InitMesh(const aiMesh* mesh, const aiScene* scene)
    {
        Mesh finalMesh = { };
        finalMesh.Material = mesh->mMaterialIndex;

        BoundingBox bounds;
        finalMesh.Vertices.reserve(mesh->mNumVertices);
        for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
        {
            const aiVector3D* pos = &(mesh->mVertices[vertexIndex]);
            const aiVector3D* normal = &(mesh->mNormals[vertexIndex]);
            const aiVector3D* tangent = (mesh->HasTangentsAndBitangents()) ? &(mesh->mTangents[vertexIndex]) : nullptr;
            const aiVector3D* biTangent = (mesh->HasTangentsAndBitangents()) ? &(mesh->mBitangents[vertexIndex]) : nullptr;
            const aiVector3D* uv = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][vertexIndex]) : nullptr;

            Vertex vertex = { };
            vertex.Position = { pos->x, pos->y, pos->z };
            vertex.Normal = { normal->x, normal->y, normal->z };
            vertex.Color = Vector4{ vertex.Normal, 1 } * .5f + .5f;
            vertex.UvX = uv->x;
            vertex.UvY = uv->y;

            bounds.Min.x = fmin(vertex.Position.X, bounds.Min.x);
            bounds.Min.y = fmin(vertex.Position.Y, bounds.Min.y);
            bounds.Min.z = fmin(vertex.Position.Z, bounds.Min.z);

            bounds.Max.x = fmin(vertex.Position.X, bounds.Min.x);
            bounds.Max.y = fmin(vertex.Position.Y, bounds.Min.y);
            bounds.Max.z = fmin(vertex.Position.Z, bounds.Min.z);

            finalMesh.Vertices.push_back(vertex);
        }

        bounds.Size = bounds.Max - bounds.Min;

        finalMesh.Indices.reserve(mesh->mNumFaces * 3);
        for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
        {
            const aiFace face = mesh->mFaces[faceIndex];
            assert(face.mNumIndices == 3);
            finalMesh.Indices.push_back(face.mIndices[0]);
            finalMesh.Indices.push_back(face.mIndices[1]);
            finalMesh.Indices.push_back(face.mIndices[2]);
        }

        return finalMesh;
    }
}
