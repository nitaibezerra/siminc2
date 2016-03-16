<?PHP

    define("ENT_SAA", "742798");
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }
    
    # - dowloadDocAnexo: TELA MONITORAMENTO DE SOLICITAÇÃO DE DECRETO (DECRETO 7689) - DOWNLOAD DOCUMENTOS ANEXO.
    function dowloadDocAnexo( $dados ){

        $arqid = $dados['arqid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ( $arqid ){
            $file = new FilesSimec("arquivodecreto", $campos, "academico");
            $file->getDownloadArquivo( $arqid );
        }
    }
    
    # - preencherSessaoEnt: TELA MONITORAMENTO DE SOLICITAÇÃO DE DECRETO (DECRETO 7689) - PREENCHE SESSÃO PARA ABRIR A TELA DE SOLICITAÇÃO.
    function preencherSessaoEnt( $dados ){
        $entid = $dados['entid'];
        
        $_SESSION['academico']['entid'] = $entid;
        
        if( $_SESSION['academico']['entid'] != ''){
            $retorno =  'S';
        }else{
            $retorno =  'N';
        }
        echo $retorno;
        die();
    }

    require_once APPRAIZ . "includes/cabecalho.inc";
    echo '<br/>';

    $db->cria_aba($abacod_tela,$url,$parametros);

    monta_titulo( 'Monitoramento - Autorização de Decreto', 'Monitoramento' );

?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    
    /*
     * CARREGAR SESSAO.
     * @param {integer} sbsid
     * @param {integer} entid
     * @returns {undefined} redirecionamento
    */
    function abrirSolicitacaoDecreto( sbsid, entid ){

        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=preencherSessaoEnt&entid="+entid,
            success: function( resp ){
                if( trim(resp) == 'S' ){
                    window.location.href = "academico.php?modulo=principal/solicitacaodecretonovo&acao=A&sbsid=" + sbsid;
                }else{
                    alert('Não foi possivél, realizar essa ação. Tente novamente mais tarde!');
                }
            }
        });
    }
    
    function dowloadDocAnexo( arqid ){
        $('#arqid').val(arqid);
        $('#requisicao').val('dowloadDocAnexo');
        $('#formulario').submit();
    }
   
    function exibeMinuta( sbsid ){
        window.location.href = "academico.php?modulo=principal/autorizacaoSolicitacaoDecreto&acao=A&requisicao=exibirSolicitacaoDescreto&sbsid=" + sbsid;
    }

    function wf_exibirHistorico( docid ){
        var url = '../geral/workflow/historico.php' + '?modulo=principal/tramitacao' + '&acao=C' + '&docid=' + docid;
        window.open( url, 'alterarEstado', 'width=675, height=500, scrollbars=yes, scrolling=no, resizebled=no' );
    }

    function pesquisarAutorizacaoDecreto( param ){
        if(trim(param) == 'fil'){
            $('#formulario').submit();
        }else{
            $('#formulario').submit();
        }
    }

</script>

<form name="formulario" id="formulario" method="post" action="" />
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="arqid" name="arqid" value=""/>

    <table class="Tabela Listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="1">
        <tr>
            <td class ="SubTituloDireita" width="30%"> Instituição: </td>
            <td>
                <?php
                    $entnome = $dados['entnome'];
                    echo campo_texto('entnome', 'N', 'S', '', 50, 100, '', '', '', '', 0, 'id="entnome"', '', $entnome, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%" > Tipo de Contrado: </td>
            <td>
               <?PHP
                    $arrTpcid = array("1", "2");

                    if ($_SESSION['academico']['entid'] == ENT_SAA) {
                        $arrTpcid[] = "3";
                    }

                    $sql = "
                        SELECT tpcid  as codigo,
                               tpcdsc as descricao
                        FROM academico.tipocontrato
                        WHERE tpcstatus = 'A' and tpcid in('" . implode("','", $arrTpcid) . "')
                    ";
                    $db->monta_combo('tpcid', $sql, 'S', 'Selecione', '', '', '', 450, 'N', 'tpcid', false, $tpcid);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%" > Espécie de Contratação: </td>
            <td>
               <?PHP
                    $sql = "
                        SELECT epcid  as codigo,
                               epcdsc as descricao
                        FROM academico.especiecontratacao
                        WHERE epcstatus = 'A'
                    ";
                    $db->monta_combo('epcid', $sql, 'S', 'Selecione', '', '', '', 450, 'N', 'epcid');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Situação do WorkFlow:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  esdid AS codigo,
                                esddsc AS descricao
                        FROM workflow.estadodocumento
                        WHERE tpdid = ".WF_FLUXO_AUTORIZACAO_BENS_SERVICOS."
                    ";
                    $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '', '', 450, 'N', 'esdid', false, $esdid, null);
                ?>
            </td>
        </tr>
    </table>

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarAutorizacaoDecreto('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarAutorizacaoDecreto('ver');"/>
            </td>
        </tr>
    </table>

</form>


<?PHP

    $aba = !$_GET['aba'] ? "ensinoSuperior" : $_GET['aba'];

    #ARRAY COM ITENS DA ABA.
    if( $db->testa_superuser() ){
        $abaAtiva = "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba={$aba}";

        $menu = array(
            0 => array("id" => 1, "descricao" => "Educação Superior", "link" => "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=ensinoSuperior"),
            1 => array("id" => 2, "descricao" => "Educação Profissional", "link" => "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=ensinoProfissional"),
            2 => array("id" => 3, "descricao" => "Demais Instituições", "link" => "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=demaisInstituicoes")
        );
        
        if( $aba == 'ensinoSuperior' ){
            $funid = " ef.funid IN (".ACA_ID_UNIVERSIDADE.")";
        }elseif( $aba == 'ensinoProfissional' ) {
            $funid = " ef.funid IN (".ACA_ID_ESCOLAS_TECNICAS.")";
        }else{
            $funid = " ef.funid IN (".ACA_ID_UNIDADES_VINCULADAS." )";
        }
        //$funid = " ef.funid IN (".ACA_ID_UNIVERSIDADE.", ".ACA_ID_ESCOLAS_TECNICAS.", ".ACA_ID_UNIDADES_VINCULADAS." )";
    }else{

        $orgid = $_SESSION['academico']['orgid'];

        switch ( $orgid ){
            case '1':
                $funid = " ef.funid IN ('".ACA_ID_UNIVERSIDADE."')";
                
                $abaAtiva = "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=ensinoSuperior";
                
                $menu = array(
                    0 => array("id" => 1, "descricao" => "Educação Superior", "link" => "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=ensinoSuperior")
                );
                break;
            case '2':
                $funid = " ef.funid IN ('".ACA_ID_ESCOLAS_TECNICAS."')";
                
                $abaAtiva = "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=ensinoProfissional";
                
                $menu = array(
                    1 => array("id" => 2, "descricao" => "Educação Profissional", "link" => "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=ensinoProfissional")
                );
                break;
            case '3':
                $funid = " ef.funid IN ('".ACA_ID_UNIDADES_VINCULADAS."')";
                
                $abaAtiva = "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=demaisInstituicoes";
                
                $menu = array(
                    2 => array("id" => 3, "descricao" => "Demais Instituições", "link" => "academico.php?modulo=principal/monitoramentoAutorizacaoDecreto&acao=A&aba=demaisInstituicoes")
                );
                break;
        }
    }

    echo montarAbasArray($menu, $abaAtiva);

    if ($_REQUEST['entnome']) {
        $entnome = $_REQUEST['entnome'];
        $where[] = " public.removeacento(ent.entnome) iLike public.removeacento( ('%{$entnome}%') ) ";
    }

    if ($_REQUEST['tpcid']){
        $tpcid = $_REQUEST['tpcid'];
        $where[] = " tpc.tpcid = '{$tpcid}'";
    }
    if ($_REQUEST['epcid']) {
        $epcid = $_REQUEST['epcid'];
        $where[] = " epc.epcid = '{$epcid}'";
    }
    if ($_REQUEST['esdid']) {
        $esdid = $_REQUEST['esdid'];
        $where[] = " doc.esdid = '{$esdid}'";
    }

    if( !$db->testa_superuser() ){
        if ($_SESSION['academico']['entid'] != ENT_SAA) {
            $where[] = " tpc.tpcid <> 3";
        }
    }

    if($where != ""){
        //$WHERE = "WHERE l.qstid = {$_SESSION['maismedicomec']['qstid']} " . implode(' AND ', $where);
        $WHERE = "AND " . implode(' AND ', $where);
    }

    $acao = "<img style=\"cursor:pointer;\"src=\"../imagens/recuo_d.gif\" title=\"Ir para a solicitação de decreto\" onclick=\"abrirSolicitacaoDecreto(' || sbs.sbsid || ',' || ent.entid || ')\"/>";

    $down_a_1 = "<img border=\"0\" onclick=\"dowloadDocAnexo('|| arq_1.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/check_p.gif\" title=\"Download do Parecer Jurídico ou Declaração de Regularidade\"/>";
    $down_a_2 = "<img border=\"0\" onclick=\"dowloadDocAnexo('|| arq_2.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/check_p.gif\" title=\"Download do Parecer Jurídico ou Declaração de Regularidade\"/>";
    
    $down_n = "<img border=\"0\" align=\"absmiddle\" src=\"../imagens/check_p_01.gif\" title=\"Não a arquivo para Download\"/>";
    
    $sql = "
        SELECT  '{$acao}',
                
                CASE WHEN anx.sbsid IS NOT NULL 
                    THEN 'Exite(m) '|| anx.total_anx || ' Anexo(s)'
                    ELSE 'Não tem anexo'
                END AS existe_arquivo,            
                
                ent.entnome,
                tpc.tpcdsc,

                CASE WHEN esd.esdid=".WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_MINISTRO." OR esd.esdid=".WF_BENS_SERVICOS_AGUARDANDO_AUTORIZACAO_SECRETARIO."
                    THEN 'Não autorizado'
                    ELSE COALESCE('<img src=\"../imagens/print.gif\" alt=\"Visualizar Impressão\" class=\"link img_middle\" title=\"Visualizar Impressão\" class=\"link\" onclick=\"exibeMinuta(' || sbs.sbsid || ')\" /> <a href=\"javascript:exibeMinuta(' || sbs.sbsid || ')\"  >' || to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0') || '</a>','N/A')
                END as avenumauto,
                
                esd.esddsc,
                
                '<img src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( \'' || doc.docid || '\' );\" title=\"\" style=\"cursor: pointer;\">' as historico,
                epc.epcdsc,
                sbs.sbsnumprocesso,
                sbs.sbsvalor

        FROM academico.solicitacaobensservicos sbs

        LEFT JOIN workflow.documento doc on doc.docid = sbs.docid
        LEFT JOIN workflow.historicodocumento hst on hst.hstid = doc.hstid
        LEFT JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid

        LEFT JOIN academico.modalidadelicitacao mdl on sbs.mdlid = mdl.mdlid
        LEFT JOIN academico.tipocontratomodalidade tcm on mdl.mdlid = tcm.mdlid
        LEFT JOIN academico.tipocontrato tpc on sbs.tpcid = tpc.tpcid
        LEFT JOIN academico.especiecontratacao epc on sbs.epcid = epc.epcid

        LEFT JOIN entidade.entidade ent on ent.entid = sbs.entid
        LEFT JOIN entidade.funcaoentidade ef ON ef.entid = ent.entid      
        
        LEFT JOIN (
                SELECT  DISTINCT sbsid, 
                        COUNT(sbsid) as total_anx 
                FROM academico.arquivodecreto
                WHERE aqdstatus = 'A'
                GROUP BY sbsid
        ) AS anx on anx.sbsid = sbs.sbsid
        
        WHERE {$funid} AND sbs.sbsstatus = 'A' {$WHERE}

        ORDER BY sbs.sbsid
    ";
    $cabecalho = array("Ação","Anexo","Instituições","Tipo de Contrato","&nbsp;","Situação","Histórico","Espécie da Contratação","Número do Processo","Valor");
    $alinhamento = Array('center', 'center','left', 'left', 'left', 'left', 'center', 'left', 'left', 'right');
    //$tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

?>
