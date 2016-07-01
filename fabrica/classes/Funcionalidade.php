<?php
class Funcionalidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "inventario.tb_simec_funcionalidade";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "co_funcionalidade" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'co_funcionalidade'         => null,
        'co_tipo_funcionalidade'    => null,
        'co_inventario'             => null,
        'co_agrupador'              => null,
        'tp_complexidade'           => null,
        'ds_alr_rlr'                => null,
        'ds_td'                     => null,
        'ds_funcionalidade'         => null,
        'qt_alr_rlr'                => null,
        'qt_td'                     => null,
        'qtd_pf'                    => null,
        'st_funcionalidade'         => null
    );
    
    /**
     * Verifica se determinada funcionalidade já está cadastrada 
     * @param type $dsFuncionalidade
     * @param type $coAgrupador
     * @param type $noAgrupador
     * @param type $coFuncionalidade
     * @return type 
     */
    public function verificaSeExisteFuncionalidade($dsFuncionalidade, $coAgrupador = NULL, $noAgrupador = NULL, $coFuncionalidade = null){
        
        $sql    = "SELECT count(co_funcionalidade) as total 
                    FROM inventario.tb_simec_funcionalidade func
                    INNER JOIN inventario.tb_simec_agrupador_funcionalidade agru
                        ON func.co_agrupador = agru.co_agrupador
                    WHERE ds_funcionalidade ilike '{$dsFuncionalidade}' ";
        
        $arrWhere  = array();
        
        if( !empty($coAgrupador) ){
            $arrWhere[] = " agru.co_agrupador = {$coAgrupador}";
        }
        
        if( !empty($noAgrupador) ){
            $arrWhere[] = " agru.no_agrupador ilike '{$noAgrupador}'";
        }
        
        if( !empty($coFuncionalidade) ){
            $arrWhere[] = " func.co_funcionalidade <> {$coFuncionalidade}";
        }
        
        $where = implode(' AND ', $arrWhere);
        $sql .= !empty($where) ? " AND {$where}" : '';
        $resultado  = $this->carregar($sql);
        return (bool) $resultado[0]['total'];
    }
}