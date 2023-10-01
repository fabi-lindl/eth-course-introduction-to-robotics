%tabula rasa
clear,clf,clc,close all

%choose logfile to plot
file = uigetfile('*.txt');

%TODO
%adjust here which part of the plot to see
t_start = 0;
t_end = 10;
pos_ylim = [-10 120];
vel_ylim = [-500 500];

%import file with 6 headerlines
A = importdata(file,' ',5);

%export information from headerlines
line = A.textdata{2,1};
weights = textscan(line,'#PID Parameters: kp: %.2f, kd: %.2f, ki: %.2f');
weight_string = sprintf('Kp: %.2f, Kd: %.2f, Ki: %.2f',weights{1}, weights{2}, weights{3});

%extract columns
t = (A.data(:,1) - A.data(1,1)) / 1000000;
x_ref = A.data(:,2);
y_ref = A.data(:,3);
vx_ref = A.data(:,4);
vy_ref = A.data(:,5);
x_raw = A.data(:,6);
y_raw = A.data(:,7);
x = A.data(:,8);
y = A.data(:,9);
vx_raw = A.data(:,10);
vy_raw = A.data(:,11);
vx = A.data(:,12);
vy = A.data(:,13);

%Step Response Plot
figure('Name',file)
hold on;
plot(t,x_ref,'--k','LineWidth',2)
plot(t,x_raw,'Color',[0, 0.4470, 0.7410, 0.2],'LineWidth',2);
plot(t,x,'Color',[0, 0.4470, 0.7410, 1],'LineWidth',2)
%plot limits for steady state error
t_ = linspace(t_start,t_end);
plot(t_,85*ones(100,1),':k');
legend('Reference','Unfiltered','Filtered','Error Limit',...
    'AutoUpdate','off','Location','Southeast');
plot(t_,75*ones(100,1),':k');
ylim(pos_ylim);
xlim([t_start t_end])
title('Step Response');
dim = [0.55 0.6 0.3 0.3];
annotation('textbox',dim,'String',weight_string,'FitBoxToText','on');
xlabel('t [s]')
ylabel('x [mm]')
grid on;
grid minor;
hold off;

%Overview Plot
figure('Name','Overview')
subplot(2,2,1)
hold on;
plot(t,x_ref,'--k','LineWidth',2)
plot(t,x_raw,'Color',[0, 0.4470, 0.7410, 0.32],'LineWidth',2);
plot(t,x,'Color',[0, 0.4470, 0.7410, 1],'LineWidth',2)
ylim(pos_ylim);
xlim([t_start t_end])
title('x-Position')
xlabel('t [s]')
ylabel('x [mm]')
grid on;
hold off;

subplot(2,2,3)
hold on;
plot(t,vx_ref,'--k','LineWidth',2)
plot(t,vx_raw,'Color',[0.8500, 0.3250, 0.0980, 0.2],'LineWidth',2);
plot(t,vx,'Color',[0.8500, 0.3250, 0.0980, 1],'LineWidth',2);
ylim(vel_ylim)
xlim([t_start t_end])
title('x-Velocity')
xlabel('t [s]')
ylabel('v_x [mm/s]')
grid on;
hold off;

subplot(2,2,2)
hold on;
plot(t,y_ref,'--k','LineWidth',2)
plot(t,y_raw,'Color',[0, 0.4470, 0.7410, 0.2],'LineWidth',2);
plot(t,y,'Color',[0, 0.4470, 0.7410, 1],'LineWidth',2);
ylim(pos_ylim);
xlim([t_start t_end])
title('y-Position')
xlabel('t [s]')
ylabel('y [mm]')
grid on;
hold off;

subplot(2,2,4)
hold on;
plot(t,vy_ref,'--k','LineWidth',2)
plot(t,vy_raw,'Color',[0.8500, 0.3250, 0.0980, 0.2],'LineWidth',2);
plot(t,vy,'Color',[0.8500, 0.3250, 0.0980, 1],'LineWidth',2);
ylim(vel_ylim)
xlim([t_start t_end])
title('y-Velocity')
xlabel('t [s]')
ylabel('v_y [mm/s]')
grid on;
hold off;

sgtitle('Overview')

% Trajectory Plot
figure
hold on
plot(x_raw,y_raw,'Color',[0, 0.4470, 0.7410, 0.2],'LineWidth',2);
plot(x,y,'Color',[0, 0.4470, 0.7410, 1],'LineWidth',2);

t = linspace(0,2*pi);
plot(80*sin(t), 80*cos(t),'--k');
plot(60*sin(t), 60*cos(t),'--k')
plot(100*sin(t), 100*cos(t),'--k')

plot(20*sin(t), 20*cos(t),'--k')
plot(20*sin(t) + 80, 20*cos(t),'--k')
plot(20*sin(t) - 80, 20*cos(t),'--k')
plot(20*sin(t), 20*cos(t) + 80,'--k')
plot(20*sin(t), 20*cos(t) - 80,'--k')

xlim([-120,120]);
ylim([-120,120]);
xlabel('x [mm]');
ylabel('y [mm]');
title('Trajectory');
axis square
grid on;
grid minor;
hold off

