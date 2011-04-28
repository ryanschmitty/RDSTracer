/**
 * POVRayParser.h
 * --------------------
 * Parser for POV Ray files (.pov)
 * Author: Ryan Schmitt
 */

#ifndef __POV_RAY_PARSER_H__
#define __POV_RAY_PARSER_H__

#define GLM_FORCE_INLINE

#include <string>
#include <vector>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include "RDSScene.h"

namespace RDST
{
   class POVRayParser
   {
   //Static Class to hide helper functions, should never be instantiated.
   private:
      explicit POVRayParser()
      {}
      explicit POVRayParser(const POVRayParser& rhs)
      {}
      ~POVRayParser()
      {}

   public:
      static SceneDescription ParseFile(const std::string& fileToParse);

   private:
      //Utils
      static std::string&  RemoveComment (std::string& line);
      static std::string&  GetWholeObject(std::string& inputText, std::ifstream& file);
      static glm::vec3     ParseVec3FromStream(std::istringstream& tokens);
      static glm::vec4     ParseVec4FromStream(std::istringstream& tokens);
      static glm::vec3     ParseScale(std::istringstream& tokens);
      static float         ParseFloat(std::string& token);
      //Main Object Parsing
      static CameraPtr     ParseCamera  (const std::string& inputText);
      static PointLightPtr ParseLight   (const std::string& inputText);
      static Finish        ParseFinish  (std::istringstream& tokens);
      static void          ParseGeomObject(std::istringstream& tokens, glm::vec4& color, glm::mat4& xforms, Finish& finish);
      static BoxPtr        ParseBox     (const std::string& inputText);
      static ConePtr       ParseCone    (const std::string& inputText);
      static PlanePtr      ParsePlane   (const std::string& inputText);
      static SpherePtr     ParseSphere  (const std::string& inputText);
      static TrianglePtr   ParseTriangle(const std::string& inputText);

   };
} // end namespace RDST

#endif