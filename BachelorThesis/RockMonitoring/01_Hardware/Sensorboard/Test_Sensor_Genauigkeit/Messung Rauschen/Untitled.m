figure
title('Rauschen Accelerometer')
subplot(3,1,1)       % add first plot in 2 x 1 grid
plot(x)
set(gca,'fontsize',18)
title('x')
xlabel('Samples')
ylabel('LSBs')

subplot(3,1,2)       % add second plot in 2 x 1 grid
plot(y)       % plot using + markers
set(gca,'fontsize',18)
title('y')
xlabel('Samples')
ylabel('LSBs')

subplot(3,1,3)       % add second plot in 2 x 1 grid
plot(z)       % plot using + markers
set(gca,'fontsize',18)
title('z')
xlabel('Samples')
ylabel('LSBs')