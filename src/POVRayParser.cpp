/**
* POVRayParser.cpp
* --------------------
* Implementation for Parser for POV Ray files (.pov)
* Author: Ryan Schmitt
*/

#include "POVRayParser.h"
#include "RDSbvh.h"
#include "RDSPointDist.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>

namespace RDST
{
   SceneDescription
   POVRayParser::ParseFile(const std::string& fileToParse)
   {
      //Scene description vars
      CameraPtr pCam;
      boost::shared_ptr< std::vector<PointLightPtr> > lights(new std::vector<PointLightPtr>());
      boost::shared_ptr< std::vector<GeomObjectPtr> > areaLights(new std::vector<GeomObjectPtr>());
      boost::shared_ptr< std::vector<GeomObjectPtr> > objs(new std::vector<GeomObjectPtr>());
      boost::shared_ptr< std::vector<GeomObjectPtr> > planes(new std::vector<GeomObjectPtr>());

      std::string line;
      std::ifstream file(fileToParse.c_str());
      if (file.fail()) {
         std::cerr << "***Error: failed to open file: " << fileToParse << std::endl;
         exit(EXIT_FAILURE);
      }
      std::cout << "Parsing File...";
      clock_t prevTime = clock();
      while (std::getline(file, line)) {
         clock_t now = clock();
         if (float(now - prevTime) / CLOCKS_PER_SEC > 1.f) {
            std::cout << ".";
            prevTime = now;
         }
         RemoveComment(line);
         std::string::size_type pos = line.find("camera");
         if (pos != std::string::npos) {
            line = line.substr(pos);
            GetWholeObject(line, file);
            pCam = ParseCamera(line);
         }
         pos = line.find("light_source");
         if (pos != std::string::npos) {
            line = line.substr(pos);
            GetWholeObject(line, file);
            lights->push_back(ParseLight(line));
         }
         pos = line.find("box");
         if (pos != std::string::npos) {
            line = line.substr(pos);
            GetWholeObject(line, file);

            //TODO: go back to this.
//            objs->push_back(ParseBox(line));

            //Do the box sampling
            BoxPtr pBox = ParseBox(line);
            float minDist = 0.f;
            boost::shared_ptr< std::vector<glm::vec3> > pSampleVec = generateDistributedPoints(500, *pBox, &minDist);
            std::vector<glm::vec3>::const_iterator cit = pSampleVec->begin();
            for (; cit != pSampleVec->end(); ++cit) {
               objs->push_back(SpherePtr(new Sphere(*cit, minDist, pBox->getColor(), glm::mat4(1.f), Finish(0.2f, 0.8f))));
            }

         }
         pos = line.find("cone");
         if (pos != std::string::npos) {
            line = line.substr(pos);
            GetWholeObject(line, file);
            objs->push_back(ParseCone(line));
         }
         pos = line.find("plane");
         if (pos != std::string::npos) {
            line = line.substr(pos);
            GetWholeObject(line, file);
            planes->push_back(ParsePlane(line));
         }
         pos = line.find("sphere");
         if (pos != std::string::npos) {
            line = line.substr(pos);
            GetWholeObject(line, file);

            //TODO: go back to this
            //objs->push_back(ParseSphere(line));

            //SurfGen, using a slow method
            SpherePtr pSphere = ParseSphere(line);
            float minDist = 0.f;
            boost::shared_ptr< std::vector<glm::vec3> > pSampleVec = generateDistributedPoints(500, *pSphere, 100000, &minDist);
            std::vector<glm::vec3>::const_iterator cit = pSampleVec->begin();
            for (; cit != pSampleVec->end(); ++cit) {
               objs->push_back(SpherePtr(new Sphere(*cit, minDist, pSphere->getColor(), glm::mat4(1.f), Finish(0.2f, 0.8f))));
            }

         }
         pos = line.find("triangle");
         if (pos != std::string::npos) {
            line = line.substr(pos);
            GetWholeObject(line, file);

            //TODO: go back to this
            //objs->push_back(ParseTriangle(line));
            
            //Do the box sampling
            TrianglePtr pTri = ParseTriangle(line);
            float minDist = 0.f;
            boost::shared_ptr< std::vector<glm::vec3> > pSampleVec = generateDistributedPoints(500, *pTri, 10000, &minDist);
            std::vector<glm::vec3>::const_iterator cit = pSampleVec->begin();
            for (; cit != pSampleVec->end(); ++cit) {
               objs->push_back(SpherePtr(new Sphere(*cit, minDist, pTri->getColor(), glm::mat4(1.f), Finish(0.2f, 0.8f))));
            }


         }
         if (objs->size() > 0 && objs->back()->getFinish().getEmissive() > 0.f) {
            areaLights->push_back(objs->back());
         }
      }
      std::cout << "Done." << std::endl;
      return SceneDescription(pCam, lights, areaLights, objs, planes, BVH(objs));
   }

   std::string&
   POVRayParser::RemoveComment(std::string& line)
   {
      std::string::size_type pos = line.find("//");
      line = line.substr(0, pos);
      return line;
   }

   std::string&
   POVRayParser::GetWholeObject(std::string& inputText, std::ifstream& file)
   {
      //check how many brackets we're dealing with from the input
      int nonMatchedBrackets = 0;
      std::string::const_iterator it(inputText.begin());
      for (; it != inputText.end(); ++it) {
         if (*it == '{') ++nonMatchedBrackets;
         else if (*it == '}') --nonMatchedBrackets;
      }
      BOOST_ASSERT(nonMatchedBrackets >= 0);
      //grab file text and store it in inputText until we get fully matched brackets
      std::string line;
      inputText += " "; //just to make sure we've got plenty white space
      while (nonMatchedBrackets > 0 && std::getline(file, line)) {
         RemoveComment(line);
         line += " "; //add white space where the \n was.
         it = line.begin();
         for (; it != line.end() && nonMatchedBrackets > 0; ++it) {
            inputText += *it;
            if (*it == '{') ++nonMatchedBrackets;
            else if (*it == '}') --nonMatchedBrackets;
            BOOST_ASSERT(nonMatchedBrackets >= 0);
         }
      }
      return inputText;
   }

   glm::vec3
   POVRayParser::ParseVec3FromStream(std::istringstream& tokens)
   {
      std::string token;

      std::getline(tokens, token, ',');
      float x = ParseFloat(token);

      std::getline(tokens, token, ',');
      float y = ParseFloat(token);

      std::getline(tokens, token, '>');
      float z = ParseFloat(token);

      return glm::vec3(x, y, z);
   }

   glm::vec4
   POVRayParser::ParseVec4FromStream(std::istringstream& tokens)
   {
      std::string token;

      std::getline(tokens, token, ',');
      float x = ParseFloat(token);

      std::getline(tokens, token, ',');
      float y = ParseFloat(token);

      std::getline(tokens, token, ',');
      float z = ParseFloat(token);

      std::getline(tokens, token, '>');
      float w = ParseFloat(token);

      return glm::vec4(x, y, z, w);
   }

   glm::vec3
   POVRayParser::ParseScale(std::istringstream& tokens)
   {
      glm::vec3 scale(1.f); //default no scaling
      while (char c = tokens.peek()) {
         if (c == '<') {
            scale = ParseVec3FromStream(tokens);
            break;
         }
         else if (std::isdigit(c)) {
            std::string token;
            tokens >> token;
            scale = glm::vec3(boost::lexical_cast<float>(token));
            break;
         }
      }
      return scale;
   }

   float
   POVRayParser::ParseFloat(std::string& token)
   {
      std::string startChars("-.0123456789");
      std::string endChars("0123456789");
      std::string::size_type startPos(token.find_first_of(startChars));
      std::string::size_type stopPos(token.find_last_of(endChars)+1);
      token = token.substr(startPos, stopPos - startPos);
      return boost::lexical_cast<float>(token);
   }

   CameraPtr
   POVRayParser::ParseCamera(const std::string& inputText)
   {
      glm::vec3 posVec, upVec, rightVec, lookAtVec;
      std::string token;
      std::istringstream tokens(inputText);
      while (std::getline(tokens, token, ' ')) {
         if (token.find("location") != std::string::npos)
            posVec = ParseVec3FromStream(tokens);
         else if (token.find("up") != std::string::npos)
            upVec = ParseVec3FromStream(tokens);
         else if (token.find("right") != std::string::npos)
            rightVec = ParseVec3FromStream(tokens);
         else if (token.find("look_at") != std::string::npos)
            lookAtVec = ParseVec3FromStream(tokens);
      }
      return CameraPtr(new Camera(posVec, upVec, rightVec, lookAtVec-posVec));
   }

   PointLightPtr
   POVRayParser::ParseLight(const std::string& inputText)
   {
      glm::vec3 posVec;
      glm::vec4 color;
      std::string token;
      std::istringstream tokens(inputText);
      while (std::getline(tokens, token, ' ')) {
         if (token.find("light_source") != std::string::npos) //since lights have no "location" identifier, we just have to assume it comes first... lame
            posVec = ParseVec3FromStream(tokens);
         else if (token.find("rgb") != std::string::npos)
            color = glm::vec4(ParseVec3FromStream(tokens), 1.f);
         else if (token.find("rgbf") != std::string::npos)
            color = ParseVec4FromStream(tokens);
      }
      return PointLightPtr(new PointLight(posVec, color));
   }

   Finish
   POVRayParser::ParseFinish(std::istringstream& tokens)
   {
      float ambient, diffuse, emissive, specular, roughness, reflection, refraction, ior;
      ambient = diffuse = emissive = specular = roughness = reflection = refraction = ior = 0.f;
      std::string token;
      bool running(true);
      while (running && std::getline(tokens, token, ' ')) {
         if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
         //Parse different types of Finish floats
         if (token.find("ambient") != std::string::npos) {
            tokens >> token;
            if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
            ambient = ParseFloat(token);
         }
         else if (token.find("diffuse") != std::string::npos) {
            tokens >> token;
            if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
            diffuse = ParseFloat(token);
         }
         else if (token.find("emissive") != std::string::npos) {
            tokens >> token;
            if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
            emissive = ParseFloat(token);
         }
         else if (token.find("specular") != std::string::npos) {
            tokens >> token;
            if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
            specular = ParseFloat(token);
         }
         else if (token.find("roughness") != std::string::npos) {
            tokens >> token;
            if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
            roughness = ParseFloat(token);
         }
         else if (token.find("reflection") != std::string::npos) {
            tokens >> token;
            if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
            reflection = ParseFloat(token);
         }
         else if (token.find("refraction") != std::string::npos) {
            tokens >> token;
            if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
            refraction = ParseFloat(token);
         }
         else if (token.find("ior") != std::string::npos) {
            tokens >> token;
            if (token.find("}") != std::string::npos) //Exit if we reach the closing '}'
            running = false;
            ior = ParseFloat(token);
         }
      }
      return Finish(ambient, diffuse, emissive, specular, roughness, reflection, refraction, ior);
   }

   void
   POVRayParser::ParseGeomObject(std::istringstream& tokens, glm::vec4& color, glm::mat4& xforms, Finish& finish)
   {
      /*boost::char_separator<char> sep(" \n");
      boost::tokenizer<boost::char_separator<char>> tok(inputText, sep);
      for (boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin(); beg!=tok.end();++beg) {
      std::cout << *beg << "\n";
      }*/
      std::string token;
      while (std::getline(tokens, token, ' ')) {
         if (token.find("finish") != std::string::npos)
            finish = ParseFinish(tokens);
         else if (token.find("translate") != std::string::npos)
            xforms = glm::translate(glm::mat4(1.f), ParseVec3FromStream(tokens)) * xforms;
         else if (token.find("rotate") != std::string::npos) {
            glm::vec3 axisRotsVec(ParseVec3FromStream(tokens));
            xforms = glm::rotate(glm::mat4(1.f), axisRotsVec.x, glm::vec3(1.f, 0.f, 0.f)) * xforms; //rotation about x
            xforms = glm::rotate(glm::mat4(1.f), axisRotsVec.y, glm::vec3(0.f, 1.f, 0.f)) * xforms; //rotation about y
            xforms = glm::rotate(glm::mat4(1.f), axisRotsVec.z, glm::vec3(0.f, 0.f, 1.f)) * xforms; //rotation about z
         }
         else if (token.find("scale") != std::string::npos)
            xforms = glm::scale(glm::mat4(), ParseScale(tokens)) * xforms;
         else if (token.find("rgbf") != std::string::npos)
            color = ParseVec4FromStream(tokens);
         else if (token.find("rgb") != std::string::npos)
            color = glm::vec4(ParseVec3FromStream(tokens), 1.f);
      }
   }

   BoxPtr
   POVRayParser::ParseBox(const std::string& inputText)
   {
      //Box vars
      glm::vec3 corner1, corner2;
      Finish finish;
      glm::mat4 xforms(1.f); //identity
      glm::vec4 color;

      //Parsing vars
      std::string token;
      std::istringstream tokens(inputText);

      //Type check
      std::getline(tokens, token, ' ');
      BOOST_ASSERT(token.find("box") != std::string::npos);

      //Parse both corners, and other object params
      corner1 = ParseVec3FromStream(tokens);
      std::getline(tokens, token, ','); //eat the comma between the locations
      corner2 = ParseVec3FromStream(tokens);
      ParseGeomObject(tokens, color, xforms, finish);

      return BoxPtr(new Box(corner1, corner2, color, xforms, finish));
   }

   ConePtr
   POVRayParser::ParseCone(const std::string& inputText)
   {
      //Cone vars
      glm::vec3 end1, end2;
      float radius1, radius2;
      Finish finish;
      glm::mat4 xforms(1.f); //identity
      glm::vec4 color;

      //Parsing vars
      std::string token;
      std::istringstream tokens(inputText);

      //Type check
      std::getline(tokens, token, ' ');
      BOOST_ASSERT(token.find("cone") != std::string::npos); //type check

      //Parse end1, radius1, end2, radius2, and other object params
      end1 = ParseVec3FromStream(tokens);
      std::getline(tokens, token, ','); //eat the comma between
      tokens >> token;
      radius1 = ParseFloat(token);
      end2 = ParseVec3FromStream(tokens);
      std::getline(tokens, token, ','); //eat the comma between
      tokens >> token;
      radius2 = ParseFloat(token);
      ParseGeomObject(tokens, color, xforms, finish);

      return ConePtr(new Cone(end1, radius1, end2, radius2, color, xforms, finish));
   }

   PlanePtr
   POVRayParser::ParsePlane(const std::string& inputText)
   {
      //Plane vars
      glm::vec3 normal;
      float distance;
      Finish finish;
      glm::mat4 xforms(1.f); //identity
      glm::vec4 color;

      //Token vars
      std::string token;
      std::istringstream tokens(inputText);

      //Type check
      std::getline(tokens, token, ' ');
      BOOST_ASSERT(token.find("plane") != std::string::npos);

      //Parse normal, distance, and other object params
      normal = glm::normalize(ParseVec3FromStream(tokens));
      std::getline(tokens, token, ','); //eat the comma between
      tokens >> token;
      distance = ParseFloat(token);
      ParseGeomObject(tokens, color, xforms, finish);

      return PlanePtr(new Plane(normal, distance, color, xforms, finish));
   }

   SpherePtr
   POVRayParser::ParseSphere(const std::string& inputText)
   {
      //Sphere vars
      glm::vec3 center;
      float radius;
      Finish finish;
      glm::mat4 xforms(1.f); //identity
      glm::vec4 color;

      //Token vars
      std::string token;
      std::istringstream tokens(inputText);

      //Type check
      std::getline(tokens, token, ' ');
      BOOST_ASSERT(token.find("sphere") != std::string::npos);

      //Parse normal, distance, and other object params
      center = ParseVec3FromStream(tokens);
      std::getline(tokens, token, ','); //eat the comma between
      tokens >> token;
      radius = ParseFloat(token);
      ParseGeomObject(tokens, color, xforms, finish);

      return SpherePtr(new Sphere(center, radius, color, xforms, finish));
   }

   TrianglePtr
   POVRayParser::ParseTriangle(const std::string& inputText)
   {
      //Triangle vars
      glm::vec3 vert1, vert2, vert3;
      Finish finish;
      glm::mat4 xforms(1.f); //identity
      glm::vec4 color;

      //Token vars
      std::string token;
      std::istringstream tokens(inputText);

      //Type check
      std::getline(tokens, token, ' ');
      BOOST_ASSERT(token.find("triangle") != std::string::npos);

      //Parse normal, distance, and other object params
      vert1 = ParseVec3FromStream(tokens);
      std::getline(tokens, token, ','); //eat the comma between
      vert2 = ParseVec3FromStream(tokens);
      std::getline(tokens, token, ','); //eat the comma between
      vert3 = ParseVec3FromStream(tokens);
      ParseGeomObject(tokens, color, xforms, finish);

      return TrianglePtr(new Triangle(vert1, vert2, vert3, color, xforms, finish));
   }

}
