<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatórios sobre o desempenho das redes estaduais e municipais (físico e financeiro)</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */


$sql = "SELECT estuf, 
			   mundescricao, 
			   muncod, 
			   (select count(*) from sispacto.identificacaousuario i inner join sispacto.pactoidadecerta p on p.picid = i.picid inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=".PFL_PROFESSORALFABETIZADOR." where p.muncod is not null and i.muncodatuacao=m.muncod) as qtdprofmun, 
			   (select count(*) from sispacto.identificacaousuario i inner join sispacto.pactoidadecerta p on p.picid = i.picid inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=".PFL_PROFESSORALFABETIZADOR." where p.estuf is not null and i.muncodatuacao=m.muncod) as qtdprofest,
			   (select count(*) from sispacto.pagamentobolsista pa inner join sispacto.identificacaousuario i on i.iusd = pa.iusd inner join sispacto.pactoidadecerta p on p.picid = i.picid where pa.pflcod=".PFL_PROFESSORALFABETIZADOR." and p.muncod is not null and i.muncodatuacao=m.muncod) as qtdbolsasprofmun,
			   (select count(*) from sispacto.pagamentobolsista pa inner join sispacto.identificacaousuario i on i.iusd = pa.iusd inner join sispacto.pactoidadecerta p on p.picid = i.picid inner join workflow.documento d on d.docid = pa.docid where d.esdid=".ESD_PAGAMENTO_EFETIVADO." and pa.pflcod=".PFL_PROFESSORALFABETIZADOR." and p.muncod is not null and i.muncodatuacao=m.muncod) as qtdbolsaspgprofmun,
			   (select count(*) from sispacto.pagamentobolsista pa inner join sispacto.identificacaousuario i on i.iusd = pa.iusd inner join sispacto.pactoidadecerta p on p.picid = i.picid where pa.pflcod=".PFL_PROFESSORALFABETIZADOR." and p.estuf is not null and i.muncodatuacao=m.muncod) as qtdbolsasprofest,
			   (select count(*) from sispacto.pagamentobolsista pa inner join sispacto.identificacaousuario i on i.iusd = pa.iusd inner join sispacto.pactoidadecerta p on p.picid = i.picid inner join workflow.documento d on d.docid = pa.docid where d.esdid=".ESD_PAGAMENTO_EFETIVADO." and pa.pflcod=".PFL_PROFESSORALFABETIZADOR." and p.estuf is not null and i.muncodatuacao=m.muncod) as qtdbolsaspgprofest,
			   (select count(*) from sispacto.identificacaousuario i inner join sispacto.pactoidadecerta p on p.picid = i.picid inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=".PFL_ORIENTADORESTUDO." where p.muncod is not null and i.muncodatuacao=m.muncod) as qtdorientadormun, 
			   (select count(*) from sispacto.identificacaousuario i inner join sispacto.pactoidadecerta p on p.picid = i.picid inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=".PFL_ORIENTADORESTUDO." where p.estuf is not null and i.muncodatuacao=m.muncod) as qtdorientadorest,
			   (select count(*) from sispacto.pagamentobolsista pa inner join sispacto.identificacaousuario i on i.iusd = pa.iusd inner join sispacto.pactoidadecerta p on p.picid = i.picid where pa.pflcod=".PFL_ORIENTADORESTUDO." and p.muncod is not null and i.muncodatuacao=m.muncod) as qtdbolsasorientadormun,
			   (select count(*) from sispacto.pagamentobolsista pa inner join sispacto.identificacaousuario i on i.iusd = pa.iusd inner join sispacto.pactoidadecerta p on p.picid = i.picid inner join workflow.documento d on d.docid = pa.docid where d.esdid=".ESD_PAGAMENTO_EFETIVADO." and pa.pflcod=".PFL_ORIENTADORESTUDO." and p.muncod is not null and i.muncodatuacao=m.muncod) as qtdbolsaspgorientadormun,
			   (select count(*) from sispacto.pagamentobolsista pa inner join sispacto.identificacaousuario i on i.iusd = pa.iusd inner join sispacto.pactoidadecerta p on p.picid = i.picid where pa.pflcod=".PFL_ORIENTADORESTUDO." and p.estuf is not null and i.muncodatuacao=m.muncod) as qtdbolsasorientadorest,
			   (select count(*) from sispacto.pagamentobolsista pa inner join sispacto.identificacaousuario i on i.iusd = pa.iusd inner join sispacto.pactoidadecerta p on p.picid = i.picid inner join workflow.documento d on d.docid = pa.docid where d.esdid=".ESD_PAGAMENTO_EFETIVADO." and pa.pflcod=".PFL_ORIENTADORESTUDO." and p.estuf is not null and i.muncodatuacao=m.muncod) as qtdbolsaspgorientadorest
		FROM territorios.municipio m";



$cabecalho = array("UF","Município","Cód do Município","Número de PA da Rede Municipal","Número de PA da Rede Estadual","Número de Bolsas de PA da Rede Municipal","Valor das Bolsas efetivamente pagas para PA da Rede Municipal","Número de Bolsas de PA da Rede Estadual","Valor das Bolsas efetivamente pagas para PA da Rede Estadual","Número de OE da Rede Municipal","Número de OE da Rede Estadual","Número de Bolsas de OE da Rede Municipal","Valor das Bolsas efetivamente pagas para OE da Rede Municipal","Número de Bolsas de OE da Rede Estadual","Valor das Bolsas efetivamente pagas para OE da Rede Estadual");

if($_REQUEST['buscar']=='2') {
	
	ob_clean();
	
	header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
	header("Pragma: no-cache");
	header("Content-type: application/xls; name=SIMEC_Relatorio_desempenho_".date("Ymdhis").".xls");
	header("Content-Disposition: attachment; filename=SIMEC_Relatorio_desempenho_".date("Ymdhis").".xls");
	header("Content-Description: MID Gera excel");
	
	$db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%', '');

	
} else {
	
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%','',true, false, false, true);
		
}






?>
<p align="center"><input type="button" name="gerarxls" value="Gerar XLS" onclick="window.location='sispacto.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=2&relatorio=relatpersonal_desempenho.inc';"></p>