% Test health metric computation math

close all;
clear all;

% Surface Water
h = -100:.001:100;

K = 100;
A = -100;
B = 1/8;
C = 0;

y = (K-A)./(1+exp(-B*(h-C))) + A - 1/2;

figure;
plot(h, y);
ylabel('New Health Metric');
xlabel('Health Metric (0 is ideal)');
title('Health Metric Computation Curve');

% dy/dsw
yp = -sign(h).*B*(K-A).*exp(-B*(h-C))./(power(1+exp(-B*(h-C)), 2)); 

figure;
plot(h, yp);
title('dy/dsw');
ylabel('\Delta Health Metric');
xlabel('Health Metric');

% Dependence on water added
tp = 100;
