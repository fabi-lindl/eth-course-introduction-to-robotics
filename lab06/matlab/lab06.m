
%% PreLab 06

%%%%%%%%%%%%%%%%%%%%
% global variables %
%%%%%%%%%%%%%%%%%%%%

%create time variable, goes from 0 to 5 sec in 0.0001 steps
timestep=0.0001;
t=0:timestep:5;

%%%%%%%%%%%%%
% Prelab Q1 %
%%%%%%%%%%%%%

 
% Differential Equatoin of the sytem: x''(t) = 30/5*beta (t)
% create the transfer function of the system (you should use the tf() function):



% set step options to have a step-size of 0.1 instead of 1 (standard), use
% stepDataOptions()



% open loop impulse and step response in one figure (two subplots):

disp("Prelab Q1")
disp("*Here goes your answers for Prelab Q1*")


%%%%%%%%%%%%%
% Prelab Q2 %
%%%%%%%%%%%%%

% Controller Parameters:

Kp=20;
Ki=1;
Kd=2;
Kd_=20;


% create the transfer function of the Controller:


% create the transfer function of the feedback loop


% Plot the closed loop step response

disp("Prelab Q2")
disp("*Here goes your answers for Prelab Q2*")


%%%%%%%%%%%%%
% Prelab Q3 %
%%%%%%%%%%%%%

% controller constants:

%Kp=?;
%Ki=?;
%Kd=?;

% create the transfer function of the Controller 


% Variables for delay:
delay=0.08; % 80ms
delay_= 1; % 1s %if you want to test with 1 sec delay

% create the transfer function of the delay in the system


% create the transfer function of the feedback loop with delay


% Plot the closed loop step response of the system with delay


% check and print design criteria for "steady state" error at 5 seconds

disp("Prelab Q3")
disp("*Here goes your answers for Prelab Q3*")

%%%%%%%%%%%%%
% Prelab Q4 %
%%%%%%%%%%%%%

disp("Prelab Q4")
disp("*Here goes your answers for Prelab Q4*")





