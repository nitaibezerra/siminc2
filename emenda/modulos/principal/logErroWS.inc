<?
set_time_limit(30000);
ini_set("memory_limit", "3000M");
require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( "Log WS FNDE", '<b>Filtros de Pesquisa</b>');
?>
<form id="formulario" name="formulario" method="post" action="">
<table class="tabela" width="95%" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="middle" style="width: 30%;"><b>PTA:</b></td>
		<td><? echo campo_texto('ptrcod', 'N', 'S', '', 11, 20, '[#]', '', '', '', 0, 'id="ptrcod"', '', $_POST['ptrcod'] ); ?></td>		
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="middle" style="width: 30%;"><b>Tipo Log:</b></td>
		<td><?
			$arTipo = array(
				array("codigo" => 'H', "descricao" => 'Habilita'),
				array("codigo" => 'P', "descricao" => 'Processo'),
				array("codigo" => 'CC', "descricao" => 'Conta Corrente'),
				array("codigo" => 'CO', "descricao" => 'Convenio'),
				array("codigo" => 'E1', "descricao" => 'Empenho Original'),
				array("codigo" => 'E2', "descricao" => 'Empenho Reforço'),
				array("codigo" => 'E3', "descricao" => 'Empenho Anulação'),
				array("codigo" => 'E4', "descricao" => 'Cancelar Convênio RAP'),
				array("codigo" => 'EC', "descricao" => 'Empenho Consulta'),
				array("codigo" => 'PG', "descricao" => 'Pagamento Solicita'),
				array("codigo" => 'PC', "descricao" => 'Pagamento Consulta'),
				array("codigo" => 'PX', "descricao" => 'Pagamento Cancela'),
				array("codigo" => 'EF', "descricao" => 'Entidade FNDE'),
				array("codigo" => 'ER', "descricao" => 'Liquida Convênio RAP'),
			);
			$logtipo = $_REQUEST['logtipo'];
		 $db->monta_combo("logtipo", $arTipo, "S", "Selecione", '', '', '', '350', 'N','logtipo'); ?></td>		
	</tr>
	<tr>
		<td colspan="2" style="text-align: center" class="SubTituloDireita"><input type="button" name="btnPesquisa" value="Pesquisar" onclick="document.getElementById('formulario').submit();"></td>		
	</tr>
	<tr>
		<td colspan="2">&nbsp;</td>
	</tr>
	<tr>
		<td colspan="2">
		<?
		$filtro = array();
		if( $_REQUEST['ptrcod'] || $_REQUEST['logtipo'] ){
			if( $_REQUEST['ptrcod'] )
				$ptrid = $db->pegaUm( "select ptrid from emenda.planotrabalho where ptrstatus = 'A' and ptrcod = ".$_REQUEST['ptrcod']." order by ptrid desc" );
				
			if( !empty($ptrid) ) array_push( $filtro, 'lo.ptrid = '.$ptrid );
			if( !empty($_REQUEST['logtipo']) ) array_push( $filtro, "lo.logtipo = '".$_REQUEST['logtipo']."'" );
			
			$sql = "SELECT 
					  lo.logid,
					  lo.ptrid,
					  lo.exfid,
					  lo.cocid,
					  lo.logtipo,
					  lo.logerro,
					  lo.usucpf,
                      us.usunome,
					  lo.logdatainclusao,
					  logenvio,
					  logresposta
					FROM 
					  emenda.logerrows lo
                      inner join seguranca.usuario us on us.usucpf = lo.usucpf 
                    ".( $filtro ? 'WHERE '.implode( ' and ', $filtro ) : '' )." order by logdatainclusao desc";
			
			$ar = $db->carregar($sql);
			$ar = $ar ? $ar : array();
			$html = '<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">';
			foreach($ar as $dados){
				$html.= '<tr><th colspan="2">DEBUG INICIO</th></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>ptrid:</b></td><td>'.$dados['ptrid'].'</td></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>exfid:</b></td><td>'.$dados['exfid'].'</td></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>cocid:</b></td><td>'.$dados['cocid'].'</td></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>logtipo:</b></td><td>'.$dados['logtipo'].'</td></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>usucpf:</b></td><td>'.$dados['usucpf'].'</td></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>usunome:</b></td><td>'.$dados['usunome'].'</td></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>logdatainclusao:</b></td><td>'.$dados['logdatainclusao'].'</td></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>Log Envia:</b></td><td>
							<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
							<tr><td>
							<pre>'.simec_htmlentities($dados['logenvio']).'</td></tr></table></td></tr>';
				$html.= '<tr><td class="subtitulodireita"><b>Log Resposta:</b></td><td><pre>'.($dados['logerro'] ? formatarXML($dados['logerro']) : formatarXML($dados['logresposta'])).'</td></tr>';
				$html.= '<tr><th colspan="2">DEBUG FIM</th></tr>';				
				$html.= '<tr><td colspan="2" height="5px"></td></tr>';				
			}
			$html.="</table>";
		}
		echo $html;
		?>
		</td>
	</tr>
</table>
</form>
<?php

function formatarXML( $xml ){
	
	$xml = str_ireplace( '><', '>¬<', $xml );
	//$xml = str_ireplace( '> <', '>¬<', $xml );
	$arrXML = explode( '¬', $xml );
	$html = '<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">';
	foreach ($arrXML as $xml) {
		$html.= '<tr><td>'.simec_htmlentities($xml).'</td></tr>';
	}
	$html.= '</table>';
	return $html;
}
/*
$ar = $ar ? $ar : array();
	if( $ar ){
		echo "----- ----- ----- ----- ----- ----- ----- ----- ----- DEBUG INICIO ----- ----- ----- ----- ----- ----- ----- ----- ----- --<br />";
		foreach($ar as $dados){
			echo "<pre>";
			echo '<b>Logid:</b> '.$dados['logid'].'<br>';
			echo '<b>Ptrid:</b> '.$dados['ptrid'].'<br>';
			echo '<b>exfid:</b> '.$dados['exfid'].'<br>';
			echo '<b>cocid:</b> '.$dados['cocid'].'<br>';
			echo '<b>logtipo:</b> '.$dados['logtipo'].'<br><br>';
			echo '<b>logerro:</b> '.simec_htmlentities($dados['logerro']).'<br><br>';
			print_r($dados);
		}
		echo "</pre>";
			echo "----- ----- ----- ----- ----- ----- ----- ----- ----- DEBUG FIM ----- ----- ----- ----- ----- ----- ----- ----- ----- --<br />";
	} else {
		echo "<pre>";
		echo '<b>Não encontrou registro!</b>';
		echo "</pre>";
	}
* */

?>