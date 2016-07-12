% figure
% title('Rauschen Accelerometer')
% subplot(5,1,1)       % add first plot in 2 x 1 grid
% plot(x)
% title('x')
% xlabel('Samples')
% ylabel('LSBs')
% 
% subplot(5,1,2)       % add second plot in 2 x 1 grid
% plot(y)       % plot using + markers
% title('y')
% xlabel('Samples')
% ylabel('LSBs')
% 
% subplot(5,1,3)       % add second plot in 2 x 1 grid
% plot(z)       % plot using + markers
% title('z')
% xlabel('Samples')
% ylabel('LSBs')
% 
% subplot(5,1,4)       % add second plot in 2 x 1 grid
% plot(pitch)       % plot using + markers
% title('pitch')
% xlabel('Samples')
% ylabel('Grad')
% 
% subplot(5,1,5)       % add second plot in 2 x 1 grid
% plot(roll)       % plot using + markers
% title('roll')
% xlabel('Samples')
% ylabel('Grad')

% subplot(3,1,1)
% plot(Zeit, Temp)
% grid on
% title('Temperatur')
% xlabel('Samples')
% ylabel('Temp [°C]')
 subplot(2,1,1)
%figure(1) 
plot(pitch)       % plot using + markers
title('pitch')
xlabel('Samples')
ylabel('pitch in Grad')
set(gca,'YTick',-90:0.1:90, 'YMinorTick','on')
grid on
subplot(2,1,2)
%figure(2)
plot(roll)       % plot using + markers
title('roll')
xlabel('Samples')
ylabel('roll in Grad')
set(gca,'YTick',-90:0.1:90, 'YMinorTick','on')
grid on