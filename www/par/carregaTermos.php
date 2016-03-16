<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/par/_funcoesPar.php";
include APPRAIZ . "www/par/_constantes.php";

$db = new cls_banco();

if( $_POST['requisicao'] == 'carregarmunicipio' ){
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio where estuf = '{$_POST['estuf']}' order by mundescricao asc";
	$arrMunicipio = $db->carregar($sql);
	$arrMunicipio = $arrMunicipio ? $arrMunicipio : array();
	
	$html = '<select name="muncod" id="muncod" class="CampoEstilo" style="width: auto">
				<option value="">Todas as Unidades Federais</option>';
	
	foreach ($arrMunicipio as $v) {
		$html.= '<option value="'.$v['codigo'].'">'.$v['descricao'].'</option>';
	}
	$html.= '</select>';
	
	
	echo $html;
	exit();
}

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

monta_titulo( 'CONSULTA DE TERMO DE COMPROMISSO', '' );
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" id="requisicao" name="requisicao" value="<?=$_POST['requisicao']; ?>">
	<table id="total" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td class="subtitulocentro" colspan="2">
			<input type="radio" name="secretaria" <?=($_POST['secretaria'] == 'SE' ? 'checked="checked"' : '') ?> id="secretaria_se" onclick="selecionaTipo(this.value)" value="SE"> Secretaria Estadual
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="secretaria" <?=($_POST['secretaria'] == 'PM' ? 'checked="checked"' : '') ?>  id="secretaria_pm" onclick="selecionaTipo(this.value)" value="PM"> Prefeitura Municipal</td>
		</tr>
		<tr id="estado">
			<td class="subtitulodireita" width="40%">UF:</td>
			<td><?
				$estuf = $_POST['estuf'];
				$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
				 $db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '', '', '', '', 'estuf' ); ?></td>
		</tr>
		<tr id="estado_mun">
			<td class="subtitulodireita" width="40%">UF:</td>
			<td><?
				$estuf_mun = $_POST['estuf_mun'];
				$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
				 $db->monta_combo( "estuf_mun", $sql, 'S', 'Todas as Unidades Federais', 'carregaMunicipio', '', '', '', '', 'estuf_mun'); ?></td>
		</tr>
		<tr id="municipio">
			<td class="subtitulodireita" width="40%">Município:</td>
			<td><div id="combomunicipio"><?
				if( $_POST['estuf_mun'] != '' ) $filtroMuni = " where estuf = '{$_POST['estuf_mun']}'";
				$muncod = $_POST['muncod'];
				$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio $filtroMuni order by mundescricao asc";
				 $db->monta_combo( "muncod", $sql, 'S', 'Todas os Municípios', '', '' ); ?></div></td>
		</tr>
		<tr id="botao">
			<td colspan="2" class="subtitulocentro">
				<input type="button" name="btnPesquisa" id="btnPesquisa" value="Pesquisar" onclick="carregarTermo()" />
			</td>
		</tr>
	</table>
</form>
<div id="mostralista">
<?
if( $_POST ){
	
	$celWidth = Array("10%","10%","50%","10%","20%");

echo ' <table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
			<tr>
				<td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
					Documentos do PAR
				</td>
			</tr>
		</table>';
	
	if( $_POST['estuf'] ){
		$filtro = "estuf = '{$_POST['estuf']}'";
	}else{
		$filtro = "muncod = '{$_POST['muncod']}'";
	}

	// PAR
	$sql = "SELECT
				'<center><img src=../imagens/icone_lupa.png style=cursor:pointer; 
					 onclick=\"window.open(\'visualizaTermo.php?dopid='||id||'\',\'visualizatermo\',
					 						\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\">
				</center>' as acao, 
				dopnumerodocumento,
				classificacao,
				'<center><img src=\"../imagens/obras/check.png\" width=\"18\"></center>' as data,
				'PAR' as origem
			FROM (
			SELECT 	
				dp.dopid as id, dp.dopnumerodocumento, dp.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, 
				dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, dp.dopnumerodocumento as doc, dopdatavalidacaogestor,
				CASE 
					WHEN pp.prptipoexecucao = 'T' THEN 'PAR - Genérico'
	               	ELSE 'PAR' 
				END as classificacao,
				(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, dopidpai
			FROM par.vm_documentopar_ativos  dp
			INNER JOIN par.modelosdocumentos   	d  ON d.mdoid = dp.mdoid
			INNER JOIN par.processopar 			pp ON pp.prpid = dp.prpid and pp.prpstatus = 'A'
			INNER JOIN par.instrumentounidade 	iu ON iu.inuid = pp.inuid 
			) as foo
			WHERE 
				contagem = mdoqtdvalidacao
				AND $filtro";

	$cabecalho = array("Selecione", "Nº do Documento" , 'Iniciativa', 'Validação', 'Origem');
	$db->monta_lista($sql, $cabecalho, 100000, 5, 'N', 'center', 'N', 'formassinatura', $celWidth);

echo ' <table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
			<tr>
				<td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
					Documentos de Obras do PAR
				</td>
			</tr>
		</table>';

	// Obras do PAR
	$sql = "SELECT
 				'<center><img src=../imagens/icone_lupa.png style=cursor:pointer; 
 					 onclick=\"window.open(\'visualizaTermo.php?dopid='||id||'\',\'visualizatermo\',
 					 						\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\">
 				</center>' as acao,
 				dopnumerodocumento,
 				classificacao,
 				'<center><img src=\"../imagens/obras/check.png\" width=\"18\"></center>' as data,
				'Obras do PAR' as origem
		 	FROM (
				SELECT 
					dp.dopid as id, dp.dopnumerodocumento, dp.mdonome as tipodocumento, iu.inuid, iu.estuf, iu.muncod, 
					dp.dopusucpfvalidacaogestor, d.mdoqtdvalidacao, dp.dopnumerodocumento as doc, dopdatavalidacaogestor,
					CASE 
						WHEN pp.protipo = 'T' THEN 'PAR - Obras'
 	            		ELSE '' 
 	            	END as classificacao,
					(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem, dopidpai
				FROM par.vm_documentopar_ativos dp
				INNER JOIN par.modelosdocumentos   	d  ON d.mdoid = dp.mdoid
				INNER JOIN par.processoobraspar 	pp ON pp.proid = dp.proid  and pp.prostatus = 'A'
				INNER JOIN par.instrumentounidade 	iu ON iu.inuid = pp.inuid
			) as foo
			WHERE
				contagem = mdoqtdvalidacao AND $filtro";
	
	$cabecalho = array("Selecione", "Nº do Documento" , 'Iniciativa', 'Validação', 'Origem');
	$db->monta_lista($sql, $cabecalho, 100000, 5, 'N', 'center', 'N', 'formassinatura', $celWidth);
	
echo ' 	<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
			<tr>
				<td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
					Documentos de Obras do PAR
				</td>
			</tr>
		</table>';
	
	if( $_POST['estuf'] ){
		$filtro = "tc.estuf = '{$_POST['estuf']}'";
	}else{
		$filtro = "tc.muncod = '{$_POST['muncod']}'";
	}

	// PAC
	$sql = "SELECT 
				CASE WHEN tc.estuf IS NOT NULL THEN
 			    	'<center><img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"window.open(\'visualizaTermoPac.php?terid='||terid||'&estuf='||tc.estuf||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>'
 			  	ELSE
 			    	'<center><img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"window.open(\'visualizaTermoPac.php?terid='||terid||'&muncod='||tc.muncod||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>' 
 			    END as acoes,
				'PAC2'||to_char(tc.terid,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY') as codigo,
				CASE 
					WHEN po.protipo = 'P' THEN 'PAC - Proinfância'
                 	WHEN po.protipo = 'Q' THEN 'PAC - Quadras'
                    WHEN po.protipo = 'C' THEN 'PAC - Cobertura' 
              	END as classificacao,
				'<center><img src=\"../imagens/obras/check.png\" title=\"Documento Validado\" width=\"18\"></center>' as aprovado,
				'PAC' as origem
			FROM 
				par.termocompromissopac  tc
			INNER JOIN par.processoobra 	po ON po.proid = tc.proid	and po.prostatus = 'A' 
			LEFT  JOIN seguranca.usuario 	u  ON u.usucpf = tc.usucpfassinatura 
			WHERE 
				tc.terstatus = 'A' AND ( usucpfassinatura is not null OR terassinado = 't' ) AND $filtro";
	
	$cabecalho = array("Selecione", "Nº do Documento" , 'Iniciativa', 'Validação', 'Origem');
	$db->monta_lista($sql, $cabecalho, 100000, 5, 'N', 'center', 'N', 'formassinatura', $celWidth);
}


?>
</div>
<script type="text/javascript">
$(document).ready(function() {
	$('#estado_mun').hide();
	$('#estado').hide();
    $('#botao').hide();
	$('#municipio').hide();
	$('#mostralista').hide();
	$('#id_img').attr('title', 'Documento foi validado pelo representante legal da entidade, mediante a inserção de assinatura pessoal no documento impresso, que se encontra arquivado no processo correspondente.');
	
	if( $('#secretaria_se').attr('checked') == true ){
		selecionaTipo('SE');
		$('#botao').show();
		$('#mostralista').show();
	} else if( $('#secretaria_pm').attr('checked') == true ) {
		selecionaTipo('PM');
		$('#botao').show();
		$('#municipio').show();
		$('#mostralista').show();
	}
});

function carregaMunicipio(estuf){
	if( estuf != '' ){
		$.ajax({
	   		type: "POST",
	   		url: "carregaTermos.php",
	   		data: "requisicao=carregarmunicipio&estuf="+estuf,
	   		async: false,
	   		success: function(msg){
	   			$('#municipio').show();
	   			$('#botao').show();
	   			document.getElementById('combomunicipio').innerHTML = msg;
	   		}
		});
	} else {
		$('#botao').hide();
		$('#municipio').hide();
	}
}

function selecionaTipo(valor){
	if(valor == 'SE'){
		$('#estado').show();
		$('#municipio').hide();
		$('#estado_mun').hide();
		$('#mostralista').hide();
		$('#botao').show();
		$('#estuf_mun').val('');
		$('#muncod').val('');
	} else {
		$('#estado').hide();
		$('#estado_mun').show();
		$('#municipio').hide();
		$('#mostralista').hide();
		$('#estuf').val('');
		$('#botao').hide();
	}
}

function carregarTermo(){
	if( $('#estado').css('display') != 'none' && $('#estuf').val() == '' ){
		alert('É necessário informar um estado!');
		$('#estuf').focus();
		return false;
	}
	
	if( $('#municipio').css('display') != 'none' && $('#muncod').val() == '' ){
		alert('É necessário informar um município!');
		$('#muncod').focus();
		return false;
	}
	$('#requisicao').val('pesquisar');
	$('#formulario').submit();
}

 

</script>