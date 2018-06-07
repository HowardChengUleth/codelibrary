// Delauney Triangulation -- O(n^2)
//  -- Triangulation of a set of points so that no point P is inside the circumcircle
//     of any triangle.
//  -- Maximizes the minimum angle of all angles of the triangles in the triangulation
//  -- 'triangles' is a vector of the indices of the vertices of triangles in the triangulation

// Include 3D convex hull code.
typedef tiii tuple<int,int,int>;
void delauney_triangulation(vector<Vector>& pts,vector<tiii>& triangles){
  triangles.clear();
  for(int i=0;i<pts.size();i++) pts[i].z = pts[i].x*pts[i].x + pts[i].y*pts[i].y;
  vector<hullFinder::hullFace> hull = hullFinder(pts).findHull();
  for(int i=0;i<hull.size();i++)
    if(hull[i].n.z < -EPS)
      triangles.push_back(make_tuple(hull[i].u,hull[i].v,hull[i].w));
}
