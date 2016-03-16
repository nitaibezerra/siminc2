<?php
    include_once APPRAIZ . "includes/workflow.php";
    require_once APPRAIZ . "includes/cabecalho.inc";
    echo '<br/>';
    $db->cria_aba($abacod_tela, $url, $parametros);
    monta_titulo('Autorização de Viagens para o Exterior', '&nbsp;');

    if ($_POST['chkbox']) {
        $chk = str_replace("]", "", $_POST['chkbox']);
        $arrDocid = explode(",aveid[", $chk);
        if ($arrDocid) {
            foreach ($arrDocid as $docid) {
                if ($docid) {
                    $aveid = $db->pegaUm("select aveid from academico.autviagemexterior where docid = $docid");
                    if ($aveid) {
                        wf_alterarEstado($docid, 728, "Autorizar", array('aveid' => $aveid));
                    }
                }
            }
        }
    }

    if ($_POST['entid']){
        $arrWhere[] = "ent.entid = {$_POST['entid']}";
    }
    if ($_POST['esdid']){
        $arrWhere[] = "ed.esdid = {$_POST['esdid']}";
    }

    if (!$_POST['ano']){
        $_POST['ano'] = date('Y');
    }
    if ($_POST['ano']) {
        $arrWhere[] = "to_char(tbl1.avedata,'YYYY') = '{$_POST['ano']}'";
    }

    $arrPerfil = pegaPerfilGeral();
    if ( in_array(PERFIL_ALTA_GESTAO,$arrPerfil) || in_array(PERFIL_REDE_FEDERAL_MINISTRO,$arrPerfil) || in_array(PERFIL_REDE_FEDERAL_SECR_EXECUTIVO,$arrPerfil) ){ 
        $acao = "
            CASE ed.esdid 
                WHEN ".WF_SOLICITACAO_VIAGEM_AGUARDANDO_AUTORIZACAO_SECRETARIA_EXECUTIVA." 
                    THEN '<input type=\"checkbox\" name=\"aveid[' || tbl1.docid || ']\" />'
                WHEN ".WF_SOLICITACAO_VIAGEM_AUTORIZADO." 
                    THEN '<img src=\"../imagens/check_p.gif\" title=\"Autorizado\" />'
                ELSE '<input type=\"checkbox\" title=\"Indisponível\" disabled=disabled name=\"aveid[' || tbl1.docid || ']\" />'
            END as acao,
        ";
    } else {
        $acao = "'' as acao,";
    }
    
    $sql = "
        select  $acao
                entnome,
                aveid,
                esddsc,
                ed.esdid,
                ent.entid,
                avenomereitor,
                COALESCE('<div style=\"white-space: nowrap\" ><img src=\"../imagens/print.png\" alt=\"Visualizar Impressão\" class=\"link img_middle\"  title=\"Visualizar Impressão\" class=\"link\" onclick=\"exibeMinuta(' || aveid || ')\" /> <a href=\"javascript:exibeMinuta(' || aveid || ')\"  >' || avenumauto || '</a></div>','N/A') as avenumauto,
                ed.esddsc as docid,
                ( select sum(qtde) from academico.autvexqtdeprg atp where atp.aveid = tbl1.aveid) as qtde,
                '<img src=\"../imagens/fluxodoc.gif\" class=\"link img_middle\" onclick=\"wf_exibirHistorico(\'' || tbl1.docid || '\')\"  />' || to_char(avedata,'DD/MM/YYYY') as avedata
        
        from  academico.autviagemexterior tbl1
        
        inner join entidade.entidade ent ON tbl1.entid = ent.entid
        inner join workflow.documento d ON d.docid = tbl1.docid
        inner join workflow.estadodocumento ed on ed.esdid = d.esdid
        
        where avestatus = 'A' ".($arrWhere ? " and " . implode(" and ", $arrWhere) : "")."
            
        order by entnome,ed.esddsc
    ";
    $arrDados = $db->carregar($sql);
    $arrDados = $arrDados ? $arrDados : array();
    
    foreach ($arrDados as $chave => $dado){
        
        $sql = "
            select  pvedsc || ' - ' || qtde as pvedsc
            from academico.autvexqtdeprg atp
            
            inner join academico.progviagemexterior prg ON prg.pveid = atp.pveid 
            
            where atp.aveid = {$dado['aveid']}
                
            order by pvedsc
        ";
        $arrProg = $db->carregarColuna($sql);
        $arrDados[$chave]['programa'] = ($arrProg ? implode("<br />", $arrProg) : "N/A");
        $arrDados[$chave]['qtde_part'] = $arrDados[$chave]['qtde'];
        $arrDados[$chave]['estado'] = $arrDados[$chave]['docid'];
        $arrDados[$chave]['data'] = $arrDados[$chave]['avedata'];
        
        unset( $arrDados[$chave]['avedata'], $arrDados[$chave]['qtde'], $arrDados[$chave]['docid'], $arrDados[$chave]['entid'], $arrDados[$chave]['esdid'], $arrDados[$chave]['esddsc'] );

        if ( !(in_array(PERFIL_ALTA_GESTAO,$arrPerfil) || in_array(PERFIL_REDE_FEDERAL_MINISTRO,$arrPerfil) || in_array(PERFIL_REDE_FEDERAL_SECR_EXECUTIVO,$arrPerfil) ) ){
            unset($arrDados[$chave]['acao']);
        }

        $arrUnidades[$dado['entid']] = array("codigo" => $dado['entid'], "descricao" => $dado['entnome']);
        $arrSituacao[$dado['esdid']] = array("codigo" => $dado['esdid'], "descricao" => $dado['esddsc']);
    }

    $sql = "
        select  $acao
                entnome,
                aveid,
                esddsc,
                ed.esdid,
                ent.entid,
                avenomereitor,
                COALESCE('<img src=\"../imagens/print.gif\" alt=\"Visualizar Impressão\" class=\"link img_middle\"  title=\"Visualizar Impressão\" class=\"link\" onclick=\"exibeMinuta(' || aveid || ')\" /> <a href=\"javascript:exibeMinuta(' || aveid || ')\"  >' || avenumauto || '</a>','N/A') as avenumauto,
                ed.esddsc as docid,
                ( select sum(qtde) from academico.autvexqtdeprg atp where atp.aveid = tbl1.aveid) as qtde,
                '<img src=\"../imagens/fluxodoc.gif\" class=\"link img_middle\" onclick=\"wf_exibirHistorico(\'' || tbl1.docid || '\')\"  />' || to_char(avedata,'DD/MM/YYYY') as avedata
        
        from academico.autviagemexterior tbl1
        
        inner join entidade.entidade ent ON tbl1.entid = ent.entid
        inner join workflow.documento d ON d.docid = tbl1.docid
        inner join workflow.estadodocumento ed on ed.esdid = d.esdid
        
        where avestatus = 'A'
        
        order by entnome,ed.esddsc
    ";
    $arrDados2 = $db->carregar($sql);
    $arrDados2 = $arrDados2 ? $arrDados2 : array();
    
    foreach ($arrDados2 as $chave => $dado) {
        $arrUnidades[$dado['entid']] = array("codigo" => $dado['entid'], "descricao" => $dado['entnome']);
        $arrSituacao[$dado['esdid']] = array("codigo" => $dado['esdid'], "descricao" => $dado['esddsc']);
    }

    if ( in_array(PERFIL_SUPERUSUARIO,$arrPerfil) || in_array(PERFIL_ALTA_GESTAO,$arrPerfil) || in_array(PERFIL_REDE_FEDERAL_MINISTRO,$arrPerfil) || in_array(PERFIL_REDE_FEDERAL_SECR_EXECUTIVO,$arrPerfil) ){ 
        $cabecalho = array("Selecionar", "Unidade", "Num. Solicitação", "Reitor", "Número de Autorização", "Programas", "Número de Participantes", "Situação", "Data da Solicitação");
    } else {
        $cabecalho = array("Unidade", "Num. Solicitação", "Reitor", "Número de Autorização", "Programas", "Número de Participantes", "Situação", "Data da Solicitação");
    }

    if ($arrUnidades) {
        foreach ($arrUnidades as $unidade) {
            $arrEntid[] = $unidade;
        }
    } else {
        $arrEntid[] = array();
    }
    if ($arrSituacao) {
        foreach ($arrSituacao as $situacao) {
            $arrEsdid[] = $situacao;
        }
    } else {
        $arrEsdid[] = array();
    }
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
               $("input[type='checkbox']").each(function(i,o) {
                       if($(this).attr("name") != "tbn_todos"){
                               if( $(this).attr("checked") == true ){
                                       chk++;
                                       arrChk[chk] = $(this).attr("name");
                               }
                       }
               });
               if(chk > 0){
                       $("[name=chkbox]").val(arrChk);
                       $("[name=form_pesquisa]").submit();
               }else{
                       alert('Selecione pelo menos uma solicitação!');
               }
        });
        $('[name=btn_ver_todos]').click(function() {
               window.location.href = window.location;
        });
    });

    function exibeMinuta(aveid)
    {
        window.location.href = "academico.php?modulo=principal/solicitacaoViagemExterior&acao=A&PopUpMinuta=1&aveid=" + aveid;
        //janela(url,800,600,"Minuta");
    }
    function wf_exibirHistorico( docid )
    {
        var url = '../geral/workflow/historico.php' +
                '?modulo=principal/tramitacao' +
                '&acao=C' +
                '&docid=' + docid;
        window.open(
                url,
                'alterarEstado',
                'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
        );
    }
    function marcarTodos(obj)
    {

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
</script>
<form name="form_pesquisa" id="form_pesquisa" method="post" action="">
    <input type="hidden" name="chkbox" id="chkbox"/>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td width="25%" valign="top" class="SubtituloDireita" >Ano:</td>
            <td>
                <?php
                    $anoini = 2011;
                    $anofim = date('Y');
                    $arrAno = array();
                    
                    for($anoini; $anoini<=$anofim; $anoini++){
                            $arrAno[] = array("codigo" => $anoini, "descricao" => $anoini);
                    }
                    $db->monta_combo('ano',$arrAno,"S","Selecione...","","","","","","","",$_POST['ano']);
                ?>
            </td>
        </tr>
        <tr>
            <td width="25%" valign="top" class="SubtituloDireita" >Unidade:</td>
            <td><?php $db->monta_combo('entid',$arrEntid,"S","Selecione...","","","","","","","",$_POST['entid']) ?></td>
        </tr>
        <tr>
            <td width="25%" valign="top" class="SubtituloDireita" >Situação:</td>
            <td><?php $db->monta_combo('esdid',$arrEsdid,"S","Selecione...","","","","","","","",$_POST['esdid']) ?></td>
        </tr>
        <tr>
            <td width="25%" valign="top" class="SubtituloDireita" ></td>
            <td class="SubtituloDireita esquerda" >
                <input type="button" value="Filtrar" name="btn_filtrar"/>
                <?php if($_POST){ ?>
                    <input type="button" value="Ver Todos" name="btn_ver_todos" />
                <?php } ?>
            </td>
        </tr>
        </table>
        <br>
            <?PHP 
                if($arrDados && ( in_array(PERFIL_SUPERUSUARIO,$arrPerfil) || in_array(PERFIL_ALTA_GESTAO,$arrPerfil) || in_array(PERFIL_REDE_FEDERAL_MINISTRO,$arrPerfil) || in_array(PERFIL_REDE_FEDERAL_SECR_EXECUTIVO,$arrPerfil) ) ){ 
            ?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
                    <td class="SubtituloDireita esquerda" >
                        <input type="checkbox" name="tbn_todos" id="tbn_todos" onclick="marcarTodos(this)"/>
                        <label for="tbn_todos">Marcar / Desmarcar Todos</label>
                    </td>
                    <td class="SubtituloDireita direita" >
                        <input type="button" value="Autorizar" name="btn_autorizar" />
                    </td>
		</tr>
            <?php } ?>
	</table>
</form>

<?php
    $db->monta_lista_array($arrDados, $cabecalho,50,10,'N','center');
    
    if($arrDados && in_array(PERFIL_ALTA_GESTAO,$arrPerfil)){ 
?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td valign="top" class="SubtituloDireita direita" >
                    <input type="button" value="Autorizar" name="btn_autorizar" />
                </td>
            </tr>
        </table>
<?php 
    } 
?>