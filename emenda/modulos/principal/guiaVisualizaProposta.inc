<?php
if($_REQUEST['preenchesession']){
	header('content-type: text/html; charset=ISO-8859-1');
	$arrGuia = $_SESSION['emenda']['cadGuia'] ? $_SESSION['emenda']['cadGuia'] : array();

	if( $_POST['action'] == 'insere' ){
		if( !in_array($_POST['guiid'], $arrGuia) ){
			array_push( $arrGuia, $_POST['guiid'] );
			$_SESSION['emenda']['cadGuia'] = $arrGuia;
		}
	} else {
		foreach ($arrGuia as $key => $v) {
			if( $v == $_POST['guiid'] ){
				unset($arrGuia[$key]);
				break;
			}
		}
		$_SESSION['emenda']['cadGuia'] = $arrGuia;
	}
	exit;
}
unset($_SESSION['emenda']['cadGuia']);
$enbid = $_REQUEST['enbid'];
$resid = $_REQUEST['resid'];
$sql = "SELECT DISTINCT
				ini.iniid,
   				g.guinome,
   				g.guijustificativa,
   				g.guiid,
   				ini.ininome,
   				gi.ginidetalhamento,
                tpe.tpedsc,
                gi.ginid,
                res.resdsc
			FROM
				emenda.guia g
                inner join emenda.guia_guiainiciativa ggi on ggi.guiid = g.guiid
                inner join emenda.guiainiciativa gi on gi.ginid = ggi.ginid
                inner join emenda.iniciativa ini on ini.iniid = gi.iniid
			    inner join emenda.tipoensino tpe on tpe.tpeid = gi.tpeid
                inner join emenda.iniciativaresponsavel inr on inr.iniid = gi.iniid
                inner join emenda.responsavel res on res.resid = inr.resid
				inner join emenda.iniciativadetalheentidade ide on ide.iniid = gi.iniid
			    inner join emenda.emendadetalheentidade ede on ede.edeid = ide.edeid
			WHERE
				ede.enbid = $enbid
			    and inr.resid = $resid
			    and gi.ginistatus = 'A'
			    and ide.idestatus = 'A'
    			and g.guistatus = 'A'";
$arrGuia = $db->carregar( $sql );
$arrGuia = $arrGuia ? $arrGuia : array();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<input type="hidden" name="resid" id="resid" value="<?=$resid; ?>">
<input type="hidden" name="enbid" id="enbid" value="<?=$enbid; ?>">
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
	 <tbody>
	 	<tr>
	 		<td colspan="4" class="subtitulocentro">Dados da Guia</td>
	 	</tr>
	 	<tr>
	 		<td colspan="4" class="SubTituloEsquerda">Selecionar Todos
	 			<input type="checkbox" name="todos" id="todos" value="" onclick="checarTodos();">
	 		</td>
	 	</tr>
	 	<?
	 	foreach ($arrGuia as $key => $guia) {
	 		$key % 2 ? $cor = "" : $cor = "#f4f4f4";
	 	 ?>
	 	<tr bgColor="<?=$cor;?>">
	 		<td><input type="checkbox" name="ckeck_guia" id="check_<?=$guia['guiid'] ?>" value="<?=$guia['guiid'].'|'.$guia['guinome'] ?>" onclick="checarGuia();"></td>
	 		<td>
	 			<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
	 			<tbody>
	 				<tr>
						<td style="text-align: right; width:20.0%;" class="SubTituloEsquerda">Guia:</td>
						<td><?=$guia['guinome']?></td>
					</tr>
	 				<tr>
						<td style="text-align: right; width:20.0%;" class="SubTituloEsquerda">Justificativa:</td>
						<td><?=(strlen($guia['guijustificativa']) > 500 ? delimitador($guia['guijustificativa']).'<br><a href="#" onclick="abreDescricao('.$guia['guiid'].', \'just\')">Cotinuar lendo...</a>': $guia['guijustificativa']); ?></td>
					</tr>
					<tr>
						<td style="text-align: right;" class="SubTituloEsquerda">Iniciativa:</td>
						<td><?=$guia['ininome']?></td>
					</tr>
					<tr>
						<td style="text-align: right;" class="SubTituloEsquerda">Tipo de Ensino:</td>
						<td><?=$guia['tpedsc']?></td>
					</tr>
					<tr>
						<td style="text-align: right;" class="SubTituloEsquerda">Responsável:</td>
						<td><?=$guia['resdsc']?></td>
					</tr>
					<tr>
						<td style="text-align: right;" class="SubTituloEsquerda">Detalhamento da Iniciativa:</td>
						<td><?=(strlen($guia['ginidetalhamento']) > 500 ? delimitador($guia['ginidetalhamento']).'<br><a href="#" onclick="abreDescricao('.$guia['guiid'].', \'detalhe\')">Cotinuar lendo...</a>': $guia['ginidetalhamento']); ?></td>
					</tr>
					<tr>
						<th colspan="2">Especificações</th>
					</tr>
					<tr>
						<td colspan="2">
							<table align="center" width="100%" class="Tabela" cellpadding="2" cellspacing="1">
								<tr>
									<th>Especificação</th>
									<th>Itens</th>
								</tr>
								<?
									$sql = "SELECT DISTINCT
												esp.espid,
												esp.espnome
											FROM
												emenda.guiaespecificacao ge
											    inner join emenda.especificacao esp on esp.espid = ge.espid
											WHERE
												ge.ginid = ".$guia['ginid'];
									
									$arrEspecificacao = $db->carregar( $sql );
									$arrEspecificacao = $arrEspecificacao ? $arrEspecificacao : array();
									
									foreach ($arrEspecificacao as $chave => $valor) {
										$chave % 2 ? $cor = "" : $cor = "#e0e0e0";
										
										$sql = "SELECT descricao FROM (
												SELECT --'Item Par: '||pic.picdescricao as descricao
													pic.picdescricao as descricao
													FROM emenda.itempar gip
														inner join par.propostaitemcomposicao pic on pic.picid = gip.picid
									                    inner join emenda.itempar_especificacao ipe on ipe.ipaid = gip.ipaid
									                    inner join emenda.guiaespecificacao ge on ge.iteid = ipe.iteid
													WHERE
														pic.picstatus = 'A'
									                            and ge.espid = {$valor['espid']}
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
									                    and gie.espid = {$valor['espid']}
												) as foo
												ORDER BY descricao";
												
										$arrItem = $db->carregarColuna( $sql );
										$arrItem = $arrItem ? $arrItem : array();
										
										echo '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
													<td>'.$valor['espnome'].'</td>
													<td>'.($arrItem ? implode('<br>', $arrItem) : '-').'</td>
												</tr>';
									}
									?>
							</table>
						</td>
					</tr>
	 			</tbody>
	 			</table>
	 		</td>
	 	</tr>
		<?
	 	} ?>
	 	<tr>
			<th align="center" colspan="2">
				<input type="button" value="Fechar" name="btnFechar" id="btnFechar" onclick="window.close();"/></th>
		</tr>
	 </tbody>
</table>
<div id="lista"></div>
<script>
//<input type="hidden" name="guia[]" id="guia" value="">

if( window.opener.document.getElementById("guia_check").value != '' ){
	var check = window.opener.document.getElementById("guia_check").value;
	var select = check.split(",");
	var strguia = '';
	for(i=0; i<select.length; i++){
		strguia = 'check_'+select[i];
		//alert(strguia);
		document.getElementById(strguia).checked = true;
		 //('#'+strguia).checked = true;
	}
	/*$("[type~=checkbox]").each(function() {
		if(this.checked == false){
			this.checked = true;
		} else {
			this.checked = false;
		}
	});*/
}
	
function checarGuia(){
	var valorcheck = '';
	var valorGuia = '';
	var guiaNome = '';
	$("[type~=checkbox]").each(function() {
		if(this.checked == true){
			valorGuia = this.value.split("|");
			if( valorcheck == '' ){
				valorcheck = valorGuia[0];
				guiaNome = valorGuia[1];
			} else {
				valorcheck = valorcheck+','+valorGuia[0];
				guiaNome = guiaNome+'<br>'+valorGuia[1];
			}
		}
	});
	//alert(guiaNome);
	window.opener.document.getElementById("guia_check").value = valorcheck;
	window.opener.document.getElementById("tr_guia").style.display = '';
	window.opener.document.getElementById("td_guia").innerHTML = guiaNome;
	if( guiaNome == '' )
		window.opener.document.getElementById("tr_guia").style.display = 'none';
}

function checarTodos(){
	$("[type~=checkbox]").each(function() {
		if(this.checked == false){
			this.checked = true;
		} else {
			this.checked = false;
		}
	});
	checarGuia();
}

function abreDescricao( guiid, tipo ){
	var janela = window.open("emenda.php?modulo=principal/popupVisualizaDescricao&acao=A&guiid="+guiid+"&tipo="+tipo,"visualiza", "scrollbars=yes,height=400,width=600");
	janela.focus();
}
</script>