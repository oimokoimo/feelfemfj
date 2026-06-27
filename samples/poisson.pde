mesh {

  mesher bamg;
  
  point a(0,0),b(1,0),c(1,1),d(0,1);


  edge dedge(a,b,c);
  edge nedge(c,d,a);

  region reg1[tri](a,b,c,d);

  domain dom1(reg1);

  vertices 800;

}


vars {

  fem u[P1];
  fem exu[P1];
  ewise e[tri4];
  ewise du[P1];
  double k,max,min;
}

scheme {

  k = 10.0;

  exu = sin(x)*cos(y);


 solve(u;tu) {
  solver PETSc(CGS);
  quadrature tri4;

  wea: integral[tri4]( k * grad(u)*grad(tu)) + bintegral[3]( g * tu) = 0;
 
  dbc: u = exu, on dedge;

  nbc: g = (u-10.0)*k, on nedge;

  }


  contour[u](mesh=off);

}

