<?
verificarTermoCompromisso(array("iusd"=>$_SESSION['sispacto3'][$sis]['iusd'],"sis"=>$sis,"pflcod"=>$pflcod_gerenciador));

if(!$_SESSION['sispacto3'][$sis]['uncid']) {
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
function selecionarTurmasTrocaA(turid) {
	window.location='sispacto3.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_COORDENADORADJUNTOIES ?>&aba=gerenciarturmas&turid='+turid;	
}


function selecionarTurmasTrocaC(turid) {
	window.location='sispacto3.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_COORDENADORLOCAL ?>&aba=gerenciarturmas&turid='+turid;	
}

function selecionarTurmasTrocaS(turid) {
	window.location='sispacto3.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_SUPERVISORIES ?>&aba=gerenciarturmas&turid='+turid;
}

function selecionarTurmasTrocaF(turid) {
	window.location='sispacto3.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_FORMADORIES ?>&aba=gerenciarturmas&turid='+turid;
}

function selecionarTurmasTrocaO(turid) {
	window.location='sispacto3.php?modulo=<?=$_REQUEST['modulo'] ?>&acao=<?=$_REQUEST['acao'] ?>&responsavelturma=<?=PFL_ORIENTADORESTUDO ?>&aba=gerenciarturmas&turid='+turid;
}

function efetuarTroca() {
	var conf = confirm('Deseja realmente efetuar as trocas de turmas?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "uncid");
		input.setAttribute("value", "<?=$_SESSION['sispacto3'][$sis]['uncid'] ?>");
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
	<? if($turcoordenadoradjuntoies) : ?>
    <tr>
    	<td class=SubTituloDireita>Turmas de Coordenadores Adjunto IES:</td>
    	<td>
    	<? 
	    $sql = "(
		
				SELECT turid::text as codigo, i.iusnome|| ' ( '||tu.turdesc||' - Turma de '||pe.pfldsc||' )' as descricao 
	    		FROM sispacto3.turmas tu 
				INNER JOIN seguranca.perfil pe ON pe.pflcod = tu.pflcod 
	    		INNER JOIN sispacto3.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto3.universidadecadastro unc ON unc.uncid = tu.uncid 
	    		WHERE t.pflcod='".PFL_COORDENADORADJUNTOIES."' AND unc.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."' ORDER BY descricao
		
	    		) UNION ALL (
			
				SELECT '999999c' as codigo, 'Coordenadores Locais sem turma'

				) UNION ALL (
			
				SELECT '999999s' as codigo, 'Supervisores IES sem turma'

				)
		";
	    
	    $db->monta_combo('turid_coordenadoradjunto', $sql, 'S', 'Selecione', 'selecionarTurmasTrocaA', '', '', '', 'S', 'turid_coordenadoradjunto','', $_REQUEST['turid']); 
	    ?> 
	    </td>
	</tr>
	<? endif; ?>
	<? if($tursupervisores) : ?>
    <tr>
    	<td class=SubTituloDireita>Turmas de Supervisores:</td>
    	<td>
    	<? 
	    $sql = "(
		
				SELECT turid as codigo, i.iusnome|| ' ( '||tu.turdesc||' )' as descricao 
	    		FROM sispacto3.turmas tu 
	    		INNER JOIN sispacto3.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto3.universidadecadastro unc ON unc.uncid = tu.uncid 
	    		WHERE t.pflcod='".PFL_SUPERVISORIES."' AND unc.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."' ORDER BY descricao
		
	    		) UNION ALL (
			
				SELECT '999999' as codigo, 'Formadores IES Locais sem turma'

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
	    		FROM sispacto3.turmas tu 
	    		INNER JOIN sispacto3.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto3.universidadecadastro unc ON unc.uncid = tu.uncid 
	    		WHERE t.pflcod='".PFL_FORMADORIES."' AND unc.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."' ORDER BY descricao

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
		
	    		SELECT turid as codigo, i.iusnome|| ' ( '||tu.turdesc||' - '||uu.unisigla||' )' as descricao 
	    		FROM sispacto3.turmas tu 
	    		INNER JOIN sispacto3.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto3.universidadecadastro unc ON unc.uncid = tu.uncid 
				INNER JOIN sispacto3.universidade uu ON uu.uniid = unc.uniid  
	    		WHERE t.pflcod='".PFL_COORDENADORLOCAL."' ".(($filtro_esfera)?$filtro_esfera:"AND unc.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."'")." ORDER BY descricao
		
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
    		FROM sispacto3.turmas tu 
    		INNER JOIN sispacto3.identificacaousuario i ON i.iusd = tu.iusd 
    		INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
    		WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' {$filtro_esfera} ORDER BY descricao
    		
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
				$f_turma = "i.picid='".$_SESSION['sispacto3'][$sis][$_SESSION['sispacto3']['esfera']]['picid']."'";
			} else {
				$f_turma = "i.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."'";
			}
			
			if(substr($_REQUEST['turid'],0,5)!='99999') {
				$pf = $db->pegaUm("SELECT pflcod FROM sispacto3.turmas WHERE turid='".$_REQUEST['turid']."'");
				if($pf) $f_turma .= " AND t.pflcod='".$pf."'";
			}
    	
    		$sql = "SELECT t.turid, t.turdesc, i.iusnome, uu.unisigla  FROM sispacto3.turmas t
    				INNER JOIN sispacto3.identificacaousuario i ON i.iusd = t.iusd 
					LEFT JOIN sispacto3.universidadecadastro unc ON unc.uncid = i.uncid 
					LEFT JOIN sispacto3.universidade uu ON uu.uniid = unc.uniid 
					INNER JOIN sispacto3.tipoperfil ti ON ti.iusd = i.iusd AND ti.pflcod=".$_REQUEST['responsavelturma']." 
    				WHERE ".((substr($_REQUEST['turid'],0,5)=='99999')?"":"turid!='".$_REQUEST['turid']."' AND")." {$f_turma} 
    				ORDER BY i.iusnome";
    		
    		$turmas_opcoes = $db->carregar($sql);
    		
    		$consulta = verificaPermissao();
    		
    		if($turmas_opcoes[0]) {
    			$html .= "<select name=troca['||i.iusd||'] class=CampoEstilo style=width:auto; ".(($consulta)?"disabled":"").">";
    			$html .= "<option value=\"\">Selecione</option>";
    			foreach($turmas_opcoes as $tuo) {
    				$html .= "<option value=".$tuo['turid'].">".$tuo['iusnome']." ( ".$tuo['turdesc']." - ".$tuo['unisigla']." )</option>";
    			}
    			$html .= "</select>";
    		}
    		
    		if($_REQUEST['responsavelturma']==PFL_ORIENTADORESTUDO){ $pfl_avaliado = PFL_PROFESSORALFABETIZADOR;$ta = "sispacto3.professoralfabetizadorturma";}
    		elseif($_REQUEST['responsavelturma']==PFL_FORMADORIES){ $pfl_avaliado = PFL_ORIENTADORESTUDO;$ta = "sispacto3.orientadorestudoturma";}
    		elseif($_REQUEST['responsavelturma']==PFL_COORDENADORADJUNTOIES) {
				if($pf==PFL_COORDENADORLOCAL) {
					$pfl_avaliado = PFL_COORDENADORLOCAL;
					$ta = "sispacto3.coordenadorlocalturma";
				} elseif($pf==PFL_SUPERVISORIES) { 
					$pfl_avaliado = PFL_SUPERVISORIES;
					$ta = "sispacto3.supervisoriesturma";
				} else {
					if($_REQUEST['turid']=='999999c') {
						$pfl_avaliado = PFL_COORDENADORLOCAL;
						$ta = "sispacto3.coordenadorlocalturma";
					} elseif($_REQUEST['turid']=='999999s') {
						$pfl_avaliado = PFL_SUPERVISORIES;
						$ta = "sispacto3.supervisoriesturma";
					}
				}
			}
    		elseif($_REQUEST['responsavelturma']==PFL_SUPERVISORIES){ $pfl_avaliado = PFL_FORMADORIES;$ta = "sispacto3.formadoriesturma";}
    		elseif($_REQUEST['responsavelturma']==PFL_COORDENADORLOCAL){ $pfl_avaliado = PFL_ORIENTADORESTUDO;$ta = "sispacto3.orientadorestudoturmacl";}
    		
    		echo '<input type="hidden" name="tabelacontrole" value="'.$ta.'">';
			
    		if(substr($_REQUEST['turid'],0,5)=='99999') {

				$sql = "SELECT i.iuscpf, i.iusnome||' - '||COALESCE(uu.unisigla,'-') as iusnome, p.pfldsc, tur.turdesc, '{$html}' FROM sispacto3.identificacaousuario i
						LEFT JOIN {$ta} ot ON i.iusd = ot.iusd
						LEFT JOIN sispacto3.turmas tur ON tur.turid = ot.turid 
						LEFT JOIN sispacto3.universidadecadastro unc ON unc.uncid = i.uncid 
						LEFT JOIN sispacto3.universidade uu ON uu.uniid = unc.uniid 
						INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN($pfl_avaliado)
						LEFT JOIN seguranca.perfil p ON p.pflcod = t.pflcod
						WHERE ot.turid IS NULL ".(($filtro_esfera)?$filtro_esfera:"AND i.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."'")." ORDER BY i.iusnome";


			} else {

	    		$sql = "SELECT i.iuscpf, i.iusnome||' - '||COALESCE(uu.unisigla,'-') as iusnome, p.pfldsc, tur.turdesc, '{$html}' FROM {$ta} ot 
	    				INNER JOIN sispacto3.turmas tur ON tur.turid = ot.turid  
	    				INNER JOIN sispacto3.identificacaousuario i ON i.iusd = ot.iusd 
	    				INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
	    				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
						LEFT JOIN sispacto3.universidadecadastro unc ON unc.uncid = i.uncid 
						LEFT JOIN sispacto3.universidade uu ON uu.uniid = unc.uniid 
	    				WHERE ot.turid='".$_REQUEST['turid']."' ORDER BY i.iusnome"; 
	    		
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