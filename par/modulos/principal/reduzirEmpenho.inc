<?php
ini_set("memory_limit","1000M");
set_time_limit(0);

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

echo'<br>';
monta_titulo( '', 'Lista de Subações a Serem Reduzidas' );
?>
<html>
	<head>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
		<link rel='stylesheet' type='text/css' href='../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	</head>
<body>
	<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" name="empid" id="empid" value="<?=$_REQUEST['empid']; ?>" />
	<input type="hidden" name="processo" id="processo" value="<?=$_REQUEST['processo']; ?>" />
	<input type="hidden" name="especie" id="especie" value="<?=$_REQUEST['especie']; ?>" />
	
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="tabela">
	<tr><td>
	<?
	$sql = "select
				'<input type=checkbox name=chk[] onclick=marcarCheckbox(this); id=chk_'||sd.sbdid||' value='||sd.sbdid||'>&nbsp;<a href=\"javascript:listarSubacao(\'' || s.sbaid || '\', \'' || es.eobano || '\', \'' || iu.inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Subação\" border=\"0\"></a>&nbsp;<a href=\"javascript:aderirPregao(\'' || sd.sbdid || '\', \'' || iu.inuid || '\')\" ></a>' as acao, 
				s.sbadsc,
				par.recuperavalorvalidadossubacaoporano(s.sbaid, es.eobano) as valorsubacao,
			    sum(es.eobvalorempenho - coalesce(emc.vlrempenhocancelado, 0) ) as valorempenho,
			    sd.sbdid,
			    sd.sbdano
			from
				par.subacao s
			    inner join par.subacaodetalhe 		sd 	on sd.sbaid = s.sbaid and s.sbastatus = 'A'
				inner join par.empenhosubacao 		es 	on es.sbaid = sd.sbaid and sd.sbdano = es.eobano and eobstatus = 'A'
				INNER JOIN par.acao 				a 	on s.aciid = a.aciid
				INNER JOIN par.pontuacao 			po 	on a.ptoid   = po.ptoid
				inner join par.instrumentounidade 	iu 	on po.inuid   = iu.inuid
				left join (
					select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.sbaid, eb.eobano from par.empenhosubacao eb
						inner join par.empenho e1 on e1.empid = eb.empid and empstatus = 'A' and eobstatus = 'A'
					where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null
					group by e1.empidpai, eb.sbaid, eb.eobano
				) as emc on emc.empidpai = es.empid and emc.sbaid = es.sbaid and emc.eobano = sd.sbdano
			where
				es.empid = {$_REQUEST['empid']}
			group by
				s.sbaid,
				iu.inuid,
				sd.sbdano,
				es.eobano,
				s.sbadsc,
			    sd.sbdid";
	
	$arrDados = $db->carregar( $sql );
	$arrDados = $arrDados ? $arrDados : array();?>
	
	<?if( $arrDados ){ ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
			<tr>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Ação</strong></td>
				<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Subação</strong></td>
				<td align="80%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Ano</strong></td>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Valor da Subação</strong></td>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Valor Empenhado</strong></td>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Valor a Reduzir</strong></td>
			</tr> 
			</thead>
			<tbody><?	
			foreach ($arrDados as $key => $v) {
				$key % 2 ? $cor = "#dedfde" : $cor = "";
				?>
					<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td align="left" title="Ação"><?=$v['acao']; ?></td>
						<td align="left" title="Subação"><?=$v['sbadsc']; ?></td>
						<td align="left" title="Ano"><?=$v['sbdano']; ?></td>
						<td align="left" title="Valor Empenhado"><?=number_format($v['valorsubacao'], '2', ',', '.'); ?>
						<td align="left" title="Valor Empenhado"><?=number_format($v['valorempenho'], '2', ',', '.'); ?>
								<input type="hidden" id="eobvalorempenho_<?=$v['sbdid']; ?>" name="eobvalorempenho[<?=$v['sbdid']; ?>]" value="<?=$v['valorempenho']; ?>"></td>
						<td align="left" title="Valor a Reduzir">
							<input type="text" style="text-align:;" name="esrvalorreduzido[<?=$v['sbdid']; ?>]" size="21" maxlength="13" value="" onkeyup="this.value=mascaraglobal('[###.]###,##',this.value);" 
								id="esrvalorreduzido_<?=$v['sbdid']; ?>" readonly="readonly" title="Valor a Reduzir" class="disabled" onblur="validaValorReduzir(<?=$v['sbdid']; ?>)"></td>
					</tr>
				<?
			}
			?>
			<tr>
				<td colspan="7">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tbody>
					<tr>
						<td class="SubTituloDireita" width="50%"><b>Valor total de empenho:</b></td>
						<td><input type="text" id="totalempenho" name="totalempenho" size="30" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
					</tr>
				</tbody>
				</table>
				</td>
			</tr>
			</tbody>
			</table>
			
		<?} else { ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tbody>
				<tr>
					<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
				</tr>
			</tbody>
			</table>
		<?} ?>
		<table align="center" border="0" width="100%" cellpadding="3" cellspacing="2">
			<tr bgcolor="#D0D0D0">
				<td>
					<center><input type="button" onclick="javascript: reduzirEmpenho();" value="Reduzir Empenho"></center>
				</td>
			</tr>
		</table>
	</td></tr></table>
	<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_reduzir";
?>
<style>
	.popup_alerta_reduzir{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_reduzir <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario","S","S","Usuário","22","","","","","","","id='ws_usuario'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha" name="ws_senha">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="button" name="btn_enviar" onclick="reduzirEmpenhoWS()" value="ok" />
					<input type="button" name="btn_reduzir" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="Reduzir" />
				</td>
			</tr>
		</table>
	</div>
</div>
	</form>

</body>
<script type="text/javascript">
function marcarCheckbox(obj) {
	var sbdid = obj.value;
	var campo = 'esrvalorreduzido_'+sbdid;
	if(obj.checked) {
		$('#'+campo).attr('class', 'normal')
		$('#'+campo).attr('readonly', false);
	} else {
		$('#'+campo).attr('class', 'disabled');
		$('#'+campo).attr('readonly', true);
		$('#'+campo).val('');
		
		validaValorReduzir(sbdid);
	}
}
function validaValorReduzir(sbdid){
	
	var valorTotal = 0;
	var valorEmpenho = $('#eobvalorempenho_'+sbdid).val();
	var valorReduzido = $('#esrvalorreduzido_'+sbdid).val();
	valorReduzido = valorReduzido.replace(/\./gi,"");
	valorReduzido = valorReduzido.replace(/\,/gi,".");
	
	if( parseFloat(valorReduzido) > parseFloat(valorEmpenho) ){
		alert('O valor a reduzir não pode ser maior que o valor empenhado!');
		$('#esrvalorreduzido_'+sbdid).val('');
		//return false;
	}
	
	$('[name*=esrvalorreduzido]').each(function(){
		var valor = $(this).val();
		if( valor == '' ){
			valor = '0,00';
		}
		valor = valor.replace(/\./gi,"");
		valor = valor.replace(/\,/gi,".");
		
		valorTotal = parseFloat(valorTotal) + parseFloat(valor);		
	});
	valorTotal = mascaraglobal('###.###.###.###,##',replaceAll(valorTotal.toFixed(2),'.',''));
	$('#totalempenho').val(valorTotal);
}

function reduzirEmpenho(){
	var check = $('[name="chk[]"]:checked').val();
	if(Number(check)){
		if( $('#totalempenho').val() == '0,00' ){
			alert('O valor a reduzir não pode ser vazio');
			return false;
		}
		document.getElementById('div_auth_reduzir').style.display = 'block';
		document.body.scrollTop = 0;
	} else {
		alert('Selecione uma subação e informe o valor a reduzir!');
		return false;
	}
}
			
function reduzirEmpenhoWS() {

	var wsusuario = document.getElementById('ws_usuario').value;
	var wssenha = document.getElementById('ws_senha').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usuário!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	document.getElementById('div_auth_reduzir').style.display = 'none';
	
	divCarregando();
	
	var formulario = $('#formulario').serialize();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/reduzirEmpenho&acao=A",
   		data: "requisicao=reduzirEmpenho&"+formulario,
   		async: false,
   		success: function(msg){
   		//document.getElementById('debug').innerHTML = msg;
   		alert(msg);
   		window.opener.location.href = window.opener.location; 
   		window.close();
   		}
	});	
	divCarregado();	
}

function listarSubacao(sbaid, ano, inuid)
{
	var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
	janela(local,800,600,"Subação");
}
</script>
</html>
<div id="debug"></div>
<?
function reduzirEmpenho($dados) {
	global $db;
	
	try{
		$data_created = date("c");
		
		/* if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
		} else { */
			$usuario = $dados['ws_usuario'];
			$senha   = $dados['ws_senha'];
		//}
		
		$dadosemp = $db->pegaLinha("SELECT e.empid, e.empcnpj, e.empnumerooriginal, e.empanooriginal, e.empnumeroprocesso, e.empcodigoespecie, e.empcodigopi, e.empcodigoesfera,
  										e.empcodigoptres, e.empfonterecurso, e.empcodigonatdespesa, e.empcentrogestaosolic, e.empanoconvenio, e.empnumeroconvenio, e.empcodigoobs,
  										e.empcodigotipo, e.empdescricao, e.empgestaoeminente, e.empunidgestoraeminente, e.empprogramafnde, e.empnumerosistema, e.empsituacao,
  										e.usucpf, e.empprotocolo, e.empnumero, vve.vrlempenhocancelado as empvalorempenho, e.ds_problema, e.valor_total_empenhado, e.valor_saldo_pagamento, e.empdata, 
  										op.prpid, op.prptipoexecucao
			 							FROM par.empenho e
											inner join par.processopar op on e.empnumeroprocesso = op.prpnumeroprocesso  and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
											inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid
										WHERE e.empid = '".$dados['empid']."'");
		
		$nu_seq_ne 							= $dadosemp['empprotocolo'];	
		$nu_cnpj_favorecido 				= pegaCnpj($_SESSION['par_var']['inuid'], $dadosemp['prpid']);
		
		if($dadosemp['empnumero']) {
			$arrNumero 						= explode("NE",$dadosemp['empnumero']);
			$nu_empenho_original			= $arrNumero[1];
			$an_exercicio_original			= $arrNumero[0];
		} else {
			$nu_empenho_original			= null;
			$an_exercicio_original			= null;
		}
		
		$vl_empenho 						= str_ireplace('.', '', $dados['totalempenho']);
		$vl_empenho 						= str_ireplace(',', '.', $vl_empenho);
		
		$co_especie_empenho					= "03";
		$co_esfera_orcamentaria_solic		= "1";
		$co_observacao						= "2";
		$co_descricao_empenho				= "0011";
		$co_gestao_emitente					= "15253";
		$co_unidade_gestora_emitente		= "153173";
		$co_unidade_orcamentaria_solic		= "26298";
		$nu_proposta_siconv					= null;
		$co_centro_gestao_solic				= $dadosemp['empcentrogestaosolic'];
		$co_natureza_despesa_solic 			= $dadosemp['empcodigonatdespesa'];
		$nu_sistema 						= $dadosemp['empnumerosistema'];
		$co_fonte_recurso_solic				= $dadosemp['empfonterecurso'];
		$co_ptres_solic						= $dadosemp['empcodigoptres'];
		$co_plano_interno_solic				= $dadosemp['empcodigopi'];
		$nu_processo						= $dadosemp['empnumeroprocesso'];
		
		if($dadosemp['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde				= "03";
			$an_convenio					= $an_exercicio;
			$nu_convenio					= $nu_convenio;
			$co_tipo_empenho				= "5";
	    }else if($dadosemp['prptipoexecucao'] == 'T'){
	        $co_programa_fnde				= "CM"; // "ED";
	        $an_convenio					= null;
			$nu_convenio					= null;
			$co_tipo_empenho				= "3";
	    }
	    
      		$sql = "SELECT distinct l.lwsid FROM par.logws l
						inner join par.historicowsprocessopar h ON l.lwsid = h.lwsid
					WHERE
						h.prpid = {$dadosemp['prpid']}
						and h.hwpxmlretorno is null
						and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessopar where prpid = {$dadosemp['prpid']})
						and l.lwstiporequest = '$co_especie_empenho'";
        	$request_id = $db->pegaUm($sql);
        	
        	if( empty($request_id) ){
		        $arrParam = array(
						'lwstiporequest' 	=> $co_especie_empenho,
		        		'usucpf' 			=> $_SESSION['usucpf']
		        );
		        $request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
        	}
	
	    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			}
			
			$arrParam = array(
					'lwsrequestdata'	=> 'now()',
					'lwsurl' 			=> $urlWS,
					'lwsmetodo' 		=> 'solicitar',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			 
			$arrParam = array(
					'prpid' 		=> $dadosemp['prpid'],
					'lwsid' 		=> $request_id,
					'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
					'hwpdataenvio' 	=> 'now()',
					'usucpf' 		=> $_SESSION['usucpf']
			);
			$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'insert' );
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute(); 
	
			$xmlRetorno = $xml;
			
			$arrParam = array(
					'hwpid'			=> $hwpid,
					'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
			 
			$arrParam = array(
					'lwsresponsedata' 	=> 'now()',
					'lwsid' 			=> $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
	
		    $xml = simplexml_load_string( stripslashes($xml));

		    echo "------ REDUÇÃO DE EMPENHO ------\n\n";
			//echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
			
			$result = (integer) $xml->status->result;
	
			if($result) {
				$arrParam = array(
						'lwserro' => false,
						'lwsid' => $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
					
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'REDUÇÃO DE EMPENHO - Sucesso'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
				
				echo "REDUÇÃO DE EMPENHO - Sucesso";
				echo $xml->status->result->message->text;
				
				$sql = "INSERT INTO par.empenho(empcnpj, empnumerooriginal, empanooriginal, empvalorempenho, empnumeroprocesso, empcodigoespecie, empcodigopi, empcodigoesfera, empcodigoptres,
				            empfonterecurso, empcodigonatdespesa, empcentrogestaosolic, empanoconvenio, empnumeroconvenio, empcodigoobs, empcodigotipo, empdescricao, empgestaoeminente, empunidgestoraeminente,
				            empprogramafnde, empnumerosistema, usucpf, empprotocolo, empidpai, empsituacao)
					    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
					    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
					    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
					    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
					            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
					            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
					            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
					            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
					            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
					            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
					            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
					            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
					            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
					            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
					            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
					            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
					            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
					            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
					            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
					            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
					            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
					            '".$_SESSION['usucpf']."',
					            '".$xml->body->nu_seq_ne."',
					            '".$dadosemp['empid']."',
					            '2 - EFETIVADO'
					            ) RETURNING empid;";
				
				$empid = $db->pegaUm($sql);
				
				$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, co_especie_empenho, empsituacao)
    					VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '$co_especie_empenho', 'REDUZIDO SIGEF');";
				$db->executar($sql);
				
				if($dados['chk']) {
					foreach($dados['chk'] as $sbdid){

						$sql = "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".$sbdid;
						$linha = $db->pegaLinha( $sql );
						
						if($dadosemp['prptipoexecucao'] == 'C') $percentual = 99;
						else $percentual = 100;
						
						$esrvalorreduzido = str_ireplace('.', '', $dados['esrvalorreduzido'][$sbdid]);
						$esrvalorreduzido = str_ireplace(',', '.', $esrvalorreduzido);
						
						if( !empty($esrvalorreduzido) && (float)$esrvalorreduzido > (float)0  ){
							$sql = "INSERT INTO par.empenhosubacao(sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
			    					VALUES ('".$linha['sbaid']."', '".$empid."', 0, '".$esrvalorreduzido."', '".$linha['sbdano']."');";
							$db->executar($sql);
						}
						
						$sql = "UPDATE par.subacaodetalhe SET ssuid = 18 WHERE sbdid = ".$sbdid;
						$db->executar($sql);
						
						$db->commit();
					}
				}
				$db->commit();
			} else {				
				$arrParam = array(
						'lwserro' => true,
						'lwsid' => $request_id
				);
				logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
					
				$arrParam = array(
						'hwpid' 		=> $hwpid,
						'hwpwebservice' => 'REDUÇÃO DE EMPENHO  - Erro'
				);
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );

				echo "\n\n\n";				
				echo "REDUÇÃO DE EMPENHO  - Erro";
				echo $xml->status->error->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->error->message->text)."\n\n";				
			}

	} catch (Exception $e){

		$arrParam = array(
				'lwserro' 			=> true,
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Reduzir Empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		
		$arrParam = array(
				'hwpid' 		=> $hwpid,
				'hwpwebservice' => 'REDUÇÃO DE EMPENHO  - Erro',
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno).' - Erro Exception: '.$erroMSG
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );

		echo "Erro-WS Reduzir Empenho no SIGEF: $erroMSG";
	}
}

function pegaCnpj($inuid, $prpid){
	global $db;
	$cnpj = $db->pegaUm("SELECT prpcnpj FROM par.processopar WHERE prpid = ".$prpid);
	if( $cnpj ){
		return $cnpj;
	} else {
		return $db->pegaUm("SELECT iue.iuecnpj 
		                     FROM par.instrumentounidade iu
		                     inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		                     WHERE
		                     	iu.inuid = {$inuid}
		                     	and iue.iuestatus = 'A'
		                        and iue.iuedefault = true");
	}
}
?>