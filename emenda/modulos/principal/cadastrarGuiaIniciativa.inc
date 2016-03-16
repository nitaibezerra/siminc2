<?php
if( $_POST['requisicao'] == 'salvar' ){
	extract($_POST);
	
	$arrGesid = $gesid;
	
	$ginidetalhamento = (($_REQUEST["ginidetalhamento"] != "") ? stripcslashes(pg_escape_string( str_replace( "'", '"', $_REQUEST["ginidetalhamento"]))) : 'NULL');
	
	if( !$ginid ){
		$sql = "INSERT INTO emenda.guiainiciativa(iniid, tpeid, ginidetalhamento) 
				VALUES ($iniid, $tpeid, '$ginidetalhamento') RETURNING ginid";
		$ginid = $db->pegaUm( $sql );
	} else {
		$sql = "UPDATE emenda.guiainiciativa SET 
				  	--iniid = $iniid,
				  	tpeid = $tpeid,
				  	ginidetalhamento = '$ginidetalhamento'
				WHERE 
				  	ginid = $ginid";
  		$db->executar( $sql );
	}
	$_SESSION['emenda']['ginid'] = $ginid;
	$sql = "DELETE FROM emenda.guia_guiainiciativa WHERE ginid = ".$ginid;
	$db->executar($sql);
	$sql = "INSERT INTO emenda.guia_guiainiciativa(guiid, ginid) 
			VALUES ({$_SESSION['emenda']['guiid']}, $ginid)";
	$db->executar($sql);
	
	$db->executar("DELETE FROM emenda.guia_guiadocumento WHERE ginid = ".$ginid);
	if( is_array($gdoid) && $gdoid[0] ){
		foreach ($gdoid as $valor) {
			$sql = "INSERT INTO emenda.guia_guiadocumento(ginid, gdoid) 
					VALUES ($ginid, $valor)";
			$db->executar( $sql );		
		}
	}
	
	$_SESSION['emenda']['ginid'] = $ginid;
		
	if( $db->commit() ){
		$db->sucesso('principal/cadastrarGuiaEspecificacao');
	} else {
		echo "<script>
				alert('Falha na operação');
			</script>";
	}
}

if( $_REQUEST['guiid'] ){
	$_SESSION['emenda']['guiid'] = $_REQUEST['guiid'];
}

//if( $_REQUEST['ginid'] ) $_SESSION['emenda']['ginid'] = $_REQUEST['ginid'];
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Guias", "Cadastrar Guias Iniciativas" );
cabecalhoGuia('CADASTRO DE INICIATIVAS');

extract( $_POST );

/*if($_REQUEST['ginid'] || $_SESSION['emenda']['ginid']){
	$sql = "SELECT 
			  	ginid,
			  	iniid,
			  	tpeid,
			  	ginidetalhamento
			FROM 
			  	emenda.guiainiciativa WHERE ginid = ".($_REQUEST['ginid'] ? $_REQUEST['ginid'] : $_SESSION['emenda']['ginid']);
	$arrGuia = $db->pegaLinha( $sql );
	$arrGuia = $arrGuia ? $arrGuia : array();
	extract($arrGuia);
}*/

if($_REQUEST['guiid'] || $_SESSION['emenda']['guiid']){
	$sql = "SELECT 
			  	gi.ginid,
			  	gi.iniid,
			  	gi.tpeid,
			  	gi.ginidetalhamento
			FROM 
			  	emenda.guiainiciativa gi
                inner join emenda.guia_guiainiciativa ggi on ggi.ginid = gi.ginid
            WHERE ggi.guiid = ".($_REQUEST['guiid'] ? $_REQUEST['guiid'] : $_SESSION['emenda']['guiid'])." and gi.ginistatus = 'A'";
	$arrGuia = $db->pegaLinha( $sql );
	$arrGuia = $arrGuia ? $arrGuia : array();
	extract($arrGuia);
	
	$_SESSION['emenda']['ginid'] = $ginid;
}

echo '<br>';
echo montarAbasArray(carregaAbasGuia('cadastrarGuiaIniciativa'), "/emenda/emenda.php?modulo=principal/cadastrarGuiaIniciativa&acao=A");
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="ginid" id="ginid" value="<?=$ginid; ?>">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:29.5%;">Iniciativa:</td>
		<td><?
		$sql = "SELECT distinct
					ini.iniid as codigo,
				    ini.ininome as descricao
				FROM
					emenda.iniciativa ini
				WHERE
					ini.inistatus = 'A'
				order by 
					ini.ininome";
		
		$arIniciativa = $db->carregar( $sql );
		$arIniciativa = $arIniciativa ? $arIniciativa : array();
		
		foreach ($arIniciativa as $chave => $val) {
			
			$sql = "SELECT 
						res.resdsc
					FROM 
					  emenda.responsavel res
					  inner join emenda.iniciativaresponsavel inr
					  	on res.resid = inr.resid
					WHERE
						inr.iniid = ".$val['codigo']."
						and inr.irestatus = 'A'";
			
			$arResp = $db->carregarColuna( $sql );
			
			$arDados[$chave] = array("codigo" => $val['codigo'],
				         		     "descricao" => implode(', ', $arResp).' - '.$val['descricao']
									);
		}

		echo $db->monta_combo("iniid", $arDados, 'S','-- Selecione --','carregaIniid', '', '',400,'S','iniid', '', '', 'Iniciativa'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:29.5%;">Tipo de Ensino:</td>
		<td><?
		$sql = "SELECT
					tip.tpeid as codigo,
				    tip.tpedsc as descricao
				FROM
					emenda.tipoensino tip
				WHERE
					tip.tpestatus = 'A'
				order by 
					tip.tpedsc";
		echo $db->monta_combo("tpeid", $sql, 'S','-- Selecione --','', '', '',250,'S','tpeid', '', '', 'Tipo de Ensino'); ?></td>
	</tr>
	<tr>
	    <td class="SubTituloDireita">Detalhamento da Iniciativa:</td>
	    <td><?=campo_textarea('ginidetalhamento', 'S', 'S', 'Detalhamento da Iniciativa', 80, 8, 4000, '', '', '', '', 'Detalhamento da Iniciativa');?></td>
	</tr>
	<?if( $_REQUEST['iniid'] || $iniid ){ ?>
	<tr>
		<td class="SubTituloDireita">Documentos Obrigatórios:</td>
		<td><?
				$gdoid = array();
				if($_REQUEST["ginid"] || $_SESSION['emenda']['ginid']) {
					$sql = "SELECT 
							  	gd.gdoid as codigo,
							  	gd.gdonome as descricao
							FROM 
							  	emenda.guiadocumento gd
							  	inner join emenda.guia_guiadocumento gc on gc.gdoid = gd.gdoid
							WHERE
								gc.ginid = ".($_REQUEST["ginid"] ? $_REQUEST["ginid"] : $_SESSION['emenda']['ginid'])."
							    and gd.gdostatus = 'A'";
					
					$gdoid = $db->carregar( $sql );
					$gdoid = $gdoid ? $gdoid : array();
				}
				$iniid = $_REQUEST['iniid'] ? $_REQUEST['iniid'] : $db->pegaUm("SELECT iniid FROM emenda.guiainiciativa WHERE ginid = ".($_REQUEST["ginid"] ? $_REQUEST["ginid"] : $_SESSION['emenda']['ginid']));
				$sql = "SELECT 
							gd.gdoid as codigo,
  							gd.gdonome as descricao
						FROM 
						  	emenda.guiadocumento gd
                            inner join emenda.iniciativa_guiadocumento igd on igd.gdoid = gd.gdoid
						WHERE 
							gd.gdostatus = 'A' 
                            and igd.iniid = $iniid
                        order by gd.gdonome";
				
				combo_popup('gdoid', $sql, '', '400x400', 0, array(), '', 'S', false, false, 05, 400 );
			?></td>
	</tr>
	<?} ?>
	<tr>
		<th align="center" colspan="2">
			<input type="button" value="Anterior" name="btnAnterior" id="btnAnterior"/>
			<input type="submit" value="Salvar" name="btnSalvar" id="btnSalvar">
			<input type="button" value="Próximo" name="btnProximo" id="btnProximo"/></th>
	</tr>
</table>
</form>
<script type="text/javascript">
$(document).ready(function(){
	if($('#ginid').val() != ''){
		$('#iniid').attr('disabled', 'disabled');
	}
	$('#btnSalvar').click(function(){
	
		if( $('#iniid').val() == '' ){
			alert('O campo "Iniciativa" é obrigatório.');
			$('#iniid').focus();
			return false;
		}
		if( $('#tpeid').val() == '' ){
			alert('O campo "Tipo de Ensino" é obrigatório.');
			$('#tpeid').focus();
			return false;
		}
		if( $('#ginidetalhamento').val() == '' ){
			alert('O campo "Justificativa" é obrigatório.');
			$('#ginidetalhamento').focus();
			return false;
		}
		
		selectAllOptions( document.getElementById( 'gdoid' ) );
		/*var valor = document.getElementById( 'gdoid' ).options[0].value;
		if( valor == '' ){
			alert('O campo "Documentos Obrigatórios" é obrigatório.');
			$('#gdoid').focus();
			return false;
		}*/
		
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	});
	
	$('#btnAnterior').click(function(){
		window.location.href = 'emenda.php?modulo=principal/guiaListaIniciativa&acao=A';
	});
	
	$('#btnProximo').click(function(){
		window.location.href = 'emenda.php?modulo=principal/cadastrarGuiaEspecificacao&acao=A';
	});
});

function carregaIniid(iniid){
	$('#requisicao').val('carregainiid');
	$('#formulario').submit();
}
</script>