<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 10:24
 */

require_once APPRAIZ . "includes/library/simec/Listagem.php";

class PlanodecarreiraEstado extends Modelo {

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.planocarreiraprofessorestado";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('pceid');//,'muncod','stacod','docid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pceid' => null,
        'estuf' => null,
        'usucpf' => null,
        'docid' => null,
        'pcestatus' => null,
        'docid2' => null
    );

    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'pceid',
        'estuf',
        'usucpf',
        'docid',
        'pcestatus',
        'docid2'
    );

    /**
     * Atributos da Tabela obrigatórios
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'estuf'
    );

    protected $atributosObrigatoriosString = array(
        'estuf' => 'Estado'
    );

    /**
     * Atributos Integer da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosInt = array(
        'pceid' => null,
        'docid' => null,
        'docid2' => null
    );

    public function getLista(){
        $cols = "est.estdescricao, spc.spcdsc, spm.spmdsc";
        $where = " where 1=1 ";
        if($_POST['estuf']) { $where .= " and pce.estuf  = '".$_POST['estuf']."'";  }
        if($_POST['esdid']) { $where .= " and doc.esdid  = ".$_POST['esdid'];  }
        if($_POST['esdid2']) { $where .= " and doc2.esdid  = ".$_POST['esdid2'];  }
        $sql = <<<DML
            select
                pce.pceid,
                {$cols}
            from sase.planocarreiraprofessorestado pce
            inner join territorios.estado est on pce.estuf = est.estuf
            left join seguranca.usuario usu on usu.usucpf = pce.usucpf
            left join workflow.documento doc on pce.docid = doc.docid
            left join workflow.documento doc2 on pce.docid2 = doc2.docid
            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
            left join sase.sitplanomunicipio spm on spm.esdid = doc2.esdid
            {$where}
            order by est.estdescricao
DML;

        $list = new Simec_Listagem();
        $list->setQuery($sql)
            ->setCabecalho(array('Estado', 'Situação Plano de Carreira', 'Situação Adequação'))
            ->addAcao('edit', array(
                'func' => 'editarPlanodeCarreira',
                'extra-params' => array(
                    'pceid'
                )
            ))
            //->setCampos(array('estdescricao', 'mundescricao', 'usunome'))
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function getLegenda(Array $estuf, $tpdid){
        global $db;
        if(count($estuf) != 0) {
            $html = <<<HTML
                <div id="legendaMapaContainer">
                <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
                <ul>
HTML;
        } else {
            $html = <<<HTML
                <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
                    <div id="tituloLegenda">
                        <div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5></div>
                        <div style="float:right;"></div>
                        <div style="clear:both;height:1px;">&nbsp;</div>
                    </div>
                    <ul>
HTML;

        }
        $where = $estuf != '' && count($estuf) > 0 ? " where pce.estuf in ('".implode("','", $estuf)."') " : "";
        $sql = "";

        switch ($tpdid){
            case '238':
                $sql = <<<DML
                    with temp_doc as (select
                                pce.pceid,
                                case
                                    when pce.docid is null then (select spcid from sase.sitplancarprofessor where spcid = 1)
                                    else spc.spcid
                                end as spcid
                            from sase.planocarreiraprofessorestado pce
                            left join workflow.documento doc on pce.docid = doc.docid
                            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                            {$where}
                            order by pceid)
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
                                pce.pceid,
                                case
                                    when pce.docid2 is null then (select spmid from sase.sitplanomunicipio where spmid = 1)
                                    else spm.spmid
                                end as spmid
                            from sase.planocarreiraprofessorestado pce
                            left join workflow.documento doc on pce.docid2 = doc.docid
                            left join sase.sitplanomunicipio spm on spm.esdid = doc.esdid
                            {$where}
                            order by pceid)
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