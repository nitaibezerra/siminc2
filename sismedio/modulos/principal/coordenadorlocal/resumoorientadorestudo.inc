<?
verificarCoordenadorLocalTermoCompromisso(array("picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']));

$consulta = verificaPermissao();

$perfis = pegaPerfilGeral();


$estado = wf_pegarEstadoAtual( $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['docid'] );
$estadoturma = wf_pegarEstadoAtual( $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['docidturma'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_LOCAL) {
	$consulta = true;
}

if($estado['esdid'] == ESD_VALIDADO_COORDENADOR_LOCAL && $estadoturma['esdid'] != ESD_FECHADO_TURMA) {
	$consulta = false;
}

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$consulta = true;
}


$ar = array("estuf" 	  => $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['estuf'],
			"muncod" 	  => $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod'],
			"dependencia" => $_SESSION['sispacto']['esfera']);

$totalalfabetizadores = carregarTotalAlfabetizadores($ar);

?>
<script>
function salvarJustificativas(goto) {

	<? foreach(array_keys($_PERGUNTA_JUSTIFICATIVA) as $key) : ?>
	var nchecks = jQuery("[name^='tjuid[<?=$key ?>]']:enabled").length;
	
	if(nchecks > 0) {
		var ncheckeds = jQuery("[name^='tjuid[<?=$key ?>]']:enabled:checked").length;
		if(ncheckeds == 0) {
			alert('Marque o tipo de Justificativa : <?=$_TIPO_ORIENTADORES[$key] ?>');
			return false;
		}
		if(jQuery("[name^='joecomentario[<?=$key ?>]']").val()=='') {
			alert('Justificativa em branco : <?=$_TIPO_ORIENTADORES[$key] ?>');
			return false;
		}
	}
	<? endforeach; ?>
	
	jQuery('#goto').val(goto);	
	document.getElementById('formulario').submit();

}
</script>
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="inserirJustificativas">
<input type="hidden" name="goto" id="goto" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Orientadores de Estudo</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Número de professores Alfabetizadores da Rede (CENSO-2012)</td>
	<td><?=campo_texto('totalalfabetizadores', "N", "N", "Número de professores Alfabetizadores da Rede (CENSO-2012)", 8, 7, "#######", "", '', '', 0, 'id="totalalfabetizadores"', '', $totalalfabetizadores['total']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Número de orientadores de estudos a serem cadastrados</td>
	<td><?=campo_texto('total_a_serem_cadastrados', "N", "N", "Número de orientadores de estudos a serem cadastrados", 8, 7, "#######", "", '', '', 0, 'id="total_a_serem_cadastrados"', '', $totalalfabetizadores['total_orientadores_a_serem_cadastrados']); ?></td>
</tr>

<tr>
	<td colspan="2">
	<table width="100%">
	<tr>
	<td>
	<? $justificativas_respostas = carregarJustificativasRespostas(array("picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'])); ?>	
	<? foreach($_TIPO_ORIENTADORES as $key => $value) : ?>
		<? $orientadoresestudo = carregarDadosIdentificacaoUsuario(array("picid"=>$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid'],"pflcod"=>PFL_ORIENTADORESTUDO,"iustipoorientador"=>$key)); ?>
		<? if($orientadoresestudo) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloEsquerda" colspan="4"><?=$value ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" width="20%">CPF</td>
			<td class="SubTituloCentro" width="40%">Nome</td>
			<td class="SubTituloCentro" width="40%">E-mail</td>
		</tr>
		<? foreach($orientadoresestudo as $oe) : ?>
		<tr>
			<td width="20%"><?=mascaraglobal($oe['iuscpf'],"###.###.###-##") ?></td>
			<td width="40%"><?=$oe['iusnome'] ?></td>
			<td width="40%"><?=$oe['iusemailprincipal'] ?></td>
		</tr>
		<? endforeach; ?>
		</table>
		<? endif; ?>
		<? if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] > 0 && ($totalalfabetizadores['total_orientadores_a_serem_cadastrados']-$totalorientadores) != count($orientadoresestudo)) : ?>
		<? $tipojustificativas = $db->carregar("SELECT tjuid as codigo, tjudesc as descricao FROM sispacto.tipojustificativa WHERE tjustatus='A' AND tjutipo='".$key."'") ?>
		<? if($tipojustificativas) : ?>
		<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
  
			<td colspan="3">
			<table class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloEsquerda" colspan="2"><?=$_PERGUNTA_JUSTIFICATIVA[$key] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%">Tipo de Justificativa</td>
				<td>
				<?
				echo "<table width=100%>";
				foreach($tipojustificativas as $tj) :
					echo "<tr><td width=5%><input type=\"checkbox\" name=\"tjuid[{$key}][]\" value=\"".$tj['codigo']."\" ".((in_array($tj['codigo'], (($justificativas_respostas[$key]['tjuids'])?$justificativas_respostas[$key]['tjuids']:array())))?"checked":"")." ".(($consulta)?"disabled":"")."></td><td>".$tj['descricao']."</td></tr>";
				endforeach;
				echo "</table>";
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Comentários adicionais</td>
				<td><? echo campo_textarea( "joecomentario[{$key}]", 'S', (($consulta)?'N':'S'), '', '70', '4', '250', '', '', '', '', '', $justificativas_respostas[$key]['joecomentario']); ?></td>
			</tr>
			</table>
			</td>
		</tr>
		</table>
		<? endif; ?>
		<? endif; ?>
		<? $totalorientadores += count($orientadoresestudo); ?>
	<? endforeach; ?>
	</td>
	<td align="center" valign="top">
	<?
	/* Barra de estado atual e ações e Historico */
	wf_desenhaBarraNavegacao( $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['docid'], array('picid' => $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']) );
	?>
	</td>
	</tr>
	</table>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" width="25%">&nbsp;</td>
	<td><input type="button" value="Anterior" onclick="window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo';">
		<? if(!$consulta) : ?> 
		<input type="button" name="salvar" value="Salvar" onclick="salvarJustificativas('sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=resumoorientadorestudo');">
		<? endif; ?>
	</td>
</tr>
</table>
</form>