<script type="text/javascript" src="./js/par.js"></script>
<?php
global $db;
if ($_POST['aderirpregaomi'] == 1) {

    set_time_limit(0);
    ini_set("memory_limit", "4000M");

    global $db;

    include_once(APPRAIZ . "par/classes/WSSigarp.class.inc");
    $oWSSigarp = new WSSigarp();

    if ($_POST['tipo'] == 'pac') {
        $proid = $db->pegaUm("SELECT 
                                ppc.proid
                            FROM 
                                par.termocompromissopac tcp
                            INNER JOIN par.processoobraspaccomposicao ppc ON ppc.proid = tcp.proid and ppc.pocstatus = 'A'
                            INNER JOIN obras.preobra pre ON pre.preid = ppc.preid
                            INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
                            WHERE
                                tcp.terstatus = 'A' AND pto.ptocategoria IS NOT NULL AND tcp.terid = {$_POST['id']}");

        if ($proid && $_POST['id']) {
            $sigarp = $oWSSigarp->solicitarManualmenteItemObra($_POST['id'], $proid, 'pac');
        } else {
            $sigarp = "Erro de sessão.";
        }
    } else {
        $proid = $db->pegaUm("SELECT 
                                ppc.proid
                            FROM 
                                par.vm_documentopar_ativos dp
                            INNER JOIN par.processoobrasparcomposicao ppc ON ppc.proid = dp.proid
                            INNER JOIN obras.preobra pre ON pre.preid = ppc.preid
                            INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
                            WHERE
                                pto.ptocategoria IS NOT NULL AND dp.dopid = {$_POST['id']}");

        if ($proid && $_POST['id']) {
            $sigarp = $oWSSigarp->solicitarManualmenteItemObra($_POST['id'], $proid, 'par');
        } else {
            $sigarp = "Erro de sessão.";
        }
    }
    echo $sigarp;
    die();
}

if ($_REQUEST['listadocumentoReformulado']) {
    header('content-type: text/html; charset=ISO-8859-1');
    $acoes = "'<img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" onclick=\"carregaTermoMinuta('||mi.dopid||');\" border=\"0\">' ";

    $perfil = pegaArrayPerfil($_SESSION['usucpf']);
    if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
            in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
            in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil)
    ) {
        $AcExluir = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\">";
        if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)) {
            $exluirSuper = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\">";
            $AcExluir = '';
        }
        $ExluirInativo = "<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
    }

    $imagemMais = "'<img style=\"cursor:pointer\" id=\"img_dimensao_' || mi.dopid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaDocumentoParReformulado(this.id,\'' || mi.dopid || '\');\" border=\"0\" />'";

    $sql = "SELECT DISTINCT
                CASE WHEN mi.dopidpai IS NOT NULL 
                THEN
                    CASE WHEN dv.dpvcpf IS NOT NULL THEN '<center>'||$imagemMais||'&nbsp;'||$acoes||'</center>' 
                        ELSE '<center>'||$imagemMais||'&nbsp;'||$acoes||'</center>'
                     END 
                ELSE
                    CASE WHEN dv.dpvcpf IS NOT NULL THEN '<center>'||$acoes||'</center>' 
                        ELSE '<center>'||$acoes||'</center>'
                    END 
                END AS acao,
                mo.mdonome ||' - '|| mi.dopnumerodocumento || '</td></tr>
                <tr style=\"display:none\" id=\"listaDocumentos_' || mi.dopid || '\" >
                        <td id=\"trV_' || mi.dopid || '\" colspan=8 ></td>
                </tr>' AS situacao
            FROM par.documentopar mi
            LEFT JOIN par.modelosdocumentos mo ON mo.mdoid = mi.mdoid
            INNER JOIN seguranca.usuario us ON us.usucpf = mi.usucpfinclusao
            LEFT JOIN par.documentoparvalidacao dv ON dv.dopid = mi.dopid AND dv.dpvstatus = 'A'
            WHERE (mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = " . $_REQUEST['dopid'] . " )
					OR mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = " . $_REQUEST['dopid'] . " )) 
				AND mi.dopstatus IN ('I')";
    $cabecalho = array("Acões", "Documento");
    $db->monta_lista($sql, $cabecalho, 20, 4, 'N', 'Center');
    exit;
}
?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/par.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript">
$(document).ready(function(){
    $('.historicoTermo').live('click', function(){
        var src = $(this).attr('src');
        var terid = $(this).attr('id');
        var linha = $(this).parent().parent();

        if( src === '../imagens/mais.gif' ){
            $(this).attr('src', '../imagens/menos.gif');
            var icone =  'listaDoc';
            $.ajax({
                type: "POST",
                url: 'par.php?modulo=principal/termoPac&acao=A',
                data: "requisicao=listaTermoFilho&terid="+terid+"&titid=1&icone="+icone,
                async: false,
                success: function(msg){
                    $(linha).after('<tr id=tr_'+terid+'><td colspan="7">'+msg+'</td></tr>');
                }
            });
        }else{
            $(this).attr('src', '../imagens/mais.gif');
            $('#tr_'+terid).remove();
        }
    });
});
    function removerValidacao(dopid) {
        if (confirm('Tem certeza que deseja remover a validação?')) {
            window.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A&dopid=' + dopid + '&exclui=S';
        }
    }
    function aderirPregaoMI(id, tipo) {
        divCarregando();
        if (confirm('Tem certeza que deseja aderir ao pregão?')) {
            jQuery.ajax({
                type: "POST",
                url: "par.php?modulo=principal/listaDocumentosParaAssinar&acao=A",
                data: "aderirpregaomi=1&tipo=" + tipo + "&id=" + id,
                async: false,
                success: function (msg) {
                    alert(msg);
                    window.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A';
                }
            });
        } else {
            divCarregado();
        }
    }
    function carregarListaDocumentoParReformulado(idImg, dopid) {
        var img = $('#' + idImg);
        var tr_nome = 'listaDocumentos_' + dopid;
        var td_nome = 'trV_' + dopid;

        if (jQuery('#' + tr_nome).css('display') === 'none') {
            img.attr('src', '../imagens/menos.gif');
            jQuery('#' + tr_nome).show();
            if (jQuery('#' + td_nome).html() === '') {
                jQuery('#' + td_nome).load('par.php?modulo=principal/listaDocumentosParaAssinar&acao=A&listadocumentoReformulado=true&dopid=' + dopid);
            }
        } else {
            img.attr('src', '../imagens/mais.gif');
            jQuery('#' + tr_nome).hide();
        }
    }

    function carregaTermoMinuta(dopid) {
        window.open('par.php?modulo=principal/teladeassinatura&acao=A&edita=1&dopid=' + dopid, 'termo', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
    }
</script>
<?php
#Cria Aba
$db->cria_aba($abacod_tela, $url, '');

# PAR
monta_titulo($titulo_modulo, 'Documentos do PAR');

# Perfil
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA, $perfil)) {
    $filtro = '';
} else {
    $filtro = ' and d.mdovalidacaogestor = true ';
}

if ($_GET['exclui'] == 'S') {
    $sql = "DELETE FROM 
                par.documentoparvalidacao 
            WHERE 
              dopid = " . $_REQUEST['dopid'];

    $db->executar($sql);
    if ($db->commit()) {
        echo "<script>
                alert('Operação realizada com sucesso!');
                window.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A';
            </script>";
        exit();
    } else {
        echo "<script>
                alert('Falha na operação!');
            </script>";
    }
}

if ($_REQUEST['estuf']) {
    $fitrosDoc = " and estuf = '{$_REQUEST['estuf']}'";
    $filtroPac = "AND tc.estuf = '" . $_REQUEST['estuf'] . "'";
} elseif ($_REQUEST['muncod']) {
    $fitrosDoc = " and muncod = '{$_REQUEST['muncod']}'";
    $filtroPac = "AND tc.muncod = '" . $_REQUEST['muncod'] . "'";
} else {
    if (!$_SESSION['par']['inuid']) {
        die("<script>
                alert('Problemas com variáveis. Clique a navegação novamente.');
                window.location='par.php?modulo=inicio&acao=C';
             </script>");
    }
    $fitrosDoc = " AND inuid = {$_SESSION['par']['inuid']} ";

    if ($_SESSION['par']['muncod']) {
        $filtroPac = "AND tc.muncod = '" . $_SESSION['par']['muncod'] . "'";
    } elseif ($_SESSION['par']['estuf']) {
        $filtroPac = "AND tc.estuf = '" . $_SESSION['par']['estuf'] . "'";
    }
}

$acoes = "'<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/teladeassinatura&acao=A&dopid='||id||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\">'";
$imagemMais = "'<img style=\"cursor:pointer\" id=\"img_dimensao_' || id || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaDocumentoParReformulado(this.id,\'' || id || '\');\" border=\"0\" />'";

$exclui = "''";
if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)) {
    $exclui = "'<img src=\"../imagens/excluir.gif\" title=\"Remover validação\" onclick=\"removerValidacao('||foo.id||')\">'";
}
//if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_PREFEITO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil ) ){
$sqlfiltro = " case when contagem >= mdoqtdvalidacao then '<img src=\"../imagens/obras/check.png\" title=\"Documento Validado\" width=\"18\">&nbsp;'||$exclui||'&nbsp;'||$acoes||'' else ''||$acoes||'' end ";
//}

$sqlfiltro = "CASE WHEN dopidpai IS NOT NULL 
		THEN
                    '<center>'||$imagemMais||'&nbsp;'||$sqlfiltro||'</center>'
		ELSE
                    '<center>'||$sqlfiltro||'</center>'
		END as acoes, ";

$celWidth = Array("10%", "20%", "70%");

# PAR
$sql = "SELECT
            $sqlfiltro doc, 
            tipodocumento || '</td></tr>
                <tr style=\"display:none\" id=\"listaDocumentos_' || id || '\" >
                        <td id=\"trV_' || id || '\" colspan=8 ></td>
            </tr>'
        FROM (
            SELECT dp.dopid as id, dp.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, dp.dopnumerodocumento as doc,
                (SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, dopidpai
            FROM par.vm_documentopar_ativos  dp
            INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
            INNER JOIN par.processopar pp ON pp.prpid = dp.prpid 
            INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
            --LEFT JOIN par.documentoparvalidacao dv ON dv.dopid = dp.dopid
            WHERE
                d.tpdcod IN (21, 102, 103, 16)
	) as foo
	WHERE 
            ( id NOT IN ( SELECT dopidreprogramado FROM par.reprogramacao WHERE repstatus = 'A' AND dopidreprogramado IS NOT NULL AND repdtfim IS NOT NULL) ) 
            AND ( id NOT IN ( SELECT dopidreprogramado FROM par.documentoparreprogramacao WHERE dprstatus = 'P' AND dopidreprogramado IS NOT NULL AND dopid = id ) ) 
            $fitrosDoc 
	UNION ALL
	SELECT
            $sqlfiltro doc, 
            tipodocumento || '</td></tr>
                <tr style=\"display:none\" id=\"listaDocumentos_' || id || '\" >
                        <td id=\"trV_' || id || '\" colspan=8 ></td>
            </tr>'
        FROM (
            SELECT distinct	dp.dopid as id, dp.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, dp.dopnumerodocumento as doc,
                (SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, dopidpai
            FROM par.vm_documentopar_ativos  dp
            INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
            INNER JOIN par.processopar pp ON pp.prpid = dp.prpid 
            INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
            INNER JOIN par.reprogramacao r ON r.dopidoriginal = dp.dopid AND r.repstatus = 'A' AND r.dopidreprogramado IS NOT NULL
            WHERE d.tpdcod IN (21, 102, 103, 16)
	) as foo
	WHERE 1 = 1 $fitrosDoc
	UNION ALL
	SELECT
            $sqlfiltro doc, 
            tipodocumento || '</td></tr>
            <tr style=\"display:none\" id=\"listaDocumentos_' || id || '\" >
                <td id=\"trV_' || id || '\" colspan=8 ></td>
            </tr>'
        FROM (
		SELECT distinct	dp.dopid as id, dp.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, dp.dopnumerodocumento as doc,
                    (SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, dopidpai
		FROM par.vm_documentopar_ativos  dp
		INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
		INNER JOIN par.processopar pp ON pp.prpid = dp.prpid 
		INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
		--LEFT JOIN par.documentoparvalidacao dv ON dv.dopid = dp.dopid
		INNER JOIN par.documentoparreprogramacao r ON r.dopid = dp.dopid AND r.dprstatus = 'P' AND r.dopidreprogramado IS NOT NULL
		WHERE
			d.tpdcod IN (21, 102, 103, 16)
	) as foo
	WHERE 1 = 1 $fitrosDoc ";

//if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_PREFEITO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil ) ){
$cabecalho = array("&nbsp;", "Nº do Documento", "Tipo de documento");
//} else {
//	$cabecalho = array("Nº do Documento", "Tipo de documento");
//}
// 	ver(simec_htmlentities($sql), d);
$dadosPar = $db->carregar($sql);
if ($dadosPar) {
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', '', '', '', $celWidth);
} else {
    $db->monta_lista_simples($sql, $cabecalho, 100, 5, 'N', '', '', '', $celWidth);
}


// Obras PAR
echo '<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela"><tr><td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">Documentos de Obras do PAR</td></tr></table>';

// Envio manual para o sigarp (MI)
if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)) {
    $acoesmi = " CASE WHEN ( SELECT count(processo) FROM par.processoobrasparcomposicao com INNER JOIN par.enviomiitemperdido eip ON eip.preid = com.preid WHERE com.proid = foo.proid ) > 0 THEN '<a href=\"javascript:aderirPregaoMI(\'' || dopid || '\', \'par\' )\" ><img src=\"../imagens/refresh2.gif\" title=\"Aderir ao pregão\" border=\"0\"></a>' ELSE '' END";
}

$sqlfiltro2 = " case when contagem >= mdoqtdvalidacao then '<img src=\"../imagens/obras/check.png\" title=\"Documento Validado\" width=\"18\">&nbsp;'||$exclui||'&nbsp;'||$acoes||'' else ''||$acoes||'' end ";

if ($acoesmi) {
    $acoesmi2 = "CASE WHEN dopidpai IS NOT NULL 
			THEN
				'<center>'||$imagemMais||'&nbsp;'||$sqlfiltro2||'&nbsp;'||$acoesmi||'</center>'
			ELSE
				'<center>'||$sqlfiltro2||'&nbsp;'||$acoesmi||'</center>'
			END as acoes, ";
} else {
    $acoesmi2 = "CASE WHEN dopidpai IS NOT NULL 
			THEN
				'<center>'||$imagemMais||'&nbsp;'||$sqlfiltro2||'</center>'
			ELSE
				'<center>'||$sqlfiltro2||'</center>'
			END as acoes, ";
}


$sql = "SELECT
			$acoesmi2 doc, tipodocumento|| '</td></tr>
            	<tr style=\"display:none\" id=\"listaDocumentos_' || id || '\" >
            		<td id=\"trV_' || id || '\" colspan=8 ></td>
            </tr>'
		 FROM (
	SELECT dp.dopid as id, iu.inuid, iu.estuf, dp.mdonome as tipodocumento, iu.muncod, dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, (SELECT dopnumerodocumento FROM par.documentopar WHERE proid = dp.proid and dopstatus <> 'E' order by dopid asc LIMIT 1) as doc,
			(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, dopidpai, pp.proid, dp.dopid
	FROM par.vm_documentopar_ativos  dp
		INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
		INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
		INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
		--LEFT JOIN par.documentoparvalidacao dv ON dv.dopid = dp.dopid
		WHERE
			d.tpdcod in (21, 102, 103, 16)
	) as foo
	WHERE 1=1 $fitrosDoc ";

//if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_PREFEITO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil ) ){
$cabecalho = array("&nbsp;", "Nº do Documento", "Tipo de documento");
//} else {
//	$cabecalho = array("Nº do Documento", "Tipo de documento");
//}
// ver($sql,d);
$dadosObrasPar = $db->carregar($sql);
if ($dadosObrasPar) {
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', '', '', '', $celWidth);
}else{
    $db->monta_lista_simples($sql, $cabecalho, 100, 5, 'N', '', '', '', $celWidth);
}


//PAC
echo '<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela"><tr><td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">Documentos do PAC</td></tr></table>';
//
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

$acoes = "'<img src=../imagens/icone_lupa.png title=\"Visualizar\" style=cursor:pointer; 
				onclick=\"window.open(\'par.php?modulo=principal/teladevalidacao&acao=A&terid='||tc.terid||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\">'";

// Envio manual para o sigarp (MI)
if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)) {
    $acoes .= " || CASE WHEN ( SELECT count(processo) FROM par.processoobraspaccomposicao com INNER JOIN par.enviomiitemperdido eip ON eip.preid = com.preid WHERE com.pocstatus = 'A' and com.proid = tc.proid ) > 0 
						THEN '<a href=\"javascript:aderirPregaoMI(\'' || tc.terid || '\', \'pac\')\" ><img src=\"../imagens/refresh2.gif\" title=\"Aderir ao pregão\" border=\"0\"></a>' ELSE '' END";
}

if (!empty($_SESSION['par']['muncod'])) {
    $codParam1 = $_SESSION['par']['muncod'];
    $codParam2 = 'false';
} else {
    $codParam1 = 'false';
    $codParam2 = $_SESSION['par']['estuf'];
}

$sql = "SELECT 
            '<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"listaTermoHistorico(\'$codParam1\', \'$codParam2\', \'' || tc.proid || '\',this);\"></center>' as mais,
            CASE WHEN tc.usucpfassinatura IS NOT NULL OR tc.terassinado = 't' 
                THEN '<center><img src=\"../imagens/obras/check.png\" title=\"Documento Validado\" width=\"18\">'||$acoes||'</center>' 
                ELSE '<center>'||$acoes||'</center>' 
        END as acoes,
                (
                    SELECT par.retornanumerotermopac(tc.proid)
                ) as ternum,
                CASE WHEN tc.terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE to_char(tc.terdataassinatura, 'DD/MM/YYYY') END as data,
                CASE WHEN tc.terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE u.usunome END as usu
        FROM 
                par.termocompromissopac  tc
        LEFT JOIN seguranca.usuario 		u   ON u.usucpf = tc.usucpfassinatura 
        LEFT JOIN par.termocompromissopac  	tcp ON tcp.terid = tc.teridpai
        WHERE 
                tc.terstatus = 'A'	
                " . $filtroPac;

$cabecalho = array("&nbsp;", "&nbsp;", "Nº do Documento", "Data da Validação", "Usuário da Validação");
$celWidth = Array("10%", "20%", "50%", "20%");

$db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '', '', '', $celWidth, '', true);