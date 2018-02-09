<?php

if( $_REQUEST['requisicao'] == 'carregarBeneficiarioEmenda' ){
	
	insereBeneficiarioEmendas( $_REQUEST );
	exit();
}

if( $_REQUEST['requisicao'] == 'cargaSiop' ){
	require(APPRAIZ . 'altorc/classes/WSAlteracoesOrcamentarias.php');	
	
	$arrParr = array('codigoUO' => $_REQUEST['codigouo'], 'codigoParlamentar' => $_REQUEST['codigoparlamentar']);
	
	$ws = new WSAlteracoesOrcamentarias();
	$retorno = $ws->obterEmendasAprovadas( $arrParr );
	
	if( $_REQUEST['mostraRetorno'] == 'N' ){	
		if( (int)$retorno->return->sucesso == 1 ){
			$arrRetorno = $retorno->return->emendasAprovadas->emendaAprovada;
			$arrRetorno = is_array($arrRetorno) ? $arrRetorno : array($arrRetorno);
			
			cargaEmendaBaseSiop( $arrRetorno, $_REQUEST );
		} else {
			ver($retorno->return->mensagensErro, $retorno, d);
		}
		
		$db->sucesso('sistema/tabelasapoio/cargaEmendaAprovadaSiop');
		
		exit();
	}
}

if( $_REQUEST['requisicao'] == 'carregarListaBeneficiario' ){
	
	$imgMais = "'<center><span style=\"font-size:16px; cursor:pointer;\" title=\"mais\" id=\"image_'||cesid||'\" class=\"glyphicon glyphicon-floppy-save\" onclick=\"carregarBeneficiarioEmenda('||cesid||', \''||cnpjbeneficiario||'\', this);\"></span></center>'";
	
	$sql = "SELECT
				$imgMais,
				formata_cpf_cnpj(cnpjbeneficiario) as cnpjbeneficiario, 
				nomebeneficiario, 
				valorrevisadobeneficiario, 
				valorapuradorcl, 
				codigomomento||'&nbsp;' as codigomomento, 
				snatual
  			FROM emenda.beneficiarioemenda
			where cesid = {$_REQUEST['cesid']}";
	
	$cabecalho = array('Ação', "cnpjbeneficiario", "nomebeneficiario", "valorrevisadobeneficiario", "valorapuradorcl", "codigomomento", "snatual");
	$db->monta_lista_simples( $sql, $cabecalho,100,5,'S','80%','S', true, false, false, false);
	die;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( 'Carga Emendas Aprovadas no SIOP', '' );
?>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" value="pesquisa" id="requisicao"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td colspan="4" class="subtitulocentro">Filtros de Pesquisa</td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="15%">Emenda:</td>
		<td width="33%">
			<?php 
				$codigoemenda = $_REQUEST["codigoemenda"];
				echo campo_texto( 'codigoemenda', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="190px">Unidade Orçamentária:</td>
			<td>
			<?php 
				
				$codigouo = $_REQUEST["codigouo"];
				$sql = array();
				$sql = "SELECT DISTINCT 
							u.unicod as codigo,
							u.unicod||' - '||u.unidsc as descricao
						FROM
							emenda.emenda e
							INNER JOIN emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
							INNER JOIN public.unidade u ON u.unicod = f.unicod
						ORDER BY descricao";
				
				$db->monta_combo("codigouo",$sql,'S',"Todas",'','','','500','N','codigouo', '', '', '', 'class="sel_chosen"');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="15%">Parlamentar:</td>
		<td width="33%">
			<?php 
				$sql = "SELECT
							autcod as codigo,
						    autnome as descricao
						FROM
							emenda.autor
						WHERE autstatus = 'A' and autcod is not null
						ORDER BY autnome";
				$codigoparlamentar = $_REQUEST["codigoparlamentar"];
				$db->monta_combo("codigoparlamentar",$sql,'S',"Todos Parlamentares",'','','','500','N','codigoparlamentar', '', '', '', 'class="sel_chosen"');
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="15%">Mostra Retorno:</td>
		<td width="33%"><input type="radio" name="mostraRetorno" value="S">Sim
						<input type="radio" name="mostraRetorno" checked="checked" value="N">Não</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="4" align="center">
			<input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisa();" style="cursor: pointer;"/>
			<input type="button" value="Carregar Emendas" id="btnCarga" name="btnCarga" onclick="carregarEmendasSiop();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
<div id="debug"></div>
<script type="text/javascript">
jq('.sel_chosen').chosen({allow_single_deselect:true});

function carregarEmendasSiop(){
	jQuery('[name="requisicao"]').val('cargaSiop');
	jQuery('[name="formulario"]').submit();	
}

function pesquisa(){	
	jQuery('[name="requisicao"]').val('pesquisar');
	jQuery('[name="formulario"]').submit();
}

function carregarBeneficiarioEmenda(cesid, cnpjbeneficiario, obj) {
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=carregarBeneficiarioEmenda&cesid="+cesid+"&cnpjbeneficiario="+cnpjbeneficiario,
   		async: false,
   		success: function(msg){
   	   		jQuery('#debug').html(msg);
   	   		alert('Operação Realizada com Sucesso');
   		}
	});
}

function carregarListaBeneficiario(cesid, obj) {
	
	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;

	if(obj.title == 'mais') {  			
		obj.title='menos';
		jQuery('#image_'+cesid).attr('class', 'glyphicon glyphicon-upload');
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=16;
		ncol.innerHTML="Carregando...";
		ncol.innerHTML="<div id='listaobracesid_" + cesid + "' ></div>";
		
		carregarListaDivEmenda( cesid );
	} else {
		obj.title='mais';
		jQuery('#image_'+cesid).attr('class', 'glyphicon glyphicon-download');
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
}

function carregarListaDivEmenda( cesid ){
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=carregarListaBeneficiario&cesid="+cesid,
   		async: false,
   		success: function(msg){
   	   		jQuery('#listaobracesid_'+cesid).html(msg);
   		}
	});
}
</script>
</form>
<?php 
//cesid
$arWere = array();

if( $_REQUEST['mostraRetorno'] == 'S' ){
		echo '<pre>';
		print_r($retorno);
		echo '</pre>';
} else {
	if( !empty($_REQUEST['codigoparlamentar']) ){
		array_push($arWere, "codigoparlamentar = '{$_REQUEST['codigoparlamentar']}'");
	}
	if( !empty($_REQUEST['codigoemenda']) ){
		array_push($arWere, "codigoemenda = '{$_REQUEST['codigoemenda']}'");
	}
	if( !empty($_REQUEST['codigouo']) ){
		array_push($arWere, "codigouo = '{$_REQUEST['codigouo']}'");
	}
	
	$imgMais = "'<center><span style=\"font-size:16px; cursor:pointer;\" title=\"mais\" id=\"image_'||cesid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaBeneficiario('||cesid||', this);\"></span></center>'";
				
	$sql = "SELECT $imgMais as mais, identificadorunicolocalizador, esfera, codigouo, codigoprograma, 
		       codigofuncao, codigosubfuncao, codigoacao||'&nbsp;' as codigoacao, codigolocalizador, 
		       naturezadespesa, resultadoprimario, fonte, iduso, codigoparlamentar, 
		       nomeparlamentar, numeroemenda, codigoemenda, emendaano, codigopartido, 
		       siglapartido, ufparlamentar, valoratual, planoorcamentario
		  FROM emenda.cargaemendasiop
		".($arWere ? ' where '.implode(' and ', $arWere) : '')." order by codigoemenda";
	
	$cabecalho = array('Ações', "identificadorUnico", "esfera", "codigouo", "codigoprograma", "codigofuncao", "codigosubfuncao", "codigoacao", "codigolocalizador",  "naturezadespesa", "resultadoprimario", 
		"fonte", "iduso", "codigoparlamentar", "nomeparlamentar", "numeroemenda", "codigoemenda", "emendaano", "codigopartido", "siglapartido", "ufparlamentar", "valoratual", "planoorcamentario");
	$db->monta_lista_simples( $sql, $cabecalho,100,5,'N','95%','S', true, false, false, true);
}

function insereBeneficiarioEmendas( $dados ){
	global $db;
	
	$sql = "select b.cnpjbeneficiario, b.nomebeneficiario, b.valorapuradorcl, b.valorrevisadobeneficiario 
			from emenda.beneficiarioemenda b 
			where b.cnpjbeneficiario = '".$dados['cnpjbeneficiario']."' and b.cesid = ".$dados['cesid']." and b.ano = '".date('Y')."'";
	$arBeneficiario = $db->pegaLinha($sql);
	
	$sql = "select c.codigoemenda, c.naturezadespesa, c.fonte from emenda.cargaemendasiop c 
			where c.cesid = {$dados['cesid']}";
	$arEmenda = $db->pegaLinha($sql);
	
	$gnd = substr($arEmenda['naturezadespesa'], 0, 1);
	$emecod = $arEmenda['codigoemenda'];
	$fonte = $arEmenda['fonte'];

	$enbid = $db->pegaUm("SELECT enbid FROM emenda.entidadebeneficiada WHERE enbcnpj='".$dados['cnpjbeneficiario']."' and enbano = '".date('Y')."'");

	if( empty($enbid) ){
		$sql = "INSERT INTO emenda.entidadebeneficiada(enbstatus, enbano, enbdataalteracao, enbnome, enbcnpj, muncod, estuf)
	    				VALUES ('A',
	    						'".date('Y')."',
	    						NOW(),
	    						'".$arBeneficiario['nomebeneficiario']."',
	    						'".$arBeneficiario['cnpjbeneficiario']."',
	    						'null',
	    						'null') RETURNING enbid";
		$enbid = $db->pegaUm($sql);
	}

	$edevalor = ($arBeneficiario['valorapuradorcl'] ? $arBeneficiario['valorapuradorcl'] : 'null');
	$edevalordisponivel = ($arBeneficiario['valorrevisadobeneficiario'] ? $arBeneficiario['valorrevisadobeneficiario'] : 'null');

	$sql = "select 
			    ed.emdid, e.etoid, a.autcod
			from emenda.emenda e
				inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
				inner join emenda.autor a on a.autid = e.autid
			where
				e.emecod = '{$emecod}'
				and ed.gndcod = '{$gnd}' 
				and ed.foncod = '{$fonte}'
			    and e.emeano = '".date('Y')."'";
	$arDetalhe = $db->pegaLinha($sql);
	$emdid = $arDetalhe['emdid'];
	$etoid = $arDetalhe['etoid'];
	$autcod = $arDetalhe['autcod'];
	
	$ededisponivelpta = 'N';
	if( $etoid == 4 ){
		$ededisponivelpta = 'S';
	}
	
	if( !empty($emdid) ){
		
		$edeid = $db->pegaUm("select edeid from emenda.emendadetalheentidade where emdid = $emdid and edestatus = 'A' and enbid = $enbid and edestatus = 'A'");
				
		if( empty($edeid) ){ 
			$sql = "INSERT INTO emenda.emendadetalheentidade ( emdid, enbid, edevalor, edevalordisponivel, usucpfalteracao, ededataalteracao, ededisponivelpta, edestatus )
					VALUES ( {$emdid}, {$enbid}, {$edevalor}, {$edevalordisponivel}, '00000000191', 'now()', '$ededisponivelpta', 'A' ) RETURNING edeid";
			
			$edeid = $db->pegaUm( $sql );
			$retorno = true;
			if( $etoid != 4 ) EnviaEmailSIOP( $emecod, $autcod );
		} else {		
			$sql = "UPDATE emenda.emendadetalheentidade SET enbid = {$enbid}, edevalor = {$edevalor}, edevalordisponivel = {$edevalordisponivel}, edestatus = 'A', ededisponivelpta = '$ededisponivelpta' WHERE edeid = {$edeid}";
			$db->executar($sql);
			$retorno = false;
		}
		$db->commit();
	}
	return $retorno;
}

function EnviaEmailSIOP( $codigoemenda, $codigoParlamentar ){
	global $db;
	
	$conteudo = '<p><b>Senhor(a) parlamentar,</b></p>
					a indicação da emenda '.$codigoemenda.'/'.date(Y).' foi validada no SIOP.<br>
					O próximo passo é o preenchimento, até 07/08/'.date(Y).' no SIMEC/Emendas da iniciativa, dos dados do responsável pela elaboração do PTA e, quando se tratar de prefeitura e secretaria estadual, da vinculação da subação.<br>
					Qualquer dúvida, tratar com a ASPAR do MEC (2022-7899/7896/7894)';

	$remetente = array('nome' => SIGLA_SISTEMA. ' - MÓDULO EMENDAS', 'email' => 'noreply@simec.gov.br');
		
	if($_SESSION['baselogin'] != "simec_desenvolvimento" && $_SESSION['baselogin'] != "simec_espelho_producao" ){
		$email = $db->pegaUm("select a.autemail from emenda.autor a where a.autcod = '{$codigoParlamentar}'");
	} else {
		$email = $_SESSION['email_sistema'];
	}
	if( !empty($email) ){
		$retorno = enviar_email($remetente, array($email), SIGLA_SISTEMA. ' - EMENDAS', $conteudo, $cc, null);
		$boEnviado = true;
	}
}
?>