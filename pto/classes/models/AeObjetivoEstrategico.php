<?php

class Model_AeObjetivoEstrategico extends Abstract_Model {

    protected $_schema = 'pde';
    protected $_name = 'ae_objetivoestrategico';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['obeid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label'=>'ID Objetivo Estratégico');
        $this->entity['obenome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '500', 'contraint' => '', 'label'=>'Objetivos Estratégicos');
    }
    
    public function getOptionsObjetivoEstrategico($where = null, $dados = array() ) {
        if( !empty($dados) && is_null($where)){
            if(!empty($dados['temid'])){
                $strTemid = implode(',', $dados['temid']);
                $where .= " AND oe.temid IN ( {$strTemid} ) ";
            }
            if(!empty($dados['mpneid'])){
                $strMpneid = implode(',', $dados['mpneid']);
                $where .= " AND oepne.mpneid IN ( {$strMpneid} ) ";
            }
        }

        $objetivoEstrategico = $this->getObjetivosEstrategicos($where);
        $objetivoEstrategico = ($objetivoEstrategico ? $objetivoEstrategico : array());
        return $this->getOptions($objetivoEstrategico, array(), 'obeid');
    }

    public function getObjetivosEstrategicos($where = null) {
		$join = '';
		if(strpos($where, 'oepne') !== false ){
			$join = 'LEFT JOIN pde.ae_objetivoestrategicoxmetapne AS oepne ON oe.obeid = oepne.obeid';
		}
        $sql = "SELECT oe.obeid as codigo, oe.obenome as descricao
        			FROM pde.ae_objetivoestrategico  AS oe
        			{$join}
        			WHERE 1=1 {$where}
        			ORDER BY obenome ";
        return $this->_db->carregar($sql);
    }

}
