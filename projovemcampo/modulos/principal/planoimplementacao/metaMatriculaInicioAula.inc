<?php
if($_SESSION['projovemcampo']['estuf']) {
	
	$esfera = "Federal";
	
	$sql = "SELECT DISTINCT
				COALESCE(sec.secacnpj,'-') as entnumcpfcnpj, est.estuf, est.estcod, apc.*
			FROM 
				projovemcampo.secretaria sec
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = sec.secaid
			INNER JOIN territorios.estado est ON est.estcod = apc.apccodibge::character(2)
			WHERE 
				est.estuf='".$_SESSION['projovemcampo']['estuf']."' AND apc.apcesfera = 'E'";
	
	$secretaria = $db->pegaLinha($sql);
}
if($_SESSION['projovemcampo']['muncod']) {
	
	$esfera = "Municipal";
	
	$sql = "SELECT DISTINCT
				COALESCE(sec.secacnpj,'-') as entnumcpfcnpj, mun.estuf, mun.mundescricao
			FROM 
				projovemcampo.secretaria sec
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = sec.secaid
			INNER JOIN territorios.municipio mun ON mun.muncod = apc.apccodibge::character(7)
			WHERE 
				mun.muncod='".$_SESSION['projovemcampo']['muncod']."' AND apc.apcesfera = 'M'";
	
	$secretaria = $db->pegaLinha($sql);
}

$meta = pegameta();
$perfil = pegaPerfilGeral();

if($esdid != ESD_EMELABORACAO && !$db->testa_superuser()) {
	$habilitado = 'N';
} elseif(in_array(PFL_CONSULTA, $perfil)){
	$habilitado = 'N';
}else{
	$habilitado = 'N';
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Orientações:</td>
		<td>
		<font color=blue>
			<p>O Plano de Implementação é um instrumento de apoio à gestão local que baliza a utilização de recursos pelo EEx,conforme critérios estabelecidos na Resolução CD?FNDE n° 11 de 16 de abril de 2014, não condicionado o início das atividades ou utilização dos recursos a aprovalção da SECADI/MEC.</p>
			<p>Somente o Secretário de Educação ou o Coordenador Geral indicado poderá desenvolver o Plano de Implementação que deverá ser validado apenas pelo Secretário de Educação.</p>
			<p>A assinatura do Secretário de educação será a única aceita na versão final do Plano de Implementação e deverá ser impressa para envio à DPEJUV/SECADI/MEC</p>
			<p>A seguir apresenta-se a meta final do termo de adesão para o exercício de 2014, bem como o período de matrícula e a data do início de aula.</p>
		</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Secretaria de Educação</td>
		<td>
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
		<tr>
			<td class="SubTituloDireita" width="20%"><b>CNPJ</b></td>
			<td><?=mascaraglobal($secretaria['entnumcpfcnpj'],'##.###.###/####-##')
// 				echo campo_texto('entnumcpfcnpj', 'N', $habilitado, 'CNPJ', 22, 19, "##.###.###/####-##", "", '', '', 0, 'id="entnumcpfcnpj"', '', (($secretaria['entnumcpfcnpj'])?mascaraglobal($secretaria['entnumcpfcnpj'],'##.###.###/####-##'):''), '' ); 
				?>
			</td>
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
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
		<tr>
			<td>Meta definida no termo de Adesão para o exercício de 2014</td>
			<td><?=$meta ?></td>
		<tr>
		<tr>
			<td>Data de abertura da matrícula</td>
			<td>--</td>
		<tr>
		<tr>
			<td>Data de encerramento da matrícula</td>
			<td>--</td>
		<tr>
		<tr>
			<td>Data de início das aulas</td>
			<td>--</td>
		<tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="proximo" value="Próximo" 
				   onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais';">
		</td>
	</tr>
</table>
<? 
 registarUltimoAcesso(); ?>
