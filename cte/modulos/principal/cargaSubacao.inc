<?php
function selecionarDimensao($dados) {
	global $db;
	if($dados['dimid']) {
		$sql = "SELECT ardid as codigo, arddsc as descricao FROM cte.areadimensao WHERE dimid='".$dados['dimid']."'";
	} else {
		$sql = array();
	}
	$db->monta_combo('ardid', $sql, 'S', 'Selecione', 'selecionarArea', '', '', '400', 'S', 'ardid');
}

function selecionarArea($dados) {
	global $db;
	if($dados['ardid']) {
		$sql = "SELECT indid as codigo, inddsc as descricao FROM cte.indicador WHERE ardid='".$dados['ardid']."'";
	} else {
		$sql = array();
	}
	$db->monta_combo('indid', $sql, 'S', 'Selecione', 'selecionarIndicador', '', '', '400', 'S', 'indid');
}

function selecionarIndicador($dados) {
	global $db;
	
	if($dados['indid']) {
		$sql = "SELECT pa.ppaid as codigo, pa.ppadsc || ' - Pontuação:' || c.ctrpontuacao as descricao FROM cte.proposicaoacao pa 
				LEFT JOIN cte.criterio c on c.crtid = pa.crtid
				INNER JOIN cte.indicador i ON i.indid = c.indid AND i.indstatus = 'A' 
				WHERE i.indid='".$dados['indid']."'";
	} else {
		$sql = array();
	}
	
	$db->monta_combo('ppaid', $sql, 'S', 'Selecione', 'selecionarAcao', '', '', '400', 'S', 'ppaid');
}

function selecionarAcao($dados) {
	global $db;
	if($dados['ppaid']) {
		$sql = "SELECT ppaid, ppadsc FROM cte.proposicaoacao WHERE ppaid='".$dados['ppaid']."'";
		$acao = $db->pegaLinha($sql);
	}
	
	echo $acao['ppaid']."||".$acao['ppadsc']."||";
	
	if($dados['ppaid']) {
		$sql = "SELECT ppsid as codigo, ppsdsc as descricao FROM cte.proposicaosubacao WHERE ppaid='".$dados['ppaid']."'";
	} else {
		$sql = array();
	}
	$db->monta_combo('ppsid', $sql, 'S', 'Selecione', 'selecionarSubacao', '', '', '400', 'S', 'ppsid');

	if($dados['ppaid']) {
		$sqlmun = "SELECT m.muncod as codigo ,
						  m.estuf || ' - ' || m.mundescricao as descricao
				   FROM cte.dimensao d
				   INNER JOIN cte.areadimensao 	  ad  ON ad.dimid = d.dimid
				   INNER JOIN cte.indicador 	  i   ON i.ardid  = ad.ardid
				   INNER JOIN cte.criterio		  c   ON c.indid  = i.indid
				   INNER JOIN cte.pontuacao 	  p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
				   INNER JOIN cte.instrumentounidade iu  ON iu.inuid = p.inuid
				   INNER JOIN cte.acaoindicador 	  a   ON a.ptoid   = p.ptoid 
				   INNER JOIN territorios.municipio m   ON m.muncod   = iu.muncod 
				   WHERE a.ppaid='".$dados['ppaid']."' 
				   ORDER BY m.mundescricao";
		
		$_SESSION['indice_sessao_combo_popup']['muncod']['sql'] = $sqlmun;
	}
}

function selecionarSubacao($dados) {
	global $db;
	
	if($dados['ppsid']) {
		$sql = "SELECT * FROM cte.proposicaosubacao WHERE ppsid='".$dados['ppsid']."'";
		$subacao = $db->pegaLinha($sql);
		
		echo $subacao['ppsmetodologia']."||".$subacao['prgid']."||".$subacao['undid']."||".$subacao['frmid']."||".$subacao['indqtdporescola']."||".$subacao['ppsparecerpadrao'];
		
	}

}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include APPRAIZ."includes/cabecalho.inc";
echo'<br>';

monta_titulo( $titulo_modulo, '' );
?>
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>

function selecionarDimensao(dimid) {
	// limpando filhos
	document.getElementById('ardid').value='';
	document.getElementById('ardid').onchange();
	
	$.ajax({
   		type: "POST",
   		url: "cte.php?modulo=principal/cargaSubacao&acao=A",
   		data: "requisicao=selecionarDimensao&dimid="+dimid,
   		async: false,
   		success: function(msg){
   			document.getElementById('tdarea').innerHTML = msg;
   		}
 		});

}

function selecionarArea(ardid) {

	document.getElementById('indid').value='';
	document.getElementById('indid').onchange();
	
	$.ajax({
   		type: "POST",
   		url: "cte.php?modulo=principal/cargaSubacao&acao=A",
   		data: "requisicao=selecionarArea&ardid="+ardid,
   		async: false,
   		success: function(msg){
   			document.getElementById('tdindicador').innerHTML = msg;
   		}
 		});

}

function selecionarIndicador(indid) {

	document.getElementById('ppaid').value='';
	document.getElementById('ppaid').onchange();
	
	$.ajax({
   		type: "POST",
   		url: "cte.php?modulo=principal/cargaSubacao&acao=A",
   		data: "requisicao=selecionarIndicador&indid="+indid,
   		async: false,
   		success: function(msg){
   			document.getElementById('tdacao').innerHTML = msg;
   		}
 		});

}

function selecionarAcao(ppaid) {

	document.getElementById('ppsid').value='';
	document.getElementById('ppsid').onchange();
	
	$.ajax({
   		type: "POST",
   		url: "cte.php?modulo=principal/cargaSubacao&acao=A",
   		data: "requisicao=selecionarAcao&ppaid="+ppaid,
   		async: false,
   		success: function(msg){
   			var dados = msg.split("||");
   			if(ppaid) {
   				document.getElementById('trdadosacao').style.display = '';
   			} else {
   				document.getElementById('trdadosacao').style.display = 'none';
   			}
   			document.getElementById('ppaid').value = dados[0];
   			document.getElementById('acidsc').value = dados[1];
   			document.getElementById('tdsubacao').innerHTML = dados[2];
   		}
 		});

}

function selecionarSubacao(ppsid) {
	
	$.ajax({
   		type: "POST",
   		url: "cte.php?modulo=principal/cargaSubacao&acao=A",
   		data: "requisicao=selecionarSubacao&ppsid="+ppsid,
   		async: false,
   		success: function(msg){
   			var dados = msg.split("||");
   			if(ppsid) {
   				document.getElementById('trdadossubacao').style.display = '';
   			} else {
   				document.getElementById('trdadossubacao').style.display = 'none';
   			}
   			
   			document.getElementById('ppsmetodologia').value   = dados[0];
   			document.getElementById('prgid').value 			  = dados[1];
   			document.getElementById('undid').value 			  = dados[2];
   			document.getElementById('frmid').value 			  = dados[3];
   			if(document.getElementById('indqtdporescola_'+dados[4])) {
   				document.getElementById('indqtdporescola_'+dados[4]).checked = true;
   			}
   			document.getElementById('ppsparecerpadrao').value = dados[5];
   			

   		}
 		});

} 

</script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="formulario" id="formulario">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td class="SubTituloCentro" colspan="2">Localização da subação</td>
</tr>
<tr>
	<td class="SubTituloDireita">Dimensão:</td>
	<td>
	<?
	$sql = "SELECT dimid as codigo, dimdsc as descricao FROM cte.dimensao WHERE dimstatus='A' AND itrid=2";
	$db->monta_combo('dimid', $sql, 'S', 'Selecione', 'selecionarDimensao', '', '', '400', 'S', 'dimid');
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Área:</td>
	<td id="tdarea"><? $db->monta_combo('ardid', array(), 'S', 'Selecione', 'selecionarArea', '', '', '400', 'S', 'ardid'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Indicador:</td>
	<td id="tdindicador"><? $db->monta_combo('indid', array(), 'S', 'Selecione', 'selecionarIndicador', '', '', '400', 'S', 'indid'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Ação:</td>
	<td id="tdacao"><? $db->monta_combo('ppaid', array(), 'S', 'Selecione', 'selecionarAcao', '', '', '400', 'S', 'ppaid'); ?></td>
</tr>
<tr id="trdadosacao" style="display:none;">
	<td colspan="2">
	<input type="hidden" name="ppaid" id="ppaid">
	<table align="center" border="0" width="100%" cellpadding="1" cellspacing="1">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados da ação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição da ação:</td>
		<td><? echo  campo_texto('acidsc', 'S', 'S', 'Descrição da ação', 50, 200, '', '', '', '', 0, 'id="acidsc" readonly="readonly"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome do responsável:</td>
		<td><? echo  campo_texto('acirpns', 'S', 'S', 'Nome do responsável', 50, 200, '', '', '', '', 0, 'id="acirpns"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cargo do responsável:</td>
		<td><? echo  campo_texto('acicrg', 'S', 'S', 'Nome do responsável', 50, 200, '', '', '', '', 0, 'id="acicrg"' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Período inicial:</td>
		<td><? echo campo_data2('acidtinicial','S', 'S', 'Período inicial', 'S' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Período final:</td>
		<td><? echo campo_data2('acidtfinal','S', 'S', 'Período final', 'S' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Resultado esperado:</td>
		<td><? echo campo_textarea( 'acirstd', 'S', 'S', '', '70', '4', '200'); ?></td>
	</tr>

	</table>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita">Subação:</td>
	<td id="tdsubacao"><? $db->monta_combo('ppsid', array(), 'S', 'Selecione', 'selecionarSubacao', '', '', '400', 'S', 'ppsid'); ?></td>
</tr>

<tr id="trdadossubacao" style="display:none;">
	<td colspan="2">
	<table align="center" border="0" width="100%" cellpadding="1" cellspacing="1">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Dados da subação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Categoria de despesa:</td>
		<td>
		<?
		$categoriaArr = array(0 => array('codigo' => '3','descricao' => 'Custeio'),
						      1 => array('codigo' => '4','descricao' => 'Capital'));
		
		$db->monta_combo('sbacategoria', $categoriaArr, 'S', 'Selecione', '', '', '', '400', 'S', 'sbacategoria'); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Forma de atendimento:</td>
		<td>
		<? 
		$sql = "SELECT foaid as codigo, foadsc as descricao FROM cte.formaatendimento";
		$db->monta_combo('foaid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'foaid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Estratégia de implementação:</td>
		<td><? echo campo_textarea('ppsmetodologia', 'S', 'S', '', '70', '4', '200'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Programa:</td>
		<td>
		<? 
		$sql = "SELECT prgid as codigo, prgdsc as descricao FROM cte.programa WHERE prgstatus='A'";
		$db->monta_combo('prgid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'prgid'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Unidade de medida:</td>
		<td>
		<?
		$sql = "SELECT undid as codigo , unddsc as descricao FROM cte.unidademedida WHERE undtipo = 'M' ORDER BY unddsc"; 
		$db->monta_combo('undid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'undid' ); 
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Forma de execução:</td>
		<td>
		<? 
		$sql = "SELECT frmid as codigo, frmdsc as descricao FROM cte.formaexecucao WHERE frmtipo='M' AND frmbrasilpro = FALSE";
		$db->monta_combo('frmid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'frmid' ); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cronograma:</td>
		<td><input type="radio" name="indqtdporescola" id="indqtdporescola_f" value="false"> Global <input type="radio" name="indqtdporescola" id="indqtdporescola_t" value="true"> Por escola</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Parecer físico e financeiro:</td>
		<td><? echo campo_textarea('ppsparecerpadrao', 'S', 'S', '', '70', '4', '200'); ?></td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Classificação IDEB:</td>
	<td>
	<?
	$sqlComboIDEB = "
			select
				tpmid as codigo,
				tpmdsc as descricao
			from territorios.tipomunicipio
			where
				gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) and
				tpmstatus = 'A'
		";
	combo_popup( "ideb", $sqlComboIDEB, "Classificação IDEB", "215x400", 0, array(), "", "S", false, false, 5, 400 );
	?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita">Município:</td>
	<td>
	<?
	$munSql = "SELECT
					tm.muncod AS codigo,
					tm.estuf || ' - ' || tm.mundescricao AS descricao
				FROM 
					territorios.municipio tm 
				WHERE muncod IS NULL
				ORDER BY
					mundescricao";
	
	combo_popup( "muncod", $munSql, "Município", "215x400", 0, array(), "", "S", false, false, 5, 400 );
	?>
	</td>
</tr>

</table>
</form>