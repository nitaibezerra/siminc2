<?php

if( $_POST['requisicao'] == 'salvar' ){
	if( $_POST['vizualiza'] ){
		foreach ($_POST['vizualiza'] as $mdoid => $mdovisivel) {
			$sql = "UPDATE par.modelosdocumentos SET mdovisivel = '{$mdovisivel}' WHERE mdoid = $mdoid";
			$db->executar( $sql );
		}		
	}
	$db->commit();
	$db->sucesso('principal/admVisualizaDocumento');
	exit();
}

require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// Monta as abas e o título
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Visualização de Documentos', '');
?>
<form id="formulario" action="" method="post">
<input type="hidden" name="requisicao" id="requisicao" value=""/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="subtitulocentro">Modelos de Documento Cadastrados</td>
	</tr>
	<tr bgcolor="#c0c0c0">
		<td>
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarVisualizacao();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>

<table id="tblform" class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<tr>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ações</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Código</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo de Documento</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome do Modelo</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data de Inclusão</strong></td>
			<td align="Center" class="title" width="10%"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inserido Por</strong></td>
		</tr>
	</thead>
<?php

	$sql = "SELECT
				mdoid as codigo, 
				tpddsc as tipo,
				mdonome,
				to_char(mdodatainclusao,'DD/MM/YYYY') as data,
				usunome,
				em.mdovisivel
			FROM
				par.modelosdocumentos em
			INNER JOIN
				public.tipodocumento tp ON tp.tpdcod = em.tpdcod
			INNER JOIN
				seguranca.usuario su ON su.usucpf = em.usucpf
			WHERE
				mdostatus = 'A'
			ORDER BY
				mdonome, mdoid";
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	$html = '';
	foreach ($arrDados as $key => $v) {
		$key % 2 ? $cor = "#F7F7F7" : $cor = "";
		
		$checkedS = '';
		$checked = '';
		if( $v['mdovisivel'] == 'S' ){
			$checkedS = 'checked="checked"';
		} elseif( $v['mdovisivel'] == 'N' ) {
			$checked = 'checked="checked"';			
		} else {
			$checked = 'checked="checked"';
		}
		
		$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td align="left" title="Ações"><input class="libera" type="radio" name="vizualiza['.$v['codigo'].']" id="vizualiza_'.$v['codigo'].'" '.$checkedS.' value="S"> Sim 
													<input class="libera" type="radio" name="vizualiza['.$v['codigo'].']" id="vizualiza_'.$v['codigo'].'" '.$checked.' value="N"> Não</td>
					<td align="right" style="color:#0066cc;" title="Código">'.$v['codigo'].'</td>
					<td align="left" title="Tipo de Documento">'.$v['tipo'].'</td>
					<td align="left" title="Nome do Modelo">'.$v['mdonome'].'</td>
					<td align="left" title="Ações">'.$v['data'].'</td>
					<td align="left" title="Ações">'.$v['usunome'].'</td>
				'; 
	}
	echo $html;
	print '<table class="listagem" cellspacing="0" cellpadding="2" border="0" align="center" width="95%">
			<tbody>
			<tr bgcolor="#ffffff">
				<td><b>Total de Registros: '.sizeof($arrDados).'</b></td>
			</tr>
			</tbody>
		  </table>';
	$cabecalho = array( "Ações", "Código", "Tipo de Documento", "Nome do Modelo", "Data de Inclusão", "Inserido Por" );
	//$db->monta_lista( $sql, $cabecalho, 100, 10, 'N','center', '', '', '', '' );

?>
</table>
</form>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function salvarVisualizacao(){

	var total = $('input.libera:checked').length;
	
	if( total == 0 ){
		alert('Selecione uma ação');
		return false;
	}
	$('#requisicao').val('salvar');
	$('#formulario').submit();
}
</script>

