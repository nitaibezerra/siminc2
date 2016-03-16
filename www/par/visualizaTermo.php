<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/par/_funcoesPar.php";
include APPRAIZ . "www/par/_constantes.php";

$db = new cls_banco();

if( $_REQUEST['dopid'] ){
	$dopid = $_REQUEST['dopid'];
} elseif( $_REQUEST['dopnumerodocumento'] ){
	$sql = "SELECT dopid FROM par.vm_documentopar_ativos WHERE dopnumerodocumento = ".$_REQUEST['dopnumerodocumento'];
	$dopid = $db->pegaUm($sql);
}

if( !$dopid ){
	echo "
		<script>
			alert('Termo inválido.');
			window.close();
		</script>";
	die();
}

$sql = "select * from (
			SELECT dopusucpfvalidacaogestor as cpfgestor, 
						to_char(dopdatavalidacaogestor, 'DD/MM/YYYY HH24:MI:SS') as data, 
						us.usunome, us.usucpf, d.tpdcod, itrid, dopid
					FROM par.vm_documentopar_ativos  dp
					INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
					INNER JOIN par.processopar pp ON pp.prpid = dp.prpid and pp.prpstatus = 'A'
					INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
					left join seguranca.usuario us on us.usucpf = dopusucpfvalidacaogestor
			union 
			SELECT dopusucpfvalidacaogestor as cpfgestor, 
						to_char(dopdatavalidacaogestor, 'DD/MM/YYYY HH24:MI:SS') as data, 
						us.usunome, us.usucpf, d.tpdcod, itrid, dopid
					FROM par.vm_documentopar_ativos  dp
					INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
					INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
					INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
					left join seguranca.usuario us on us.usucpf = dopusucpfvalidacaogestor
			) as foo
		 WHERE dopid = ".$dopid; 

$html = $db->pegaLinha($sql);


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
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>
	<?
	$cabecalhoBrasao .= "<table width=\"95%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" >";
$cabecalhoBrasao .= "<tr>" .
				"<td colspan=\"100\">" .($html['tpdcod'] == '101' ? monta_cabecalho_relatorio_par('29/11/2011', '100') : monta_cabecalho_relatorio_par('', '100') ).
				"</td>" .
			  "</tr>
			  </table>";
					echo $cabecalhoBrasao;
	?>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td style="font-size: 12px; font-family:arial;">
			<div>
			<?php 
				echo html_entity_decode ( pegaTermoCompromissoArquivo($dopid, '') );
			?>
			</div>
		</td>
	</tr>
</table>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr style="text-align: center;">
		<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
			<b>Validado pelo <?echo ($html['itrid'] == 1 ? 'secretário(a) de educação' : 'prefeito') ?> <?=$html['usunome'] ?> - CPF: <?=formatar_cpf($html['usucpf']) ?> em <?=$html['data']; ?> </b>
		</td>
	</tr>
</table>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubtituloDireita, div_rolagem" style="text-align: center;">
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>