<?php
if( $_REQUEST['requisicao'] == 'excluir' ){
	$sql = "DELETE FROM emenda.guiaespecificacao WHERE espid = ".$_REQUEST['espid'];
	$db->executar($sql);
	if( $db->commit() ){
		$db->sucesso('principal/cadastrarGuiaEspecificacao');
	} else {
		echo "<script>
				alert('Falha na operação');
			</script>";
	}
}

if( $_POST['requisicao'] == 'salvar' ){
	extract($_POST);
	$arrIteid = $iteid;
	
	$sql = "DELETE FROM emenda.guiaespecificacao WHERE espid = $espid";
	$db->executar($sql);
	if( is_array($arrIteid) && $arrIteid[0] ){
		foreach ($arrIteid as $iteid) {
			$arIteid = explode('_', $iteid);
			$sql = "INSERT INTO emenda.guiaespecificacao(espid, iteid, ginid) 
					VALUES ($espid, {$arIteid[1]}, {$_SESSION['emenda']['ginid']})";
			$db->executar($sql);	
		}
	} else {
		$iteid = $iteid ? $iteid : 'null';
		$sql = "INSERT INTO emenda.guiaespecificacao(espid, iteid, ginid) 
				VALUES ($espid, $iteid, {$_SESSION['emenda']['ginid']})";
		$db->executar($sql);
	}
	
	if( $db->commit() ){
		$db->sucesso('principal/cadastrarGuiaEspecificacao');
	} else {
		echo "<script>
				alert('Falha na operação');
			</script>";
	}
}
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Guias", "Cadastrar Guias Especificação" );
cabecalhoGuia('CADASTRO DE ESPECIFICAÇÕES');
echo '<br>';
echo montarAbasArray(carregaAbasGuia('cadastrarGuiaEspecificacao'), "/emenda/emenda.php?modulo=principal/cadastrarGuiaEspecificacao&acao=A");
extract( $_POST );
$espid = $espid ? $espid : $_REQUEST['espid'];

if($espid){	
	$espkit = $db->pegaUm("SELECT espkit FROM emenda.especificacao WHERE espid = ".$espid);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" name="requisicao" id="requisicao" value="<?=$_REQUEST['requisicao']; ?>">
<input type="hidden" name="gieid" id="gieid" value="<?=$gieid; ?>">
<input type="hidden" name="espkit" id="espkit" value="<?=$espkit; ?>">

<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:29.5%;">Especificação:</td>
		<td><?
		
		if( $_REQUEST['requisicao'] == 'alterar' ){
			$sql = "SELECT 
				  	esp.espnome
				FROM 
				  	emenda.especificacao esp
				WHERE
				    esp.espstatus = 'A'
				    and esp.espid = $espid";
			echo $db->pegaUm( $sql );	    
			echo '<input type="hidden" name="espid" id="espid" value="'.$espid.'">';
		} else {
			$sql = "SELECT 
				  	esp.espid as codigo,
				  	esp.espnome as descricao
				FROM 
				  	emenda.especificacao esp
                    inner join emenda.especificacao_programacaoexercicio pe on pe.espid = esp.espid
                    inner join emenda.iniciativaespecificacao ie on ie.espid = esp.espid
				WHERE
				    esp.espstatus = 'A'
                    and pe.prsano = '{$_SESSION['exercicio']}'
                    and ie.iniid in (select iniid from emenda.guiainiciativa where ginid = {$_SESSION['emenda']['ginid']})
                    and esp.espid not in (select espid from emenda.guiaespecificacao where espid = esp.espid)
				order by esp.espnome";
				
			echo $db->monta_combo("espid", $sql, 'S','-- Selecione --','carregarItens', '', '',350,'S','espid', '', '', 'Especificação');
		}
		?></td>
	</tr>
	<?if($_REQUEST["espid"] && $espkit == 't') { ?>
	<tr>
		<td class="SubTituloDireita">Itens:</td>
		<td><?
				$iteid = array();
				if($_REQUEST["espid"]) {
					$sql = "select tipo||'_'||codigo as codigo, descricao, tipo from (
							SELECT 
								ipe.iteid as codigo,
							    pic.picdescricao as descricao,
							    'P' as tipo
							FROM emenda.itempar gip
								inner join par.propostaitemcomposicao pic on pic.picid = gip.picid
	                            inner join emenda.itempar_especificacao ipe on ipe.ipaid = gip.ipaid
	                            inner join emenda.guiaespecificacao ge on ge.iteid = ipe.iteid and ge.espid = ipe.espid
							WHERE
								pic.picstatus = 'A'
	                            and ipe.espid = {$_REQUEST["espid"]}
	                            and ge.ginid = {$_SESSION['emenda']['ginid']}
							UNION
								SELECT
							    	ipe.iteid as codigo,
							        pto.ptodescricao as descricao,
							        'O' as tipo
							    FROM
							    	emenda.itemobras io
							        inner join obras.pretipoobra pto on pto.ptoid = io.ptoid
							        inner join emenda.itempar_especificacao ipe on ipe.itoid = io.itoid
	                            	inner join emenda.guiaespecificacao ge on ge.iteid = ipe.iteid and ge.espid = ipe.espid
							    WHERE
							    	pto.ptostatus = 'A'
							        and ipe.espid = {$_REQUEST["espid"]}
							        and ge.ginid = {$_SESSION['emenda']['ginid']}) 
							as foo
							order by
								descricao";
					
					$iteid = $db->carregar( $sql );
					$iteid = $iteid ? $iteid : array();
				}
				$sql = "select tipo||'_'||codigo as codigo, descricao, tipo from (
						SELECT 
							ipe.iteid as codigo,
						    pic.picdescricao as descricao,
						    'P' as tipo
						FROM emenda.itempar gip
							inner join par.propostaitemcomposicao pic on pic.picid = gip.picid
                            inner join emenda.itempar_especificacao ipe on ipe.ipaid = gip.ipaid
						WHERE
							pic.picstatus = 'A'
                            and ipe.espid = {$_REQUEST["espid"]}
						UNION
						SELECT
					    	ipe.iteid as codigo,
					        pto.ptodescricao as descricao,
					        'O' as tipo
					    FROM
					    	emenda.itemobras io
					        inner join obras.pretipoobra pto on pto.ptoid = io.ptoid
					        inner join emenda.itempar_especificacao ipe on ipe.itoid = io.itoid
					    WHERE
					    	pto.ptostatus = 'A'
					        and ipe.espid = {$_REQUEST["espid"]}) 
							as foo
							order by
								descricao";
				
				if(is_array($iteid)){
					$arrIteid = array();
					foreach ($iteid as $v) {
						array_push($arrIteid, $v['codigo']);
					}					
				}
				$codigos_fixos = $arrIteid;
				$mensagem_fixo = 'Este item ja está vinculado a especificação no PTA';
				combo_popup('iteid', $sql, '', '400x400', 0, $codigos_fixos, $mensagem_fixo, 'S', false, false, 10, 600 );
				echo obrigatorio();
			?></td>
	</tr>
	<?} ?>
	<tr>
		<th align="center" colspan="2">
			<input type="button" value="Anterior" name="btnAnterior" id="btnAnterior"/>
			<input type="submit" value="Salvar" name="btnSalvar" id="btnSalvar"></th>
	</tr>
</table>
</form>
<?

$acao = "( '<center>
					<img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarEspecificacao('|| esp.espid ||');\" border=0 alt=\"Ir\" title=\"Alterar\"> ' ||
	    		   '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluirEspecificacao('|| esp.espid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";

$acaoE = "( '<center>
					<img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarEspecificacao('|| esp.espid ||');\" border=0 alt=\"Ir\" title=\"Alterar\"> ' ||
	    		   '<img src=\"/imagens/excluir_01.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Não pode excluir, pois já está vinculada ao PTA\"></center>' )";
	
	$sql = "SELECT DISTINCT
				case when pie.pteid is null then
					$acao
                else
                	$acaoE
                end as acao,
				esp.espid,
				esp.espnome
			FROM
				emenda.especificacao esp
			    inner join emenda.guiaespecificacao ige on ige.espid = esp.espid
                inner join emenda.guiainiciativa gi on gi.ginid = ige.ginid
                inner join emenda.guia_guiainiciativa ggi on ggi.ginid = gi.ginid
                left join emenda.guia_planotrabalho gp
                	inner join emenda.ptiniciativa pti on pti.ptrid = gp.ptrid
                    inner join emenda.ptiniciativaespecificacao pie on pie.ptiid = pti.ptiid
                    inner join emenda.iniciativaespecificacao ie on ie.iceid = pie.iceid
                on gp.guiid = ggi.guiid and ie.iniid = gi.iniid and ie.espid = esp.espid
			WHERE
				esp.espstatus = 'A'
				and ige.ginid = {$_SESSION['emenda']['ginid']}
			order by 
				esp.espnome";
	
	$arrDados = $db->carregar( $sql );
	$arrDados = $arrDados ? $arrDados : array();
	$cabecalho = array("Opção", "Especificação", "Itens");
	
	$arrRegistro = array();
	foreach ($arrDados as $key => $v) {
		$sql = "SELECT descricao FROM (
					SELECT --'Item Par: '||pic.picdescricao as descricao
						pic.picdescricao as descricao
						FROM emenda.itempar gip
							inner join par.propostaitemcomposicao pic on pic.picid = gip.picid
		                    inner join emenda.itempar_especificacao ipe on ipe.ipaid = gip.ipaid
		                    inner join emenda.guiaespecificacao ge on ge.iteid = ipe.iteid
						WHERE
							pic.picstatus = 'A'
		                            and ge.espid = {$v['espid']}
					union 
						SELECT
		                    --'Item Obras: '||pto.ptodescricao as descricao
		                    pto.ptodescricao as descricao
		                FROM
		                    emenda.itemobras io
		                    inner join obras.pretipoobra pto on pto.ptoid = io.ptoid
		                    inner join emenda.itempar_especificacao gie on gie.itoid = io.itoid
		                    inner join emenda.guiaespecificacao ge on ge.iteid = gie.iteid
		                WHERE
		                    pto.ptostatus = 'A'
		                    and gie.espid = {$v['espid']}
					) as foo
					ORDER BY descricao";		
					
		$arrItem = $db->carregarColuna( $sql );
		$arrItem = $arrItem ? $arrItem : array();
		
		$arrRegistro[$key] = array(
								"acao" => $v['acao'],
								"espnome" => $v['espnome'],
								"itens" => ($arrItem ? implode('<br>', $arrItem) : '-'),
								);	
	}	

	$db->monta_lista_array($arrRegistro, $cabecalho, 50, 10, 'N', 'center');
?>
<script type="text/javascript">
$(document).ready(function(){
	/*if($('#requisicao').val() == 'alterardados'){
		$('#espid').attr('readonly', 'readonly');
	}*/
	$('#btnSalvar').click(function(){
	
		if( $('#espid').val() == '' ){
			alert('O campo "Especificação" é obrigatório.');
			$('#espid').focus();
			return false;
		}
		
		if( $('#espkit').val() == 't' ){
			selectAllOptions( document.getElementById( 'iteid' ) );
			var valor = document.getElementById( 'iteid' ).options[0].value;
			if( valor == '' ){
				alert('O campo "Itens" é obrigatório.');
				$('#ipaid').focus();
				return false;
			}
		}
		
		$('#requisicao').val('salvar');
		$('#formulario').submit();
	});
	
	$('#btnAnterior').click(function(){
		window.location.href = 'emenda.php?modulo=principal/cadastrarGuiaIniciativa&acao=A';
	});
	
});

function carregarItens(espid){
	$('#requisicao').val('carregarespid');
	$('#formulario').submit();
}

function alterarEspecificacao(espid){
	window.location.href = 'emenda.php?modulo=principal/cadastrarGuiaEspecificacao&acao=A&espid='+espid+'&requisicao=alterar';
}

function excluirEspecificacao(espid){
	window.location.href = 'emenda.php?modulo=principal/cadastrarGuiaEspecificacao&acao=A&espid='+espid+'&requisicao=excluir';
}
</script>