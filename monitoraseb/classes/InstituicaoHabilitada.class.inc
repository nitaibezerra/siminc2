<?php

include_once APPRAIZ . "www/monitoraseb/geral/workflow.php";

class InstituicaoHabilitada extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitoraseb.instituicaohabilitada";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ihdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
        'cmtid' => null,
        'entid' => null,
        'docid' => null,
        'ihdstatus' => null,
        'ihdjststatus' => null
     );

    protected $arrPesquisa = null;

    public function listarInstituicao($coonid, $sbaid, $usucpf){
        $sisid = $_SESSION['sisid'];

        $where = "where";
        if($coonid){
            $where .= " c.coonid = $coonid and ";
        }
        if($sbaid){
            $where .= " sc.sbaid = $sbaid and ";
        }

        $sql = "select distinct '<center>
                                     <a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/instituicao/cadastrarinstituicao&acao=A&ihdid='||ih.ihdid||'\'\">
                                         <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                                     </a>
                                     <a style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){excluirInstituicao(\''||ih.ihdid||'\');}\">
                                         <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                                     </a>
                                 </center>' as acao
                      ,c.coosigla || ' - ' || c.coodsc as coordenacao
                      ,cm.cmtdsc as cursomestre
                      ,e.entnome as instituicao
                  from monitoraseb.instituicaohabilitada ih
                 inner join entidade.entidade e
                    on ih.entid = e.entid
                 inner join monitoraseb.cursomestre cm
                    on ih.cmtid = cm.cmtid
                 inner join monitoraseb.subacaocoordenacao sc
                    on cm.scoid = sc.scoid
                 inner join monitoraseb.coordenacao c
                    on sc.coonid = c.coonid
                 inner join monitora.pi_subacao s
                    on sc.sbaid = s.sbaid
                 inner join monitoraseb.usuarioresponsabilidade ur
                    on sc.coonid = ur.coonid
                   and sc.usucpf = ur.usucpf
                 inner join seguranca.perfil p
                    on p.pflcod = ur.pflcod
                $where p.pflnivel = 3
                   and ((ur.usucpf = '$usucpf'
                   and not exists (select pu1.usucpf
                                    from seguranca.perfilusuario pu1
                                   inner join seguranca.perfil p1
                                      on p1.pflcod = pu1.pflcod
                                   where p1.pflnivel = 1
                                     and pu1.usucpf = ur.usucpf
                                     and p1.sisid = $sisid)
                   and not exists (select ur1.usucpf
                                    from monitoraseb.usuarioresponsabilidade ur1
                                   inner join seguranca.perfil p1
                                      on p1.pflcod = ur1.pflcod
                                   where p1.pflnivel = 2
                                     and ur1.rpustatus = 'A'
                                     and ur1.usucpf = ur.usucpf
                                     and p1.sisid = $sisid))
                   or exists (select ur2.usucpf
                                from monitoraseb.usuarioresponsabilidade ur2
                               inner join seguranca.perfil p2
                                  on p2.pflcod = ur2.pflcod
                               where p2.pflnivel = 2
                                 and ur2.rpustatus = 'A'
                                 and ur2.usucpf = '$usucpf'
                                 and ur.coonid = ur2.coonid)
                   or exists (select pu2.usucpf
                                from seguranca.perfilusuario pu2
                               inner join seguranca.perfil p3
                                  on p3.pflcod = pu2.pflcod
                               where p3.pflnivel = 1
                                 and pu2.usucpf = '$usucpf'
                                 and p3.sisid = $sisid))
                order by cursomestre, instituicao, coordenacao";

        $cabecalho = array("Ações","Coordenação","Curso Mestre","Instituição");

        $this->monta_lista($sql,$cabecalho,100,50,"N","center","N","",array( '5%', '25%', '25%', '45%' ));
    }

    public function pesquisarInstituicao($entnome){
        $sql = "select distinct '<center>
                                     <a style=\"cursor:pointer;\" onclick=\"selecionarInstituicao(\''||e.entid||'\');\">
                                         <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                                     </a>
                                 </center>' as acao
                      ,e.entnome
                  from entidade.entidade e
                 inner join entidade.funcaoentidade fe
                    on e.entid = fe.entid
                 inner join entidade.funcao f
                    on fe.funid = f.funid
                 where e.entstatus = 'A'
                   and fe.fuestatus = 'A'
                   and f.funstatus = 'A'
                   and f.funtipo = 'J'
                   and f.funid in (3,4,12,10,11,16,18,56)
                   and e.entnome ilike '%$entnome%'
                 order by e.entnome";

        $cabecalho = array("Ações","Instituição");

        $this->monta_lista($sql,$cabecalho,30,50,"N","center","N","",array( '5%', '95%' ));
    }

    public function consultarInstituicao($entid){
        $sql = "select distinct e.entid
                      ,e.entnome
                      ,coalesce(e.entnumcpfcnpj,'') as entnumcpfcnpj
                      ,coalesce(e.entemail,'') as entemail
                      ,d.endlog || case when endnum is not null and endnum <> '' then ', ' || endnum else '' end || case when endbai is not null then ' - ' || endbai else '' end as entendlog
                      ,coalesce(m.regcod,'') as entregcod
                      ,coalesce(m.mundsc,'') as entmundsc
                      ,coalesce(d.endcep,'') as entendcep
                      ,'' as entdscsite
                      ,coalesce(e.entnumdddcomercial || e.entnumcomercial,'') as entnumcomercial
                      ,coalesce(r.entnome,'') as dlcnome
                      ,coalesce(r.entnumcpfcnpj,'') as dlccpf
                      ,coalesce(r.entnumdddcomercial || r.entnumcomercial,'') as dlcnumcomercial
                      ,coalesce(r.entemail,'') as dlcemail
                  from entidade.entidade e
                  left join entidade.entidadeendereco ee
                 inner join entidade.endereco d
                 inner join public.municipio m
                    on (d.muncod = m.muncod)
                    on (ee.endid = d.endid)
                    on (e.entid = ee.entid
                   and ee.tpeid = 1)
                  left join entidade.funentassoc fea
                 inner join entidade.funcaoentidade rfe
                 inner join entidade.entidade r
                    on (rfe.entid = r.entid
                   and r.entstatus = 'A')
                    on (fea.fueid = rfe.fueid
                   and rfe.fuestatus = 'A'
                   and rfe.funid = 21)
                    on (e.entid = fea.entid)
                 where e.entstatus = 'A'
                   and e.entid = $entid";

        return $this->carregar($sql);
    }

    public function validarPermissaoGravar($usucpf, $ihdid){
        $permissoes = array();
        $sisid = $_SESSION['sisid'];

        $sql = "select distinct u.usucpf
                  from seguranca.usuario u
                 inner join seguranca.perfilusuario pu
                    on u.usucpf = pu.usucpf
                 inner join seguranca.perfil p
                    on p.pflcod = pu.pflcod
                 where u.usucpf = '$usucpf'
                   and p.sisid = $sisid
                   and (exists (select sc.usucpf
                                  from monitoraseb.instituicaohabilitada ih
                                 inner join monitoraseb.cursomestre cm
                                    on cm.cmtid = ih.cmtid
                                 inner join monitoraseb.subacaocoordenacao sc
                                    on cm.scoid = sc.scoid
                                 inner join monitoraseb.usuarioresponsabilidade ur
                                    on sc.coonid = ur.coonid
                                 inner join workflow.documento d
                                    on ih.docid = d.docid
                                 where ih.ihdid = $ihdid
                                   and u.usucpf = ur.usucpf
                                   and ur.rpustatus = 'A'
                                   and ((p.pflnivel = 3
                                   and (d.esdid = ".WF_ESTADO_PREENCHIMENTO."
                                    or d.esdid = ".WF_ESTADO_DEVOLVIDO_AO_COORDENADOR_TECNICO."))
                                    or (p.pflnivel = 2
                                   and d.esdid = ".WF_ESTADO_AVALIACAO."))))";
        $gravar = $this->pegaUm($sql);

        $permissoes['gravar'] = ($gravar?'S':'N');

        $sql = "select p.pflnivel
                  from seguranca.usuario u
                 inner join seguranca.perfilusuario pu
                    on u.usucpf = pu.usucpf
                 inner join seguranca.perfil p
                    on p.pflcod = pu.pflcod
                 inner join monitoraseb.usuarioresponsabilidade ur
                    on u.usucpf = ur.usucpf
                 where ur.rpustatus = 'A'
                   and u.usucpf = '$usucpf'
                   and p.sisid = $sisid
                   and p.pflnivel = 2
                   and ur.coonid in (select sc.coonid
                                       from monitoraseb.subacaocoordenacao sc
                                 inner join monitoraseb.cursomestre cm
                                   on cm.scoid = sc.scoid
                                 inner join monitoraseb.instituicaohabilitada ih
                                    on cm.cmtid = ih.cmtid
                                 where ih.ihdid = $ihdid)";

        $nivel = $this->pegaUm($sql);

        $permissoes['nivel'] = ($nivel?2:3);

        return $permissoes;
    }

    public function consultarInstituicaoHabilitada($ihdid){
        $sql = "SELECT sc.usucpf
                      ,sc.coonid
                      ,sc.sbaid
                      ,ih.cmtid
                      ,ih.entid
                      ,ih.docid
                      ,ih.ihdstatus
                      ,ih.ihdjststatus
                      ,ih.ihdclcnome
                      ,ih.ihdclccpf
                      ,ih.ihdclcfuncao
                      ,ih.ihdclcemail
                      ,ih.ihdclctelefone
                      ,ih.ihdid
                  FROM monitoraseb.instituicaohabilitada ih
                 inner join monitoraseb.cursomestre cm
                    on ih.cmtid = cm.cmtid
                 inner join monitoraseb.subacaocoordenacao sc
                    on cm.scoid = sc.scoid
                 where ih.ihdid = $ihdid";

        return $this->pegaLinha(iconv("UTF-8", "ISO-8859-1", $sql));
    }

    public function salvar($post){
        extract($post);

        if($cmtid==null && $cmtid_disable!=null){
            $cmtid = $cmtid_disable;
        }
        if($sbaid==null && $sbaid_disable!=null){
            $sbaid = $sbaid_disable;
        }

        $sql = "select count(*)
                  from monitoraseb.instituicaohabilitada ih
                 where ih.entid = $entid
                   and ih.cmtid = $cmtid";

        if($ihdid){
            $sql .= " and ih.ihdid != $ihdid";
        }

        $count = $this->pegaUm($sql);

        if($count > 0){
            echo "<script>alert('Já existe um cadastro associando a instituição selecionada ao curso mestre selecionado!');</script>";
            return false;
        }else{

            if(!$docid){
                $docid = wf_cadastrarDocumento(WORKFLOW_MONITORAMENTO_SEB, "Fluxo da Instituição Habilitada - ID $ihdid");
            }

            if($ihdid){
                $sql = "update monitoraseb.instituicaohabilitada
                           set cmtid=$cmtid, entid=$entid, docid=$docid, ihdstatus='$ihdstatus'
                              ,ihdjststatus='$ihdjststatus', ihdclcnome='$ihdclcnome', ihdclccpf='".pega_numero($ihdclccpf)."'
                              ,ihdclcfuncao='$ihdclcfuncao', ihdclcemail='$ihdclcemail', ihdclctelefone='$ihdclctelefone'
                         where ihdid=$ihdid returning ihdid";
            } else{
                $sql = "insert into monitoraseb.instituicaohabilitada
                            (cmtid, entid, docid, ihdstatus, ihdjststatus, ihdclcnome
                            ,ihdclccpf, ihdclcfuncao, ihdclcemail, ihdclctelefone)
                        values ($cmtid, $entid, $docid, '$ihdstatus', '$ihdjststatus', '$ihdclcnome'
                               ,'".pega_numero($ihdclccpf)."', '$ihdclcfuncao', '$ihdclcemail', '$ihdclctelefone') returning ihdid";
            }

            $ihdid = $this->pegaUm($sql);

            $this->commit();

            direcionar("?modulo=principal/instituicao/cadastrarinstituicao&acao=A&ihdid=$ihdid","Operação realizada com sucesso!");

            return true;
        }
    }
}