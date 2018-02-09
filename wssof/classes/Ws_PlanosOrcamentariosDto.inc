<?php
/**
 * Classe de mapeamento da entidade wssof.ws_planosorcamentariosdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */



/**
 * Wssof_Ws_planosorcamentariosdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Ws_PlanosOrcamentariosDto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'wssof.ws_planosorcamentariosdto';

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
        'codigomomento' => null,
        'data_insercao' => null,
        'identificadorunicoacao' => null,
        'titulo' => null,
        'exercicio' => null,
        'codigoproduto' => null,
        'planoorcamentario' => null,
        'codigounidademedida' => null,
        'datahoraalteracao' => null,
        'identificadorunico' => null,
        'snatual' => null,
        'dataultimaatualizacao' => null,
        'detalhamento' => null,
        'datahora' => null,
        'descproduto' => null,
        'codigoindicadorplanoorcamentario' => null,
        'descunidademedida' => null,
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