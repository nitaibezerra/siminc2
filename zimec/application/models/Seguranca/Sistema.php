<?php
class Model_Seguranca_Sistema extends Zend_Db_Table
{
    protected $_schema = 'seguranca';
    protected $_name   = 'sistema';
    
    public function carregarPorDiretorio($diretorio) {
    	$select = $this->select ();
    	$select->setIntegrityCheck ( false );
    	$select->from (array('sistema' => 'seguranca.sistema'));
    	$select->where("sisdiretorio like '%{$diretorio}%'");
    	$select->order('sisdsc');

    	return $this->fetchRow($select);
    }
    
    public function getAll() {
    	$select = $this->select ();
    	$select->setIntegrityCheck ( false );
    	$select->from (array('sistema' => 'seguranca.sistema'));
    	$select->order('sisdsc');
    
    	return $this->fetchAll( $select );
    }
    
}