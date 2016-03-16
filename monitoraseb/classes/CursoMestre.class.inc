<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "www/monitoraseb/geral/workflow.php";
class CursoMestre extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitoraseb.cursomestre";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cmtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                          'cmtid' => null,
                                        'scoid' => null,
                                        'tgeid' => null,
                                        'ncmid' => null,
                                        'cmtdsc' => null,
                                        'cmtementa' => null,
                                        'cmtobjespecifico' => null,
                                        'cmtcrghordistancia' => null,
                                        'cmtcrghorpresencial' => null,
                                        'cmtnumaluprojetado' => null,
                                        'cmtnumalumin' => null,
                                        'cmtnumalumax' => null,
                                        'cmtcertificacao' => null,
                                        'cmtreqcursista' => null
                                      );

    protected $arrPesquisa = null;

    public function mostraComboCoordenacao(){
        $sql = "SELECT distinct c.coonid as codigo, c.coosigla || ' - ' || c.coodsc as descricao FROM monitoraseb.coordenacao c ORDER BY descricao";
        $this->monta_combo('coonid', $sql, 'S', 'Selecione...', 'listarSubacao', '', '', 300, 'N','coonid');
    }

    public function listarCursoMestrePorCoordenacaoSubacao($coonid,$sbaid, $usucpf){
        $sisid = $_SESSION['sisid'];
        $where = " where 1=1 ";
        if(!empty($sbaid)){
            $where .= " and sbc.sbaid = '".$sbaid."'";
        }
        if(!empty($coonid)){
            $where .= " and sbc.coonid = '".$coonid."'";
        }
        $sql = "SELECT '<center>
                    <a style=\"cursor:pointer;\" onclick=\"detalharCursoMestre(\''||cmt.cmtid||'\');\">
                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                    </a>
                    <a style=\"cursor:pointer;\" onclick=\"' || case when (select count(*) from monitoraseb.instituicaohabilitada ih where ih.cmtid = cmt.cmtid) = 0 then
                    'if(confirm(\'Deseja excluir o registro?\')){removerCursoMestre(\''||cmt.cmtid||'\');}'
                    else 'alert(\'Registro não pode ser excluído!\');' end || '\">
                    <img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
                    </a>
                    </center>' as acao,
                    cmt.cmtdsc as cmtdsc,
                    coo.coodsc as coodsc,
                    sba.sbatitulo as sbatitulo
                FROM monitoraseb.cursomestre cmt
                JOIN monitoraseb.subacaocoordenacao sbc ON (cmt.scoid = sbc.scoid)
                JOIN monitoraseb.coordenacao coo ON (sbc.coonid = coo.coonid)
                JOIN monitora.pi_subacao sba ON (sbc.sbaid = sba.sbaid)
                ". $where."
                   and
                       exists( SELECT usu.usucpf
                                FROM seguranca.usuario usu
                                JOIN monitoraseb.usuarioresponsabilidade uresp ON (usu.usucpf = uresp.usucpf)
                                JOIN monitoraseb.coordenacao coor ON (uresp.coonid = coor.coonid)
                                JOIN seguranca.perfilusuario pfu ON (pfu.usucpf = usu.usucpf)
                                JOIN seguranca.perfil per ON (per.pflcod = pfu.pflcod)
                                WHERE usu.usucpf ='$usucpf' and per.sisid = $sisid and ((uresp.rpustatus = 'A' and coor.coonid=sbc.coonid) or (per.pflnivel = 1))
                    )

            ORDER BY cmtdsc, coodsc, sbatitulo";

        $cabecalho = array("Ação","Curso Mestre","Coordenação","Subação");
        $this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
    }

    public function comboCoordenadoresTecnicos($cootec,$permissoes){
        $sisid = $_SESSION['sisid'];

        $sql = "select distinct u.usucpf as codigo, u.usunome as descricao
                      from seguranca.usuario u
                     inner join monitoraseb.usuarioresponsabilidade ur
                        on u.usucpf = ur.usucpf
                     inner join seguranca.perfil p
                        on p.pflcod = ur.pflcod
                     where ur.rpustatus = 'A'
                       and p.pflnivel = 3
                       and ((ur.usucpf = '$cootec'
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
                                      and ur2.usucpf = '$cootec'
                                      and ur.coonid = ur2.coonid)
                        or exists (select pu2.usucpf
                                     from seguranca.perfilusuario pu2
                                    inner join seguranca.perfil p3
                                       on p3.pflcod = pu2.pflcod
                                    where p3.pflnivel = 1
                                      and pu2.usucpf = '$cootec'
                                      and p3.sisid = $sisid))";

        $resultado = $this->carregar($sql);

        $this->monta_combo("cootec",$resultado,$permissoes,count($resultado) > 1 ? "Selecione..." : "","listar_coordenacao","","","200","S","cootec","",$cootec);
    }

    public function campoModalidade($cmtcrghordistancia,$cmtcrghorpresencial,$campoHabilitado){
        $mlD = ($cmtcrghordistancia && !empty($cmtcrghordistancia) && $cmtcrghordistancia!=0)?true:false;
        $mlP = ($cmtcrghorpresencial && !empty($cmtcrghorpresencial) && $cmtcrghorpresencial!=0)?true:false;
        $modalidade = ($mlD && $mlP) ? 's' :(($mlP)? 'p' : 'd' );
        echo   "<input type='radio' name='modalidade' value='d' ".($modalidade=='d'?"CHECKED":"")." onchange=\"habilitaCampoModalidade('d')\" ".($campoHabilitado!='N'?'':'readonly="readonly" disabled="disabled" class="disabled"')."> A Distância
                <input type='radio' name='modalidade' value='p' ".($modalidade=='p'?"CHECKED":"")." onchange=\"habilitaCampoModalidade('p')\" ".($campoHabilitado!='N'?'':'readonly="readonly" disabled="disabled" class="disabled"')."> Presencial
                <input type='radio' name='modalidade' value='s' ".($modalidade=='s'?"CHECKED":"")." onchange=\"habilitaCampoModalidade('s')\" ".($campoHabilitado!='N'?'':'readonly="readonly" disabled="disabled" class="disabled"')."> Semi Presencial";
    }

    public function campoNivelCurso($permissoes){
        global $opcoes;
        $sql = "SELECT ncmid as codigo, ncmdsc as descricao FROM monitoraseb.nivelcursomestre ORDER BY ncmid";
        $this->monta_radio('ncmid', $sql, $permissoes, '');
    }

    public function campoNivelEscolaridade($cmtid, $permissoes){
        global $marcados;
        if($cmtid && $cmtid!=""){
            $resultado = $this->carregar("SELECT tgeid FROM monitoraseb.cursomestregrauescolaridade WHERE cmtid = ".$cmtid ." ORDER BY tgeid");

            if($resultado && !empty($resultado)){
                foreach($resultado as $escolaridade){
                    $marcados[] = $escolaridade['tgeid'];
                }
            }else{
                $marcados = array();
            }
        }
        else{
            $marcados = array();
        }

        $sql = "SELECT tgeid as codigo, tgedescricao as descricao FROM public.tipograuescolaridade ORDER BY tgeid";
        $this->monta_checkbox_Nivel('tgeid[]', $sql, $marcados, '  ',$permissoes);
    }

    private function monta_checkbox_Nivel($nome, $query, $marcados, $separador='  ', $permissoes) {
            if(!is_array($marcados) || @count($marcados)<1) {
                $marcados = array();
            }

            $saida = "";
            $rescombo=$this->carregar($query);
            if(is_array($rescombo) && @count($rescombo)>0) {
                for ($i=0, $j=count($rescombo)-1;$i<count($rescombo);$i++) {
                    $checked = array_search($rescombo[$i]['codigo'], $marcados) !== false ? 'checked="checked"' : '';
                    echo $rescombo[$i]['indice']."<input type=\"checkbox\" name=\"" . $nome . "\" value=\"" . $rescombo[$i]['codigo'] . "\" " . $checked . " ".($permissoes!='N'?'':'readonly="readonly" disabled="disabled" class="disabled"')."/> " . $rescombo[$i]['descricao'];
                    if($i!=$j) echo $separador;
                }
            }
        }

    public function excluirCursoMestre($cmtid){
        $this->executar("DELETE FROM monitoraseb.cursomestre WHERE cmtid = $cmtid");
        $this->commit();
        direcionar('?modulo=principal/cursomestre/consultarcursomestre&acao=A','Operação realizada com sucesso!');
    }

    public function salvarCursoMestre($post, $files){

        extract($post);
        //trata nulo
        $ncmid = (!empty($ncmid))?$ncmid:'null';
        $cmtdsc = (!empty($cmtdsc))?$cmtdsc:'';
        $cmtementa = (!empty($cmtementa))?$cmtementa:'';
        $cmtobjespecifico = (!empty($cmtobjespecifico))?$cmtobjespecifico:'';
        $cmtcrghordistancia = (!empty($cmtcrghordistancia))?$cmtcrghordistancia:'null';
        $cmtcrghorpresencial = (!empty($cmtcrghorpresencial))?$cmtcrghorpresencial:'null';
        $cmtnumaluprojetado = (!empty($cmtnumaluprojetado))?$cmtnumaluprojetado:'null';
        $cmtnumalumin = (!empty($cmtnumalumin))?$cmtnumalumin:'null';
        $cmtnumalumax = (!empty($cmtnumalumax))?$cmtnumalumax:'null';
        $cmtcertificacao = (!empty($cmtcertificacao))?$cmtcertificacao:'';
        $cmtreqcursista = (!empty($cmtreqcursista))?$cmtreqcursista:'';
        $cmtduracao = (!empty($cmtduracao))?$cmtduracao:'';

        if($coonid_disable)
            $coonid = $coonid_disable;

        if($sbaid_disable)
        $sbaid = $sbaid_disable;

        if($sbaid && $coonid){
            $subacaoCoordenacao = $this->carregar("SELECT scoid FROM monitoraseb.subacaocoordenacao WHERE sbaid = ".$sbaid." AND coonid =". $coonid);
            $scoid = (count($subacaoCoordenacao) && $subacaoCoordenacao[0])? $subacaoCoordenacao[0]['scoid'] : 'null';
        }

        if(!$docid){
            $docid = wf_cadastrarDocumento(WORKFLOW_MONITORAMENTO_SEB, "Fluxo do Curso Mestre - ID $cmtid");
        }

        $sql ="";
        if($cmtid==null || empty($cmtid)){
            $sql = "INSERT INTO monitoraseb.cursomestre(
                        scoid,
                        ncmid,
                        cmtdsc,
                        cmtementa,
                        cmtobjespecifico,
                        cmtcrghordistancia,
                        cmtcrghorpresencial,
                        cmtnumaluprojetado,
                        cmtnumalumin,
                        cmtnumalumax,
                        cmtcertificacao,
                        cmtreqcursista,
                        docid,
                        cmtduracao
                    )
                    VALUES (
                        $scoid,
                        $ncmid,
                        '$cmtdsc',
                        '$cmtementa',
                        '$cmtobjespecifico',
                        $cmtcrghordistancia,
                        $cmtcrghorpresencial,
                        $cmtnumaluprojetado,
                        $cmtnumalumin,
                        $cmtnumalumax,
                        '$cmtcertificacao',
                        '$cmtreqcursista',
                        $docid,
                        '$cmtduracao'
                    )";
           }else{
            $sql = "UPDATE monitoraseb.cursomestre
                       SET scoid=$scoid,
                           ncmid=$ncmid,
                           cmtdsc='$cmtdsc',
                           cmtementa='$cmtementa',
                           cmtobjespecifico='$cmtobjespecifico',
                           cmtcrghordistancia=$cmtcrghordistancia,
                           cmtcrghorpresencial=$cmtcrghorpresencial,
                           cmtnumaluprojetado=$cmtnumaluprojetado,
                           cmtnumalumin=$cmtnumalumin,
                           cmtnumalumax=$cmtnumalumax,
                           cmtcertificacao='$cmtcertificacao',
                           cmtreqcursista='$cmtreqcursista',
                           docid=$docid,
                           cmtduracao='$cmtduracao'
                     WHERE  cmtid=$cmtid";
           }
           $sql .=" returning cmtid";
           $cmtid = $this->pegaUm($sql);

           //salva itens grade curricular
        if(isset($_SESSION['listagemGradeCurricular'])){
            $listagemGradeCurricular = $_SESSION['listagemGradeCurricular'];
            $itensGCSalvos="";
            foreach($listagemGradeCurricular as $gradeCurricular){
                $itensGCSalvos.=($itensGCSalvos!="")?",":"";
                $sqlGC = "";
                if($gradeCurricular['gcrid'] || empty($gradeCurricular['gcrid'])){
                    $sqlGC = "INSERT INTO monitoraseb.gradecurricular(
                                        cmtid, tgcid, gcrnome, gcrnumhoraula, gcrementa)
                                    VALUES (".$cmtid.",
                                            ".$gradeCurricular['tgcid'].",
                                            '".$gradeCurricular['gcrnome']."',
                                            ".$gradeCurricular['gcrnumhoraula'].",
                                            '".$gradeCurricular['gcrementa']."')";
                }else{
                    $sqlGC = "UPDATE monitoraseb.gradecurricular
                                   SET cmtid=".$cmtid.",
                                       tgcid=".$gradeCurricular['tgcid'].",
                                       gcrnome='".$gradeCurricular['gcrnome']."',
                                       gcrnumhoraula=".$gradeCurricular['gcrnumhoraula'].",
                                       gcrementa='".$gradeCurricular['gcrementa']."'
                                 WHERE gcrid=".$gradeCurricular['gcrid'];
                }
                $sqlGC .=" returning gcrid";
                $itensGCSalvos .= $this->pegaUm($sqlGC);
            }

            //apaga itens removidos
            $condicao = ($itensGCSalvos=="")?"":" and gcrid not in ($itensGCSalvos)";
            $sqldeleteGC = "DELETE FROM monitoraseb.gradecurricular
                             WHERE cmtid=".$cmtid." ".$condicao ;
            $this->executar($sqldeleteGC);
            unset($_SESSION['listagemGradeCurricular']);
        }

        //salva itens publico alvo
        $sql = "delete from monitoraseb.cursomestrepublicoalvo
                     where cmtid = $cmtid";

        $this->executar($sql);

        if(!empty($funid[0]) ){
            foreach ($funid as $id) {
                $sql = "insert into monitoraseb.cursomestrepublicoalvo(cmtid, funid)
                        values ($cmtid, $id)";

                $this->executar($sql);
            }
        }

        //salva nivel escolaridade
        $sql = "DELETE FROM monitoraseb.cursomestregrauescolaridade
                 WHERE cmtid = $cmtid";

        $this->executar($sql);

        if($tgeid && !empty($tgeid)){
            foreach ($tgeid as $idEscolaridade){
                $sql = "INSERT INTO monitoraseb.cursomestregrauescolaridade(cmtid, tgeid)
                        VALUES ($cmtid,$idEscolaridade)";
                 $this->executar($sql);
            }
        }

        //salva itens equipe
        if(isset($_SESSION['listagemEquipe'])){
            $listagemEquipe = $_SESSION['listagemEquipe'];
            $itensEqSalvos="";
            foreach($listagemEquipe as $equipe){
                $itensEqSalvos.=($itensEqSalvos!="")?",":"";
                $sqlEq = "";
                if($equipe['ecmid'] || empty($equipe['ecmid'])){
                    $sqlEq = "INSERT INTO monitoraseb.equipecursomestre(
                                    cmtid, catid, unrid, ecmdscfuncao, ecmnummin, ecmnummax,
                                    ecmdscatribuicao)
                                    VALUES (".$cmtid.",
                                            ".$equipe['catid'].",
                                            '".$equipe['unrid']."',
                                            '".$equipe['ecmdscfuncao']."',
                                            ".$equipe['ecmnummin'].",
                                            ".$equipe['ecmnummax'].",
                                            '".$equipe['ecmdscatribuicao']."')";
                }else{
                    $sqlEq = "UPDATE monitoraseb.equipecursomestre
                                   SET cmtid=".$cmtid.",
                                       catid=".$equipe['catid'].",
                                       unrid='".$equipe['unrid']."',
                                       ecmdscfuncao='".$equipe['ecmdscfuncao']."',
                                       ecmnummin=".$equipe['ecmnummin']."
                                       ecmnummax=".$equipe['ecmnummax']."
                                       ecmdscatribuicao='".$equipe['ecmdscatribuicao']."'
                                 WHERE ecmid=".$equipe['ecmid'];
                }
                $sqlEq .=" returning ecmid";
                $itensEqSalvos .= $this->pegaUm($sqlEq);
            }

            //apaga itens removidos
            $condicao = ($itensEqSalvos=="")?"":" and ecmid not in ($itensEqSalvos)";
            $sqldeleteEq = "DELETE FROM monitoraseb.equipecursomestre
                             WHERE cmtid=".$cmtid." ".$condicao ;
            $this->executar($sqldeleteEq);
            unset($_SESSION['listagemEquipe']);
        }

    //salva itens equipe
        if(isset($_SESSION['listagemOfertaVaga'])){
            $listagemVagas = $_SESSION['listagemOfertaVaga'];
            $itensVagaSalvos="";
            foreach($listagemVagas as $vaga){
                $itensVagaSalvos.=($itensVagaSalvos!="")?",":"";
                $sqlVaga = "";
                if($vaga['ovgid'] || empty($vaga['ovgid'])){
                    $sqlVaga = "INSERT INTO monitoraseb.ofertavaga(
                                    cmtid, ovgnumvagas, ovganobase)
                                    VALUES (".$cmtid.",
                                            ".$vaga['ovgnumvagas'].",
                                            ".$vaga['ovganobase'].")";
                }else{
                    $sqlVaga = "UPDATE monitoraseb.equipecursomestre
                                   SET cmtid=".$cmtid.",
                                       ovgnumvagas=".$vaga['ovgnumvagas'].",
                                       ovganobase=".$vaga['ovganobase'].",
                                 WHERE ovgid=".$vaga['ovgid'];
                }
                $sqlVaga .=" returning ovgid";
                $itensVagaSalvos .= $this->pegaUm($sqlVaga);
            }

            //apaga itens removidos
            $condicao = ($itensVagaSalvos=="")?"":" and ovgid not in ($itensVagaSalvos)";
            $sqldeleteVaga = "DELETE FROM monitoraseb.ofertavaga
                             WHERE cmtid=".$cmtid." ".$condicao ;
            $this->executar($sqldeleteVaga);
            unset($_SESSION['listagemOfertaVaga']);
        }

        for($i = 0; $i < count($files); $i++){
            if($files["arquivos_$i"] && $files["arquivos_$i"]["name"]){
                $campos = array("cmtid" => $cmtid,
                                "tidid" => ${"tidid_{$i}"}
                               );
                $file = new FilesSimec("cursomestreanexo", $campos ,"monitoraseb");
                $file->setUpload(${"arqdescricao_{$i}"},"arquivos_$i");
            }
        }

        $this->commit();
        direcionar("?modulo=principal/cursomestre/cadastrarcursomestre&acao=A&cmtid=$cmtid","Operação realizada com sucesso!");
    }

    public function carregaGradeCurricularPorIdCursoMestre($cmtid){
        $sql = "SELECT tgc.tgcid as tgcid, tgc.tgcdsc as tgcdsc, gc.gcrnome as gcrnome, gc.gcrnumhoraula as gcrnumhoraula, gc.gcrementa as gcrementa, gc.gcrid as gcrid
                FROM monitoraseb.gradecurricular gc
                JOIN monitoraseb.tipogradecurricular tgc ON (gc.tgcid = tgc.tgcid)
                WHERE gc.cmtid=$cmtid";
        $listagem = $this->carregar($sql);
        return ($listagem) ? $listagem : array();
    }

    public function carregaCursoMestrePorId($cmtid){
        $sql = "SELECT cm.cmtid, cm.scoid, cm.ncmid, cm.cmtdsc, cm.cmtementa, cm.cmtobjespecifico, cm.cmtcrghordistancia,
                   cm.cmtcrghorpresencial, cm.cmtnumaluprojetado, cm.cmtnumalumin, cm.cmtnumalumax,
                   cm.cmtcertificacao, cm.cmtreqcursista, cm.docid, sc.sbaid, sc.coonid, sc.usucpf, cm.cmtduracao
                  FROM monitoraseb.cursomestre cm
                  JOIN monitoraseb.subacaocoordenacao sc ON (cm.scoid = sc.scoid)
                  WHERE cmtid =".$cmtid;
        return $this->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );
    }

    public function buscaSubacaoCoordenacao($scoid){
        $listagem = $this->carregar("SELECT  sbaid, coonid FROM monitoraseb.subacaocoordenacao WHERE scoid = ".$scoid);
        return ($listagem) ? $listagem[0] : array();
    }

    public function comboTipoGC($combotipo){
        $sql = "SELECT tgcid || ':'|| tgcdsc as codigo,  tgcdsc as descricao FROM monitoraseb.tipogradecurricular ORDER BY tgcdsc";
        $this->monta_combo('combotipo', $sql, 'S', 'Selecione...', '', '', '', '', 'S',$combotipo);
    }

    public function comboCategoria(){
        $sql = "SELECT catid || ':'|| catnome as codigo,  catnome as descricao FROM monitoraseb.categoria ORDER BY catnome";
        $this->monta_combo('combocategoria', $sql, 'S', 'Selecione...', '', '', '', '', 'S','comboCategoria');
    }

    public function comboUnidadeReferencia(){
        $sql = "SELECT unrid || ':'|| unrdsc as codigo,  unrdsc as descricao FROM monitoraseb.unidadereferencia ORDER BY unrdsc";
        $this->monta_combo('combounidaderef', $sql, 'S', 'Selecione...', '', '', '', '', 'S','combounidaderef');
    }

    public function carregaEquipePorIdCursoMestre($cmtid){
        $sql = "SELECT cat.catid, cat.catnome,
                    urf.unrid, urf.unrdsc,
                    eqp.ecmdscfuncao, eqp.ecmnummin, eqp.ecmnummax,
                    eqp.ecmdscatribuicao, eqp.ecmid
                FROM monitoraseb.equipecursomestre eqp
                JOIN monitoraseb.categoria cat ON (eqp.catid = cat.catid)
                JOIN monitoraseb.unidadereferencia urf ON (eqp.unrid = urf.unrid)
                WHERE eqp.cmtid=$cmtid";
        $listagem = $this->carregar($sql);
        return ($listagem) ? $listagem : array();
    }

    public function carregaOfertaVagaPorIdCursoMestre($cmtid){
        $sql = "SELECT ovgnumvagas, ovganobase, ovgid
                FROM monitoraseb.ofertavaga
                WHERE cmtid=$cmtid";
        $listagem = $this->carregar($sql);
        return ($listagem) ? $listagem : array();
    }

    public function excluirAnexo($arqid){
        $sql = "delete from monitoraseb.cursomestreanexo
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
        direcionar('?modulo=principal/cursomestre/cadastrarcursomestre&acao=A');
    }

    public function comboTipoDocumento(){
        $sql = "select tidid as codigo
                      ,tiddsc as descricao
                  from monitoraseb.tipodocumento
                 order by descricao";

        $this->monta_combo("tidid_0", $sql, "S", "Selecione...", "", "", "", "160", "S", "tidid_0");
    }

    public function listaDocumentos($cmtid,$permiteGravar){
        $excluir = ($permiteGravar!="S")?"''":"'<center><a style=\" cursor: pointer;\"onclick=\"if(confirm(\'Deseja excluir o registro?\')){javascript:atualizaDocumentoAnexo(' || a.arqid || ', \'CursoMestre\', $cmtid, \'$permiteGravar\');}\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'";
        if($cmtid && !empty($cmtid)){
            $sql = "select $excluir as acao
                          ,to_char(a.arqdata,'DD/MM/YYYY') as arqdata
                          ,td.tiddsc
                          ,a.arqdescricao
                          ,'<a style=\"cursor: pointer; color: blue;\" onclick=\"javascript:abrirAnexo(' || a.arqid || ');\">' || a.arqnome || '.'|| a.arqextensao ||'</a>'
                          ,round(a.arqtamanho/1024.0, 2) || ' KB' as arqtamanho
                          ,u.usunome
                      from public.arquivo a
                     inner join monitoraseb.cursomestreanexo cma
                        on a.arqid = cma.arqid
                     inner join monitoraseb.tipodocumento td
                        on cma.tidid = td.tidid
                     inner join seguranca.usuario u
                        on u.usucpf = a.usucpf
                     where a.arqstatus = 'A'
                       and cma.cmtid = ".$cmtid;
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

public function validarPermissaoGravar($usucpf, $cmtid){
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
                                  from monitoraseb.cursomestre cm
                                 inner join monitoraseb.subacaocoordenacao sc
                                    on cm.scoid = sc.scoid
                                 inner join monitoraseb.usuarioresponsabilidade ur
                                    on sc.coonid = ur.coonid
                                 inner join workflow.documento d
                                    on cm.docid = d.docid
                                 where cm.cmtid = $cmtid
                                   and u.usucpf = ur.usucpf
                                   and ur.rpustatus = 'A'
                                   and ((p.pflnivel = 3
                                   and (d.esdid = ".WF_ESTADO_PREENCHIMENTO."
                                    or d.esdid = ".WF_ESTADO_DEVOLVIDO_AO_COORDENADOR_TECNICO."))
                                    or (p.pflnivel = 2
                                   and d.esdid = ".WF_ESTADO_AVALIACAO.")))))";
        $gravar = $this->pegaUm($sql);

        return ($gravar)?'S':'N';
    }
}
