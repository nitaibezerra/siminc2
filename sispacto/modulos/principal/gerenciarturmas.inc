<?
verificarTermoCompromisso(array("iusd"=>$_SESSION['sispacto'][$sis]['iusd'],"sis"=>$sis,"pflcod"=>$pflcod_gerenciador));

if(!$_SESSION['sispacto'][$sis]['uncid']) {
	corrigirAcessoUniversidade(array('sis' => $sis));
}
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>

function selecionarTurmasTroca(turid) {
	window.location=window.location+'&aba=gerenciarturmas&turid='+turid;
}

function efetuarTroca() {
	var conf = confirm('Deseja realmente efetuar as trocas de turmas?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "uncid");
		input.setAttribute("value", "<?=$_SESSION['sispacto'][$sis]['uncid'] ?>");
		document.getElementById("formtrocar").appendChild(input);

		document.getElementById("formtrocar").submit();
	} 
}

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Gerenciar Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto/sispacto.php?modulo=principal/{$sis}/".(($sis=='universidade')?'universidadeexecucao':(($sis=='coordenadorlocal')?'coordenadorlocalexecucao':$sis))."&acao=A&aba=gerenciarturmas"); ?></td>
</tr>
<tr>
	<td colspan="2" valign="top">
	<form method=post name="formtrocar" id="formtrocar">
	<input type="hidden" name="requisicao" value="trocarTurmas">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<? if($turformadores) : ?>
    <tr>
    	<td class=SubTituloDireita>Turmas de Formadores:</td>
    	<td>
    	<? 
	    $sql = "(
	    		SELECT turid as codigo, uni.unisigla || ' / ' || uni.uninome || ' - ' || i.iusnome || ' ( '||tu.turdesc||' )' as descricao 
	    		FROM sispacto.turmas tu 
	    		INNER JOIN sispacto.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto.universidadecadastro unc ON unc.uncid = tu.uncid 
	    		INNER JOIN sispacto.universidade uni ON uni.uniid = unc.uniid 
	    		WHERE pflcod='".PFL_FORMADORIES."' AND unc.uncid='".$_SESSION['sispacto'][$sis]['uncid']."' ORDER BY descricao
	    		) UNION ALL (
				SELECT t.turid as codigo, uni.unisigla || ' / ' || uni.uninome || ' - ' || i.iusnome || ' ( '||t.turdesc||' ) - EXTINTA' as descricao  
				FROM sispacto.orientadorturma ot 
				INNER JOIN sispacto.tipoperfil tp ON tp.iusd = ot.iusd AND tp.pflcod=".PFL_ORIENTADORESTUDO."
				INNER JOIN sispacto.turmas t ON t.turid = ot.turid 
				INNER JOIN sispacto.universidadecadastro unc ON unc.uncid = t.uncid 
				INNER JOIN sispacto.universidade uni ON uni.uniid = unc.uniid 
				INNER JOIN sispacto.identificacaousuario i ON i.iusd = t.iusd
				LEFT JOIN sispacto.tipoperfil tp2 ON tp2.iusd = t.iusd 
				WHERE tp2.tpeid IS NULL AND t.uncid='".$_SESSION['sispacto'][$sis]['uncid']."' 
				GROUP BY t.turid, uni.unisigla, uni.uninome, i.iusnome, t.turdesc
	    		)";
	    
	    $db->monta_combo('turid_formador', $sql, 'S', 'Selecione', 'selecionarTurmasTroca', '', '', '', 'S', 'turid_formador','', $_REQUEST['turid']); 
	    ?> 
	    </td>
	</tr>
	<? endif; ?>
	<? if($turorientadores) : ?>
    <tr><td class=SubTituloDireita>Turmas de Orientadores de Estudo:</td><td><? 
    
    $sql = "(
    		SELECT turid as codigo, CASE WHEN pt.muncod IS NOT NULL THEN 'Municipal ' || mun.estuf || ' / ' || mun.mundescricao 
    								     WHEN pt.estuf IS NOT NULL THEN 'Estadual ' || est.estuf || ' / ' || est.estdescricao
    									 END || ' - ' || i.iusnome || ' ( '||tu.turdesc||' )' as descricao 
    		FROM sispacto.turmas tu 
    		INNER JOIN sispacto.identificacaousuario i ON i.iusd = tu.iusd 
    		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
    		INNER JOIN sispacto.pactoidadecerta pt ON pt.picid = tu.picid 
    		LEFT JOIN territorios.municipio mun ON mun.muncod = pt.muncod 
    		LEFT JOIN territorios.estado est ON est.estuf = pt.estuf  
    		WHERE pflcod='".PFL_ORIENTADORESTUDO."' {$filtro_esfera} ORDER BY descricao
    		) UNION ALL (
			SELECT tu.turid as codigo, CASE WHEN pt.muncod IS NOT NULL THEN 'Municipal ' || mun.estuf || ' / ' || mun.mundescricao 
    								     WHEN pt.estuf IS NOT NULL THEN 'Estadual ' || est.estuf || ' / ' || est.estdescricao
    									 END || ' - X : ' || i.iusnome || ' ( '||tu.turdesc||' )' as descricao  
			FROM sispacto.identificacaousuario i 
			LEFT JOIN sispacto.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN sispacto.turmas tu ON tu.iusd = i.iusd AND tu.picid = i.picid 
    		INNER JOIN sispacto.pactoidadecerta pt ON pt.picid = tu.picid 
    		LEFT JOIN territorios.municipio mun ON mun.muncod = pt.muncod 
    		LEFT JOIN territorios.estado est ON est.estuf = pt.estuf  
    		WHERE (t.pflcod!='".PFL_ORIENTADORESTUDO."' OR t.pflcod IS NULL) {$filtro_esfera} ORDER BY descricao
			
    		)";
    
    $db->monta_combo('turid_orientador', $sql, 'S', 'Selecione', 'selecionarTurmasTroca', '', '', '', 'S', 'turid_orientador','', $_REQUEST['turid']); ?> 
    </td></tr>
    <? endif; ?>
    <tr>
    	<td colspan="2">
    	<? 
    	if($_REQUEST['turid']) :
    	
    		echo "<input type=hidden name=\"turidantigo\" value=\"".$_REQUEST['turid']."\">";
    	
    		$sql = "SELECT t.turid, t.turdesc, i.iusnome FROM sispacto.turmas t
    				INNER JOIN sispacto.identificacaousuario i ON i.iusd = t.iusd 
    				WHERE turid!='".$_REQUEST['turid']."' AND (t.picid IN(SELECT picid FROM sispacto.turmas WHERE turid='".$_REQUEST['turid']."') OR t.uncid IN(SELECT uncid FROM sispacto.turmas WHERE turid='".$_REQUEST['turid']."')) 
    				ORDER BY t.turid";
    		
    		$turmas_opcoes = $db->carregar($sql);
    		
    		$consulta = verificaPermissao();
    		
    		if($turmas_opcoes[0]) {
    			$html .= "<select name=troca['||ius.iusd||'] class=CampoEstilo style=width:auto; ".(($consulta)?"disabled":"").">";
    			$html .= "<option value=\"\">Selecione</option>";
    			foreach($turmas_opcoes as $tuo) {
    				$html .= "<option value=".$tuo['turid'].">".$tuo['turdesc']." ( ".str_replace(array("'"),array(" "),$tuo['iusnome'])." )</option>";
    			}
    			$html .= "</select>";
    		}

    		$sql = "SELECT ius.iuscpf, ius.iusnome, p.pfldsc, tur.turdesc, '{$html}' FROM sispacto.orientadorturma ot 
    				INNER JOIN sispacto.turmas tur ON tur.turid = ot.turid  
    				INNER JOIN sispacto.identificacaousuario ius ON ius.iusd = ot.iusd 
    				INNER JOIN sispacto.tipoperfil t ON t.iusd = ius.iusd 
    				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
    				WHERE ot.turid='".$_REQUEST['turid']."'";
    		
    		$cabecalho = array("CPF","Nome","Perfil","Turma Atual","Turma Destinado");
    		$db->monta_lista_simples($sql,$cabecalho,2000,10,'N','100%','N',$totalregistro=false , $arrHeighTds = false , $heightTBody = false, $boImprimiTotal = true);
    	
    	endif; 
    	?>
    	</td>
    </tr>
	<tr>
	<td class="SubTituloCentro" colspan="2">
		<? if(!$consulta) : ?>
		<input type="button" name="buscar" value="Efetuar Troca" onclick="efetuarTroca();">
		<? endif; ?>
	</td>
	</tr>
	</table>
	</form>

	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	<? if($goto_pro) : ?>
	<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
	<? endif; ?>
	</td>
</tr>
</table>