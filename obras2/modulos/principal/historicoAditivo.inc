<?php
switch ( $_REQUEST['op'] ){
	case 'downloadArquivo':
		$arqid = $_GET['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file 		  = new FilesSimec(null, null, "obras2");
			$file->getDownloadArquivo($arqid);
		}
		die('<script>
				location.href=\'?modulo=principal/cadContrato&acao=E\';
			 </script>');
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";


if ( $_GET['acao'] != 'E' ){
	$crtid = $_SESSION['obras2']['crtid'];
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
	
	$arAba = getArAba('cadastrocontratoedicao');
	echo montarAbasArray($arAba, "?modulo=principal/historicoAditivo&acao=A");
}else {
	$crtid = $_GET['crtid'];
?> 
<html>
    <head>
        <title>Histórico de Aditivos</title>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
<body marginheight="0" marginwidth="0">
<?php
}

monta_titulo('Histórico de Aditivos', '');

$contrato = new Contrato();
$dadoHist = $contrato->listaHistoricoByContratoPai( $crtid );
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
		<center>
<?php
$i=0;
foreach ( $dadoHist as $dado ){
//	extract( $dado );
	if ( $dado['ttaid'] ){
		$i++;
		$titulo = 'Aditivo ' . $i;
	}else{
		$titulo = 'Contrato Original';
	}
?>
		<fieldset style="background-color: #FFFFFF;">
			<legend>
				<img src="/imagens/mais.gif" onclick="abreHistorico(<?=$dado['crtid']?>, true)" id="img_<?=$dado['crtid']?>" border="0" style="cursor:pointer;">
				<?=$titulo ?>
			</legend>
			<div id="conteudo_<?=$dado['crtid']?>" style="display:none;">

				<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
				<tr>
					<td width="265" class="subtitulodireita">Órgão:</td>
					<td>
						<?php 
						$orgao = new Orgao();
						echo $orgao->pegaDescricao( $dado['orgid'] );
						?>
					</td>
				</tr>
				<?php 
				$crthabil = 'S';
				if ( $dado['ttaid'] ):
					$crthabil = 'N';
				?>
				<tr>
					<td width="265" class="subtitulodireita">Tipo de Aditivo:</td>
					<td>
						<?php 
						$tipoTermo = new TipoTermoAditivo( $dado['ttaid'] );
						echo $tipoTermo->ttadsc;
						?>
					</td>
				</tr>
				<tr>
					<td width="265" class="subtitulodireita">Denominação:</td>
					<td>
						<?php 
						echo $dado['crtdenominacao'];
						?>
					</td>
				</tr>
				<tr>
					<td width="265" class="subtitulodireita">Data de Assinatura do Aditivo:</td>
					<td>
						<?php 
						echo formata_data( $dado['crtdtassinaturaaditivo'] );
						?>
					</td>
				</tr>
				<tr>
					<td width="265" class="subtitulodireita">Aditivo de Supressão:</td>
					<td>
						<?php 
						echo ($dado['crtsupressao'] == 't' ? 'Sim' : 'Não');
						?>
					</td>
				</tr>
				<tr>
					<td width="265" class="subtitulodireita">Justificativa:</td>
					<td>
						<?php 
						echo nl2br( $dado['crtjustificativa'] );
						?>
					</td>
				</tr>
				<?php 
				endif;
				?>
				<tr>
					<td width="265" class="subtitulodireita">Licitação:</td>
					<td>
						<?php 
							$licitacao = new Licitacao();
							echo $licitacao->pegaDescricaoPorLicid( $dado['licid'] );
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Empresa Contratada</td>
					<td>
					  <?php
						$empresa 		= new Entidade( $dado['entidempresa'] );
						$entnomeempresa = $empresa->entnome;
						$entidempresa 	= $empresa->getPrimaryKey();
					  ?>
					  <span id="entnomeempresa"><?php echo $entnomeempresa; ?></span>
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita">Data de Assinatura do Contrato:</td>
					<td><?=formata_data( $dado['crtdtassinatura'] ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Prazo de Vigência do Contrato (dias):</td>
					<td><?=$dado['crtprazovigencia']; ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Data de término do contrato:</td>
					<td><?=formata_data( $dado['crtdttermino'] ); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Valor do Contrato(R$):</td>
					<td><?=number_format($dado['crtvalorexecucao'], 2, ',', '.'); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Percentual BDI:</td>
					<td><?=number_format($dado['crtpercentualdbi'], 2, ',', '.'); ?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
				</tr>
                <tr>
                    <td class="SubTituloDireita">Data Inserção:</td>
                    <td><?=formata_data( $dado['dt_cadastro'] ); ?></td>
                </tr>
				<?php
				if ( $dado['arqid'] ): 
					$arquivo = new Arquivo( $dado['arqid'] );
				?>
				<tr>
					<td class="SubTituloDireita">Anexo:</td>
					<td>
						<a href="?modulo=principal/historicoAditivo&acao=A&op=downloadArquivo&arqid=<?php echo $dado['arqid'] ?>">
							&nbsp;&nbsp;
							<img src="/imagens/anexo.gif">
							<?php echo $arquivo->arqnome . '.' . $arquivo->arqextensao ?>
						</a>
					</td>
				</tr>
				<?php
				endif;
				?>
				<tr bgcolor="#FFFFFF" id="tr_obra_contrato">
					<td colspan="2" valign="top" id="td_obra_contrato">
						<?php
						if ( $dado['crtid'] ):
							$obraContrato 	   = new ObrasContrato();
							$dadosObraContrato = $obraContrato->listaByContrato( $dado['crtid'] );
							foreach ( $dadosObraContrato as $dados ):
//								$umdid 			  		= $dados['umdid']; 
//								$ocrqtdconstrucao 		= number_format( $dados['ocrqtdconstrucao'], 2, ',', '.'); 
//								$ocrdtordemservico 		= formata_data( $dados['ocrdtordemservico'] ); 
//								$ocrdtinicioexecucao 	= formata_data( $dados['ocrdtinicioexecucao'] ); 
//								$ocrprazoexecucao 		= $dados['ocrprazoexecucao']; 
//								$ocrdtterminoexecucao 	= formata_data(  $dados['ocrdtterminoexecucao'] ); 
//								$ocrvalorexecucao 		= number_format( $dados['ocrvalorexecucao'], 2, ',', '.'); 
//								$ocrcustounitario 		= number_format( $dados['ocrcustounitario'], 2, ',', '.'); 
//								$ocrpercentualdbi 		= number_format( $dados['ocrpercentualdbi'], 2, ',', '.'); 
								$obrid 					= $dados['obrid']; 
								$obrnome 				= $dados['obrnome'];
						?>
						<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" id="table_obra_<?php echo $obrid ?>" style="margin-bottom: 10px;">
							<tr>
								<td class="subtitulodireita" width="20%">
									Obra:
									<input name="obrid[]" id="obrid" value="<?php echo $obrid ?>" type="hidden">
								</td>
								<td id="txt_obra" width="80%" colspan="3">
									<?php echo $obrid . ' - ' . $obrnome ?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita" width="20%">Data da Ordem de Serviço:</td>
								<td width="30%"><?=formata_data( $dados['ocrdtordemservico'] ); ?></td>
								<td class="SubTituloDireita" width="20%">Valor Contratado da Obra (R$):</td>
								<td width="30%"><?=number_format($dados['ocrvalorexecucao'], 2, ',', '.'); ?></td>
							</tr>
							<tr>
								<td class="subtitulodireita">Início de Execução da Obra:</td>
								<td><?=formata_data( $dados['ocrdtinicioexecucao'] ); ?></td>
								<td class="SubTituloDireita">Área/Quantidade a ser Construída:</td>
								<td>
									<?php
									$unidadeMedida = new UnidadeMedida( $dados['umdid'] );
									echo number_format($dados['ocrqtdconstrucao'], 2, ',', '.'); 
									echo '&nbsp;&nbsp;Unidade de Medida:&nbsp;' . $unidadeMedida->umdeesc;
									?>
								</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Prazo de Execução (dias):</td>
								<td><?=$dados['ocrprazoexecucao']; ?></td>
								<td class="SubTituloDireita">Custo Unitário R$:</td>
								<td><?=number_format($dados['ocrcustounitario'], 2, ',', '.'); ?> (R$ / Unidade de Medida)</td>
							</tr>
							<tr>
								<td class="SubTituloDireita">Término de Execução da Obra:</td>
								<td><?=formata_data( $dados['ocrdtterminoexecucao'] ); ?></td>
								<td class="SubTituloDireita">Percentual BDI:</td>
								<td><?=number_format($dados['ocrpercentualdbi'], 2, ',', '.'); ?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
							</tr>
						</table>				
						<?php
							endforeach;
						endif;
						unset( $umdid, $ocrqtdconstrucao, $ocrdtordemservico, $ocrdtinicioexecucao, $ocrprazoexecucao, $ocrdtterminoexecucao, $ocrvalorexecucao, $ocrcustounitario, $ocrpercentualdbi, $obrid, $obrnome );
						?>				
					</td>
				</tr>
			</table>

			</div>
		</fieldset>
<?php	
}
//dbg($dadoHist, d);
?>
		</center>
		</td>
	</tr>
</table>

<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function abreHistorico( crtid, exibir ){

	if ( exibir == true ){
		$('#conteudo_' + crtid).show();
		$('#img_' + crtid).attr('src', '/imagens/menos.gif')
						  .attr('onclick', '')
						  .unbind('click')
						  .click(function () {abreHistorico(crtid, false)} );
	}else{
		$('#conteudo_' + crtid).hide();
		$('#img_' + crtid).attr('src', '/imagens/mais.gif')
						  .attr('onclick', '')
						  .unbind('click')
						  .click(function (){ abreHistorico(crtid, true) });
	}
}
//-->
</script>
<?php 
if ( $_GET['acao'] == 'E' ){
?>
</body>
</html>
<?php	
}
?>
