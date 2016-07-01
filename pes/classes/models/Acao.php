<?php

class Model_Acao extends Abstract_Model
{
    
    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesacao';
    
    /**
     * Nome da chave primaria
     * @var string
     */
    protected $_primary = 'acacodigo';
    
    public function carregarAcaoPorEntidadeETipoDespesa($entcodigo, $tidcodigo)
    {
        $sql = "SELECT aca.acacodigo, aca.acasituacao, pla.placodigo, aca.acadescricaoacao, aca.acanomeresponsavel FROM pes.pesacao aca
                LEFT JOIN pes.pesplanoacao pla ON (aca.placodigo = pla.placodigo)
                LEFT JOIN pes.pestipodespesa tid ON (pla.tidcodigo = tid.tidcodigo)
                LEFT JOIN pes.pesentidade ent ON (pla.entcodigo = ent.entcodigo)
                WHERE ent.entcodigo = {$entcodigo} AND tid.tidcodigo = {$tidcodigo}";
                
        $return = $this->_db->carregar($sql);
        return $return;
    }
    
}
