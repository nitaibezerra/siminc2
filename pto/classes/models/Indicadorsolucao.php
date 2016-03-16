<?php

class Model_Indicadorsolucao extends Abstract_Model
{
    protected $_schema = 'pto';
    protected $_name = 'indicadorsolucao';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['insid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['indid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
    }

    public function getOptionsIndicador($where = null, $dados = array())
    {
        $where = '';
        if( !empty($dados) && is_null($where) && !empty($dados['acaid'])){
            $strAcaid = implode(',', $dados['acaid']);
            $where = " AND acaid IN ( {$strAcaid} ) ";
        }
        $sql = "SELECT indid as codigo, indid || ' - ' ||indnome as descricao FROM painel.indicador
        WHERE indstatus = 'A'
        {$where}
        ORDER BY codigo ";
        $dados = $this->_db->carregar($sql);
        return $this->getOptions($dados, array(), 'indid');
    }

    public function salvarIndicador($arrayIndicador, $idSolucao){
        if (is_array($arrayIndicador)) {
            $this->deleteAllByValues(array('solid' => $idSolucao));
            foreach( $arrayIndicador as $indicadorID){
                $this->setAttributeValue('solid', $idSolucao);
                $this->setAttributeValue('indid', $indicadorID);
                $id = $this->save();
                if($id == false){
                    throw new Exception('Erro ao inserir Indicador.');
                }
            }
        }
    }

    public function getIndicadorPainel($solid){
        $solid = (int)$solid;
        $sql = "SELECT ind.indid, ind.indnome AS nome, ind.indobjetivo AS descricao, ind.indformula AS formula, ind.indfontetermo AS fonte, per.perdsc AS periodicidade
                    FROM painel.indicador ind
                    INNER JOIN painel.periodicidade per ON per.perid = ind.perid
                    LEFT JOIN pto.indicadorsolucao indsol ON indsol.indid = ind.indid
                    LEFT JOIN pto.solucao sol ON sol.solid = indsol.solid
                    WHERE  sol.solid = 	{$solid}
               ";
        return $this->_db->carregar($sql);
    }
}
