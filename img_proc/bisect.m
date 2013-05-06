function [ val ] = bisect( f, a, b, tol, nmax)
    val = NaN;
    for i = 1:nmax
        c = floor( (a + b)/2 );
        
        f_c = f(c);
        if( ( f_c == 0 ) || ( (b - a)/2 < tol ) )
            val = f_c;
            return;
        end
        
        f_a = f(a);
        if( f_c*f_a > 0 )
            a = c;
        else
            b = c;
        end
    end
    
    error('Iterations exceeded nmax');
end

