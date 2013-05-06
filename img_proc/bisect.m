function [ val ] = bisect( f, a, b, tol, nmax)
    val = NaN;
    for i = 1:nmax
        c = floor( (a + b)/2 );
        if( ( f(c) == 0 ) || ( (b - a)/2 < tol ) )
            val = f(c);
            return;
        end
        if( f(c)*f(a) > 0 )
            a = c;
        else
            b = c;
        end
    end
    disp('failed');
end

