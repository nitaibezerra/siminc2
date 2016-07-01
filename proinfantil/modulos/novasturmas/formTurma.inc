<?php
if($_REQUEST['totalTurmas']){
	$sql = "SELECT 		sum(alaquantidade) as total
			FROM		proinfantil.turma tur
			INNER JOIN	proinfantil.novasturmasdadosmunicipios ntm ON tur.muncod = ntm.muncod
			INNER JOIN	proinfantil.mdsalunoatendidopbf map ON map.turid = tur.turid
			WHERE		tur.muncod = '{$_SESSION['proinfantil']['muncod']}'
			AND			ttuid = {$_REQUEST['ttuid']}
			AND			turtipoestabelecimento = {$_REQUEST['turtipoestabelecimento']}
			AND			titid = {$_REQUEST['titid']}
			AND			timid = {$_REQUEST['timid']}
			AND			turstatus = 'A'";
	$rs = $db->pegaUm($sql);
	echo $rs;
	die;
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
include  "_funcoes_novasturmas.php";
echo "<br/>";
$rsMatriculasTurmas = recuperarDadosMatriculasTurmas();
$perfis = pegaPerfil($_SESSION["usucpf"]);
$docid = criaDocumentoNovasTurmas($_SESSION['proinfantil']['muncod']);
$esdid = pegaEstadoAtualNovasTurmas($docid);
?>

<table class="tabela" align="center" cellspacing="0" cellpadding="3" width="100%">
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" id="btnVoltar" />
		</td>
		<td class="TituloTela">Novas Turmas</td>
	</tr>
</table>
<br/>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	$('#btnVoltar').click(function(){
		document.location.href = 'proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A';
	});

	function abreTurma(nome,id){
		return window.open('proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&escolhaTurma='+nome+'&turid='+id,'modelo',"height=600,width=950,scrollbars=yes,top=50,left=200");
	}
</script>
<?php

$arMnuid = array();
if($_SESSION['baselogin']=="simec_desenvolvimento") {
	$abacod_tela = 57524;
	if(!$_SESSION['proinfantil']['turid']){
		$arMnuid = array(11766, 11767);
	}
}else{
	$abacod_tela = 57524;
	if(!$_SESSION['proinfantil']['turid']){
		$arMnuid = array(11125);
	}
}

if( /*$esdid != WF_NOVASTURMAS_EM_ANALISE ||*/ in_array(EQUIPE_MUNICIPAL,$perfis) || in_array(SECRETARIO_ESTADUAL,$perfis)){
	$arMnuid[] = 11139;
}

// Monta abas
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

monta_titulo('Cadastro de Novas Turmas','');
cabecalhoTurma(); 

if($rsMatriculasTurmas): 
	$qtdCreche    = $rsMatriculasTurmas['ntmqtdturmacrechepublica']-$rsMatriculasTurmas['ntcqtdturmacrechepublica'];
	$qtdCrecheC   = $rsMatriculasTurmas['ntmqtdturmacrecheconveniada']-$rsMatriculasTurmas['ntcqtdturmacrecheconveniada'];
	$totalCreche  = $qtdCreche + $qtdCrecheC;
	
	if($qtdCreche > 0){
		if($qtdCreche > $totalCreche){
			$qtdCrechePub = $qtdCreche + $qtdCrecheC;
		} else {
			$qtdCrechePub = $qtdCreche;
		}		
	} else {
		$qtdCrechePub = 0;
	}

	if($qtdCrecheC > 0){
		if($qtdCrecheC > $totalCreche){
			$qtdCrecheCon = $qtdCrecheC + $qtdCreche;
		} else {
			$qtdCrecheCon = $qtdCrecheC;
		}		
	} else {
		$qtdCrecheCon = 0;
	}
	
	$qtdPreEscola  = $rsMatriculasTurmas['ntmqtdturmapreescolapublica']-$rsMatriculasTurmas['ntcqtdturmapreescolapublica'];
	$qtdPreEscolaC = $rsMatriculasTurmas['ntmqtdturmapreescolaconveniada']-$rsMatriculasTurmas['ntcqtdturmapreescolaconveniada'];
	$totalPreEscola  = $qtdPreEscola + $qtdPreEscolaC;

	if($qtdPreEscola > 0){
		if($qtdPreEscola > $totalPreEscola){
			$qtdPreEscolaPub = $qtdPreEscola + $qtdPreEscolaC;
		} else {
			$qtdPreEscolaPub = $qtdPreEscola;
		}
	} else {
		$qtdPreEscolaPub = 0;
	}

	if($qtdPreEscolaC > 0){
		if($qtdPreEscolaC > $totalPreEscola){
			$qtdPreEscolaCon = $qtdPreEscolaC + $qtdPreEscola;
		} else {
			$qtdPreEscolaCon = $qtdPreEscolaC;
		}
	} else {
		$qtdPreEscolaCon = 0;
	}

	$qtdUnificada  = $rsMatriculasTurmas['ntmqtdturmaunificadapublica']-$rsMatriculasTurmas['ntcqtdturmaunificadapublica'];
	$qtdUnificadaC = $rsMatriculasTurmas['ntmqtdturmaunificadaconveniada']-$rsMatriculasTurmas['ntcqtdturmaunificadaconveniada'];
	$totalUnificada  = $qtdUnificada + $qtdUnificadaC;
	
	if($qtdUnificada > 0){
		if($qtdUnificada > $totalUnificada){
			$qtdUnificadaPub = $qtdUnificada + $qtdUnificadaC;
		} else {
			$qtdUnificadaPub = $qtdUnificada;
		}
	} else {
		$qtdUnificadaPub = 0;
	}

	if($qtdUnificadaC > 0){
		if($qtdUnificadaC > $totalUnificada){
			$qtdUnificadaCon = $qtdUnificadaC + $qtdUnificada;
		} else {
			$qtdUnificadaCon = $qtdUnificadaC;
		}
	} else {
		$qtdUnificadaCon = 0;
	}
	
	$indice=0;	

	$turmasJaCadastradas = mostrarTurmas();
	
	// PUBLICA = 1 | PRIVADA = 2 
	// CRECHE = 1 | PRÉ-ESCOLA = 2 | UNIFICADA = 3 
	
	$listaDeTurmasCrechePublica 	  = array();
	$listaDeTurmasCrecheConveniada    = array();
	$listaDeTurmasPreEscolaPublica    = array();
	$listaDeTurmasPreEscolaConveniada = array();
	$listaDeTurmasUnificadaPublica    = array();
	$listaDeTurmasUnificadaConveniada = array();
	
	//Lista de turmas já cadastradas
	
	if(!empty($turmasJaCadastradas)){
		foreach($turmasJaCadastradas as $turmaCadastrada){
			//------------- CRECHE PUBLICA   -------------------//
			if ($turmaCadastrada['ttuid'] == 1 && $turmaCadastrada['turtipoestabelecimento'] == 1 ){ // CRECHE PUBLICA
				$qtdCrechePub = $qtdCrechePub - 1; 
				$listaDeTurmasCrechePublica[$turmaCadastrada['turid']] = 'cadastrada';
				$nomeDeTurmasCrechePublica[$turmaCadastrada['turid']] = $turmaCadastrada['turdsc'];
			}
			//-------------CRECHE CONVENIADA  -------------------//
			if ($turmaCadastrada['ttuid'] == 1 && $turmaCadastrada['turtipoestabelecimento'] == 2 ){ // CRECHE CONVENIADA
				$qtdCrecheCon = $qtdCrecheCon - 1; 
				$listaDeTurmasCrecheConveniada[$turmaCadastrada['turid']] = 'cadastrada';
				$nomeDeTurmasCrecheConveniada[$turmaCadastrada['turid']] = $turmaCadastrada['turdsc'];
			}
			//-------------PREESCOLA PUBLICA  -------------------//
			if ($turmaCadastrada['ttuid'] == 2 && $turmaCadastrada['turtipoestabelecimento'] == 1 ){ // PREESCOLA PUBLICA
				$qtdPreEscolaPub = $qtdPreEscolaPub - 1; 
				$listaDeTurmasPreEscolaPublica[$turmaCadastrada['turid']] = 'cadastrada';
				$nomeDeTurmasPreEscolaPublica[$turmaCadastrada['turid']] = $turmaCadastrada['turdsc'];
			}	
			//-------------PREESCOLA CONVENIADA  -------------------//
			if ($turmaCadastrada['ttuid'] == 2 && $turmaCadastrada['turtipoestabelecimento'] == 2 ){ // PREESCOLA CONVENIADA
				$qtdPreEscolaCon = $qtdPreEscolaCon - 1; 
				$listaDeTurmasPreEscolaConveniada[$turmaCadastrada['turid']] = 'cadastrada';
				$nomeDeTurmasPreEscolaConveniada[$turmaCadastrada['turid']] = $turmaCadastrada['turdsc'];
			}
			//-------------UNIFICADA PUBLICA  -------------------//
			if ($turmaCadastrada['ttuid'] == 3 && $turmaCadastrada['turtipoestabelecimento'] == 1 ){ // UNIFICADA PUBLICA
				$qtdUnificadaPub = $qtdUnificadaPub - 1; 
				$listaDeTurmasUnificadaPublica[$turmaCadastrada['turid']] = 'cadastrada';
				$nomeDeTurmasUnificadaPublica[$turmaCadastrada['turid']] = $turmaCadastrada['turdsc'];
			}
			//-------------UNIFICADA CONVENIADA  -------------------//
			if ($turmaCadastrada['ttuid'] == 3 && $turmaCadastrada['turtipoestabelecimento'] == 2 ){ // UNIFICADA PUBLICA
				$qtdUnificadaCon = $qtdUnificadaCon - 1; 
				$listaDeTurmasUnificadaConveniada[$turmaCadastrada['turid']] = 'cadastrada';
				$nomeDeTurmasUnificadaConveniada[$turmaCadastrada['turid']] = $turmaCadastrada['turdsc'];
			}
		}
	}
	
	//Propostas de turmas que podem ser cadastras
	for($x=1;$x<=$qtdCrechePub;$x++){
		$listaDeTurmasCrechePublica[$x] = 'naocadastrada';
	}
	
	for($x=1;$x<=$qtdCrecheCon;$x++){
		$listaDeTurmasCrecheConveniada[$x] = 'naocadastrada';
	}
	
	for($y=1;$y<=$qtdPreEscolaPub;$y++){
		$listaDeTurmasPreEscolaPublica[$y] = 'naocadastrada';
	}
	
	for($y=1;$y<=$qtdPreEscolaCon;$y++){
		$listaDeTurmasPreEscolaConveniada[$y] = 'naocadastrada';
	}
	
	for($z=1;$z<=$qtdUnificadaPub;$z++){
		$listaDeTurmasUnificadaPublica[$z] = 'naocadastrada';
	}
	
	for($z=1;$z<=$qtdUnificadaCon;$z++){
		$listaDeTurmasUnificadaConveniada[$z] = 'naocadastrada';
	}

		if($qtdCreche>0 || $qtdPreEscola>0 || $qtdUnificada>0 || $qtdCrecheC>0 || $qtdPreEscolaC>0 || $qtdUnificadaC>0): ?>
		<table class="listagem" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 style="width:95%;">					
			<thead>
				<th>Novas Turmas</th>
				<th>Ação</th>
			</thead>
			<tbody>
			<?php if(count($listaDeTurmasCrechePublica)> 0  || count($listaDeTurmasCrecheConveniada) ){ ?>
				<?php $x = 1;
				
				foreach($listaDeTurmasCrechePublica as $chave => $turmas){ ?>
					<?php $cor = ($indice%2) ? '' : '#F7F7F7'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
						<td> Creche Pública <?php echo $x; ?><?php echo $nomeDeTurmasCrechePublica[$chave] ? " ( " . $nomeDeTurmasCrechePublica[$chave] . " ) " : "" ; ?></td>
						<td align="center">
						<?php if($turmas == 'cadastrada'){ ?>
							<input type="button" id="creche_publica_<?php echo $x ?>" value="Editar Turma" onclick="abreTurma('creche_publica_<?php echo $x ?>','<?php echo $chave; ?>');"/>
						<?php } else { ?>
							<input type="button" id="creche_publica_<?php echo $x ?>" value="Cadastrar Turma" onclick="abreTurma('creche_publica_<?php echo $x ?>','<?php echo $chave; ?>');"/>
						<?php } ?>
						</td>								
					</tr>
					<?php $indice++; ?>
				<?php $x++; } ?>
				
				<?php $x = 1;
				foreach($listaDeTurmasCrecheConveniada as $chave => $turmas){ ?>
					<?php $cor = ($indice%2) ? '' : '#F7F7F7'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
						<td>Creche Conveniada <?php echo $x; ?><?php echo $nomeDeTurmasCrecheConveniada[$chave] ? " ( " . $nomeDeTurmasCrecheConveniada[$chave] . " ) " : "" ; ?></td>
						<td align="center">
							<?php if($turmas == 'cadastrada'){ ?>
								<input type="button" id="creche_conveniada_<?php echo $x ?>" value="Editar Turma" onclick="abreTurma('creche_conveniada_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } else { ?>
								<input type="button" id="creche_conveniada_<?php echo $x ?>" value="Cadastrar Turma" onclick="abreTurma('creche_conveniada_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } ?>								
						</td>								
					</tr>
					<?php $indice++; ?>
				<?php $x++; } ?>
			<?php } else { ?>
				<tr>
					<td align="center" colspan="2"><b>Este município não tem direito a creche.</b></td>					
				</tr>
			<?php } ?>
				
			<?php if(count($listaDeTurmasPreEscolaPublica)> 0  || count($listaDeTurmasPreEscolaConveniada) ){ ?>
				<?php $y = 1;
				foreach($listaDeTurmasPreEscolaPublica as $chave => $turmas){ ?>				
					<?php $cor = ($indice%2) ? '' : '#F7F7F7'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
						<td>Pré-escola Pública <?php echo $y; ?><?php echo $nomeDeTurmasPreEscolaPublica[$chave] ? " ( " . $nomeDeTurmasPreEscolaPublica[$chave] . " ) " : "" ; ?></td>
						<td align="center">
							<?php if($turmas == 'cadastrada'){ ?>
								<input type="button" id="preescola_publica_<?php echo $y ?>" value="Editar Turma" onclick="abreTurma('preescola_publica_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } else { ?>
								<input type="button" id="preescola_publica_<?php echo $y ?>" value="Cadastrar Turma" onclick="abreTurma('preescola_publica_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } ?>								
						</td>								
					</tr>
					<?php $indice++; ?>
				<?php $y++; }; ?>
				
				<?php $y = 1;
				foreach($listaDeTurmasPreEscolaConveniada as $chave => $turmas){ ?>	
					<?php $cor = ($indice%2) ? '' : '#F7F7F7'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
						<td>Pré-escola Conveniada <?php echo $y; ?><?php echo $nomeDeTurmasPreEscolaConveniada[$chave] ? " ( " . $nomeDeTurmasPreEscolaConveniada[$chave] . " ) " : "" ; ?></td>
						<td align="center">
							<?php if($turmas == 'cadastrada'){ ?>
								<input type="button" id="preescola_conveniada_<?php echo $y ?>" value="Editar Turma" onclick="abreTurma('preescola_conveniada_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } else { ?>
								<input type="button" id="preescola_conveniada_<?php echo $y ?>" value="Cadastrar Turma" onclick="abreTurma('preescola_conveniada_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } ?>
						</td>								
					</tr>
					<?php $indice++; ?>
				<?php $y++; }; ?>
			<?php } else { ?>
				<tr>
					<td align="center" colspan="2"><b>Este município não tem direito a pré-escola.</b></td>					
				</tr>
			<?php } ?>
				
			<?php if(count($listaDeTurmasUnificadaPublica)> 0  || count($listaDeTurmasUnificadaConveniada) ){ ?>	
				<?php $z = 1;
				foreach($listaDeTurmasUnificadaPublica as $chave => $turmas){ ?>	
					<?php $cor = ($indice%2) ? '' : '#F7F7F7'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
						<td>Unificada Pública <?php echo $z; ?> (matrículas de creche e pré-escola na mesma turma) <?php echo $nomeDeTurmasUnificadaPublica[$chave] ? " ( " . $nomeDeTurmasUnificadaPublica[$chave] . " ) " : "" ; ?></td>
						<td align="center">								
							<?php if($turmas == 'cadastrada'){ ?>
								<input type="button" id="unificada_publica_<?php echo $z ?>" value="Editar Turma" onclick="abreTurma('unificada_publica_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } else { ?>
								<input type="button" id="unificada_publica_<?php echo $z ?>" value="Cadastrar Turma" onclick="abreTurma('unificada_publica_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } ?>
						</td>								
					</tr>
					<?php $indice++; ?>
				<?php $z++; }  ?>
							
				<?php $z = 1;
				foreach($listaDeTurmasUnificadaConveniada as $chave => $turmas){ ?>	
					<?php $cor = ($indice%2) ? '' : '#F7F7F7'; ?>
					<tr bgcolor="<?php echo $cor; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor; ?>';">
						<td>Unificada Conveniada <?php echo $z; ?> (matrículas de creche e pré-escola na mesma turma) <?php echo $nomeDeTurmasUnificadaConveniada[$chave] ? " ( " . $nomeDeTurmasUnificadaConveniada[$chave] . " ) " : "" ; ?></td>
						<td align="center">								
							<?php if($turmas == 'cadastrada'){ ?>
								<input type="button" id="unificada_conveniada_<?php echo $z ?>" value="Editar Turma" onclick="abreTurma('unificada_conveniada_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } else { ?>
								<input type="button" id="unificada_conveniada_<?php echo $z ?>" value="Cadastrar Turma" onclick="abreTurma('unificada_conveniada_<?php echo $x ?>','<?php echo $chave; ?>');"/>
							<?php } ?>
						</td>								
					</tr>
					<?php $indice++; ?>
				<?php $z++; }  ?>
			<?php } else { ?>
				<tr>
					<td align="center" colspan="2"><b>Este município não tem direito a unificada.</b></td>					
				</tr>
			<?php } ?>
			</tbody>
		</table>
	<?php endif; ?>
<?php endif; ?>