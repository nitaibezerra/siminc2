<?php
if($_SESSION['projovemurbano']['estuf']) {

$esfera = "Estadual";

$sql = "SELECT COALESCE(ent.entnumcpfcnpj,'-') as entnumcpfcnpj, ende.estuf, est.estcod, ent.entid FROM entidade.entidade ent
			INNER JOIN entidade.endereco ende ON ende.entid=ent.entid 
			INNER JOIN entidade.funcaoentidade fen ON fen.entid=ent.entid 
			INNER JOIN territorios.estado est ON est.estuf=ende.estuf 
			WHERE ende.estuf='".$_SESSION['projovemurbano']['estuf']."' AND fen.funid='6' ORDER BY entid DESC";

$secretaria = $db->pegaLinha($sql);

$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='".$_SESSION['projovemurbano']['estuf']."' AND ppuid = '".$_SESSION['projovemurbano']['ppuid']."'");
if($sugestaoampliacao['suaverdade']=="t") {
if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
}

}

if($_SESSION['projovemurbano']['muncod']) {

$esfera = "Municipal";

$sql = "SELECT ent.entnumcpfcnpj, ende.estuf, mun.mundescricao, ent.entid FROM entidade.entidade ent
			INNER JOIN entidade.endereco ende ON ende.entid=ent.entid 
			INNER JOIN territorios.municipio mun ON mun.muncod=ende.muncod  
			INNER JOIN entidade.funcaoentidade fen ON fen.entid=ent.entid 
			WHERE ende.muncod='".$_SESSION['projovemurbano']['muncod']."' AND fen.funid='7' ORDER BY entid DESC";

$secretaria = $db->pegaLinha($sql);


$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='".$_SESSION['projovemurbano']['muncod']."' AND ppuid = '".$_SESSION['projovemurbano']['ppuid']."'");
if($sugestaoampliacao['suaverdade']=="t") {
if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
}
//ver($meta);
}

$perfil = pegaPerfilGeral();

if($esdid != ESD_EMELABORACAO && !$db->testa_superuser()) {
$habilitado = 'N';
} elseif(in_array(PFL_CONSULTA, $perfil)){
$habilitado = 'N';
}else{
$habilitado = 'S';
}
//ver($_SESSION['projovemurbano']['muncod'],d);
?>
<script
	language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita">Orientações:</td>
		<td><font color=blue>
		<p>A seguir apresenta-se a meta final do termo de adesão para o
		exercício de 2012, bem como o período de matrícula e a data do início
		de aula.</p>
		</font></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Secretaria de Educação</td>
		<td>

		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" width="100%">
			<tr>
				<td class="SubTituloDireita" width="20%"><b>CNPJ</b></td>
				<td><? echo campo_texto('entnumcpfcnpj', 'S', $habilitado, 'CNPJ', 22, 19, "##.###.###/####-##", "", '', '', 0, 'id="entnumcpfcnpj"', '', (($secretaria['entnumcpfcnpj'])?mascaraglobal($secretaria['entnumcpfcnpj'],'##.###.###/####-##'):''), '' ); ?></td>
			
			
			<tr>
			
			
			<tr>
				<td class="SubTituloDireita" width="20%"><b>Esfera</b></td>
				<td><?=$esfera ?></td>
			
			
			<tr>
			
			
			<tr>
				<td class="SubTituloDireita" width="20%"><b>Estado</b></td>
				<td><?=$secretaria['estuf'] ?></td>
			
			
			<tr>
			
			
			<tr>
				<td class="SubTituloDireita" width="20%"><b>Município</b></td>
				<td><?=(($secretaria['mundescricao'])?$secretaria['mundescricao']:'-') ?></td>
			
			
			<tr>
		
		</table>

		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">&nbsp;</td>
		<td>

		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" width="100%">
			<tr>
				<td>Meta definida no termo de Adesão para o exercício de 2012</td>
				<td><?=$meta ?></td>
			
			
			<tr>
			
			
			<tr>
				<td>Data de abertura da matrícula</td>
				<td>15/03/2012</td>
			
			
			<tr>
			
			
			<tr>
				<td>Data de encerramento da matrícula</td>
				<td>15/06/2012</td>
			
			
			<tr>
			
			
			<tr>
				<td>Data de início das aulas</td>
				<td>18/06/2012</td>
			
			
			<tr>
		
		</table>

		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button"
			name="anterior" value="Anterior"
			onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel';">
		<input type="button" name="proximo" value="Próximo"
			onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo';"></td>
	</tr>
</table>
<? registarUltimoAcesso(); ?>
