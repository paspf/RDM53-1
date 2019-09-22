close all
clearvars

% Have user browse for a file, from a specified "starting folder."
% For convenience in browsing, set a starting folder from which to browse.
userProfile = getenv('USERPROFILE');
myDocsFolder = sprintf('%s\\My Documents', userProfile);
startingFolder = myDocsFolder;
if ~exist(startingFolder, 'dir')
  % If that folder doesn't exist, just start in the current folder.
  startingFolder = pwd;
end
% Get the name of the file that the user wants to use.
defaultFileName = fullfile(startingFolder, '*.txt*');
[baseFileName, folder] = uigetfile(defaultFileName, 'Select a file');
if baseFileName == 0
  % User clicked the Cancel button.
  return;
end
fullFileName = fullfile(folder, baseFileName);

fid = fopen(fullFileName, 'r');
formatSpec = '%f';
sizeA = [3 Inf];
A = fscanf(fid,formatSpec,sizeA);
x = A(1,:);
y = A(2,:);
z = A(3,:);
mx_offset = (max(x) + min(x))/2;
my_offset = (max(y) + min(y))/2;
mz_offset = (max(z) + min(z))/2;

avg_delta_x =(max(x) - min(x))/2;
avg_delta_y =(max(y) - min(y))/2;
avg_delta_z =(max(z) - min(z))/2;

avg_delta = avg_delta_x + avg_delta_y + avg_delta_z;
avg_delta = avg_delta /3;

x_scale = avg_delta / avg_delta_x;
y_scale = avg_delta / avg_delta_y;
z_scale = avg_delta / avg_delta_z;

xmid = x-mx_offset;
ymid = y-my_offset;
zmid = z-mz_offset;

xfin = xmid * x_scale;
yfin = ymid * y_scale;
zfin = zmid * z_scale;

figure('Name', 'Recorded Data');
scatter(x,y,'b');
hold on
scatter(x,z,'b');
scatter(y,z,'b');
axis equal
w = waitforbuttonpress;
q = questdlg('Would you like to continue?');
close all

figure('Name', 'Data with corrected Offset');
scatter(xmid,ymid,'b');
hold on
scatter(xmid,zmid,'b');
scatter(ymid,zmid,'b');
axis equal
text = "Recommended Offset:" + "\n"...
    + "X Offset = " + sprintf('%0.5f',mx_offset) + "\n"...
    + "Y Offset = " + sprintf('%0.5f',my_offset) + "\n"...
    + "Z Offset = " + sprintf('%0.5f',mz_offset);
text = compose(text);

q = "No";
while q ~= "Yes" 
w = waitforbuttonpress;
waitfor(msgbox(text,'Offset Values'));
q = questdlg('Would you like to continue?');
end
close all


figure('Name','Data with corrected Scale & Offset');
scatter(xfin,yfin,'b');
hold on
scatter(xfin,zfin,'b');
scatter(yfin,zfin,'b');
axis equal;
text = "Recommended Scale:" + "\n" + "\n"...
    + "Average Scale = " + sprintf('%0.5f',avg_delta)+ "\n"...
    + "X Scale = " + sprintf('%0.5f', x_scale)+ "\n"...
    + "Y Scale = " + sprintf('%0.5f', y_scale)+ "\n"...
    + "Z Scale = " + sprintf('%0.5f', z_scale);
    
text = compose(text);

q = "No";
while q ~= "Yes" 
w = waitforbuttonpress;
waitfor(msgbox(text,'Scale values'));
q = questdlg('Would you like to continue?');
end
close all