<?PHP
    include_once APPRAIZ . "includes/workflow.php";   

    function autorizar($dados) {
	global $db;

	$_arr = array(
            ESD_AGUARDANDO_MINISTRO => AED_AUTORIZAR_MINISTRO,
            ESD_AGUARDANDO_SECRETARIO => AED_AUTORIZAR_SECRETARIO
        );

	if($_POST['chkbox']){
            $chk = str_replace("]","",$_POST['chkbox']);
            $arrDocid = explode(",sbsid[",$chk);
            if($arrDocid){
                foreach($arrDocid as $docid){
                    if($docid){
                        $sql_sol = "
                            select  s.tpcid,
                                    d.esdid,
                                    s.sbsid
                            from academico.solicitacaobensservicos s
                            inner join workflow.documento d on d.docid = s.docid
                            where d.docid = {$docid}
                        ";
                        $dadoSolicitacao = $db->pegaLinha($sql_sol);
                        if($dadoSolicitacao){
                            $tpcid = $dadoSolicitacao['tpcid'];
                            $sbsid = $dadoSolicitacao['sbsid'];
                            $aedid = $_arr[$dadoSolicitacao['esdid']];
                            wf_alterarEstado( $docid, $aedid, "Autorização em lote" , array('tpcid' => $tpcid, 'secretario' => 1, 'ministro' => 2, 'sbsid' => $sbsid, 'aveid' => 0));
                        }
                    }
                }
            }
	}
        
        $dados['sbsid'] = $sbsid;
        $dados['impri'] = 'N'; #parametro para que não seja impresso o decreto mas, apenas criado o arquivo. 
        
        exibirSolicitacaoDescreto($dados);
        
	echo "<script>alert('Solicitação autorizada com sucesso');window.location='academico.php?modulo=principal/autorizacaoSolicitacaoDecreto&acao=A';</script>";
    }

    /**
     * functionName excluirImpreAutorizacao
     *
     * @author Luciano F. Ribeiro
     *
     * @param array $dados é usado o id do documento o doc id.
     * @return exclusão fisica do arquivo.
     *
     * @version v1
    */
    function excluirImpreAutorizacao( $dados ){
        global $db;
        
        $dir = APPRAIZ.'arquivos/academico/autorizacoesdecretogovernanca/';
        
        $arquivo = "{$dir}autorizacao_{$dados['arquivo']}.pdf";
        
        if( file_exists($arquivo) ){
            if( $dados['arquivo'] != '' ){
                $msg = "Arquivo excluido, Operação realizada com sucesso!";
                unlink($arquivo);
            }else{
                $msg = "Ocorreu algum problema com a exclusão do arquivo, por favor tente novamente!";
            }
        }else{
            $msg = "Ocorreu algum problema e é possivel que o arquivo não exista, verifique a existencia do arquivo e tente novamente!";
        }
        $db->sucesso('principal/autorizacaoSolicitacaoDecreto', '&acao=A', $msg);
    }    

    function pesquisarAutorizacaoDecreto( $dados ){
        if($dados['pesquisar'] == 'filtros'){
            $_SESSION['filtro']['entnome'] =  $dados['entnome'];
            $_SESSION['filtro']['tpcid'] =  $dados['tpcid'];
            $_SESSION['filtro']['epcid'] =  $dados['epcid'];
            $_SESSION['filtro']['esdid'] =  $dados['esdid'];
        }else{
            unset($_SESSION['filtro']);
        }
        die('OK');
    }

    function verificaExisteArquivo( $dados ){
        
        $usucpf = $dados['usucpf'];
        $arq    = str_replace(".", "_", $dados['arq']);
        $nome_arquivo = "autorizacao_{$arq}.pdf"; 
        
        $arquivo = APPRAIZ.'arquivos/academico/autorizacoesdecretogovernanca/'.$nome_arquivo;
        
        if( file_exists($arquivo) ){
            echo "<resp>S</resp>";
        }else{
            if( trim($_SESSION['usucpf']) == trim($usucpf) ){
                echo "<resp>S</resp>";
            }
            echo "<resp>N</resp>";
        }
        die();
    }
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }

    $perfis = pegaPerfilGeral();

    require_once APPRAIZ . "includes/cabecalho.inc";
    echo '<br/>';

    $db->cria_aba($abacod_tela,$url,$parametros);

    monta_titulo( 'Autorização - Decreto de Governança', '&nbsp;' );
?>

<style>
    .SubtituloTabela{background-color:#cccccc}
    .negrito{font-weight:bold}
    .bold{font-weight:bold}
    .normal{font-weight:normal}
    .center{text-align: center;}
    .direita{text-align: right;}
    .esquerda{text-align: left;}
    .msg_erro{color:#990000}
    .link{cursor: pointer}
    .mini{width:12px;height:12px}
    .sucess_msg{color: blue;}
    .img_middle{vertical-align:middle;border:0px}
    .hidden{display:none}
    .absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
    .fechar{position:relative;right:-5px;top:-26px;}
    .img{background-color:#FFFFFF}
    .red{color:#990000}
</style>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script type="text/javascript">
    $(function() {

        $('[name=btn_filtrar]').click(function() {
            $("#form_pesquisa").submit();
        });

        $('[name=btn_autorizar]').click(function() {
               var chk = 0;
               var arrChk = new Array();

               $("input[type='checkbox']").each(function(i,o){
                    if($(this).attr("name") != "tbn_todos"){
                        if( $(this).attr("checked") == true ){
                            chk++;
                            arrChk[chk] = $(this).attr("name");
                        }
                    }
               });

               if(chk > 0){
                    $("[name=chkbox]").val(arrChk);
                    $("[name=requisicao]").val('autorizar');
                    $("[name=form_pesquisa]").submit();
               }else{
                    alert('Selecione pelo menos uma solicitação!');
               }
        });

        $('[name=btn_ver_todos]').click(function() {
            window.location.href = window.location;
        });

    });

    function excluirImpreAutorizacao(arquivo){
        var confirma = confirm("Deseja realmente excluir essa Impressão de arquivo?");
        if( confirma ){
            $('#arquivo').val(arquivo);
            $('#requisicao').val('excluirImpreAutorizacao');
            $('#form_pesquisa').submit();
        }
    }

    function exibeMinuta(sbsid, arq, usucpf){
        $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=verificaExisteArquivo&arq="+arq+"&usucpf="+usucpf,
            success: function( resp ){
                var msg = pegaRetornoAjax('<resp>', '</resp>', resp, true);
                
                if( msg == 'N' ){
                    alert('Essa é a primeira vez que esse documento será impresso. É necessário que o documento seja impresso pelo o usuário que o autorizou!');
                }else{
                    window.location.href = "academico.php?modulo=principal/autorizacaoSolicitacaoDecreto&acao=A&requisicao=exibirSolicitacaoDescreto&sbsid=" + sbsid;
                }
            }
        });
    }

    function wf_exibirHistorico( docid ){
        var url = '../geral/workflow/historico.php' + '?modulo=principal/tramitacao' + '&acao=C' + '&docid=' + docid;

        window.open( url, 'alterarEstado', 'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no' );
    }

    function marcarTodos(obj){
	if($(obj).attr("checked") == true){
            $("input[type='checkbox']").each(function(i,o) {
                if($(this).attr("disabled") == false){
                    $(this).attr("checked",true);
                }
            });
	}

	if($(obj).attr("checked") == false){
            $("input[type='checkbox']").each(function(i,o) {
                if($(this).attr("disabled") == false){
                    $(this).attr("checked",false);
                }
            });
	}
    }

    function pesquisarAutorizacaoDecreto( param ){
        if(trim(param) == 'filtros'){
            $('[name=pesquisar]').val('filtros');
            $('#form_pesquisa').submit();
        }else{
            $('[name=pesquisar]').val('ver_todos');
            $('#form_pesquisa').submit();
        }
    }

</script>

    <form name="form_pesquisa" id="form_pesquisa" method="post" action="" />
        <input type="hidden" name="chkbox" id="chkbox" />
        <input type="hidden" id="requisicao" name="requisicao" value="" />
        <input type="hidden" id="pesquisar" name="pesquisar" value="" />
        <input type="hidden" id="arquivo" name="arquivo" value="" />

        <table class="Tabela Listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="1">
            <tr>
                <td class ="SubTituloDireita" width="30%"> Instituição: </td>
                <td>
                    <?php
                        //$entnome = $_SESSION['filtro']['entnome'] != '' ? $_SESSION['filtro']['entnome'] : '';
                        echo campo_texto('filtro_entnome', 'N', 'S', '', 50, 100, '', '', '', '', 0, 'id="filtro_entnome"', '', $entnome, null, '', null);
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" width="30%" > Tipo de Contrado: </td>
                <td>
                   <?PHP
                        $arrTpcid = array("1", "2");

                        if( $_SESSION['academico']['entid'] == ENT_SAA || in_array(PERFIL_SUPERUSUARIO, $perfis) ){
                            $arrTpcid[] = "3";
                        }

                        //$tpcid = $_SESSION['filtro']['tpcid'] != '' ? $_SESSION['filtro']['tpcid'] : '';
                        $sql = "
                            SELECT tpcid  as codigo,
                                   tpcdsc as descricao
                            FROM academico.tipocontrato
                            WHERE tpcstatus = 'A' and tpcid in('" . implode("','", $arrTpcid) . "')
                        ";
                        $db->monta_combo('filtro_tpcid', $sql, 'S', 'Selecione', '', '', '', 450, 'N', 'filtro_tpcid', false, $tpcid);
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" width="30%" > Espécie de Contratação: </td>
                <td>
                   <?PHP
                        //$epcid = $_SESSION['filtro']['epcid'] != '' ? $_SESSION['filtro']['epcid'] : '';
                        $sql = "
                            SELECT epcid  as codigo,
                                   epcdsc as descricao
                            FROM academico.especiecontratacao
                            WHERE epcstatus = 'A'
                        ";
                        $db->monta_combo('filtro_epcid', $sql, 'S', 'Selecione', '', '', '', 450, 'N', 'filtro_epcid', false, $epcid, null);
                    ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita">Situação do WorkFlow:</td>
                <td>
                    <?PHP
                        //$esdid = $_SESSION['filtro']['esdid'] != '' ? $_SESSION['filtro']['esdid'] : '';
                        $sql = "
                            SELECT  esdid AS codigo,
                                    esddsc AS descricao
                            FROM workflow.estadodocumento
                            WHERE tpdid = ".WF_FLUXO_AUTORIZACAO_BENS_SERVICOS."
                        ";
                        $db->monta_combo("filtro_esdid", $sql, 'S', 'Selecione...', '', '', '', 450, 'N', 'filtro_esdid', false, $esdid, null);
                    ?>
                </td>
            </tr>
        </table>

        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="button" name="pesquisa" value="Pesquisar" onclick="pesquisarAutorizacaoDecreto('filtros');"/>
                    <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarAutorizacaoDecreto('vertudo');"/>
                </td>
            </tr>
        </table>

<?PHP
    if( in_array(PERFIL_REDE_FEDERAL_MINISTRO, $perfis) || in_array(PERFIL_REDE_FEDERAL_SECR_EXECUTIVO, $perfis) ){
?>

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubtituloDireita esquerda" >
                    <input type="checkbox" name="tbn_todos" id="tbn_todos" onclick="marcarTodos(this)" /> <label for="tbn_todos" style="margin-left:5px;"> Marcar / Desmarcar Todos </label>
                </td>
                <td class="SubtituloDireita direita" >
                    <input type="button" value="Autorizar" name="btn_autorizar" />
                </td>
            </tr>
        </table>

<?PHP
    }

    #----------------------------------------------------------------- INICO FILTROS ---------------------------------------------------------#
    if ($_REQUEST['pesquisar'] == 'filtros') {
        if ($_REQUEST['filtro_entnome']) {
            $entnome = $_REQUEST['filtro_entnome'];
            $where[] = " public.removeacento(ent.entnome) iLike public.removeacento( ('%{$entnome}%') ) ";
        }

        if ($_REQUEST['filtro_tpcid']){
            $tpcid = $_REQUEST['filtro_tpcid'];
            $where[] = " tpc.tpcid = '{$tpcid}'";
        }
        if ($_REQUEST['filtro_epcid']) {
            $epcid = $_REQUEST['filtro_epcid'];
            $where[] = " epc.epcid = '{$epcid}'";
        }
        if ($_REQUEST['filtro_esdid']) {
            $esdid = $_REQUEST['filtro_esdid'];
            $where[] = " doc.esdid = '{$esdid}'";
        }
    }else{
        unset( $_SESSION['filtro'] );
    }

    if( !$db->testa_superuser() ){
        if( $_SESSION['academico']['entid'] != ENT_SAA ) {
            $where[] = " tpc.tpcid <> 3";
        }
    }

    if($where != ""){
        $WHERE = "AND " . implode(' AND ', $where);
    }

    #----------------------------------------------------------------- FIM FILTROS ---------------------------------------------------------#

    $stWhere = '';
    if(!academico_possui_perfil(PERFIL_ADMINISTRADOR)){
        $stWhere = " AND sbs.entid in (select entid from academico.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A') ";
    }

    $checkbox = '';
    if( in_array(PERFIL_REDE_FEDERAL_MINISTRO, $perfis) || in_array(PERFIL_REDE_FEDERAL_SECR_EXECUTIVO, $perfis) ){
        $checkbox = "<input type=checkbox  name=\"sbsid[' || sbs.docid || ']\">";
    }
    
    if( in_array(PERFIL_SUPERUSUARIO, $perfis) ){
        $acao_1 = "
            '<img src=\"../imagens/excluir.gif\" width=\"11\" title=\"Excluir Impressão\" class=\"link\" onclick=\"excluirImpreAutorizacao( \''||to_char(hst.htddata,'YYYYMMDD')||'_'||lpad(sbs.sbsid::varchar,4,'0')||'\' )\" />
            <img src=\"../imagens/print.gif\" title=\"Visualizar Impressão\" class=\"link\" onclick=\"exibeMinuta( '||sbs.sbsid||',\''||to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0')||'\',\''|| hst.usucpf ||'\' )\" /> 
            <a href=\"javascript:exibeMinuta( '||sbs.sbsid||',\''||to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0')||'\',\''|| hst.usucpf ||'\' )\" >' || to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0') || '</a>'
        ";        
    }else{
        $acao_1 = "
            '<img src=\"../imagens/print.gif\" title=\"Visualizar Impressão\" class=\"link\" onclick=\"exibeMinuta( '||sbs.sbsid||',\''||to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0')||'\',\''|| hst.usucpf ||'\' )\" /> 
            <a href=\"javascript:exibeMinuta( '||sbs.sbsid||',\''||to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0')||'\',\''|| hst.usucpf ||'\' )\" >' || to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0') || '</a>'
        ";
    }
    
    $acao_2 = "
        '<img src=\"../imagens/excluir.gif\" width=\"11\" title=\"Excluir Impressão\" class=\"link\" onclick=\"excluirImpreAutorizacao( '|| doc.docid ||' )\" />
        <img src=\"../imagens/print.gif\" title=\"Visualizar Impressão\" class=\"link\" onclick=\"exibeMinuta( '||sbs.sbsid||',\''||to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0')||'\',\''|| hst.usucpf ||'\' )\" /> 
        <a href=\"javascript:exibeMinuta( '||sbs.sbsid||',\''||to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0')||'\',\''|| hst.usucpf ||'\' )\" >' || to_char(hst.htddata,'YYYYMMDD')||'.'||lpad(sbs.sbsid::varchar,4,'0') || '</a>'
    ";
    

    $sql = "
        SELECT  CASE
                    WHEN esd.esdid=".ESD_AGUARDANDO_MINISTRO." OR esd.esdid=".ESD_AGUARDANDO_SECRETARIO." THEN '{$checkbox}'
                    WHEN esd.esdid=".ESD_AUTORIZADO_MINISTRO." OR esd.esdid=".ESD_AUTORIZADO_SECRETARIO." THEN '<img src=\"../imagens/check_p.gif\" title=\"Autorizado\" />'
                ELSE '' 
                END as acao,

                ent.entnome,
                tpc.tpcdsc,

                CASE WHEN esd.esdid=".ESD_AUTORIZADO_MINISTRO." OR esd.esdid=".ESD_AUTORIZADO_SECRETARIO."
                    THEN 
                        CASE WHEN hst.usucpf = '{$_SESSION['usucpf']}'
                            THEN {$acao_2}
                            ELSE {$acao_1}
                        END
                    ELSE 'Não autorizado'
                 END as avenumauto,

                esd.esddsc,
                '<img onclick=\"wf_exibirHistorico( \'' || doc.docid || '\' );\" title=\"\" src=\"http://simec-d/imagens/fluxodoc.gif\" style=\"cursor: pointer;\">' as historico,
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

        WHERE sbs.sbsstatus = 'A' {$WHERE}
        {$stWhere}
        ORDER BY sbs.sbsid
    ";
    $cabecalho = array("&nbsp;","Instituições","Tipo de Contrato","&nbsp;","Situação","Histórico","Espécie da Contratação","Número do Processo","Valor");
    $db->monta_lista($sql,$cabecalho,300,5,'N','center',$par2);

    if( in_array(PERFIL_REDE_FEDERAL_MINISTRO, $perfis) || in_array(PERFIL_REDE_FEDERAL_SECR_EXECUTIVO, $perfis) ){
?>
        <br>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td valign="top" class="SubtituloDireita direita" >
                    <input type="button" value="Autorizar" name="btn_autorizar" />
                </td>
            </tr>
        </table>
    </form>
<?PHP
    }
?>