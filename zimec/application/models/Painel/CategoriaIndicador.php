<?php
class Model_Painel_CategoriaIndicador extends Zend_Db_Table 
{
	protected $_schema = 'painel';
	protected $_name = 'caixapesquisa';
	
	public function getDetalhamentoEscolas($codInep)
    {
         $ano = date('Y');

        $sql = "SELECT ind.indid as codigo, ind.acaid as acaid, aca.acadsc as desccategoria, ind.indnome as descricao FROM painel.detalheseriehistorica dsh
            INNER JOIN painel.seriehistorica seh ON seh.sehid = dsh.sehid
            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
            INNER JOIN painel.indicador ind ON ind.indid = seh.indid
            INNER JOIN painel.acao aca ON aca.acaid = ind.acaid
            WHERE  ind.indstatus='A' AND indpublicado is true AND CASE WHEN indcumulativo = 'S' THEN (seh.sehstatus='A' OR seh.sehstatus='H')
                                           WHEN indcumulativo = 'N' THEN (seh.sehstatus='A')
                                           WHEN indcumulativo = 'A' THEN (dpedatainicio >= '{$ano}-01-01' AND dpedatainicio <= '{$ano}-12-31')
                                          END and dsh.dshcod='{$codInep}'
            GROUP BY aca.acadsc, ind.indnome, ind.indid, ind.acaid
            ORDER BY aca.acadsc, ind.indnome";

        return $this->getDefaultAdapter()->query($sql)->fetchAll();
    }

   	public function getDetalhamentoMunicipios($munCod)
    {
        $ano = date('Y');
        $sql = "SELECT ind.indid as codigo, ind.acaid as acaid, aca.acadsc as desccategoria, ind.indnome as descricao FROM painel.detalheseriehistorica dsh
        INNER JOIN painel.seriehistorica seh ON seh.sehid = dsh.sehid
        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
        INNER JOIN painel.indicador ind ON ind.indid = seh.indid
        INNER JOIN painel.acao aca ON aca.acaid = ind.acaid
        WHERE  ind.indstatus='A' AND indpublicado is true AND CASE WHEN indcumulativo = 'S' THEN (seh.sehstatus='A' OR seh.sehstatus='H')
							   WHEN indcumulativo = 'N' THEN (seh.sehstatus='A')
							   WHEN indcumulativo = 'A' THEN (dpedatainicio >= '{$ano}-01-01' AND dpedatainicio <= '{$ano}-12-31')
						      END and dsh.dshcodmunicipio='{$munCod}'
        GROUP BY aca.acadsc, ind.indnome, ind.indid, ind.acaid, ind.secid
        ORDER BY aca.acadsc, ind.indnome, ind.indid, ind.acaid, ind.secid";

        return $this->getDefaultAdapter()->query($sql)->fetchAll();
    }

	public function getDetalhamentoEstados($esccodinep)
    {
        $ano = date('Y');

        $sql = "SELECT ind.indid as codigo, ind.acaid as acaid, aca.acadsc as desccategoria, ind.indnome as descricao FROM painel.detalheseriehistorica dsh
                 INNER JOIN painel.seriehistorica seh ON seh.sehid = dsh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
                INNER JOIN painel.indicador ind ON ind.indid = seh.indid
                INNER JOIN painel.acao aca ON aca.acaid = ind.acaid
                WHERE  ind.indstatus='A' AND indpublicado is true AND CASE WHEN indcumulativo = 'S' THEN (seh.sehstatus='A' OR seh.sehstatus='H')
							   WHEN indcumulativo = 'N' THEN (seh.sehstatus='A')
							   WHEN indcumulativo = 'A' THEN (dpedatainicio >= '{$ano}-01-01' AND dpedatainicio <= '{$ano}-12-31')
                                              END and dsh.dshuf = '{$esccodinep}'
                  GROUP BY aca.acadsc, ind.indnome, ind.indid, ind.acaid, ind.secid
        ORDER BY aca.acadsc, ind.indnome, ind.indid, ind.acaid, ind.secid";



        return $this->getDefaultAdapter()->query($sql)->fetchAll();
    }

}
