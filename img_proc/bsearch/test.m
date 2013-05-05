clc; clear;
A = sort(randi(1000, 1, 500));
idx = randi(500);
res = bsearch(A, A(idx), @comp );
if( idx == res )
    disp('success');
else 
    disp('fail');
end
fprintf('key = %d\tidx = %d', A(idx), res));


