<?
verificarTermoCompromisso(array("iusd"=>$_SESSION['sispacto2'][$sis]['iusd'],"sis"=>$sis,"pflcod"=>$pflcod_gerenciador));

if(!$_SESSION['sispacto2'][$sis]['uncid']) {
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
function selecionarTurmasTrocaC(turid) {
	window.location='sispacto2.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_COORDENADORLOCAL ?>&aba=gerenciarturmas&turid='+turid;	
}

function selecionarTurmasTrocaS(turid) {
	window.location='sispacto2.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_SUPERVISORIES ?>&aba=gerenciarturmas&turid='+turid;
}

function selecionarTurmasTrocaF(turid) {
	window.location='sispacto2.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_FORMADORIES ?>&aba=gerenciarturmas&turid='+turid;
}

function selecionarTurmasTrocaO(turid) {
	window.location='sispacto2.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_ORIENTADORESTUDO ?>&aba=gerenciarturmas&turid='+turid;
}

function efetuarTroca() {
	var conf = confirm('Deseja realmente efetuar as trocas de turmas?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "uncid");
		input.setAttribute("value", "<?=$_SESSION['sispacto2'][$sis]['uncid'] ?>");
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
	<? if($tursupervisores) : ?>
    <tr>
    	<td class=SubTituloDireita>Turmas de Supervisores:</td>
    	<td>
    	<? 
	    $sql = "(
		
	    		SELECT turid as codigo, i.iusnome|| ' ( '||tu.turdesc||' )' as descricao 
	    		FROM sispacto2.turmas tu 
	    		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto2.universidadecadastro unc ON unc.uncid = tu.uncid 
	    		WHERE t.pflcod='".PFL_SUPERVISORIES."' AND unc.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."' ORDER BY descricao
		
	    		) UNION ALL (
				SELECT t.turid as codigo, i.iusnome || ' ( '||t.turdesc||' ) - EXTINTA' as descricao  
				FROM sispacto2.orientadorturmaoutros ot 
				INNER JOIN sispacto2.tipoperfil tp ON tp.iusd = ot.iusd AND tp.pflcod IN(".PFL_FORMADORIES.",".PFL_COORDENADORLOCAL.")
				INNER JOIN sispacto2.turmas t ON t.turid = ot.turid 
				INNER JOIN sispacto2.universidadecadastro unc ON unc.uncid = t.uncid 
				INNER JOIN sispacto2.universidade uni ON uni.uniid = unc.uniid 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd
				LEFT JOIN sispacto2.tipoperfil tp2 ON tp2.iusd = t.iusd 
				WHERE tp2.tpeid IS NULL AND t.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."' 
				GROUP BY t.turid, uni.unisigla, uni.uninome, i.iusnome, t.turdesc
	    		) UNION ALL (
			
				SELECT '999999' as codigo, 'Formadores IES/Coordenadores Locais sem turma'

				)";
	    
	    $db->monta_combo('turid_supervisor', $sql, 'S', 'Selecione', 'selecionarTurmasTrocaS', '', '', '', 'S', 'turid_supervisor','', $_REQUEST['turid']); 
	    ?> 
	    </td>
	</tr>
	<? endif; ?>
	<? if($turformadores) : ?>
    <tr>
    	<td class=SubTituloDireita>Turmas de Formadores:</td>
    	<td>
    	<? 
	    $sql = "(
		
	    		SELECT turid as codigo, i.iusnome|| ' ( '||tu.turdesc||' )' as descricao 
	    		FROM sispacto2.turmas tu 
	    		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto2.universidadecadastro unc ON unc.uncid = tu.uncid 
	    		WHERE t.pflcod='".PFL_FORMADORIES."' AND unc.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."' ORDER BY descricao
		
	    		) UNION ALL (
				SELECT t.turid as codigo, i.iusnome || ' ( '||t.turdesc||' ) - EXTINTA' as descricao  
				FROM sispacto2.orientadorturma ot 
				INNER JOIN sispacto2.tipoperfil tp ON tp.iusd = ot.iusd AND tp.pflcod=".PFL_ORIENTADORESTUDO."
				INNER JOIN sispacto2.turmas t ON t.turid = ot.turid 
				INNER JOIN sispacto2.universidadecadastro unc ON unc.uncid = t.uncid 
				INNER JOIN sispacto2.universidade uni ON uni.uniid = unc.uniid 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd
				LEFT JOIN sispacto2.tipoperfil tp2 ON tp2.iusd = t.iusd 
				WHERE tp2.tpeid IS NULL AND t.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."' 
				GROUP BY t.turid, uni.unisigla, uni.uninome, i.iusnome, t.turdesc
	    		) UNION ALL (
			
				SELECT '9999999' as codigo, 'Orientadores de Estudo sem turma'

				)";
	    
	    $db->monta_combo('turid_formador', $sql, 'S', 'Selecione', 'selecionarTurmasTrocaF', '', '', '', 'S', 'turid_formador','', $_REQUEST['turid']); 
	    ?> 
	    </td>
	</tr>
	<? endif; ?>
	<? if($turcoordenadoreslocais) : ?>
    <tr>
    	<td class=SubTituloDireita>Turmas de Coordenadores Locais:</td>
    	<td>
    	<? 
	    $sql = "(
		
	    		SELECT turid as codigo, i.iusnome|| ' ( '||tu.turdesc||' )' as descricao 
	    		FROM sispacto2.turmas tu 
	    		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto2.universidadecadastro unc ON unc.uncid = tu.uncid 
	    		WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND unc.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."' ORDER BY descricao

		
	    		) UNION ALL (
			SELECT tu.turid as codigo, i.iusnome||' ( '||tu.turdesc||' ) - EXTINTA' as descricao  
			FROM sispacto2.identificacaousuario i 
			LEFT JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN sispacto2.turmas tu ON tu.iusd = i.iusd 
    		WHERE ((t.pflcod='".PFL_COORDENADORLOCAL."' AND i.iusstatus='I') OR t.pflcod IS NULL) AND tu.turdesc ilike '%Turma CL%'  {$filtro_esfera} ORDER BY descricao
		
	    		) UNION ALL (
			
				SELECT '99999' as codigo, 'Orientadores de Estudo sem turma de CL'

				)";
	    
	    $db->monta_combo('turid_coordenadorlocal', $sql, 'S', 'Selecione', 'selecionarTurmasTrocaC', '', '', '', 'S', 'turid_coordenadorlocal','', $_REQUEST['turid']); 
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
		
    		SELECT turid as codigo, i.iusnome||' ( '||tu.turdesc||' )' as descricao 
    		FROM sispacto2.turmas tu 
    		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = tu.iusd 
    		INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
    		WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' {$filtro_esfera} ORDER BY descricao
    		
    		) UNION ALL (
    		
			SELECT tu.turid as codigo, i.iusnome||' ( '||tu.turdesc||' ) - EXTINTA' as descricao  
			FROM sispacto2.identificacaousuario i 
			LEFT JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN sispacto2.turmas tu ON tu.iusd = i.iusd 
    		WHERE ((t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='I') OR t.pflcod IS NULL) AND tu.turdesc ilike '%Turma OE%'  {$filtro_esfera} ORDER BY descricao
			
    		) UNION ALL (
    		
			SELECT '99999999' as codigo, 'Professores Alfabetizadores sem turma'
			
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
    	
    		if($sis=='coordenadorlocal') {
				$f_turma = "i.picid='".$_SESSION['sispacto2'][$sis][$_SESSION['sispacto2']['esfera']]['picid']."'";
			} else {
				$f_turma = "i.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."'";
			}
    	
    		$sql = "SELECT t.turid, t.turdesc, i.iusnome FROM sispacto2.turmas t
    				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd 
					INNER JOIN sispacto2.tipoperfil ti ON ti.iusd = i.iusd AND ti.pflcod=".$_REQUEST['responsavelturma']." 
    				WHERE ".((substr($_REQUEST['turid'],0,5)=='99999')?"":"turid!='".$_REQUEST['turid']."' AND")." {$f_turma} 
    				ORDER BY i.iusnome";
    		
    		$turmas_opcoes = $db->carregar($sql);
    		
    		$consulta = verificaPermissao();
    		
    		if($turmas_opcoes[0]) {
    			$html .= "<select name=troca['||ius.iusd||'] class=CampoEstilo style=width:auto; ".(($consulta)?"disabled":"").">";
    			$html .= "<option value=\"\">Selecione</option>";
    			foreach($turmas_opcoes as $tuo) {
    				$html .= "<option value=".$tuo['turid'].">".$tuo['iusnome']." ( ".$tuo['turdesc']." )</option>";
    			}
    			$html .= "</select>";
    		}
    		
    		if($_REQUEST['responsavelturma']==PFL_ORIENTADORESTUDO){ $pfl_avaliado = PFL_PROFESSORALFABETIZADOR;$ta = "sispacto2.orientadorturma";}
    		elseif($_REQUEST['responsavelturma']==PFL_FORMADORIES){ $pfl_avaliado = PFL_ORIENTADORESTUDO;$ta = "sispacto2.orientadorturma";}
    		elseif($_REQUEST['responsavelturma']==PFL_SUPERVISORIES){ $pfl_avaliado = PFL_FORMADORIES.",".PFL_COORDENADORLOCAL.",".PFL_FORMADORIESP;$ta = "sispacto2.orientadorturmaoutros";}
    		elseif($_REQUEST['responsavelturma']==PFL_COORDENADORLOCAL){ $pfl_avaliado = PFL_ORIENTADORESTUDO;$ta = "sispacto2.orientadorturmaoutros";}
    		
    		echo '<input type="hidden" name="tabelacontrole" value="'.$ta.'">';
			
    		if(substr($_REQUEST['turid'],0,5)=='99999') {

				$sql = "SELECT ius.iuscpf, ius.iusnome, p.pfldsc, tur.turdesc, '{$html}' FROM sispacto2.identificacaousuario ius
						LEFT JOIN {$ta} ot ON ius.iusd = ot.iusd
						LEFT JOIN sispacto2.turmas tur ON tur.turid = ot.turid
						INNER JOIN sispacto2.tipoperfil t ON t.iusd = ius.iusd AND t.pflcod IN($pfl_avaliado)
						LEFT JOIN seguranca.perfil p ON p.pflcod = t.pflcod
						WHERE ot.turid IS NULL AND ius.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."' ORDER BY ius.iusnome";


			} else {

	    		$sql = "SELECT ius.iuscpf, ius.iusnome, p.pfldsc, tur.turdesc, '{$html}' FROM {$ta} ot 
	    				INNER JOIN sispacto2.turmas tur ON tur.turid = ot.turid  
	    				INNER JOIN sispacto2.identificacaousuario ius ON ius.iusd = ot.iusd 
	    				INNER JOIN sispacto2.tipoperfil t ON t.iusd = ius.iusd 
	    				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
	    				WHERE ot.turid='".$_REQUEST['turid']."' ORDER BY ius.iusnome"; 
	    		
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
	<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'])) ?>
	</td>
</tr>
</table>