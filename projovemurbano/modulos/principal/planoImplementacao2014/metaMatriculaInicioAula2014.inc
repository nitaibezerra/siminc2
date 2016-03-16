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
// 	if( $_SESSION['projovemurbano']['ppuid'] == '3' ){
	$sql = "SELECT adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']}";
    	
    	$ajustado = $db->pegaUm($sql);
	
    	if( $ajustado == 't' ){
    		$not = ",7,10,13";
    	}else{
    		$not = ",9,12,15";
    	}
	
		$sql = "SELECT
					mtp.mtpvalor as valor,
					mtp.tpmid as tipo
				FROM
	                projovemurbano.tipoprograma tpr
	            INNER JOIN projovemurbano.tipoprograma_programaprojovemurbano nn ON nn.tprid = tpr.tprid
	            INNER JOIN projovemurbano.tipometadoprograma tmp ON tmp.tprid = nn.tprid
	            INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.tpmid = tmp.tpmid
	            WHERE
	                nn.ppuid != 1
	                AND mtp.pjuid = {$_SESSION['projovemurbano']['pjuid']}
	                AND mtpvalor > 0
	                AND tmp.tpmid NOT IN  (2,5,8,11,14$not)
	                AND tpr.tprid = {$_SESSION['projovemurbano']['tprid']}";
		
		$meta = $db->pegaUm($sql);
		
// 	}else{
// 		$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='".$_SESSION['projovemurbano']['estuf']."'");		
// 		if($sugestaoampliacao['suaverdade']=="t") {
// 			if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
// 		}
// 	}
	
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
// 	if( $_SESSION['projovemurbano']['ppuid'] == '3' ){
	
    	$sql = "SELECT adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']}";
    	
    	$ajustado = $db->pegaUm($sql);

    	if( $ajustado == 't' ){
    		$not = ",7,10,13";
    	}else{
    		$not = ",9,12,15";
    	}
	
		$sql = "SELECT
					mtpvalor as valor,
					mtp.tpmid as tipo
				FROM
	                projovemurbano.tipoprograma tpr
	            INNER JOIN projovemurbano.tipoprograma_programaprojovemurbano nn ON nn.tprid = tpr.tprid
	            INNER JOIN projovemurbano.tipometadoprograma tmp ON tmp.tprid = nn.tprid
	            INNER JOIN projovemurbano.metasdoprograma mtp ON mtp.tpmid = tmp.tpmid
	            WHERE
	                nn.ppuid != 1
	                AND mtp.pjuid = {$_SESSION['projovemurbano']['pjuid']}
	                AND mtpvalor > 0
	                AND tmp.tpmid NOT IN  (2,5,8,11,14$not)
	                AND tpr.tprid = {$_SESSION['projovemurbano']['tprid']}";
		
		$meta = $db->pegaUm($sql);
		
// 	}else{
// 		$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='".$_SESSION['projovemurbano']['muncod']."'");		
// 		if($sugestaoampliacao['suaverdade']=="t") {
// 			if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
// 		}
// 	}
	
}

$perfil = pegaPerfilGeral();

if($esdid != ESD_EMELABORACAO && !$db->testa_superuser()) {
	$habilitado = 'N';
} elseif(in_array(PFL_CONSULTA, $perfil)){
	$habilitado = 'N';
}else{
	$habilitado = 'S';
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Orientações:</td>
		<td>
		<font color=blue>
			<font color=blue>
			<p>O Plano de Implementação é um instrumento de apoio à gestão local que baliza a utilização de recursos pelo EEx, 
				conforme os critérios estabelecidos na Resolução 08/2014 de 16 de abril de 2014, não condicionando o início das atividades ou da utilização de recursos a aprovação da SECADI/MEC.</p>
			<p>Somente o Secretário de Educação ou o Coordenador Geral indicado poderá desenvolver o Plano de Implementação que deverá ser validado apenas pelo Secretário de Educação.</p>
			<p>A assinatura do Secretário de Educação será a única aceita na versão final do Plano de Implementação e deverá ser impressa para envio à DPEJUV/SECADI/MEC.</p>
			</font>
		</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Secretaria de Educação</td>
		<td>
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
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
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="100%">
		<tr>
			<td>Meta definida no termo de Adesão para o exercício de 2014</td>
			<td><?=$meta ?></td>
		<tr>
		<tr>
			<td>Data de abertura da matrícula</td>
			<td></td>
		<tr>
		<tr>
			<td>Data de encerramento da matrícula</td>
			<td></td>
		<tr>
		<tr>
			<td>Data de início das aulas</td>
			<td></td>
		<tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="proximo" value="Próximo" 
				   onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba=<?=$_GET['aba'] ?>&aba2=profissionais';">
		</td>
	</tr>
</table>
<? registarUltimoAcesso(); ?>
