mesh {
 point a(0,0),p(0,1);
 point d(2,0);

 edge ap(a,b);
 edge apd(a,p,d),all(a,p,d,a);
 region a[rect](a,b,c,d :[a,b,c],[k,l,m]);
 region b[tri] (a,b,c);
 domain dom(a,b);
 vertices (1000);
}
var {
 fem u[P1],v[P1];
 double a = 1000, b;
 int a,b,c;
 ewise a[gaux];
}

element P1b[tri]
{
(0,0): 1-r-s;
(1,0): r;
(0,1): s;
(1/3,1/3): (1-r-s)*r*s;
}

element PK2[rect]
{
    double d1;
    double d2;
    double d3;
    double d4;

    double t1;
    double t2;
    double t3;
    double t4;
    double t5;
    double t6;
    double t7;
    double t8;

    d1 = (x2-x1)*(y4-y1)-(x4-x1)*(y2-y1);
    d2 = (x3-x2)*(y1-y2)-(x1-x2)*(y3-y2);
    d3 = (x4-x3)*(y2-y3)-(x2-x3)*(y4-y3);
    d4 = (x1-x4)*(y3-y4)-(x3-x4)*(y1-y4);

    t1 = (d1 - d3 )/(d1 + d3)/8.0;
    t2 = (d2 - d4 )/(d2 + d4)/8.0;
    t3 = (d3 - d1 )/(d3 + d1)/8.0;
    t4 = (d4 - d2 )/(d4 + d2)/8.0;

    t5 = (d4 - d1 )/(d1 + d3)/4.0;
    t6 = (d1 - d2 )/(d2 + d4)/4.0;
    t7 = (d2 - d3 )/(d3 + d1)/4.0;
    t8 = (d3 - d4 )/(d4 + d2)/4.0;

   (-1,-1):  (r- 1)*(1-s)*(1+s+r)*0.25 + t1 * (1-r*r)*(1-s*s);
   ( 1,-1):  -(1+r)*(1-s)*(1-r+s)*0.25 + t2 * (1-r*r)*(1-s*s);
   ( 1, 1):  -(1+r)*(1+s)*(1-s-r)*0.25 + t3 * (1-r*r)*(1-s*s);
   (-1, 1):  (r- 1)*(1+s)*(1+r-s)*0.25 + t4 * (1-r*r)*(1-s*s);

   ( 0,-1):  (1-r*r)*(1-s)*0.5         + t5 * (1-r*r)*(1-s*s);
   ( 1, 0):  (1+r)*(1-s * s)*0.5       + t6 * (1-r*r)*(1-s*s);
   ( 0, 1):  (1- r*r)*(1+s)*0.5        + t7 * (1-r*r)*(1-s*s);
   (-1, 0):  (1-r)*(1- s*s)*0.5        + t8 * (1-r*r)*(1-s*s);
}

quadrature tet2[tetra] {   /* quadratic order */
  double a,b;

  a = (5.0-sqrt(5.0))/20.0;
  b = (5.0+3.0*sqrt(5.0))/20.0;

  (a,a,a) : 1.0 / 24.0;
  (a,a,b) : 1.0 / 24.0;
  (a,b,a) : 1.0 / 24.0;
  (b,a,a) : 1.0 / 24.0;
}


quadrature tet3[tetra] {   /* Cubic order */
  double p2,p4,p6;
  double w,wm;

  p2 =  1.0 / 2.0;
  p4 =  1.0 / 4.0;
  p6 =  1.0 / 6.0;

  w  =  9.0 / 20.0 / 6.0;
  wm = -4.0 /  5.0 / 6.0;

  (p4,p4,p4) : wm;
  (p2,p6,p6) : w;
  (p6,p2,p6) : w;
  (p6,p6,p2) : w;
  (p6,p6,p6) : w;
}

quadrature gauss2x2[rect] {
    double p =  0.577350269189626;
    double pm= -0.577350269189626;
   ( p, p): 1;
   ( p,pm): 1;
   (pm, p): 1;
   (pm,pm): 1;
}

scheme {

 ProgramModel feelfem2018;

 solve [u,v; tu,tv] {
    solver bicgstab;
    quadrature tri4;

    weq: integral(dx(u)*dx(tu)+dy(u)*dy(tu))+integral[tri2](u*tu)-bintegral(g*tu) = 0;
    weq: integral(dx(v)*dx(tv)+dy(v)*dy(tv))+integral[tri2](v*tv)-bintegral(h*tv) = 0;

    dbc : u = 100, on ab;
    nbc : g = dk(u-100), on a,b;
 }
 dt = 0;
 dt = -10;
}
