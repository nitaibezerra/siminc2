<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<link href="css/_termoCooperacao.css" type="text/css" rel="stylesheet"></link>
<script>

function PrintElementID(id, pg) {
	//document.location.href = pg;
	window.open(pg);	
	return true;
}
</script>
<?php

verificaCodigoTermoSessao();

if ($_REQUEST['imprime'] == 'true') {
	$boPopup = true;
	$imprime = true;
}

if($boPopup){
	echo '	
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	';
}else{
	require_once APPRAIZ . 'includes/cabecalho.inc';
}

echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '' );
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

$sql = "select 
			tcpparecertecnico,
			tcpconsidentproponente,
			tcpconsidproposta,
			tcpconsidobjeto,
			tcpconsidobjetivo,
			tcpconsidjustificativa,
			tcpconsidvalores,
			tcpconsidcabiveis,
			tcpusucpfparecer
		from 
			monitora.termocooperacao 
		where 
			tcpid = ". $_SESSION['elabrev']['tcpid'];
 
$dadoParecer = !empty($_SESSION['elabrev']['tcpid']) ? $db->pegaLinha($sql) : array();

if ($estadoAtual == EM_ANALISE_OU_PENDENTE && verificaEquipeTecnicaProponente()) {
    $habilita_parecer = 'N';
    $habilitaSalvar = 'N';
}

if (($estadoAtual == EM_ANALISE_COORDENACAO && verificaEquipeTecnicaProponente()) ||
    ($estadoAtual == EM_ANALISE_COORDENACAO && in_array(PERFIL_COORDENADOR_SEC, $perfis))) {
    $habilita_parecer = 'S';
    $habilitaSalvar = 'S';
}

if (in_array(PERFIL_SUPER_USUARIO, $perfis)) {
    $habilita_parecer = 'S';
    $habilitaSalvar = 'S';
}

?>
<form action="" method="post" enctype="multipart/form-data" id="formulario" name="formulario">
	<input type="hidden" id="tcpid" name="tcpid" value="<?=$_SESSION['elabrev']['tcpid'] ?>" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="aba" name="aba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="acaoAba" name="acaoAba" value="<?=$_REQUEST['aba'] ?>" />
	<input type="hidden" id="arptipo" name="arptipo" value="P">
	<input type="hidden" id="imprime" name="imprime" value="" />
	<input type="hidden" id="abaAtual" name="abaAtual" value="<?=$_REQUEST['aba'] ?>" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloCentro" valign="bottom">Parecer técnico</td>
		</tr>
	</table>
	<input type="hidden" name="tcpparecertecnico" id="tcpparecertecnico" value="" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Considerações sobre a entidade proponente:</td>
			<td>
				<?php if($imprime): ?>
					<?php echo $dadoParecer['tcpconsidentproponente']; ?>
				<?php else: ?>
					<?=campo_textarea( 'tcpconsidentproponente', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '', $dadoParecer['tcpconsidentproponente']);?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Considerações sobre a proposta:</td>
			<td>
				<?php if($imprime): ?>
					<?php echo $dadoParecer['tcpconsidproposta']; ?>
				<?php else: ?>
					<?=campo_textarea( 'tcpconsidproposta', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidproposta']);?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Considerações sobre o objeto:</td>
			<td>				
				<?php if($imprime): ?>
					<?php echo $dadoParecer['tcpconsidobjeto']; ?>
				<?php else: ?>
					<?=campo_textarea( 'tcpconsidobjeto', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidobjeto']);?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Considerações sobre o objetivo:</td>
			<td>				
				<?php if($imprime): ?>
					<?php echo $dadoParecer['tcpconsidobjetivo']; ?>
				<?php else: ?>
					<?=campo_textarea( 'tcpconsidobjetivo', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidobjetivo']);?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Considerações sobre a justificativa:</td>
			<td>				
				<?php if($imprime): ?>
					<?php echo $dadoParecer['tcpconsidjustificativa']; ?>
				<?php else: ?>
					<?=campo_textarea( 'tcpconsidjustificativa', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidjustificativa']);?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Considerações sobre os valores:</td>
			<td>				
				<?php if($imprime): ?>
					<?php echo $dadoParecer['tcpconsidvalores']; ?>
				<?php else: ?>
					<?=campo_textarea( 'tcpconsidvalores', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidvalores']);?>
				<?php endif; ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="15%">Outras considerações cabíveis:</td>
			<td>				
				<?php if($imprime): ?>
					<?php echo $dadoParecer['tcpconsidcabiveis']; ?>
				<?php else: ?>
					<?=campo_textarea( 'tcpconsidcabiveis', 'S', $habilita_parecer, '', 150, 5, 1000, '', '', '', '', '',$dadoParecer['tcpconsidcabiveis']); ?>
				<?php endif; ?>
			</td>
		</tr>
		
		<?php if($dadoParecer['tcpusucpfparecer']): ?>
			<tr>
				<td height="20"></td>
				<td></td>
			</tr>
		
			<tr>
				<td colspan="2" ><b>Parecer técnico elaborado por:&nbsp;
				
					<b>				
					<?php echo formatar_cpf($dadoParecer['tcpusucpfparecer']); echo " - ".$db->pegaum("select usunome from seguranca.usuario where usucpf = '{$dadoParecer['tcpusucpfparecer']}'") ?>
					</b>
				</td>
			</tr>
		
		<?php endif; ?>
		
	</table>

    <?php
    $habilitaInserir = quemPodeAnexarArquivo($habilitaInserir);
    ?>
	
	<table><tr><td>&nbsp;</td></tr></table>
	
	<table id="anexos" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tr>
			<td colspan="6" class="subtitulocentro" style="background-color: #CFCFCF;">Anexos</td>
		</tr>
		<tr id="tr_titulo">
			<td class="subtituloesquerda" >Ação</td>
			<td class="subtituloesquerda" >Arquivo</td>
			<td class="subtituloesquerda" >Descrição</td>
			<td class="subtituloesquerda" >Usuário</td>
			<td class="subtituloesquerda" >Data de inclusão</td>
			<td class="subtituloesquerda" >Visualizar arquivo</td>
		</tr>
		<?php listaAnexos('P'); ?>
	</table>

	<?php if (in_array(PERFIL_SUPER_USUARIO, $perfis)
                ||($estadoAtual == EM_ANALISE_COORDENACAO && verificaEquipeTecnicaConcedente())
                || ($estadoAtual == EM_ANALISE_OU_PENDENTE && in_array(PERFIL_COORDENADOR_SEC, $perfis))) : ?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1" id="">
			<tr>
				<td colspan="3">
					<a href="javascript:void(0)" class="inserirAnex"> 
						<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> Inserir Anexo
					</a>
				</td>
			</tr>
		</table>
	<?php endif; ?>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="3" align="center">
				<?php if ($habilitaSalvar == 'S' && !$imprime) : ?>
					<input type="button" id="previsao" value="Gravar e Voltar" class="navegarSalvar"/> 
					<input type="button" id="salva" value="Gravar" class="salvar"/>
					<input type="button" id="imprime" value="Imprimir" onclick="PrintElementID('imprimirTela', 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=parecertecnico&imprime=true')" />
					<input type="button" id="anexo" value="Gravar e Continuar" class="navegarSalvar"/>
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>