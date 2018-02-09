<?
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

include_once APPRAIZ . 'includes/workflow.php';
$db->cria_aba( $abacod_tela, $url, '');

monta_titulo( 'Validação do Plano de Trabalho', 'Pendências');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptridReformulacao"], true);
echo "<br />";

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";

$ptrid = ($_SESSION["emenda"]["ptridReformulacao"]) ? $_SESSION["emenda"]["ptridReformulacao"] : $_REQUEST["ptrid"];

if ( $ptrid ){
	$arPTA = $db->pegaLinha("SELECT resid, refid FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");	
}

validaSessionPTA( $ptrid );

// Recupera ou cria o docid.
$docid = criarDocumento( $ptrid );

$obj = new ValidacaoReformulacaoPTA($ptrid);
$obj->validacaoPlanoTrabalho();

// flag de erros
$existePendencias = false;

//$ptrimprimirpta = $db->pegaUm("SELECT ptrimprimirpta FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");

?>

<script language="JavaScript" src="../../includes/funcoes.js"></script>
	<?php 
		/*if( $ptrimprimirpta != 'f' ){
			//echo abrePTAconsolidado($ptrid, '', 'convenio');
		}*/ 
	?>
<table align="center" class="Tabela" cellpadding="10" cellspacing="1">
 <tbody>
	<tr>
		<td width="10%" style="text-align: left;" class="SubTituloEsquerda">Pendências existentes no Plano de Trabalho</td>
		<td width="90%" style="text-align: center;" class="SubTituloEsquerda">Enviar PTA</td>
	</tr>
	<tr>
		<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<br />
			<table style="border-color:#ffffff;" align="center" class="Tabela" cellpadding="10" cellspacing="0">
			 <tbody>
			 	<tr>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita" colspan="2">
						<b><font size="2">Legenda:</font></b>
						<br />
						<img src="../imagens/valida2.gif" /> Existem pendências nos dados informados na aba.
						<br />
						<img src="../imagens/valida1.gif" /> Dados validados e em situação correta.
					</td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
						ABA
					</td>
					<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
						PENDÊNCIAS
					</td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Iniciativas:</b>
						<?= ((!empty($obj->retorno["iniciativas"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>	
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["iniciativas"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["iniciativas"]); $i++) {
								echo '- ' . $obj->retorno["iniciativas"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Cronograma de Execução e Desembolso:</b>
						<?= ((!empty($obj->retorno["cronograma"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["cronograma"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["cronograma"]); $i++) {
								echo '- ' . $obj->retorno["cronograma"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b>Cadastro de Escolas Beneficiadas:</b>
						<?= ((!empty($obj->retorno["escolasbeneficiadas"])) ? '<img src="../imagens/valida2.gif" />' : '<img src="../imagens/valida1.gif" />') ?>
					</td>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<?
						if(!empty($obj->retorno["escolasbeneficiadas"])) {
							$existePendencias = true;
							
							for($i=0; $i<count($obj->retorno["escolasbeneficiadas"]); $i++) {
								echo '- ' . $obj->retorno["escolasbeneficiadas"][$i] . '<br />';
							}
						} else {
							echo '- ' . "Não existem pendências.";
						}
					?>
					</td>
				</tr>
			 </tbody>
			</table>
		</td>
		<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
		<?
//			if($existePendencias) {
//				echo "<font color='red'>Não é possível enviar seu Plano de Trabalho devido às pendências listadas ao lado.</font>";
//			}
//			else if(pegarEstadoAtual( $ptrid ) != EM_ELABORACAO) {
//				echo "<font color='red'>O seu Plano de Trabalho já foi enviado para análise.</font>";
//			}
//			else if( $docid ) {
//				// barra de estado atual,ações e historico
//				wf_desenhaBarraNavegacao( $docid , array( 'ptrid' => $ptrid, 'resid' => $resid , 'obj' => $obj ) );
//			}
			
			wf_desenhaBarraNavegacao( $docid , array( 'ptrid' => $ptrid, 'resid' => $arPTA['resid'] , 'url' => $_SESSION['favurl'], 'refid' => $arPTA['refid'], 'tipo' => 'popup' ) );
		?>
		</td>
	</tr>
 </tbody>
</table>