function plane=getPlane3d(points)
    
    x1=points(1,1);x2=points(2,1);x3=points(3,1);
    y1=points(1,2);y2=points(2,2);y3=points(3,2);
    z1=points(1,3);z2=points(2,3);z3=points(3,3);
    
    a=(y2-y1)*(z3-z1)-(y3-y1)*(z2-z1);
    b=(z2-z1)*(x3-x1)-(z3-z1)*(x2-x1);
    c=(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
    
    d=-a*x1-b*y1-c*z1;
    
    plane=[a b c d];

end