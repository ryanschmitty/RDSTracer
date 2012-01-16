/**
* File: RDSIndirectIllum.cpp
* --------------------
* Implementation for Indirect Illumination functions.
* Author: Ryan Schmitt
*/

#include "RDSIndirectIllum.h"
#define GLM_FORCE_INLINE
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace RDST
{
   Rasterizer::Rasterizer(int w, int h, const SceneDescription& desc)
   : width(w), height(h), desc(desc)
   {
      initGL();

      glGenFramebuffersEXT(1, &fbo);
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

      glGenTextures(1, &depthtexture);
      glBindTexture(GL_TEXTURE_2D, depthtexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthtexture, 0);

      glGenTextures(1, &colortexture);
      glBindTexture(GL_TEXTURE_2D, colortexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colortexture, 0);

      GLenum attachmentList[] = { GL_COLOR_ATTACHMENT0_EXT };
      glDrawBuffers(1, attachmentList);

      GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
      if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
         printf("Rasterizer: Framebuffer loading error...\n");
         exit(-1);
      }

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); //unbind fbo for now
//      printf("lol\n");

      loadVBO(desc);

//      glMatrixMode(GL_PROJECTION);
//      glLoadIdentity();
//      gluPerspective(90,
//                     1.f,
//                     0.001f,
//                     100.f);
//      glMatrixMode(GL_MODELVIEW);
//      glViewport(0, 0, 8, 8);
//      
//      lights(desc);
   }

   void Rasterizer::initGL()
   {
      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);
//      int argc = 0;
//      char** argv;
//      glutInit(&argc, argv);
//      glutInitWindowSize(width, height); // 8x8 cube faces, not that this setting matters for the FBO
//      glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//      glutCreateWindow("Surfel Rasterizing");
//      glEnable(GL_DEPTH_TEST);
//      glEnable(GL_NORMALIZE);
//      glClearColor(0.0, 0.0, 0.0, 1.0);
//      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//      glEnable(GL_COLOR_MATERIAL);
//      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//      
//      //Error check
//      GLenum err = glewInit();
//      if (err != GLEW_OK || !glewIsSupported("GL_VERSION_2_0")) {
//         printf("OpenGL 2.0 not supported. No shaders!\n");
//         printf("%s\n", glewGetErrorString(err));
//         printf("%s\n", (char*)glGetString( GL_VERSION ) );
//         exit(-1);
//      }
//      printf("OpenGL 2.0 supported. Using OpenGL %s \n\n",(char*)glGetString( GL_VERSION ));
   }

   void Rasterizer::loadVBO(const SceneDescription& desc) {
      const std::vector<GeomObjectPtr>& tris = desc.surfels();
      //Allocate space on the stack since we won't need it later
      GLfloat vertices [3*3*tris.size()];
      GLfloat normals  [3*3*tris.size()];
      GLfloat colors   [3*3*tris.size()];

      int i=0;
      for(std::vector<GeomObjectPtr>::const_iterator it = tris.begin(); it != tris.end(); ++it) {
         //Get all the data!
         Triangle* tri = static_cast<Triangle*>(&(**it));
         glm::vec3 v0 = tri->getVertex0();
         glm::vec3 v1 = tri->getVertex1();
         glm::vec3 v2 = tri->getVertex2();
         glm::vec3 vCp1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
         glm::vec3 vCp2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
         glm::vec3 normal = glm::cross(vCp1, vCp2);
         normal = glm::normalize(normal);
         glm::vec4 color = tri->getColor();
         //Store all the data!
         //Vertex 0
         vertices[i] = v0.x;
         normals[i]  = normal.x;
         colors[i]   = color.r;
         ++i;
         vertices[i] = v0.y;
         normals[i]  = normal.y;
         colors[i]   = color.g;
         ++i;
         vertices[i] = v0.z;
         normals[i]  = normal.z;
         colors[i]   = color.b;
         ++i;
         //Vertex 1
         vertices[i] = v1.x;
         normals[i]  = normal.x;
         colors[i]   = color.r;
         ++i;
         vertices[i] = v1.y;
         normals[i]  = normal.y;
         colors[i]   = color.g;
         ++i;
         vertices[i] = v1.z;
         normals[i]  = normal.z;
         colors[i]   = color.b;
         ++i;
         //Vertex 2
         vertices[i] = v2.x;
         normals[i]  = normal.x;
         colors[i]   = color.r;
         ++i;
         vertices[i] = v2.y;
         normals[i]  = normal.y;
         colors[i]   = color.g;
         ++i;
         vertices[i] = v2.z;
         normals[i]  = normal.z;
         colors[i]   = color.b;
         ++i;
      }

      int numItems = 3*3*tris.size();
      size_t bytesPerList = sizeof(GLfloat)*numItems;

      //Generate VBO and load it full of data
      glGenBuffers(1, &vboId);
      glBindBuffer(GL_ARRAY_BUFFER, vboId);
      glBufferData(GL_ARRAY_BUFFER, 3*bytesPerList, 0, GL_STATIC_DRAW);
      glBufferSubData(GL_ARRAY_BUFFER, 0,              bytesPerList, vertices);
      glBufferSubData(GL_ARRAY_BUFFER, bytesPerList,   bytesPerList, normals);
      glBufferSubData(GL_ARRAY_BUFFER, 2*bytesPerList, bytesPerList, colors);
   }

   void Rasterizer::geometry(const SceneDescription& desc) {
      /* DRAW GEOMETRY */
      glBindBuffer(GL_ARRAY_BUFFER, vboId);

      // enable vertex arrays
      glEnableClientState(GL_NORMAL_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);
      glEnableClientState(GL_VERTEX_ARRAY);

      // before draw, specify vertex and index arrays with their offsets
      int numVertices = desc.surfels().size() * 3; //3 vertices per surfel
      int numItemsPerList = 3*numVertices; //3 GLfloats per vertex/color/normal
      int bytesPerList = sizeof(GLfloat) * numItemsPerList;
      glVertexPointer(3, GL_FLOAT, 0, 0);
      glNormalPointer(GL_FLOAT, 0, (void*)bytesPerList);
      glColorPointer(3, GL_FLOAT, 0, (void*)(2*bytesPerList));

      //Draw a number of vertices (e.g. all of them!)
      glDrawArrays(GL_TRIANGLES, 0, 3*desc.surfels().size());

      glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
      glDisableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);

      glBindBuffer(GL_ARRAY_BUFFER, 0);
   }

   void Rasterizer::view(::Camera& camera) {
      /* CAMERA */
      gluLookAt( camera.loc.x, camera.loc.y, camera.loc.z, // eye location
                 camera.loc.x+camera.dir.x, camera.loc.y+camera.dir.y, camera.loc.z+camera.dir.z, // Look at point
                 camera.up.x, camera.up.y, camera.up.z // up vector
               );
   }

   void Rasterizer::lights(const SceneDescription& desc) {
      /* LIGHTS */
//      glEnable(GL_LIGHTING);
      glDisable(GL_LIGHTING);
      int i=0;
      unsigned int curLight = GL_LIGHT0;
      for(std::vector<PointLightPtr>::const_iterator it = desc.lights().begin(); it != desc.lights().end(); ++it) {
         if (i++>7) break; //Only allow 8 lights
         glEnable(curLight++);
         glm::vec3 lpos = (*it)->getPos();
         glm::vec4 lcolor = (*it)->getColor();
         //Point light position
         GLfloat lightPos[] = {lpos.x, lpos.y, lpos.z, 0.0};
         GLfloat ambientComp[] = {lcolor.r, lcolor.g, lcolor.b, 0.0};
         //Diffuse composition
//         GLfloat diffuseComp[] = {lcolor.r, lcolor.g, lcolor.b, 0.0};
         GLfloat diffuseComp[] = {0.f, 0.f, 0.f, 0.f};
         //Specular composition
         GLfloat specularComp[] = {0.0, 0.0, 0.0, 0.0};
         //Point light
         glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
         glLightfv(GL_LIGHT0, GL_AMBIENT, ambientComp);
         glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseComp);
         glLightfv(GL_LIGHT0, GL_SPECULAR, specularComp);
      }
   }

   GLuint Rasterizer::rasterSurfels(::Camera& camera)
   {
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(camera.fov,
                     float(width)/height,
                     camera.zNear,
                     camera.zFar);
      glMatrixMode(GL_MODELVIEW);
      glViewport(0, 0, width, height);

      //draw
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      view(camera);
      lights(desc);
      geometry(desc);
      glFlush();

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
      
      return colortexture;
   }

   glm::vec3 IndirectIllumMonteCarlo(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      glm::vec3 indirectColor(0.f);
      if (recursionsLeft > 0) {
         int sqsamps = (int)sqrtf((float)scene.opts().monteCarloSamples);
         //Construct basis for uniform sample -> intersection point transform
         glm::vec3 k = glm::vec3(0,1,0);
         glm::vec3 tan = fabs(intrs.n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, intrs.n))*intrs.n)); //Gram-Schmidt Process
         glm::vec3 bin = glm::cross(intrs.n, tan);
         glm::mat3 xform = glm::mat3(tan, intrs.n, bin);
         //For each sample
         for (int i=0; i<sqsamps; ++i) {
            for (int j=0; j<sqsamps; ++j) {
               float u1 = (float)i/sqsamps + unifRand()/sqsamps;
               float u2 = (float)j/sqsamps + unifRand()/sqsamps;
               //glm::vec3 unitSamp = Samplers::CosineHemisphereSample(u1,u2);
               //glm::vec3 unitSamp = Samplers::BadCosineHemisphereSample(u1,u2);
               glm::vec3 unitSamp = Samplers::UniformHemisphereSample(u1,u2);
               glm::vec3 sampleDir = glm::normalize(xform * unitSamp);
               Ray ray = Ray(sampleDir, intrs.p+RAY_EPSILON*sampleDir);
               Intersection* pIsect = Tracer::RaySceneIntersect(ray, scene);
               if (pIsect->hit) {
                  indirectColor += Tracer::ShadePoint(*pIsect, scene, recursionsLeft-1) * glm::vec3(intrs.surf.color) * glm::dot(intrs.n, ray.d);
               }
               delete pIsect;
            }
         }
         indirectColor /= (float)scene.opts().monteCarloSamples;
      }
      return indirectColor;
   }

   glm::vec3 IndirectIllumSurfelRaster(const Intersection& intrs, const SceneDescription& scene)
   {
      glm::vec3 indirectColor(0.f);
      //Solve for tangent
      glm::vec3 k = glm::vec3(0,1,0);
      glm::vec3 tan = fabs(intrs.n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, intrs.n))*intrs.n)); //Gram-Schmidt Process
      //Generate 6 cube faces (i.e. 5 cameras)
      ::Camera cameras [5];
      float near = 0.01f, far = 15.f;
      glm::vec3 loc = intrs.p + (near*intrs.n);
//      glm::vec3 loc = intrs.p;
      glm::vec3 dir1 = tan;
      glm::vec3 dir2 = glm::rotate(tan,  90.f, intrs.n);
      glm::vec3 dir3 = glm::rotate(tan, 180.f, intrs.n);
      glm::vec3 dir4 = glm::rotate(tan, 270.f, intrs.n);
      glm::vec3 dir5 = intrs.n;
      for (int i=0; i<4; ++i) {
         glm::vec3 dir;
         if      (i==0)   dir = dir1;
         else if (i==1)   dir = dir2;
         else if (i==2)   dir = dir3;
         else  /*(i==3)*/ dir = dir4;
         cameras[i] = ::Camera(90, //fov
                               loc.x, loc.y, loc.z, //loc
                               dir.x, dir.y, dir.z, //dir
                               intrs.n.x, intrs.n.y, intrs.n.z, //up
                               near, far); //clipping planes
      } 
      cameras[4] = ::Camera(90, //fov
                            loc.x, loc.y, loc.z, //loc
                            dir5.x, dir5.y, dir5.z, //dir
                            tan.x, tan.y, tan.z, //up
                            near, far); //clipping planes
      //Rasterize cube faces (i.e. render 8x8 texture per camera)
      static Rasterizer rstr(8, 8, scene); //TODO: static? or should I redesign?
      unsigned char pixelData[4*64];
//      printf("\nPoint!\n");
      for (int i=0; i<5; ++i) {
         GLuint tex = rstr.rasterSurfels(cameras[i]);
         glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixelData);
         float weight = 0.f;
         for (int j=0; j<64; ++j) {
               int x = j%8, y = j/8;
               glm::vec3 U(cameras[i].up.x, cameras[i].up.y, cameras[i].up.z);
               glm::vec3 D(cameras[i].dir.x, cameras[i].dir.y, cameras[i].dir.z);
               glm::vec3 R = -glm::cross(U, D);
               glm::vec3 M = loc + (near*D);
               glm::vec3 pt = M + (-0.875f*near + 0.25f*(float)x*near)*R + (-0.875f*near + 0.25f*(float)y*near)*U;
               glm::vec3 v = glm::normalize(pt - intrs.p); 
               weight = glm::dot(intrs.n, v);
//               if (j%8 == 0) printf("\n");
//               printf("<%f %f %f, %f> ", pixelData[4*j]/255.f, pixelData[4*j+1]/255.f, pixelData[4*j+2]/255.f, weight);
//               printf("%f ", weight);
            indirectColor.r += pixelData[4*j+0]/255.f; //red
            indirectColor.g += pixelData[4*j+1]/255.f; //green
            indirectColor.b += pixelData[4*j+2]/255.f; //blue
            //indirectColor += pixelData[4*j+3]; //alpha
            indirectColor *= weight;
         }
//         printf("\n");
      }
//      printf("Point done!\n");
      indirectColor /= 64.f * 5.f;
//      printf("final color: %f %f %f\n", indirectColor.r, indirectColor.g, indirectColor.b);

      return 6.f*indirectColor;
   }
}
