<?php
/**
 * Classe de mapeamento da entidade wssof.ws_localizadoresdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_localizadoresdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_LocalizadoresDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_localizadoresdto';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('identificadorunico');
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'totalfisico' => null,
        'descricao' => null,
        'codigomomento' => null,
        'anotermino' => null,
        'snexclusaologica' => null,
        'minicipio' => null,
        'identificadorunicoacao' => null,
        'exercicio' => null,
        'totalfinanceiro' => null,
        'codigoregiao' => null,
        'justificativarepercussao' => null,
        'anoinicio' => null,
        'municipio' => null,
        'regiao' => null,
        'siglaregiao' => null,
        'uf' => null,
        'codigolocalizador' => null,
        'mesanotermino' => null,
        'datahoraalteracao' => null,
        'mesanoinicio' => null,
        'identificadorunico' => null,
        'dataultimaatualizacao' => null,
        'snacompanhamentoopcional' => null,
        'codigotipoinclusao' => null,
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