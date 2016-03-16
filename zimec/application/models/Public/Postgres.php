<?php
class Model_Public_Postgres extends Zend_Db_Table
{
    protected $_name = 'pg_stat_activity';
    protected $_primary = 'pid';
    
    public function getTempoQuery()
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('activity' => 'pg_stat_activity'), array("date_part('epoch', now() - query_start)::integer as segundos"));
        $select->where("query not like '%IDLE%'");
        $select->where("query not ilike '%COPY%'");
        $select->where("query not ilike '%VACUUM%'");
        $select->where("date_part('epoch', now() - query_start)::integer < 86400");
         
        return $this->fetchRow($select);
    }
    
    public function getQuantidadeQuery()
    {
    	$select = $this->select();
    	$select->setIntegrityCheck(false);
    	$select->from(array('activity' => 'pg_stat_activity'), array("pid"));
    	$select->where("query not like '%IDLE%'");
    	$select->where("date_part('epoch', now() - query_start)::integer < 86400");
    	 
    	return $this->fetchAll($select);
    }
}
