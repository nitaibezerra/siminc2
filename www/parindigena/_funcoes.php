<?php
/********************* FUNÇÕES DO MÓDULO *******************************/
/*
 * Pega array com perfis
 */
function arrayPerfil(){
	global $db;
	
	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 32
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
				$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

/*
 * Montar árvore
 */
function montarArvore(){
	global $db;
	
	# Pegar perfis do usuário
	$perfil = arrayPerfil();
	
	/*
	 * Valida pelos tipos de perfis
	 * se true trará os estados associados aos perfis em usuarioresponsabilidade
	 */
	if ( !in_array(PARIND_SUPER_USUARIO,$perfil) && !in_array(PARIND_ADMINISTRADOR,$perfil) && !in_array(PARIND_COORDENACAO,$perfil) ){
			$estadosUsuario = $db->carregarColuna("SELECT
													estuf 
												   FROM
													parindigena.usuarioresponsabilidade
												   WHERE
													rpustatus = 'A' AND
													pflcod IN (".implode(',',$perfil).") AND
													usucpf = '".$_SESSION["usucpf"]."'",
													"estuf");
			/*
			 * Monta where,
			 * filtro por estado
			 */
			$wh = "WHERE estuf IN ('".implode("','",$estadosUsuario)."')";		
	}
		
	/*
	$estadosUsuario = $db->carregar("SELECT
									  estuf 
									 FROM
									  parindigena.usuarioresponsabilidade
									 WHERE
									  usucpf = '".$_SESSION["usucpf"]."'");
	$wh = "";
	if($estadosUsuario) {
		for($i=0; $i < count($estadosUsuario); $i++) {
			if($i==0) {
				$es = "'".$estadosUsuario[$i]["estuf"]."'";
			} else {
				$es .= ",'".$estadosUsuario[$i]["estuf"]."'";
			}
		
			if($i == (count($estadosUsuario)-1)) {
				$wh .= "WHERE estuf in (".$es.")";
			}
		}
	}
	*/
	echo "<table id=\"tabela\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellpadding=\"3\" align=\"center\">";	
	
	$estados = $db->carregar("SELECT 
							   estdescricao,
							   estuf 
							  FROM
							   territorios.estado 
							  ".$wh." 
							  ORDER BY
							   estdescricao");
	/*
	 * Se não tiver acesso a nenhum estado
	 */
	if (!$estados){
		echo "	  <tr style=\"background-color:".$cor.";\" id=\"".$id."\">
					<td align='center' style='color:red;' colspan=6>
						Nenhuma Unidade de Federação associada ao usuário. Entre em contato com o administrador do sistema.
					</td>
				  </tr>
				</tbody>
			  </table>";		
		return;
	}
	
	/*
	 * Monta lista de estados
	 */
	echo "<thead>
			<tr style=\"background-color: #e0e0e0\">
				<td style=\"font-weight:bold; text-align:center; width:5%;\">Ação</td>			
				<td style=\"font-weight:bold; text-align:center; width:65%;\">Título</td>
				<td style=\"font-weight:bold; text-align:center; width:10%;\">Situação</td>
				<td style=\"font-weight:bold; text-align:center; width:10%;\">Início</td>
				<td style=\"font-weight:bold; text-align:center; width:10%;\">Término</td>			
			</tr>
		  </thead>
		  <tbody>";
			
	for($i=0; $i < count($estados); $i++) {
		if(fmod($i, 2) == 0)		
			$cor = '#fafafa';
		else		
			$cor = '#f0f0f0';
		
		$id = $estados[$i]["estuf"];
		
		echo "<tr style=\"background-color:".$cor.";\" id=\"".$id."\">
				<td>&nbsp;</td>
				<td>
					<a href=\"javascript:void(0);\" onclick=\"alteraIcone('".$id."', 1);\">
					<img id=\"img_".$id."\" src=\"../imagens/mais.gif\" border=\"0\">				
					</a>&nbsp;&nbsp;<b>".$estados[$i]["estdescricao"]."</b>				
				</td>
				<td>&nbsp;</td>
				<td>&nbsp;</td>
				<td>&nbsp;</td>
			  </tr>";
	}
	echo "</tbody>
		  </table>";
}


/********************* FIM FUNÇÕES DO MÓDULO ***************************/

// -------------------- FUNÇÕES TEMPORÁRIAS DO OBRAS --------------------

/**
 * Função que cadastra as obras
 * 
 * @author Fernando Araújo Bagno da Silva
 * @param array $dados
 *
 */
function obras_cadastra_obras($dados){

	global $db;
		
	// Insere os dados na tabela endereco
	$sql = sprintf("
					INSERT INTO 
						parindigena.itemmonitoramento (itmstatus, usucpf, covid, grpid, estuf, obrid)
					VALUES 
						('A', '%s', '%s', 4, '%s', %d)",
					$_SESSION['usucpf'],
					$_REQUEST['covid'],
					$_REQUEST['estuf'],
					$_REQUEST['obrid']
					);
					
	$db->executar($sql);
	
	//$itmid = $db->pegaUm($sql);
	
	//INSERINDO POVOS INDÍGENAS
	$povid = $_REQUEST['povid'];	
	foreach ($povid as $item){
		$sql = sprintf("
						INSERT INTO 
							parindigena.povoobrainfraestrutura (obrid, povid)
						VALUES 
							(%d, %d)",
						$_REQUEST['obrid'],
						$item
						);
		$db->executar($sql);
		
	}
	
	
	//INSERINDO LINGUAS
	$linid = $_REQUEST['linid'];	
	foreach ($linid as $item){
		$sql = sprintf("
						INSERT INTO 
							parindigena.linguaobrainfraestrutura (obrid, linid)
						VALUES 
							(%d, %d)",
						$_REQUEST['obrid'],
						$item
						);
		$db->executar($sql);
	
	}
	
	//INSERINDO TERRITÓRIOS
	$teoid = $_REQUEST['teoid'];	
	foreach ($teoid as $item){
		$sql = sprintf("
						INSERT INTO 
							parindigena.territorioobrainfraestrutura (obrid, terid)
						VALUES 
							(%d, %d)",
						$_REQUEST['obrid'],
						$item
						);
		
		$db->executar($sql);
	
	}
		
	$db->commit();
	
	echo '
		<script>
			alert("Operação realizada com sucesso!");
			window.opener.location.href = window.opener.location.href; 
			self.close();
		</script>';
	
}

/**
 * Função que atualiza a obra
 *
 * @author Fernando Araújo Bagno da Silva
 * @param array $dados
 * 
 */
function obras_atualiza_obras($dados){
	
	global $db;
	
	$sql = sprintf("
					UPDATE
						parindigena.itemmonitoramento
						SET covcod = '%s',
						obrid = %d
					WHERE
						itmid = %d",
					$_REQUEST['covcod'],
					$_REQUEST['obrid'],
					$_REQUEST['itmid']
					);
					
	$db->executar($sql);
	
	//APAGANDO ASSOCIATIVAS****************************************************************************
	$sql = sprintf("DELETE 
    					FROM parindigena.territorioobrainfraestrutura 
    				WHERE 
    					obrid = %d",
    				$_REQUEST['obrid']);
    $db->executar($sql);
    
    $sql = sprintf("DELETE 
    					FROM parindigena.povoobrainfraestrutura
    				WHERE 
    					obrid = %d",
    				$_REQUEST['obrid']);
    $db->executar($sql);
    
    $sql = sprintf("DELETE 
    					FROM parindigena.linguaobrainfraestrutura
    				WHERE 
    					obrid = %d",
    				$_REQUEST['obrid']);
    $db->executar($sql);    
	//**************************************************************************************************
	
	//INSERINDO POVOS INDÍGENAS
	$povid = $_REQUEST['povid'];	
	foreach ($povid as $item){
		$sql = sprintf("
						INSERT INTO 
							parindigena.povoobrainfraestrutura (obrid, povid)
						VALUES 
							(%d, %d)",
						$_REQUEST['obrid'],
						$item
						);
		$db->executar($sql);
		//echo $sql."<BR>";
	}
	
	
	//INSERINDO LINGUAS
	$linid = $_REQUEST['linid'];	
	foreach ($linid as $item){
		$sql = sprintf("
						INSERT INTO 
							parindigena.linguaobrainfraestrutura (obrid, linid)
						VALUES 
							(%d, %d)",
						$_REQUEST['obrid'],
						$item
						);
		$db->executar($sql);
		//echo $sql."<BR>";
	}
	
	//INSERINDO TERRITÓRIOS
	$teoid = $_REQUEST['teoid'];	
	foreach ($teoid as $item){
		$sql = sprintf("
						INSERT INTO 
							parindigena.territorioobrainfraestrutura (obrid, terid)
						VALUES 
							(%d, %d)",
						$_REQUEST['obrid'],
						$item
						);
		$db->executar($sql);
		//echo $sql."<BR>";
	}	
	
	$db->commit();
	
	echo '
		<script>
			alert("Operação realizada com sucesso!");
			window.opener.location.href = window.opener.location.href; 
			self.close();
		</script>';
}

/**
 * Função que deleta uma obra
 *
 * @author Fernando Araújo Bagno da Silva
 * @param integer $obrid
 * 
 */
function obras_deleta_obras($obrid){
	
	global $db;
	$db->executar("
			UPDATE 
				obras.obrainfraestrutura 
			SET 
				obsstatus = 'I' WHERE obrid = " . $obrid);
	
	$db->commit();
	$_REQUEST["acao"] = "A";
	$db->sucesso("inicio");
	
}

/**
 * Função que busca os dados da obra
 *
 * @author Fernando Araújo Bagno da Silva
 * @param string $obrid
 * @return array
 * 
 */
function obras_busca_obras($obrid){
	
	global $db;
	
	$dados = $db->pegaLinha("
						SELECT 
							* 
						FROM 
							obras.obrainfraestrutura ob 
						WHERE 
							ob.obrid = {$obrid}");
	
	return $dados;
			
}

function verificaEtapa( $friid ){
	global $db;
	
	$sql = "select fristatus from parindigena.formacaoinicial where friid = $friid";
	$status = $db->pegaUm($sql);
	if( $status == 'P'){
		$msgSemEtapa = "<img src=\"../imagens/atencao.png\"><a style=\"color: red;\"> Ainda não foi cadastrada nenhuma etapa para o curso. Para completar o cadastro, informe as etapas na aba \"Etapas\".</a>";
	}else{
		$msgSemEtapa = ""; 
	}
	return $msgSemEtapa;
}

function calculoValorProgramado($sbaid, $ano, $escola){
	
	global $db;
	
	if($escola == 't'){ // SQL quando a subação for por escola
			$select = "sum(cos.cosvlruni * ecs.ecsqtd) AS cronograma,";
			
			$inner = "INNER JOIN cte.escolacomposicaosubacao ecs ON cos.cosid = ecs.cosid";
			
		}else{ // SQL quando a subação for global
			$select = "sum(cos.cosqtd * cos.cosvlruni ) AS cronograma,";
			
			$inner = "INNER JOIN cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano >= '{$ano}'";
		}
		
		$sql=	"select 
					{$select}	
					'{$ano}' as ano
					from 
						cte.subacaoindicador sba
					INNER JOIN
						cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano >= '{$ano}'
					{$inner}
					where sba.sbaid = {$sbaid}
					group by sba.sbaid";
					
		return $db->pegaLinha($sql);
}

?>