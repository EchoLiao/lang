#ifndef _3DOBJECT_H
#define _3DOBJECT_H

#include <assert.h>
#include <iostream>
#include <cmath>

// vertex in 3d-coordinate system
struct sPoint{
  float x, y, z;
};

// plane equation
// 平面方程为: ax + by + cz + d = 0
struct sPlaneEq{
	float a, b, c, d;
};

// structure describing an object's face
// * 3个整形索引指定了模型中三角形的三个顶点
// * 临近的3个三角形面片的索引
//     1) 用作标识面是否邻面, 0表示没有, 非0表示有;
//     2) 用作标识面的邻面的索引(保存的值为邻面索引加一,
//        在使用到时, 须减一).
// * 平面方程描述了三角形面片所在的平面(显然也可以知道平面的法线)
// * 最后一个变量指定这个三角形面片是否投出阴影
struct sPlane{
	unsigned int p[3];
	unsigned int neigh[3];
	sPlaneEq PlaneEq;
	bool visible;
};

// object structure
// 用下面的结构描述一个产生阴影的物体
struct glObject{
	GLuint nPlanes, nPoints;
	sPoint points[100];
	sPlane planes[200];
};

// load object
inline int ReadObject(const char *st, glObject *o){
  FILE *file;
  unsigned int i;

  file = fopen(st, "r");
  if (!file) return 0;

  //points
  // 读取顶点
  fscanf(file, "%d", &(o->nPoints));
  for (i=1;i<=o->nPoints;i++){
    // 注意: Data/*.txt 的面的索引是从1开始计数, 所以i初始化为1.
    // 顶点数据在该程序中都是通过 o->planes 进行索引的!
    fscanf(file, "%f", &(o->points[i].x));
    fscanf(file, "%f", &(o->points[i].y));
    fscanf(file, "%f", &(o->points[i].z));
  }

  //planes
  // 读取三角形面个数
  fscanf(file, "%d", &(o->nPlanes));
  for (i=0;i<o->nPlanes;i++){
    // 读取三角形面的顶点索引(三点确定一个面)
    fscanf(file, "%d", &(o->planes[i].p[0]));
    fscanf(file, "%d", &(o->planes[i].p[1]));
    fscanf(file, "%d", &(o->planes[i].p[2]));
  }
  return 1;
}

// connectivity procedure - based on Gamasutra's article
// hard to explain here
// 查找每个面的相邻的面
inline void SetConnectivity(glObject *o){
	unsigned int p1i, p2i, p1j, p2j;
	unsigned int P1i, P2i, P1j, P2j;
	unsigned int i,j,ki,kj;

    assert(o->nPlanes >= 2);

	for(i=0;i<o->nPlanes-1;i++) { // 对于模型中的每一个面A
		for(j=i+1;j<o->nPlanes;j++) { // 对于除了面A的其它的面B
			for(ki=0;ki<3;ki++) { // 对于面A的每一个顶点
				if(!o->planes[i].neigh[ki]) { // 如果面A的这一邻面还没被找到过
					for(kj=0;kj<3;kj++) { // 对于面B的每一个顶点
						p1i=ki;
						p2i=(ki+1)%3;
						p1j=kj;
						p2j=(kj+1)%3;

                        // 面A的边(p1i, p2i)
						p1i=o->planes[i].p[p1i];
						p2i=o->planes[i].p[p2i];
                        // 面B的边(p1j, p2j)
						p1j=o->planes[j].p[p1j];
						p2j=o->planes[j].p[p2j];

                        // 如果面A的边(p1i, p2i)和面B的边(p1j, p2j)为同一条边,
                        // 则下面公式的 P1i=P1j, 且P2i=P2j .
						P1i=((p1i+p2i)-abs(p1i-p2i))/2;
						P2i=((p1i+p2i)+abs(p1i-p2i))/2;
						P1j=((p1j+p2j)-abs(p1j-p2j))/2;
						P2j=((p1j+p2j)+abs(p1j-p2j))/2;

						if((P1i==P1j) && (P2i==P2j)){  //they are neighbours
                            // 面i的邻面为面j, 面j的邻面为面i. 在这里设置时把
                            // 索引加一的原因是为了对 neigh 进行两用:
                            //     1) 用作标识面是否邻面, 0表示没有, 非0表示有;
                            //     2) 用作标识面的邻面的索引(保存的值为邻面索引加一,
                            //        在使用到时, 须减一).
							o->planes[i].neigh[ki] = j+1;
							o->planes[j].neigh[kj] = i+1;
						}
					}
				}
            }
        }
    }
}

void m3dNormalizeVector(struct sPoint *u)
{
    float len = u->x * u->x + u->y * u->y + u->z * u->z;
    assert(len != 0.0);
    u->x /= len;
    u->y /= len;
    u->z /= len;
}


void m3dCrossProduct(struct sPoint *result, const struct sPoint *u,
        const struct sPoint *v)
{
    result->x =  u->y * v->z - v->y * u->z;
    result->y = -u->x * v->z + v->x * u->z;
    result->z =  u->x * v->y - v->x * u->y;
}

// function for computing a plane equation given 3 points
inline void CalcPlane(glObject o, sPlane *plane){
    // [(<<计算机图形学几何工具算法详解>> P492)]
    sPoint v1, v2, v3;
    // v1 = p1 - p0, v2 = p2 - p0
    v1.x = o.points[plane->p[1]].x - o.points[plane->p[0]].x;
    v1.y = o.points[plane->p[1]].y - o.points[plane->p[0]].y;
    v1.z = o.points[plane->p[1]].z - o.points[plane->p[0]].z;
    v2.x = o.points[plane->p[2]].x - o.points[plane->p[0]].x;
    v2.y = o.points[plane->p[2]].y - o.points[plane->p[0]].y;
    v2.z = o.points[plane->p[2]].z - o.points[plane->p[0]].z;

    // cross = v1 x v2
    m3dCrossProduct(&v3, &v1, &v2);
    m3dNormalizeVector(&v3);
    plane->PlaneEq.a = v3.x;
    plane->PlaneEq.b = v3.y;
    plane->PlaneEq.c = v3.z;
    // d = -(n * p0)
    plane->PlaneEq.d = -( v3.x * o.points[plane->p[0]].x
                        + v3.y * o.points[plane->p[0]].y
                        + v3.z * o.points[plane->p[0]].z );
}

// procedure for drawing the object - very simple
void DrawGLObject(glObject o){
	unsigned int i, j;

	glBegin(GL_TRIANGLES);
	for (i=0; i<o.nPlanes; i++){
		for (j=0; j<3; j++){
            glNormal3f(o.planes[i].PlaneEq.a,
                    o.planes[i].PlaneEq.b,
                    o.planes[i].PlaneEq.c);
			glVertex3f(o.points[o.planes[i].p[j]].x,
					o.points[o.planes[i].p[j]].y,
					o.points[o.planes[i].p[j]].z);
		}
	}
	glEnd();
}

void  CastShadow(glObject *o, float *lp){
	unsigned int	i, j, k, jj;
	unsigned int	p1, p2;
	sPoint			v1, v2;
	float			side;

	//set visual parameter
	for (i=0;i<o->nPlanes;i++){
		// chech to see if light is in front or behind the plane (face plane)
		side =	o->planes[i].PlaneEq.a*lp[0]+
				o->planes[i].PlaneEq.b*lp[1]+
				o->planes[i].PlaneEq.c*lp[2]+
				o->planes[i].PlaneEq.d*lp[3];
		if (side >0) o->planes[i].visible = 1;
				else o->planes[i].visible = 0;
	}

 	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_STENCIL_TEST);
	glColorMask(0, 0, 0, 0);
	glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

	// first pass, stencil operation decreases stencil value
	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	for (i=0; i<o->nPlanes;i++){
		if (o->planes[i].visible)
			for (j=0;j<3;j++){
				k = o->planes[i].neigh[j];
				if ((!k) || (!o->planes[k-1].visible)){
					// here we have an edge, we must draw a polygon
					p1 = o->planes[i].p[j];
					jj = (j+1)%3;
					p2 = o->planes[i].p[jj];

					//calculate the length of the vector
					v1.x = (o->points[p1].x - lp[0])*100;
					v1.y = (o->points[p1].y - lp[1])*100;
					v1.z = (o->points[p1].z - lp[2])*100;

					v2.x = (o->points[p2].x - lp[0])*100;
					v2.y = (o->points[p2].y - lp[1])*100;
					v2.z = (o->points[p2].z - lp[2])*100;
					
					//draw the polygon
					glBegin(GL_TRIANGLE_STRIP);
						glVertex3f(o->points[p1].x,
									o->points[p1].y,
									o->points[p1].z);
						glVertex3f(o->points[p1].x + v1.x,
									o->points[p1].y + v1.y,
									o->points[p1].z + v1.z);

						glVertex3f(o->points[p2].x,
									o->points[p2].y,
									o->points[p2].z);
						glVertex3f(o->points[p2].x + v2.x,
									o->points[p2].y + v2.y,
									o->points[p2].z + v2.z);
					glEnd();
				}
			}
	}

	// second pass, stencil operation increases stencil value
	glFrontFace(GL_CW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	for (i=0; i<o->nPlanes;i++){
		if (o->planes[i].visible)
			for (j=0;j<3;j++){
				k = o->planes[i].neigh[j];
				if ((!k) || (!o->planes[k-1].visible)){
					// here we have an edge, we must draw a polygon
					p1 = o->planes[i].p[j];
					jj = (j+1)%3;
					p2 = o->planes[i].p[jj];

					//calculate the length of the vector
					v1.x = (o->points[p1].x - lp[0])*100;
					v1.y = (o->points[p1].y - lp[1])*100;
					v1.z = (o->points[p1].z - lp[2])*100;

					v2.x = (o->points[p2].x - lp[0])*100;
					v2.y = (o->points[p2].y - lp[1])*100;
					v2.z = (o->points[p2].z - lp[2])*100;
					
					//draw the polygon
					glBegin(GL_TRIANGLE_STRIP);
						glVertex3f(o->points[p1].x,
									o->points[p1].y,
									o->points[p1].z);
						glVertex3f(o->points[p1].x + v1.x,
									o->points[p1].y + v1.y,
									o->points[p1].z + v1.z);

						glVertex3f(o->points[p2].x,
									o->points[p2].y,
									o->points[p2].z);
						glVertex3f(o->points[p2].x + v2.x,
									o->points[p2].y + v2.y,
									o->points[p2].z + v2.z);
					glEnd();
				}
			}
	}

	glFrontFace(GL_CCW);
	glColorMask(1, 1, 1, 1);

	//draw a shadowing rectangle covering the entire screen
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.1f, 0.1f,-0.10f);
		glVertex3f(-0.1f,-0.1f,-0.10f);
		glVertex3f( 0.1f, 0.1f,-0.10f);
		glVertex3f( 0.1f,-0.1f,-0.10f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glShadeModel(GL_SMOOTH);
}

#endif // _3DOBJECT_H_
