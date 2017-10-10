function points3d=linear_triangle(points_1,points_2,P1)
for i=1:size(points_1,1)
    A=[ points_1(i,1)*P1(3,:)-P1(1,:);
        points_1(i,2)*P1(3,:)-P1(2,:);
        points_2(i,1)*P2(3,:)-P2(1,:);
        points_2(i,2)*P2(3,:)-P2(2,:)];
    [U,S,V]=svd(A);
    points3d(i,:)=V(:,4)/V(4,4);
    %disp(A*points3d(i,:)');
end