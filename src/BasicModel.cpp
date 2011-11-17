#include "BasicModel.h"

BasicModel::BasicModel(string filename)
{
   //initialize values
   max_x = max_y = max_z = FLT_MIN;
   min_x = min_y = min_z = FLT_MAX;
   center.x = 0.0;
   center.y = 0.0;
   center.z = 0.0;

   //read mesh data
   parseFile(filename);
}

BasicModel::~BasicModel()
{
   // Delete vector<Vec3f *> Vertices
   for(vector<Vec3f *>::iterator i = Vertices.begin();  i != Vertices.end(); i++) {
      Vec3f* temp = *i;
      delete temp;
   }

   // Delete vector<Vec3f *> VerticesNormals
   for(vector<Vec3f *>::iterator i = VerticesNormals.begin(); i != VerticesNormals.end(); i++) {
      Vec3f* temp = *i;
      delete temp;
   }

   // Delete vector<Tri *> Triangles
   for(vector<Tri *>::iterator i = Triangles.begin(); i != Triangles.end(); i++) {
      Tri* temp = *i;
      delete temp;
   }
}

//open the file for reading and parse the mesh
void BasicModel::parseFile(string filename)
{
   // Open the filestream
   ifstream infile(filename.c_str());
   if(infile.is_open()) {

      while(!infile.eof()) {
         string line;
         Vec3f *v = 0;

         //Read the line
         getline(infile,line);

         // Ignore commented lines
         if(line.find("#") == 0)
            continue;

         // Found a vertex
         if(line.find("Vertex") != string::npos) {
            // Parse the coordinate Vec3f and add the the Vertices
            // NOTE: Assumes all vertices appear in index order
            v = parseVertex(line);
            Vertices.push_back(v);
            VerticesNormals.push_back(new Vec3f(0.0,0.0,0.0));

            // Track/Find the model's extents
            // X
            if (v->x > max_x) max_x = v->x; 
            if (v->x < min_x) min_x = v->x;
            // Y
            if (v->y > max_y) max_y = v->y; 
            if (v->y < min_y) min_y = v->y;
            // Z
            if (v->z > max_z) max_z = v->z; 
            if (v->z < min_z) min_z = v->z;
         }
         // found a Face
         else if(line.find("Face") != string::npos) {
            Triangles.push_back(parseTri(line));
         }
      }
      // End of File
      infile.close();

      // average coords for center of mass
	   center.x = ((max_x - min_x) / 2.f) + min_x;
      center.y = ((max_y - min_y) / 2.f) + min_y;
      center.z = ((max_z - min_z) / 2.f) + min_z;

      // Calculate the max extent
      // FIXME: should this also use the max/min Z?
      max_extent = (max_y-min_y) > (max_x-min_x) ? max_y-min_y : max_x-min_x;

      // Normalize the normals
      for(vector<Vec3f *>::iterator i = VerticesNormals.begin(); i != VerticesNormals.end(); i++) {
         (*i)->normalize();
      }
   } 
   else {
      cout << "Error opening file: " << filename << endl;
      return;
   }
}

// Parses the Tri from the line in the following format
//    "Face <(ignored)> <v1> <v2> <v3> {rgb=(<r>,<g>,<b>)}"
// returns *new* Tri (you delete)
BasicModel::Tri* BasicModel::parseTri(string line)
{
   Tri* t = parseFace(line);

   //Calculate the normal for this triangle
   Vec3f* v1 = Vertices[t->v1];
   Vec3f* v2 = Vertices[t->v2];
   Vec3f* v3 = Vertices[t->v3];

   Vec3f vCp1(v2->x - v1->x,v2->y - v1->y,v2->z - v1->z);
   Vec3f vCp2(v3->x - v1->x,v3->y - v1->y,v3->z - v1->z);
   Vec3f normal = vCp1.cross(vCp2);
   normal.normalize();

   t->normal = normal;

   v1 = VerticesNormals[t->v1];
   v2 = VerticesNormals[t->v2];
   v3 = VerticesNormals[t->v3];

   v1->x += normal.x;
   v1->y += normal.y;
   v1->z += normal.z;

   v2->x += normal.x;
   v2->y += normal.y;
   v2->z += normal.z;

   v3->x += normal.x;
   v3->y += normal.y;
   v3->z += normal.z;

   return t;
}

// Draws the model
void BasicModel::draw() {
   glPushMatrix();
      // Prepare for render
      glScalef(1.0f/max_extent, 1.0f/max_extent, 1.0f/max_extent); //WHY!?
      glTranslatef(-center.x, -center.y, -center.z); //Set center of mesh to (0,0,0)
      // Render each triangle
      glBegin(GL_TRIANGLES);
         for(vector<Tri *>::iterator i = Triangles.begin(); i != Triangles.end(); i++) {
            Tri* t = (*i);
            // V1
            glNormal3f(VerticesNormals[t->v1]->x,
                       VerticesNormals[t->v1]->y,
                       VerticesNormals[t->v1]->z);
            glVertex3f(Vertices[t->v1]->x, 
                       Vertices[t->v1]->y,
                       Vertices[t->v1]->z);
            // V2
            glNormal3f(VerticesNormals[t->v2]->x,
                       VerticesNormals[t->v2]->y,
                       VerticesNormals[t->v2]->z);
            glVertex3f(Vertices[t->v2]->x, 
                       Vertices[t->v2]->y,
                       Vertices[t->v2]->z);
            // V3
            glNormal3f(VerticesNormals[t->v3]->x,
                       VerticesNormals[t->v3]->y,
                       VerticesNormals[t->v3]->z);
            glVertex3f(Vertices[t->v3]->x, 
                       Vertices[t->v3]->y,
                       Vertices[t->v3]->z);
         }
      glEnd();
   glPopMatrix();
}



