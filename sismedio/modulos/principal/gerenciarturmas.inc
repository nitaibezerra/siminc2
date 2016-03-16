<?
verificarTermoCompromisso(array("iusd"=>$_SESSION['sismedio'][$sis]['iusd'],"sis"=>$sis,"pflcod"=>$pflcod_gerenciador));

if(!$_SESSION['sismedio'][$sis]['uncid']) {
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

function selecionarTurmasTrocaF(turid) {
	window.location='sismedio.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?><?=(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"") ?>&responsavelturma=<?=PFL_FORMADORREGIONAL ?>&aba=gerenciarturmas&turid='+turid;
}

function selecionarTurmasTrocaO(turid) {
	window.location='sismedio.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?><?=(($_REQUEST['uncid'])?"&uncid=".$_REQUEST['uncid']:"") ?>&responsavelturma=<?=PFL_ORIENTADORESTUDO ?>&aba=gerenciarturmas&turid='+turid;
}

function efetuarTroca() {
	var conf = confirm('Deseja realmente efetuar as trocas de turmas?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "uncid");
		input.setAttribute("value", "<?=$_SESSION['sismedio'][$sis]['uncid'] ?>");
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
	<td><? echo carregarOrientacao($abaativa); ?></td>
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
	    		SELECT turid as codigo, tu.turdesc as descricao 
	    		FROM sismedio.turmas tu 
	    		INNER JOIN sismedio.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sismedio.universidadecadastro unc ON unc.uncid = tu.uncid 
	    		WHERE pflcod='".PFL_FORMADORREGIONAL."' AND unc.uncid='".$_SESSION['sismedio'][$sis]['uncid']."' ORDER BY descricao
	    		) UNION ALL (
				SELECT t.turid as codigo, t.turdesc||' - EXTINTA' as descricao  
				FROM sismedio.orientadorturma ot 
				INNER JOIN sismedio.tipoperfil tp ON tp.iusd = ot.iusd AND tp.pflcod=".PFL_ORIENTADORESTUDO."
				INNER JOIN sismedio.turmas t ON t.turid = ot.turid 
				INNER JOIN sismedio.universidadecadastro unc ON unc.uncid = t.uncid 
				INNER JOIN sismedio.universidade uni ON uni.uniid = unc.uniid 
				INNER JOIN sismedio.identificacaousuario i ON i.iusd = t.iusd
				LEFT JOIN sismedio.tipoperfil tp2 ON tp2.iusd = t.iusd 
				WHERE tp2.tpeid IS NULL AND t.uncid='".$_SESSION['sismedio'][$sis]['uncid']."' 
				GROUP BY t.turid, uni.unisigla, uni.uninome, i.iusnome, t.turdesc
	    		) UNION ALL (
				SELECT '9999999' as codigo, 'Orientadores de Estudo sem turma' as descricao
				)";
	    
	    $db->monta_combo('turid_formador', $sql, 'S', 'Selecione', 'selecionarTurmasTrocaF', '', '', '', 'S', 'turid_formador','', $_REQUEST['turid']); 
	    ?> 
	    </td>
	</tr>
	<? endif; ?>
	<? if($turorientadores) : ?>
    <tr>
    <td class=SubTituloDireita>Turmas de Orientadores de Estudo:</td>
    <td>
    
    <? 
    
    $sql = "(
		
    		SELECT turid as codigo, tu.turdesc as descricao 
    		FROM sismedio.turmas tu 
    		INNER JOIN sismedio.identificacaousuario i ON i.iusd = tu.iusd 
    		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
    		WHERE pflcod='".PFL_ORIENTADORESTUDO."' {$filtro_esfera} ORDER BY descricao
    		
    		) UNION ALL (
    		
			SELECT tu.turid as codigo, tu.turdesc||' - EXTINTA' as descricao  
			FROM sismedio.identificacaousuario i 
			LEFT JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN sismedio.turmas tu ON tu.iusd = i.iusd 
    		WHERE ((t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='I') OR t.pflcod IS NULL)  {$filtro_esfera} ORDER BY descricao
			
    		) UNION ALL (

    		SELECT '99999999' as codigo, 'Professores/Coordenadores Pedágogico sem turma'
			
			)";
    
    $db->monta_combo('turid_orientador', $sql, 'S', 'Selecione', 'selecionarTurmasTrocaO', '', '', '', 'S', 'turid_orientador','', $_REQUEST['turid']); 
    
    ?>
    </td>
    
    </tr>
    <? endif; ?>
    <tr>
    	<td colspan="2">
    	<? 
    	if($_REQUEST['turid']) :
    	
    		echo "<input type=hidden name=\"turidantigo\" value=\"".$_REQUEST['turid']."\">";
    	
    		$sql = "SELECT t.turid, t.turdesc, i.iusnome FROM sismedio.turmas t
    				INNER JOIN sismedio.identificacaousuario i ON i.iusd = t.iusd 
					INNER JOIN sismedio.tipoperfil ti ON ti.iusd = i.iusd AND ti.pflcod=".$_REQUEST['responsavelturma']." 
    				WHERE ".(($_REQUEST['turid']=='99999999')?"":"turid!='".$_REQUEST['turid']."' AND")." i.uncid='".$_SESSION['sismedio'][$sis]['uncid']."' 
    				ORDER BY t.turdesc";
    		
    		$turmas_opcoes = $db->carregar($sql);
    		
    		$consulta = verificaPermissao();
    		
    		if($turmas_opcoes[0]) {
    			$html .= "<select name=troca['||ius.iusd||'] class=CampoEstilo style=font-size:x-small;width:auto; ".(($consulta)?"disabled":"").">";
    			$html .= "<option value=\"\">Selecione</option>";
    			foreach($turmas_opcoes as $tuo) {
    				$html .= "<option value=".$tuo['turid'].">".$tuo['turdesc']."</option>";
    			}
    			$html .= "</select>";
    		}
    		
    		if(substr($_REQUEST['turid'],0,6)=='999999') {
    		
    			if($_REQUEST['responsavelturma']==PFL_ORIENTADORESTUDO) $pfl_avaliado = PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO;
    			elseif($_REQUEST['responsavelturma']==PFL_FORMADORREGIONAL) $pfl_avaliado  = PFL_ORIENTADORESTUDO;
    		
    			$sql = "SELECT ius.iuscpf, ius.iusnome, p.pfldsc, tur.turdesc, '{$html}' FROM sismedio.identificacaousuario ius
    			LEFT JOIN sismedio.orientadorturma ot ON ius.iusd = ot.iusd
    			LEFT JOIN sismedio.turmas tur ON tur.turid = ot.turid 
    			LEFT JOIN sismedio.tipoperfil t2 ON t2.iusd = tur.iusd AND t2.pflcod IN({$_REQUEST['responsavelturma']})
    			INNER JOIN sismedio.tipoperfil t ON t.iusd = ius.iusd AND t.pflcod IN($pfl_avaliado)
    			LEFT JOIN seguranca.perfil p ON p.pflcod = t.pflcod
    			WHERE (ot.turid IS NULL OR t2.tpeid IS NULL) AND ius.uncid='".$_SESSION['sismedio'][$sis]['uncid']."'";
    		
    		} else {
    		

    		$sql = "SELECT ius.iuscpf, ius.iusnome, p.pfldsc, tur.turdesc, '{$html}' FROM sismedio.orientadorturma ot 
    				INNER JOIN sismedio.turmas tur ON tur.turid = ot.turid  
    				INNER JOIN sismedio.identificacaousuario ius ON ius.iusd = ot.iusd 
    				INNER JOIN sismedio.tipoperfil t ON t.iusd = ius.iusd 
    				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
    				WHERE ot.turid='".$_REQUEST['turid']."'";
    		
    		}
    		
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