<?php
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_SESSION["emenda"]["ptridAnalise"] ) ? $_SESSION["emenda"]["ptridAnalise"] : $_SESSION["emenda"]["ptrid"];

$ptrid = $_SESSION["emenda"]["ptridAnalise"];

validaSessionPTA( $ptrid );

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $ptrid );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $ptrid );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
// Cria o título da tela
monta_titulo( 'Gerenciamento de Fluxos do Workflow', "");

echo cabecalhoPlanoTrabalho($ptrid, true);

montaVisualizarPTA( $ptrid, false, $boReformulacao );

$arDados = carregaDadosEmpenhoPagamento( $ptrid );

$perfil = pegaPerfilArray( $_SESSION['usucpf'] );

// flag de erros
$existePendencias = false;
// Recupera ou cria o docid.
$docid = criarDocumento( $ptrid );
?>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<table align="center" class="Tabela" cellpadding="10" cellspacing="1">
 <tbody>
	<tr>
		<td width="10%" style="text-align: left;" class="SubTituloEsquerda">Pendências existentes para alterar o estado do workflow</td>
		<td width="90%" style="text-align: center;" class="SubTituloEsquerda">Alterar Estado</td>
	</tr>
	<tr>
		<td width="100%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<br />
			<table style="border-color:#ffffff;" align="center" class="Tabela" cellpadding="10" cellspacing="0">
			 <tbody>
			 	<tr>
					<td width="80%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita" colspan="5">
						<b><font size="2">Legenda:</font></b>
						<br />
						<img src="../imagens/valida2.gif" /> Existem pendências nos dados informados para envio.
						<br />
						<img src="../imagens/valida1.gif" /> Dados validados e em situação correta de envio.
					</td>
				</tr>
				<tr>
					<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
						De
					</td>
					<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
						Para
					</td>
					<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
						Descrição
					</td>
					<?if( $db->testa_superuser() ){ ?>
						<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
							PreAção
						</td>
						<td width="100" style="border:solid 1px #ffffff; text-align: center;" class="SubTituloEsquerda">
							PosAção
						</td>
					<?} ?>
				</tr>
				<?
				$sql = "SELECT 
							aedid, aeddscrealizar, esdiddestino, aedobs, aedposacao, aedcondicao
						FROM 
							workflow.acaoestadodoc 
						WHERE 
							esdidorigem = $estadoAtual
						    and aedstatus = 'A'
						    and aedvisivel = 't'";
				
				$arAcoes = $db->carregar( $sql );
				$arAcoes = ($arAcoes ? $arAcoes : array());
				
				$obFluxos = new pendeciaFluxosWorkflow( $db, $ptrid );
				
				foreach ($arAcoes as $key => $v) {				
				?>
				<tr>
					<?if( $key == 0 ){ ?>
					<td width="10%" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda" rowspan="<?=sizeof( $arAcoes ); ?>">
						<b><?echo $db->pegaUm($sql = "select esddsc from workflow.estadodocumento where esdid = ".$estadoAtual);?></b>
					</td>
					<?} ?>
					<td width="20%" style="border:solid 1px #ffffff; text-align: right;" class="SubTituloEsquerda">
						<b><?=$v['aeddscrealizar']; ?></b>
					</td>					
					<td width="50%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
					<? 
					$msg = $obFluxos->validaIdentificacaoProcesso( $v['esdiddestino'] ); //($v['aedobs'] ? $v['aedobs'] : "Não existem pendências.");
					$retorno = ( empty( $msg ) ? '<img src="../imagens/valida1.gif" /> Não existem pendências' : '<img src="../imagens/valida2.gif" /> '.implode( '<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;', $msg) );
					echo $retorno;					
					?>
					</td>
					<?if( $db->testa_superuser() ){ ?>
						<td width="10%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
						<?echo ($v['aedcondicao'] ? $v['aedcondicao'] : "&nbsp;");?>
						</td>
						<td width="10%" style="border:solid 1px #ffffff; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
						<?echo ($v['aedposacao'] ? $v['aedposacao'] : "&nbsp;");?>
						</td>
					<?} ?>
				</tr>
				<?} ?>
			 </tbody>
			</table>
		</td>
		<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
		<?
			$boHistorico = null;
			if( in_array( INSTITUICAO_BENEFICIADA, pegaPerfilArray( $_SESSION['usucpf'] ) ) ){
				$boHistorico = array('historico' => true);
			}
			
			wf_desenhaBarraNavegacao( $docid , array( 'ptrid' => $ptrid, 'resid' => $arPTA['resid'] , 'url' => $_SESSION['favurl'] ), $boHistorico );
		?>
		</td>
	</tr>
 </tbody>
</table>