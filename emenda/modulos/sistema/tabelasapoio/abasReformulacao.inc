<?php

if( $_POST['requisicao'] == 'salvar' ){
	
	if( is_array($_POST['menu']) ){
		$db->executar("DELETE FROM emenda.abasreformulacao WHERE trefid = {$_POST['trefid']} and abacod = {$_POST['abacod']}");
		foreach ($_POST['menu'] as $menu) {
			$sql = "INSERT INTO emenda.abasreformulacao(trefid, abacod, mnuid) 
					VALUES ({$_POST['trefid']}, {$_POST['abacod']}, $menu)";
			
			$db->executar($sql);
		}
		$db->commit();
	}
	$db->sucesso( 'sistema/tabelasapoio/abasReformulacao', '' );
	exit();
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, $parametros );

monta_titulo( 'Gerenciar Abas Reformulação', '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.14.custom.min.js"></script>
<link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
<style>
.button{
	width: 120px; 
	height: 25px;
}
</style>
<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center" border="0">
		<tr>
			<td class="subtitulodireita" width="25%"><b>Reformulação:</b></td>
			<td><?
			$sql = "select 
						trefid as codigo,
						trefnome as descricao
					from emenda.tiporeformulacao 
					where trefstatus = 'A'
					order by trefnome";
			
			$trefid = $_REQUEST['trefid'];
			echo $db->monta_combo('trefid', $sql, 'S', 'Selecione...', 'carregarAbas', '', '', 350, '', 'trefid', false ); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="25%"><b>Abas:</b></td>
			<td><?
			$sql = "select 
						a.abacod as codigo,
						a.abadsc as descricao
					from seguranca.aba a 
					where a.sisid = 57
					order by a.abadsc";
			
			$abacod = $_REQUEST['abacod'];
			echo $db->monta_combo('abacod', $sql, 'S', 'Selecione...', 'carregarAbas', '', '', 350, '', 'abacod', false ); ?></td>
		</tr>
	<?if( $abacod && $trefid ){ ?>
		<tr>
			<td class="subtitulodireita" width="25%"><b>Menu:</b></td>
			<td>
			<!--  <input type="checkbox" name="menu[]" id="menu" value=""> -->
			<?
			print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width: 100%">';			
			print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b>Selecione as abas que não deverão aparecer</b></td></tr></table>';
			
			$sql = "SELECT
						case when (select count(*) from emenda.abasreformulacao where mnuid = m.mnuid and abacod = a.abacod and trefid = $trefid) > 0 then 
					    	'<center><input type=\"checkbox\" checked=\"checked\" name=\"menu[]\" id=\"menu\" value=\"'||m.mnuid||'\"></center>'
					    else 
					    	'<center><input type=\"checkbox\" name=\"menu[]\" id=\"menu\" value=\"'||m.mnuid||'\"></center>'
					    end  as acoes,
						m.mnucod as codigo, 
					    m.mnudsc as descricao,
					    m.mnulink
					FROM seguranca.aba a
						inner join seguranca.aba_menu am on am.abacod = a.abacod 
					    inner join seguranca.menu m on m.mnuid = am.mnuid
					WHERE 
						a.abacod = '$abacod'
						and a.sisid = 57
					    and m.mnustatus = 'A'
					ORDER BY
						m.mnucod";
			
			$cabecalho = array( 'Ação', "Codigo", "Descrição", "Link" );
			$db->monta_lista_simples($sql, $cabecalho, 100, 30, 'N', '100%', '', '', '', '', true);
	 	?></td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="3" align="center">
				<input type="button" value="Salvar" class="button" name="btnSalvar" id="btnSalvar" onclick="salvaAbas();" style="cursor: pointer;">
			</td>
		</tr>
	<?} ?>
	</table>
</form>
<script type="text/javascript">

function carregarAbas(valor){
	if( $('[name="trefid"]').val() != '' && $('[name="abacod"]').val() != '' ){
		$('#formulario').submit();
	}
}

function salvaAbas(){

	if($('[name="trefid"]').val() == ''){
		alert('Informe a Reformulação');
		$('[name="trefid"]').focus();
		return false;
	}
	if($('[name="abacod"]').val() == ''){
		alert('Informe a Abas');
		$('[name="abacod"]').focus();
		return false;
	}
	if( $('[name="menu[]"]:checked').length == '0'){
		alert('Selecione um Menu.');
		return false;
	}
	$('#requisicao').val('salvar');
	$('#formulario').submit();
}
</script>