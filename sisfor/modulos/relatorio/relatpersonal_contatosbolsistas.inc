<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sisfor.js"></script>

<p align="center" style="font-size:16px;"><b>Contato dos Bolsistas</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

?>
<form method="post" id="formulario" action="sisfor.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_contatosbolsistas.inc">
<input type="hidden" name="filtros" id="filtros" value="1">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<!-- 
<tr>
	<td class="SubTituloDireita">UF :</td>
	<td><?php 
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estufatuacao', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF5', '', '', '', 'S', 'estufatuacao', '');
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio5">
	<?
	if($muncodatuacao) {
		carregarMunicipiosPorUF(array('id'=>'muncod_abrangencia','name'=>'muncod_abrangencia','estuf'=>$estufatuacao,'valuecombo'=>$muncodatuacao));
	} else echo "Selecione uma UF";
	?>
	</td>
</tr>
 -->
<tr>
	<td class="SubTituloDireita">Perfil</td>
	<td><? 
	
	$sql = "SELECT p.pflcod as codigo, p.pfldsc as descricao FROM seguranca.perfil p
			WHERE p.pflcod IN(".PFL_COORDENADOR_INST.",
							  ".PFL_COORDENADOR_CURSO.",
							  ".PFL_COORDENADOR_ADJUNTO_IES.",
							  ".PFL_SUPERVISOR_IES.",
							  ".PFL_FORMADOR_IES.",
							  ".PFL_PROFESSOR_PESQUISADOR.",
							  ".PFL_TUTOR.")";
	
	$db->monta_combo('pflcod', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'pflcod', '',$_REQUEST['pflcod']);
	
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Ver HTML"><input type="button" value="Ver XLS" onclick="jQuery('#filtros').val('2');jQuery('#formulario').submit();"></td>
</tr>
</table>
</form>



<? 
if($_REQUEST['filtros']) : 

if($_REQUEST['estufatuacao']) {
	$wh[] = "m.estuf='".$_REQUEST['estufatuacao']."'";
}

if($_REQUEST['muncodatuacao']) {
	$wh[] = "m.muncod='".$_REQUEST['muncodatuacao']."'";
}

if($_REQUEST['pflcod']) {
	$wh[] = "t.pflcod='".$_REQUEST['pflcod']."'";
}

echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
echo '<tr>';
echo '<td>';

$sql = "SELECT DISTINCT s.unicod,
			   u.uniabrev||' - '||u.unidsc as universidade,
			   CASE WHEN s.ieoid IS NOT NULL THEN cur.curid  ||' - '|| cur.curdesc 
				    WHEN s.cnvid IS NOT NULL THEN cur2.curid ||' - '|| cur2.curdesc 
				    WHEN s.ocuid IS NOT NULL THEN oc.ocunome 
				    WHEN s.oatid IS NOT NULL THEN oatnome END as curso,
			   CASE WHEN s.ieoid IS NOT NULL THEN cor.coordsigla 
				    WHEN s.cnvid IS NOT NULL THEN cor2.coordsigla 
				    WHEN s.ocuid IS NOT NULL THEN cor3.coordsigla 
				    WHEN s.oatid IS NOT NULL THEN cor4.coordsigla END as secretaria,
			   replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
			   i.iusnome,
			   i.iusemailprincipal,
			   pe.pfldsc,
			   mun.estuf||' - '||mun.mundescricao as ufcidade,
			   CASE WHEN e.ientipo='R' THEN 'Residencial' 
					WHEN e.ientipo='P' THEN 'Profissional'
					WHEN e.ientipo='D' THEN 'Descanso' END as tipo,
		
  			   e.iencep,
  			   e.iencomplemento,
  			   e.iennumero,
			   e.ienlogradouro,
  			   e.ienbairro,
			   (SELECT '( '||itedddtel||' ) '||itenumtel as telefone FROM sisfor.identificacaotelefone WHERE iusd=i.iusd AND itetipo='C') as telefonecomercial, 
			   (SELECT '( '||itedddtel||' ) '||itenumtel as telefone FROM sisfor.identificacaotelefone WHERE iusd=i.iusd AND itetipo='R') as telefoneresidencial
		FROM sisfor.identificacaousuario i
		INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN seguranca.perfil pe ON pe.pflcod = t.pflcod
		INNER JOIN sisfor.sisfor s ON s.sifid = t.sifid 
		INNER JOIN public.unidade u ON u.unicod = s.unicod
		
LEFT JOIN catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
LEFT JOIN catalogocurso2014.curso cur ON cur.curid = ieo.curid
LEFT JOIN catalogocurso2014.coordenacao cor ON cor.coordid = cur.coordid
LEFT JOIN sisfor.cursONaovinculado cnv ON cnv.cnvid = s.cnvid
LEFT JOIN catalogocurso2014.curso cur2 ON cur2.curid = cnv.curid
LEFT JOIN catalogocurso2014.coordenacao cor2 ON cor2.coordid = cur2.coordid
LEFT JOIN seguranca.usuario usu ON usu.usucpf = s.usucpf
LEFT JOIN sisfor.outrocurso oc ON oc.ocuid = s.ocuid
LEFT JOIN catalogocurso2014.coordenacao cor3 ON cor3.coordid = oc.coordid 
LEFT JOIN sisfor.outraatividade oat ON oat.oatid = s.oatid 
LEFT JOIN catalogocurso2014.coordenacao cor4 ON cor4.coordid = oat.coordid 
		
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
		LEFT JOIN sisfor.identificaoendereco e ON e.iusd = i.iusd 
		LEFT JOIN territorios.municipio mun ON mun.muncod = e.muncod  
		WHERE i.iusstatus='A' ".(($wh)?"AND ".implode(" AND ",$wh):"");

$cabecalho = array("UO","IES","Curso","Secretaria","CPF","Nome","E-mail","Perfil","UF - Município","Tipo endereço","CEP","Complemento","Número","Logradouro","Bairro","Telefone Comercial","Telefone Residencial");

if($_REQUEST['filtros']=='2') {

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_contatos_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_contatos_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000000,5,'N','100%','');
	exit;

} else {
	$db->monta_lista_simples($sql,$cabecalho,100000000,5,5,'N','100%','N');
}

echo '</td>';
echo '</tr>';
echo '</table>';


endif; 
?>