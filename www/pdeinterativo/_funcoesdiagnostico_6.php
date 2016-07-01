<?php
function recuperaInstalacoesFisicas()
{
	global $db;
	$sql = "
			select 
				* 
			from 
				pdeinterativo.infrainstalacaofisica 
			where 
				ifistatus = 'A' 
			order 
				by ifidesc";
	return $db->carregar($sql);
}

function salvarDistorcaoInfraestruturaInstalacao()
{
	global $db;
	
	extract($_POST);
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	$sql = "update pdeinterativo.respostainfrainstalacaofisica  set rifstatus = 'I' where pdeid = $pdeid;";
	$db->executar($sql);
	$db->commit();
	
	
	if($num_adequado){
		foreach($num_adequado as $ifiid => $num){
			if($num != "" || $num_inadequado[$ifiid] != ""){
				
				$num = $num != "" ? str_replace(".","",$num) : "null";
				$num_inadequado[$ifiid] = $num_inadequado[$ifiid] != "" ? str_replace(".","",$num_inadequado[$ifiid]) : "null";
				$justificativa[$ifiid] = $justificativa[$ifiid] ? "'".$justificativa[$ifiid]."'" : "null";
				$total = ($num ? (int)$num : 0) + ($num_inadequado[$ifiid] ? (int)$num_inadequado[$ifiid] : 0);
				
				$sql= "insert into 
							pdeinterativo.respostainfrainstalacaofisica 
						(ifiid,rifqtdadequado,rifqtdinadequado,rifqtdtotal,rifporque,rifstatus,pdeid)
							values 
						($ifiid,$num,".$num_inadequado[$ifiid].",".$total.",".$justificativa[$ifiid].",'A',$pdeid);";
				$db->executar($sql);
				$db->commit();
				
			}
		}
	}
	
	salvarRespostasPorEscola();
	salvarAbaResposta("diagnostico_6_1_instalacoes");
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_2_equipamentos");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_1_instalacoes");
	}
	
	
	exit;
	
}

function recuperaInstalacoesFisicasPorEscola($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select * from pdeinterativo.respostainfrainstalacaofisica where pdeid = $pdeid and rifstatus = 'A'";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			$arrD[$dado['ifiid']] = array( 
											"adequado" => $dado['rifqtdadequado'],
											"inadequado" => $dado['rifqtdinadequado'],
											"total" => $dado['rifqtdtotal'],
											"justificativa" => $dado['rifporque']
										 );	
		}
		return $arrD;
	}else{
		return array();
	}
	
}

function existeEspacos($dados) {
	global $db;
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	$sql = "SELECT 
				count(*)
			FROM 
				pdeinterativo.respostainstalacaoespaco resp 
			WHERE
				resp.pdeid = $pdeid
			AND
				resp.riestatus = 'A'
			LIMIT
				1";
	
	$rieid = $db->pegaUm($sql);
	
	return (($rieid)?TRUE:FALSE);
}

function carregarEspacos($dados) {
	global $db;
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	echo "<p>Clique no botão abaixo para inserir informações sobre o(s) espaço(s).</p>";
	
	$sql = "SELECT 
				'<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"gerenciarEspacos(\'' || rieid || '\');\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirEspaco(\'' || rieid || '\');\"></center>' as acoes, 
				tipo.tiedesc,
				CASE WHEN rienatureza = 'P'
					 THEN 'Pública'
					 ELSE 'Privada'
				END as natureza,
				freq.tfrdesc
			FROM 
				pdeinterativo.respostainstalacaoespaco resp 
			INNER JOIN 
				pdeinterativo.tipoinstalacaoespaco tipo ON tipo.tieid = resp.tieid
			INNER JOIN
				pdeinterativo.tipofrequencia freq ON freq.tfrid = resp.tfrid
			WHERE
				resp.pdeid = $pdeid
			AND
				tipo.tiestatus = 'A'
			AND
				resp.riestatus = 'A'
			AND
				tfrstatus = 'A'";
	
	$cabecalho = array("&nbsp;","Tipo de Espaço","Natureza","Frequência de Uso");
	
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
	echo "<p align=\"right\"><input type=\"button\" value=\"Incluir Espaço\" onclick=\"gerenciarEspacos();\"></p>";
}



function gerenciarEspacos($dados) {
	global $db;

	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo "<script>";
	
	$sqlTipoEspaco = "select tieid as codigo, tiedesc as descricao from pdeinterativo.tipoinstalacaoespaco where tiestatus = 'A' order by tiedesc";
	
	$arrNatureza = array( 0 => array( "codigo" => "P" , "descricao" => "Pública" ),
						  1 => array( "codigo" => "R" , "descricao" => "Privada" )  
					);
	$sql = "select * from pdeinterativo.tipofrequencia where tfrstatus = 'A' order by tfrdesc";
	$arrFrequecia = $db->carregar($sql);
	
	echo "function validarEspaco(){
					if(document.getElementById('tieid').value=='') {
						alert('Selecione o tipo de espaço!');
						return false;
					}
					if(document.getElementById('rienatureza').value=='') {
						alert('Selecione a natureza!');
						return false;
					}
					if( jQuery('[name=tfrid]:checked').length <= 0 ) {
						alert('Selecione a frequência!');
						return false;
					}
					document.getElementById('form_espaco').submit()
				  }";
	
	echo "</script>";
	
	echo "<form method=post id=form_espaco>";
	
	if($dados['rieid']) {
		echo "<input type=hidden name=requisicao value=atualizarEspaco>";
		$arrDadosEspaco = $db->pegaLinha("SELECT * from pdeinterativo.respostainstalacaoespaco
										 WHERE rieid ='".$dados['rieid']."'");
		extract($arrDadosEspaco);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirEspaco>";
	}
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir Espaço</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Tipo de Espaço:</td>";
	echo "<td>".$db->monta_combo('tieid', $sqlTipoEspaco, 'S', 'Selecione', '', '', '', '200', 'S', 'tieid', true, $tieid)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Natureza:</td>";
	echo "<td>".$db->monta_combo('rienatureza', $arrNatureza, 'S', 'Selecione', '', '', '', '200', 'S', 'rienatureza', true, $rienatureza)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Frequência de uso:</td>";
	echo "<td>";
	if($arrFrequecia){
		$n=0;
		foreach($arrFrequecia as $freq){
			echo "<div style=\"width:60px;float:left;white-space: nowrap;margin-left:".($n == 0 ? "0px" : "20px" )."\" ><input type=\"radio\" ".($freq['tfrid'] == $tfrid ? "checked='checked'" : "")." name=\"tfrid\" value=\"{$freq['tfrid']}\" /> ".$freq['tfrdesc']."</div> ";
			if($n == 2){
				echo "<br />";
				$n = 0;
			}else{
				$n++;
			}
		}
	}
	echo "</td></tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=gravar value=Gravar onclick=validarEspaco();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function inserirEspaco($dados) {
	global $db;
	
	$sql = "INSERT INTO pdeinterativo.respostainstalacaoespaco(
            pdeid, tieid, tfrid, rienatureza, riestatus)
    		VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$dados['tieid']."', '".$dados['tfrid']."', '".$dados['rienatureza']."', 'A');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Espaço inserido com sucesso');
			window.opener.carregarEspacos();
			window.close();
		  </script>";
}

function atualizarEspaco($dados) {
	global $db;
	$sql = "UPDATE pdeinterativo.respostainstalacaoespaco
   			SET tieid='".$dados['tieid']."', tfrid='".$dados['tfrid']."', rienatureza='".$dados['rienatureza']."'
 			WHERE rieid='".$dados['rieid']."';";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Espaço atualizado com sucesso!');
			window.opener.carregarEspacos();
			window.close();
		  </script>";
}

function excluirEspaco($dados)
{
	global $db;
	
	$sql = "UPDATE pdeinterativo.respostainstalacaoespaco SET riestatus='I' WHERE rieid='".$dados['rieid']."'";
	$db->executar($sql);
	$db->commit();

	echo "Espaço removido com sucesso!";
}

function excluirTodosEspaco()
{
	global $db;
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	$sql = "UPDATE pdeinterativo.respostainstalacaoespaco SET riestatus='I' WHERE pdeid = $pdeid";
	$db->executar($sql);
	$db->commit();
}

function recuperaCategoriaMateriaisEquipamentos()
{
	global $db;
	$sql = "select * from pdeinterativo.categoriamaterialequipamento where cmestatus = 'A'";
	return $db->carregar($sql);
}

function verificaExisteEspaco()
{
	global $db;
	$sql = "select * from pdeinterativo.respostainstalacaoespaco where pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']} and riestatus = 'A'";
	$existe = $db->pegaUm($sql);
	if($existe){
		echo "exite";	
	}
}

function salvarRespostaEspaco()
{
	global $db;
	extract($_POST);
	$sql = "select rppresposta from pdeinterativo.respostaprojetoprograma where rpptipo = 'E' and pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']} and rppstatus = 'A' and rppmodulo = 'F';";
	$rppresposta = $db->pegaUm($sql);
	if($rppresposta && $rppresposta != $resposta){
		 $db->executar("update pdeinterativo.respostaprojetoprograma set rppresposta = $resposta where rpptipo = 'E' and pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']} and rppstatus = 'A' and rppmodulo = 'F'");
	}else{
		$db->executar("insert into pdeinterativo.respostaprojetoprograma (rppresposta,rppmodulo,pdeid,rpptipo,rppstatus) values ($resposta,'F',{$_SESSION['pdeinterativo_vars']['pdeid']},'E','A')");
	}
	$db->commit();
}

function recuperaMateriaisEquipamentos($cmeid)
{
	global $db;
	$sql = "select 
				* 
			from 
				pdeinterativo.categtipomatequip cat
			inner join
				pdeinterativo.tipomaterialequipamento mat ON mat.tmeid = cat.tmeid 
			where 
				cat.cmeid = $cmeid 
			and 
				tmestatus = 'A'";
	return $db->carregar($sql);
}

function salvarDistorcaoInfraestruturaEquipamentos()
{
	global $db;
	
	extract($_POST);
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	$sql = "update pdeinterativo.respostamaterialequipamento set rmestatus = 'I' where pdeid = $pdeid;";
	$db->executar($sql);
	$db->commit();
	
	if($num_bom){
		foreach($num_bom as $tmeid => $bom){
			if(trim($bom) != "" || trim($num_ruim[$rmeid]) != ""){
				
				$bom = trim($bom) != "" ? str_replace(".","",$bom) : "null";
				$ruim = trim($num_ruim[$tmeid]) != "" ? str_replace(".","",$num_ruim[$tmeid]) : "null";
				$num_necessaria = $necessaria[$tmeid] != "" ? str_replace(".","",$necessaria[$tmeid]) : "null";
				$atende = $num_necessaria && $num_necessaria != "null" ? "false" : "true";
				$total = ($bom ? (int)$bom : 0) + ($ruim ? (int)$ruim : 0);
				
				$sql= "insert into 
							pdeinterativo.respostamaterialequipamento
						(pdeid,tmeid,rmeqtdbom,rmeqtdruin,rmeqtdtotal,rmeatende,rmeqtdideal,rmestatus)
							values 
						($pdeid,$tmeid,$bom,$ruim,$total,$atende,$num_necessaria,'A');";
				$db->executar($sql);
				$db->commit();
				
			}
		}
	}
	
	salvarRespostasPorEscola();
	salvarAbaResposta("diagnostico_6_2_equipamentos");
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_2_equipamentos");
	}
	
	exit;
}

function recuperaInstalacoesEquipamentos($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select * from pdeinterativo.respostamaterialequipamento where pdeid = $pdeid and rmestatus = 'A'";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			$arrD[$dado['tmeid']] = array( 
											"bom" => $dado['rmeqtdbom'],
											"ruim" => $dado['rmeqtdruin'],
											"total" => $dado['rmeqtdtotal'],
											"atende" => ($dado['rmeatende'] == "f" ? "N" : "S"),
											"necessita" => $dado['rmeqtdideal']
										 );	
		}
		return $arrD;
	}else{
		return array();
	}
}

function recuperaInstalacoesInadequadas($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select distinct
				*,
				infra.ifiid
			from 
				pdeinterativo.infrainstalacaofisica infra 
			left join
				pdeinterativo.respostainfrainstalacaofisica resp ON infra.ifiid = resp.ifiid and pdeid = $pdeid and ifistatus = 'A'
			where 
				rifstatus = 'A'
			and
				rifqtdinadequado != 0
			and
				rifqtdinadequado is not null 
			order by
				ifidesc";
	
	return $db->carregar($sql);
	
}

function recuperaInstalacoesNecessarias($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select distinct
				resp.rifqtdadequado,
				resp.rifqtdinadequado,
				resp.rifqtdtotal,
				resp.rifporque,
				resp.rifstatus,
				resp.rifcritico,
				resp.rifcritico2,
				resp.rifsuficiente,
				resp.rifresolvidoescola,
				infra.ifiid,
				infra.ifidesc,
				infra.ifidevepossuir
			from 
				pdeinterativo.infrainstalacaofisica infra 
			left join
				pdeinterativo.respostainfrainstalacaofisica resp ON infra.ifiid = resp.ifiid and pdeid = $pdeid and ifistatus = 'A'
			where 
				rifstatus = 'A'
			and
				ifidevepossuir = true
			and
				(rifqtdadequado = 0 
			or 
				rifqtdadequado is null)
			and 
				infra.ifiid in (5,8,13,19,25,26,27)
			and
				(rifqtdinadequado = 0
			or
				rifqtdinadequado is null)
			order by
				ifidesc";
	
	return $db->carregar($sql);
	
}


function verificaCheckBoxInstalacao($ifiid)
{
	
}

function recuperaEquipamentosRuins($pdeid = null)
{
	global $db;
	
	$pdeid = !$pdeid ? $_SESSION['pdeinterativo_vars']['pdeid'] : $pdeid;
	
	$sql = "select 
				*
			from 
				pdeinterativo.respostamaterialequipamento resp
			inner join
				pdeinterativo.tipomaterialequipamento equip ON equip.tmeid = resp.tmeid
			where 
				rmestatus = 'A'
			and
				pdeid = $pdeid
			and
				rmeqtdruin is not null
			and
				rmeqtdruin != 0
			and
				tmestatus = 'A'
			order by
				tmedesc";
	
	return $db->carregar($sql);
}

function verificaCheckBoxEquipamento($tmeid)
{
	
}

function salvarSinteseInfraestrutura()
{
	global $db;
	
	extract($_POST);
	
	$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
	
	//Opções das perguntas indicadas como Raramente ou Nunca
	if($arrRepid){
		$sql = "update pdeinterativo.respostapergunta set critico = false where repid in (".implode(",",$arrRepid).");";
		$db->executar($sql);
		$db->commit();
	}
	if($chk_problemas['opcao']){
		foreach($chk_problemas['opcao'] as $repid => $valor){
			$sql= "update pdeinterativo.respostapergunta set critico = true where repid = $repid;";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	//Instalações
	$sql="update pdeinterativo.respostainfrainstalacaofisica set rifcritico = false where pdeid = '$pdeid' and rifstatus = 'A';";
	$db->executar($sql);
	$db->commit();
	
	if($chk_problemas['instalacao']){
		foreach($chk_problemas['instalacao'] as $ifiid => $valor){
			
			$sqlC = "select 
						count(*) 
					from 
						pdeinterativo.respostainfrainstalacaofisica 
					where 
						ifiid = $ifiid
					and 
						pdeid = '$pdeid' 
					and 
						rifstatus = 'A'";
			if(!$db->pegaUm($sqlC)){
				$sql= "insert into 
							pdeinterativo.respostainfrainstalacaofisica
						(ifiid,rifqtdadequado,rifqtdinadequado,rifqtdtotal,rifporque,rifstatus,pdeid,rifcritico)
							values
						($ifiid,0,0,0,null,'A',$pdeid,true);";
				$db->executar($sql);
				$db->commit();
				
			}else{
				$sql= "update 
						pdeinterativo.respostainfrainstalacaofisica
					set 
						rifcritico = true 
					where 
						ifiid = $ifiid
					and 
						pdeid = '$pdeid' 
					and 
						rifstatus = 'A';";	
				$db->executar($sql);
				$db->commit();
			}
			
		}
	}
	
	//Equipamentos
	$sql="update pdeinterativo.respostamaterialequipamento set remcritico = false where pdeid = '$pdeid' and rmestatus = 'A';";
	$db->executar($sql);
	$db->commit();
	
	if($chk_problemas['equipamento']){
		foreach($chk_problemas['equipamento'] as $tmeid => $valor){
			$sql= "update 
						pdeinterativo.respostamaterialequipamento
					set 
						remcritico = true 
					where 
						tmeid = $tmeid
					and 
						pdeid = '$pdeid' 
					and 
						rmestatus = 'A';";
			
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	salvarAbaResposta("diagnostico_6_3_sintese");
	
	if($hdn_redirect == "C"){
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese");
	}else{
		header("Location: pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese");
	}

	exit;
	
}