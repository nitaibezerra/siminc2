var TipoFuncaoALI = function() {
    TipoFuncao.apply( this );
};

TipoFuncaoALI.prototype = new TipoFuncao();

TipoFuncaoALI.prototype.calcularIntervaloTRAR   = function( qtdTRAR ){
    
    var index = '';
    
    if( qtdTRAR == 1 ) {
        index   = 'linhaA';
    }else if( qtdTRAR >=2 && qtdTRAR <= 5 ) {
        index   = 'linhaB';
    }else{
        index   = 'linhaC';
    }

    return index;
};

TipoFuncaoALI.prototype.calcularIntervaloTD = function( qtdTD ){
    
    var index = '';
    
    if( qtdTD < 20 ) {
        index   = 'coluna1';
    }else if( qtdTD >=20 && qtdTD <= 50 ) {
        index   = 'coluna2';
    }else{
        index   = 'coluna3';
    }

    return index;
};

TipoFuncaoALI.prototype.calcularQuantidadePF  = function( complexidade  ){
    var qtdPF = 0;
    switch( complexidade ){
        case TipoFuncao.BAIXA: 
            qtdPF   = 7;
            break;
        case TipoFuncao.MEDIA: 
            qtdPF   = 10;
            break;
        case TipoFuncao.ALTA: 
            qtdPF   = 15;
            break;
    }
    
    return qtdPF;
};
