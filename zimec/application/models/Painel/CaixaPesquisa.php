<?php
class Model_Painel_CaixaPesquisa extends Zend_Db_Table
{
    protected $_schema = 'painel';
    protected $_name = 'caixapesquisa';

    public function getCaixasPesquisa()
    {


        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('caixapesquisa' => 'painel.caixapesquisa'), array('cxpid', 'cxpicone',
            new Zend_Db_Expr('(CASE WHEN caixapesquisa.regid is not null THEN (select regunidade from painel.regionalizacao r where caixapesquisa.regid = r.regid) ELSE cxpunidade END) as cxpunidade')
        ));
        $select->where("cxpid = 6 or cxpid = 8 or cxpid = 14 or cxpid = 3");

        return $this->fetchAll($select);
    }


    public function getHospitais($busca)
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('ent' => 'entidade.entidade'), array('ent.entid as codigo', 'UPPER(ent.entnome) as descricao', 'mun.mundescricao  as municipio'));
        $select->join(array('fen' => 'entidade.funcaoentidade'), 'fen.entid = ent.entid');
        $select->join(array('ende' => 'entidade.endereco'), 'ende.entid = ent.entid');
        $select->join(array('mun' => 'territorios.municipio'), 'ende.muncod = mun.muncod');
        $select->where("fen.funid = 16");
        $select->where("( UPPER(removeacento(ent.entsig)) like UPPER(removeacento('%{?}%')) OR UPPER(removeacento(ent.entnome)) like UPPER(removeacento('%{?}%')) )", $busca);
        $select->order('descricao');
        $select->limit('51');
        return $this->fetchAll($select);
    }

    public function getCampusProfissional($busca)
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('ent' => 'entidade.entidade'), array('ent.entid as codigo', 'UPPER(ent.entnome) as descricao', 'mun.mundescricao  as municipio'));
        $select->join(array('fen' => 'entidade.funcaoentidade'), 'fen.entid = ent.entid');
        $select->join(array('ende' => 'entidade.endereco'), 'ende.entid = ent.entid');
        $select->join(array('mun' => 'territorios.municipio'), 'ende.muncod = mun.muncod');
        $select->where("fen.funid = 17");
        $select->where("( UPPER(removeacento(ent.entsig)) like UPPER(removeacento('%{?}%')) OR UPPER(removeacento(ent.entnome)) like UPPER(removeacento('%{?}%')) )", $busca);
        $select->order('descricao');
        $select->limit('51');
        return $this->fetchAll($select);
    }

    public function getInstituicoesDePos($busca)
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('iepg' => 'painel.iepg'), array('iepid as codigo', 'UPPER(iepdsc) as descricao', 'mun.mundescricao  as municipio'));
        $select->join(array('mun' => 'territorios.municipio'), 'iepg.muncod = mun.muncod');
        $select->where("UPPER(removeacento(iepdsc)) like UPPER(removeacento('%{?}%'))", $busca);
        $select->order('descricao');
        $select->limit('51');
        return $this->fetchAll($select);
    }



    public function getCampusSuperior($busca)
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('ent' => 'entidade.entidade'), array('ent.entid as codigo', 'UPPER(ent.entnome) as descricao', 'mun.mundescricao  as municipio'));
        $select->join(array('fen' => 'entidade.funcaoentidade'), 'fen.entid = ent.entid');
        $select->join(array('ende' => 'entidade.endereco'), 'ende.entid = ent.entid');
        $select->join(array('mun' => 'territorios.municipio'), 'ende.muncod = mun.muncod');
        $select->where("fen.funid = 18");
        $select->where("( UPPER(removeacento(ent.entsig)) like UPPER(removeacento('%{?}%')) OR UPPER(removeacento(ent.entnome)) like UPPER(removeacento('%{?}%')) )", $busca);
        $select->order('descricao');
        $select->limit('51');
        return $this->fetchAll($select);
    }

    public function getEscolas($busca)
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('esc' => 'painel.escola'), array('esccodinep as codigo', 'UPPER(escdsc) as descricao', 'escmunicipio as municipio'));
        $select->where("UPPER(removeacento(escdsc)) like UPPER(removeacento('%{$busca}%'))");
        $select->order('codigo asc');
        $select->limit('20');
        return $this->fetchAll($select);
    }
    public function getEstados($busca)
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('esc' => 'territorios.estado'), array('estuf as codigo', 'UPPER(estdescricao) as descricao'));

        $select->where("UPPER(removeacento(estdescricao)) like UPPER(removeacento('%{$busca}%')) or UPPER(removeacento(estuf)) like UPPER(removeacento('%{$busca}%'))");
        $select->order('descricao');
        $select->limit('51');
        return $this->fetchAll($select);

    }

    public function getAcoes($busca)
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('acao' => 'painel.acao'), array('DISTINCT acaid as codigo', 'UPPER(acadsc) as descricao'));

        $select->where("UPPER(removeacento(acadsc)) like UPPER(removeacento('%{?}%'))", $busca);
        $select->where("acastatus = 'A'");
        $select->order('descricao');
        $select->limit('51');
        return $this->fetchAll($select);

    }

    public function getIndicadores($busca)
    {

        $sql ="select DISTINCT indi.indid as codigo, UPPER(indnome) as descricao from painel.indicador as indi
        where ( UPPER(removeacento(indnome)) like UPPER(removeacento('%{$busca}%')) OR indi.indid::text = removeacento('%{$busca}%') )
        and indi.indstatus = 'A' AND indi.indpublicado is true order by descricao limit 51";

        return $this->getDefaultAdapter()->query($sql)->fetchAll();

    }


    public function getPolos($busca)
    {
        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('pol' => 'painel.polo'), array('polid as codigo', 'UPPER(poldsc) as descricao', 'mun.mundescricao as municipio'));
        $select->join(array('mun' => 'territorios.municipio'), 'mun.muncod = pol.muncod');
        $select->where("UPPER(removeacento(poldsc)) like UPPER(removeacento('%{?}%'))", $busca);

        $select->order('descricao');
        $select->limit('51');
        return $this->fetchAll($select);

    }

    public function getMunicipios($busca)
    {


        $sql ="select DISTINCT muncod as codigo, UPPER(mundescricao) as descricao from
								territorios.municipio mun
							left join
								territorios.estado est ON est.estuf = mun.estuf
							where
								UPPER(removeacento(mundescricao)) like UPPER (removeacento('%{$busca}%'))


							ORDER BY
								descricao
							LIMIT 51 ";

        return $this->getDefaultAdapter()->query($sql)->fetchAll();




    }

    public function getPolo($busca) //Verificar Query
    {

        $select = $this->select();
        $select->setIntegrityCheck(false);
        $select->from(array('pol' => 'painel.polo'), array('polid as codigo', 'UPPER(poldsc) as descricao', 'mun.mundescricao as municipio'));
        $select->join(array('mun' => 'territorios.municipio'), 'mun.muncod = pol.muncod');
        $select->where("UPPER(removeacento(poldsc)) like UPPER(removeacento('%{$busca}%'))");

        $select->order('descricao');
        $select->limit('51');
        return $this->fetchAll($select);

    }



}
