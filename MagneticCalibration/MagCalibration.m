clear all
clearvars
fid = fopen('log2.txt', 'r');
formatSpec = '%f';
sizeA = [3 Inf];
A = fscanf(fid,formatSpec,sizeA);
x = A(1,:);
y = A(2,:);
z = A(3,:);
mx_offset = (max(x) + min(x))/2
my_offset = (max(y) + min(y))/2
mz_offset = (max(z) + min(z))/2

mx_scale =(max(x) - min(x))/2;
my_scale =(max(y) - min(y))/2;
mz_scale =(max(z) - min(z))/2;

avg_scale = ((mx_scale + my_scale + mz_scale)/3);

xmid = x-mx_offset;
ymid = y-my_offset;
zmid = z-mz_offset;

xfin = avg_scale./(xmid-mx_scale);
yfin = avg_scale./(ymid-my_scale);
zfin = avg_scale./(ymid-my_scale);

figure('Name', 'Recorded Data');
scatter(x,y,'b');
hold on
scatter(x,z,'b');
scatter(y,z,'b');
axis equal

figure('Name', 'Data with corrected Offset');
scatter(xmid,ymid,'b');
hold on
scatter(xmid,zmid,'b');
scatter(ymid,zmid,'b');
axis equal

figure('Name','Data with corrected Scale & Offset');
scatter(xfin,yfin,'b');
hold on
scatter(xfin,zfin,'b');
scatter(yfin,zfin,'b');
axis equal;