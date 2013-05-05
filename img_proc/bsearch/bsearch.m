function [ idx ] = bsearch( array, key, cmp_fnc )
left = 1;
right = length( array );

while( right>left ) 
    mid = floor( (left+right)/2 );
    if cmp_fnc( array(mid), key ) < 0
        left = mid + 1;
    else
        right = mid;
    end
end

if cmp_fnc( array(left), key ) == 0
    idx = left;
else
    idx = [];
end