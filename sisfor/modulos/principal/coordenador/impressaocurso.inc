<?php
if($_REQUEST['curid'] || $_REQUEST['cnvid']){
	if($_REQUEST['cnvid']){
		$_REQUEST['curid'] = recuperarCursoNVinculado($_REQUEST['cnvid']);
	}
	include_once APPRAIZ . "www/catalogocurso2014/_funcoes.php";
	include_once APPRAIZ . "www/catalogocurso2014/_componentes.php";
	include_once APPRAIZ . "catalogocurso2014/modulos/principal/impressaoCurso.inc";
} else { ?>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td align="center"><label class="TituloTela" style="color:#000000;">Dados Gerais</label></td>
		</tr>			
	</table>	
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<?php if($_REQUEST['ocuid']){ 
	$outrocursos = listarOutrosCursos($_REQUEST['ocuid']);?>
		<tr>
			<td class="subtituloDireita" width="25%">Secretaria responsável:</td>
			<td><?php echo $outrocursos['coordsigla']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome do curso:</td>
			<td><?php echo $outrocursos['ocunome']; ?></td>
		</tr>		
		<tr>
			<td class="subtituloDireita">Descrição:</td>
			<td><?php echo $outrocursos['ocudesc']; ?></td>
		</tr>				
		<tr>
			<td class="subtituloDireita">Vagas Propostas:</td>
			<td><?php echo $outrocursos['sifqtdvagas']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">LOA (R$):</td>
			<td><?php echo $outrocursos['sifvalorloa']; ?></td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Outras Fontes (R$):</td>
			<td><?php echo $outrocursos['sifvaloroutras']; ?></td>
		</tr>			
	<?php } elseif($_REQUEST['oatid']){ 
	$atividades = listarOutrasAtividades($_REQUEST['oatid']); ?>
		<tr>
			<td class="subtituloDireita" width="25%">Secretaria responsável:</td>
			<td><?php echo $atividades['coordsigla']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome da atividade:</td>
			<td><?php echo $atividades['oatnome']; ?></td>
		</tr>		
		<tr>
			<td class="subtituloDireita">Ano Proposta:</td>
			<td><?php echo $atividades['oatanoproposta']; ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">LOA (R$):</td>
			<td><?php echo $atividades['sifvalorloa']; ?></td>
		</tr>		
	<?php } ?>
	</table>	
<?php } ?>