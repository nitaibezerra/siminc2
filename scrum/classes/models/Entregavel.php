<?php

class Model_Entregavel extends Abstract_Model
{

    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'scrum';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'entregavel';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    public function getProgramSubProgramByStory($estid)
    {
        $sql = "SELECT prg.prgid , subprg.subprgid , est.estid , ent.entid --, ent.entdsc , ent.enthrsexec 
                FROM scrum.entregavel ent
                LEFT JOIN scrum.estoria est ON est.estid = ent.estid
                LEFT JOIN scrum.subprg subprg ON subprg.subprgid = est.subprgid
                LEFT JOIN scrum.programa prg ON prg.prgid = subprg.prgid 
                WHERE est.estid = {$estid}";
        
        return $this->_db->pegaLinha($sql);
    }
    
    /**
     * Montando a entidade
     * 
     */
    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['entid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['entdsc'] = array('value' => '', 'type' => 'text', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
        $this->entity['enthrsexec'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
        $this->entity['entordsprint'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
        $this->entity['estid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['entstid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['usucpfsol'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '11', 'contraint' => '');
        $this->entity['usucpfresp'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => '');
        $this->entity['entdtcad'] = array('value' => '', 'type' => 'time without time zone', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
        $this->entity['sptid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['dmdid'] = array('value' => '', 'type' => 'bigint', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
    }

    public function getAllPostit($prgid, $idSprint)
    {
        $sqlProximaSptrin = "SELECT 
                        prg.prghrsprint,
                        ent.enthrsexec,
                        ent.entordsprint,
                        ent.entdsc,
                        ent.entid,
                        spg.subprgdsc,
                        spg.subprgcolor,
                        est.esttitulo,
                        ent.sptid,
                        COALESCE(usu.usunome, '') AS usucpfresp_dsc,
                        ent.entstid
                FROM scrum.entregavel ent
                    INNER JOIN scrum.estoria est USING(estid)
                    INNER JOIN scrum.subprg spg USING(subprgid)
                    INNER JOIN scrum.programa prg USING(prgid)
                    LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                    LEFT JOIN scrum.sprint spt ON (spt.sptid = ent.sptid)
                WHERE prgid = {$prgid}
                AND spt.sptid = {$idSprint}
                ORDER BY ent.entordsprint ASC, ent.entid DESC";
        $entregaveisProximaSprint = $this->_db->carregar($sqlProximaSptrin);
        
        return ($entregaveisProximaSprint)? $entregaveisProximaSprint : array();
    }
    
    public function getAllPostitBackLog($prgid)
    {
        $sqlEntregaveisBackLog = "SELECT 
                                        prg.prghrsprint,
                                        ent.enthrsexec,
                                        ent.entordsprint,
                                        ent.entdsc,
                                        ent.entid,
                                        spg.subprgdsc,
                                        spg.subprgcolor,
                                        est.esttitulo,
                                        ent.sptid,
                                        COALESCE(usu.usunome, '') AS usucpfresp_dsc,
                                        ent.entstid
                                FROM scrum.entregavel ent
                                    INNER JOIN scrum.estoria est USING(estid)
                                    INNER JOIN scrum.subprg spg USING(subprgid)
                                    INNER JOIN scrum.programa prg USING(prgid)
                                    LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
                                    --LEFT JOIN scrum.sprint ON spt (spt.sptid = ent.sptid)
                                WHERE prgid = {$prgid}
                                AND sptid IS NULL
                                    ORDER BY ent.entordsprint ASC, ent.entid DESC";
        $entregaveisBackLog = $this->_db->carregar($sqlEntregaveisBackLog);
        return $entregaveisBackLog;
    }
    
    
    
    
    
    
    
    
    
    public function carregarUsuarioPorCPF($usucpf)
    {
        if ($usucpf)
        {
            $sql = "SELECT * FROM seguranca.usuario WHERE usucpf = '{$usucpf}'";
            return $this->_db->pegaLinha($sql);
        } else
        {
            return false;
        }
    }

    public function pegarTudoPorEstoria($estId)
    {
        $sqlSidId = "SELECT * 
                            FROM scrum.estoria est
                            LEFT JOIN scrum.subprg sub ON (sub.subprgid = est.subprgid)
                            --LEFT JOIN demandas.sididassociasisid sid ON (sid.sisid = sub.sisid) 
                            WHERE est.estid = {$estId}";
        return $this->_db->pegaLinha($sqlSidId);
    }

    public function getAllByProgramAndSprint($prgid, $sptid)
    {
        $sql = "
SELECT 
        spg.subprgid,
        spg.subprgdsc,
        spg.subprgcolor,
        est.estid,
        est.esttitulo,
        sta.entstid,
        sta.entstdsc,
        prg.prghrsprint,
        ent.enthrsexec,
        ent.entordsprint,
        ent.entdsc,
        ent.entid,
        COALESCE(usu.usunome, '') AS usucpfresp_dsc
FROM scrum.entregavel ent
    INNER JOIN scrum.estoria est USING(estid)
    INNER JOIN scrum.subprg spg USING(subprgid)
    INNER JOIN scrum.programa prg USING(prgid)
    LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
    LEFT JOIN scrum.entstatus sta on (sta.entstid = ent.entstid)
    LEFT JOIN scrum.sprint spt ON (spt.sptid = ent.sptid)
WHERE prgid = {$prgid}
    AND spt.sptid = {$sptid}

    ORDER BY spg.subprgdsc, est.esttitulo, ent.entordsprint ASC, ent.entid DESC";

        $result = $this->_db->carregar($sql);
        if ($result) {
            $newResult = array();
            foreach ($result as $item) {
                $newResult[$item['subprgid']]['nome_subprograma'] = $item['subprgdsc'];
                $newResult[$item['subprgid']]['filhos_subprograma'][$item['estid']]['nome_estoria'] = $item['esttitulo'];

                if ($item['usucpfresp_dsc']) {
                    $nomeResponsavel = explode(' ', $item['usucpfresp_dsc']);
                } else {
                    $nomeResponsavel = '';
                }

                $newResult[$item['subprgid']]['filhos_subprograma'][$item['estid']]['filhos_estoria'][$item['entstid']][] = array('usucpfresp_dsc' => $nomeResponsavel[0], 'entdsc' => $item['entdsc'], 'prghrsprint' => $item['prghrsprint'], 'enthrsexec' => $item['enthrsexec'], 'subprgcolor' => $item['subprgcolor'], 'entid' => $item['entid']);
                //            $listaNova[$item['subprgid']]['filhos_subprograma'][$item['estid']]['filhos_estoria'][] =  $item;
            }
            
            
            $result = $newResult;
        } 
        
        return $result;
    }
        public function retornaDadosParaContagemDeHoras($prgid)
    {
        $sql = "
        SELECT 
            spg.subprgid,
            spg.subprgdsc,
            est.estid,
            est.esttitulo,
            sta.entstid,
            count(0) as  demandas,
            sum(ent.enthrsexec) as totalhoras
        FROM scrum.entregavel ent
            INNER JOIN scrum.estoria est USING(estid)
            INNER JOIN scrum.subprg spg USING(subprgid)
            INNER JOIN scrum.programa prg USING(prgid)
            LEFT JOIN seguranca.usuario usu ON (ent.usucpfresp = usu.usucpf)
            LEFT JOIN scrum.entstatus sta on (sta.entstid = ent.entstid)
            LEFT JOIN scrum.sprint spt ON (spt.sptid = ent.sptid)
        WHERE prgid = {$prgid}
            AND sta.entstid <> 5
        GROUP BY 1,2,3,4,5
        ORDER BY 2,4";

        $result = $this->_db->carregar($sql);
         
        if ($result) {
            $newResult = array();
            foreach ($result as $item) {
                $newResult[$item['subprgid']]['nome_subprograma'] = $item['subprgdsc'];
                $newResult[$item['subprgid']]['filhos_subprograma'][$item['estid']]['nome_estoria'] = $item['esttitulo'];
                $newResult[$item['subprgid']]['filhos_subprograma'][$item['estid']]['status'][$item['entstid']]['demandas'] = $item['demandas'];
                $newResult[$item['subprgid']]['filhos_subprograma'][$item['estid']]['status'][$item['entstid']]['totalhoras'] = $item['totalhoras'];
            }
            $result = $newResult;
        } 
        
        return $result;
    }
      
}

    