
fwant = 200;

Tsample = 0.0055e-3;
Twant = 1/fwant;
Twant_sample = Twant/120;
delayUs = (Twant_sample - Tsample)*1e6

%% Q6
FreqArray = [20, 40, 60, 80, 100, 200];
Vin = [3.12, 3.12, 3.12, 3.12, 3.12, 3.12];
Vout = [1.56, 1.56, 1.56, 1.56, 1.56, 1.56];
plot(FreqArray, Vout./Vin, 'r*--', 'linewidth', 1, 'Markersize', 10.5)
title('Ratio of output voltage to input voltage')
xticks([20 40 60 80 100 200]);
xlabel('Frequency (Hz)')
ylabel('Vout / Vin');
grid on

%% Q7
FreqArray = [20, 40, 60, 80, 100, 200];
Vin = [3.12, 3.12, 3.12, 3.12, 3.12, 3.12];
Vout = [2.88, 2.64, 2.28, 2, 1.76, 1.08];
PhaseShift = [14, 34,  38, 53, 59, 68];
figure(1)
plot(FreqArray, Vout./Vin, 'r*--', 'linewidth', 1, 'Markersize', 10.5)
title('Ratio of output voltage to input voltage')
xticks([20 40 60 80 100 200]);
xlabel('Frequency (Hz)')
ylabel('Vout / Vin');
grid on

figure(2)
plot(FreqArray, PhaseShift, 'r*--', 'linewidth', 1, 'Markersize', 10.5)
title('Phase Shift (Out - In) in function of frequency')
xlabel('Frequency (Hz)')
ylabel('Phase Shift in degrees');
xticks([20 40 60 80 100 200]);
grid on