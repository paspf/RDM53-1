function returnvalue = showVals(x,y,z,figuretitle)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
close all
f = figure('Name', figuretitle');
%subplot(1,2,2)
scatter3(x,y,z,'black');
hold on;
title(figuretitle + " 3D");
axis equal;

%s = subplot(1,2,1);
figure('Name', figuretitle');

scatter(x,y);
hold on
scatter(x,z);
scatter(y,z);
title(figuretitle + " 2D");
axis equal
grid on
waitfor(f);
returnvalue = "exiting";
end

