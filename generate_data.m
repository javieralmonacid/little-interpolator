% Generate simple linear ramp

Tend = 0.5;
dt = 0.01;
tstart = 0.1;
tend = 0.4;
activation_level = 100;

t = 0:dt:Tend;
a = 0.0 .* (t <= tstart) + ((activation_level/100) / (tend-tstart) * (t-tstart)) .* ((t<tend) & (t>tstart)) + (activation_level/100) .* (t >= tend);
fileID = fopen('linear_ramp.txt','w');
fprintf(fileID,'%6.2f %6.4f\n',[t; a]);
fclose(fileID);

