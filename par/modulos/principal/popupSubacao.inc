<?php 
$anoref = $_GET['anoref'] ? $_GET['anoref'] : date('Y');
$sbaid = $_GET['sbaid'] ? $_GET['sbaid'] : 1; 

if($_POST['quantidade']){

	$sql = "UPDATE par.subacaotemporariacronograma SET
			inicio = '{$_POST['inicio']}',
			fim = '{$_POST['fim']}',
			anotermino = '{$_POST['anotermino']}',
			quantidade = '{$_POST['quantidade']}'
			WHERE subid = {$sbaid} AND anoreferencia = '{$anoref}'";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>document.location.href = 'par.php?modulo=principal/popupSubacao&acao=A&anoref=".$anoref."&sbaid=".$sbaid."';</script>";
}

$obPropostaSubacaoControle = new PropostaSubacaoControle();
$arPropostaSubacao = $obPropostaSubacaoControle->carregaPropostaSubacaoPorSbaid($sbaid);
$arPropostaSubacao = ($arPropostaSubacao) ? $arPropostaSubacao : array();

$obCronogramaControle = new CronogramaControle();
$arCronograma = $obCronogramaControle->carregaCronogramaPorSbaid($sbaid, $anoref);
$arCronograma = ($arCronograma) ? $arCronograma : array();

$obBeneficiarioControle = new BeneficiarioControle();
$arBeneficiario = $obBeneficiarioControle->carregaBeneficiarioPorSbaid($sbaid, $anoref);
$arBeneficiario = ($arBeneficiario) ? $arBeneficiario : array();

$obItensComposicaoControle = new ItensComposicaoControle();
$arItensComposicao = $obItensComposicaoControle->carregaItensComposicaoPorSbaid($sbaid, $anoref);
$arItensComposicao = ($arItensComposicao) ? $arItensComposicao : array();

?>
<html>
	<head>
		<title>PAR - Cadastro de subação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
		<script type="text/javascript">
			function popupItenComposicao(id, ano)
			{
				if(id){
					url = 'par.php?modulo=principal/popupItensComposicao&acao=A&icoid='+id+'&ano='+ano;
				} else {
					url = 'par.php?modulo=principal/popupItensComposicao&acao=A&ano='+ano;
				}
				return window.open(url, 'Itens Composicao', "height=600,width=950,scrollbars=yes,top=50,left=200" );
			}

			function popupBeneficiario(id, ano)
			{
				url = 'par.php?modulo=principal/popupBeneficiarios&acao=A&id='+id+'&ano='+ano;
				return window.open(url, 'Itens Composicao', "height=600,width=800,scrollbars=yes,top=50,left=200" );
			}
		</script>	
	</head>
	<body>
	<?php	
	monta_titulo( 'Cadastro de subação', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
	?>
	<form action="<? echo $_SERVER['REQUEST_URI'] ?>" method="post">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Localização da subação</b></td>
			</tr>
			<tr>
				<td width="150px" class="SubTituloDireita">Dimensão:</td>
				<td><?php echo $arPropostaSubacao['dimdsc'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Área:</td>
				<td><?php echo $arPropostaSubacao['aredsc'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Indicador:</td>
				<td><?php echo $arPropostaSubacao['inddsc'] ?></td>
			</tr>
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Dados da subação</b></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Descrição da subação:</td>
				<td><?php echo $arPropostaSubacao['ppsdsc'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Forma de atendimento:</td>
				<td><?php echo $arPropostaSubacao['foadescricao'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Estratégia de implementação:</td>
				<td><?php echo ucwords($arPropostaSubacao['estrategiaimplementacao']) ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Unidade de medida:</td>
				<td><?php echo ucwords($arPropostaSubacao['unddsc']) ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Forma de execução:</td>
				<td><?php echo $arPropostaSubacao['frmdsc'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Cronograma:</td>
				<td><?php echo $arPropostaSubacao['crono'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita"></td>
				<td><input type="button" value="Editar" /></td>
			</tr>			
		</table>
		<br />
		<?php print carregaAbasAnosSubacao("par.php?modulo=principal/popupSubacao&acao=A&anoref={$anoref}"); ?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="2"><b>Detalhamento por ano que compõe a subação</b></td>
			</tr>
			<tr>
				<td width="150px" class="SubTituloDireita">Quantidade:</td>
				<td>
					<?php echo campo_texto('quantidade', 'S', 'S', '', 8, 10, '', '','','','','','',$arCronograma['quantidade']) ?>
				</td>
			</tr>
			<tr>
				<td width="150px" class="SubTituloDireita">Cronograma físico:</td>
				<td>
					<?php 
					$arMeses = array(
									array('codigo'=>'1', 'descricao'=>'Janeiro'),
									array('codigo'=>'2', 'descricao'=>'Fevereiro'),
									array('codigo'=>'3', 'descricao'=>'Março'),
									array('codigo'=>'4', 'descricao'=>'Abril'),
									array('codigo'=>'5', 'descricao'=>'Maio'),
									array('codigo'=>'6', 'descricao'=>'Junho'),
									array('codigo'=>'7', 'descricao'=>'Julho'),
									array('codigo'=>'8', 'descricao'=>'Agosto'),
									array('codigo'=>'9', 'descricao'=>'Setembro'),
									array('codigo'=>'10', 'descricao'=>'Outubro'),
									array('codigo'=>'11', 'descricao'=>'Novembro'),
									array('codigo'=>'12', 'descricao'=>'Dezembro')
									);
									
					$db->monta_combo('inicio', $arMeses, 'S', 'Selecione...', '', '','','','','','',$arCronograma['inicio']);
					echo " a ";
					$db->monta_combo('fim', $arMeses, 'S', 'Selecione...', '', '','','','','','',$arCronograma['fim']);
					echo " Ano de término: ";
					echo campo_texto('anotermino', 'N', 'S', '', 6, 8, '', '','','','','','',$arCronograma['anotermino']);
					
					?>
				</td>
			</tr>
			<tr>
				<td colspan="2" class="SubTituloEsquerda"><input type="submit" value="Salvar"></td>
			</tr>
		</table>
		
		<br />
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="6"><strong>Itens de Composição</strong></td>
			</tr>
			<tr bgcolor="#e9e9e9" align="center">
				<th>Ações</th>
				<th>Identificação do Item</th>
				<th>Unidade de Medida</th>
				<th>Quantidade</th>
				<th>Qtd	Valor Unitário	Total</th>
				<th>Total</th>
			</tr>
			<?php if($arItensComposicao): ?>
				<?php $x=2; ?>
				<?php foreach($arItensComposicao as $item): ?>
					<?php 
					$cor = ($x % 2) ? '#f0f0f0' : 'white';
					$x++;
					?>				
					<tr style="background:<?php echo $cor ?>;">
						<td align="left" valign="top">
							<a href="javascript:void(0)" onclick="popupItenComposicao('<?php echo $item['icoid'] ?>')"><img border="0" src="../imagens/alterar.gif"></a>
						</td>
						<td align="left" valign="top"><?php echo ucwords($item['icodescricao']) ?></td>
						<td align="left" valign="top"><?php echo ucwords($item['unddsc']) ?></td>
						<td align="right" valign="top"><?php echo $item['icoquantidade'] ?></td>
						<td align="right" valign="top"><?php echo formata_valor($item['icovalor']) ?></td>
						<td align="right" valign="top"><?php echo formata_valor($item['icovalortotal']) ?></td>
					</tr>
				<?php endforeach; ?>			
				<tr>
					<td colspan="6">
						<a href="#" style="text-decoration: none" onclick="popupItenComposicao('', '<?php echo $anoref ?>')"><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" /> Editar / Inserir itens de composição</a>
					</td>
				</tr>
			<?php else: ?>
				<tr>
					<td align="center" colspan="6">
						<p><b>Não existem itens de composição para este ano.</b></p>
					</td>
				</tr>
				
			<?php endif; ?>
		</table>
		
		<br />
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr bgcolor="#e9e9e9" align="center">
				<td colspan="6"><strong>Beneficiários</strong></td>
			</tr>
			<tr bgcolor="#e9e9e9" align="center">
				<!-- th width="80px" align="left" valign="top">Ações</th -->
				<th align="left" valign="top">Descrição</th>
				<th align="right" valign="top">Total urbana</th>
				<th align="right" valign="top">Total rural</th>
				<th align="right" valign="top">Total</th>
			</tr>
			<?php if($arBeneficiario): ?>
				<?php $x=2; ?>
				<?php foreach($arBeneficiario as $beneficiario): ?>
					<?php 
					$cor = ($x % 2) ? '#f0f0f0' : 'white';
					$x++;
					?>
					<tr style="background:<?php echo $cor ?>;">
						<!-- td align="left" valign="top">
							<img src="../imagens/alterar.gif">
						</td -->
						<td align="left" valign="top"><?php echo ucwords($beneficiario['bendsc']) ?></td>
						<td align="right" valign="top"><?php echo $beneficiario['vlrurbano'] ?></td>
						<td align="right" valign="top"><?php echo $beneficiario['vlrrural'] ?></td>
						<td align="right" valign="top"><?php echo $beneficiario['total'] ?></td>
					</tr>					
				<?php endforeach; ?>
				<tr>
					<td colspan="5">
						<a href="#" style="text-decoration: none" onclick="popupBeneficiario('<?php echo $sbaid ?>', '<?php echo $anoref ?>')"><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" /> Editar beneficiários</a>
					</td>
				</tr>
			<?php else: ?>
				<tr>
					<td align="center" colspan="5">
						<p><b>Não existem beneficiários para este ano.</b></p>
					</td>
				</tr>
			<?php endif; ?>
			
		</table>
	</form>
	</body>
</html>