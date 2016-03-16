<?php
class Model_Public_Exercicio extends Zend_Db_Table
{
    protected $_name = 'programacaoexercicio';

	public function getExercicios($filtro)
    {
    	if ($filtro->sisdiretorio && $filtro->sisexercicio)
    	{
    		$select = $this->select();
    		$select->setIntegrityCheck(false);
    		$select->from(array('table' => 'pg_stat_user_tables'), array('schemaname', 'relname'));
    		$select->where("schemaname = ?", $filtro->sisdiretorio);
    		$select->where("relname = 'programacaoexercicio'");
    		$select->order('relname');
    		
    		$tabelaExercicio = $this->fetchAll($select)->toArray();
    		
    		if ($tabelaExercicio) {
    			$select = $this->select();
    			$select->setIntegrityCheck(false);
    			$select->from(array('table' => "{$filtro->sisdiretorio}.programacaoexercicio"), array('prsano as codigo', 'prsano as descricao', 'prsexerccorrente', 'prsexercicioaberto'));
    			$select->order('prsano');
    			
        		return $this->fetchAll($select);
    		}
		}
    }
}
