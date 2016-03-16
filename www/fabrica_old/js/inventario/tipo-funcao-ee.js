var TipoFuncaoEE= function() {
    TipoFuncao.apply( this );
};

TipoFuncaoEE.prototype = new TipoFuncao();

TipoFuncaoEE.prototype.calcularIntervaloTRAR   = function( qtdTRAR ){
    
    var index = '';
    
    if( qtdTRAR < 2 ) {
        index   = 'linhaA';
    }else if( qtdTRAR  == 2 ) {
        index   = 'linhaB';
    }else{
        index   = 'linhaC';
    }

    return index;
};

TipoFuncaoEE.prototype.calcularIntervaloTD = function( qtdTD ){
    
    var index = '';
    
    if( qtdTD < 5 ) {
        index   = 'coluna1';
    }else if( qtdTD >=5 && qtdTD <= 15 ) {
        index   = 'coluna2';
    }else{
        index   = 'coluna3';
    }

    return index;
};

TipoFuncaoEE.prototype.calcularQuantidadePF  = function( complexidade  ){
    var qtdPF = 0;
    switch( complexidade ){
        case TipoFuncao.BAIXA: 
            qtdPF   = 3;
            break;
        case TipoFuncao.MEDIA: 
            qtdPF   = 4;
            break;
        case TipoFuncao.ALTA: 
            qtdPF   = 6;
            break;
    }
    
    return qtdPF;
};