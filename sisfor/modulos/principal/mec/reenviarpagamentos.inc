<?
$goto_ant = 'sisfor.php?modulo=principal/mec/mec&acao=A&aba=aprovarnomes';
?>
<script>
function reenviarPagamentos() {

	var conf = confirm('Deseja realmente reenviar pagamentos selecionados?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "requisicao");
		input.setAttribute("value", "reenviarPagamentos");
		document.getElementById("formreenviarpagamentos").appendChild(input);
		document.getElementById("formreenviarpagamentos").submit();
	}

}

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Orientações</td>
	<td colspan="2">
	[ORIENTACOES]
	</td>
</tr>
<tr>
	<td colspan="3">
	<? if(!$consulta) : ?>
	<form method="post" name="formbuscar" id="formbuscar">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
    	<td class="SubTituloDireita">CPF</td>
    	<td><?=campo_texto('iuscpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iuscpf']); ?></td>
    </tr>
    <tr>
	    <td class="SubTituloDireita">Nome</td>
	    <td><?=campo_texto('iusnome', "N", "S", "Nome", 67, 60, "", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iusnome']); ?></td>
    </tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="buscar" value="Buscar" onclick="document.getElementById('formbuscar').submit();">
		<input type="button" name="limpar" value="Limpar" onclick="window.location=window.location;">
		</td>
	</tr>
	</table>
	</form>
	<? else : ?>
	Perfil sem permissão para acessar esta TELA
	<? endif; ?>
<?

if($_REQUEST['iuscpf']) {
	$wh[] = "i.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'";
}

if($_REQUEST['iusnome']) {
	$wh[] = "i.iusnome ilike '".$_REQUEST['iusnome']."'";
}

$perfis = pegaPerfilGeral();
if(!$perfil) $perfil = array();

if(in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis) || in_array(PFL_DIRETOR_MEC,$perfis)) {
	$coordids = $db->carregarColuna("SELECT DISTINCT coordid FROM sisfor.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod IN('".PFL_EQUIPE_MEC."','".PFL_COORDENADOR_MEC."','".PFL_DIRETOR_MEC."') ANd rpustatus='A'");

	if($coordids)
		$wh[] = "(cur.coordid IN(".implode(",",$coordids).") OR cur2.coordid IN(".implode(",",$coordids).") OR oc.coordid IN(".implode(",",$coordids).") OR cor4.coordid IN(".implode(",",$coordids)."))";

}


if($wh) {

	$sql = "SELECT '<input type=\"checkbox\" name=\"doc[]\" value=\"'||p.docid||'\">' as chk, 
					trim(replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')) as iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pbovlrpagamento, 
					uniabrev||' - '||unidsc as universidade, 
					case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
					     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
					     when s.ocuid is not null then oc.ocunome 
					     when s.oatid is not null then oatnome end as curso,
					'Ref. ' || m.mesdsc || ' / ' || f.fpbanoreferencia as periodo,
					e.esddsc 
			FROM sisfor.identificacaousuario i 
			INNER JOIN sisfor.pagamentobolsista p ON p.iusd = i.iusd 
			INNER JOIN sisfor.tipoperfil t ON t.tpeid = p.tpeid 
			INNER JOIN sisfor.sisfor s ON s.sifid = t.sifid 
				 
INNER JOIN public.unidade u on u.unicod = s.unicod
LEFT JOIN catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
LEFT JOIN catalogocurso2014.curso cur on cur.curid = ieo.curid
LEFT JOIN catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
LEFT JOIN sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
LEFT JOIN catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
LEFT JOIN catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
LEFT JOIN seguranca.usuario usu on usu.usucpf = s.usucpf
LEFT JOIN sisfor.outrocurso oc on oc.ocuid = s.ocuid
LEFT JOIN catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
LEFT JOIN sisfor.outraatividade oat on oat.oatid = s.oatid 
LEFT JOIN catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid
				
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			INNER JOIN sisfor.folhapagamento f ON f.fpbid = p.fpbid
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE d.esdid IN(".ESD_PAGAMENTO_NAO_AUTORIZADO.") ".(($wh)?"AND ".implode(" AND ",$wh):"")."
			ORDER BY iuscpf, f.fpbid";
	
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","R$ da bolsa","Universidade","Curso","Período","Situação");
	$db->monta_lista($sql,$cabecalho,10000,5,'N','center','N',"formreenviarpagamentos");
	
	echo "<p align=\"center\"><input type=\"button\" value=\"Reenviar pagamento\" name=\"reenviar\"  onclick=\"reenviarPagamentos();\"></p>";

} else {
	echo "<p align=center><b>Por favor inserir filtro de pesquisa (CPF/Nome)</b></p>";
}

?>
	</td>
</tr>


<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	</td>
</tr>


</table>
