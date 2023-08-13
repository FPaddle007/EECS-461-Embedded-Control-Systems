% ====================================================================================================
% Embedded Control Pre-Lab 6
% Description:
% Simulates the motion of a haptic wheel attached to a virtual wheel whose rotational intertia is J_w. 
% The wheels are attached through a shaft with torsional spring constant k. The simulation assumes 
% that the wheel is turned through a fixed initial angle and then held constant.
% ====================================================================================================
clc
clear
format long

k = ?????            % spring constant in N-mm/degree 
Jw = ????            % rotational inertia in N-mm/(degree/sec^2)

step_time = 1;          % time of step change in wheel position (seconds)
Theta_z_0 = 45;         % size of step change in wheel position (degrees) = theta_z_0

tfinal = 10;            % final time for simulation (seconds)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Virtual Wheel System - Continuous, No Damping

b = 0;                  % no damping
char_roots_cont = roots([Jw b k]);    % characteristic roots
Deltat = .001;          % simulation step size    (seconds)  -- should be small with respect to T_n

maximum_torque = k*45;   % N-mm - max torque specified in lab assignment
omega_n = sqrt(k/Jw);   % frequency of oscillation in rad/sec
f_n = omega_n/(2*pi);    % frequency of oscillation in Hz
T_n = 1/f_n;            % period of oscillation in seconds

% Simulate Model
sim('virtual_wheel')

% Compute torque limits in N-mm at wheel determined
% by 24-76% duty cycle
dutycycle_max = .76;
motor_torque_max = (dutycycle_max-.5)*3162.5;
 

clear torque_limit
for i = 1:length(tout)
    torque_limit(i) = motor_torque_max;
end
 
figure(1)
clf

  
plot(tout,yout(:,1),tout,yout(:,2),...
     tout,yout(:,4),tout,yout(:,3),tout,torque_limit,'k-',tout,-torque_limit,'k-')
h = xlabel('time, seconds');
set(h,'interpreter','latex','fontsize',14); 
ylabel('continuous system, no damping','FontSize',16)

h = title(['$k = $',num2str(k,4),' Nmm/deg, $J_w = $',num2str(Jw,2),...
    ' Nmm/(deg/sec$^2$), $f_n = $',num2str(f_n,2),' Hz']);
set(h,'interpreter','latex','fontsize',14); 
h = legend('$\theta_{w}$ (deg)','$d\theta_{w}/dt$ (deg/sec)','torque (Nmm)',...
    '$\theta_z$ (deg)','torque limit','Location','Southwest');
set(h,'interpreter','latex','fontsize',14); 
set(findall(gcf,'type','line'),'linewidth',2)   
   

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Virtual Wheel System - Discrete, No Damping

b = 0;                                      % no damping
T = .001;
char_roots_disc = 1+T*roots([Jw b k])    % characteristic roots of system discretized with
                                            % forward euler aproximation
magnitude_roots = abs(char_roots_disc)

% Simulate Model
sim('virtual_wheel_discrete')

clear torque_limit
for i = 1:length(tout)
    torque_limit(i) = motor_torque_max;
end
 
figure(2)
clf
plot(tout,yout(:,1),tout,yout(:,2),...
     tout,yout(:,4),tout,yout(:,3),tout,torque_limit,'k-',tout,-torque_limit,'k-')
h = xlabel('time, seconds');
set(h,'interpreter','latex','fontsize',14); 
ylabel('discrete system, no damping','FontSize',16)
h = title(['$k = $',num2str(k,4),' Nmm/deg, $J_w = $',num2str(Jw,2),...
    ' Nmm/(deg/sec$^2$), $f_n = $',num2str(f_n,2),' Hz']);
set(h,'interpreter','latex','fontsize',14); 
h = legend('$\theta_{w}$ (deg)','$d\theta_{w}/dt$ (deg/sec)','torque (Nmm)',...
    '$\theta_z$ (deg)','torque limit','Location','Southwest');
set(h,'interpreter','latex','fontsize',14); 
set(findall(gcf,'type','line'),'linewidth',2) 


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Virtual Wheel System - Continuous, With Damping
 
b = ????           % damping constant in N-mm/(degree/sec)
char_roots_cont = roots([Jw b k]);	% characteristic roots
Deltat = .001;          % simulation step size    (seconds)  -- should be small with respect to T_n

% Simulate Model
sim('virtual_wheel_damping')

clear torque_limit
for i = 1:length(tout)
    torque_limit(i) = motor_torque_max;
end
 
figure(3)
clf

plot(tout,yout(:,1),tout,yout(:,2),...
     tout,yout(:,4),tout,yout(:,3),tout,torque_limit,'k-',tout,-torque_limit,'k-')

h = xlabel('time, seconds');
set(h,'interpreter','latex','fontsize',14); 
ylabel('continuous system with damping','FontSize',16)
h = title(['$k = $',num2str(k,4),' Nmm/deg, $J_w = $',num2str(Jw,2),...
    ' Nmm/(deg/sec$^2$), $f_n = $',num2str(f_n,2),' Hz']);
set(h,'interpreter','latex','fontsize',14); 
h = legend('$\theta_{w}$ (deg)','$d\theta_{w}/dt$ (deg/sec)','torque (Nmm)',...
    '$\theta_z$ (deg)','torque limit','Location','Southwest');
set(h,'interpreter','latex','fontsize',14); 
set(findall(gcf,'type','line'),'linewidth',2) 
   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Virtual Wheel System - Discrete, With Damping
 
b = ????                                 % damping constant in N-mm/(degree/sec)
T = .001;
char_roots_disc = 1+T*roots([Jw b k])    % characteristic roots of system discretized with
                                             % forward euler aproximation
magnitude_roots = abs(char_roots_disc)
                                             
% Simulate Model 
sim('virtual_wheel_damping_discrete')

clear torque_limit
for i = 1:length(tout)
    torque_limit(i) = motor_torque_max;
end
 
figure(4)
clf
plot(tout,yout(:,1),tout,yout(:,2),...
     tout,yout(:,4),tout,yout(:,3),tout,torque_limit,'k-',tout,-torque_limit,'k-')
h = xlabel('time, seconds');
set(h,'interpreter','latex','fontsize',14); 
ylabel('discrete system with damping','FontSize',16)
h = title(['$k = $',num2str(k,4),' Nmm/deg, $J_w = $',num2str(Jw,2),...
    ' Nmm/(deg/sec$^2$), $f_n = $',num2str(f_n,2),' Hz']);
set(h,'interpreter','latex','fontsize',14); 
h = legend('$\theta_{w}$ (deg)','$d\theta_{w}/dt$ (deg/sec)','torque (Nmm)',...
    '$\theta_z$ (deg)','torque limit','Location','Southwest');
set(h,'interpreter','latex','fontsize',14); 
set(findall(gcf,'type','line'),'linewidth',2) 
   
   
   
 