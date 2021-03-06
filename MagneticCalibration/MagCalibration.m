close all
clearvars

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
x = A(1,:)*1.2;
y = A(2,:);
z = A(3,:)*0.9;
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

xmid = (x-mx_offset);
ymid = (y-my_offset);
zmid = (z-mz_offset);

xfin = xmid * x_scale;
yfin = ymid * y_scale;
zfin = zmid * z_scale;

todo = "someval";

text = "Offset Values" + "\n"+ ...
    "X-Axis Offset: " + sprintf('%f0.5',mx_offset) +"\n"+...
    "Y-Axis Offset: " + sprintf('%f0.5',my_offset) +"\n"+...
    "Z-Axis Offset: " + sprintf('%f0.5',mz_offset) +"\n"+"\n"+...
    "Scale Values" + "\n"+ ...
    "X-Axis Scale: " + sprintf('%f0.5',x_scale) +"\n"+...
    "Y-Axis Scale: " + sprintf('%f0.5',y_scale) +"\n"+...
    "Z-Axis Scale: "+ sprintf('%f0.5',z_scale);
text = compose(text);

waitfor(msgbox(text,"Offset & Scale"));

while todo ~= "exit"
    text = "Please choose one of the following options";
    opt1 = "Original Data";
    opt3 = "Offset Data";
    opt5 = "Scaled Data";
    todo = questdlg(text, 'Main Menu', opt1, opt3, opt5, "exit");
    
    switch todo
        case opt1
            todo = showVals(x,y,z,opt1);
        case opt3
            todo = showVals(xmid,ymid,zmid,opt3);     
        case opt5
            todo = showVals(xfin,yfin,zfin,opt5);
        otherwise
            break;
    end
end

