<?php
require APPRAIZ . 'minc/modulos/principal/monitoramento/monitoramentoController.php';
$perfis = arrayPerfil();
$pode = in_array(PERFIL_MINC_CADASTRADOR, $perfis)&&in_array(PERFIL_MINC_SUPER_USUARIO, $perfis);

$habilita = 'N';
$bloq = 'S';
if($pode){
	$habilita = 'S';
	$bloq = 'N';
}
$arMnuid = array();
if ($docid) {
    $sqlDocumento = "select * from workflow.documento where docid = {$docid}";
    $resultDocumento = $db->pegaLinha($sqlDocumento);

    if ($resultDocumento && !in_array($resultDocumento['esdid'], array(/*ESTADO_DOCUMENTO_AVALIACAO, ESTADO_DOCUMENTO_FINALIZADO, ESTADO_DOCUMENTO_AVALIADO, */ESTADO_DOCUMENTO_ENVIADO_FNDE/*, ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR, ESTADO_DOCUMENTO_AVALIACAO*/))) {
        $arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
    }
}
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

$titulo = "Mais Cultura nas Escolas";
$subtitulo = "Monitoramento";
echo monta_titulo($titulo, $subtitulo);
echo cabecalho();

require APPRAIZ . 'minc/modulos/principal/monitoramento/monitoramento_js.php';
echo "<script>
			alert('Prezados(as),\\nInformamos que o prazo para o preenchimento do Monitoramento do Programa Mais Cultura nas Escolas foi prorrogado, ainda sem data definida. \\nA aba monitoramento permanecerá aberta para constantes atualizações.');
	  </script>";
	
//exit;

?>
<script type="text/javascript">


jQuery(document).ready(function(){
        
        jQuery('.imagemexcluir').attr('disabled', true);
        jQuery('.inserirparceiro').attr('disabled', true);
        jQuery('.salvarorcamento').attr('disabled', true);
        jQuery('.salvaresporadica').attr('disabled', true);
        
        <?php if(in_array(PERFIL_MINC_CADASTRADOR, $perfis) || in_array(PERFIL_MINC_SUPER_USUARIO, $perfis)){ ?>
            jQuery('.imagemexcluir').attr('disabled', false);
            jQuery('.inserirparceiro').attr('disabled', false);
            jQuery('.salvarorcamento').attr('disabled', false);
            jQuery('.salvaresporadica').attr('disabled', false);
        <?php } ?>
            
});
</script>
	<form id="form1" enctype="multipart/form-data" name="form1" method="POST">
		<input type="hidden" value="" name="requisicao" id="requisicao"/>
		<table align="center" width="95%" border="0" cellpadding="2" cellspacing="1">
			<tr bgcolor="#e7e7e7">
				<td align="center" style="font-size: 13px" colspan="2">
					<br/>

					<p><b>Escolas e Iniciativas Culturais Parceiras</b></p>
				</td>
			</tr>
			<tr>
				<td align="left" style="font-size: 13px" colspan="2">
					<p>Este instrumento foi criado com o intuito de acompanhar o desenvolvimento das atividades do Programa Mais Cultura nas Escolas, servindo também como espaço de registro de informações pedagógicas, técnicas e orçamentárias. Neste sentido, escola e Iniciativa Cultural Parceira devem preencher conjuntamente os campos que compõem este monitoramento, na perspectiva de consolidar a parceria e fortalecer a ação pedagógica no desenvolvimento do Programa Mais Cultura nas Escolas. Cabe salientar que este monitoramento estará disponível e editável para possíveis alterações, atualizações e novas informações no decorrer de toda a execução do Plano de Atividade Cultural.</p>
				</td>
			</tr>
			<?= '<br/>'; ?>
			<tr bgcolor="#e7e7e7">
				<td align="Center" style="font-size: 13px" colspan='2'>
					<p><b>Iniciativa Cultural Parceira</b></p>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Indique a Iniciativa Cultural Parceira</td>
				<td>
					&nbsp; <input type="radio" name="montipoiniciativa" id="cpf" value="1" <?= (($montipoiniciativa == 1) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> CPF &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="montipoiniciativa" id="cnpj" value="2" <?= (($montipoiniciativa == 2) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> CNPJ
				</td>
			</tr>
			<tr class="cpf">
				<td class="SubTituloDireita" width="20%">CPF:</td>
				<td><?= campo_texto('moncpfcnpj1', "S", $habilita, "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id ="moncpfcnpj" class="parceiro"', '', ($moncpfcnpj ? mascaraglobal($moncpfcnpj, '###.###.###-##') : ''), "pegaNome(this)"); ?></td>
			</tr>

			<!------------------------------------->

			<tr class="cnpj">
				<td class="SubTituloDireita" width="20%">CNPJ:</td>
				<td><?= campo_texto('moncpfcnpj2', "S", $habilita, "CNPJ", 20, 18,"##.###.###/####-##", "", '', '', 0, 'id ="moncpfcnpj_cnpj" class="parceiro"', '', ($moncpfcnpj ? mascaraglobal($moncpfcnpj, '##.###.###/####-##') : ''),""); ?></td>
			</tr>
			<tr class="cpf cnpj">
				<td class="SubTituloDireita" width="20%">Nome:</td>
				<td><input type="text" name="monnomeinicultpar" class="parceiro" id="monnomeinicultpar" maxlength="70" size="68" value="<?= $monnomeinicultpar; ?>" readonly="true" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>></td>
			</tr>
			<tr class="cpf cnpj">
				<td class="SubTituloDireita" width="20%">Email:</td>
				<td><input type="text" name="monemailicp" title="E-mail" class="parceiro" id="monemailicp" value="<?= $monemailicp; ?>" maxlength="70" size="68" <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>></td>
			</tr>
			<tr class="cpf cnpj">
				<td class="SubTituloDireita" width="20%">Telefone</td>
				<td><?= campo_texto('montelefoneicp', "N", $habilita, "Telefone", 20, 21,"##-####-####", "", '', '', 0, 'id ="montelefoneicp" class="parceiro"', '', ($montelefoneicp ? mascaraglobal($montelefoneicp, '##-####-####') : ''),""); ?></td>
			</tr>

			<!------------------------------------->


			<tr class="cpf cnpj">
				<td class="SubTituloDireita" width="20%">Ocorreu alteração da anteriormente cadastrada no <?php echo SIGLA_SISTEMA; ?>?</td>
				<td>
					&nbsp; <input type="radio" name="alteracao" id="alteracao" value="t" class="parceiro1" <?= (($monocorreualteracaodaicp == "t") ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> SIM &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="alteracao" id="alteracao" value="f" class="parceiro1" <?= (($monocorreualteracaodaicp == "f") ? "checked" : ($monocorreualteracaodaicp == '') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> NÃO
				</td>
			</tr>
			<tr id="arquivo1" class="alteracao">
				<td class="SubTituloDireita" width="20%">Neste espaço a escola deverá anexar a Ata do Conselho escolar, apresentando as justificativas e motivos da mudança da Iniciativa Cultural Parceira</td>
				<td>
					<?php
					$sql = "
							SELECT
								arq.arqid, 
								arq.arqnome, 
								arq.arqextensao
							FROM minc.monitoramento mon
							INNER JOIN minc.mceanexo ang ON ang.monid = mon.monid
							INNER JOIN public.arquivo arq USING (arqid)
							WHERE
								mceid = '{$_SESSION['minc']['mceid']}'
							";

					$infoInstLegal = $db->pegaLinha($sql);
					if (isset($infoInstLegal['arqid']) && !empty($infoInstLegal['arqid'])): ?>
						<input type="hidden" name="arquivo" id="arquivo" value="uploaded"/>
						<? /*if($habilita=='S'){*/ ?>
						<img <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> src="../imagens/excluir.gif" style=cursor:pointer; onclick="excluirAnexo('<?php echo $infoInstLegal['arqid']; ?>');"/>&nbsp;
						<? /*}*/ ?>
						<a href="minc.php?modulo=principal/monitoramento&acao=A&requisicao=downloadArquivo&arqid=<?php echo $infoInstLegal['arqid']; ?>">
							<?= "{$infoInstLegal['arqnome']}.{$infoInstLegal['arqextensao']}"; ?>
						</a>
					<? else: ?>
						<input type="file" name="arquivo" id="arquivo" class="parceiro"/>
					<?endif; ?>
				</td>
			</tr>
			<tr class="cpf cnpj">
				<td class="SubTituloDireita" width="20%">A Iniciativa Cultural Parceira pode ser identificada, prioritariamente, como:</td>
				<td>
					<?
					$sql = "SELECT
								tpiid as codigo,
								tpidescricao as descricao
							FROM
								minc.tipoidentidadeiniciativa
							Order by codigo";
					$db->monta_combo("tpiid", $sql, $habilita, '--Selecione--', '', '', '', '', '', 'tpiid', '', '', '', '', 'class="parceiro"');
					?>
				</td>
			</tr>
			<tr class="identifica2">
				<td class="SubTituloDireita" width="20%">Especifique o tipo</td>
				<td><?= campo_texto('mondescricaotipoidentidadeespecifico', "S", $habilita, "Nome", 67, 70, "", "", '', '', 0, 'class="parceiro"'); ?></td>
			</tr>
			<tr class="cpf">
				<td class="SubTituloDireita" width="20%">A Iniciativa cultural Parceira Representa Grupo/Coletivo Artistico Cultural?</td>
				<td>
					&nbsp; <input type="radio" name="monrepresentagrupo" id="monrepresentagrupo" value="t" class="parceiro1" <?= (($monrepresentagrupo == 't') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> SIM &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="monrepresentagrupo" id="monrepresentagrupo" value="f" class="parceiro1" <?= (($monrepresentagrupo == 'f') ? "checked" : ($monrepresentagrupo == '') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> NÃO
				</td>
			</tr>
			<tr id="representa" class="cultura">
				<td class="SubTituloDireita" width="20%">Informe Area de Atuação e o nome dos integrantes do grupo que atuam no Mais Cultura nas Escolas.</td>
				<td><?= campo_texto('monnomeintegrantes', "S", $habilita, "Nome", 67, 70, "", "", '', '', 0, 'class="parceiro"'); ?></td>
			</tr>
			<tr class="cpf cnpj">
				<td class="SubTituloDireita" width="20%"> Identifica-se com a temática</td>
				<td>
					<?
					$sql = "SELECT
								ttmid as codigo,
								ttmdescricao as descricao
							FROM
								minc.tipotematica
							Order by codigo";
					$db->monta_combo("ttmid", $sql, $habilita, '--Selecione--', '', '', '', '', '', 'ttmid', '', '', '', '', 'class="parceiro"');
					?>
				</td>
			</tr>
			<tr class="tematica2">
				<td class="SubTituloDireita" width="20%">Identifique de Forma Objetiva:</td>
				<td>
					<?= campo_texto('mondescreveidentidadecultural', "S", $habilita, "Nome", 67, 70, "", "", '', '', 0, 'class="parceiro"'); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Como sua escola se identifica:</td>
				<td><?
					$sql = "SELECT
								tieid as codigo,
								tiedescricao as descricao
							FROM
								minc.tipoidentidadeescola
							Order by codigo";
					$db->monta_combo("tieid", $sql, $habilita, '--Selecione--', '', '', '', '', '', 'tieid', '', '', '', '', 'class="parceiro"');
					?>
				</td>
			</tr>
			<tr class="indigena2">
				<td class="SubTituloDireita" width="20%">Especifique a etnia:</td>
				<td><?= campo_texto('monetiniaescola', "S", $habilita, "Nome etnia", 67, 70, "", "", '', '', 0, 'class="parceiro"'); ?></td>
			</tr>
		</table>
		<br>
		<table align="center" width="95%" border="0" cellpadding="2" cellspacing="1">
			<tr bgcolor="#e7e7e7">
				<td align="center" style="font-size: 13px" colspan="2">
					<p><b>Coordenador do Mais Cultura nas Escolas</b></p>
				</td>
			</tr>
			<tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Cordenador(a) é o mesmo do Programa Mais Educação?</td>
				<td>
					&nbsp; <input type="radio" name="monmesmocorrdenadormaiseducacao" id="monmesmocorrdenadormaiseducacao" value="t" <?= (($monmesmocorrdenadormaiseducacao == 't') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> SIM &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="monmesmocorrdenadormaiseducacao" id="monmesmocorrdenadormaiseducacao" value="f" <?= (($monmesmocorrdenadormaiseducacao == 'f') ? "checked" : ($monmesmocorrdenadormaiseducacao == '') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> NÃO
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Cordenador(a) é a Initiciativa Cultural Parceira?</td>
				<td>
					&nbsp; <input type="radio" name="moncoordeiniciativacultparceira" id="moncoordeiniciativacultparceira" value="t" <?= (($moncoordeiniciativacultparceira == 't') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> SIM &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="moncoordeiniciativacultparceira" id="moncoordeiniciativacultparceira" value="f" <?= (($moncoordeiniciativacultparceira == 'f') ? "checked" : ($moncoordeiniciativacultparceira == '') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> NÃO
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Cordenador(a) permanece o mesmo(a) registrado(a) na inscrição do Plano de Atividade Cultural?</td>
				<td>
					&nbsp; <input type="radio" name="moncoordmesmodainscr" id="moncoordmesmodainscr" value="t" <?= (($moncoordmesmodainscr == 't') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> SIM &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="moncoordmesmodainscr" id="moncoordmesmodainscr" value="f" <?= (($moncoordmesmodainscr == 'f') ? "checked" : ($moncoordmesmodainscr == '') ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>> NÃO
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Nome:</td>
				<td><?= campo_texto('monnomecoord', "N", $habilita, "Nome", 67, 70, "", "", '', '', 0, 'class="monnomecoord"'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">CPF:</td>
				<td><?= campo_texto('moncpfcoord', "N", $habilita, "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'class="moncpfcoord"'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Telefone</td>
				<td><?= campo_texto('montelcoord', "N", $habilita, "Telefone", 16, 14, "##-####-####", "", '', '', 0, 'class="montelcoord"'); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Email:</td>
				<td><?= campo_texto('monemailcoord', "N", $habilita, "E-mail", 67, 70, "", "", '', '', 0, 'class="monemailcoord"'); ?></td>
			</tr>
		</table>

		<br>
		<table align="center" width="95%" border="0" cellpadding="0" cellspacing="1">
			<tr bgcolor="#e7e7e7">
				<td align="center" style="font-size: 13px" colspan="3" class="SubTituloCentro">
					Orçamento
				</td>
			</tr>
			<tr>
			<tr>
				<td align="left" style="font-size: 12px" colspan="3" class="SubTituloEsquerda">
					<p>
						Este espaço é destinado para informar detalhadamente os valores gastos com o desenvolvimento do Programa Mais Cultura nas Escolas.<br> Indentifique os valores gastos atualmente com cada ação orçamentária, considerando os valores recebidos na primeira parcela.
					</p>
				</td>
			</tr>
			<tr>
				<td align="center" style="font-size: 11px" width="70%" class="SubTituloCentro">
					O que
				</td>
				<td align="center" style="font-size: 11px" width="15%" class="SubTituloCentro">
					1ª Parcela (R$)
				</td>
				<!--   <td align="center" style="font-size: 11px" width="15%" class="SubTituloCentro">--><!-- 		  	2ª Parcela (R$) --><!-- 		  </td> -->
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Total do Orçamento</td>
				<td align="center" style="font-size: 14px; font-weight:bold">
					<?
					if ($monvalorparcela) {
						echo number_format($monvalorparcela, 2, ",", ".");
					} else {
						echo '0,00';
					}

					if (!$monaquisicaomaterialconsumo1) $monaquisicaomaterialconsumo1 = 0;
					if (!$moncontratacaoservicosculturais1) $moncontratacaoservicosculturais1 = 0;
					if (!$moncontratacaoservicosdiversos1) $moncontratacaoservicosdiversos1 = 0;
					if (!$monlocacaodeinstrumentos1) $monlocacaodeinstrumentos1 = 0;
					if (!$monaquisicaomateriaispermanentes1) $monaquisicaomateriaispermanentes1 = 0;

					$soma = ($monaquisicaomaterialconsumo1 + $moncontratacaoservicosculturais1 + $moncontratacaoservicosdiversos1 + $monlocacaodeinstrumentos1 + $monaquisicaomateriaispermanentes1);
					$saldo = $monvalorparcela - $soma;

					if ($monaquisicaomaterialconsumo1 == 0) $monaquisicaomaterialconsumo1 = '';
					if ($moncontratacaoservicosculturais1 == 0) $moncontratacaoservicosculturais1 = '';
					if ($moncontratacaoservicosdiversos1 == 0) $moncontratacaoservicosdiversos1 = '';
					if ($monlocacaodeinstrumentos1 == 0) $monlocacaodeinstrumentos1 = '';
					if ($monaquisicaomateriaispermanentes1 == 0) $monaquisicaomateriaispermanentes1 = '';
					?>
				</td>
				<!-- 	<td align="center"><? /* echo $monvalorparcela;*/ ?></td>-->
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Aquisição de Materiais de consumo</td>
				<td align="center"><?= campo_texto('monaquisicaomaterialconsumo1', "N", $habilita, "Materiais de consumo", 10, 10, "###.###,##", "", '', '', 0, 'class="soma1"', '', (($monaquisicaomaterialconsumo1) ? number_format($monaquisicaomaterialconsumo1, 2, ",", ".") : '')); ?></td>
				<!--<td align="center"><? /* echo campo_texto('monaquisicaomaterialconsumo2', "N", '', "Materiais de consumo", 10, 10, "###.###,##", "", '', '', 0, 'class="soma2"','',(($monaquisicaomaterialconsumo2)?number_format($monaquisicaomaterialconsumo2,2,",","."):'')); */ ?></td>-->
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Contratação de serviços culturais necessários às atividades artisticas Pedagógicas</td>
				<td align="center"><?= campo_texto('moncontratacaoservicosculturais1', "N", $habilita, "Atividades artisticas Pedagógicas", 10, 10, "###.###,##", "", '', '', 0, 'class="soma1"', '', (($moncontratacaoservicosculturais1) ? number_format($moncontratacaoservicosculturais1, 2, ",", ".") : '')); ?></td>
				<!--<td align="center"><? /* echo campo_texto('moncontratacaoservicosculturais2', "N", '', "Atividades artisticas Pedagógicas", 10, 10, "###.###,##", "", '', '', 0, 'class="soma2"','',(($moncontratacaoservicosculturais2)?number_format($moncontratacaoservicosculturais2,2,",","."):'')); */ ?></td>-->
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Contratação de serviços diversos</td>
				<td align="center"><?= campo_texto('moncontratacaoservicosdiversos1', "N", $habilita, "Serviços diversos", 10, 10, "###.###,##", "", '', '', 0, 'class="soma1"', '', (($moncontratacaoservicosdiversos1) ? number_format($moncontratacaoservicosdiversos1, 2, ",", ".") : '')); ?></td>
				<!--<td align="center"><? /* echo campo_texto('moncontratacaoservicosdiversos2', "N", '', "Serviços diversos", 10, 10, "###.###,##", "", '', '', 0, 'class="soma2"','',(($moncontratacaoservicosdiversos2)?number_format($moncontratacaoservicosdiversos2,2,",","."):'')); */ ?></td>-->
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Locação de instrumentos, transporte e equipamentos</td>
				<td align="center"><?= campo_texto('monlocacaodeinstrumentos1', "N", $habilita, "Instrumentos, transporte e equipamentos", 10, 10, "###.###,##", "", '', '', 0, 'class="soma1"', '', (($monlocacaodeinstrumentos1) ? number_format($monlocacaodeinstrumentos1, 2, ",", ".") : '')); ?></td>
				<!--<td align="center"><? /* echo campo_texto('monlocacaodeinstrumentos2', "N", '', "Instrumentos, transporte e equipamentos", 10, 10, "###.###,##", "", '', '', 0, 'class="soma2"','',(($monlocacaodeinstrumentos2)?number_format($mmonlocacaodeinstrumentos2,2,",","."):'')); */ ?></td>-->
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Aquisição de materiais permanentes e equipamentos</td>
				<td align="center"><?= campo_texto('monaquisicaomateriaispermanentes1', "N", $habilita, "Materiais permanentes e equipamentos", 10, 10, "###.###,##", "", '', '', 0, 'class="soma1"', '', (($monaquisicaomateriaispermanentes1) ? number_format($monaquisicaomateriaispermanentes1, 2, ",", ".") : '')); ?></td>
				<!--<td align="center"><? /*echo campo_texto('monaquisicaomateriaispermanentes2', "N", '', "Materiais permanentes e equipamentos", 10, 10, "###.###,##", "", '', '', 0, 'class="soma2"','',(($mmonaquisicaomateriaispermanentes2)?number_format($monaquisicaomateriaispermanentes2,2,",","."):'') ); */ ?></td>-->
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Saldo do orçamento</td>
				<td align="center"><?= campo_texto('monsaldoorcamento1', "N", 'N', "Saldo do orçamento", 10, 10, "###.###,##", "", '', '', 0, '', '', (($saldo) ? number_format($saldo, 2, ",", ".") : '')); ?></td>
				<!--<td align="center"><? /*echo campo_texto('monsaldoorcamento2', "N", 'N', "Saldo do orçamneto", 10, 10, "###.###,##", "", '', '', 0); */ ?></td>-->
			</tr>
		</table>
		<table align="center" width="95%" border="0" cellpadding="2" cellspacing="2">
			<tr>
				<td align="center">
					<br> <input type="button" class="inserirparceiro" value="Salvar"> <br><br>
				</td>
			</tr>
		</table>
	</form>

<? monta_titulo('Desenvolvimento de Atividades', ''); ?>

	<form id="form2" enctype="multipart/form-data" name="form2" method="POST">
		<input type="hidden" value="" name="requisicao2" id="requisicao2"/>
		<table align="center" width="95%" border="0" cellpadding="2" cellspacing="2">
			<tr bgcolor="#e7e7e7">
				<td align="center" style="font-size: 13px" colspan="2">
					<p><b>Atividades Frequentes</b></p>
				</td>
			</tr>
			<tr>
			<tr>
				<td align="left" style="font-size: 13px" colspan="2" class="SubTituloEsquerda">
					Entende-se por atividades frequentes aquelas realizadas constantemente no desenvolvimento da ação proposta.
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Datas Reais de execução</td>
				<td>Inicio:&nbsp;
					<?= campo_data2('atidatainicio', 'N', $habilita, '', 'S'); ?>
					&nbsp;&nbsp;&nbsp; Término:&nbsp;
					<?= campo_data2('atidatafim', 'N', $habilita, '', 'S'); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Dia(s) da semana em que executa a atividade</td>
				<td>
					<input type="checkbox" value="1" name="atidiasdasemana_seg" <?= (($montipoiniciativa == 1) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>>Seg. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="checkbox" value="1" name="atidiasdasemana_ter" <?= (($montipoiniciativa == 1) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>>Ter. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="checkbox" value="1" name="atidiasdasemana_qua" <?= (($montipoiniciativa == 1) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>>Qua. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="checkbox" value="1" name="atidiasdasemana_qui" <?= (($montipoiniciativa == 1) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>>Qui. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="checkbox" value="1" name="atidiasdasemana_sex" <?= (($montipoiniciativa == 1) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>>Sex. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="checkbox" value="1" name="atidiasdasemana_sab" <?= (($montipoiniciativa == 1) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>>Sab. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="checkbox" value="1" name="atidiasdasemana_dom" <?= (($montipoiniciativa == 1) ? "checked" : ""); ?> <?= $habilita=='N' ? 'disabled="disabled"' : ''; ?>>Dom. &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Turno</td>
				<td>
					<?
					$arrTurno = array(array("codigo" => "M", "descricao" => "Matutino"), array("codigo" => "V", "descricao" => "Vespertino"), array("codigo" => "N", "descricao" => "Noturno"));
					$db->monta_combo('atiturno', $arrTurno, $habilita, 'Selecione o Turno', '', '', '', '150', 'N', 'atiturno');
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Descreva detalhadamente a atividade/ação</td>
				<td>
					<?= campo_textarea('atidescricao', "N", $habilita, "Serviços diversos", 60, 5, "", "", '', '', 0, ''); ?>
				</td>
			</tr>
			<tr>
				<td></td>
				<td>
					<br> <input type="button" class="salvarorcamento" value="Inserir"> <br><br>
				</td>
			</tr>
		</table>
	</form>
<?
$sql = "
		SELECT
			'<img border=\'0\' class = \'imagemexcluir\' src=\'../imagens/excluir.gif\' style=\'cursor:pointer;\' onclick=\'apagarItem('||atiid||')\'>' as ordem,
			atidescricao as descricao,
			to_char(atidatainicio::date,'DD/MM/YYYY') as datainicial,
			to_char(atidatafim::date,'DD/MM/YYYY') as datafinal,
			case when substr(atidiasdasemana, 1, 1) = '1' then 'Seg. ' else '' end 
	        ||
	        case when substr(atidiasdasemana, 2, 1) = '1' then 'Ter. ' else '' end 
	        ||
	        case when substr(atidiasdasemana, 3, 1) = '1' then 'Qua. ' else '' end
	        ||
	        case when substr(atidiasdasemana, 4, 1) = '1' then 'Qui. ' else '' end
	        ||
	        case when substr(atidiasdasemana, 5, 1) = '1' then 'Sex. ' else '' end
	        ||
	        case when substr(atidiasdasemana, 6, 1) = '1' then 'Sáb. ' else '' end
	        ||
	        case when substr(atidiasdasemana, 7, 1) = '1' then 'Dom. ' else '' end
	        as periodicidade,
	        case when atiturno = 'M' then 'Matutino' 
	        	 when atiturno = 'V' then 'Vespertino'
	        	 when atiturno = 'N' then 'Noturno'
	        end
			as turno
		FROM
			minc.atividadesmonitoramento atv
		INNER JOIN minc.monitoramento mon ON mon.monid = atv.monid
		WHERE
			mceid = '{$_SESSION['minc']['mceid']}'
		AND atitipo = 'F'
				";
$cabecalho = array("Ação", "Descrição", "Data Inicial", "Data Final", "Periodicidade", "Turno");

$db->monta_lista($sql, $cabecalho, 100, 5, '', '95%', '', '');

?>
	<br>
	<form id="form3" enctype="multipart/form-data" name="form3" method="POST">
		<input type="hidden" value="" name="requisicao3" id="requisicao3"/>
		<table align="center" width="95%" border="0" cellpadding="2" cellspacing="2">
			<tr bgcolor="#e7e7e7">
				<td align="center" style="font-size: 13px" colspan="2">
					<p><b>Atividades Esporádicas</b></p>
				</td>
			</tr>
			<tr>
			<tr>
				<td align="left" style="font-size: 13px" colspan="2" class="SubTituloEsquerda">
					Entende-se por atividades esporádicas aquelas realizadas ocasionalmente.
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Datas Reais de execução</td>
				<td>Inicio:&nbsp;
					<?= campo_data2('atidatainicio2', 'N', $habilita, '', 'S'); ?>
					&nbsp;&nbsp;&nbsp; Término:&nbsp;
					<?= campo_data2('atidatafim2', 'N', $habilita, '', 'S'); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Descreva detalhadamente a atividade/ação</td>
				<td>
					<?= campo_textarea('atidescricao2', "N", $habilita, "Serviços diversos", 60, 5, '', "", '', '', 0, ''); ?>
				</td>
			</tr>
			<tr>
				<td></td>
				<td>
					<br> <input type="button" class="salvaresporadica" value="Inserir"> <br><br>
				</td>
			</tr>
		</table>
	</form>
<?
$sqlAtividadesmonitoramento = "
		SELECT
			'<img border=\'0\' class = \'imagemexcluir\'src=\'../imagens/excluir.gif\' style=\'cursor:pointer;\' onclick=\'apagarItem('||atiid||')\'>' as ordem,
			atidescricao as descricao,
			to_char(atidatainicio::date,'DD/MM/YYYY') as datainicial,
			to_char(atidatafim::date,'DD/MM/YYYY') as datafinal
		FROM
			minc.atividadesmonitoramento atv
		INNER JOIN minc.monitoramento mon ON mon.monid=atv.monid
		WHERE
			mceid = '{$_SESSION['minc']['mceid']}'
		AND atitipo = 'E'
				";

$cabecalho = array("Ordem", "Descrição", "Data Inicial", "Data Final");

$db->monta_lista($sqlAtividadesmonitoramento, $cabecalho, 100, 5, '', '95%', '', '');
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
	function pegaNome(obj) {
		var cpf = obj.value;
		var nome = document.getElementById('monnomeinicultpar');
		cpf = cpf.replace(/[^0-9]+/g,'');

		//valida cpf
		if (!validar_cpf(cpf)) {
			alert('CPF inválido!');
			obj.value = "";
			nome.value = "";
			//obj.focus();
			return false;
		}

		// Faz uma requisição ajax
		var req = new Ajax.Request('/includes/webservice/cpf.php', {
			method: 'post',
			parameters: '&ajaxCPF=' + cpf,
			asynchronous: false,
			onComplete: function (res) {
				//alert(res.responseText);
				if (res.responseText) {
					linhaItem = res.responseText.split('|');
					colunaItem = linhaItem[0].split('#');

					if (colunaItem[1]) {
						nome.value = colunaItem[1];
					} else {
						alert("CPF não existe!");
						obj.value = "";
						nome.value = "";
						//obj.focus();
					}

				} else {
					alert("CPF não existe!");
					obj.value = "";
					nome.value = "";
					//obj.focus();
				}

			}
		});
	}
</script>
