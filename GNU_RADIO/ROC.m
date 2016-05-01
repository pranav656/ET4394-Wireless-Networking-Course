%This is a modified version of the script in [3].
% Non-Detected Signal distribution
Pnotarget = makedist('Normal','mu', -72.15,'sigma',1.068);
% Detected Signal distribution 
Ptarget = makedist('Normal', 'mu', -55.453,'sigma',4.65);
threshold = -65 ; % threshold  in dB. Change according to needs
Pfa = 1 - cdf(Pnotarget,threshold) % probability of false alarm
Pd = 1 - cdf(Ptarget,threshold) % probability of detection
Level=[-80:-40];
figure(1);
plot(Level,Pnotarget.pdf(Level));
hold on
plot(Level,Ptarget.pdf(Level),'m');
title('RTL-SDR detection probability')
hold on
Y = 0:0.1:0.3;
X = threshold * ones(size(Y));
plot(X, Y, 'r--')
legend('Free Channel Signals ','Detected Channel Signals', 'threshold')
xlabel ('Level (dB)')
Pfa_ROC = 1 - cdf(Pnotarget,Level); % prob of false alarm
Pd_ROC = 1 - cdf(Ptarget,Level); % prob of detection
figure(2);
plot(Pfa_ROC,Pd_ROC);
title('ROC curves')
ylabel ('Probability of Detection')
xlabel ('Probability of False Alarm')
