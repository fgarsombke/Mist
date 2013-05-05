function [ val ] = comp( a, b )
    if( a > b )
        val = 1;
    elseif( a == b )
        val = 0;
    else
        val = -1;
    end
end