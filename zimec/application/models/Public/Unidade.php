<?php
class Model_Public_Unidade extends Zend_Db_Table
{
    protected $_schema = 'public';
    protected $_name   = 'unidade';

    public function getUnidadesAtivas()
    {
        $rowSet = $this->fetchAll(array('unistatus = ?'=>'A', 'unitpocod = ?'=>'U'), array('orgcod', 'unidsc'));

        $aUnidades = array();
        foreach ($rowSet as $row) {
            $aUnidades[$row->unicod] = $row->unidsc;
        }
        return $aUnidades;
    }
}
