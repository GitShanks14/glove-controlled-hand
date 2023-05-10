clear;
load("Readings-08-05/suraj5.mat");

jj=1;
T=10;
Fs = 100;

ii = length(t);

figure;
while jj <= 5
    subplot(5,1,jj);
    plot(t(1:ii),v(1:ii,jj));
    xlabel("Time (s)");
    ylabel("Amplitude (V)");
    title("Voltage vs Time signal");
    xlim([0 T]);
    %ylim([-200 1024]);
    jj = jj + 1;
end