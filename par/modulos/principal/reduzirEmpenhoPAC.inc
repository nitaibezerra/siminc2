<?php
ini_set("memory_limit","1000M");
set_time_limit(0);

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

echo'<br>';

if( $_REQUEST['especie'] == 'reduzir' ){
	monta_titulo( '', 'Lista de Obras a Serem Reduzidas' );
	$descricaoBotao = 'Reduzir Empenho';
} elseif( $_REQUEST['especie'] == 'cancelar' ){
	monta_titulo( '', 'Lista de Obras a Serem Canceladas os Restos a Pagar' );
	$descricaoBotao = 'Cancelar Restos a Pagar';
}
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
	$sql = "SELECT
				'<input type=checkbox name=chk[] onclick=marcarCheckbox(this); id=chk_'||po.preid||' value='||po.preid||'>' as acao, 
				po.predescricao,
				po.prevalorobra as valorobra,
				--sum(distinct es.eobvalorempenho - coalesce(vlrempenhocancelado, 0)) as valorempenho,
				v.saldo as valorempenho,
				po.preid
			FROM
				obras.preobra po 
				INNER JOIN obras.pretipoobra 				pp on po.ptoid = pp.ptoid
				inner join par.vm_saldo_empenho_por_obra 	v  on v.preid = po.preid
				/*INNER JOIN par.empenhoobra 			 		es ON es.preid 	= po.preid 	AND prestatus = 'A' AND po.preidpai IS NULL and eobstatus = 'A'
				left join (
			        select sum(eobvalorempenho) as vlrempenhocancelado, e1.empidpai, eb.preid
			            from par.empenhoobra eb
			                inner join par.empenho e1 on e1.empid = eb.empid
			            where e1.empcodigoespecie in ('03', '13', '04') and empidpai is not null and empstatus = 'A'
			            group by e1.empidpai, eb.preid
			    ) as emc on emc.preid = es.preid*/
			WHERE
				v.preid in (select preid from par.empenhoobra where empid = {$_REQUEST['empid']} and eobstatus = 'A') AND
				prestatus = 'A' AND  
                po.preidpai IS NULL AND
				po.prevalorobra IS NOT NULL AND
				CASE WHEN pp.ptocategoria IS NOT NULL 
				THEN 
					(SELECT count(itc.preid) FROM obras.preitenscomposicaomi itc WHERE /*itc.itcquantidade > 0 AND*/ itc.preid = po.preid ) > 0
				ELSE 
					(SELECT count(itc.itcid) FROM obras.preitenscomposicao itc INNER JOIN obras.preplanilhaorcamentaria ppo ON itc.itcid = ppo.itcid AND ppo.preid = po.preid  WHERE /*itc.itcquantidade > 0 AND*/ ppo.preid = po.preid ) > 0
				END
			GROUP BY
				po.predescricao,
				po.preid,
				po.prevalorobra,
				v.saldo";
	
	$arrDados = $db->carregar( $sql );
	$arrDados = $arrDados ? $arrDados : array();?>
	
	<?if( $arrDados ){ ?>
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
			<tr>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Ação</strong></td>
				<td align="60%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Obra</strong></td>
				<td align="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor=""><strong>Valor da Obra</strong></td>
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
						<td align="left" title="Subação"><?=$v['predescricao']; ?></td>
						<td align="left" title="Valor Empenhado"><?=number_format($v['valorobra'], '2', ',', '.'); ?>
								<input type="hidden" id="valorobra_<?=$v['preid']; ?>" name="valorobra[<?=$v['preid']; ?>]" value="<?=$v['valorobra']; ?>"></td>
						<td align="left" title="Valor Empenhado"><?=number_format($v['valorempenho'], '2', ',', '.'); ?>
								<input type="hidden" id="eobvalorempenho_<?=$v['preid']; ?>" name="eobvalorempenho[<?=$v['preid']; ?>]" value="<?=$v['valorempenho']; ?>"></td>
						<td align="left" title="Valor a Reduzir">
							<input type="text" style="text-align:;" name="erpvalorreduzido[<?=$v['preid']; ?>]" size="21" maxlength="13" value="" onkeyup="this.value=mascaraglobal('[###.]###,##',this.value);" 
								id="erpvalorreduzido_<?=$v['preid']; ?>" readonly="readonly" title="Valor a Reduzir" class="disabled" onblur="validaValorReduzir(<?=$v['preid']; ?>)"></td>
					</tr>
				<?
			}
			?>
			<tr>
				<td colspan="5">
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
					<center><input type="button" onclick="javascript: reduzirEmpenho();" value="<?=$descricaoBotao ?>"></center>
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
	var preid = obj.value;
	var campo = 'erpvalorreduzido_'+preid;
	if(obj.checked) {
		$('#'+campo).attr('class', 'normal')
		$('#'+campo).attr('readonly', false);
	} else {
		$('#'+campo).attr('class', 'disabled');
		$('#'+campo).attr('readonly', true);
		$('#'+campo).val('');
		
		validaValorReduzir(preid);
	}
}

function validaValorReduzir(preid){
	
	var valorTotal = 0;
	var valorEmpenho = $('#eobvalorempenho_'+preid).val();
	var valorReduzido = $('#erpvalorreduzido_'+preid).val();
	valorReduzido = valorReduzido.replace(/\./gi,"");
	valorReduzido = valorReduzido.replace(/\,/gi,".");
	
	if( parseFloat(valorReduzido) > parseFloat(valorEmpenho) ){
		alert('O valor selecionado não pode ser maior que o valor empenhado!');
		$('#erpvalorreduzido_'+preid).val(mascaraglobal('###.###.###.###,##',replaceAll(parseFloat(valorEmpenho).toFixed(2),'.','')));
		return false;
	}
	
	$('[name*=erpvalorreduzido]').each(function(){
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
			alert('O valor selecionado não pode ser vazio');
			return false;
		}
		document.getElementById('div_auth_reduzir').style.display = 'block';
		document.body.scrollTop = 0;
	} else {
		alert('Selecione uma Obra e informe o valor a reduzir!');
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
   		url: window.location.href,
   		data: "requisicao=reduzirEmpenhoPAC&"+formulario,
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
</script>
</html>
<div id="debug"></div>
<?
function reduzirEmpenhoPAC($dados) {
	
	global $db;
	
	try{
            $data_created = date("c");
            
            if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
                  $usuario = 'MECTIAGOT';
                  $senha   = 'M3135689';
            } else {
                  $usuario = $dados['ws_usuario'];
                  $senha   = $dados['ws_senha'];
            }
            
            $sql = "SELECT 
						e.empid, e.empcnpj, e.empnumerooriginal, e.empanooriginal, e.empnumeroprocesso, e.empcodigoespecie, e.empcodigopi, e.empcodigoesfera,
						e.empcodigoptres, e.empfonterecurso, e.empcodigonatdespesa, e.empcentrogestaosolic, e.empanoconvenio, e.empnumeroconvenio, e.empcodigoobs,
						e.empcodigotipo, e.empdescricao, e.empgestaoeminente, e.empunidgestoraeminente, e.empprogramafnde, e.empnumerosistema, e.empsituacao,
						e.usucpf, e.empprotocolo, e.empnumero, vve.vrlempenhocancelado as empvalorempenho, e.ds_problema, e.valor_total_empenhado, e.valor_saldo_pagamento, e.empdata, 
						op.proid, op.protipo
					FROM par.empenho e
					INNER JOIN par.processoobra op on e.empnumeroprocesso = op.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and op.prostatus = 'A'
					inner join par.v_vrlempenhocancelado vve on vve.empid = e.empid and empstatus = 'A' 
					WHERE e.empid = '".$dados['empid']."'";
            
            $dadosemp = $db->pegaLinha( $sql );

            $nu_seq_ne			= $dadosemp['empprotocolo']; 
            $nu_cnpj_favorecido	= $dadosemp['empcnpj']; 
            
            if($dadosemp['empnumero']) {
                  $arrNumero            	= explode("NE",$dadosemp['empnumero']);
                  $nu_empenho_original   	= $arrNumero[1];
                  $an_exercicio_original   	= $arrNumero[0];
            } else {
                  $nu_empenho_original               = null;
                  $an_exercicio_original             = null;
            }
            
            $vl_empenho	= str_ireplace('.', '', $dados['totalempenho']);
            $vl_empenho	= str_ireplace(',', '.', $vl_empenho);
            
            if( $dados['especie'] == 'reduzir' ){
                  $co_especie_empenho       = "03";
                  $co_descricao_empenho = "0011";
            } elseif( $dados['especie'] == 'cancelar' ) {
                  $co_especie_empenho       = "04";
                  $co_descricao_empenho = "0022";    // RAP
            }
            
            $co_esfera_orcamentaria_solic   = "1";
            $co_observacao                  = "2";
            $co_gestao_emitente             = "15253";
            $co_unidade_gestora_emitente	= "153173";
            $co_unidade_orcamentaria_solic  = "26298";
            $nu_proposta_siconv             = null;
            $co_centro_gestao_solic         = $dadosemp['empcentrogestaosolic'];
            $co_natureza_despesa_solic      = $dadosemp['empcodigonatdespesa'];
            $nu_sistema                     = $dadosemp['empnumerosistema'];
            $co_fonte_recurso_solic         = $dadosemp['empfonterecurso'];
            $co_ptres_solic                 = $dadosemp['empcodigoptres'];
            $co_plano_interno_solic         = $dadosemp['empcodigopi'];
            $nu_processo                    = $dadosemp['empnumeroprocesso'];
            $co_programa_fnde               = $dadosemp['empprogramafnde']; // SE PROINFÂNCIA = BW ||  SE QUADRAS OU COBERTURA = CN
        	$an_convenio                    = null;
            $nu_convenio                    = null;
			$co_tipo_empenho                = "3";
			
      		$sql = "SELECT distinct l.lwsid FROM par.logws l
						inner join par.historicowsprocessoobra h ON l.lwsid = h.lwsid
					WHERE
						h.proid = {$dadosemp['proid']}
						and h.hwpxmlretorno is null
						and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessoobrapar where proid = {$dadosemp['proid']})
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
          			'proid' 		=> $dadosemp['proid'],
          			'lwsid' 		=> $request_id,
          			'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
          			'hwpdataenvio' 	=> 'now()',
          			'usucpf' 		=> $_SESSION['usucpf']
          	);
          	$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'insert' );
      
			$xml = Fnde_Webservice_Client::CreateRequest()
                  	->setURL($urlWS)
                   	->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
                	->execute();
      
        	$xmlRetorno = $xml;
        	
        	$arrParam = array(
        			'hwpid'			=> $hwpid,
        			'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
        	);
        	logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );
        	
        	$arrParam = array(
        			'lwsresponsedata' 	=> 'now()',
        			'lwsid' 			=> $request_id
        	);
        	logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
      
  			$xml = simplexml_load_string( stripslashes($xml));

         	echo "------ REDUÇÃO DE EMPENHO ------\n\n";
                  
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
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );
				
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
                	foreach($dados['chk'] as $preid){
                
                		$percentual = 100;
                
                		$erpvalorreduzido = str_ireplace('.', '', $dados['erpvalorreduzido'][$preid]);
                		$erpvalorreduzido = str_ireplace(',', '.', $erpvalorreduzido);
                			
                		$eobvalorempenho = ($dados['eobvalorempenho'][$preid] - $erpvalorreduzido);
                
                		$valorobra = $dados['valorobra'][$preid];
                
                		/* $eobpercentualemp1 = ($eobvalorempenho * $percentual);
                		$eobpercentualemp2 = $eobpercentualemp1 / $valorobra;
                		$eobpercentualemp = number_format($eobpercentualemp2, 2, '.', ''); */
                		
                		$sql = "INSERT INTO par.empenhoobra(preid, empid, eobpercentualemp2, eobvalorempenho, eobpercentualemp)
								VALUES ('".$preid."', '".$empid."', 0, '".$erpvalorreduzido."', 0);";
                		$db->executar($sql);
                
                		$db->commit();
                		
                		$sql = "SELECT empnumero FROM par.empenho WHERE empid = $empid";
                		$empnumero = $db->pegaUm($sql);
                			
                		insereHistoricoStatusObra( $empid, Array( $preid ), 'I', "Obra inativada pela Redução de empenho $empnumero" );
                		
                		inativaObras2SemSaldoEmpenho( $empid, Array( $preid ) );
                	}
                }               
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
				logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );
				
                echo "\n\n\n";                     
                echo "REDUÇÃO DE EMPENHO  - Erro";
                echo $xml->status->error->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->error->message->text)."\n\n";
            	$hwpwebservice = "REDUÇÃO DE EMPENHO  - Erro";                   
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
            logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessoobra', 'alter' );

            echo "Erro-WS Reduzir Empenho no SIGEF: $erroMSG";
      }
}
?>