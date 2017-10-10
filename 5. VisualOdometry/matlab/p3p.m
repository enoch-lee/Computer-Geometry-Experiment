clc;clear;



points=[503.551479289941,222.849704142012;
    322.123076923077,524.230769230769;
    599.513609467456,542.223668639053;
    449.572781065089,365.293491124260];

img=imread('checkerboard.jpg');

% imshow(img);hold on
% points=ginput(4);
% for i=1:4
% plot(points(i,1),points(i,2),'ro','MarkerSize',10);
% end

world_points=[90,0,30;
    180,0,150;
    30,0,180;
    120,0,90];

%focal length and optical center in pixels
fx=1027.80;
fy=998.59;
cx=596.04;
cy=474.25;



%image coordinates to camera coordinates
points(:,1)=(points(:,1)-cx)/fx;
points(:,2)=(points(:,2)-cy)/fy;
points(:,3)=1;

N=sqrt(points(:,1).^2+points(:,2).^2+points(:,3).^2);
for i=1:3
points(i,:)=points(i,:)/N(i);
end

%prepare the P3P equation system
u=points(1,:);
v=points(2,:);
w=points(3,:);

cos_uv=u(1)*v(1)+u(2)*v(2)+u(3)*v(3);
cos_uw=u(1)*w(1)+u(2)*w(2)+u(3)*w(3);
cos_vw=v(1)*w(1)+v(2)*w(2)+v(3)*w(3);

% cos_uv=(u(1)*v(1)+u(2)*v(2)+1)/(sqrt(u(1)^2+u(2)^2+1)*sqrt(v(1)^2+v(2)^2+1));
% cos_uw=(u(1)*w(1)+u(2)*w(2)+1)/(sqrt(u(1)^2+u(2)^2+1)*sqrt(w(1)^2+w(2)^2+1));
% cos_vw=(v(1)*w(1)+v(2)*w(2)+1)/(sqrt(v(1)^2+v(2)^2+1)*sqrt(w(1)^2+w(2)^2+1));

A=world_points(1,:);
B=world_points(2,:);
C=world_points(3,:);

dAB=sqrt((A(1)-B(1))^2+(A(2)-B(2))^2+(A(3)-B(3))^2);
dBC=sqrt((B(1)-C(1))^2+(B(2)-C(2))^2+(B(3)-C(3))^2);
dAC=sqrt((A(1)-C(1))^2+(A(2)-C(2))^2+(A(3)-C(3))^2);

a=dBC^2/dAB^2;
b=dAC^2/dAB^2;

p=2*cos_vw;
q=2*cos_uw;
r=2*cos_uv;

syms x y ;

% a4  =  a^2 + b^2 - 2*a - 2*b + 2*(1 -r^2)*b*a + 1;
% a3  =  -2*q*a^2 - r*p*b^2 + 4*q*a+ (2*q+p*r)*b + (r^2*q - 2*q + r*p)*a*b - 2*q;
% a2  =  (2+q^2)*a^2 + (p^2 + r^2 - 2)*b^2 - (4 + 2*q^2)*a - (p*q*r + p^2)*b - (p*q*r + r^2)*a*b + q^2 + 2; 
% a1  =  -2*q*a^2 - r*p*b^2 + 4*q*a + (p*r + q*p^2 - 2*q)*b + (r*p+2*q)*a*b - 2*q; 
% a0  =  a^2 + b^2 - 2*a + (2 - p^2)*b - 2*a*b + 1;
% b1  =  b*((p^2 - p*q*r + r^2)*a + (p^2 -r^2)*b - p^2 + p*q*r - r^2)^2;
% b0  =  ((1 - a - b)*x^2 + (a - 1)*q*x - a + b + 1)*...
%  ((r^3*(a^2 + b^2 - 2*a - 2*b + (2 - r^2)*a*b + 1))*x^3 + ...
%  r^2*(p + p*a^2 - 2*r*q*a*b + 2*r*q*b - 2*r*q - 2*p*a - 2*p*b + p*r^2*b + 4*r*q*a + q*r^3*a*b - 2*r*q*a^2 + 2*p*a*b + p*b^2 - r^2*p*b^2)*x^2 +...
%  (r^5*(b^2 - a*b) - r^4*p*q*b + r^3*(q^2 - 4*a - 2*q^2*a + q^2*a^2 + 2*a^2 - 2*b^2 + 2) + r^2*(4*p*q*a - 2*p*q*a*b + 2*p*q*b - 2*p*q - 2*p*q*a^2) +...  
%  r*(p^2*b^2 - 2*p^2*b + 2*p^2*a*b - 2*p^2*a + p^2 + p^2*a^2))*x +... 
%  (2*p*r^2 - 2*r^3*q + p^3 - 2*p^2*q*r + p*q^2*r^2)*a^2 + (p^3 - 2*p*r^2)*b^2 + (4*q*r^3 - 4*p*r^2 - 2*p^3 + 4*p^2*q*r - 2*p*q^2*r^2)*a + ...
%  (-2*q*r^3 + p*r^4 + 2*p^2*q*r - 2*p^3)*b + (2*p^3 + 2*q*r^3 - 2*p^2*q*r)*a*b + ... 
%  p*q^2*r^2 - 2*p^2*q*r + 2*p*r^2 + p^3 - 2*r^3*q);
a2 = a * a; b2 = b * b; p2 = p * p; q2 = q * q; r2 = r * r;
pr = p * r; pqr = q * pr;
ab = a * b; a_2 = 2*a; a_4 = 4*a;
r3 = r2*r; pr2 = p*r2; r3q = r3 * q;
x2=x^2;
temp = (p2*(a-1+b) + r2*(a-1-b) + pqr - a*pqr);
b0 = b * temp * temp;
b1=((1-a-b)*x2 + (q*a-q)*x + 1 - a + b) *...
            (((r3*(a2 + ab*(2 - r2) - a_2 + b2 - 2*b + 1)) * x +...
            (r3q*(2*(b-a2) + a_4 + ab*(r2 - 2) - 2) + pr2*(1 + a2 + 2*(ab-a-b) + r2*(b - b2) + b2))) * x2 +...
            (r3*(q2*(1-2*a+a2) + r2*(b2-ab) - a_4 + 2*(a2 - b2) + 2) + r*p2*(b2 + 2*(ab - b - a) + 1 + a2) + pr2*q*(a_4 + 2*(b - ab - a2) - 2 - r2*b)) * x +...
            2*r3q*(a_2 - b - a2 + ab - 1) + pr2*(q2 - a_4 + 2*(a2 - b2) + r2*b + q2*(a2 - a_2) + 2) +...
            p2*(p*(2*(ab - a - b) + a2 + b2 + 1) + 2*q*r*(b + a_2 - a2 - ab - 1)));



A = -2 * b + b2 + a2 + 1 + ab*(2 - r2) - a_2;
B = q2 + b2*(r2 + p2 - 2) - b*(p2 + pqr) - ab*(r2 + pqr) + (a2 - a_2)*(2 + q2) + 2;
C = pr*(ab-b2+b) + q*((p2-2)*b + 2 * (ab - a2) + a_4 - 2);
D = 1 + 2*(b - a - ab) + b2 - b*p2 + a2;
E = q*(-2*(ab + a2 + 1 - b) + r2*ab + a_4) + pr*(b - b2 + ab);

%b0=vpa(b0);

eqns1=[(1-a)*y^2-a*x^2+a*x*y*r-y*p+1==0,(1-b)*x^2-b*y^2+b*x*y*r-x*q+1==0];
eqns=[A*x^4+B*x^3+C*x^2+D*x+E==0,b1*y-b0==0];

%solve the P3P equation system
vars=[x,y];
[solx,soly]=solve(eqns,vars);
solx=vpa(solx);

%finding 3D coordinates for points A, B, C

x1=solx(1);
y1=soly(1);
vv=double(x1^2+y1^2-2*x1*y1*cos_uv);

dPC=double(sqrt(dAB^2/vv));
dPA=double(dPC*x1);
dPB=double(dPC*y1);

Ac=u.*dPA;
Bc=v.*dPB;
Cc=w.*dPC;

camera_points=[Ac(1),Ac(2),Ac(3);
    Bc(1),Bc(2),Bc(3);
    Cc(1),Cc(2),Cc(3)];

%camera_points=double(camera_points);

world_points=[90,0,30;
    180,0,150;
    30,0,180;];


%B=R*A+t
    A=world_points;
    B=camera_points;
    centroid_A = mean(A);
    centroid_B = mean(B);
     
    N = size(A,1);
    H = (A - repmat(centroid_A, N, 1))' * (B - repmat(centroid_B, N, 1));
    [U,S,V] = svd(H);
    
    R = V*U'
    t = -R*centroid_A' + centroid_B'
    
    K=[1027.80, 0 , 596.04;
    0, 998.59, 474.25;
    0, 0, 1];
    Rt=[R,t];
    P=K*Rt;
    P=P/P(12);
    disp(P);
    
    
    
