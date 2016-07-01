<?php
class Model_Demandas_Demanda extends Zend_Db_Table
{
    protected $_schema = 'demandas';
    protected $_name   = 'demanda';

    public function getDemandasByCPF($cpf) 
    {
    	$sql = "SELECT sum(atrasadas) as atrasadas, sum(nodia) as nodia, sum(avencer) as avencer, sum(pausadas) as pausadas from
                (
                SELECT count(*) as atrasadas, 0 as nodia, 0 as avencer, 0 as pausadas
                    FROM
                        demandas.demanda as d
                    LEFT JOIN
                        workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN
                        workflow.estadodocumento ed ON ed.esdid = doc.esdid
                    WHERE
                        d.usucpfexecutor = '{$cpf}'
                        AND d.usucpfdemandante is not null
                        AND d.dmdstatus = 'A'
                        AND ed.esdstatus = 'A'
                        AND doc.esdid in (91,92,107,108)
                        AND d.dmddatafimprevatendimento < CURRENT_DATE
                        and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

                    union all

                    SELECT 0, count(*), 0, 0
                    FROM
                        demandas.demanda as d
                    LEFT JOIN
                        workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN
                        workflow.estadodocumento ed ON ed.esdid = doc.esdid
                    WHERE
                        d.usucpfexecutor = '{$cpf}'
                        AND d.usucpfdemandante is not null
                        AND d.dmdstatus = 'A'
                        AND ed.esdstatus = 'A'
                        AND doc.esdid in (91,92,107,108)
                        AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') = to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                        and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

                    union all

                    SELECT 0, 0, count(*), 0
                    FROM
                        demandas.demanda as d
                    LEFT JOIN
                        workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN
                        workflow.estadodocumento ed ON ed.esdid = doc.esdid
                    WHERE
                        d.usucpfexecutor = '{$cpf}'
                        AND d.usucpfdemandante is not null
                        AND d.dmdstatus = 'A'
                        AND ed.esdstatus = 'A'
                        AND doc.esdid in (91,92,107,108)
                        AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') > to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                        and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

                    union all

                    SELECT 0, 0, 0, count(*)
                    FROM
                        demandas.demanda as d
                    LEFT JOIN
                        workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN
                        workflow.estadodocumento ed ON ed.esdid = doc.esdid
                    WHERE
                        d.usucpfexecutor = '{$cpf}'
                        AND d.usucpfdemandante is not null
                        AND d.dmdstatus = 'A'
                        AND ed.esdstatus = 'A'
                        AND doc.esdid in (91,92,107,108)
                        AND d.dmdid in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ) as total";
                
                
    	$select = $this->getAdapter()->query($sql);
    	
    	return $this->fetchRow($select);
    }
}
