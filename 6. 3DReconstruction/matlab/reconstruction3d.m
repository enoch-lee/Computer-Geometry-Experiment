clear;clc;
points_1=[278, 177;
 276, 300;
 275, 429;
 275, 553;
 464, 155;
 461, 308;
 454, 467;
 446, 612;
 220, 533;
 242, 540;
 509, 633;
 549, 648;
 275, 579;
 300, 589;
 328, 598;
 356, 609;
 388, 620;
 422, 633;
 217, 595;
 243, 607;
 271, 618;
 298, 631;
 329, 644;
 362, 658];

points_2=[220, 31;
 225, 180;
 232, 341;
 238, 488;
 420, 51;
 421, 175;
 423, 308;
 423, 431;
 127, 520;
 172, 509;
 460, 420;
 484, 413;
 263, 511;
 299, 498;
 333, 489;
 362, 476;
 394, 467;
 420, 457;
 317, 538;
 353, 523;
 388, 510;
 420, 498;
 449, 487;
 474, 476];

P2=[-5.077103883103874e-05, -0.0002282514290614632, 0.05085131503322079, 0.9987059284817111;
 0.0009970468412888963, 0.004482679335705257, -0.9987064726378619, 0.05085266648109309;
 4.132033255630632e-06, -2.058218700987936e-07, 0.001999073631872648, 0.0006890045325940215];

P1=[1, 0, 0, 0;
 0, 1, 0, 0;
 0, 0, 1, 0];

%points_1(:,3)=1;

for i=1:size(points_1,1)
    A=[ points_1(i,1)*P1(3,:)-P1(1,:);
        points_1(i,2)*P1(3,:)-P1(2,:);
        points_2(i,1)*P2(3,:)-P2(1,:);
        points_2(i,2)*P2(3,:)-P2(2,:)];
    [U,S,V]=svd(A);
    points3d(i,:)=V(:,4)/V(4,4);
    %disp(A*points3d(i,:)');
end

%points3d=points3d(:,1:3);
F=[4.768448838192316e-07, 3.099052971381128e-06, -0.0007897715110512412;
 4.161667584960991e-06, -4.828925268444691e-08, 0.001961449901076424;
 -0.0009983384340737579, -0.004488485621847195, 1];
points_1(:,3)=1;
points_2(:,3)=1;

for i=1:4
wallLine3d_1(i,:)=cross(points_1(i,:),points_1(i+4,:));
wallLine3d_2(i,:)=cross(points_2(i,:),points_2(i+4,:));
end
wallLine3d_1(5,:)=cross(points_1(13,:),points_1(18,:));
wallLine3d_1(6,:)=cross(points_1(19,:),points_1(24,:));
wallLine3d_2(5,:)=cross(points_2(13,:),points_2(18,:));
wallLine3d_2(6,:)=cross(points_2(19,:),points_2(24,:));

n=1;
% for i=1:2:5
%  
% vanishPoint(n,:)=cross(wallLine3d_1(i,:),wallLine3d_1(i+1,:));
% n=n+1;
% %epip_line(i,:)=F*vanishPoint(i,:)';
% end

for i=1:3
vanishPoint_1(i,:)=cross(wallLine3d_1(i,:),wallLine3d_1(i+1,:));
vanishPoint_2(i,:)=cross(wallLine3d_2(i,:),F*vanishPoint_1(i,:)');
%epip_line(i,:)=F*vanishPoint(i,:)';
end

for i=1:size(vanishPoint_1,1)
    A=[ vanishPoint_1(i,1)*P1(3,:)-P1(1,:);
        vanishPoint_1(i,2)*P1(3,:)-P1(2,:);
        vanishPoint_2(i,1)*P2(3,:)-P2(1,:);
        vanishPoint_2(i,2)*P2(3,:)-P2(2,:)];
    [U,S,V]=svd(A);
    vanishPoint3d(i,:)=V(:,4)/V(4,4);
    %disp(A*points3d(i,:)');
end


% for i=1:3
% B=[skew_sym(vanishPoint_2(i,:))*P1;
%     wallLine3d_2(i,:)*P2];
% [U1,S1,V1]=svd(B);
% vanishPoint3d(i,:)=V1(:,4)/V1(4,4);
% end

inf_plane=getPlane3d(vanishPoint3d);
H=eye(3,3);
H(:,4)=0;
H(4,:)=inf_plane;

for i=1:size(points_1,1)
    affinePoints3d(i,:)=(H*points3d(i,:)')';
    affinePoints3d(i,:)=affinePoints3d(i,:)/affinePoints3d(i,4);
end

%plot3(points3d(:,3),points3d(:,1),points3d(:,2),'.');
plot3(affinePoints3d(:,3),affinePoints3d(:,1),affinePoints3d(:,2),'.');
grid on
t_wall_points=[affinePoints3d(1,:);affinePoints3d(3,:);affinePoints3d(6,:)];
t_floor_points=[affinePoints3d(13,:);affinePoints3d(15,:);affinePoints3d(20,:)];
affineWallPlane3d=getPlane3d(t_wall_points);
affineWallPlane3d=affineWallPlane3d/affineWallPlane3d(4);
affineFloorPlane3d=getPlane3d(t_floor_points);
affineFloorPlane3d=affineFloorPlane3d/affineFloorPlane3d(4);

%plot wall plane and floor plane
% x=-10:.1:10;
% [X,Y] = meshgrid(x);
% plane3d=affineWallPlane3d;
% plane3d=plane3d/plane3d(4);
% a=plane3d(1); b=plane3d(2); c=plane3d(3); d=plane3d(4);
% Z=(d- a * X - b * Y)/c;
% surf(X,Y,Z)
% shading flat
% xlabel('x'); ylabel('y'); zlabel('z')
% hold on;
% x=-10:.1:10;
% [X,Y] = meshgrid(x);
% plane3d=affineFloorPlane3d;
% plane3d=plane3d/plane3d(4);
% a=plane3d(1); b=plane3d(2); c=plane3d(3); d=plane3d(4);
% Z=(d- a * X - b * Y)/c;
% surf(X,Y,Z);
% hold on;
% plot3(affinePoints3d(:,3),affinePoints3d(:,1),affinePoints3d(:,2),'.');
% 
% [4.33587593303957e-05,8.37874139578300e-05,1.02183874407667,1]