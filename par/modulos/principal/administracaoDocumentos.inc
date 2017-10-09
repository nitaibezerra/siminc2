<?php 

if($_REQUEST['requisicao'] == 'ativaAcompanhamento')
{
	$dopid = $_POST['dopid'];
	$sql = "update par.documentopar set dopacompanhamento = true where dopid = " . $dopid;
	$db->executar($sql);
	if($db->commit())
	{
		die('ok');
	}
	else
	{
		die('erro');
	}
	exit();
}

function enviaEmailReprogramacaoBloqueada(){

	$db = new cls_banco ();
	$inuid 			= $_SESSION['par']['inuid'];
	$prpid 			=  $_REQUEST['prpid'];
	$numprocesso 	=  $_REQUEST['numprocesso'];
	$tipo 			=  $_REQUEST['tipo'];
	
	$sqlFinalizado = "SELECT prpfinalizado FROM par.processopar WHERE prpid = $prpid";
	$finalizado = $db->pegaUm($sqlFinalizado);
	
	if($finalizado == 't' )
	{
            echo 'Este processo encontra-se finalizado, favor entrar em contato com a equipe do FNDE';
            die();
	}
	
	$sqlErbid = "SELECT erbid FROM par.emailreprogramacaobloqueada WHERE erbnumeroprocesso = '{$numprocesso}' AND erbstatus = 'A'";
	
	if($db->pegaUm($sqlErbid))
	{
            if ($tipo == 'prazo'){
		echo 'Não é possível solicitar reprogramação de prazo visto que este Termo de Compromisso já teve sua vigência expirada.<br>   Orientamos que proceda a devolução de possíveis saldos e rendimentos.';
            } else {
		echo 'Não é possível solicitar reprogramação de subação visto que este Termo de Compromisso já teve sua vigência expirada.<br>   Orientamos que proceda a devolução de possíveis saldos e rendimentos.';
            }
		die();
	}
	
	
	if( !empty($inuid) ){
		
		$sql = "SELECT
					iu.itrid,
				CASE WHEN iu.itrid = 2 THEN
					iu.muncod
				WHEN
					iu.itrid = 1 THEN
					iu.estuf
				END as filtro
				FROM
					par.instrumentounidade iu
				WHERE
					inuid = {$inuid}
		";
		$result = $db->pegaLinha($sql);
		$itrid = $result['itrid'];
		$filtro = $result['filtro'];
	
	
		if( ($itrid == 2) && ($filtro)  )
		{
			// 
			$sqlEmail = "SELECT
					distinct ent.entemail as email
				FROM
					par.entidade ent
				INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
				INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
				WHERE
					ent.dutid in( 7,2)
					and ent.entstatus = 'A'
				AND
				mun.muncod in ( '{$filtro}' )
				AND 
					ent.entemail <> ''
				AND 
					ent.entemail is not null
		";
		}
		else if( ($itrid == 1) && ($filtro))
		{
			$sqlEmail = "
				SELECT
					distinct ent.entemail as email
				FROM
					par.entidade ent
				INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
				INNER JOIN territorios.estado est on est.estuf = ent2.estuf
				
				WHERE
					ent.entstatus='A'
				AND
					ent.dutid in( 9,10)
				AND
					ent2.estuf in ( '{$filtro}' )
				AND 
					ent.entemail <> ''
				AND 
					ent.entemail is not null
			";
		}
	
		$resultEmail = $db->carregar($sqlEmail);
		
		$resultEmail = ($resultEmail ) ? $resultEmail  : Array();
		
		if(count($resultEmail) > 0)
		{
			foreach($resultEmail as $k => $v)
			{
				$arrEmail[] = $v['email']; 
			}
		}
		else
		{
			echo '   Não é possível solicitar reprogramação de prazo ou de subação visto que este Termo de Compromisso já teve sua vigência expirada.<br>   Orientamos que proceda a devolução de possíveis saldos e rendimentos.';
			die();
		}
			
			$sql = "SELECT
			 				CASE WHEN dp2.dopano::boolean THEN
			 					dp.dopnumerodocumento::text || '/' || dp2.dopano::text
			 				ELSE
			 					dp.dopnumerodocumento::text
			 				END
			 					as ndocumento
			 			FROM par.documentopar dp
				
						LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
			 			WHERE dp.dopid = (select dopid from par.documentopar where prpid = {$prpid} and dopstatus = 'A' limit 1)";
			
			$numTermo = $db->pegaUm($sql);
			
			$strMensagem = "<pre><p style=\"text-align: justify;\">Prezados,<br>
						        &nbsp;&nbsp;Não é possível solicitar reprogramação de prazo ou de subação visto que este Termo de Compromisso já teve sua vigência expirada.<br>
						        &nbsp;&nbsp;Orientamos que proceda a devolução de possíveis saldos e rendimentos.<br>
								&nbsp;&nbsp;Número do termo: {$numTermo}<br>
							Atenciosamente,<br>
							Equipe do PAR
							</p></pre>";
			
			$remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
			
			$strMensagem = html_entity_decode($strMensagem);

			$strAssunto = "Reprogramação do termo {$numTermo} bloqueada";
			
			$sqlEmailEnviado = "INSERT into par.emailreprogramacaobloqueada 
									( erbdatainclusao, erbnumeroprocesso)
								VALUES
									( 'now()' , '{$numprocesso}')";
			
			if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" )
			{	
				$db->executar($sqlEmailEnviado);
				$db->commit();
				echo 'Não é possível solicitar reprogramação de prazo ou de subação visto que este Termo de Compromisso já teve sua vigência expirada. <br> Orientamos que proceda a devolução de possíveis saldos e rendimentos.';
				die();
			}
			elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
			{
				$strEmailTo = array($_SESSION['email_sistema']);
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
				if($retorno)
				{
					$db->executar($sqlEmailEnviado);
					$db->commit();
				}
				echo 'Não é possível solicitar reprogramação de prazo ou de subação visto que este Termo de Compromisso já teve sua vigência expirada. <br> Orientamos que proceda a devolução de possíveis saldos e rendimentos.';
				die();
			}
			else
			{
				$strEmailTo = $arrEmail;
				$retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
				if($retorno)
				{
					$db->executar($sqlEmailEnviado);
					$db->commit();
				}
				echo 'Não é possível solicitar reprogramação de prazo ou de subação visto que este Termo de Compromisso já teve sua vigência expirada. <br> Orientamos que proceda a devolução de possíveis saldos e rendimentos.';
				die();
			}
	
						
		} else {
			
			echo 'Erro de sessão';die();
		}
	
	global $db;
	
	echo 'Não é possível solicitar reprogramação de prazo ou de subação visto que este Termo de Compromisso já teve sua vigência expirada. <br> Orientamos que proceda a devolução de possíveis saldos e rendimentos.';
	
	die();
}
function mostraDialogo(){
	global $db;
	
	$sql = "SELECT dopnumerodocumento FROM par.documentopar WHERE dopid = {$_POST['dopid']}";
	
	$numdocumento = $db->pegaUm($sql);

	monta_titulo( "N° do Documento: $numdocumento", "" );
?>
<table align="center" cellspacing="0" cellpadding="5" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
	<tr>
		<td align="center" bgcolor="white"><b>Solicitações de Prazo</b>
		</td>
	</tr>
</table>
<?php
	$sql = "SELECT DISTINCT
				to_char(dpr.dprdata, 'DD/MM/YYYY') as data_criacao,
				to_char(dpr.dprdataprazo, 'MM/YYYY') as novo_prazo,
				dpr.dprjustificativa as justificativa
			FROM 
				par.documentoparreprogramacao dpr
			WHERE
				dprjustificativa IS NOT NULL AND dprstatus = 'A'
				AND dopid = {$_POST['dopid']}";

	$cabecalho = array("Data da Solicitação", "Novo Prazo (MM/AAAA)", "Justificativa" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N', "","","",null, Array('ordena'=> false) );	
?>
<table align="center" cellspacing="0" cellpadding="5" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
	<tr>
		<td align="center" bgcolor="white"><b>Solicitações de Subação</b>
		</td>
	</tr>
</table>	
<?php

	$sql = "SELECT DISTINCT
				sba.sbaid as codigo,
				par.retornacodigosubacao( sba.sbaid )||' '||sbadsc as descricao
			FROM 
				par.documentoparreprogramacaosubacao dps
			INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = dps.sbdid
			INNER JOIN par.subacao sba ON sba.sbaid = sbd.sbaid AND sbastatus = 'A'
			WHERE
				dpsjustificativa IS NOT NULL AND dpsstatus = 'A'
				AND dopid = {$_POST['dopid']}";
//                                ver($sql, d);
	$sbaids = $db->carregar($sql);
				
	if( is_array($sbaids) ){
?>
<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
<?php
		foreach( $sbaids as $k => $sbaid ){
			
?>
	<tr>
		<td bgcolor="#f5f5f5"><b><?=$sbaid['descricao'] ?></b></td>
	</tr>
	<tr>
		<td align="center" bgcolor="white">
<?php
			$sql = "SELECT DISTINCT
						sbdano as ano,
						to_char(dps.dpsdata, 'DD/MM/YYYY') as datacriacao,
						dps.dpsjustificativa as justificativa
					FROM 
						par.documentoparreprogramacaosubacao dps
					INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = dps.sbdid
					WHERE
						dpsjustificativa IS NOT NULL
						AND dopid = {$_POST['dopid']}
						AND sbaid = {$sbaid['codigo']}";
//		ver($sql,d);
			$cabecalho = array("Ano da Subação", "Data da Solicitação", "Justificativa" );
			$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
		}
?>
		</td>
	</tr>
</table>
<?php
	}
	?>
	<table align="center" cellspacing="0" cellpadding="5" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
		<tr>
			<td align="center" bgcolor="white"><b>Histórico de Reprogramação/Prorrogação</b>
			</td>
		</tr>
	</table>
		
	
	<?php 
	if($_POST['dopid'])
	{	
		$prpidLog = $db->pegaUm(" SELECT prpid from par.documentopar where dopid = {$_POST['dopid']}");
		
		if($prpidLog)
		{
			$sql = " SELECT
						'<img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" onclick=\"abreProtocolo('||hsr.hsrid||');\" border=\"0\">' as acao, 
						hsrprotocolo,
						to_char(hsrdata, 'DD/MM/YYYY HH24:MI') as data_solicitacao,
						usucpf || ' - ' || usunome as nome_usuario,
						CASE WHEN hsrtipo = 'S' THEN
							'Subação'
						ELSE
							'Prazo'
						END AS tipo,
						CASE WHEN hsrtipo = 'S' THEN
							array_to_string( 
								array(SELECT par.retornacodigosubacao(s.sbaid) || ' - ' ||  sbadsc  FROM par.historicosolicitacaoreprogramacaosubacao hrs
									INNER JOIN par.subacao s ON s.sbaid = hrs.sbaid where hsr.hsrid = hrs.hsrid )
									, '</br>' 
							) 
						ELSE
							''
						END
						as subacoes,
						(SELECT
							CASE WHEN dp2.dopano::boolean THEN
							dp.dopnumerodocumento::text || '/' || dp2.dopano::text
						ELSE
							dp.dopnumerodocumento::text
						END
							as ndocumento
						FROM 
							par.documentopar dp
					
						LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
						WHERE dp.dopid = hsr.dopid) as numero_doc,
						
						hsrjustificativa as justificativa
					
					FROM par.historicosolicitacaoreprogramacao hsr
					INNER JOIN seguranca.usuario u ON hsr.hsrcpf = u.usucpf where  prpid = {$prpidLog}";
			
				$cabecalho = array("Ação", "Nº Protocolo", "Data Solicitação", "CPF/Nome Solicitante", "Tipo", "Subações", "Nº do Documento", "Justificativa" );
				$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N', "","","",null, Array('ordena'=> false) );
		}
		
	}

}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

$bloqueiaParaConsultaEstMun = false;
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
if (in_array(  PAR_PERFIL_CONSULTA_ESTADUAL, $perfil ) || in_array(  PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil ) )
{
	$bloqueiaParaConsultaEstMun = true;
}

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( ! $bloqueiaParaConsultaEstMun )
{
	if($_GET['download'] == 'S'){
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
	    echo"<script>window.location.href = 'par.php?modulo=principal/administracaoDocumentos&acao=A';</script>";
	    exit;
	}
	
	
	if( $_POST['aderirpregaomi'] == 1 ){
		
		set_time_limit(0);
		ini_set("memory_limit", "4000M");
		
		global $db;
		
		include_once(APPRAIZ."par/classes/WSSigarp.class.inc");
		$oWSSigarp = new WSSigarp();
		
		$proid = $db->pegaUm("select 
									ppc.proid
								from 
									par.termocompromissopac tcp
								INNER JOIN par.processoobraspaccomposicao ppc ON ppc.proid = tcp.proid and ppc.pocstatus = 'A'
								INNER JOIN obras.preobra pre ON pre.preid = ppc.preid
								INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
								WHERE
									tcp.terstatus = 'A' AND pto.ptocategoria IS NOT NULL AND tcp.terid = {$_POST['terid']}");
		
		if( $proid && $_POST['terid'] ){
			$sigarp = $oWSSigarp->solicitarManualmenteItemObra($_POST['terid'], $proid);
		} else {
			$sigarp = "Erro de sessão.";
		}
			
		echo $sigarp;
		die();
	}
	
}
	if($_REQUEST['listadocumentoReformulado']){

		header('content-type: text/html; charset=ISO-8859-1');
		$acoes = "'<img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" onclick=\"carregaTermoMinuta('||mi.dopid||');\" border=\"0\">' ";
		
		$perfil = pegaArrayPerfil($_SESSION['usucpf']);
		if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || 
			in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || 
			in_array( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil )
		){
			$AcExluir = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\">";
			if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil )){
				$exluirSuper = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\">";
				$AcExluir = '';
			}
			$ExluirInativo = "<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
		}
		
		$imagemMais = "'<img style=\"cursor:pointer\" title=\"mais\" id=\"img_dimensao_' || mi.dopid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaDocumentoParReformulado(this,''' || mi.dopid || ''');\" border=\"0\" />'";
		
		$sql = "Select	DISTINCT
					CASE WHEN mi.dopidpai IS NOT NULL 
					THEN
						case when dv.dpvcpf is not null then '<center>'||$imagemMais||'&nbsp;'||$acoes||'</center>' 
							else '<center>'||$imagemMais||'&nbsp;'||$acoes||'</center>'
						 END 
					ELSE
						case when dv.dpvcpf is not null then '<center>'||$acoes||'</center>' 
							else '<center>'||$acoes||'</center>'
						 END 
					END as acao,
				    mo.mdonome ||' - '|| mi.dopnumerodocumento,
				   'Reformulado'
				    as situacao
				From par.documentopar mi
			    Left Join par.modelosdocumentos mo on mo.mdoid = mi.mdoid
			    Inner Join seguranca.usuario us on us.usucpf = mi.usucpfinclusao
			    left join par.documentoparvalidacao dv on dv.dopid = mi.dopid and dv.dpvstatus = 'A'
				Where (mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = ".$_REQUEST['dopid']." )
						or mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = ".$_REQUEST['dopid']." )) 
					and mi.dopstatus IN ('I')";
		//ver(simec_htmlentities($sql),d);
		$cabecalho = array("Acões", "Documento", "Situação");
		$db->monta_lista_simples($sql, $cabecalho,1000000,5,'N','100%','S', false, false, false, false);	
		exit;
	}
?>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script>

function abreProtocolo( hsrid )
{
	window.open('par.php?modulo=principal/popUpProtocoloReprogramacao&acao=A&hsrid=' + hsrid ,  '' ,'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
}

function enviaEmailReprogramacaoBloqueada( prpid, numprocesso, tipo){

	jQuery.ajax({
   		type: "POST",
   		url: 'par.php?modulo=principal/administracaoDocumentos&acao=A',
   		data: "&req=enviaEmailReprogramacaoBloqueada&prpid="+prpid+"&numprocesso="+numprocesso+"&tipo="+tipo,
   		async: false,
   		success: function(msg){
   			msg = msg.replace("<br>", "\n", "g" );
			alert(msg);
   			
   		}
	});
	
	
}

function removerValidacao(dopid){
	if(confirm('Tem certeza que deseja remover a validação?')){
		window.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A&dopid='+dopid+'&exclui=S';
	}
}
function abreReprogramacao(dopid, tipo){
    if( dopid != '' && dopid != undefined ){
        if (tipo == 'prazo') {
            window.open("par.php?modulo=principal/popupReprogramacaoPAR&acao=A&dopid="+dopid,"reprogramacaoPar","height=400,width=600,scrollbars=yes,top=50,left=200");
        } else {
            window.open("par.php?modulo=principal/popupReprogramacaoPARSubacoes&acao=A&dopid="+dopid,"reprogramacaoPar","height=400,width=600,scrollbars=yes,top=50,left=200");
        }
    }
}


function abreObra(preid, tooid, sbaid, sobano)
{

    if( tooid == 1 ){
		url = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+preid;
    }else{
		url = 'par.php?modulo=principal/subacaoObras&acao=A&preid='+preid+'&ano='+sobano+'&sbaid='+sbaid;
    }
	window.open(url, 'popupAvisoObras', "height=600,width=930,scrollbars=yes,top=50,left=200" );
}
function abreAcompanhamento( dopid , dopacompanhamento)
{
	if( ! dopacompanhamento)
	{
		if(confirm("Deseja realmente iniciar o acompanhamento para este termo?"))
		{
			jQuery.ajax({
		   		type: "POST",
		   		url: "par.php?modulo=principal/administracaoDocumentos&acao=A",
		   		data: "requisicao=ativaAcompanhamento&dopid=" + dopid,
		   		async: false,
		   		success: function(msg)
		   		{
		   			if(msg == 'ok')
		   			{
		   				window.location.href = 'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&dopid='+dopid;
		   			}
		   			if(msg == 'erro')
		   			{
		   				alert('Houve um erro na solicitação');
		   			}
		   			
		   		}
		   	});
		   	
			
		}
	}
	else
	{
		window.location.href = 'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&dopid='+dopid;
		
	}
}

function aderirPregaoMI(terid){
	divCarregando();
	if(confirm('Tem certeza que deseja aderir ao pregão?')){
		jQuery.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/listaDocumentosParaAssinar&acao=A",
	   		data: "aderirpregaomi=1&terid=" + terid,
	   		async: false,
	   		success: function(msg){
	   			alert(msg);
	   			window.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A';
	   		}
	   	});
	} else {
		divCarregado();
	}
}
function carregaTermoMinuta(dopid){
	window.open('par.php?modulo=principal/teladeassinatura&acao=A&edita=1&dopid='+dopid,'termo','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}

function carregarListaDocumentoParReformulado(obj, dopid){
	/*var img 	 = jQuery('#'+idImg);
	var tr_nome = 'listaDocumentos_'+ dopid;
	var td_nome  = 'trV_'+ dopid;

	if(jQuery('#'+tr_nome).css('display') == 'none'){
		img.attr('src', '../imagens/menos.gif');
		jQuery('#'+tr_nome).show();
		if(jQuery('#'+td_nome).html() == ''){
			jQuery('#'+td_nome).load('par.php?modulo=principal/administracaoDocumentos&acao=A&listadocumentoReformulado=true&dopid='+dopid);
		}
	} else {
		img.attr('src', '../imagens/mais.gif');
		jQuery('#'+tr_nome).hide();
	}*/

	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=10;
		ncol.innerHTML="Carregando...";
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "listadocumentoReformulado=true&dopid="+dopid,
			async: false,
			success: function(msg){
			ncol.innerHTML="<div id='listatermoprocesso_" + dopid + "' >" + msg + "</div>";
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
}

function mostraAvisosSU(){

	
    jQuery(function(){
                jQuery("#dialog_vigencia").dialog({
                    modal: true,
                    width: '90%',
					height: 700,
                    open: function(){
			            jQuery('.ui-widget-overlay').bind('click',function(){
			                jQuery('#dialog_vigencia').dialog('close');
			            })
			        },
                    buttons: {
                        Fechar: function() {
                            jQuery("#dialog_detalhe_processo").html('');
                            jQuery( this ).dialog( "close" );
                        }
                    }
                });
		 
    });

}
</script>
<?php

// Este parâmetro ($arrAbas) irá servir para retirar a aba "Acompanhamento" que só deverá estar visível quando ela mesma estiver ativa
$arrAbas = array( PAR_ESCONDE_ABA_ACOMPANHAMENTO );
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
if( in_array( array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO), $perfil )){
	$arrAbas = array( PAR_ESCONDE_ABA_ACOMPANHAMENTO, 15356 );
}
$db->cria_aba( $abacod_tela, $url, '', $arrAbas );
//PAR
$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);

if( $_SESSION['par']['inuid'] == '' ){
	echo "
		<script>
			alert('Erro de sessão.');
			window.location.href = 'par.php?modulo=principal/listaMunicipios&acao=A';
		</script>";
	die();
}

$inuid = $_SESSION['par']['inuid'];
$sql = "select
			case when iue.iuesituacaohabilita = 'Habilitado' then
				'<img src=../imagens/workflow/1.png title=\"'||iue.iuesituacaohabilita||'\" style=cursor:pointer;>'
			else
				'<img src=../imagens/workflow/2.png title=\"'||iue.iuesituacaohabilita||'\" style=cursor:pointer;>'
			end as habilitado
			from
				 par.instrumentounidadeentidade iue
			WHERE
				inuid = {$inuid} AND iue.iuedefault = true";
$habilitado = $db->pegaUm($sql);
// Monta tarja vermelha com aviso de pendência de obras

$arrParam = array();
if($_SESSION['par']['itrid'] == 1)
{
	$esferaRestricao = 'E';
	$muncodRestricao = false;
	$ufRestricao = $_SESSION['par']['estuf'];
        $arrParam['estuf'] = $_SESSION['par']['estuf'];
}
else
{
	
	// verificar CACS somente nos municípios
	/* $cacs = adapterConnection::coorporativo()->carregar("SELECT DISTINCT sit_mandato FROM integracao.cacs WHERE co_mun_ibge_conselho = '".$_SESSION['par']['muncod']."'");
	
	$sit_cacs = $cacs[0]['sit_mandato'];
	
	if($sit_cacs=='REGULAR') {
		$resultado_cacs = '<b style=\'margin-left:40px\'>CACS:</b> <img onclick="javascript:alert(\'Situação do município : REGULAR\')" style="cursor:pointer;" title="Regular" src="../imagens/workflow/1.png">';
	} else {
		$resultado_cacs = '<b style=\'margin-left:40px\'>CACS:</b> <img onclick="javascript:alert(\'Situação do município : '.$sit_cacs.'\')" style="cursor:pointer;" title="Irregular" src="../imagens/workflow/2.png">';
	} */
	
	
	$esferaRestricao = 'M';
	$muncodRestricao =  $_SESSION['par']['muncod'];
	$ufRestricao = $_SESSION['par']['estuf'];
    $arrParam['muncod'] = $_SESSION['par']['muncod'];
}

$resultado = montarAvisoCabecalho($esferaRestricao, $muncodRestricao, $ufRestricao, null, null, null, 'acompanhamento');

if(! $resultado )
{
	$resultado = '<b style=\'margin-left:40px\'>Pendência de Obras:</b> <img onclick="javascript:alert(\'Não existem pendências de obras\')" style="cursor:pointer;" title="Habilitado" src="../imagens/workflow/1.png">';
}
else 
{
	
}


# Verifica se o município posssui Plano de Educacao
$booResultado = hasPlanoEducacao($arrParam);
$strPlanoEducacao = '';
if ($booResultado) {
    $strPlanoEducacao = " <b style='margin-left:40px'>Plano de Educação:</b> <img style=\"cursor:pointer;\" title=\"Plano de Educação\" src=\"../imagens/workflow/1.png\">";
} else {
    $strPlanoEducacao = " <b style='margin-left:40px'>Plano de Educação:</b> <img style=\"cursor:pointer;\" title=\"Plano de Educação\" src=\"../imagens/workflow/2.png\">";
}


$avisos = "<span style='display:none;' id='botao_avisos'  onclick=\"javascript:mostraAvisosSU()\" > <a href='#' style='color:black;'> <b> <br><br><img src=\"../imagens/Post-It-icon.png\" style=\"width:19px;\" title=\"Remover validação\" >  Avisos/Pendências </b></a></span>";
//monta_titulo( $nmEntidadePar, " <b>Entidade Habilitada:</b>  $habilitado $resultado &nbsp; $strPlanoEducacao" );
monta_titulo( $nmEntidadePar, " <b>Entidade Habilitada:</b> $habilitado &nbsp; $resultado &nbsp; $strPlanoEducacao &nbsp; $avisos &nbsp; $resultado_cacs " );

monta_titulo( $titulo_modulo, 'Documentos do PAR' );
//
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_EQUIPE_TECNICA, $perfil )){
	$filtro = '';
} else {
	$filtro = ' and d.mdovalidacaogestor = true ';
}

if( $_GET['exclui'] == 'S' ){
	$sql = "DELETE FROM 
				par.documentoparvalidacao 
			WHERE 
			  dopid = ".$_REQUEST['dopid'];
	
	$db->executar( $sql );
	if($db->commit()){
		echo "<script>
					alert('Operação realizada com sucesso!');
					window.location.href = 'par.php?modulo=principal/listaDocumentosParaAssinar&acao=A';
				</script>";
		exit();
	} else{
		echo "<script>
					alert('Falha na operação!');
				</script>";
	}
}

if( $_REQUEST['estuf'] ){
	$fitrosDoc = " and estuf = '{$_REQUEST['estuf']}'";
	$filtroPac = "AND tc.estuf = '".$_REQUEST['estuf']."'";
}elseif( $_REQUEST['muncod'] ){
	$fitrosDoc = " and muncod = '{$_REQUEST['muncod']}'";
	$filtroPac = "AND tc.muncod = '".$_REQUEST['muncod']."'";
} else {
	if(!$_SESSION['par']['inuid']) {
		
		die("<script>
				alert('Problemas com variáveis. Clique a navegação novamente.');
				window.location='par.php?modulo=inicio&acao=C';
			 </script>");
	
	}
	$fitrosDoc = " AND inuid = {$_SESSION['par']['inuid']} ";
	
	if( $_SESSION['par']['muncod'] ){
		$filtroPac = "AND tc.muncod = '".$_SESSION['par']['muncod']."'";
	} elseif( $_SESSION['par']['estuf'] ){
		$filtroPac = "AND tc.estuf = '".$_SESSION['par']['estuf']."'";
	}
}

$acaomais = "CASE WHEN dopidpai IS NOT NULL THEN '<img style=\"cursor:pointer\" title=\"mais\" id=\"img_dimensao_' || id || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaDocumentoParReformulado(this,''' || id || ''');\" border=\"0\" />' ELSE '' END";
$acoes = "'<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/teladeassinatura&acao=A&dopid='||id||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\">'";

$exclui = "''";
if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ){
	$exclui = "'<img src=\"../imagens/excluir.gif\" title=\"Remover validação\" onclick=\"removerValidacao('||foo.id||')\">'";
}
if( ! $bloqueiaParaConsultaEstMun )
{
	$sqlfiltro =	"case when contagem = mdoqtdvalidacao then '<center>'||$acaomais||'&nbsp;<img src=\"../imagens/obras/check.png\" title=\"Documento Validado\" width=\"18\">&nbsp;'||$exclui||'&nbsp;'||$acoes||'</center>' else '<center>'||$acaomais||'&nbsp;'||$acoes||'</center>' end as acoes,";
}
else
{
    if( in_array(  PAR_PERFIL_CONSULTA_ESTADUAL, $perfil ) || in_array(  PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil )|| in_array( PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA, $perfil )){
        $sqlfiltro = "case when contagem = mdoqtdvalidacao then '<center>'||$acaomais||'&nbsp;<img src=\"../imagens/obras/check.png\" title=\"Documento Validado\" width=\"18\">&nbsp;&nbsp;'||$acoes||'</center>' else '<center>'||$acaomais||'&nbsp;'||$acoes||'</center>' end as acoes,";
    } else {
        $sqlfiltro = " '' as acoes,";
    }
}
$botaoreprogramacao = "''";
if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_PREFEITO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) ){
	$botaoreprogramacao = "CASE WHEN id IS NOT NULL 
            THEN '<center>' ||  
                CASE WHEN (SELECT 
                         to_date(dopdatafimvigencia,'MM/YYYY') - to_date('".date('m/Y')."','MM/YYYY') as dia 
                    FROM par.documentopar
                    WHERE dopid = id) > 90 
                    THEN '<input type=\"button\" class=\"btn-gray\"  style=\"border: solid #a3a3a3 1px;\" value=\"Prazo\" onclick=\"alert(\'Não é possível solicitar Reprogramação de Prazo, pois o Termo de Compromisso vigente possui mais de 90 dias para o vencimento.\')\">'
                    ELSE '<input type=\"button\" value=\"Prazo\" style=\"background-color: red; border-color:red;\" onclick=\"abreReprogramacao(' || id ||', \'prazo\')\">'
                END
                    || 
                    '<input type=\"button\" value=\"Subações\" onclick=\"abreReprogramacao(' || id || ')\">
                </center>' 
            ELSE '' 
        END";
}
else if(  $bloqueiaParaConsultaEstMun  )
{
	$botaoreprogramacao = "CASE WHEN id IS NOT NULL 
            THEN '<center>' ||  
                CASE WHEN (SELECT 
                         to_date(dopdatafimvigencia,'MM/YYYY') - to_date('".date('m/Y')."','MM/YYYY') as dia 
                    FROM par.documentopar
                    WHERE dopid = id) > 90 
                    THEN '<input type=\"button\" class=\"btn-gray\"  style=\"border: solid #a3a3a3 1px;\" value=\"Prazo\" onclick=\"alert(\'Não é possível solicitar Reprogramação de Prazo, pois o Termo de Compromisso vigente possui mais de 90 dias para o vencimento.\')\">'
                    ELSE '<input type=\"button\" value=\"Prazo\" class=\"btn-gray\"  style=\"border: solid #a3a3a3 1px;\" value=\"Prazo\" onclick=\"alert(\'Esta ação não está disponível para seu perfil.\')\">'
                END
                    || 
                    '<input type=\"button\" value=\"Subações\" class=\"btn-gray\"  style=\"border: solid #a3a3a3 1px;\" onclick=\"alert(\'Esta ação não está disponível para seu perfil.\')\">
                </center>' 
            ELSE '' 
        END";
}


// Colocando sql da coluna saldo em um string pois é utilizada em 3 lugares diferentes
$campoSaldo = " (
                    select coalesce(saldo, 'Não Informado') as saldo from
                    (
                            select par.retornasaldoprocesso(prpnumeroprocesso) AS saldo
                    ) as saldomes
                ) ";

// PAR
$sql = "SELECT
			$sqlfiltro 
			'<span class=\"processo_detalhe\">' || prpnumeroprocesso || '</span>' as processo,
			doc, 
			tipodocumento, 
			CASE WHEN  
				( (date_trunc('MONTH', to_date(data_vigencia,'MM/YYYY')) + INTERVAL '1 MONTH - 1 day')::date - current_date) < 90 
				AND ( (date_trunc('MONTH', to_date(data_vigencia,'MM/YYYY')) + INTERVAL '1 MONTH - 1 day')::date - current_date) > 0
			THEN
				'<span style=\"color:red\">'	|| data_vigencia || '</span>'
			ELSE
				data_vigencia
			END
				as data_vigencia,
			dopvalortermo as vt, 
			valorempenho as ve, 
			valorpagamentosolicitado as ps,
			valorpagamento as vp,  
			dados_bancarios,
			{$campoSaldo} as sb,
			'<div 	class=\"dialogo\"  id=\"' || id || '\"><img style=\"cursor:pointer\"
					width=55px src=\"/imagens/gadget_busca.png\" border=\"0\" /></div>'  as dialogo,
			CASE WHEN dopdatafimvigencia IS NOT NULL AND (((('01/'||data_vigencia)::date + INTERVAL '1 month')::date-1) >= current_date) AND prpfinalizado IS NOT TRUE
				THEN
					CASE WHEN mdoid <> 3
						THEN
							CASE WHEN id IN ( SELECT dopid FROM par.documentoparreprogramacao WHERE dprstatus = 'P' )
								THEN '<center><b>Aguardando Validação de Reprogramação</b></center>'
								ELSE
									CASE WHEN prpfinalizado is null then
										".$botaoreprogramacao."
									ELSE
										'<center>
                                                                                    <input type=\"button\" title=\"Processo finalizado\" value=\"Prazo\" disabled \">
                                                                                    <input type=\"button\" title=\"Processo finalizado\" value=\"Subações\" disabled \">
                                                                                </center>'
									END
							END
						ELSE '<center>
                                                        <input type=\"button\" value=\"Prazo\" disabled \">
                                                        <input type=\"button\" value=\"Subações\" disabled \">
                                                    </center>'
					END
				ELSE '<center>
                                        <input type=\"button\" class=\"btn-gray\" style=\"border: solid #a3a3a3 1px;\" value=\"Prazo\" onclick=\"enviaEmailReprogramacaoBloqueada( \\' '|| prpid ||' \\', \\''|| prpnumeroprocesso ||'\\', \'prazo\'  )\">
                                        <input type=\"button\" class=\"btn-gray\" style=\"border: solid #a3a3a3 1px;\" value=\"Subações\" onclick=\"enviaEmailReprogramacaoBloqueada( \\' '|| prpid ||' \\', \\''|| prpnumeroprocesso ||'\\'  )\">
                                    </center>'
			END as r, ";



	$sql .= "'<a href=\"par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&download=S&arqid='|| arqid ||'\" ><img align=absmiddle src=../imagens/anexo.gif border=0></a>' as anexo";


$arrPflsTR = Array( PAR_PERFIL_SUPER_USUARIO,
					PAR_PERFIL_ADMINISTRADOR,
					PAR_PERFIL_ANALISTA_MERITOS,
					PAR_PERFIL_PREFEITO,
					PAR_PERFIL_EQUIPE_MUNICIPAL,
					PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,

					PAR_PERFIL_EQUIPE_ESTADUAL,
					PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,
					PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,

					PAR_PERFIL_ANALISTA_TR );

if( (possui_perfil( $arrPflsTR )) || ($bloqueiaParaConsultaEstMun) ){
	$sql .= ",'<center><input type=button value=TR dopid='|| id ||' class=tor \"></center>' as tor";
}else{
	$sql .= ",'<center> - </center>' as tor";
}


$listaSemAcompanhamento = false;

$estado = $_SESSION['par']['estuf'];
$inuid = $_SESSION['par']['inuid'];

// @todo por enquanto somente para administrador e super usuários terão acesso ao acompanhamento
if( (in_array(  PAR_PERFIL_SUPER_USUARIO, $perfil ) || (in_array(  PAR_PERFIL_PREFEITO , $perfil )) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )) || (in_array( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil )) || (in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil )))
{
		$sql .=",'<center><input type=\"button\"
				value=' ||
				CASE WHEN
					acompanhamento
				THEN
					'\" Em Andamento\"' ||  'onclick=\"abreAcompanhamento(' || id || ', ' || acompanhamento || ' )\"></center>'
				ELSE
					CASE WHEN
						EXISTS(
								SELECT
									 si.icoid
								FROM
									par.subacao s
									INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
									INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
									INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
									--INNER JOIN par.subacaoitenscomposicaonotasfiscais nf ON si.icoid = nf.icoid
									INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
									INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
								WHERE
									s.sbaid in (
										SELECT DISTINCT
												s.sbaid
										FROM
											par.subacao s
										inner join
											par.subacaodetalhe sd on sd.sbaid = s.sbaid 
										inner join
											par.termocomposicao tc on tc.sbdid = sd.sbdid 
										inner join
											par.documentopar dp on dp.dopid = tc.dopid 
										WHERE
											dp.dopid = id
							)

								AND tc.dopid = id
						)
					THEN
						'\" Finalizado\"  ' ||  'onclick=\"abreAcompanhamento(' || id || ', \'finalizado\' )\"></center>'
					ELSE
						'\" Iniciar\"' ||  'onclick=\"abreAcompanhamento(' || id || ', ' || acompanhamento || ' )\"></center>'
					END
				END";
	}
	else if ( (in_array( PAR_PERFIL_CONSULTA_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil )) || (in_array( PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil )) || in_array(PAR_PERFIL_CONSULTA, $perfil) )
	{
		$sql .=",
				CASE WHEN
					acompanhamento
				THEN
					'<center><input type=\"button\" value=\" Em Andamento\" onclick=\"abreAcompanhamento(' || id || ', ' || acompanhamento || ' )\"></center>'
				ELSE
					CASE WHEN
						EXISTS(
								SELECT
									 si.icoid
								FROM
									par.subacao s
									INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
									INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
									INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano
									--INNER JOIN par.subacaoitenscomposicaonotasfiscais nf ON si.icoid = nf.icoid
									INNER JOIN par.subacaoitenscomposicaocontratos sicc ON  si.icoid = sicc.icoid
									INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
								WHERE
									s.sbaid in (
										SELECT DISTINCT
												s.sbaid
										FROM
											par.subacao s
										inner join
											par.subacaodetalhe sd on sd.sbaid = s.sbaid -- and sd.sbdano = '2013' -- tabela de detalhe da subação
										inner join
											par.termocomposicao tc on tc.sbdid = sd.sbdid -- tabela fica a composicao do termo (os itens)
										inner join
											par.documentopar dp on dp.dopid = tc.dopid -- dopnumerodocumento = 4851 -- fica o termo de compromisso
										WHERE
											dp.dopid = id
							)

								AND tc.dopid = id
						)
					THEN
						'<center><input type=\"button\" value=\"Finalizado\"  onclick=\"abreAcompanhamento(' || id || ', \'finalizado\' )\"></center>'
					ELSE
						'<center><input type=\"button\" value=\"Iniciar\" style=\"border: solid #a3a3a3 1px;\" class=\"btn-gray\"></center>'
					END
				END";

	}
	else
	{
		$listaSemAcompanhamento = true;
	}

		     $sql .=
							"
		 FROM (
			SELECT
				dp.dopid as id,
                dp.prpid,
                dp.dopstatus,
				dp.dopacompanhamento as acompanhamento,
				dp.dopidpai as dopidpai,
				d.mdoid as mdoid,
				d.tpdcod as tipo_doc,
				(SELECT 
					to_char(vigencia, 'MM/YYYY') -- seleciona maior vigência entre documento validado e ex-ofício
				FROM (
				       SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia --Seleciona maior vigência entre termos validados
					      FROM par.documentopar  d
					      INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid AND v.dpvstatus = 'A'
					      WHERE d.prpid = pp.prpid --8146
					      AND dopstatus <> 'E' AND dopdatafimvigencia IS NOT NULL
					      AND mdoid NOT IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
				       UNION ALL
				       SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia -- Seleciona maior vigência de Ex-Ofício
					      FROM par.documentopar  d
					      WHERE d.prpid = pp.prpid
					      AND dopstatus <> 'E' AND dopdatafimvigencia IS NOT NULL
					      AND mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
				) as foo
				GROUP BY vigencia
				ORDER BY vigencia DESC LIMIT 1
				) as data_vigencia,
				mdonome as tipodocumento,
				pp.prpfinalizado,
				iu.inuid,
				iu.estuf,
				iu.muncod,
				dp.dopusucpfvalidacaogestor,
				d.mdoqtdvalidacao,
				dp.dopnumerodocumento as doc,
				pp.prpnumeroprocesso,
				(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem,
				--dp.dopvalortermo::numeric(20,2),
				(SELECT dopvalortermo::numeric(20,2) FROM par.documentopar  d
					INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid
					WHERE prpid = pp.prpid
					AND dopstatus <> 'E'
					AND dpvstatus = 'A'
					AND mdoid NOT IN (79,65,66,68,76,80,67,73,82)
					ORDER BY d.dopid DESC
					LIMIT 1
				) as dopvalortermo,
				Coalesce((select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado where processo = pp.prpnumeroprocesso ), 0.00)
						as valorempenho,
				pgs.valor_pagamento as valorpagamentosolicitado,
				pm.valor_pagamento as valorpagamento,
				--CASE WHEN pm.pagsituacaopagamento = '2 - EFETIVADO' THEN pm.valor_pagamento END as valorpagamento,
				dp.arqid,
				dp.dopdatafimvigencia,
				'Banco: '||coalesce(prpbanco, 'n/a')||'<br> Conta: '||coalesce(prpagencia, 'n/a')||'<br> Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios
			FROM
				par.documentopar dp
			INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
			INNER JOIN par.processopar pp ON pp.prpid = dp.prpid
			INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
			INNER JOIN ( select
							d.dopid,
							sum(vve.vrlempenhocancelado) + sum(coalesce(emr.vrlreforco,0)) as valor
						from
							par.documentopar d
						inner join par.processopar prp on prp.prpid = d.prpid
						inner join par.empenho emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
						inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
						left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie
									from par.empenho
									where empcodigoespecie in ('02') and empstatus = 'A'
									group by
										empnumeroprocesso,
										empcodigoespecie,
										empidpai) as emr on emr.empidpai = emp.empid
						inner join par.empenhosubacao ems on ems.empid = emp.empid AND eobstatus = 'A' where d.dopstatus = 'A' group by d.dopid ) em ON em.dopid = dp.dopid
			LEFT JOIN (select
		                    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
		                from
		                    par.documentopar d
		                inner join par.processopar prp on prp.prpid = d.prpid
		                inner join par.empenho 			emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
		                inner join par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
		                inner join par.pagamentosubacao ps  ON ps.pagid = pag.pagid and pobstatus = 'A'
                        where d.dopstatus = 'A' group by d.dopid, pagsituacaopagamento) pm ON pm.dopid = dp.dopid
		    LEFT JOIN (select
		                    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
		                from
		                    par.documentopar d
		                inner join par.processopar 		prp on prp.prpid = d.prpid
		                inner join par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
		                inner join par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
		                inner join par.pagamentosubacao ps  on ps.pagid = pag.pagid and pobstatus = 'A'
                        where d.dopstatus = 'A' group by d.dopid, pagsituacaopagamento) pgs ON pgs.dopid = dp.dopid
		) as foo
		where dopstatus = 'A' $fitrosDoc AND id IS NOT NULL

		--@todo confirmar com thiago
		AND tipo_doc in (102, 21, 16 )

		";
	if( ! $listaSemAcompanhamento ) {
            $cabecalho = array("&nbsp;","Nº do  Processo","Nº do Documento", "Tipo de documento", "Data de Vigência", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancários", "Saldo Bancário<br />(CC + CP + Fundo)", "Diálogo",
"Reprogramação Subações / Prorrogação de Prazo", "Anexos", "Termos de Referência", "Acompanhamento");
	} else {
            $cabecalho = array("&nbsp;","Nº do Processo","Nº do Documento", "Tipo de documento", "Data de Vigência","Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancários", "Saldo Bancário<br />(CC + CP + Fundo)", "Diálogo",
"Reprogramação Subações / Prorrogação de Prazo", "Anexos", "Termos de Referência");
	}
// ver(simec_htmlentities($sql),d);
//$db->monta_lista($sql,$cabecalho,100,5,'N','','N', false);

$db->monta_lista_simples($sql, $cabecalho,1000000,5,'N','100%','S', true, false, false, true);

if( ! $bloqueiaParaConsultaEstMun )
{
	// Obras PAR
	echo '<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela"><tr><td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">Documentos de Obras do PAR</td></tr></table>';


    // Alterando nome da coluna do processo
    $campoSaldo = str_replace('prpnumeroprocesso', 'pronumeroprocesso', $campoSaldo);

	$sql = "SELECT
				$sqlfiltro '<span class=\"processo_detalhe\">' || pronumeroprocesso || '</span>' as pronumeroprocesso, doc, tipodocumento,data_vigencia, qtdObra, dopvalortermo as vt, 
				valorempenho as ve, 
				valorpagamentosolicitado as ps,
				valorpagamento as vp, 
				dados_bancarios,
				{$campoSaldo} as sb
			FROM (
				SELECT 
					dp.dopid as id, dp.dopidpai,
                                        (SELECT 
                                            to_char(vigencia, 'MM/YYYY') -- seleciona maior vigência entre documento validado e ex-ofício
                                        FROM (
                                            SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia --Seleciona maior vigência entre termos validados
                                                 FROM par.documentopar  d
                                                 INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid AND v.dpvstatus = 'A'
                                                 WHERE d.proid = dp.proid
                                                 AND dopstatus <> 'E'
                                                 AND mdoid NOT IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                                            UNION ALL
                                            SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia -- Seleciona maior vigência de Ex-Ofício
                                                 FROM par.documentopar  d
                                                 WHERE d.proid = dp.proid
                                                 AND dopstatus <> 'E'
                                                 AND mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                                        ) as foo
                                        GROUP BY vigencia
                                        ORDER BY vigencia DESC LIMIT 1
					) as data_vigencia,
					 dp.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, (select dopnumerodocumento from par.documentopar where proid = dp.proid and dopstatus <> 'E' order by dopid asc LIMIT 1) as doc,
					'Banco: '||coalesce(probanco, 'n/a')||'<br> Conta: '||coalesce(proagencia, 'n/a')||'<br> Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
					(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem,
					dp.dopvalortermo::numeric(20,2), 
					em.valor as valorempenho,
					pgs.valor_pagamento as valorpagamentosolicitado,
					pm.valor_pagamento as valorpagamento,
					--CASE WHEN pm.pagsituacaopagamento = '2 - EFETIVADO' THEN pm.valor_pagamento END as valorpagamento,
					pp.pronumeroprocesso,
                                        (select count(pc.preid) from par.processoobraspar po inner join par.processoobrasparcomposicao pc on pc.proid = po.proid where pc.pocstatus = 'A' and po.proid = pp.proid) as qtdObra
				FROM par.vm_documentopar_ativos  dp
				INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
				INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
				INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
				LEFT JOIN ( 
					SELECT
						dopid,
						sum(valor) as valor
					FROM
						(
						SELECT DISTINCT
							d.dopid, 
							vve.empid,
							vve.vrlempenhocancelado as valor
						FROM 
							par.documentopar d
						INNER JOIN par.processoobraspar prp on prp.proid = d.proid and prp.prostatus = 'A'
						INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
						INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = emp.empid
						LEFT  JOIN (
							SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
							FROM par.empenho
							WHERE empcodigoespecie IN ('02') AND empstatus = 'A'
							GROUP BY empnumeroprocesso, empcodigoespecie, empidpai
							) as emr ON emr.empidpai = emp.empid 
						inner join par.empenhoobrapar ems on ems.empid = emp.empid and eobstatus = 'A'  
						) as foo
					GROUP BY dopid
					) em ON em.dopid = dp.dopid
					LEFT JOIN (select 
				                    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
				                from 
				                    par.vm_documentopar_ativos d
				                inner join par.processopar 		prp on prp.prpid = d.prpid
				                inner join par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
				                inner join par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
				                inner join par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' group by d.dopid, pagsituacaopagamento) pm ON pm.dopid = dp.dopid
				    LEFT JOIN (select 
				                    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
				                from 
				                    par.vm_documentopar_ativos d
				                inner join par.processopar 		prp on prp.prpid = d.prpid
				                inner join par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
				                inner join par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
				                inner join par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' group by d.dopid, pagsituacaopagamento) pgs ON pgs.dopid = dp.dopid
			) as foo
			WHERE 1=1 $fitrosDoc 
			ORDER BY doc"; 
// 	ver($sql);
	$cabecalho = array("&nbsp;","Nº do Processo","Nº do Documento", "Tipo de documento","Data de Vigência", "Qnt de Obras", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancário", "Saldo Bancário<br />(CC + CP + Fundo)");
	
	$db->monta_lista_simples($sql,$cabecalho,100,5,'N','','N', false, array(10,20,70));
	
	
	//PAC
	echo '<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela"><tr><td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">Documentos do PAC</td></tr></table>';
	//
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	
	$acoes = "'<img src=../imagens/icone_lupa.png title=\"Visualizar\" style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/teladevalidacao&acao=A&terid='||tc.terid||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\">'";
	
	// Envio manual para o sigarp (MI)
	if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ){
		$acoes .= " || CASE WHEN ( SELECT count(processo) FROM par.processoobraspaccomposicao com INNER JOIN par.enviomiitemperdido eip ON eip.preid = com.preid WHERE com.pocstatus = 'A' and com.proid = tc.proid ) > 0 THEN '<a href=\"javascript:aderirPregaoMI(\'' || tc.terid || '\')\" ><img src=\"../imagens/refresh2.gif\" title=\"Aderir ao pregão\" border=\"0\"></a>' ELSE '' END";
	}

    // Alterando nome da coluna do processo
    $campoSaldo = str_replace('prpnumeroprocesso', 'pronumeroprocesso', $campoSaldo);

	$sql = "SELECT
				CASE WHEN usucpfassinatura IS NOT NULL OR terassinado = 't' 
					THEN '<center><img src=\"../imagens/obras/check.png\" title=\"Documento Validado\" width=\"18\">'||$acoes||'</center>' 
					ELSE '<center>'||$acoes||'</center>' 
				END as acoes,
				'<span class=\"processo_detalhe\">' || pro.pronumeroprocesso || '</span>' as pronumeroprocesso,
				'PAC2'||to_char(tc.terid,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY') as ternum,
				CASE WHEN terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE to_char(tc.terdataassinatura, 'DD/MM/YYYY') END as data,
				to_char(vig.data, 'DD/MM/YYYY') as datafimvigencia,
				CASE WHEN terassinado = 't' AND tc.terdataassinatura IS NULL THEN 'Validado Manualmente' ELSE u.usunome END as usu,
                                (select count(pc.preid) from par.processoobra po inner join par.processoobraspaccomposicao pc on pc.proid = po.proid where
                                        pc.pocstatus = 'A' and po.proid = pro.proid) as qtdObra,
				( select sum( prevalorobra ) from par.termoobra ter inner join obras.preobra po on po.preid = ter.preid AND po.prestatus = 'A' WHERE ter.terid = tc.terid ) as valor_termo,
				em.valor as valorempenho,
				sum(pm.valor_pagamento) as valorpagamentosolicitado,
				sum(CASE WHEN pm.pagsituacaopagamento = '2 - EFETIVADO' THEN pm.valor_pagamento ELSE 0 END) as valorpagamento,
				'Banco: '||coalesce(probanco, 'n/a')||'<br> Conta: '||coalesce(proagencia, 'n/a')||'<br> Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
				{$campoSaldo} as sb
			FROM 
				par.termocompromissopac  tc
			INNER JOIN par.processoobra 	pro ON pro.proid = tc.proid and pro.prostatus = 'A'
			LEFT  JOIN seguranca.usuario 	u 	ON u.usucpf = tc.usucpfassinatura 
			LEFT JOIN ( 
				SELECT
					terid,
					sum(valor) as valor
				FROM
					( 
					SELECT DISTINCT
						tc.terid, 
						vve.empid,
						vve.vrlempenhocancelado + coalesce(emr.vrlreforco,0) as valor
					FROM 
						par.termocompromissopac tc
					INNER JOIN par.processoobra 			prp ON prp.proid = tc.proid and prp.prostatus = 'A'
					INNER JOIN par.empenho 					emp ON emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
					INNER JOIN par.v_vrlempenhocancelado 	vve ON vve.empid = emp.empid
					LEFT  JOIN (
						SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
						FROM par.empenho
						WHERE empcodigoespecie in ('02') AND empstatus = 'A'
						GROUP BY empnumeroprocesso, empcodigoespecie, empidpai) as emr on emr.empidpai = emp.empid 
					INNER JOIN par.empenhoobra ems on ems.empid = emp.empid and eobstatus = 'A' 
					) as foo
				GROUP BY terid
				) as em ON em.terid = tc.terid
			LEFT JOIN (
				SELECT 
					tc.terid, sum( pobvalorpagamento ) as valor_pagamento, pag.pagsituacaopagamento
				FROM 
					par.termocompromissopac tc
				INNER JOIN par.processoobra 	prp ON prp.proid = tc.proid and prp.prostatus = 'A'
				INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
				INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
				INNER JOIN par.pagamentoobra 	ps  ON ps.pagid = pag.pagid  
				GROUP BY tc.terid, pagsituacaopagamento ) pm ON pm.terid = tc.terid
				left join (
						select distinct
						    terid,
						    max(prazo) as data
						from(
						    SELECT
						        po.preid,
						        case when vig.data is not null then vig.data else (MIN(pag.pagdatapagamentosiafi) + 720) end  as prazo,
						        tc.terid, popvalidacao
						    FROM
						        par.pagamentoobra po
						        inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
						        inner join par.empenho emp on emp.empid = pag.empid and emp.empstatus = 'A'
						        inner join par.termoobraspaccomposicao tc on tc.preid = po.preid
						                                    
						        left join obras.preobraprorrogacao pp on pp.preid = po.preid and pp.popdataprazoaprovado is not null and pp.popvalidacao = 't'
						        left join(
						            SELECT popdataprazoaprovado as data, preid FROM obras.preobraprorrogacao WHERE popstatus = 'A'
						        ) vig on vig.preid = po.preid
						group by po.preid, vig.data, tc.terid, popvalidacao
						) as foo 
						group by terid) vig on vig.terid = tc.terid
			WHERE 
				tc.terstatus = 'A'	
				$filtroPac
			GROUP BY
				tc.usucpfassinatura, tc.terassinado, tc.proid, tc.terid, tc.terdatainclusao, tc.terdataassinatura,
				u.usunome, vig.data,
				em.valor, pro.proid,
				pro.probanco, pro.proagencia, pro.nu_conta_corrente, pro.pronumeroprocesso
			ORDER BY
				tc.terid"; 
// ver($sql,d);
	$cabecalho = array("&nbsp;","Nº do Processo","Nº do Documento", "Data da Validação", "Vigência do Termo", "Usuário da Validação", "Qnt de Obras", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Dados Bancários", "Saldo Bancário<br />(CC + CP + Fundo)", "Diálogo");

	$db->monta_lista_simples($sql,$cabecalho,100,5,'N','','N', false, array(10,20,35, 35));
	
	#relatorio de atendimento dos estados
	if($_SESSION['par']['itrid'] == 1){
            include APPRAIZ . 'includes/classes/relatorio.class.inc';
            
            $estuf = $_SESSION['par']['estuf'];
            
            include APPRAIZ . '/par/classes/modelo/RelatorioAtendimentoEstado.class.inc';
            $relAtendimentoEstado = new RelatorioAtendimentoEstado($estuf);
            $dados = $relAtendimentoEstado->montarData();
            $agrup = $relAtendimentoEstado->montarAgrupador();
            $col = $relAtendimentoEstado->montarColuna();

            $r = new montaRelatorio();
            $r->setAgrupador($agrup, $dados, $agrupcol);
            $r->setColuna($col);
            $r->setBrasao(false);
            $r->setTotNivel(true);
            $r->setEspandir(false);
            $r->setMonstrarTolizadorNivel(true);
            $r->setTotalizador(true);
            $r->setTolizadorLinha(true);
            echo '<br /><table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" class="tabela" style="border-top: none; border-bottom: none;"><tbody><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">Convênios de 2007 a 2010: '.$estuf.'</td></tr></tbody></table>';
            echo $r->getRelatorio();
        }
	
}	
//AVISOS

// Funcão para verificar vigencia das obras do par e
function listaVigenciaObrasPar()
{
	global $db;

	// Estado ou município
	if($_SESSION['par']['muncod'])
	{
		$responsavel = "Prefeito(a)";
		$local = "Município";
	} else
	{
		$responsavel = "Secretário(a) Estadual";
		$local = "Estado";
	}

	$arrWhere = Array("inu.inuid = {$_SESSION['par']['inuid']}","pre.prestatus = 'A'","pre.preidpai IS NULL");

	// RECUPERA OBRAS DO PAR
	$sql = "
	    SELECT DISTINCT
			pre.preid as preid,
			pre.predescricao as nome_obra
		FROM
			obras.preobra pre
		INNER JOIN par.subacaoobra 			sbo ON sbo.preid = pre.preid
        INNER JOIN par.subacao 				sub on sub.sbaid = sbo.sbaid AND sub.sbastatus = 'A'
        INNER JOIN par.acao 				aca ON aca.aciid = sub.aciid AND aca.acistatus = 'A'
        INNER JOIN par.pontuacao 			pon ON pon.ptoid = aca.ptoid AND pon.ptostatus = 'A'
        INNER JOIN par.instrumentounidade 	inu ON inu.inuid = pon.inuid AND ( pre.estuf = inu.estuf OR pre.muncod = inu.muncod )
		WHERE
			".implode(' AND ',$arrWhere)."
		ORDER BY
		    pre.predescricao
	";

	$arPreObrasSemCobertura = $db->carregar($sql);
	$arPreObrasSemCobertura = $arPreObrasSemCobertura ? $arPreObrasSemCobertura : array();


	if(is_array($arPreObrasSemCobertura)) {
		foreach($arPreObrasSemCobertura as $k => $preobra)
		{
				
			/* Trecho que preenche a coluna Fim da vigência */
			$sql = "
            SELECT
    			MIN(pag.pagdatapagamentosiafi) AS data_primeiro_pagamento,
    			MIN(pag.pagdatapagamentosiafi) + 720 as prazo
    		FROM
    			par.pagamentoobrapar po
    		INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
    		WHERE
    			po.preid = " . $preobra['preid'];

			$dataPrimeiroPagamento = $db->carregar($sql);

            $dataAtual = '';
            if( $dataPrimeiroPagamento[0]['data_primeiro_pagamento'] ){
                $sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = ".$preobra['preid'];
                $prorrogadoAtivo = $db->pegaUm($sql);

                $sql = "SELECT popid FROM obras.preobraprorrogacao WHERE popstatus = 'P' AND preid = ".$preobra['preid'];
                $prorrogadoPendente = $db->pegaUm($sql);

                if( $prorrogadoAtivo ) {
                    $dataAtual = $prorrogadoAtivo;
                } elseif($prorrogadoPendente) {
                    $dataAtual = null;
                }else{
                    $dataAtual = $dataPrimeiroPagamento[0]['prazo'];
                }
            }

			if($dataAtual)
			{
				$mes = substr($dataAtual, 5,2);
				$ano = substr($dataAtual, 0,4);
				$dia = substr($dataAtual, 8,2);
				$data = $dia.'/'.$mes.'/'.$ano;
				$arPreObrasSemCobertura[$k]['fimvigencia'] = $data;
			}
		}


		$resultVigencias = verificaProximosVencimentosObras( $arPreObrasSemCobertura ) ;

		if($resultVigencias['resultado']) {
				
			$texto = "<pre style='text-align: justify;'>
Prezados Senhores,<br>
Informamos que existem obras vencendo nos próximos 60 dias e que ainda não foram prorrogadas. Segue lista:<br>				
{$resultVigencias['tabela']}<br> 
Caso o {$local} precise de mais prazo para conclusão da(s) obra(s) é necessário que o(a) {$responsavel} acesse o SIMEC com o seu CPF  e senha, clique no módulo PAR, selecione a visualização: Árvore, clique em
<a href='par.php?modulo=principal/listaObrasParUnidade&acao=A'>Lista de Obras</a>.<br>
Em seguida o Sistema apresentará a relação das obras com seus prazos de Fim de Vigência.<br> O  {$responsavel} deve clicar em “Solicitar Prorrogação da Vigência”, inserir o prazo solicitado, a justificativa fundamentada para o pedido e clicar em salvar.
Antes de solicitar a prorrogação de vigência é imprescindível que o {$local} faça a atualização do cronograma de execução de todas as obras, bem como, os dados de execução das obras inseridos pelo(a) engenheiro(a) do município, pois utilizaremos como base para avaliação desses pedidos, e também para a liberação de recursos. <br>Portanto, para que possamos fazer uma análise coerente é indispensável que as obras estejam atualizadas no SIMEC no módulo de monitoramento de obras.
<br>Para dúvidas, solicitamos  entrar em contato com a Equipe Técnica da COVEN - Coordenação de Convênio, por meio do E-mail: grupo.prorrogação@fnde.gov.br.
Para os pedidos aprovados constará no SIMEC o novo prazo de fim da vigência das obras, bem como os pareceres de deferimento para visualização e impressão.</pre>";
				
			return $texto;
		} else {
		    return false;
		}

		$fimVigenciaObra = '';
	}
}
		// Função para verificar proximos vencimentos de obras nos proximos 15, 30 e 60 dias
function verificaProximosVencimentosObras( $arrayObras ) {
    if( is_array($arrayObras) ) {
		$objDataTime = new Data();
		foreach( $arrayObras as $dadoV ) {
		    if($dadoV['fimvigencia']) {
                if( strlen($dadoV['fimvigencia']) == 7 ) {
                    // XX/XXXX
                    $mes = substr($dadoV['fimvigencia'], 0,2);
                    $ano = substr($dadoV['fimvigencia'], 3,4);
                    $data = $mes . "/" .  $ano;
                    $qtdDiasMesVigencia = $objDataTime->getQuantidadeDiasMes($mes);
                    $diasRestantes = $objDataTime->quantidadeDeDiasEntreDuasDatas(date('d/m/Y'), $qtdDiasMesVigencia .'/'. $data ,'DD/MM/AAAA');

                } else {
                    // XX/XX/XXXX
                    $diasRestantes = $objDataTime->quantidadeDeDiasEntreDuasDatas(date('d/m/Y'), $dadoV['fimvigencia'] ,'DD/MM/AAAA');
                }

                if($diasRestantes > 0 ) {
                    if( $diasRestantes < 15 ) {
                        $arrObrasVencendo[] = array(
                            'preid'  => $dadoV['preid'],
                            'vigencia'  => $dadoV['fimvigencia'],
                            'dias'		=> 15,
                            'nome_obra'	=> $dadoV['nome_obra'],
                            'tipo_doc'	=> $dadoV['tipo_doc']
                        );
                    } elseif( $diasRestantes < 30 ) {
                        $arrObrasVencendo[] = array(
                            'preid'  => $dadoV['preid'],
                            'vigencia'  => $dadoV['fimvigencia'],
                            'dias'		=> 30,
                            'nome_obra'	=> $dadoV['nome_obra'],
                            'tipo_doc'	=> $dadoV['tipo_doc']
                        );
                    } elseif( $diasRestantes < 60 ) {
                        $arrObrasVencendo[] = array(
                            'preid'  => $dadoV['preid'],
                            'vigencia'  => $dadoV['fimvigencia'],
                            'dias'		=> 60,
                            'nome_obra'	=> $dadoV['nome_obra'],
                            'tipo_doc'	=> $dadoV['tipo_doc']
                        );
                    }
                }
            }
        }
    }

    $arrObrasVencendo = (is_array($arrObrasVencendo)) ? $arrObrasVencendo : Array();
	$tabela = false;

	if(count($arrObrasVencendo) > 0) {
		$tabela = '
			<table>
            <thead>
            <tr>
            <td>Nº Obra</td>
            <td>Nome da obra</td>
            <td>Vencimento</td>
            </tr>
            </thead>
		</tbody>';
		foreach($arrObrasVencendo as $k => $v ) {
		    $tabela .= "<tr>
                            <td>
                            {$v['preid']}
                            </td>
                            <td>
                            {$v['nome_obra']}
                            </td>
                            <td>
                            {$v['vigencia']}
                            </td>
                            </tr>";

            $arrObras[] 	= $v['nome_obra'];
			$arrVigencia[] 	= $v['vigencia'];
		}


		$tabela .= "
		</tbody>
		</table>";

		return array(
		'resultado' => true,
		'tabela' 	=> $tabela
		);
    } else {
		return array(
			'resultado' => false,
			'tabela' 	=> $tabela
		);
	}
}

// Função para verificar a vigencia das obras PAC e emitir aviso caso tenham obras a vencer
function listaVigenciaPAC()
{

	global $db;
	//// --------------------------------------- PAC 2 --------------------------------------- ////
	// Lista de obras PAC
	if($_SESSION['par']['muncod']) {
		$muncodpar = " = '" . $_SESSION['par']['muncod'] . "'";
		$esfera = "M";
		$ptoclassificacaoobra = array("'Q'", "'C'", "'P'");
		$filtroMuncodOREstuf = "pre.muncodpar  = '{$_SESSION['par']['muncod']}'";
		$responsavel = "Prefeito(a)";
		$local = "Município";
	} else {
		$muncodpar = "IS NULL";
		$esfera = "E";
		$ptoclassificacaoobra = array("'Q'", "'C'");
		$filtroMuncodOREstuf = "pre.estufpar = '{$_SESSION['par']['estuf']}'";
		$responsavel = "Secretário(a) Estadual";
		$local = "Estado";
	}

	$sqlObrasPac = "
	SELECT
	pre.preid as preid,
	pre.predescricao as nome_obra,
	'PAC' as tipo_doc
	FROM
	obras.preobra pre
	LEFT  JOIN obras2.obras obr ON obr.obrid = pre.obrid AND obridpai IS NULL
	INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
	INNER JOIN workflow.documento doc ON doc.docid = pre.docid
	INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
	WHERE
	$filtroMuncodOREstuf
	AND pto.ptoesfera IN ('" . $esfera . "','T')
		        AND pre.preesfera = '" . $esfera . "'
		        AND pto.ptoclassificacaoobra IN (" . implode(',', $ptoclassificacaoobra) . ")
		        AND pre.presistema = '23'
		        AND pre.prestatus = 'A'
		        AND pre.tooid = 1
		        AND pre.preidpai IS NULL
		    ORDER BY
		        pre.preprioridade
	";
	$arPreObrasComCobertura = $db->carregar($sqlObrasPac);
	$arPreObrasComCobertura = ($arPreObrasComCobertura) ? $arPreObrasComCobertura : Array();

	if(count($arPreObrasComCobertura) > 0) {

	    foreach($arPreObrasComCobertura as $k => $v) {
            $sql = " SELECT
                        MIN(pag.pagdatapagamentosiafi) AS data_primeiro_pagamento,
                        MIN(pag.pagdatapagamentosiafi) + 720 as prazo
                    FROM
                        par.pagamentoobra po
                    INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
                    WHERE po.preid =  {$v['preid']}";

            $dataPrimeiroPagamento = $db->carregar($sql);

            $dataAtual = '';
            if( $dataPrimeiroPagamento[0]['data_primeiro_pagamento'] ){
                $sql = "SELECT popdataprazoaprovado FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid = ".$v['preid'];
                $prorrogadoAtivo = $db->pegaUm($sql);

                $sql = "SELECT popid FROM obras.preobraprorrogacao WHERE popstatus = 'P' AND preid = ".$v['preid'];
                $prorrogadoPendente = $db->pegaUm($sql);

                if( $prorrogadoAtivo ) {
                    $dataAtual = $prorrogadoAtivo;
                } elseif($prorrogadoPendente) {
                    $dataAtual = null;
                }else{
                    $dataAtual = $dataPrimeiroPagamento[0]['prazo'];
                }
            }

            if($dataAtual) {
                $mes = substr($dataAtual, 5,2);
                $ano = substr($dataAtual, 0,4);
                $dia = substr($dataAtual, 8,2);
                $data = $dia.'/'.$mes.'/'.$ano;
                $arPreObrasComCobertura[$k]['fimvigencia'] = $data;
            }
        }
    }

    # Verifica datas das vigencias
    $resultVigencias = verificaProximosVencimentosObras( $arPreObrasComCobertura ) ;

    if($resultVigencias['resultado']) {
						
        $texto = "<pre style='text-align: justify;'>
        Prezados Senhores,<br>
        Informamos que existem obras vencendo nos próximos 60 dias e que ainda não foram prorrogadas. Segue lista:
        {$resultVigencias['tabela']}<br>
        Caso o {$local} precise de mais prazo para conclusão da(s) obra(s) é necessário que o(a) {$responsavel} acesse o SIMEC com o seu CPF  e senha, clique no módulo PAR, selecione a visualização: Árvore, clique em
        <a href='par.php?modulo=principal/listaObrasParUnidade&acao=A'>Lista de Obras</a>.<br>
        Em seguida o Sistema apresentará a relação das obras com seus prazos de Fim de Vigência.<br> O  {$responsavel} deve clicar em “Solicitar Prorrogação da Vigência”, inserir o prazo solicitado, a justificativa fundamentada para o pedido e clicar em salvar.
        Antes de solicitar a prorrogação de vigência é imprescindível que o {$local} faça a atualização do cronograma de execução de todas as obras, bem como, os dados de execução das obras inseridos pelo(a) engenheiro(a) do município, pois utilizaremos como base para avaliação desses pedidos, e também para a liberação de recursos. <br>Portanto, para que possamos fazer uma análise coerente é indispensável que as obras estejam atualizadas no SIMEC no módulo de monitoramento de obras.
        <br>Para dúvidas, solicitamos  entrar em contato com a Equipe Técnica da COVEN - Coordenação de Convênio, por meio do E-mail: grupo.prorrogação@fnde.gov.br.
        Para os pedidos aprovados constará no SIMEC o novo prazo de fim da vigência das obras, bem como os pareceres de deferimento para visualização e impressão.</pre>";
        return $texto;
    } else {
	    return false;
	}

}


//mostra show modal informes
// buscando informes
$sql = "SELECT
DISTINCT dopdatafimvigencia as fimvigencia,
dopnumerodocumento,
dopid,
'PAR' as tipo_doc
FROM
par.vm_documentopar_ativos dp
inner join par.processopar prp on prp.prpid = dp.prpid
inner join par.instrumentounidade iu on iu.inuid = prp.inuid
WHERE
( prp.prpfinalizado IS NULL OR prp.prpfinalizado = 'f') AND iu.inuid = {$_SESSION['par']['inuid']}

UNION ALL

select
		DISTINCT dopdatafimvigencia as fimvigencia,
		dopnumerodocumento,
		dopid,
		'OBRAS_PAR' as tipo_doc

		from
		par.vm_documentopar_ativos dp
		inner join par.processoobraspar prp on prp.proid = dp.proid
		inner join par.instrumentounidade iu on iu.inuid = prp.inuid
		WHERE
		( prp.profinalizado IS NULL OR prp.profinalizado = 'f') AND iu.inuid = {$_SESSION['par']['inuid']}
		";


		$dadosVigencia = $db->carregar($sql);

		$arrDP = array();

		//include 'includes/classes/dateTime.inc';
		$objDataTime = new Data();

		if( is_array($dadosVigencia) ){
		foreach( $dadosVigencia as $dadoV ){
		if( strlen($dadoV['fimvigencia']) == 7 ){ // XX/XXXX
			$mes = substr($dadoV['fimvigencia'], 0,2);
			$ano = substr($dadoV['fimvigencia'], 3,4);
			$data = $mes . "/" .  $ano;
				
				
				
		} else { // XX/XX/XXXX
			$mes = substr($dadoV['fimvigencia'], 3,2);
			$ano = substr($dadoV['fimvigencia'], 6,4);
			$data = $mes . "/" .  $ano;

				
			}
			$qtdDiasMesVigencia = $objDataTime->getQuantidadeDiasMes($mes);

			$diasRestantes = $objDataTime->quantidadeDeDiasEntreDuasDatas(date('d/m/Y'), $qtdDiasMesVigencia .'/'. $data ,'DD/MM/AAAA');

			if($diasRestantes > 0 )
			{
			$numDoc = getNumDoc($dadoV['dopid']);
				
			if( $diasRestantes < 15 )
			{
			$arrTermosVencendo[] = array(
				'numDoc' 	=> $numDoc,
				'vigencia'  => $dadoV['fimvigencia'],
						'dias'		=> 15,
								'dopid'		=> $dadoV['dopid'],
					'tipo_doc'	=> $dadoV['tipo_doc']
							);
			}
			elseif( $diasRestantes < 30 )
			{
			$arrTermosVencendo[] = array(
					'numDoc' 	=> $numDoc,
					'vigencia'  => $dadoV['fimvigencia'],
					'dias'		=> 30,
					'dopid'		=> $dadoV['dopid'],
					'tipo_doc'	=> $dadoV['tipo_doc']
			);

			}
			elseif( $diasRestantes < 60 )
			{
			$arrTermosVencendo[] = array(
				'numDoc' 	=> $numDoc,
				'vigencia'  => $dadoV['fimvigencia'],
					'dias'		=> 60,
					'dopid'		=> $dadoV['dopid'],
					'tipo_doc'	=> $dadoV['tipo_doc']
				);

			}
				
		}

			}

			}

			$arrTermosVencendo = ($arrTermosVencendo) ? $arrTermosVencendo : array();

			if( (count($arrTermosVencendo)) > 0  && (is_array($arrTermosVencendo)) )
			{

			foreach($arrTermosVencendo as $k => $v )
			{
			if($v['tipo_doc'] == 'PAR')
		{
				$arrDoc[] 	 = $v['numDoc'];
				$arrVigencia[] = $v['vigencia'];
			}
		elseif($v['tipo_doc'] == 'OBRAS_PAR')
		{
			$arrDocObras[] 	 = $v['numDoc'];
			$arrVigenciaObras[] = $v['vigencia'];
				
		}
	}

	if((is_array($arrDoc)) && (is_array($arrVigencia)) )
	{
	$docs = implode(', ', $arrDoc);
	$vigencias = implode(', ', $arrVigencia);
		
$texto = "<pre style='text-align: justify;'>Prezados Senhores,<br>	
Informamos que a(s) vigência(s) do (s) Termo (s) de Compromisso(s) nº (s) {$docs} firmado(s) com esse Estado/Prefeitura e o Fundo Nacional de Desenvolvimento da Educação –FNDE, expirará(ão) em {$vigencias}.<br>
Para viabilizar o pedido de prorrogação verifique na tela de Execução e acompanhamento
</pre>";
	
	$titulo = "Vigências";
	$arrayAvisos[] = array('titulo' => $titulo, 'texto' => $texto, 'status' => E_ERROR);
		
	}
	/*if((is_array($arrVigenciaObras)) && (is_array($arrDocObras)) )
	{
	$docsObras = implode(', ', $arrDocObras);
	$vigenciasObras = implode(', ', $arrVigenciaObras);
	}
	*/
	/*
	// Aviso de termos de obras do par (suspenso por enquanto)
	if((is_array($arrDocObras)) && (is_array($arrVigenciaObras)) )
	{
	$textoObras = "Prezado(s) Senhor(es),

	Informamos que a(s) vigência(s) do (s) Termo (s) de Compromisso(s) nº (s) {$docsObras} firmado(s) com esse Estado/Prefeitura e o Fundo Nacional de Desenvolvimento da Educação –FNDE, expirará(ão) em {$vigenciasObras}.
	Para viabilizar o pedido de prorrogação clique no link abaixo:

	<br><br> <a href='par.php?modulo=principal/administracaoDocumentos&acao=A'>Execução e acompanhamento</a>
	";
	$tituloObras = "Vigências - Obras PAR";
	$arrayAvisos[] = array('titulo' => $tituloObras, 'texto' => $textoObras);
	}
	*/

	?>
<!-- 
    <script type="text/javascript">
       /* jQuery(function(){
                    jQuery("#dialog_vigencia").dialog({
                        modal: true,
                        width: 1200,
                        title: 'Vigências',
                        buttons: {
                            Fechar: function() {
                                jQuery("#dialog_detalhe_processo").html('');
                                jQuery( this ).dialog( "close" );
                            }
                        }
                    });
        });*/
    </script>
 -->
<?php } 

$listaVigenciaPAR = listaVigenciaObrasPar();

if($listaVigenciaPAR)
{
	$tituloObrasPAR = "Vigências - Obras PAR";
	$arrayAvisos[] = array('titulo' => $tituloObrasPAR, 'texto' => $listaVigenciaPAR);
}

$listaVigenciaPAC = listaVigenciaPAC();

if($listaVigenciaPAC)
{
	$tituloObrasPAC = "Vigências - Obras PAC";
	$arrayAvisos[] = array('titulo' => $tituloObrasPAC, 'texto' => $listaVigenciaPAC);
}

// Aviso de obras que estiverem em reformulação (temporário)
$sqlObrasAviso = "
				SELECT
					pre.preid,
					pre.predescricao,
					pre.tooid,
					sob.sbaid,
					sob.sobano
				FROM
					obras.preobra pre
				INNER JOIN workflow.documento 		doc ON doc.docid = pre.docid
				INNER JOIN par.instrumentounidade 	inu ON (inu.muncod = pre.muncodpar AND pre.preesfera = 'M' ) OR (inu.estuf = pre.estufpar AND pre.preesfera = 'E' )
				LEFT  JOIN par.subacaoobra			sob ON sob.preid = pre.preid
				WHERE
					doc.esdid IN (1486, 1488) AND
					inu.inuid = {$_SESSION['par']['inuid']}";

$dadosObrasAviso = $db->carregar($sqlObrasAviso);

if($dadosObrasAviso[0]['preid']!='')
{	
		
// 	$texto = "O Fundo Nacional de Desenvolvimento da Educação (FNDE)
// 				fará reformulação das obras de municípios interessados e que assinaram 
// 				o termo de compromisso com a utilização de Metodologias Inovadoras para convêncional <a href='#' onclick='javascript:abrepopUpAvisos()'>Clique aqui para ver a mensagem completa</a>";
		
	$texto = "<pre style='text-align: justify;'>Prezado Gestor,<br>
Seu município possui obras em reformulação MI para convencional, conforme lista abaixo.<br> 
Para informações de preenchimento da reformulação acesse o manual <a onclick=abrepopUpManual() >AQUI</a>:
			<table class='listagem' cellspacing='1' cellpadding='2' align='center' style='color:333333; width: 100%'>
				<thead>
					<tr>
						<th>N° Obra</th>
						<th>Nome da Obra</th>
					</tr>
				</thead>
				<tbody>";
	foreach($dadosObrasAviso as $obra){
		$texto .= "
					<tr>
						<td style='color:#999999;'><a onclick=\"abreObra('{$obra['preid']}', '{$obra['tooid']}', '{$obra['sbaid']}', '{$obra['sobano']}')\" >{$obra['preid']}</a></td>
						<td>{$obra['predescricao']}</td>
					</tr>";
	}
	
	$texto .= "
				</tbody>
			</table></pre>";
	
	
	$titulo = "Obras em reformulação";
	$arrayAvisos[] = array('titulo' => $titulo, 'texto' => $texto, 'status' => E_WARNING);
}

$dadosObrasAvisoMI = verificarProcessoObraMI($_SESSION['par']['inuid']);

if( $dadosObrasAvisoMI == 't' )
{ 
	$texto = "<pre style='text-align: justify;'> 
As obras aprovadas com Metodologia Inovadora, contratadas e não contratadas,  estão passíveis de reformulação para metodologia convencional.<br>
Para as obras com o contrato firmado,  recomenda-se que o município  observe as cláusulas contratuais e as diretrizes da legislação em vigor para a adoção das medidas legais cabíveis com o propósito de embasar sua decisão administrativa.<br>	
<a href='par.php?modulo=principal/pedidoReformulacaoObras&acao=A'>Clique aqui</a> para visualizar a lista de obras passíveis de reformulação.<br>
<a href='documentos/arquivos/proinfancia_tipo_1-2_manual-reformulacao-mi-convencional.pdf' target='_blank'>Clique aqui</a> para baixar o manual de orientações para reformulação.<br>			
Caberá ao município e ao Distrito Federal a responsabilidade de realizar o processo licitatório e execução das obras.</pre>";

	$titulo = "OBRAS MI PASSíVEIS DE REFORMULAçãO";
	$arrayAvisos[] = array('titulo' => $titulo, 'texto' => $texto, 'status' => 99);
}

$sql = "SELECT count(sr.preid)
		FROM par.instrumentounidade iu
			inner join par.processoobra po on po.muncod = iu.muncod
		    inner join par.processoobraspaccomposicao ppc on ppc.proid = po.proid and ppc.pocstatus = 'A'
		    inner join par.solicitacaoreformulacaoobras sr on sr.preid = ppc.preid and sr.sfostatus = 'A'
		    inner join obras.preobra pr on pr.preid = sr.preid and pr.prestatus = 'A'
		    inner join workflow.documento d on d.docid = pr.docid and d.esdid = ".WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL."
		WHERE
			iu.inuid = ".$_SESSION['par']['inuid'];
$totalObraMiReformulacao = $db->pegaUm($sql);

if( $totalObraMiReformulacao > 0 ){
	$texto = "<pre style='text-align: justify;'>
Prezado (a) Senhor (a) Prefeito (a).<br>Informamos que a solicitação de abertura do processo de reformulação para troca do tipo de projeto de sua Creche Proinfância foi acatada possibilitando a alteração do projeto para construção em metodologia convencional, conforme solicitação cadastrada.<br>
Sendo assim, comunicamos que o sistema SIMEC - Módulo PAR (Plano Trabalho / Árvore / Lista de Obras) já se encontra aberto para alteração, sendo necessáriaatualização da documentação (tipo de obra, plantas técnicas, etc), conforme o passo-a-passo do Manual para Reformulação de Obras Metodologia Inovadora para Metodologia Convencional disponibilizado no portal do FNDE, no link:<br>
<a href='http://www.fnde.gov.br/arquivos/category/130-proinfancia?download=9490:proinfancia-creche-de-tipo-1-e-2-manual-reformulacao-mi-convencional' target='_blank'>
http://www.fnde.gov.br/arquivos/category/130-proinfancia?download=9490:proinfancia-creche-de-tipo-1-e-2-manual-reformulacao-mi-convencional.</a>
					</pre>";
	
	$titulo = "OBRAS MI EM REFORMULAçãO";
	$arrayAvisos[] = array('titulo' => $titulo, 'texto' => $texto, 'status' => 99);
}


# Verifica Diligências de Contratos
$sqlContratoDiligencia = "SELECT DISTINCT
                            '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location.href=\'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoContratos&dopid='||dop.dopid||'\'\" border=\"0\">' as acao,
                            dop.dopnumerodocumento as numero_termo,
                            par.retornacodigosubacao(sd.sbaid) as subacao,
                            con.connumerocontrato,
                            con.condiligencia
                        FROM par.processopar pp 
                        INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = pp.prpid
                        INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = pp.prpid AND ppc.ppcstatus = 'A'
                        INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid 
                        INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icostatus = 'A'
                        INNER JOIN par.subacaoitenscomposicaocontratos sicc ON sicc.icoid = sic.icoid AND sicc.sccstatus = 'A'
                        INNER JOIN par.contratos con ON con.conid = sicc.conid AND con.constatus = 'A' AND con.condiligencia IS NOT NULL
                        WHERE pp.inuid = {$inuid}";
$arrContratoDiligencia = $db->carregar($sqlContratoDiligencia);
$arrContratoDiligencia = ($arrContratoDiligencia) ? $arrContratoDiligencia : array();
if (count($arrContratoDiligencia)> 0) {
$htmlContratoDiligencia = "<p>Existem Contratos com divergência de informações para um ou mais itens vinculados. Este(s) documento(s) deve(m) ser alterado(s).</p><br />";
$htmlContratoDiligencia .= "<table class='tabela'>
                                <thead>
                                    <tr>
                                        <th>Ação</th>
                                        <th>N° Termo</th>
                                        <th>Subação</th>
                                        <th>Nº Contrato</th>
                                        <th>Parecer de Rejeição</th>
                                    </tr>
                                </thead>
                                <tbody>";
                                foreach($arrContratoDiligencia as $contrato) {
                                    $htmlContratoDiligencia .= "<tr>
                                            <td>{$contrato['acao']}</td>
                                            <td>{$contrato['numero_termo']}</td>
                                            <td>{$contrato['subacao']}</td>
                                            <td>{$contrato['connumerocontrato']}</td>
                                            <td>{$contrato['condiligencia']}</td>
                                        </tr>";
                                }
$htmlContratoDiligencia .= "</tbody></table>";
$arrayAvisos[] = array('titulo' => 'Contratos em Diligência', 'texto' => $htmlContratoDiligencia, 'status' => 99);
}

# Verifica Diligências de Notas Fiscais
$sqlNFDiligencia = "SELECT DISTINCT
                            '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"window.location.href=\'par.php?modulo=principal/acompanhamentoDocumentos&acao=A&abas=acompanhamentoNotas&dopid='||dop.dopid||'\'\" border=\"0\">' as acao,
                            dop.dopnumerodocumento as numero_termo,
                            par.retornacodigosubacao(sd.sbaid) as subacao,
                            ntf.ntfnumeronotafiscal,
                            ntf.ntfdiligencia
                        FROM par.processopar pp 
                        INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = pp.prpid
                        INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = pp.prpid AND ppc.ppcstatus = 'A'
                        INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid 
                        INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icostatus = 'A'
                        INNER JOIN par.subacaoitenscomposicaonotasfiscais sicntf ON sicntf.icoid = sic.icoid AND sicntf.scnstatus = 'A'
                        INNER JOIN par.notasfiscais ntf ON ntf.ntfid = sicntf.ntfid AND ntf.ntfstatus = 'A' AND ntf.ntfdiligencia IS NOT NULL
                        WHERE pp.inuid = {$inuid}";
$arrNFDiligencia = $db->carregar($sqlNFDiligencia);
$arrNFDiligencia = ($arrNFDiligencia) ? $arrNFDiligencia : array();
if (count($arrNFDiligencia)> 0) {
$htmlNFDiligencia = "<p>Existem Contratos com divergência de informações para um ou mais itens vinculados. Este(s) documento(s) deve(m) ser alterado(s).</p><br />";
$htmlNFDiligencia .= "<table class='tabela'>
                                <thead>
                                    <tr>
                                        <th>Ação</th>
                                        <th>N° Termo</th>
                                        <th>Subação</th>
                                        <th>Nº Contrato</th>
                                        <th>Parecer de Rejeição</th>
                                    </tr>
                                </thead>
                                <tbody>";
                                foreach($arrNFDiligencia as $nota) {
                                    $htmlNFDiligencia .= "<tr>
                                            <td>{$nota['acao']}</td>
                                            <td>{$nota['numero_termo']}</td>
                                            <td>{$nota['subacao']}</td>
                                            <td>{$nota['ntfnumeronotafiscal']}</td>
                                            <td>{$nota['ntfdiligencia']}</td>
                                        </tr>";
                                }
$htmlNFDiligencia .= "</tbody></table>";
$arrayAvisos[] = array('titulo' => 'Notas Fiscais em Diligência', 'texto' => $htmlNFDiligencia, 'status' => 99);
}

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<div id="dialog-dialogo" title="Solicitações do Processo" style="display:none;"> </div>
<style type="">
.ui-dialog-titlebar{
	text-align: center;
}
.btn-gray{
	background: #adadad;
  	background-image: -webkit-linear-gradient(top, #adadad, #707070);
  	background-image: -moz-linear-gradient(top, #adadad, #707070);
  	background-image: -ms-linear-gradient(top, #adadad, #707070);
  	background-image: -o-linear-gradient(top, #adadad, #707070);
  	background-image: linear-gradient(to bottom, #adadad, #707070);
}
.btn-gray:hover {
  background: #707070;
  background-image: -webkit-linear-gradient(top, #707070, #adadad);
  background-image: -moz-linear-gradient(top, #707070, #adadad);
  background-image: -ms-linear-gradient(top, #707070, #adadad);
  background-image: -o-linear-gradient(top, #707070, #adadad);
  background-image: linear-gradient(to bottom, #707070, #adadad);
}
</style>
<script type="text/javascript" src="js/par.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">

	jQuery.noConflict();

	jQuery(document).ready(function(){

		jQuery('.tor').click(function(){

			var dopid = jQuery(this).attr('dopid');
			window.location.href = 'par.php?modulo=principal/tor/torDocumentos&acao=A&dopid='+dopid;
		});
		
		jQuery('.dialogo').live('click',function(){
			var dopid = jQuery(this).attr('id');
			var numprocesso = replaceAll(jQuery(this).parent().parent().find('[title="Nº Processo"]').html(),'<br>','');
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "&req=mostraDialogo&dopid="+dopid+"&numprocesso="+numprocesso,
		   		async: false,
		   		success: function(msg){
		   			jQuery( "#dialog-dialogo" ).html(msg);	
// 		   			jQuery( "#dialog-dialogo" ).show();
		   			jQuery( "#dialog-dialogo" ).dialog({
						resizable: false,
						height:600,
						width:800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							"OK": function() {
								jQuery( this ).dialog( "close" );							
							}
						}
					});
		   		}
			});
		});
	});

</script>

<?php 
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	
	include_once APPRAIZ ."includes/library/simec/Avisos.class.php";
	$objAvisos = new Avisos();
	$arrayAvisos = ($arrayAvisos) ? $arrayAvisos : array();
			
	if ( count(array_intersect( unserialize(GRUPO_DE_PERFIL_ESTADUAL_EDICAO), $perfil)) > 0 || count(array_intersect( unserialize(GRUPO_DE_PERFIL_MUNICIPAL_EDICAO), $perfil)) > 0 )
	{
		#Abre a mensagem na tela automaticamente
		if(count($arrayAvisos) > 0  )
		{
			$objAvisos->getAvisos($arrayAvisos);
		}		
	}
	
	if(count($arrayAvisos) > 0  )
	{
		#Mostra o icone de pendência
		$objAvisos->getAvisos($arrayAvisos, true);
		echo '<script>jQuery("#botao_avisos").show();</script>';
	}
	
	
?>