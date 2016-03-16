/**
 * Definição da classe TipoFuncao
 */
var TipoFuncao = function(){
    
    this.matrizComplexidade = {
        linhaA   : { 
            coluna1 : TipoFuncao.BAIXA, 
            coluna2 : TipoFuncao.BAIXA, 
            coluna3 : TipoFuncao.MEDIA 
        },
        linhaB   : { 
            coluna1 : TipoFuncao.BAIXA, 
            coluna2 : TipoFuncao.MEDIA, 
            coluna3 : TipoFuncao.ALTA 
        },
        linhaC  : { 
            coluna1 : TipoFuncao.MEDIA, 
            coluna2 : TipoFuncao.ALTA, 
            coluna3 : TipoFuncao.ALTA 
        }
    };
};

/**
 * Constantes com os tipos existentes de complexidade
 * 'A' - Alta
 * 'M' - Média
 * 'B' - Baixa
 */
TipoFuncao.BAIXA    = 'B';
TipoFuncao.MEDIA    = 'M';
TipoFuncao.ALTA     = 'A';

/**
 * Retorna a complexidade do baseada no valor de ALR/RLR e TD informados
 * O valor está contido na propriedade matrizComplexidade
 * @param int qtdTRAR - Valor de ALR/RLR
 * @param int qtdTD - Valor de TD
 * @return String 
 */
TipoFuncao.prototype.calcularComplexidade  = function( qtdTRAR, qtdTD ){
    var indexA  = this.calcularIntervaloTRAR(qtdTRAR);
    var indexB  = this.calcularIntervaloTD(qtdTD);
    
    return this.matrizComplexidade[indexA][indexB];
};

/**
 * Retorna o tipo de cálculo a ser utilizado 
 * a partir do tipo de função informada
 * Os tipos de função são: AIE, ALI, CE, EE, SE 
 * Baseado no factory pattern
 * @return TipoFuncao
 */
TipoFuncao.retornaTipoCalculo  = function( tipoFuncionalidade ){
    if( tipoFuncionalidade == 2 ){
        return new TipoFuncaoAIE();
    } else if( tipoFuncionalidade == 3 ) {
        return new TipoFuncaoALI();
    } else if( tipoFuncionalidade == 4  ) {
        return new TipoFuncaoCE();
    } else if( tipoFuncionalidade == 5 ) {
        return new TipoFuncaoEE()
    } else if( tipoFuncionalidade == 6 ) {
        return new TipoFuncaoSE();
    }
    return null;
};

/**
 * Retorna o índice utilizado para calcular o tipo de complexidade
 * baseado o valor ALR/RLR
 * O índice está relacionado a propriedade matrizComplexidade 
 * Deve ser reemplementado em cada tipo de função para seu devido 
 * @param int qtdTD - quantidade de ALR/RLR
 * @return String índice
 */
TipoFuncao.prototype.calcularIntervaloTRAR  = function( qtdTRAR ){
    throw new Error('Implementar método');
};

/**
 * Retorna o índice utilizado para calcular o tipo de complexidade
 * baseado no valor TD
 * O índice está relacionado a propriedade matrizComplexidade 
 * Deve ser reemplementado em cada tipo de função para seu devido 
 * @param int qtdTD - quantidade de TD 
 * @return String índice
 */
TipoFuncao.prototype.calcularIntervaloTD  = function( qtdTD ){
    throw new Error('Implementar método');
}; 

/**
 * Calcula a quantidade de pontos de função dado uma complexidade
 * Deve ser reemplementado em cada tipo de função para seu devido 
 * @param String complexidade - Tipo de complexidade a ser utilizada ( 'A', 'M', 'B')
 * Complexidades:
 * 'A' - Alta
 * 'M' - Média
 * 'B' - Baixa
 * 
 * @return int
 */
TipoFuncao.prototype.calcularQuantidadePF  = function( complexidade  ){
    throw new Error('Implementar método');
};
