clear;
clc;
%=============�������G===========%
%A(1,:) �Ĥ@Row ; A�]�G�A1�^�Ĥ@Col
TrafficModel1_Simulation_PDF = csvread('SimArrivalMode1_M_5000.csv');
%TrafficModel1_Analysis_PDF = csvread('AnalArrivalModel_M_5000.csv');
%%
% TrafficModel1_Simulation =
% csvread('SimArrivalMode1_M_5000_RA_Slot_1_12000.csv');
%%
% TrafficModel2_Analysis =
% csvread('Analysis_TrafficMode2_M_5000_RA_Slot_0_2000.csv');

xs0 = TrafficModel1_Simulation_PDF(:,1);
ys0 = TrafficModel1_Simulation_PDF(:,2);
%xs1 = TrafficModel1_Analysis_PDF(:,1);
%ys1 = TrafficModel1_Analysis_PDF(:,2);

%% Check Arrival Model 1
% figure(6);
% xs0 = TrafficModel1_Simulation(1:12000,1);
% ys0 = TrafficModel1_Simulation(1:12000,2);

% hsa = plot(xs0, ys0,'bsquare',xs0, ys0, 'gdiamond', xs0, ys2, 'ro'); 
%hsa = plot(xs0, ys0,'ro',xs1, ys1, 'b-'); 
hs = plot(xs0, ys0,'bo');
% %  ha=plot(xa0, ya0,'b-'); 
set(hs, 'linewidth', 1.5);    % ���u�e�קאּ1.5
% %hold on;
% % set(gca,'xtick',[0:2000:18000],'ytick',[0:500:5000]);
% set(gca,'ytick',[0:1:10]);
axis([0, 20, 0,0.4]);
xlabel('k'),ylabel('P(X=k)');
title('Sim. Arrival Model 1');
legend('Sim');
% legend('Sim','Anal');
% %legend('Nc/N Simulation','Nc/N Numerical','(Nc+Ns)/N Simulation','(Nc+Ns)/N Numerical');
% legend('Sim Mi[1], M=5000','Sim Mi[1], M=10000','Sim Mi[1], M=30000');
% % legend('Analysis');
% grid on


