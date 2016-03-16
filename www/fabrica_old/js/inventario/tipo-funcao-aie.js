/**
 * Definição da classe TipoFuncaoAIE
 */
var TipoFuncaoAIE = function() {
    TipoFuncao.apply( this );
};

TipoFuncaoAIE.prototype = new TipoFuncao();

TipoFuncaoAIE.prototype.calcularIntervaloTRAR   = function( qtdTRAR ){
    
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

TipoFuncaoAIE.prototype.calcularIntervaloTD = function( qtdTD ){
    
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

TipoFuncaoAIE.prototype.calcularQuantidadePF  = function( complexidade  ){
    var qtdPF = 0;
    switch( complexidade ){
        case TipoFuncao.BAIXA: 
            qtdPF   = 5;
            break;
        case TipoFuncao.MEDIA: 
            qtdPF   = 7;
            break;
        case TipoFuncao.ALTA: 
            qtdPF   = 10;
            break;
    }
    
    return qtdPF;
};