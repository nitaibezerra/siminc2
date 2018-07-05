<?php
/**
 * Classe de mapeamento da entidade proposta.subunidaderplimite
 *
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_Subunidaderplimite
 *
 */
class Proposta_Model_Subunidaderplimite extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.subunidaderplimite';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'srlid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'suoid' => array('tabela' => 'public.subunidadeorcamentaria', 'pk' => 'suoid'),
        'irpcod' => array('tabela' => 'public.identresultprimario', 'pk' => 'irpcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'srlid' => null,
        'suoid' => null,
        'irpcod' => null,
        'srllimite' => null,
        'srldtcadastro' => null,
        'srlstatus' => null
    );

    public function montarSqlBuscarPorSuoidIrpcod(){
        $sql = "
            SELECT
                *
            FROM $this->stNomeTabela
            WHERE
                srlstatus = 'A'
                AND suoid = ". (int)$this->suoid. "
                AND irpcod = '". (int)$this->irpcod. "'
        ";
        return $sql;
    }
    
    public function buscarPorSuoidIrpcod() {
        $sql = $this->montarSqlBuscarPorSuoidIrpcod();
        $resultado = $this->pegaLinha($sql);
        if($resultado){
            $this->popularDadosObjeto($resultado);
        }
        
        return $this;
    }
}//end Class

