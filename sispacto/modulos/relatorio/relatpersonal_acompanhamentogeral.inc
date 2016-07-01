<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Acompanhamento Geral</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

if(in_array(PFL_COORDENADORIES,$perfis)) {
	
	$universidade = $db->pegaLinha("SELECT u.uncid, uu.unisigla||' / '||uu.uninome as universidade FROM sispacto.identificacaousuario i 
								 	INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
								 	INNER JOIN sispacto.universidadecadastro u ON u.uncid = i.uncid 
								 	INNER JOIN sispacto.universidade uu ON uu.uniid = u.uniid  
								 	WHERE i.iusd='".$_SESSION['sispacto']['universidade']['iusd']."'");
	
}

?>
<body>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td class="SubTituloDireita">Universidade : </td>
	<td>
	<?
	if($universidade) {

		echo "<input type=hidden name=uncid id=uncid value=\"".$universidade['uncid']."\">".$universidade['universidade'];
		$_REQUEST['uncid'] = $universidade['uncid'];

	} else {

		$sql = "SELECT u.uncid as codigo, uu.unisigla||' / '||uu.uninome as descricao FROM sispacto.universidadecadastro u 
				inner join sispacto.universidade uu on uu.uniid = u.uniid";
		
		$db->monta_combo('uncid', $sql, 'S', 'TODOS', '', '', '', '', 'N', 'uncid', '', $_REQUEST['uncid']);
		
	}
	?>
	</td>
</tr>
</table>
<?

if(!$universidade) echo '<p align="center"><input type="button" value="Visualizar" id="btnvisualizar" onclick="window.location=\'sispacto.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_acompanhamentogeral.inc&uncid=\'+document.getElementById(\'uncid\').value;this.disabled=true;"> <input type=button value="Limpar" onclick="window.location=\'sispacto.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_acompanhamentogeral.inc\';"></p>';


if($_REQUEST['uncid']) :
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<tr>
	<td>
	<?
	$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao
			FROM sispacto.folhapagamento f
			INNER JOIN sispacto.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE f.fpbstatus='A' AND rf.uncid='".$_REQUEST['uncid']."' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'01')::date,'YYYYmmdd')";
	
	$periodos = $db->carregar($sql);
	
	if($periodos[0]) {
		foreach($periodos as $p) {
			
			echo "<p><b>>>".$p['descricao']."</b></p>";

			$sql = "SELECT
							  replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf,
							   foo.iusnome,
							   foo.pfldsc,
							   foo.rede,
							   ".criteriosAprovacao('restricao4')."
						FROM (
						SELECT  m.menid,
								m.iusd,
								i.iuscpf,
								i.iusnome,
								i.iustermocompromisso,
								p.pfldsc,
								p.pflcod,
								e.esdid,
								i.iusdocumento,
								i.iustipoprofessor,
								i.iusnaodesejosubstituirbolsa,
								d.docid,
								(SELECT COUNT(mapid) FROM sispacto.materiaisprofessores mp WHERE mp.iusd=m.iusd) as totalmateriaisprofessores,
								(SELECT COUNT(*) FROM sispacto.turmasprofessoresalfabetizadores pa WHERE tpastatus='A' AND (coalesce(tpatotalmeninos,0)+coalesce(tpatotalmeninas,0))!=0 AND pa.iusd=m.iusd) as totalturmas,
								(SELECT COUNT(mavid) FROM sispacto.mensarioavaliacoes ma  WHERE ma.menid=m.menid AND ma.mavfrequencia=0) as numeroausencia,
								(SELECT COUNT(*) FROM sispacto.gestaomobilizacaoperguntas gm WHERE gm.iusd=m.iusd) as rcoordenadorlocal,
								(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens
								 FROM sispacto.aprendizagemconhecimentoturma a
								 INNER JOIN sispacto.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid
								 WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND t.iusd=m.iusd) as aprendizagem,
								(SELECT COUNT(mavid) FROM sispacto.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
								(SELECT COUNT(DISTINCT fpbid) FROM sispacto.mensario me  WHERE me.iusd=m.iusd) as numerogeralavaliacoes,
								COALESCE((SELECT AVG(mavtotal) FROM sispacto.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
								COALESCE((SELECT AVG(iccvalor) FROM sispacto.respostasavaliacaocomplementar r INNER JOIN sispacto.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid WHERE r.iusdavaliado=i.iusd AND r.fpbid = m.fpbid),0.00) as notacomplementar,
								m.fpbid,
								t.fpbidini,
								t.fpbidfim,
								pp.plpmaximobolsas,
								CASE WHEN pic.picid IS NOT NULL THEN
																	CASE WHEN pic.muncod IS NOT NULL THEN m1.estuf||'/'||m1.mundescricao||' ( Municipal )'
																		 WHEN pic.estuf IS NOT NULL THEN m2.estuf||'/'||m2.mundescricao||' ( Estadual )'
																	END
									 ELSE 'Equipe IES' END as rede
						FROM sispacto.mensario m
						INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO."
						INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid
						INNER JOIN sispacto.identificacaousuario i ON i.iusd = m.iusd
						INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
						INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
						LEFT JOIN sispacto.pagamentoperfil pp ON pp.pflcod = p.pflcod
						LEFT JOIN sispacto.pactoidadecerta pic ON pic.picid = i.picid
						LEFT JOIN territorios.municipio m1 ON m1.muncod = pic.muncod
						LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao
						WHERE d.esdid NOT IN('".ESD_APROVADO_MENSARIO."','".ESD_INVALIDADO_MENSARIO."') AND i.uncid='".$_REQUEST['uncid']."' AND m.fpbid='".$p['codigo']."' ".(($_REQUEST['iusnome_f'])?"AND i.iusnome ILIKE '".$_REQUEST['iusnome_f']."%'":"")." ".(($_REQUEST['iuscpf_f'])?"AND i.iuscpf ILIKE '".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf_f'])."%'":"")."
						) foo
						ORDER BY foo.iusnome ASC
						";
			
			$cabecalho = array("CPF","Nome","Perfil","Rede","Restrições");
			$db->monta_lista($sql,$cabecalho,100000,5,'N','center','N','formulario','','',null,array('ordena'=>false));

			
		}
		
	}
	
	

	?>
	</td>

</tr>

</table>
<?
else :
 
echo '<p align=center>Selecione a universidade</p>';

endif;

?>
</body>