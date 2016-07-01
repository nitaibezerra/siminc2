<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<?php
header('content-type: text/html; charset=ISO-8859-1');
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/par/_funcoes.php";
include APPRAIZ . "www/par/_funcoesPar.php";
include APPRAIZ . "www/par/_constantes.php";

$db = new cls_banco();
$terid = $_REQUEST['terid'];

$sql = "SELECT
			to_char(terdataassinatura, 'DD/MM/YYYY') as terdataassinatura,
			usucpfassinatura,
			terassinado, proid
		FROM
			par.termocompromissopac 
		WHERE 
			terid = ".$terid;
$dados = $db->pegaLinha( $sql );

$terdocumento = pegaTermoCompromissoArquivo( '', $terid );

if( empty($terdocumento) ){
	$html = retornaHTMLTermo( $terid );
	
	gravaHtmlDocumento($html, $terid, $dados['proid'], 'PAC');
} else {
	$html = $terdocumento;
}

$sql = "SELECT usunome FROM seguranca.usuario WHERE usucpf = '".$dados['usucpfassinatura']."'";
$nome = $db->pegaUm( $sql );

if( $_REQUEST['muncod'] ){
	if( $nome == '' ){
		$nome = 'manualmente';
	} else {
		$nome = 'pelo(a) Prefeito(a) '.$nome.' - CPF: '.formatar_cpf($dados['usucpfassinatura']).' em '.$dados['terdataassinatura'];
	}
} else {
	if( $nome == '' ){
		$nome = 'manualmente';
	} else {
		$nome = 'Secretário(a) de Educação '.$nome.' - CPF: '.formatar_cpf($dados['usucpfassinatura']).' em '.$dados['terdataassinatura'];
	}
}
//if( $html['tpdcod'] != '102' ) $display = 'style="display: none;"';

function monta_cabecalho_relatorio_par( $data = '', $largura ){
	
	global $db;
	
	$data = $data ? $data : date( 'd/m/Y' );
	
	$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" align="center"><img src="../imagens/brasao.gif" width="45" height="45" border="0">'			
				//.'		<td nowrap align="center" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>'				
//				.'			Acompanhamento da Execução Orçamentária<br/>'					
				.'			FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />'
				.'		</td>'
				//.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'					
				//.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'					
				//.'			Hora da Impressão: '.$data .' - ' . date( 'H:i:s' ) . '<br />'					
				//.'		</td>'	
				.'	</tr>'					
				.'</table><br><br>';					
								
		return $cabecalho;						
						
}
?>
<style>

@media print {.notprint { display: none } .div_rolagem{display: none} }	
@media screen {.notscreen { display: none; }
.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
 
</style>
<table id="termo1" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>
	<?
	$cabecalhoBrasao .= "<table width=\"95%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" >";
$cabecalhoBrasao .= "<tr>" .
				"<td colspan=\"100\">" .(is_array($html) && $html['tpdcod'] == '101' ? monta_cabecalho_relatorio_par('29/11/2011', '100') : monta_cabecalho_relatorio_par('', '100') ).
				"</td>" .
			  "</tr>
			  </table>";
					echo $cabecalhoBrasao;
	?>
<table id="termo" width="95%" align="left" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td style="font-size: 12px; font-family:arial;">
			<div>
			<?php 
				echo html_entity_decode ($html);
			?>
			</div>
		</td>
	</tr>
</table>
<br>
<br>
<br>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr style="text-align: center;">
		<td><div style="display: <?=( empty($dados['usucpfassinatura']) ? 'none' : 'block') ?>"><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO</b><br><br></div>
			<b>Validado <?=$nome ?></b>
		</td>
	</tr>
</table>
<table id="termo2" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>