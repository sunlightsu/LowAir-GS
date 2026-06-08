#include "ObjModelRenderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QDebug>
#include <QFileInfo>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

ObjModelRenderer::ObjModelRenderer()
{
    initializeOpenGLFunctions();
}

ObjModelRenderer::~ObjModelRenderer() = default;

bool ObjModelRenderer::load(const QString& filePath,
                             const QVector3D& offset,
                             float scale,
                             const QVector3D& color)
{
    m_offset = offset;
    m_scale  = scale;
    m_color  = color;
    m_modelName = QFileInfo(filePath).baseName();
    m_meshes.clear();
    m_loaded = false;
    m_triangleCount = 0;
    m_vertexCount   = 0;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filePath.toStdString(),
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qWarning() << "[ObjModelRenderer] Failed to load:" << filePath
                   << "Error:" << importer.GetErrorString();
        return false;
    }

    // 遍历所有 Mesh
    for (unsigned int mi = 0; mi < scene->mNumMeshes; ++mi) {
        const aiMesh* mesh = scene->mMeshes[mi];
        SubMesh sub;

        // 顶点和法向量
        sub.vertices.reserve(mesh->mNumVertices * 3);
        sub.normals.reserve(mesh->mNumVertices * 3);
        for (unsigned int vi = 0; vi < mesh->mNumVertices; ++vi) {
            sub.vertices.push_back(mesh->mVertices[vi].x);
            sub.vertices.push_back(mesh->mVertices[vi].y);
            sub.vertices.push_back(mesh->mVertices[vi].z);
            if (mesh->HasNormals()) {
                sub.normals.push_back(mesh->mNormals[vi].x);
                sub.normals.push_back(mesh->mNormals[vi].y);
                sub.normals.push_back(mesh->mNormals[vi].z);
            } else {
                sub.normals.push_back(0.0f);
                sub.normals.push_back(1.0f);
                sub.normals.push_back(0.0f);
            }
        }

        // 索引
        sub.indices.reserve(mesh->mNumFaces * 3);
        for (unsigned int fi = 0; fi < mesh->mNumFaces; ++fi) {
            const aiFace& face = mesh->mFaces[fi];
            if (face.mNumIndices == 3) {
                sub.indices.push_back(face.mIndices[0]);
                sub.indices.push_back(face.mIndices[1]);
                sub.indices.push_back(face.mIndices[2]);
            }
        }

        m_triangleCount += static_cast<int>(mesh->mNumFaces);
        m_vertexCount   += static_cast<int>(mesh->mNumVertices);
        m_meshes.push_back(std::move(sub));
    }

    m_loaded = true;
    qDebug() << "[ObjModelRenderer] Loaded:" << filePath
             << "| meshes:" << m_meshes.size()
             << "| vertices:" << m_vertexCount
             << "| triangles:" << m_triangleCount;
    return true;
}

void ObjModelRenderer::render()
{
    if (!m_loaded) return;

    glPushMatrix();
    glTranslatef(m_offset.x(), m_offset.y(), m_offset.z());
    glScalef(m_scale, m_scale, m_scale);

    // 简单漫反射光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat lightPos[]  = { 50.0f, 100.0f, 50.0f, 1.0f };
    GLfloat lightAmb[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiff[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiff);

    GLfloat matAmb[]  = { m_color.x() * 0.4f, m_color.y() * 0.4f, m_color.z() * 0.4f, 1.0f };
    GLfloat matDiff[] = { m_color.x(),         m_color.y(),         m_color.z(),         1.0f };
    GLfloat matSpec[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   matAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   matDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  matSpec);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    for (const auto& sub : m_meshes) {
        if (sub.indices.empty()) continue;
        glVertexPointer(3, GL_FLOAT, 0, sub.vertices.data());
        glNormalPointer(GL_FLOAT, 0, sub.normals.data());
        glDrawElements(GL_TRIANGLES,
                       static_cast<GLsizei>(sub.indices.size()),
                       GL_UNSIGNED_INT,
                       sub.indices.data());
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glPopMatrix();
}
