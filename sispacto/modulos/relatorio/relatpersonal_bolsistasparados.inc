<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório de Bolsistas sem tramitação de pagamento</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

if(!$_REQUEST['dias']) $_REQUEST['dias'] = "45";

echo '<p> Situação pagamento : ';
$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_PAGAMENTOBOLSA."' AND esdstatus='A' ORDER BY esdordem";
$db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdid');
echo '</p>';

echo '<p>Dias sem tramitação : <input type="text" class="normal" name="dias" id="dias" onkeyup="this.value=mascaraglobal(\'########\',this.value);" value="'.$_REQUEST['dias'].'"></p>';
echo '<p align="center"><input type="button" value="Aplicar" onclick="window.location=\'sispacto.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_bolsistasparados.inc&esdid=\'+document.getElementById(\'esdid\').value+\'&dias=\'+document.getElementById(\'dias\').value;"></p>';

if($_REQUEST['esdid']) {
	$f_esd = "AND d.esdid='".$_REQUEST['esdid']."'";
}

$sql = "SELECT p.pboid, replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, i.iusnome, l.pfldsc, e.esddsc, to_char(h.htddata,'dd/mm/YYYY HH24:MI') as htddata, 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as periodo, u.uninome 
FROM sispacto.pagamentobolsista p 
INNER JOIN sispacto.universidade u ON u.uniid = p.uniid 
INNER JOIN sispacto.identificacaousuario i ON i.iusd = p.iusd 
INNER JOIN seguranca.perfil l ON l.pflcod = p.pflcod 
INNER JOIN sispacto.folhapagamento f ON f.fpbid = p.fpbid 
INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
INNER JOIN workflow.documento d ON d.docid = p.docid AND d.tpdid=87
INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
INNER JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
WHERE NOW() > h.htddata + interval '".$_REQUEST['dias']." days' AND d.esdid NOT IN(664,661) {$f_esd}
ORDER BY h.htddata";

$cabecalho = array("ID Pagamento","CPF","Nome","Perfil","Situação","Última tramitação","Período","Universidade");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);

?>