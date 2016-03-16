<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "www/monitoraseb/geral/workflow.php";

class Subacao extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitoraseb.subacaocoordenacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "scoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
        'coonid' => null,
        'sbaid' => null,
        'usucpf' => null,
        'docid' => null,
        'scpobjgeral' => null,
        'scpobjespecifico' => null,
        'scpdsc' => null
     );

    protected $arrPesquisa = null;

    public function comboCoordenadorTecnico($usucpf, $carregarSubordinados=false, $default=null, $exibeSubacao=false){
        $sisid = $_SESSION['sisid'];

        if($carregarSubordinados){
            $sql = "select distinct u.usucpf as codigo, u.usunome as descricao
                      from seguranca.usuario u
                     inner join monitoraseb.usuarioresponsabilidade ur
                        on u.usucpf = ur.usucpf
                     inner join seguranca.perfil p
                        on p.pflcod = ur.pflcod
                     where ur.rpustatus = 'A'
                       and p.pflnivel = 3
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
                                      and p3.sisid = $sisid))";
        } else{
            $sql = "select u.usucpf as codigo, u.usunome as descricao
                      from seguranca.usuario u
                     where u.usucpf = '$usucpf'";
        }

        $lista = $this->carregar($sql);

        $this->monta_combo("usucpf",$sql,(count($lista) > 1 ? "S" : "N"),(count($lista) > 1 ? "Selecione..." : ""),(count($lista) > 1 ? ($exibeSubacao ? 'listar_coordenacao' : "listar_coordenacao_sem_subacao") : ""),"","","200","N","usucpf","",($default ? $default : $usucpf));
    }

    public function listaDocumentos($scoid, $permiteGravar){
        if($scoid!=null){
            $excluir = ($permiteGravar!="S")?"''":"'<center><a style=\" cursor: pointer;\"onclick=\"if(confirm(\'Deseja excluir o registro?\')){javascript:atualizaDocumentoAnexo(' || a.arqid || ', \'Subacao\', $scoid, \'$permiteGravar\');}\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'";
            $sql = "select $excluir as acao
                          ,to_char(a.arqdata,'DD/MM/YYYY') as arqdata
                          ,td.tiddsc
                          ,a.arqdescricao
                          ,'<a style=\"cursor: pointer; color: blue;\" onclick=\"javascript:abrirAnexo(' || a.arqid || ');\">' || a.arqnome || '.'|| a.arqextensao ||'</a>'
                          ,round(a.arqtamanho/1024.0, 2) || ' KB' as arqtamanho
                          ,u.usunome
                      from public.arquivo a
                     inner join monitoraseb.subacaoanexo saa
                        on a.arqid = saa.arqid
                     inner join monitoraseb.tipodocumento td
                        on saa.tidid = td.tidid
                     inner join seguranca.usuario u
                        on u.usucpf = a.usucpf
                     inner join monitoraseb.subacaocoordenacao sc
                        on saa.sbaid = sc.sbaid
                     where a.arqstatus = 'A'
                       and sc.scoid = $scoid";

        }

        $cabecalho = array("Ação",
                               "Data Inclusão",
                               "Tipo Documento",
                               "Descrição Documento",
                               "Nome Arquivo",
                               "Tamanho",
                               "Responsável");
        $this->monta_lista_simples($sql,$cabecalho,50,10,'N','95%','N',false,false,false,true);
    }

    public function comboPublicoAlvo($scoid, $permiteGravar=false){
        global $funid;

        $sql = "select distinct f.funid as codigo
                      ,f.fundsc as descricao
                  from entidade.funcao f
                 inner join monitoraseb.subacaopublicoalvo spa
                    on f.funid = spa.funid
                 inner join monitoraseb.subacaocoordenacao sc
                    on spa.sbaid = sc.sbaid
                 where f.funstatus = 'A'
                   and f.funtipo = 'F'
                   and sc.scoid = $scoid
                 order by descricao";

        $funid = $this->carregar($sql);

        $sql = "select f.funid as codigo
                      ,f.fundsc as descricao
                  from entidade.funcao f
                 where f.funstatus = 'A'
                   and f.funtipo = 'F'
                 order by descricao";

        combo_popup('funid', $sql, 'Selecione o(s) Público(s)-alvo', '360x460', 0, array(), '', (($permiteGravar)?'S':'N'));
    }

    public function consultarSubAcao($scoid){
        $sql = "select scoid
                      ,coonid
                      ,sbaid
                      ,usucpf
                      ,docid
                      ,scpobjgeral
                      ,scpobjespecifico
                      ,scpdsc
                  from monitoraseb.subacaocoordenacao
                 where scoid = $scoid";

        return $this->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );
    }

    public function comboCoordenacao($coonid, $usucpf=null, $acao=""){
        $sisid = $_SESSION['sisid'];

        if(!$coonid && !$usucpf){
            $usucpf = $_SESSION['usucpf'];
        }

        if($usucpf){
            $sql = "select distinct c.coonid as codigo, c.coosigla || ' - ' || c.coodsc as descricao
                      from monitoraseb.coordenacao c
                     inner join monitoraseb.usuarioresponsabilidade ur
                        on c.coonid = ur.coonid
                     inner join seguranca.perfil p
                        on p.pflcod = ur.pflcod
                     where ur.rpustatus = 'A'
                       and p.pflnivel = 3
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
                                      and p3.sisid = $sisid))";
        } else{
            $sql = "select c.coonid as codigo, c.coosigla || ' - ' || c.coodsc as descricao
                      from monitoraseb.coordenacao c
                     where c.coonid = $coonid";
        }

        $lista = $this->carregar($sql);

        $this->monta_combo("coonid", $lista, ($usucpf ? "S" : "N"), (count($lista) > 1 ? "Selecione..." : ""), $acao, "", "", "300", "N", "coonid", "", $coonid);
    }

    public function comboSubacao($sbaid){
       $sql = "select s.sbaid as codigo
                     ,case when s.sbasigla is null or char_length(s.sbasigla)=0 then s.sbatitulo else s.sbasigla || ' - ' || s.sbatitulo end as descricao
                 from monitora.pi_subacao s
                where s.sbaid = $sbaid
                order by descricao";

        $this->monta_combo('sbaid', $sql, 'N', 'Selecione...', '', '', '', '', 'N','sbaid','',$sbaid);
    }

    public function listaAcoes($sbaid){
        $sql = "select distinct aca.acacod, aca.acadsc
                  from monitora.acao aca
                 inner join monitora.ptres dtl
                    on aca.acaid = dtl.acaid
                 inner join monitora.pi_subacaodotacao sd
                    on dtl.ptrid = sd.ptrid
                 inner join public.unidade uni
                    on uni.unicod = dtl.unicod
                 where  sd.sbaid = $sbaid
                 order by aca.acadsc";

        $cabecalho = array("Código","Ação");
        $this->monta_lista_simples($sql,$cabecalho,100,50,'N','95%','N',false,false,false,true);
    }

    public function listarSubacao($coonid, $cootec, $super){
        $sisid = $_SESSION['sisid'];

        $where = "where ur.rpustatus = 'A' and";
        if($coonid){
            $where .= " c.coonid = '$coonid' and ";
        }
        $sql = "select distinct '<center>
                                     <a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/subacao/cadastrarsubacao&acao=A&scoid='||sc.scoid||'\'\">
                                         <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                                     </a>
                                 </center>' as acao
                      ,c.coosigla || ' - ' || c.coodsc as coordenacao
                      ,s.sbatitulo as subacao
                  from monitoraseb.subacaocoordenacao sc
                 inner join monitoraseb.coordenacao c
                    on sc.coonid = c.coonid
                 inner join monitora.pi_subacao s
                    on sc.sbaid = s.sbaid
                 inner join monitoraseb.usuarioresponsabilidade ur
                    on sc.coonid = ur.coonid
                   and sc.usucpf = ur.usucpf
                 inner join seguranca.perfil p
                    on p.pflcod = ur.pflcod ";

        if($cootec){
            $sql .= "$where p.pflnivel = 3
                        and ur.usucpf = '$cootec'";
        } else{
            $sql .= "$where p.pflnivel = 3
                        and ((ur.usucpf = '$super'
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
                                      and ur2.usucpf = '$super'
                                      and ur.coonid = ur2.coonid)
                        or exists (select pu2.usucpf
                                     from seguranca.perfilusuario pu2
                                    inner join seguranca.perfil p3
                                       on p3.pflcod = pu2.pflcod
                                    where p3.pflnivel = 1
                                      and pu2.usucpf = '$super'
                                      and p3.sisid = $sisid))";
        }

        $sql .= " order by coordenacao, subacao";

        $cabecalho = array("Ações","Coordenação","SubAção");
        $this->monta_lista($sql,$cabecalho,100,50,"N","center","N","",array( '5%', '25%', '70%' ));
    }

    public function comboTipoDocumento(){
        $sql = "select tidid as codigo
                      ,tiddsc as descricao
                  from monitoraseb.tipodocumento
                 order by descricao";

        $this->monta_combo("tidid_0", $sql, "S", "Selecione...", "", "", "", "160", "S", "tidid_0");
    }

    function salvar($scoid, $docid, $post, $files){
        extract($post);

        if(!$docid){
            $docid = wf_cadastrarDocumento(WORKFLOW_MONITORAMENTO_SEB, "Fluxo da SubAção x Coordenação - ID $scoid");
        }

        $sql = "update monitoraseb.subacaocoordenacao
                   set scpobjgeral='$scpobjgeral'
                      ,scpobjespecifico='$scpobjespecifico'
                      ,scpdsc='$scpdsc'
                      ,docid=$docid
                 where scoid=$scoid";

        $this->executar($sql);

        $sql = "delete from monitoraseb.subacaopublicoalvo
                 where sbaid = $sbaid";

        $this->executar($sql);

        if(!empty($funid[0]) ){
            foreach ($funid as $id) {
                $sql = "insert into monitoraseb.subacaopublicoalvo(sbaid, funid)
                        values ($sbaid, $id)";

                $this->executar($sql);
            }
        }

        for($i = 0; $i < count($files); $i++){
            if($files["arquivos_$i"] && $files["arquivos_$i"]["name"]){
                $campos = array("sbaid" => $sbaid,
                                "tidid" => ${"tidid_{$i}"}
                               );
                $file = new FilesSimec("subacaoanexo", $campos ,"monitoraseb");
                $file->setUpload(${"arqdescricao_{$i}"},"arquivos_$i");
            }
        }

        $this->commit();

        direcionar("?modulo=principal/subacao/cadastrarsubacao&acao=A&scoid=$scoid","Operação realizada com sucesso!");
    }

    public function excluirAnexo($arqid){
        $sql = "delete from monitoraseb.subacaoanexo
                 where arqid=$arqid";

        $this->executar($sql);

        $sql = "update public.arquivo
                   set arqstatus = 'I'
                 where arqid=$arqid";

        $this->executar($sql);

        $this->commit();

        $file = new FilesSimec();
        $file->excluiArquivoFisico($arqid);

    }

    public function abrirAnexo($arqid){
        $file = new FilesSimec();
        $arqid = $arqid;
        $arquivo = $file->getDownloadArquivo($arqid);
        direcionar('?modulo=principal/subacao/cadastrarsubacao&acao=A');
    }

    public function validarPermissaoGravar($usucpf, $scoid){
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
                   and (p.pflnivel = 1
                    or (exists (select sc.usucpf
                                  from monitoraseb.subacaocoordenacao sc
                                 inner join monitoraseb.usuarioresponsabilidade ur
                                    on sc.coonid = ur.coonid
                                 inner join workflow.documento d
                                    on sc.docid = d.docid
                                 where sc.scoid = $scoid
                                   and u.usucpf = ur.usucpf
                                   and ur.rpustatus = 'A'
                                   and ((p.pflnivel = 3
                                   and (d.esdid = ".WF_ESTADO_PREENCHIMENTO."
                                    or d.esdid = ".WF_ESTADO_DEVOLVIDO_AO_COORDENADOR_TECNICO."))
                                    or (p.pflnivel = 2
                                   and d.esdid = ".WF_ESTADO_AVALIACAO.")))))";
        $gravar = $this->pegaUm($sql);

        $permissoes['gravar'] = ($gravar ? true : false);

        return $permissoes;
    }
}