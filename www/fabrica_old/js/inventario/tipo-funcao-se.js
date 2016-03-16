var TipoFuncaoSE = function() {
    TipoFuncao.apply( this );
};

TipoFuncaoSE.prototype = new TipoFuncao();

TipoFuncaoSE.prototype.calcularIntervaloTRAR   = function( qtdTRAR ){
    
    var index = '';
    
    if( qtdTRAR < 2 ) {
        index   = 'linhaA';
    }else if( qtdTRAR  >= 2 && qtdTRAR <= 3 ) {
        index   = 'linhaB';
    }else{
        index   = 'linhaC';
    }

    return index;
};

TipoFuncaoSE.prototype.calcularIntervaloTD = function( qtdTD ){
    
    var index = '';
    
    if( qtdTD < 6 ) {
        index   = 'coluna1';
    }else if( qtdTD >= 6 && qtdTD <= 19 ) {
        index   = 'coluna2';
    }else{
        index   = 'coluna3';
    }

    return index;
};

TipoFuncaoSE.prototype.calcularQuantidadePF  = function( complexidade  ){
    var qtdPF = 0;
    switch( complexidade ){
        case TipoFuncao.BAIXA: 
            qtdPF   = 4;
            break;
        case TipoFuncao.MEDIA: 
            qtdPF   = 5;
            break;
        case TipoFuncao.ALTA: 
            qtdPF   = 7;
            break;
    }
    
    return qtdPF;
};