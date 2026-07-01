mesh {
 point a(0,0),p(0,1);
 region a[rect](a,b,c,d :[a,b,c],[k,l,m]);
 region b[tri] (a,b,c);
 domain dom(a,b);
 vertices (1000);
}
var {
 fem u[P1],v[P1];
 double a = 0, b;
 int a,b,c;
 ewise a[gaux];
}
scheme {

 solve [u; tu] {
    solver bicgstab;
    quadrature tri4;

    weq: integral(dx(u)*dx(tu)+dy(u)*dy(tu))+integral[tri2](u*tu)-bintegral(g*tu) = 0;
    
 }
 dt = 0;
 dt = -10;
}
