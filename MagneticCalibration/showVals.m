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

scatter(x,y, 'b');
hold on
scatter(x,z, 'r');
scatter(y,z, 'g');
title(figuretitle + " 2D");
axis equal
grid on
legend({'(x,y)','(x,z)', '(y,z)'},'Location','southwest')
waitfor(f);
returnvalue = "exiting";
end

