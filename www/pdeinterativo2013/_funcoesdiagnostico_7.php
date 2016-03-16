<?
function diagnostico_7_sintese($dados) {
	global $db;
	
	$pdeid = $_SESSION['pdeinterativo2013_vars']['pdeid'];
	
	if($dados['respostaideb']) {
		foreach($dados['respostaideb'] as $campo => $ideb) {
			$db->executar("UPDATE pdeinterativo2013.respostaideb SET ".$campo."=".$ideb." WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
			$db->commit();
		}
	}
	
	if($dados['respostataxarendimento']) {
		foreach($dados['respostataxarendimento'] as $campo => $tx) {
			$db->executar("UPDATE pdeinterativo2013.respostataxarendimento SET ".$campo."=".$tx." WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
			$db->commit();
		}
	}
	
	if($dados['respostaprovabrasil']) {
		foreach($dados['respostaprovabrasil'] as $campo => $pb) {
			$db->executar("UPDATE pdeinterativo2013.respostaprovabrasil SET ".$campo."=".$pb." WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
			$db->commit();
		}
	}
	
	if($dados['critico2']) {
		foreach($dados['critico2'] as $indice => $valor) {
			$sql = "UPDATE pdeinterativo2013.respostapergunta SET critico2=".$valor." WHERE repid='".$indice."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['suficiente']) {
		foreach($dados['suficiente'] as $indice => $valor) {
			$sql = "UPDATE pdeinterativo2013.respostapergunta SET suficiente=".$valor." WHERE repid='".$indice."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['resolvidoescola']) {
		foreach($dados['resolvidoescola'] as $indice => $valor) {
			$sql = "UPDATE pdeinterativo2013.respostapergunta SET resolvidoescola=".$valor." WHERE repid='".$indice."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['pessoasresolvidoescola']) {
		foreach($dados['pessoasresolvidoescola'] as $indice => $valor) {
			$sql = "UPDATE pdeinterativo2013.respostapergunta SET resolvidoescola=".$valor." WHERE repid='".$indice."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['respostapaiscomunidade']) {
		foreach($dados['respostapaiscomunidade'] as $campo => $vl) {
			$db->executar("UPDATE pdeinterativo2013.respostapaiscomunidade SET ".$campo."=".$vl." WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
			$db->commit();
		}
	}
	
	if($dados['pessoas2']) {
		foreach($dados['pessoas2'] as $indice => $valor) {
			$pesids = explode(",",$indice);
			foreach($pesids as $pesid) {
				$sql = "UPDATE pdeinterativo2013.pessoa SET critico2=".$valor." WHERE pesid='".$pesid."'";
				$db->executar($sql);
				$db->commit();
			}
		}
	}
	
	if($dados['pessoassuficiente']) {
		foreach($dados['pessoassuficiente'] as $indice => $valor) {
			$pesids = explode(",",$indice);
			foreach($pesids as $pesid) {
				$sql = "UPDATE pdeinterativo2013.pessoa SET suficiente=".$valor." WHERE pesid='".$pesid."'";
				$db->executar($sql);
				$db->commit();
			}
		}
	}
	
	if($dados['pessoasresolvidoescola']) {
		foreach($dados['pessoasresolvidoescola'] as $indice => $valor) {
			$pesids = explode(",",$indice);
			foreach($pesids as $pesid) {
				$sql = "UPDATE pdeinterativo2013.pessoa SET resolvidoescola=".$valor." WHERE pesid='".$pesid."'";
				$db->executar($sql);
				$db->commit();
			}
		}
	}
	
	if($dados['intalacao']['critico']) {
		$sql = "";
		foreach($dados['intalacao']['critico'] as $ifiid => $valor) {
			$rifcritico2 = !$valor ? "null" : $valor;
			$rifsuficiente = !$dados['intalacao']['suficiente'][$ifiid] ? "null" : $dados['intalacao']['suficiente'][$ifiid];
			$rifresolvidoescola = !$dados['intalacao']['resolvidoescola'][$ifiid] ? "null" : $dados['intalacao']['resolvidoescola'][$ifiid];
			$sql = "UPDATE 
						pdeinterativo2013.respostainfrainstalacaofisica 
					SET 
						rifcritico2 = $rifcritico2,
						rifsuficiente = $rifsuficiente,
						rifresolvidoescola = $rifresolvidoescola 
					WHERE 
						pdeid='".$pdeid."'
					and 
						ifiid = ".$ifiid."
					and
						rifstatus = 'A';";
			$db->executar($sql);
			$db->commit();
		}

	}
	
	if($dados['equipamento']['critico']) {
		$sql = "";
		foreach($dados['equipamento']['critico'] as $tmeid => $valor) {
			$remcritico2 = !$valor ? "null" : $valor;
			$remsuficiente = !$dados['equipamento']['suficiente'][$tmeid] ? "null" : $dados['equipamento']['suficiente'][$tmeid];
			$remresolvidoescola = !$dados['equipamento']['resolvidoescola'][$tmeid] ? "null" : $dados['equipamento']['resolvidoescola'][$tmeid];
			$sql= "UPDATE 
						pdeinterativo2013.respostamaterialequipamento
					SET 
						remcritico2 = $remcritico2,
						remsuficiente = $remsuficiente,
						remresolvidoescola = $remresolvidoescola 
					WHERE 
						pdeid='".$pdeid."'
					and 
						tmeid = ".$tmeid."
					and
						rmestatus = 'A';";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['taxadistorcao']['critico']) {
		$sql = "";
		foreach($dados['taxadistorcao']['critico'] as $arrTurmas => $valor) {
			$diacritico2 = !$valor ? "null" : $valor;
			$diasuficiente = !$dados['taxadistorcao']['suficiente'][$arrTurmas] ? "null" : $dados['taxadistorcao']['suficiente'][$arrTurmas];
			$diaresolvidoescola = !$dados['taxadistorcao']['resolvidoescola'][$arrTurmas] ? "null" : $dados['taxadistorcao']['resolvidoescola'][$arrTurmas];
						
			$sql= "update 
								pdeinterativo2013.distorcaoaproveitamento 
							set 
								diacritico2 = $diacritico2,
								disuficiente = $diasuficiente,
								diaresolvidoescola = $diaresolvidoescola 
							where 
								fk_cod_turma in (".$arrTurmas.") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'D' 
							and 
								pdeid = '$pdeid';";
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	if($dados['taxareprovacao']['critico']) {
		$sql = "";
		foreach($dados['taxareprovacao']['critico'] as $arrTurmas => $valor) {
			$diacritico2 = !$valor ? "null" : $valor;
			$diasuficiente = $dados['taxareprovacao']['suficiente'][$arrTurmas] == "" ? "null" : $dados['taxareprovacao']['suficiente'][$arrTurmas];
			$diaresolvidoescola = $dados['taxareprovacao']['resolvidoescola'][$arrTurmas] == "" ? "null" : $dados['taxareprovacao']['resolvidoescola'][$arrTurmas];			
			$sql= "update 
								pdeinterativo2013.distorcaoaproveitamento 
							set 
								diacritico2 = $diacritico2,
								disuficiente = $diasuficiente,
								diaresolvidoescola = $diaresolvidoescola 
							where 
								fk_cod_turma in (".$arrTurmas.") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'R' 
							and 
								pdeid = '$pdeid';";
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	if($dados['taxaabandono']['critico']) {
		$sql = "";
		foreach($dados['taxaabandono']['critico'] as $arrTurmas => $valor) {
			$diacritico2 = !$valor ? "null" : $valor;
			$diasuficiente = $dados['taxaabandono']['suficiente'][$arrTurmas] == "" ? "null" : $dados['taxaabandono']['suficiente'][$arrTurmas];
			$diaresolvidoescola = $dados['taxaabandono']['resolvidoescola'][$arrTurmas] == "" ? "null" : $dados['taxaabandono']['resolvidoescola'][$arrTurmas];
			$sql= "update 
								pdeinterativo2013.distorcaoaproveitamento 
							set 
								diacritico2 = $diacritico2,
								disuficiente = $diasuficiente,
								diaresolvidoescola = $diaresolvidoescola 
							where 
								fk_cod_turma in (".$arrTurmas.") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'A' 
							and 
								pdeid = '$pdeid';";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['disciplina']['critico']) {
		$sql = "";
		foreach($dados['disciplina']['critico'] as $disciplina => $arrTurmas) {
			foreach($arrTurmas as $turma => $valor) {
				$dtdcritico2 = !$valor ? "null" : $valor;
				$dtdsuficiente = !$dados['disciplina']['suficiente'][$disciplina][$turma] ? "null" : $dados['disciplina']['suficiente'][$disciplina][$turma];
				$dtdresolvidoescola = !$dados['disciplina']['resolvidoescola'][$disciplina][$turma] ? "null" : $dados['disciplina']['resolvidoescola'][$disciplina][$turma];
				$sql= "update 
								pdeinterativo2013.distorcaodisciplina 
							set 
								dtdcritico2 = $dtdcritico2,
								dtdsuficiente = $dtdsuficiente,
								dtdresolvidoescola = $dtdresolvidoescola 
							where 
								fk_cod_turma in (".$turma.")
							and
								fk_cod_disciplina = $disciplina
							and 
								dtdstatus = 'A'
							and 
								pdeid = '$pdeid';";
				$db->executar($sql);
				$db->commit();
				
			}
		}
	}
	
	salvarAbaResposta("diagnostico_7_sintese");

	
	echo "<script>
			alert('Dados gravados com sucesso. Vamos agora para o Plano Geral!');
			window.location='".$dados['togo']."';
		  </script>";
}

function verificaTaxaDimensao7($taxa,$arrTurmas,$tipo)
{
	global $db;
	
	$pdeid = $_SESSION['pdeinterativo2013_vars']['pdeid'];
	
	if($arrTurmas && $taxa && $tipo){
		switch($taxa){
			case "distorcao":
				$sql = "select distinct
							fk_cod_turma
						from 
							pdeinterativo2013.distorcaoaproveitamento 
						where
							$tipo = true
						and 
							fk_cod_turma in (".implode(",",$arrTurmas).") 
						and 
							diastatus = 'A'
						and
							diamarcado = 'D' 
						and 
							pdeid = '$pdeid';";
			break;
			
			case "reprovacao":
				$sql = "select distinct
							fk_cod_turma
						from 
							pdeinterativo2013.distorcaoaproveitamento 
						where
							$tipo = true 
						and 
							fk_cod_turma in (".implode(",",$arrTurmas).") 
						and 
							diastatus = 'A'
						and
							diamarcado = 'R' 
						and 
							pdeid = '$pdeid';";
			break;
			
			case "abandono":
				$sql = "select distinct
							fk_cod_turma
						from 
							pdeinterativo2013.distorcaoaproveitamento 
						where
							$tipo = true 
						and 
							fk_cod_turma in (".implode(",",$arrTurmas).") 
						and 
							diastatus = 'A'
						and
							diamarcado = 'A' 
						and 
							pdeid = '$pdeid';";
			break;
			
			default:
				return false;
			break;
	
		}
		
		$numTurmas = $db->carregarColuna($sql);
		if($numTurmas){
			if(count($numTurmas) == count($arrTurmas)){
				return "t";
			}else{
				return false;
			}
		}else{
			switch($taxa){
				case "distorcao":
					$sql = "select distinct
								fk_cod_turma
							from 
								pdeinterativo2013.distorcaoaproveitamento 
							where
								$tipo = false
							and 
								fk_cod_turma in (".implode(",",$arrTurmas).") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'D' 
							and 
								pdeid = '$pdeid';";
				break;
				
				case "reprovacao":
					$sql = "select distinct
								fk_cod_turma
							from 
								pdeinterativo2013.distorcaoaproveitamento 
							where
								$tipo = false 
							and 
								fk_cod_turma in (".implode(",",$arrTurmas).") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'R' 
							and 
								pdeid = '$pdeid';";
				break;
				
				case "abandono":
					$sql = "select distinct
								fk_cod_turma
							from 
								pdeinterativo2013.distorcaoaproveitamento 
							where
								$tipo = false 
							and 
								fk_cod_turma in (".implode(",",$arrTurmas).") 
							and 
								diastatus = 'A'
							and
								diamarcado = 'A' 
							and 
								pdeid = '$pdeid';";
				break;
				
				default:
					return false;
				break;
		
			}
			
			$numTurmas = $db->carregarColuna($sql);
			
			if($numTurmas){
				if(count($numTurmas) == count($arrTurmas)){
					return "f";
				}else{
					return false;
				}
			}
		}
	}else{
		return false;
	}
}

function verificaDisciplinaDimensao7($disciplina,$arrTurmas,$tipo)
{
global $db;
	
	$pdeid = $_SESSION['pdeinterativo2013_vars']['pdeid'];
	
	$sql = "	select
					distinct fk_cod_turma
				from 
						pdeinterativo2013.distorcaodisciplina 
					where 
						$tipo = true 
					and 
						fk_cod_disciplina = $disciplina
					and
						fk_cod_turma in (".implode(",",$arrTurmas).")
					and 
						dtdstatus = 'A'
					and 
						pdeid = '$pdeid' 
					and 
						dtdnumreprovado is not null;";

	$numTurmas = $db->carregarColuna($sql);
	
	if($numTurmas){
		if(count($numTurmas) == count($arrTurmas)){
			return "t";
		}else{
			return false;
		}
	}else{
		$sql = "	select
					distinct fk_cod_turma
				from 
						pdeinterativo2013.distorcaodisciplina 
					where 
						$tipo = false 
					and 
						fk_cod_disciplina = $disciplina
					and
						fk_cod_turma in (".implode(",",$arrTurmas).")
					and 
						dtdstatus = 'A'
					and 
						pdeid = '$pdeid' 
					and 
						dtdnumreprovado is not null;";

		$numTurmas = $db->carregarColuna($sql);
		
		if($numTurmas){
			if(count($numTurmas) == count($arrTurmas)){
				return "f";
			}else{
				return false;
			}
		}
	}
}

function exibeParecerDimensao($abaid)
{
	global $db;
	
	$sql = "select 
				esdid 
			from 
				pdeinterativo2013.pdinterativo pde
			left join
				workflow.documento est ON est.docid = pde.docid 
			where 
				pde.pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']} and pdistatus = 'A'";
	$docid = $db->pegaUm($sql);
	
	if(!$docid){
		return false;
	}
	
	$arrPerfil = pegaPerfilGeral();

	if(in_array(PDEINT_PERFIL_EQUIPE_MEC, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_PAR_ESTADUAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_PAR_MUNICIPAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_MUNICIPAL, $arrPerfil) || in_array(PDEINT_PERFIL_COMITE_ESTADUAL, $arrPerfil) || $db->testa_superuser() ){
		if($docid == WF_ESD_COMITE || $docid == WF_ESD_COMITE_SEMPDE){
			$permissao = true;
		}else{
			$permissao = false;
		}
	}else{
		$permissao = false;
	}

	$sql = "select
				abacod,
				prcaprovado,
				prcparecer as parecer
			from
				pdeinterativo2013.aba aba
			left join
				pdeinterativo2013.parecer par ON par.abaid = aba.abaid and par.pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}
			where
				aba.abaid = $abaid 
			order by
				prcdata desc
			limit
				1";
	$arrDados = $db->pegaLinha($sql);
	extract($arrDados);
	$arrNum = explode("_",$abacod);
	$num_dimensao = $arrNum[1];

	?>
	<tr>
		<td class="direita" >Parecer sobre a Dimensão <?php echo $num_dimensao ?></td>
		<td colspan="2">
			<input <?php echo $permissao ? "" : "disabled='disabled'" ?> type="radio" name="rdn_parecer[<?php echo $abaid ?>]" <?php echo $prcaprovado == "t" ? "checked='checked'" : ""  ?> value="t" /> A Dimensão <?php echo $num_dimensao ?> está aprovada. <br />
			<input <?php echo $permissao ? "" : "disabled='disabled'" ?> type="radio" name="rdn_parecer[<?php echo $abaid ?>]" <?php echo $prcaprovado == "f" ? "checked='checked'" : ""  ?> value="f" /> A Dimensão <?php echo $num_dimensao ?> precisa ser ajustada de acordo com o parecer abaixo.
			<table>
				<tr>
					<td><?php echo campo_textarea("parecer_$abaid","S",($permissao ? "S" : "N"),"",80,5,250,"","","","","",$parecer) ?></td>
					<td valign="middle" >
						<input <?php echo $permissao ? "" : "disabled='disabled'" ?> type="button" name="btn_s_parecer" onclick="salvarParecer('<?php echo $abaid ?>')" value="Salvar Parecer" />
						<input type="button" name="btn_ver_historico" onclick="historicoParecer('<?php echo $abaid ?>')" value="Visualizar Histórico" />
						<span id="span_parecer_<?php echo $abaid ?>" ></span>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<?php
	
}

function salvarParecer($abaid)
{
	global $db;
	
	extract($_POST);
	
	if(!$abaid){
		return false;
	}
	
	$usucpf = "'".$_SESSION['usucpf']."'";
	$parecer = "'".utf8_decode($parecer)."'";
	$rdn_parecer = $rdn_parecer == "f" ? "false" : "true";
	
	$parecer = $rdn_parecer == "false" ? $parecer : "null";
	
	$sql = "update
				pdeinterativo2013.parecer
			set
				prcstatus = 'H'
			where
				abaid = $abaid and pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
	$prcid = $db->pegaUm($sql);
	$db->commit();
	
	$sql = "insert into 
				pdeinterativo2013.parecer
			(prcaprovado,abaid,usucpf,prcstatus,prcparecer,pdeid)
				values
			($rdn_parecer,$abaid,$usucpf,'A',$parecer,{$_SESSION['pdeinterativo2013_vars']['pdeid']})";
	$db->executar($sql);
	
	$db->commit();
	echo "true";
	
}

function historicoParecer($abaid)
{
	global $db;
	
	monta_titulo( "Histórico do Parecer", '&nbsp' );
	
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	$abaid = $_GET['abaid'];
	
	$sql = "select
				to_char(prcdata,'DD/MM/YYYY') as data,
				(CASE WHEN prcaprovado is true
					THEN 'Sim'
					ELSE 'Não'
				END) as aprovado,
				usu.usunome,
				COALESCE(prcparecer,'N/A') as parecer
			from
				pdeinterativo2013.aba aba
			inner join
				pdeinterativo2013.parecer par ON par.abaid = aba.abaid
			inner join
				seguranca.usuario usu ON usu.usucpf = par.usucpf
			where
				aba.abaid = $abaid 
				and
				par.pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}
			order by
				prcdata desc";
	
	$arrCabecalho = array("Data","Dimensão Aprovada?","Quem Fez","Parecer");
	$db->monta_lista($sql,$arrCabecalho,100,10,"N","center","N");
	
}
?>