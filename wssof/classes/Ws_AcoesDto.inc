<?php
/**
 * Classe de mapeamento da entidade wssof.ws_acoesdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_acoesdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_AcoesDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_acoesdto';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('identificadorunico');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'codigoobjetivo' => null,
        'codigosubfuncao' => null,
        'codigotipoacao' => null,
        'descricao' => null,
        'codigoiniciativa' => null,
        'codigomomento' => null,
        'codigoacao' => null,
        'snexclusaologica' => null,
        'formaacompanhamento' => null,
        'codigoorgao' => null,
        'localizadores' => null,
        'codigofuncao' => null,
        'sndireta' => null,
        'detalhamentoimplementacao' => null,
        'titulo' => null,
        'exercicio' => null,
        'sntransferenciavoluntaria' => null,
        'codigoproduto' => null,
        'unidaderesponsavel' => null,
        'sndescentralizada' => null,
        'sntransferenciaobrigatoria' => null,
        'snaquisicaoinsumoestrategico' => null,
        'codigotipoinclusaoacao' => null,
        'datahorainclusao' => null,
        'baselegal' => null,
        'beneficiario' => null,
        'codigounidademedida' => null,
        'snregionalizarnaexecucao' => null,
        'snlinhacredito' => null,
        'especificacaoproduto' => null,
        'identificadorunico' => null,
        'dataultimaatualizacao' => null,
        'snacompanhamentoopcional' => null,
        'snparticipacaosocial' => null,
        'codigoprograma' => null,
        'codigoesfera' => null,
        'identificacaosazonalidade' => null,
        'finalidade' => null,
        'insumosutilizados' => null,
    );

    public function inserir(){

        $arCampos  = array();
        $arValores = array();
        $arSimbolos = array();

        foreach( $this->arAtributos as $campo => $valor ){

            if( $valor !== null ){
                $arCampos[]  = $campo;
                $arValores[] = trim( pg_escape_string( $valor ) );
            }
        }

        if( count( $arValores ) ){
            $sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' ) ";
            return $this->executar($sql);
        }
    } // Fim _inserir()

}//end Class
?>