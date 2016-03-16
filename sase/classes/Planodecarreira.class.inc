<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 10:24
 */

require_once APPRAIZ . "includes/library/simec/Listagem.php";

class Planodecarreira extends Modelo {

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.planocarreiraprofessor";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('pcpid');//,'muncod','stacod','docid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pcpid' => null,
        'estuf' => null,
        'muncod' => null,
        'usucpf' => null,
        'docid' => null,
        'pcpstatus' => null,
        'docid2' => null
    );

    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'pcpid',
        'estuf',
        'muncod',
        'usucpf',
        'docid',
        'pcpstatus',
        'docid2'
    );

    /**
     * Atributos da Tabela obrigatórios
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'estuf',
        'muncod'
    );

    protected $atributosObrigatoriosString = array(
        'estuf' => 'Estado',
        'muncod' => 'Município'
    );

    /**
     * Atributos Integer da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosInt = array(
        'pcpid' => null,
        'docid' => null,
        'docid2'
    );

    public function retornaPerfil($usucpf = null){
        $arrPerfis = arrayPerfil($usucpf);

        if(!in_array(PFLCOD_SASE_SUPER_USUARIO, $arrPerfis)){
            if(in_array(PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE, $arrPerfis)){
                return PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE;
            } elseif(in_array(PFLCOD_SASE_TECNICO_DIVAPE, $arrPerfis)){
                return PFLCOD_SASE_TECNICO_DIVAPE;
            }
        } else {
            return PFLCOD_SASE_SUPER_USUARIO;
        }
    }

    public function retornaResponsabilidades($usucpf = null){
        global $db;
        $usucpf = $usucpf == null ? $_SESSION['usucpf'] : $usucpf;
        $arrPerfis = $this->retornaPerfil($usucpf);
        $ret = array(
            'estuf' => null,
            'muncod' => null
        );

        switch($arrPerfis){
            case PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE:
                $sql = "select estuf From sase.usuarioresponsabilidade where usucpf = '{$usucpf}' and rpustatus = 'A'";
                $ret['estuf'] = $db->carregarColuna($sql, 'estuf');
                break;

            case PFLCOD_SASE_TECNICO_DIVAPE:
                $sql = "select distinct mun.estuf From sase.usuarioresponsabilidade rpu inner join territorios.municipio mun on mun.muncod = rpu.muncod where usucpf = '{$usucpf}' and rpustatus = 'A'";
                $ret['estuf'] = $db->carregarColuna($sql, 'estuf');

                $sql = "select muncod from sase.usuarioresponsabilidade where usucpf = '{$usucpf}' and rpustatus = 'A'";
                $ret['muncod'] = $db->carregarColuna($sql, 'muncod');
                break;
        }

        return $ret;
    }

    public function getLista(){
        $pflcod = $this->retornaPerfil();
        $cols = "pcp.estuf, mun.mundescricao, spc.spcdsc, spm.spmdsc";
        $where = " where 1=1 ";
        if($_POST['estuf']) { $where .= " and pcp.estuf  = '".$_POST['estuf']."'";  }
        if($_POST['muncod']){ $where .= " and pcp.muncod = '".$_POST['muncod']."'"; }
        if($_POST['esdid']) { $where .= " and doc.esdid  = ".$_POST['esdid'];  }
        if($_POST['esdid2']) { $where .= " and doc2.esdid  = ".$_POST['esdid2'];  }
        $join = "";
        switch($pflcod){
            case PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE:
                $join = "inner join sase.usuarioresponsabilidade rpu on rpu.estuf = mun.estuf and rpu.usucpf = '{$_SESSION['usucpf']}' and rpu.rpustatus = 'A'";
                break;

            case PFLCOD_SASE_TECNICO_DIVAPE:
                $join = "inner join sase.usuarioresponsabilidade rpu on rpu.muncod = mun.muncod and rpu.usucpf = '{$_SESSION['usucpf']}' and rpu.rpustatus = 'A'";
                break;
        }
        $sql = <<<DML
            select
                pcp.pcpid,
                {$cols}
            from sase.planocarreiraprofessor pcp
            inner join territorios.municipio mun on pcp.muncod = mun.muncod
            {$join}
            left join seguranca.usuario usu on usu.usucpf = pcp.usucpf
            left join workflow.documento doc on pcp.docid = doc.docid
            left join workflow.documento doc2 on pcp.docid2 = doc2.docid
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
            {$where}
            order by pcp.estuf, mun.mundescricao
DML;

        $list = new Simec_Listagem();
        $list->setQuery($sql)
             ->setCabecalho(array('Estado', 'Município', 'Situação Plano de Carreira', 'Situação Adequação'))
             ->addAcao('edit', array(
                 'func' => 'editarPlanodeCarreira',
                 'extra-params' => array(
                     'pcpid'
                 )
             ))
             ->setCampos(array('estuf', 'mundescricao', 'usunome'))
             ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function getLegenda(Array $estuf, $tpdid){
        global $db;
        $html = <<<HTML
            <div id="legendaMapaContainer">
            <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
            <ul>
HTML;
        $where = $estuf != '' && count($estuf) > 0 ? " where pcp.estuf in ('".implode("','", $estuf)."') " : "";
        $sql = "";

        switch ($tpdid){
            case '238':
                $sql = <<<DML
                    with temp_doc as (select
                                pcp.pcpid,
                                case
                                    when pcp.docid is null then (select spcid from sase.sitplancarprofessor where spcid = 1)
                                    else spc.spcid
                                end as spcid
                            from sase.planocarreiraprofessor pcp
                            left join workflow.documento doc on pcp.docid = doc.docid
                            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                            {$where}
                            order by pcpid)
                    select
                        spc.spcid,
                        spc.spcdsc,
                        spc.spccor,
                        spc.esdid,
                        (select count(*) from temp_doc where spcid = spc.spcid) as total
                    from sase.sitplancarprofessor spc
                    group by spc.spcid, spc.spcdsc, spc.spccor
                    order by spc.spcid
DML;
                break;
            case '240':
                $sql = <<<DML
                    with temp_doc as (select
                                pcp.pcpid,
                                case
                                    when pcp.docid2 is null then (select spmid from sase.sitplanomunicipio where spmid = 1)
                                    else spm.spmid
                                end as spmid
                            from sase.planocarreiraprofessor pcp
                            left join workflow.documento doc on pcp.docid2 = doc.docid
                            left join sase.sitplanomunicipio spm on spm.esdid = doc.esdid
                            {$where}
                            order by pcpid)
                    select
                        spm.spmid as spcid,
                        spm.spmdsc as spcdsc,
                        spm.spccor,
                        spm.esdid,
                        (select count(*) from temp_doc where spmid = spm.spmid) as total
                    from sase.sitplanomunicipio spm
                    group by spm.spmid, spm.spmdsc, spm.spccor
                    order by spm.spmid
DML;

                break;
        }
        $lis = $db->carregar($sql);
        foreach ($lis as $key => $value) {
            $total = $value['total'] != '' ? $value['total'] : 0;
            $html .= <<<HTML
                <li>
                    <table>
                        <tr>
                            <td>
                                <span style='background:{$value['spccor']};' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$total}</b>&nbsp;&nbsp;
                            </td>
                            <td>
                                {$value['spcdsc']}
                            </td>
                        </tr>
                    </table>
                </li>
HTML;
        }
        $html .= <<<HTML
            </ul>
        </div>
HTML;

        echo $html;
    }

}