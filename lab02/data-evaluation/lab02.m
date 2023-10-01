%% Lab 02 IRM 2021 %%

%% PostLab Q2_2 %%
filename = 'voltage_data.txt';
data = importdata(filename);
mean_val = mean(data);
std_val = std(data);
formatSpec = 'Mean is %f [V] and std deviation is %f [V]\n';
fprintf(formatSpec,mean_val,std_val)
MeanStd = sprintf(formatSpec,mean_val,std_val);
figure(1)
plot(data, '-o')
grid on
title('Q2.2: Noisy sensor output', 'FontSize', 14)
xlabel('sample number', 'FontSize', 14);
ylabel('voltage value [ V ]', 'FontSize', 14);
legend(MeanStd,'FontSize',14);

%% PostLab Q2_3
% 50 averaged values, each average done on 20 samples
filename = 'voltag_avg_20_samples_per_position.txt';
data = importdata(filename);
figure(2)
mean_val = mean(data);
std_val = std(data);
formatSpec = 'Mean is %f [V] and std deviation is %f [V]\n';
fprintf(formatSpec,mean_val,std_val);
MeanStd = sprintf(formatSpec,mean_val,std_val);
plot(data, '-o');
grid on
title('Q2.3: Average of 20 voltage consecutive values for each datapoint',...   
      'FontSize', 14)
xlabel('sample number',  'FontSize', 14);
ylabel('voltage value [ V ]',  'FontSize', 14);
legend(MeanStd,'FontSize',14);

% 50 averaged values, each average done on 200 samples
filename = 'voltage_avg_200_samples_per_position.txt';
data = importdata(filename);
mean_val = mean(data);
std_val = std(data);
formatSpec = 'Mean is %f [V] and std deviation is %f [V]\n';
fprintf(formatSpec,mean_val,std_val);
MeanStd = sprintf(formatSpec,mean_val,std_val);
figure(3)
plot(data, '-o')
grid on
title('Q2.3: Average of 200 consecutive voltage values for each datapoint', ...
         'FontSize', 14)
xlabel('sample number',  'FontSize', 14);
ylabel('voltage value [ V ]',  'FontSize', 14);
legend(MeanStd,'FontSize',14);

%% PostLab Q3 %%
filename = 'voltage_tesla_distance.txt';
data = importdata(filename);
% disregard last measurement around 10mm because of saturation
field_mT = data(1:end-1,2);
z = data(1:end-1,3);
plot(z, field_mT, 'o', 'Linewidth',1.5);
hold on
R = 20; % mm
D = 10; %mm
% Theoretical magnetic field function Br/2 * f(D,R,z)
B_fun = @(Br,z)Br/2*((D+z)./(sqrt(R^2+(D+z).^2))- z./(sqrt(R^2 + z.^2)));                          
% initial guess for Br
Br0_guess = 157;
% Least square fitting
Br = lsqcurvefit(B_fun,Br0_guess,z,field_mT);
zlin = linspace(z(1),z(end));
plot(zlin,B_fun(Br,zlin), 'Linewidth', 1.5)
legend('Sensor Data','Least Square fitting','FontSize', 14)
xlabel('distance [mm]', 'FontSize', 14);
ylabel('B-field [mT]','FontSize', 14);
title('Q3.4: Magnetic field in mT versus distance in mm','FontSize', 14);
grid on
hold off
