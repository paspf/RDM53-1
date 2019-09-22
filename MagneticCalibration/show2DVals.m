function show2DVals(x,y,z,title)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
close all;
f = figure;
scatter(x,y);
hold
scatter(x,z);
scatter(y,z);
title(title);
waitfor(f);
end

